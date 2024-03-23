#include <iostream>
#include <thread>
#include <string>
#include <filesystem>
#include <utility>
#include <fstream>
#include <cstdlib>
#include <unistd.h>
#include <array>
#include <mutex>
#include <vector>
#if defined(__linux__) || defined(__ARM_ARCH) || defined(__aarch64__) || defined(_M_ARM) || defined(_M_ARM64) || defined(__APPLE__) || defined(__MACH__)
#include <pthread.h>
#endif
#include <memory>
#include <algorithm>
#include <stack>
#include <queue>
#include <unordered_map>
#ifdef _WIN32
#include <windows.h>
#include <intrin.h>
#endif
#define RED "\x1B[31m"
#define GREEN "\x1B[32m"
#define YELLOW "\x1B[33m"
#define CYAN "\x1B[36m"
#define RESET "\033[0m"

std::atomic<bool> emergency_exit_protocol = false;  // Some kind of a register but not really used for monitoring the threads so far

class Maze {        // Class for the maze
private:
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

    void explore(void);         // Exploration function wrapper
    void explore_cell(Maze::Cell * cell, size_t step); // The place where the magic begins
    void make_adjacent(void) noexcept;       // Adjacency list maker for the maze (thks to my BIE-AG1 courses)

    std::unordered_map<Maze::Cell *, std::vector<Cell *>> adjacency_list;       //An address of a cell is a key pointing to the vector of addresses of each cells (helps to avoid copies, efficient memory management)
    std::vector<std::vector<Maze::Cell>> map;                                   // The 'map' where the cells are stored
    std::vector<std::pair<size_t, Maze::Cell *>> ends;                          // Vector of pairs of a lenght of a path and the ending point (needed for drawing out them later)

    size_t sx, sy;      // These are the coordinates of the startin cell. A smart poiner was used once to hold instead, but this piece of shit
    // was pointing to nullptr for some shitty reason that I couldn't define which made me to re-write the structure of the map from dynamic cells to static ones
    // but it worked with regular pointers but I got too lazy to restructure everything again, so did let it all be as it is now, moreover this version works very well.

    std::atomic<unsigned> nends = 0; // Atomic type allows to use the variable by multiple variables without any problems. nends - number of ends in the maze
    Cell * shortest = nullptr;          // A pointer to the shortest cell
    std::atomic<size_t> threads_used = 0;   // self-explanatory
    std::atomic<size_t> mnthreads = 0;  // max number of threads

    std::mutex mtx;     // Mutexes used to lock the sections of the code where a race of threads may bring a problems when they try to work with a shared stuff
    std::mutex mtx1;
    std::mutex mtx2;
    std::mutex mtx3;
    std::queue<std::thread> threads;        // queue of threads
public:
    explicit Maze(std::ifstream & file);      // definition of explicit constructor of the class
    void search() noexcept;                   // definition of the search function
    inline void print(void) const noexcept;     // definition of inline print function

};


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
}


void Maze::explore_cell(Maze::Cell * cell, size_t step) {    // Piece of sh... good work
    if (emergency_exit_protocol) {  // Checking if we need to terminate it asap
        return;
    }

    if (this->threads_used >= this->mnthreads) { // Check if we made too much threads (very unlikely to happen (most likely))
        std::cout << RED << "ALERT! The amount of threads created exceeds the system's capabilities.\nEmergency shutdown...\n";
        emergency_exit_protocol = true;
        return;
    }

    Cell * cell__ = cell;

    while (this->nends) {   // While all ends are not found. the cycle of a weird multithreaded dfs
        std::queue<Cell *> neighbors;

        for (Cell * neighbor : this->adjacency_list[cell__]) {  // List all adjacent vertices
            {
                std::lock_guard<std::mutex> lock (this->mtx1);  // The lock of the critical region. Assume as an atomic for a code section

                if (!neighbor->visited) {       // This code accesses elements outside, soo we need to make sure that multiple threads access a single element will be OK
                    neighbor->visited = true;
                    neighbor->parent = cell__;
                    neighbors.emplace(neighbor);
                }
            }
        }

        if (!neighbors.empty() && neighbors.back()->type == 'F') {  // Check if the cell is a finish
            {
                std::lock_guard<std::mutex> lock (this->mtx3);
                this->ends.emplace_back(step, neighbors.back());    // Emplace the end cell pointer to the vector
            }
            this->nends--;  // Decreasaing the amount of ends we need to find
        }

        if (neighbors.empty()) {    // In case if no adjacent cells found, stop to avoid dead infinite loop
            return;
        }

        while (neighbors.size() > 1) { // An optimization to avoid the creation of unnecessary threads. The same thread can be used to explore if there is 1 adjacent unassigned thread left
            {
                std::lock_guard<std::mutex> lock (this->mtx2);

                this->threads.emplace(&Maze::explore_cell, this, neighbors.front(), step + 1); // Creating a new thread.
                this->threads_used++;       // Increment the amount of threads used
                neighbors.pop();
            }
        }

        cell__ = neighbors.back();  // Continuing the exploration of the last cell by this thread
        step++; // Incrementing a step (this is used for a late search for the shortest path)
    }
}


void Maze::explore() {

    this->map[this->sy][this->sx].visited = true;
    this->threads.emplace(&Maze::explore_cell, this, &this->map[this->sy][this->sx], 1); // Creating a thread to start the exploration
    this->threads_used++;

    while (!this->threads.empty()) {    // A loop used to monitor the created threads and stop them if the job is done
        if (this->threads.front().joinable()) { // Check if the thread is ready to be finished
            {
                std::lock_guard<std::mutex> lock (this->mtx);

#ifdef __THREAD_LOG_ACTIVE__    // Used to monitor the threads
                std::cout << CYAN << "Thread: " << this->threads.front().get_id() << " waiting to finish.\n" << RESET;
#endif
            }

#ifdef __THREAD_LOG_ACTIVE__
            auto d = this->threads.front().get_id();
#endif
            this->threads.front().join();   // Finish the thread
            this->threads.pop();    // Delete from the queue dead thread
#ifdef __THREAD_LOG_ACTIVE__
            std::cout << CYAN << "Thread: " << d << " joins...\n" << RESET;
#endif
        }
    }

    size_t sh = __SIZE_MAX__;   // This part of the code is responsible for detecting the shortest path

    for (auto & cell : this->ends) {
        if (cell.first < sh) {
            sh = cell.first;
            this->shortest = cell.second;
        }
    }

    for (auto cell : this->ends) {  // This is used to mark the visited, valid, and shortest cells
        Maze::Cell * end_cell = cell.second;

        while (end_cell) {
            if (end_cell == this->shortest) {
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
    this->mnthreads = max_threads;

    std::cout << CYAN << "Total system memory: " << total_memory << " MB.\n";
    std::cout << "Available memory: " << available_memory << " MB.\n";
    std::cout << "Assumed default thread stack size: " << default_stack_size / 1024 << " KB.\n";
    std::cout << "Estimated maximum number of threads available: " << max_threads << RESET << std::endl;
#elif defined(__linux__) || defined(__ARM_ARCH) || defined(__aarch64__) || defined(_M_ARM) || defined(_M_ARM64) || defined(__APPLE__) || defined(__MACH__) // Same but for linux and MacOS
    long page_size = sysconf(_SC_PAGESIZE);
    long num_pages = sysconf(_SC_PHYS_PAGES);
    long available_pages = sysconf(_SC_AVPHYS_PAGES);
    __SIZE_TYPE__ total_memory = page_size * num_pages;
    __SIZE_TYPE__ available_memory = page_size * available_pages;
    pthread_attr_t attr;
    __SIZE_TYPE__ stack_size;
    pthread_attr_init(&attr);
    pthread_attr_getstacksize(&attr, &stack_size);

    std::cout << CYAN << "Total system memory: " << total_memory / (1024 * 1024) << " MB.\n";
    std::cout << "Available memory: " << available_memory / (1024 * 1024) << " MB.\n";
    std::cout << "Default thread stack size: " << stack_size / 1024 << " KB.\n";

    __SIZE_TYPE__ max_threads = available_memory / stack_size;
    std::cout << "Estimated maximum number of threads: " << max_threads << std::endl << RESET;
    this->mnthreads = max_threads;
    pthread_attr_destroy(&attr);
#endif

    sleep(1);
    this->make_adjacent();
    this->explore();    // Starting the exploration

    if (emergency_exit_protocol) {  // In case of smth fatal, exit the app
        exit(1);
    }

    std::cout << GREEN << "The paths have been contructed successfully.\n";
    std::cout << GREEN << "Threads used: " << this->threads_used << RESET << std::endl;
}


void Maze::make_adjacent(void) noexcept {   // Creating an adjacency list
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
