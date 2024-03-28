#include <iostream>
#include <thread>
#include <string>
#include <filesystem>
#include <utility>
#include <fstream>
#include <cstdlib>
#include <unistd.h>
#include <array>
#include <functional>
#include <condition_variable>
#include <mutex>
#include <vector>
#if defined(__linux__)
#include <pthread.h>
#endif
#if defined(__ARM_ARCH) || defined(__aarch64__) || defined(_M_ARM) || defined(_M_ARM64) || defined(__APPLE__) || defined(__MACH__)
#include <mach/mach_host.h>
#include <pthread.h>
#endif
#include <memory>
#include <algorithm>
#include <stack>
#include <queue>
#include <unordered_map>
#ifdef _WIN32
#include <windows.h>
#endif
#define RED "\x1B[31m"
#define GREEN "\x1B[32m"
#define YELLOW "\x1B[33m"
#define CYAN "\x1B[36m"
#define RESET "\033[0m"

class ThreadPool;

class Maze {        // Class for the maze
public:
    typedef struct Cell {   // Defining the structure for a cell
        __SIZE_TYPE__ x;    // Coordinates
        __SIZE_TYPE__ y;
        char type;          // Type (0, 1, 2, else)

        struct Cell * parent = nullptr; // Pointer to a parent for backtracking

        Cell(__SIZE_TYPE__ x, __SIZE_TYPE__ y, char type) { // Constructor for the cell
            this->x = x;
            this->y = y;
            this->type = type;
        }

        bool visited = false;   // Bools used for defining the sub-type of the cell
        bool valid_cell = false; // A cell that is a part of a path
        bool shortest = false; // A cell that is a part of a shortest path
    } Cell;

private:
    void explore(size_t max_threads);         // Exploration function wrapper
    void explore_cell(Maze::Cell * cell, size_t step); // The place where the magic begins

    std::unordered_map<Maze::Cell *, std::vector<Cell *>> adjacency_list;       //An address of a cell is a key pointing to the vector of addresses of each cells (helps to avoid copies, efficient memory management)
    std::vector<std::vector<Maze::Cell>> map;                                   // The 'map' where the cells are stored
    std::vector<std::pair<size_t, Maze::Cell *>> ends;                          // Vector of pairs of a lenght of a path and the ending point (needed for drawing out them later)

    size_t sx, sy;      // These are the coordinates of the startin cell. A smart poiner was used once to hold instead, but this piece of shit
    // was pointing to nullptr for some shitty reason that I couldn't define which made me to re-write the structure of the map from dynamic cells to static ones
    // but it worked with regular pointers but I got too lazy to restructure everything again, so did let it all be as it is now, moreover this version works very well.

    std::atomic<unsigned> nends = 0; // Atomic type allows to use the variable by multiple variables without any problems. nends - number of ends in the maze
    Cell * shortest = nullptr;          // A pointer to the shortest cell

public:
    explicit Maze(std::ifstream & file);      // definition of explicit constructor of the class
    void search() noexcept;                   // definition of the search function
    inline void print(void) const noexcept;     // definition of inline print function
    inline void make_adjacent(void) noexcept;

    ThreadPool * pool = nullptr;
    std::mutex mtx1, mtx2, mtx3, mtx4;
};


class ThreadPool {      // Definition of the custom thread pool for the maze search
public:
    explicit ThreadPool(size_t threads, unsigned ends) {       // Explicit constructor that accepts max number of threads allowed to create.
        this->ends = ends;
        this->max_threads = threads;
    };

    ~ThreadPool();                      // Destructor

    void enqueue(const std::function<void()>& task);        // Defintion of the function that enques tasks for the threads
    void start_worker();                                    // Defintion of the function that starts a thread with a task
    void wait_until_done();                                 // Defintion of the function where the main thread waits untill all of the job is done.
    std::atomic<unsigned> n_threads = 0;                    // Atomic variable to control the amount of threads created.
    std::atomic<unsigned> ends = 0;

private:
    std::vector<std::thread> workers;                       // Defintion of the vector of the threads to store so I could join one by one later.
    std::queue<std::function<void()>> tasks;                // Queue of tasks to do

    std::mutex wait_mutex;                                  // Mutexes to prevent race conditions
    std::mutex work_mutex;
    std::mutex enqueue_mutex;

    size_t max_threads;                                     // Basically a constant that limits the amount of threads to create

    std::condition_variable tasks_done;                    // cond. variable used for signalling the main thread about the end of the job

};


ThreadPool::~ThreadPool() {                                     // Destruction of the class objects and the threads
    for (std::thread & worker : this->workers) {
        if (worker.joinable()) {
            worker.join();
        }
    }
}


void ThreadPool::wait_until_done() {                        // Definition of function where the main thread stucks
    std::unique_lock<std::mutex> lock (this->wait_mutex);
    this->tasks_done.wait(lock, [this]{return !this->n_threads;});      // Wait untill there are no threads running anymore.
}


void ThreadPool::start_worker() {                                       // The function where we emplace the newly created thread with assigned anonymous function
    this->workers.emplace_back([this] {
        std::function<void()> task;                                     // Static object of function
        {
            std::lock_guard<std::mutex> lock (this->work_mutex);        // Lock guard because accessing shared memory

            task = std::move(this->tasks.front());          // Assign an anonymous function to the task
            this->tasks.pop();
        }

        task();             // The thread proceeds to execute the assigned anonymous function.

        this->n_threads--;      // After completion we decrement the active threads

        if (!this->n_threads) {
            this->tasks_done.notify_all();      // Check if the threads are done, then we ask the main thread to stop waiting and proceed
        }
        return;             // The thread finsihes here.
    });
}


void ThreadPool::enqueue(const std::function<void()> & task) {      // Function to enque the task and call a worker
    this->enqueue_mutex.lock();                             // Mutex is locked to prevent race condition
    if (this->n_threads < this->max_threads && this->ends) {// Check if we are not exceeding max allowed threads
        this->tasks.emplace(task);                          // Emplacing a function into the queue
        this->n_threads++;                                  // Incrementing the threads used
        this->start_worker();                               // Calling a thread to execute the function
    }
    this->enqueue_mutex.unlock();
}


void Maze::explore_cell(Maze::Cell * cell, size_t step) {    // Piece of sh... good work
    Cell * cell__ = cell;

    for (Cell * neighbor : this->adjacency_list[cell__]) {      // A thread will check the adjacent cells, basically the recursive DFS.
        std::lock_guard<std::mutex> lock (this->mtx2);
        if (!neighbor->visited) {
            neighbor->visited = true;
            neighbor->parent = cell__;

            if (neighbor->type == 'F') {
                this->pool->ends--;
                this->ends.emplace_back(step, neighbor);
            }

            Cell * s_cell = neighbor;
            auto explore_task = [this, s_cell, step]() {this->explore_cell(s_cell, step + 1);};     // We create a lambda function as an entity and pass it to the pool
            this->pool->enqueue(explore_task);
        }
    }
}


Maze::Maze(std::ifstream & file) {  // Class constructor
    bool start_exists = false;
    bool finish_exists = false;
    std::string line;

    size_t y = 0;
    size_t x = 0;

    while (std::getline(file, line)) {  // Retrieving the maze line by line
        x = 0;
        std::vector<Maze::Cell> vec;

        for (const char & symbol : line) {  // Retrieving the cell cell by line
            switch (symbol) {
                case '0':   // 0 means - path
                    vec.emplace_back(Maze::Cell(x, y, ' '));
                    break;

                case '1':   // 1 - means start
                    if (!start_exists) {    // Making sure that there exists only one starting point
                        vec.emplace_back(Maze::Cell(x, y,'S'));
                        start_exists = true;
                        this->sx = x;
                        this->sy = y;
                    } else {
                        throw std::runtime_error("Start point already exists.");
                    }

                    break;

                case '2':   // 2 - means finish
                    vec.emplace_back(Maze::Cell(x, y, 'F'));
                    finish_exists = true;
                    this->nends++;
                    break;

                default:    // Everything else - a wall
                    vec.emplace_back(Maze::Cell(x, y, '#'));
                    break;
            }
            x++;
        }

        this->map.emplace_back(std::move(vec)); // Moving allows to avoid deep copies which would add complexity to the program. Basically returning without copying.
        // I have no idea of how it works very-very deep inside but it is very fun. It made a Python user to rage at me once :D
        y++;
    }

    if (!finish_exists) {   // I guess it is self explanatory
        throw std::runtime_error("Exit point doesn't exist.");
    }

    if (!start_exists) {    // Same thing
        throw std::runtime_error("Start point doesn't exist.");
    }

    this->make_adjacent();
}


void Maze::explore(const size_t max_threads) {
    this->pool = new ThreadPool(max_threads, this->nends);       // Initialize the thread pool with the max number of threads allowed

    this->map[this->sy][this->sx].visited = true;       // By default, the starting point is the visited one.
    Cell * start_cell = &this->map[this->sy][this->sx];

    auto explore_task = [this, start_cell]() {      // Creating a lambda function and pass to the pool
        this->explore_cell(start_cell, 1);
    };

    this->pool->enqueue(explore_task);
    this->pool->wait_until_done();              // The main thread goes here and sleeps untill the thread pool makes the job done
    delete this->pool;                          // Destroying the pool

    size_t sh = __SIZE_MAX__;   // This part of the code is responsible for detecting the shortest path

    for (auto & cell : this->ends) {        // We find the path that had the least steps ever
        if (cell.first < sh) {
            sh = cell.first;
            this->shortest = cell.second;
        }
    }

    /// BACKTRACK MECHANISM
    for (auto cell : this->ends) {  // This is used to mark the visited, valid, and shortest cells.
        Maze::Cell * end_cell = cell.second;

        while (end_cell) {
            if (end_cell == this->shortest) {   // If the cell is a part of the shortest path, we mark it differently
                while (end_cell) {
                    end_cell->shortest = true;
                    end_cell = end_cell->parent;
                }
                break;
            }
            end_cell->valid_cell = true;
            end_cell = end_cell->parent;
        }
    }
}


inline void Maze::print(void) const noexcept {      // Used to print the maze
    for (__SIZE_TYPE__ y=0; y<this->map.size(); y++) {
        for (__SIZE_TYPE__ x=0; x<this->map[y].size(); x++) {
            if (this->map[y][x].shortest) {             // Print green if the cell is a part of the shortest path
                std::cout << GREEN << '*' << RESET;
            } else if (this->map[y][x].valid_cell) {
                std::cout << CYAN << '*' << RESET;        // Print cyan if it is a valid path
            } else if (this->map[y][x].visited) {
                std::cout << YELLOW << '.' << RESET;     // Print yellow dots if the cell was explored
            } else {
                if (this->map[y][x].type == '#') {        // Print if it is a wall
                    std::cout << "█";
                } else {
                    std::cout << this->map[y][x].type;
                }
            }
        }
        std::cout << std::endl;
    }
}


inline void Maze::make_adjacent(void) noexcept {   // Creating an adjacency list
    for (__SIZE_TYPE__ i=0; i<this->map.size(); i++) {
        for (__SIZE_TYPE__ j=0; j<this->map[i].size(); j++) {
            if (this->map[i][j].type != '#') {
                this->adjacency_list[&this->map[i][j]] = {};

                if (i > 0 and this->map[i-1].size() > j and this->map[i-1][j].type != '#') {
                    this->adjacency_list[&this->map[i][j]].emplace_back(&this->map[i-1][j]);
                }

                if (j + 1 < this->map[i].size() and this->map[i][j+1].type != '#') {
                    this->adjacency_list[&this->map[i][j]].emplace_back(&this->map[i][j+1]);
                }

                if (i + 1 < this->map.size() and this->map[i+1].size() > j and this->map[i+1][j].type != '#') {
                    this->adjacency_list[&this->map[i][j]].emplace_back(&this->map[i+1][j]);
                }

                if (j > 0 and this->map[i][j-1].type != '#') {
                    this->adjacency_list[&this->map[i][j]].emplace_back(&this->map[i][j-1]);
                }

                if (this->adjacency_list[&this->map[i][j]].empty()) {
                    this->adjacency_list.erase(&this->map[i][j]);
                }
            }
        }
    }
}


void Maze::search() noexcept {
    std::cout << "Starting the search...\n";
    std::cout << RESET << "Searching the path...\n";

#ifdef _WIN32   // Section used to calculate the amount of threads this app can create.
    MEMORYSTATUSEX mem_info;
    mem_info.dwLength = sizeof(MEMORYSTATUSEX);
    GlobalMemoryStatusEx(&mem_info);

    SIZE_T total_memory = mem_info.ullTotalPhys / (1024 * 1024);
    SIZE_T available_memory = mem_info.ullAvailPhys / (1024 * 1024);
    SIZE_T default_stack_size = 1 << 20;
    SIZE_T max_threads = available_memory * (1024 * 1024) / default_stack_size;

    std::cout << CYAN << "Total system memory: " << total_memory << " MB.\n";
    std::cout << "Available memory: " << available_memory << " MB.\n";
    std::cout << "Assumed default thread stack size: " << default_stack_size / 1024 << " KB.\n";
    std::cout << "Estimated maximum number of threads available: " << max_threads << RESET << std::endl;
#elif defined(__linux__)
    // Retrieve the size of a page in bytes.
    long page_size = sysconf(_SC_PAGESIZE);
    // Get the total number of pages in the physical memory.
    long num_pages = sysconf(_SC_PHYS_PAGES);
    // Get the number of available pages in the physical memory.
    long available_pages = sysconf(_SC_AVPHYS_PAGES);
    // Calculate total memory by multiplying page size with the number of pages.
    __SIZE_TYPE__ total_memory = page_size * num_pages;
    // Calculate available memory using page size and available pages.
    __SIZE_TYPE__ available_memory = page_size * available_pages;
    pthread_attr_t attr;
    __SIZE_TYPE__ stack_size;
    // Initialize thread attribute object.
    pthread_attr_init(&attr);
    // Get the default stack size for threads.
    pthread_attr_getstacksize(&attr, &stack_size);

    // Display total system memory in MB.
    std::cout << CYAN << "Total system memory: " << total_memory / (1024 * 1024) << " MB.\n";
    // Display available system memory in MB.
    std::cout << "Available memory: " << available_memory / (1024 * 1024) << " MB.\n";
    // Display default stack size for threads in KB.
    std::cout << "Default thread stack size: " << stack_size / 1024 << " KB.\n";

    // Estimate the maximum number of threads based on available memory and stack size.
    __SIZE_TYPE__ max_threads = available_memory / stack_size;
    std::cout << "Estimated maximum number of threads: " << max_threads << std::endl << RESET;
    // Clean up thread attribute object.
    pthread_attr_destroy(&attr);

#elif defined(__ARM_ARCH) || defined(__aarch64__) || defined(_M_ARM) || defined(_M_ARM64) || defined(__APPLE__) || defined(__MACH__)
    // Initialize variables for page size and the mach port.
    vm_size_t page_size;
    mach_port_t mach_port = mach_host_self();
    vm_statistics64_data_t vm_stats;
    // Set the count for the vm_statistics data.
    mach_msg_type_number_t count = sizeof(vm_stats) / sizeof(natural_t);

    // Retrieve the page size and memory statistics from the system.
    if (host_page_size(mach_port, &page_size) == KERN_SUCCESS &&
        host_statistics64(mach_port, HOST_VM_INFO, (host_info64_t)&vm_stats, &count) == KERN_SUCCESS) {
        // Calculate free memory using the number of free pages and page size.
        long long free_memory = static_cast<long long>(vm_stats.free_count) * static_cast<long long>(page_size);
        // Calculate available memory (free + inactive pages).
        long long available_memory = free_memory + static_cast<long long>(vm_stats.inactive_count) * static_cast<long long>(page_size);

        pthread_attr_t attr;
        // Initialize thread attribute object.
        pthread_attr_init(&attr);
        size_t stack_size;
        // Get the default stack size for threads.
        pthread_attr_getstacksize(&attr, &stack_size);
        // Clean up thread attribute object.
        pthread_attr_destroy(&attr);

        // Display free and available memory in MB.
        std::cout << CYAN << "Free memory: " << free_memory / (1024 * 1024) << " MB.\n";
        std::cout << "Available memory: " << available_memory / (1024 * 1024) << " MB.\n";
        // Display the default thread stack size in KB.
        std::cout << "Default thread stack size: " << stack_size / 1024 << " KB.\n";

        // Estimate the maximum number of threads based on available memory and stack size.
        size_t max_threads = available_memory / stack_size;
        std::cout << "Estimated maximum number of threads: " << max_threads << std::endl << RESET;

    } else {
        // If retrieving the system information fails, display an error.
        std::cerr << "Failed to analyse RAM." << std::endl;
        _Exit(1);
    }
#endif


    sleep(1);

    try {
        this->explore(1000);    // Starting the exploration
    } catch (std::system_error & exception) {
        std::cout << RED << exception.what();
    }


    std::cout << GREEN << "The paths have been contructed successfully.\n";
}


int main() {
#ifdef _WIN32                                // section needed for allowing windows terminal to display colored text
    system(" ");
    SetConsoleOutputCP(CP_UTF8);
#endif
    std::cout << "The program started successfully." << std::endl;          // Intro menu
    std::cout << "The program reads a file and constructs a maze following these rules:\n";
    std::cout << "* There should be only one entrance defined by the character - '1'.\n";
    std::cout << "* There might be 1 <= n exits anywhere defined by the character - '2'.\n";
    std::cout << "* There might be a path anywhere defined by the character - '0'.\n";
    std::cout << "* There might be a wall anywhere defined by characters with the following IDs in the ASCII table\n 32-42, 44-47, 51-126.\n";
    std::cout << "Use CTRL + Z and Enter, to exit the program.\n";
    std::cout << YELLOW << "For the emergency exit use CTRL + C.\n" << RESET;

#ifdef _WIN32   // Section to define the way you provide the complete path for each operation system
    std::cout << "Enter a complete path to the file containing the maze; for ex. \"C:\\Windows\\Boot\\BootDebuggerFiles.ini\" (without quotes).\nPath:";
#elif defined(__linux__) || defined(__ARM_ARCH) || defined(__aarch64__) || defined(_M_ARM) || defined(_M_ARM64) || defined(__APPLE__) || defined(__MACH__)
    std::cout << "Enter a complete path to the file containing the maze; for ex. \"~/User/Desktop/file.txt\" (without quotes).\n Path:";
#endif

    std::string path_str;
    std::filesystem::path path; // Temporary variable needed to check the path's correctness
    std::ifstream file;         // Variable responsible for the file

    std::unique_ptr<Maze> maze = nullptr;   //The smart pointer of the Maze class

    while (true) {                  // The entire program
        std::cin >> path_str;       // Entering the path to the maze
        path = path_str;

        if (feof(stdin)) {          // Checking if you decide to end the program via shortcut
            std::cout << YELLOW << "Exiting..." << std::endl;
            sleep(3);
            return EXIT_SUCCESS;
        }

        if (!path.is_absolute()) {      // Check if the path is absolute (used for security reasons)
            std::cout << RED << "The path is invalid.\n" << RESET << "Try again:";
        } else if (!std::filesystem::exists(path)) {    // Check if the file exists
            std::cout << RED << "The filename is invalid or doesn't exist.\n" << RESET << "Try again:";
        } else {
            file.open(path_str.c_str());    // Opening the file

            if (!file) {        // Check if it is actually opened.
                std::cerr << RED << "Error occurred while opening " << path_str << std::endl << RESET << "Try again:";
            } else {
                std::cout << GREEN << "The file has been opened successfully.\n" << YELLOW << "Building a maze...\n" << RESET;

                try {   // Creating a maze in case if all went good
                    maze = std::make_unique<Maze>(file);
                } catch (const std::exception & e) {        // Catching an exception during the creation
                    std::cout << RED << e.what() << std::endl << RESET;
                    file.clear();
                    file.close();
                    file.seekg(0, std::ios::beg);
                    std::cout << "Try again:";
                    goto b0x00;             // Continuing the cycle
                }

                std::cout << GREEN << "The maze has been built successfully.\n" << RESET << "The maze looks like:\n";

                maze->print();      // Printing the entire maze

                std::cout << "\nSelect next operations:\n";

                char ch;
                while (true) {      // New loop for the maze manipulation
                    std::cout << "* '1' Print.\n* '2' Find short path.\n* '3' Reset.\n* '4' Load new maze.\n* '5' Exit.\nSelect:";
                    std::cin >> ch;

                    if (feof(stdin)) {
                        std::cout << YELLOW << "Exiting..." << std::endl;
                        sleep(3);
                        return EXIT_SUCCESS;
                    }

                    switch (ch) {   // Retrieving the operations from the user
                        case '1':
                            maze->print();
                            break;

                        case '2':
                            maze->search(); // Execute the search for the paths
                            std::cout << RESET << std::endl;
                            break;

                        case '3':
                            std::cout << YELLOW << std::endl << "Resetting the maze...\n" << RESET;
                            file.close();
                            file.open(path_str.c_str());
                            maze = std::make_unique<Maze>(file);    // Basically restoring the maze
                            std::cout << GREEN << "Resetted.\n" << RESET;
                            break;

                        case '4':   // Opening another maze from the file
                            file.close();
                            std::cout << YELLOW << "Deleting the maze...\n" << RESET;
                            std::cout << "Enter an absolute path to a new maze:";
                            goto b0x00;

                        case '5':   // Exiting the app.
                            std::cout << YELLOW << "Exiting..." << std::endl;
                            sleep(3);
                            return EXIT_SUCCESS;

                        default:
                            std::cout << RED << "Invalid option." << RESET << "Try again:";
                            break;
                    }
                }
            }
        }
        b0x00:;
    }
}