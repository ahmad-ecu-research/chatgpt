#include <iostream>
#include <fstream>
#include <vector>
#include <climits>

// Kadane's algorithm to find maximum subarray sum
int maxSubArray(std::vector<int>& nums) {
    int n = nums.size();
    int max_current = nums[0];
    int max_global = nums[0];

    for (int i = 1; i < n; i++) {
        max_current = std::max(nums[i], max_current + nums[i]);
        max_global = std::max(max_global, max_current);
    }

    return max_global;
}

int main() {
    // Open the input file
    std::ifstream infile("input.txt");
    if (!infile.is_open()) {
        std::cerr << "Error opening input file!" << std::endl;
        return 1;
    }

    // Read numbers from the file into a vector
    std::vector<int> nums;
    int number;
    while (infile >> number) {
        nums.push_back(number);
    }

    infile.close();

    // Calculate the maximum subarray sum
    int result = maxSubArray(nums);

    // Open the output file
    std::ofstream outfile("output.txt");
    if (!outfile.is_open()) {
        std::cerr << "Error opening output file!" << std::endl;
        return 1;
    }

    // Write the result to the output file
    outfile << result << std::endl;

    outfile.close();

    return 0;
}
