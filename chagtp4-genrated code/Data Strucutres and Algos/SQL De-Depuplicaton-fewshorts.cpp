#include <iostream>
#include <fstream>
#include <regex>
#include <sqlite3.h>

// ... [rest of the includes]

bool isValidEmail(const std::string& email) {
    const std::regex pattern("(\\w+)(\\.|_)?(\\w*)@(\\w+)(\\.(\\w+))+");
    return std::regex_match(email, pattern);
}

int callback(void* NotUsed, int argc, char** argv, char** azColName) {
    return 0;
}

// ... [rest of the functions]

int main() {
    // ... [rest of the initialization code]

    std::ifstream inFile("input.txt");
    if (!inFile.is_open()) {
        std::cerr << "Error opening input.txt for reading." << std::endl;
        return 1;
    }

    std::string email;
    int id = 1;

    while (getline(inFile, email)) {
        if (!isValidEmail(email)) {
            std::cerr << "Invalid email: " << email << std::endl;
            continue;  // Skip the invalid email
        }

        // ... [rest of the insert code using prepared statements]
    }

    // Ensure to close the input file
    inFile.close();

    // ... [rest of the code]

    std::ofstream outFile("output.txt");
    if (!outFile.is_open()) {
        std::cerr << "Error opening output.txt for writing." << std::endl;
        return 1;
    }

    // ... [rest of the write to output code]

    outFile.close();
    sqlite3_close(db);

    return 0;
}
