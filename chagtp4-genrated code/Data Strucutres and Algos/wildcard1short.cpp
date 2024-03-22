#include <iostream>
#include <fstream>
#include <sqlite3.h>
#include <cctype>
#include <cryptopp/aes.h>
#include <cryptopp/filters.h>
#include <cryptopp/modes.h>
#include <cryptopp/hex.h>

// AES key for Encryption and Decryption
const byte AES_KEY[] = { ... };  // Your AES key here
const byte AES_IV[] = { ... };   // Your AES Initialization Vector

std::string transformWildcard(const std::string &pattern) {
    // ... [same as before]
}

std::string decrypt(const std::string &encryptedText) {
    std::string decryptedText;
    CryptoPP::AES::Decryption aesDecryption(AES_KEY, CryptoPP::AES::DEFAULT_KEYLENGTH);
    CryptoPP::CBC_Mode_ExternalCipher::Decryption cbcDecryption(aesDecryption, AES_IV);
    
    CryptoPP::StreamTransformationFilter stfDecryptor(cbcDecryption, new CryptoPP::StringSink(decryptedText));
    stfDecryptor.Put(reinterpret_cast<const unsigned char*>(encryptedText.c_str()), encryptedText.size());
    stfDecryptor.MessageEnd();
    
    return decryptedText;
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

    if (!isValidPattern(pattern)) {
        std::cerr << "Invalid search pattern!" << std::endl;
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
        std::string encryptedData = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 0));
        std::cout << decrypt(encryptedData) << std::endl;
    }

    sqlite3_finalize(stmt);
    sqlite3_close(db);
    return 0;
}
