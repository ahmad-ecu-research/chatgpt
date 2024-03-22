#include <iostream>
#include <fstream>
#include <sqlite3.h>

int callback(void* NotUsed, int argc, char** argv, char** azColName) {
    return 0;
}

void deleteDuplicates(sqlite3* db) {
    char* errorMessage = 0;

    const char* sql = R"(
        DELETE FROM Emails
        WHERE Id NOT IN (
            SELECT MIN(Id)
            FROM Emails
            GROUP BY Email
        );
    )";

    if (sqlite3_exec(db, sql, callback, 0, &errorMessage) != SQLITE_OK) {
        std::cerr << "Error executing query: " << errorMessage << std::endl;
        sqlite3_free(errorMessage);
    }
}

int main() {
    // Open SQLite connection
    sqlite3* db;
    if (sqlite3_open(":memory:", &db) != SQLITE_OK) {
        std::cerr << "Error opening database." << std::endl;
        return 1;
    }

    // Create table
    char* errorMessage = 0;
    const char* createTableSQL = "CREATE TABLE Emails(Id INT PRIMARY KEY, Email TEXT);";
    if (sqlite3_exec(db, createTableSQL, callback, 0, &errorMessage) != SQLITE_OK) {
        std::cerr << "Error creating table: " << errorMessage << std::endl;
        sqlite3_free(errorMessage);
        return 1;
    }

    // Read input file and insert data
    std::ifstream inFile("input.txt");
    std::string email;
    int id = 1;

    while (getline(inFile, email)) {
        std::string insertSQL = "INSERT INTO Emails(Id, Email) VALUES(" + std::to_string(id++) + ", '" + email + "');";
        if (sqlite3_exec(db, insertSQL.c_str(), callback, 0, &errorMessage) != SQLITE_OK) {
            std::cerr << "Error inserting data: " << errorMessage << std::endl;
            sqlite3_free(errorMessage);
            return 1;
        }
    }
    inFile.close();

    // Delete duplicates
    deleteDuplicates(db);

    // Write to output file
    std::ofstream outFile("output.txt");

    sqlite3_stmt* stmt;
    const char* selectSQL = "SELECT Email FROM Emails ORDER BY Id;";
    if (sqlite3_prepare_v2(db, selectSQL, -1, &stmt, 0) == SQLITE_OK) {
        while (sqlite3_step(stmt) == SQLITE_ROW) {
            outFile << sqlite3_column_text(stmt, 0) << std::endl;
        }
        sqlite3_finalize(stmt);
    } else {
        std::cerr << "Error preparing statement." << std::endl;
    }

    outFile.close();
    sqlite3_close(db);

    return 0;
}
