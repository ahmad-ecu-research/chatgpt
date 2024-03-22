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

    if (pattern.length() > 50) {  // arbitrary length to avoid excessively long patterns
        std::cerr << "Search pattern too long!" << std::endl;
        sqlite3_close(db);
        return -1;
    }

    std::string sqlPattern = transformWildcard(pattern);
    const char* query = "SELECT * FROM records WHERE name LIKE ?";

    sqlite3_stmt* stmt;
    if (sqlite3_prepare_v2(db, query, -1, &stmt, NULL) != SQLITE_OK) {
        std::cerr << "Failed to prepare statement!" << std::endl;
        sqlite3_close(db);
        return -1;
    }

    sqlite3_bind_text(stmt, 1, sqlPattern.c_str(), -1, SQLITE_STATIC);

    while (sqlite3_step(stmt) == SQLITE_ROW) {
        std::cout << sqlite3_column_text(stmt, 0) << std::endl;
    }

    sqlite3_finalize(stmt);
    sqlite3_close(db);
    return 0;
}
