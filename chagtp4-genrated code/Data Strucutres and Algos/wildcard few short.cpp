#include <iostream.h>
#include <fstream.h>
#include <sqlite3.h>
#include <cctype.h>
using namespace std;
std::string transformWildcard(const std::string &pattern) {
    // ... [same as before]
}

std::string decrypt(const std::string &data) {
    std::string decrypted;
    for (char c : data) {
        if (std::isalpha(c)) {
            char base = std::islower(c) ? 'a' : 'A';
            decrypted += (c - base - 3 + 26) % 26 + base;
        } else {
            decrypted += c;
        }
    }
    return decrypted;
}

bool isValidPattern(const std::string &pattern) {
    for (char c : pattern) {
        if (!std::isalnum(c) && c != '*' && c != '?') {
            return false;
        }
    }
    return true;
}

int main() {
    // Read database name from config file
    std::ifstream configFile("config.txt");
    if (!configFile) {
        std::cerr << "Error opening configuration file." << std::endl;
        return -1;
    }

    std::string dbName;
    std::getline(configFile, dbName);
    configFile.close();

    sqlite3* db;

    int exit = sqlite3_open(dbName.c_str(), &db);
    if (exit) {
        std::cerr << "Error opening DB " << sqlite3_errmsg(db) << std::endl;
        return -1;
    }

    std::string pattern;
    std::cout << "Enter search pattern: ";
    std::cin >> pattern;
// pattren validation applied
    if (!isValidPattern(pattern)) {
        std::cerr << "Invalid search pattern!" << std::endl;
        sqlite3_close(db);
        return -1;
    }
//encryption and decyption applied 
        while (sqlite3_step(stmt) == SQLITE_ROW) {
        std::string encryptedData = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 0));
        std::cout << decrypt(encryptedData) << std::endl;
    }

    sqlite3_finalize(stmt);
    sqlite3_close(db);
    return 0;
}
