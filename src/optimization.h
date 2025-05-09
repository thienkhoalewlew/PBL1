#ifndef OPTIMIZATION_H
#define OPTIMIZATION_H

#include <vector>
#include <utility>

// Hàm tìm chu trình trong phương án để cải tiến
std::vector<std::pair<int, int>> timChuTrinh(
    const std::vector<std::vector<int>>& phuong_an, 
    int start_i, 
    int start_j);

// Hàm tìm phương án tối ưu bằng phương pháp MODI
std::vector<std::vector<int>> timPhuongAnToiUu(
    const std::vector<std::vector<int>>& chi_phi, 
    std::vector<std::vector<int>> phuong_an);

#endif // OPTIMIZATION_H
