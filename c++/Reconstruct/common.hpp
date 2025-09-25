#pragma once 

#include <vector>
#include <numeric>
#include <algorithm>

float* flattenVector(const std::vector<std::vector<float>>& vec, size_t& total_size) {
    total_size = std::accumulate(vec.begin(), vec.end(), 0ull,
        [](size_t sum, const auto& row) { return sum + row.size(); });
    
    if (total_size == 0) return nullptr;
    
    float* result = new float[total_size];
    float* current = result;
    
    for (const auto& row : vec) {
        current = std::copy(row.begin(), row.end(), current);
    }
    
    return result;
}

float** convertVectorToFloatPtr(const std::vector<std::vector<float>>& vec) {
    if (vec.empty()) return nullptr;
    
    // Allocate memory for row pointers
    float** result = new float*[vec.size()];
    
    for (size_t i = 0; i < vec.size(); ++i) {
        if (vec[i].empty()) {
            result[i] = nullptr;
            continue;
        }
        
        // Allocate memory for each row and copy data
        result[i] = new float[vec[i].size()];
        std::copy(vec[i].begin(), vec[i].end(), result[i]);
    }
    
    return result;
}