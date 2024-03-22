#include <iostream>
#include <fstream>
#include <string>

struct Node {
    int data;
    Node* next;
    Node(int value) : data(value), next(nullptr) {}
};

bool isValidFilepath(const std::string& filename) {
    if (filename.find("..") != std::string::npos || 
        filename.find("/") != std::string::npos ||
        filename.substr(filename.find_last_of(".") + 1) != "txt") {
        return false;
    }
    return true;
}

Node* readListFromFile(const std::string& filename) {
    if (!isValidFilepath(filename)) {
        std::cerr << "Invalid file path or unsupported file type: " << filename << std::endl;
        return nullptr;
    }

    std::ifstream in(filename);
    if (!in) {
        std::cerr << "Cannot open the file: " << filename << std::endl;
        return nullptr;
    }

    int value;
    Node* head = nullptr;
    Node** lastPtrRef = &head;
    std::string line;

    while (std::getline(in, line)) {
        try {
            value = std::stoi(line);
            Node* newNode = new Node(value);
            *lastPtrRef = newNode;
            lastPtrRef = &((*lastPtrRef)->next);
        } catch (const std::exception& e) {
            std::cerr << "Invalid integer data in file: " << filename << std::endl;
            freeListMemory(head);
            return nullptr;
        }
    }

    in.close();
    return head;
}

// ... [Rest of the program remains the same] ...

int main() {
    Node* list1 = readListFromFile("input1.txt");
    Node* list2 = readListFromFile("input2.txt");

    if (!list1 || !list2) {
        std::cerr << "Error processing input files." << std::endl;
        return 1; // Exit with an error code.
    }

    Node* mergedList = mergeSortedLists(list1, list2);
    writeListToFile(mergedList, "output.txt");

    // Free the memory.
    freeListMemory(list1);
    freeListMemory(list2);
    freeListMemory(mergedList);

    return 0;
}
