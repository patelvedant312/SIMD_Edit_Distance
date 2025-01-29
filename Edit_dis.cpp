#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <random>
#include <chrono>

using namespace std;

class Solution {
public:
    string generateLargeString(int length) {
        string str;
        str.reserve(length);  // Reserve memory for the string to avoid reallocation
        static const char charset[] = "abcdefghijklmnopqrstuvwxyz"; // Define character set

        static mt19937_64 gen(random_device{}());  // Random number generator
        uniform_int_distribution<int> dist(0, sizeof(charset) - 2);  // Distribution for selecting characters

        // Generate random string of specified length using the character set
        for (int i = 0; i < length; ++i) {
            str.push_back(charset[dist(gen)]);
        }
        return str;  // Return the generated string
    }
    
    // Calculate the edit distance between two strings using dynamic programming
    int editDistance(const string& str1, const string& str2) {
        int m = str1.size();  // Length of first string
        int n = str2.size();  // Length of second string
        int previous;  // Store previous value during computation
        vector<int> curr(n + 1, 0);  // Current row of the dynamic programming table

        // Initialize the first row of the table
        for (int j = 0; j <= n; j++) {
            curr[j] = j;
        }

        // Calculate edit distance using dynamic programming
        for (int i = 1; i <= m; i++) {
            previous = curr[0];  // Store the value of the previous cell
            curr[0] = i;  // Update the first cell of the current row
            for (int j = 1; j <= n; j++) {
                int temp = curr[j];  // Store the value of the current cell
                if (str1[i - 1] == str2[j - 1]) {  // Characters match
                 curr[j] = previous;  // Update current cell with previous value
                } else {  // Characters don't match
                // Update current cell with minimum of adjacent cells + 1
                curr[j] = 1 + min(previous, min(curr[j - 1], curr[j]));
                }
                previous = temp;  // Update previous value for next iteration
            }
        }

        return curr[n];  // Return the edit distance
    }
};

int main() {
    Solution ob;

    int length1 = 29550; // Adjust the length as needed
    int length2 = 30000;
    string str1 = ob.generateLargeString(length1);  // Generate random string
    string str2 = ob.generateLargeString(length2);

    // Print lengths of the generated strings
    cout << "Length of str1: " << str1.length() << endl;
    cout << "Length of str2: " << str2.length() << endl;

    auto start = chrono::steady_clock::now();  // Start measuring execution time
    int ans = ob.editDistance(str1, str2);  // Calculate edit distance
    auto end = chrono::steady_clock::now();  // Stop measuring execution time
    
    // Calculate and print execution time
    chrono::duration<double> diff = end - start;
    cout << "Execution time: " << diff.count() << " seconds" << endl;

    return 0;  // Return 0 to indicate successful completion
}
