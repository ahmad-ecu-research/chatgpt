#include <iostream>
#include <fstream>

struct Node {
    int data;
    Node* next;
    Node(int value) : data(value), next(nullptr) {}
};

Node* mergeSortedLists(Node* l1, Node* l2) {
    Node dummy(0);
    Node* tail = &dummy;

    while (l1 != nullptr && l2 != nullptr) {
        if (l1->data < l2->data) {
            tail->next = l1;
            l1 = l1->next;
        } else {
            tail->next = l2;
            l2 = l2->next;
        }
        tail = tail->next;
    }

    if (l1 != nullptr) tail->next = l1;
    if (l2 != nullptr) tail->next = l2;

    return dummy.next;
}

Node* readListFromFile(const std::string& filename) {
    std::ifstream in(filename);
    if (!in) {
        std::cerr << "Cannot open the File: " << filename << std::endl;
        return nullptr;
    }

    int value;
    Node* head = nullptr;
    Node** lastPtrRef = &head;

    while (in >> value) {
        Node* newNode = new Node(value);
        *lastPtrRef = newNode;
        lastPtrRef = &((*lastPtrRef)->next);
    }

    in.close();
    return head;
}

void writeListToFile(Node* head, const std::string& filename) {
    std::ofstream out(filename);
    if (!out) {
        std::cerr << "Cannot open the File: " << filename << std::endl;
        return;
    }

    while (head != nullptr) {
        out << head->data << " ";
        head = head->next;
    }

    out.close();
}

int main() {
    Node* list1 = readListFromFile("input1.txt");
    Node* list2 = readListFromFile("input2.txt");

    Node* mergedList = mergeSortedLists(list1, list2);
    writeListToFile(mergedList, "output.txt");

    // Free the memory (optional, as OS will reclaim it at program end).
    while (mergedList != nullptr) {
        Node* next = mergedList->next;
        delete mergedList;
        mergedList = next;
    }

    return 0;
}
