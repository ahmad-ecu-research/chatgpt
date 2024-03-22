#include <iostream>
#include <fstream>
#include <vector>
#include <sstream>
#include <queue>
#include <limits.h>
#include <filesystem>

using namespace std;
namespace fs = std::filesystem;

struct TreeNode {
    int val;
    TreeNode* left;
    TreeNode* right;
    TreeNode(int x) : val(x), left(NULL), right(NULL) {}
};

bool isValidBST(TreeNode* root, long long minVal = LONG_MIN, long long maxVal = LONG_MAX) {
    if (!root) return true;
    if (root->val <= minVal || root->val >= maxVal) return false;
    return isValidBST(root->left, minVal, root->val) && isValidBST(root->right, root->val, maxVal);
}

TreeNode* readTreeFromFile(const string &filename) {
    ifstream fin(filename);
    if (!fin) {
        cerr << "Failed to open input file." << endl;
        return nullptr;
    }
    
    string line;
    if (!getline(fin, line)) {
        cerr << "Empty file or read error." << endl;
        return nullptr;
    }
    stringstream ss(line);
    int rootVal;
    if (!(ss >> rootVal)) {
        cerr << "Invalid root value." << endl;
        return nullptr;
    }
    
    if (rootVal == -1) return nullptr;
    
    TreeNode* root = new TreeNode(rootVal);
    queue<TreeNode*> q;
    q.push(root);

    while (getline(fin, line)) {
        stringstream ss(line);
        int n = q.size();
        if (n == 0) {
            cerr << "Malformed input." << endl;
            deleteTree(root);
            return nullptr;
        }

        for (int i = 0; i < n; ++i) {
            TreeNode* curr = q.front();
            q.pop();
            
            int leftVal, rightVal;
            if (!(ss >> leftVal) || !(ss >> rightVal)) {
                cerr << "Malformed input." << endl;
                deleteTree(root);
                return nullptr;
            }

            if (leftVal != -1) {
                curr->left = new TreeNode(leftVal);
                q.push(curr->left);
            }

            if (rightVal != -1) {
                curr->right = new TreeNode(rightVal);
                q.push(curr->right);
            }
        }
    }
    fin.close();
    return root;
}

void deleteTree(TreeNode* root) {
    if (!root) return;
    deleteTree(root->left);
    deleteTree(root->right);
    delete root;
}

void writeResultToFile(const string &filename, bool isValid) {
    ofstream fout(filename);
    if (!fout) {
        cerr << "Failed to open output file." << endl;
        return;
    }
    fout << (isValid ? "YES" : "NO") << endl;
    fout.close();
}

// Validate and sanitize file paths to avoid directory traversal attacks.
string sanitizePath(const string &path) {
    return fs::canonical(fs::path(path)).string();
}

int main() {
    string inputPath, outputPath;

    cout << "Enter the path for the input file: ";
    cin >> inputPath;
    inputPath = sanitizePath(inputPath);

    cout << "Enter the path for the output file: ";
    cin >> outputPath;
    outputPath = sanitizePath(outputPath);

    TreeNode* root = readTreeFromFile(inputPath);
    if (root) {
        bool valid = isValidBST(root);
        writeResultToFile(outputPath, valid);
        deleteTree(root);
    }

    return 0;
}
