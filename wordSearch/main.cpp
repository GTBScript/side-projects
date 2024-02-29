#include <iostream>
#include <utility>
#include <vector>
#include <string>
#include <fstream>
#include <filesystem>
#include <ctime>
#include <cstdlib>
#include <unistd.h>

#define RED "\x1B[31m"
#define GREEN "\x1B[32m"
#define YELLOW "\x1B[33m"
#define CYAN "\x1B[36m"
#define RESET "\033[0m"

typedef struct Letter {     // Structure for letters in the matrix that would handle the appereance
    char ch;
    std::string fclr;

    explicit Letter(const char ch, std::string fclr) : ch(ch), fclr(std::move(fclr)) {}     // Explicit constructor
} Letter;

/// Used to print out the matrix in the current state
inline void print_matrix(const std::vector<std::vector<Letter>> & matrix) {
    for (const std::vector<Letter> & vector : matrix) {
        for (const Letter & letter : vector) {
            std::cout << letter.fclr << letter.ch;
        }
        std::cout << std::endl;
    }
}

/// Used to create the matrix out of the file
std::vector<std::vector<Letter>> make_matrix(std::ifstream * file) {
    std::string line;
    std::vector<std::vector<Letter>> matrix;

    while (std::getline(*file, line)) {
        std::vector<Letter> row;
        for (const char & ch : line) {
            Letter letter (ch, RESET);
            row.emplace_back(std::ref(letter));
        }
        matrix.emplace_back(row);
    }

    return matrix;
}

/// Checking procedure used to check if the file content appears to be rectangular
constexpr bool is_rectangular(std::ifstream & file) noexcept {
    std::string line;
    size_t expected_length = 0;
    bool first_line = true;

    while (std::getline(file, line)) {
        if (first_line) {
            expected_length = line.length();
            first_line = false;
        } else if (line.length() != expected_length) {
            return false;
        }
    }

    return true;
}

// Function used to execute procedures of search in 8 directions in the matrix one by one untill we find the word
std::vector<Letter *> search(std::vector<std::vector<Letter>> & matrix, const std::string & word, const int x, const int y) noexcept {
    std::vector<Letter *> path;
    path.emplace_back(&matrix[y][x]);
    int i = 1;

    // Check if we can check towards right
    if (matrix[0].size() - x >= word.length()) {
        for (int __x = x + 1; __x<matrix[0].size(); __x++, i++) {
            if (matrix[y][__x].ch == word[i]) {
                path.emplace_back(&matrix[y][__x]);
            } else {
                break;
            }
        }

        if (path.size() == word.size()) {
            return path;
        } else {
            path.clear();
        }
    }


    if (x + 1 >= word.length()) { // Check if we can check towards left
        i = 1;
        path.emplace_back(&matrix[y][x]);

        for (int __x = x - 1; __x >= 0; __x--, i++) {
            if (matrix[y][__x].ch == word[i]) {
                path.emplace_back(&matrix[y][__x]);
            } else {
                break;
            }
        }

        if (path.size() == word.size()) {
            return path;
        } else {
            path.clear();
        }
    }


    if (y + 1 >= word.length()) { // Check if we can check towards up
        i = 1;
        path.emplace_back(&matrix[y][x]);

        for (int __y = y + 1; __y < matrix.size(); __y++, i++) {
            if (matrix[__y][x].ch == word[i]) {
                path.emplace_back(&matrix[__y][x]);
            } else {
                break;
            }
        }

        if (path.size() == word.size()) {
            return path;
        } else {
            path.clear();
        }
    }


    if (matrix.size() - y >= word.length()) { // Check if we can check towards down
        i = 1;
        path.emplace_back(&matrix[y][x]);

        for (int __y = y + 1; __y < matrix.size(); __y++, i++) {
            if (matrix[__y][x].ch == word[i]) {
                path.emplace_back(&matrix[__y][x]);
            } else {
                break;
            }
        }

        if (path.size() == word.size()) {
            return path;
        } else {
            path.clear();
        }
    }

    // Check diagonally down-right
    if ((matrix[0].size() - x >= word.length()) && (matrix.size() - y >= word.length())) {
        i = 1;
        int __x = static_cast<int>(x) + 1;
        int __y = static_cast<int>(y) + 1;
        path.emplace_back(&matrix[y][x]);

        while (__x < matrix[0].size() && __y < matrix.size()) {
            if (word[i] == matrix[__y][__x].ch) {
                path.emplace_back(&matrix[__y][__x]);
            } else {
                break;
            }

            __y++;
            __x++;
            i++;
        }

        if (path.size() == word.size()) {
            return path;
        } else {
            path.clear();
        }
    }

    // Check diagonally down-left
    if ((x + 1 >= word.length()) && (matrix.size() - y >= word.length())) {
        i = 1;
        int __x = static_cast<int>(x) - 1;
        int __y = static_cast<int>(y) + 1;
        path.emplace_back(&matrix[y][x]);

        while (__x >= 0 && __y < matrix.size()) {
            if (word[i] == matrix[__y][__x].ch) {
                path.emplace_back(&matrix[__y][__x]);
            } else {
                break;
            }

            __y++;
            __x--;
            i++;
        }

        if (path.size() == word.size()) {
            return path;
        } else {
            path.clear();
        }
    }

    // Check diagonally up-right
    if ((matrix[0].size() - x >= word.length()) && (y + 1 >= word.length())) {
        i = 1;
        int __x = static_cast<int>(x) + 1;
        int __y = static_cast<int>(y) - 1;
        path.emplace_back(&matrix[y][x]);

        while (__x < matrix[0].size() && __y >= 0) {
            if (word[i] == matrix[__y][__x].ch) {
                path.emplace_back(&matrix[__y][__x]);
            } else {
                break;
            }

            __x++;
            __y--;
            i++;
        }

        if (path.size() == word.size()) {
            return path;
        } else {
            path.clear();
        }
    }

    // Check diagonally up-left
    if ((x + 1 >= word.length()) && (y + 1 >= word.length())) {
        i = 1;
        int __x = static_cast<int>(x) - 1;
        int __y = static_cast<int>(y) - 1;
        path.emplace_back(&matrix[y][x]);

        while (__x >= 0 && __y >= 0) {
            if (word[i] == matrix[__y][__x].ch) {
                path.emplace_back(&matrix[__y][__x]);
            } else {
                break;
            }

            __x--;
            __y--;
            i++;
        }

        if (path.size() == word.size()) {
            return path;
        } else {
            path.clear();
        }
    }

    return path;
}

/// Wrapper for the search function, where we check if the matrix contains the first letter of the desired word
bool contains(std::vector<std::vector<Letter>> & matrix, const std::string & word) noexcept {
    for (int y=0; y<matrix.size(); y++) {
        for (int x=0; x<matrix[0].size(); x++) {
            if (word[0] == matrix[y][x].ch) {
                std::vector<Letter *> path = search(matrix, word, x, y);

                if (!path.empty()) {    // if the word is found, we highlight it with a certain color
                    const std::vector<std::string> colors = {"\x1B[31m", "\x1B[32m", "\x1B[33m", "\x1B[36m", "\x1B[35m", "\x1B[37m"};

                    const std::string & color = colors[rand() % 6];

                    for (Letter *& letter : path) {
                        letter->fclr = color;
                    }

                    return true;
                }
            }
        }
    }

    return false;
}

// Function used to extract the unchanged characters to form a passphrase
std::string extract(std::vector<std::vector<Letter>> & matrix) {
    std::string txt;

    for (int i=0; i<matrix.size(); i++) {
        for (int j=0; j<matrix[0].size(); j++) {
            if (matrix[i][j].fclr == "\033[0m") {
                txt += matrix[i][j].ch;
            }
        }
    }

    return txt;
}


int main() {
    srand(static_cast<unsigned>(time(nullptr)));    // Random generator set up

    std::string path_str;
    std::vector<std::vector<Letter>> matrix;

    std::cout << "The program started successfully.\n";
    std::cout << "* CTRL + Z to exit the program.\n";
    std::cout << "Enter the complete path to the file containing matrix of characters.\nMake sure the content view is rectangular!\n";
    std::cout << "The program is sensetive to lowercase and uppercase characters!" << std::endl;
    std::cout << "The current version supports only ASCII characters!\n";
    std::cout << R"(Path (for example: C:\Windows\Boot\BootDebuggerFiles.ini): )";

    std::filesystem::path path;
    std::ifstream file;

    do {       // Main menu loop, used to simulate terminal GUI since I do not have enough time to build normal SFML gui app right now, sorry :3
        std::cin >> path_str;
        path = path_str;

        if (feof(stdin)) {
            std::cout << YELLOW << "Exiting...\n";
            sleep(3);
            return EXIT_SUCCESS;
        }

        if (!path.is_absolute()) {  // Check for the security reasons as the program would require the full path to the file avoiding potential file hacks.
            std::cout << RED << "Invalid path.\n" << RESET << "Try again: ";
        } else if (!std::filesystem::exists(path)) {
            std::cout << RED << "The file does not exist.\n" << RESET << "Try again: ";
        } else {
            file.open(path_str.c_str());

            if (!file) {    // Check if the file opened successfully.
                std::cerr << RED << "Error occurred while opening " << path_str << std::endl << RESET << "Try again: ";
            } else {
                std::cout << GREEN << "The file has been opened successfully.\n" << YELLOW << "Checking the content...\n";
                if (!is_rectangular(file)) {        // Check for the rectangular data view, since we deal with matrix.
                    std::cout << RED << "The content is not rectangular.\n" << YELLOW << "Check for TABS and whitespaces!\n" << RESET << "Try again: ";
                    file.close();
                    continue;
                } else {
                    file.clear();
                    file.seekg(0, std::ios::beg);
                    std::cout << YELLOW << "Building the matrix...\n";
                    break;
                }
            }
        }
    } while (true);

    // When all checks are passed above, we begin the main stuff

    matrix = make_matrix(&file);

    std::cout << GREEN << "The matrix has been built successfully.\n" << RESET << "The matrix:\n" << CYAN;

    print_matrix(matrix);       // Print the current matrix built

    std::string word;

    while (true) {          // The loop where the words are searched.
        std::cout << RESET << "Enter the word you want to highlight: ";
        std::cin >> word;

        if (feof(stdin)) {      // Detecting EOF if nothing to enter. (CTRL + Z)
            std::cout << YELLOW << "Extracting the remainging characters...\n";
            std::string txt = extract(matrix);

            if (txt.empty()) {
                std::cout << "Nothing to extract.\n";
                return EXIT_SUCCESS;
            }

            std::cout << YELLOW << "Building the passphrase...\n";
            std::cout << RESET << "The passphrase: " << txt << std::endl;

            std::cout << YELLOW << "The program will exit automatically in 10 seconds.";
            sleep(10);


            return EXIT_SUCCESS;
        }

        if (word.empty()) {
            std::cout << YELLOW << "The word is empty.\n" << RESET << "Try again: ";
        } else {
            contains(matrix, word);
            print_matrix(matrix);
        }
    }
}
