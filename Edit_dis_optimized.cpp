#include <bits/stdc++.h>
#include <iostream>
#include <string>
#include <vector>
#include <random>
#include <chrono>
#include <immintrin.h>

using namespace std;

class Solution {
public:
    string generateLargeString(int length) {
        string str;
        str.reserve(length);
        static const char charset[] =
            "abcdefghijklmnopqrstuvwxyz";

        static mt19937_64 gen(random_device{}());
        uniform_int_distribution<int> dist(0, sizeof(charset) - 2);

        for (int i = 0; i < length; ++i) {
            str.push_back(charset[dist(gen)]);
        }
        return str;
    }
    
    // space optimization
    int editDistance(const string& str1, const string& str2) {
        int m = str1.size();
        int n = str2.size();
        vector<int> curr(n + 1, 0);

        // Initialize vector for SIMD computation
        __m256i prev = _mm256_set_epi32(0, 1, 2, 3, 4, 5, 6, 7);

        for (int j = 0; j <= n; j++) {
            curr[j] = j;
        }
        for (int i = 1; i <= m; i++) {
            int prev_val = curr[0];
            curr[0] = i;
            __m256i temp = prev;
            for (int j = 1; j <= n; j += 8) {
                __m256i curr_val = _mm256_loadu_si256((__m256i*)&curr[j]);
                __m256i str1_val = _mm256_set1_epi32(str1[i - 1]);
                __m256i str2_val = _mm256_loadu_si256((__m256i*)&str2[j - 1]);

                __m256i cmp = _mm256_cmpeq_epi32(str1_val, str2_val);
                __m256i min_val = _mm256_min_epi32(temp, curr_val);
                min_val = _mm256_min_epi32(min_val, prev);
                prev = curr_val;

                // Update curr using min_val and cmp
                __m256i result = _mm256_blendv_epi8(_mm256_add_epi32(min_val, _mm256_set1_epi32(1)), temp, cmp);
                _mm256_storeu_si256((__m256i*)&curr[j], result);

                temp = curr_val;
            }
            prev = _mm256_set1_epi32(prev_val);
        }
        return curr[n];
    }
};

int main() {
    Solution ob;

    int length1 = 29550; // Adjust the length as needed
    int length2 = 30000;
    string str1 = ob.generateLargeString(length1);
    string str2 = ob.generateLargeString(length2);

    cout << "Length of str1: " << str1.length() << endl;
    cout << "Length of str2: " << str2.length() << endl;

    auto start = chrono::steady_clock::now();
    int ans = ob.editDistance(str1, str2);
    auto end = chrono::steady_clock::now();
    
    chrono::duration<double> diff = end - start;
    cout << "Execution time: " << diff.count() << " seconds" << endl;

    return 0;
}
