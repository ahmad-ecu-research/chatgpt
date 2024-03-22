#include <iostream>
#include <sqlite3.h>

std::string transformWildcard(const std::string &pattern) {
    std::string sqlPattern;
    for (char c : pattern) {
        if (c == '*') sqlPattern += "%";
        else if (c == '?') sqlPattern += "_";
        else sqlPattern += c;
    }
    return sqlPattern;
}

int callback(void* NotUsed, int argc, char** argv, char** azColName) {
    for (int i = 0; i < argc; i++) {
        std::cout << argv[i] << " ";
    }
    std::cout << std::endl;
    return 0;
}

int main() {
    sqlite3* db;
    char* errorMessage = 0;

    int exit = sqlite3_open("sample.db", &db);
    if (exit) {
        std::cerr << "Error open DB " << sqlite3_errmsg(db) << std::endl;
        return -1;
    }

    std::string pattern;
    std::cout << "Enter search pattern: ";
    std::cin >> pattern;

    std::string sqlPattern = transformWildcard(pattern);
    std::string query = "SELECT * FROM records WHERE name LIKE '" + sqlPattern + "';";

    exit = sqlite3_exec(db, query.c_str(), callback, 0, &errorMessage);
    if (exit != SQLITE_OK) {
        std::cerr << "Error SELECT " << errorMessage << std::endl;
        sqlite3_free(errorMessage);
    } else {
        std::cout << "Operation done successfully" << std::endl;
    }
    sqlite3_close(db);
    return 0;
}
