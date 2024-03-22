// ... [previous code]

// Prepare statement for inserting data
const char* insertSQL = "INSERT INTO Emails(Id, Email) VALUES(?, ?);";
sqlite3_stmt* insertStmt;
if (sqlite3_prepare_v2(db, insertSQL, -1, &insertStmt, 0) != SQLITE_OK) {
    std::cerr << "Error preparing insert statement." << std::endl;
    return 1;
}

// Read input file and insert data
std::ifstream inFile("input.txt");
std::string email;
int id = 1;

while (getline(inFile, email)) {
    // Bind parameters
    sqlite3_bind_int(insertStmt, 1, id++);
    sqlite3_bind_text(insertStmt, 2, email.c_str(), -1, SQLITE_STATIC);

    // Execute the statement
    if (sqlite3_step(insertStmt) != SQLITE_DONE) {
        std::cerr << "Error executing insert statement." << std::endl;
        return 1;
    }

    // Reset the prepared statement for the next row of data
    sqlite3_reset(insertStmt);
}
sqlite3_finalize(insertStmt);  // finalize the prepared statement
inFile.close();

// ... [rest of the code]
