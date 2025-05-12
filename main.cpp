#include <iostream>
#include <vector>
#include <string>
#include <map>
#include <pthread.h>
#include <sstream>
#include <mutex>
#include <algorithm>
#include "test_cases.cpp"
#include <sstream>
#include <set>
using namespace std;


struct KeyValue {
    string key;
    int value;
};

// Helper function to parse output into a set of strings
set<string> parse_output(const string& output) {
    stringstream ss(output);
    set<string> parsed_output;
    string line;
    while (getline(ss, line)) {
        if (!line.empty()) {
            parsed_output.insert(line);
        }
    }
    return parsed_output;
}

// Mutexes for synchronizing access to shared data
mutex map_mutex, shuffle_mutex, reduce_mutex;

// Function to split input string into words
vector<string> process_words(const string& input) {
    vector<string> words;
    stringstream ss(input);
    string wd;
    while (ss >> wd) {
        words.push_back(wd);
    }
    return words;
}

// Map function for each thread
void* m_func(void* arg) {
    auto data = static_cast<vector<string>*>(arg);
    vector<KeyValue>* map_output = new vector<KeyValue>;
    
    for (const auto& word : *data) {
        map_output->push_back({word, 1});
    }

    return map_output;
}

// Shuffle function to group by key
void s_func(const vector<KeyValue>& map_output, map<string, vector<int>>& s_opt) {
    for (const auto& kv : map_output) {
        lock_guard<mutex> guard(shuffle_mutex);
        s_opt[kv.key].push_back(kv.value);
    }
}

// Reduce function for each thread
void* rd_func(void* arg) {
    auto data = static_cast<pair<string, vector<int>>*>(arg);
    int sum = 0;

    for (int value : data->second) {
        sum += value;
    }

    auto* result = new pair<string, int>(data->first, sum);
    return result;
}

// Function to display map phase output
void show_m_opt(const vector<KeyValue>& map_output) {
    cout << "Map Phase Output:" << endl;
    for (const auto& kv : map_output) {
        cout << "(" << kv.key << ", " << kv.value << ")" << endl;
    }
}

// Function to display shuffle phase output
void show_s_opt(const map<string, vector<int>>& s_opt) {
    cout << "Shuffle Phase Output:" << endl;
    for (const auto& kv : s_opt) {
        cout << "(" << kv.first << ", [";
        for (size_t i = 0; i < kv.second.size(); ++i) {
            cout << kv.second[i];
            if (i < kv.second.size() - 1) cout << ", ";
        }
        cout << "])" << endl;
    }
}

// Function to display reduce phase output
void show_r_opt(const vector<pair<string, int>>& reduce_output) {
    cout << "Reduce Phase Output:" << endl;
    for (const auto& kv : reduce_output) {
        cout << "(" << kv.first << ", " << kv.second << ")" << endl;
    }
}

void run_test_cases() {
    for (size_t i = 0; i < test_cases.size(); ++i) {
        cout << "Running Test Case " << i + 1 << "..." << endl;

        // Extract test case input and expected output
        string input = test_cases[i].input;
        string expected_output = test_cases[i].expected_output;

        // Process the input
        vector<string> words = process_words(input);

        // Map Phase
        int m_threads = (count(input.begin(), input.end(), ' ') + 1) / 3;
        m_threads = max(1, m_threads); // Ensure at least 1 thread

        int chunk_size = (words.size() + m_threads - 1) / m_threads; // Handle uneven splits
        pthread_t map_threads[m_threads];
        vector<vector<KeyValue>*> map_output_ptrs(m_threads);

        vector<KeyValue> t_m_output;
        for (int t = 0; t < m_threads; ++t) {
            size_t start = t * chunk_size;
            size_t end = min(start + chunk_size, words.size());
            vector<string>* chunk = new vector<string>(words.begin() + start, words.begin() + end);
            pthread_create(&map_threads[t], NULL, m_func, chunk);
        }

        for (int t = 0; t < m_threads; ++t) {
            void* result;
            pthread_join(map_threads[t], &result);
            vector<KeyValue>* map_output = static_cast<vector<KeyValue>*>(result);
            t_m_output.insert(t_m_output.end(), map_output->begin(), map_output->end());
            delete map_output;
        }

        // Shuffle Phase
        map<string, vector<int>> s_opt;
        s_func(t_m_output, s_opt);

        // Reduce Phase
        int total_r_threads = s_opt.size();
        pthread_t reduce_threads[total_r_threads];
        vector<pair<string, int>> reduce_results(total_r_threads);
        vector<pair<string, vector<int>>> shuffle_entries(s_opt.begin(), s_opt.end());

        for (int t = 0; t < total_r_threads; ++t) {
            pthread_create(&reduce_threads[t], NULL, rd_func, &shuffle_entries[t]);
        }

        for (int t = 0; t < total_r_threads; ++t) {
            void* result;
            pthread_join(reduce_threads[t], &result);
            pair<string, int>* reduce_output = static_cast<pair<string, int>*>(result);
            reduce_results[t] = *reduce_output;
            delete reduce_output;
        }

        // Convert output to string for comparison
        stringstream actual_output_ss;
        for (const auto& kv : reduce_results) {
            actual_output_ss << "(" << kv.first << ", " << kv.second << ")" << endl;
        }
        string actual_output = actual_output_ss.str();

        // Parse and compare outputs (unordered)
        auto expected_parsed = parse_output(expected_output);
        auto actual_parsed = parse_output(actual_output);

        if (expected_parsed == actual_parsed) {
            cout << "Test Case " << i + 1 << " PASSED!" << endl;
        } else {
            cout << "Test Case " << i + 1 << " FAILED!" << endl;
            cout << "Expected:\n" << expected_output;
            cout << "Got:\n" << actual_output;
        }
        cout << "----------------------------------" << endl;
    }
}

int main() {
     run_test_cases();
    return 0;
}

