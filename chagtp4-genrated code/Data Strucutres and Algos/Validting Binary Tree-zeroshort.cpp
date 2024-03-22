#include <iostream>
#include <fstream>
#include <vector>
#include <sstream>
#include <queue>
#include <limits.h>

using namespace std;

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
    getline(fin, line);
    stringstream ss(line);
    int rootVal;
    ss >> rootVal;
    
    if (rootVal == -1) return nullptr;
    
    TreeNode* root = new TreeNode(rootVal);
    queue<TreeNode*> q;
    q.push(root);

    while (getline(fin, line)) {
        stringstream ss(line);
        int n = q.size();
        for (int i = 0; i < n; ++i) {
            TreeNode* curr = q.front();
            q.pop();
            
            int leftVal, rightVal;
            ss >> leftVal;
            if (leftVal != -1) {
                curr->left = new TreeNode(leftVal);
                q.push(curr->left);
            }

            ss >> rightVal;
            if (rightVal != -1) {
                curr->right = new TreeNode(rightVal);
                q.push(curr->right);
            }
        }
    }
    fin.close();
    return root;
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

int main() {
    TreeNode* root = readTreeFromFile("input.txt");
    bool valid = isValidBST(root);
    writeResultToFile("output.txt", valid);

    return 0;
}
