#include <iostream>
#include <fstream>
#include <memory>

struct Node {
    int data;
    std::unique_ptr<Node> next;
    Node(int x) : data(x), next(nullptr) {}
};

std::unique_ptr<Node> readLinkedListFromFile(const std::string& filename) {
    std::ifstream infile(filename);
    if (!infile.is_open()) {
        std::cerr << "Failed to open file!" << std::endl;
        return nullptr;
    }

    std::unique_ptr<Node> head = nullptr, tail = nullptr;
    int value;
    size_t count = 0, MAX_NODES = 1000000; // limit the size for safety

    while (infile >> value) {
        if (count >= MAX_NODES) {
            std::cerr << "Input too large!" << std::endl;
            break;
        }

        auto newNode = std::make_unique<Node>(value);
        if (!head) {
            head = std::move(newNode);
            tail = head.get();
        } else {
            tail->next = std::move(newNode);
            tail = tail->next.get();
        }

        count++;
    }

    if (infile.fail() && !infile.eof()) {
        std::cerr << "Unexpected input format!" << std::endl;
        return nullptr;
    }

    infile.close();
    return head;
}

std::unique_ptr<Node> reverseKGroup(Node* head, int k) {
    if (!head || k == 1) return std::unique_ptr<Node>(head);

    auto dummy = std::make_unique<Node>(0);
    dummy->next = std::unique_ptr<Node>(head);

    Node *cur = dummy.get(), *nex = nullptr, *pre = dummy.get();
    size_t count = 0;
    while (cur->next) {
        cur = cur->next.get();
        count++;
    }

    while (count >= static_cast<size_t>(k)) {
        cur = pre->next.get();
        nex = cur->next.get();
        for (int i = 1; i < k; i++) {
            cur->next = std::move(nex->next);
            nex->next = std::move(pre->next);
            pre->next = std::move(nex);
            nex = cur->next.get();
        }
        pre = cur;
        count -= k;
    }
    return std::move(dummy->next);
}

void writeLinkedListToFile(const Node* head, const std::string& filename) {
    std::ofstream outfile(filename);
    if (!outfile.is_open()) {
        std::cerr << "Failed to open file for writing!" << std::endl;
        return;
    }

    while (head) {
        outfile << head->data << " ";
        head = head->next.get();
    }

    outfile.close();
}

int main() {
    std::string inputFilename = "input.txt";
    std::string outputFilename = "output.txt";

    auto head = readLinkedListFromFile(inputFilename);

    int k;
    std::cout << "Enter value of k: ";
    std::cin >> k;

    if (k <= 0) {
        std::cerr << "Invalid k value!" << std::endl;
        return 1;
    }

    auto modifiedHead = reverseKGroup(head.release(), k);
    writeLinkedListToFile(modifiedHead.get(), outputFilename);

    return 0;
}
