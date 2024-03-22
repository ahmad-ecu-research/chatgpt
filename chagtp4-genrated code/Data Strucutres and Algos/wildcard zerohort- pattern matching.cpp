#include <iostream>
#include <fstream>
#include <vector>
#include <string>

bool isMatch(const std::string &s, const std::string &p) {
    int m = s.length(), n = p.length();
    std::vector<std::vector<bool>> dp(m+1, std::vector<bool>(n+1, false));

    dp[0][0] = true;
    for (int j = 1; j <= n; j++) {
        if (p[j-1] == '*') {
            dp[0][j] = dp[0][j-1];
        }
    }

    for (int i = 1; i <= m; i++) {
        for (int j = 1; j <= n; j++) {
            if (p[j-1] == '*') {
                dp[i][j] = dp[i-1][j] || dp[i][j-1];
            } else if (p[j-1] == '?' || s[i-1] == p[j-1]) {
                dp[i][j] = dp[i-1][j-1];
            }
        }
    }
    
    return dp[m][n];
}

int main() {
    std::ifstream inputFile("input.txt");
    std::ofstream outputFile("output.txt");

    if (!inputFile || !outputFile) {
        std::cerr << "Error opening files!" << std::endl;
        return 1;
    }

    int t;
    inputFile >> t;
    inputFile.ignore();  // consume newline

    while (t--) {
        std::string text, pattern;
        std::getline(inputFile, text);
        std::getline(inputFile, pattern);

        bool result = isMatch(text, pattern);
        outputFile << (result ? "true" : "false") << std::endl;
    }

    inputFile.close();
    outputFile.close();
    return 0;
}
