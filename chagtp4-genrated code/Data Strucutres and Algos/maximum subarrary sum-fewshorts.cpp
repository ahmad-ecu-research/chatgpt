#include <iostream>
#include <fstream>
#include <vector>
#include <climits>
#include <string>

// Kadane's algorithm to find maximum subarray sum
int maxSubArray(const std::vector<int>& nums) {
    int n = nums.size();
    int max_current = nums[0];
    int max_global = nums[0];

    for (int i = 1; i < n; i++) {
        max_current = std::max(nums[i], max_current + nums[i]);
        max_global = std::max(max_global, max_current);
    }

    return max_global;
}

int main(int argc, char* argv[]) {
    if (argc != 3) {
        std::cerr << "Usage: " << argv[0] << " <input_file> <output_file>" << std::endl;
        return 1;
    }

    std::string inputFileName = argv[1];
    std::string outputFileName = argv[2];

    std::vector<int> nums;

    // Use a code block to ensure RAII for infile
    {
        std::ifstream infile(inputFileName);
        if (!infile) {
            std::cerr << "Error opening input file!" << std::endl;
            return 1;
        }

        int number;
        std::string line;
        while (std::getline(infile, line)) {
            std::istringstream iss(line);
            if (!(iss >> number)) {
                std::cerr << "Invalid integer data encountered!" << std::endl;
                return 1;
            }
            nums.push_back(number);
        }

        // Check for an empty input file
        if (nums.empty()) {
            std::cerr << "Input file is empty!" << std::endl;
            return 1;
        }

        // infile will be automatically closed when going out of scope
    }

    // Calculate the maximum subarray sum
    int result = maxSubArray(nums);

    // Use a code block to ensure RAII for outfile
    {
        std::ofstream outfile(outputFileName);
        if (!outfile) {
            std::cerr << "Error opening output file!" << std::endl;
            return 1;
        }

        // Write the result to the output file
        outfile << result << std::endl;

        // outfile will be automatically closed when going out of scope
    }

    return 0;
}
