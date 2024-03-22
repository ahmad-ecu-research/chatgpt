#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <cctype>

bool isValidFileName(const std::string &filename) {
    for (char c : filename) {
        if (!std::isalnum(c) && c != '.' && c != '-' && c != '_') {
            return false;
        }
    }
    return true;
}

bool isMatch(const std::string &s, const std::string &p) {
    // ... [rest of the isMatch function as previously provided]
}

int main() {
    std::string inputFileName = "input.txt";
    std::string outputFileName = "output.txt";

    // Sanitize file names
    if (!isValidFileName(inputFileName) || !isValidFileName(outputFileName)) {
        std::cerr << "Invalid file name!" << std::endl;
        return 1;
    }

    std::ifstream inputFile(inputFileName);
    std::ofstream outputFile(outputFileName);

    if (!inputFile || !outputFile) {
        std::cerr << "Error opening files!" << std::endl;
        return 1;
    }

    int t;
    if (!(inputFile >> t)) {
        std::cerr << "Invalid input format!" << std::endl;
        return 1;
    }
    inputFile.ignore();  // consume newline

    while (t--) {
        std::string text, pattern;
        if (!std::getline(inputFile, text) || !std::getline(inputFile, pattern)) {
            std::cerr << "Error reading from input file!" << std::endl;
            return 1;
        }

        bool result = isMatch(text, pattern);
        outputFile << (result ? "true" : "false") << std::endl;
    }

    inputFile.close();
    outputFile.close();
    return 0;
}
