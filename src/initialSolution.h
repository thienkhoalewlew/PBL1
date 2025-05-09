#ifndef INITIAL_SOLUTION_H
#define INITIAL_SOLUTION_H

#include <vector>

// Hàm tìm phương án ban đầu sử dụng quy tắc góc Tây Bắc
std::vector<std::vector<int>> timPhuongAnBanDauGocTayBac(
    const std::vector<std::vector<int>>& chi_phi, 
    std::vector<int> kho_hang, 
    std::vector<int> cua_hang);

// Hàm tìm phương án ban đầu với quy tắc cước phí nhỏ nhất
std::vector<std::vector<int>> timPhuongAnBanDau(
    const std::vector<std::vector<int>>& chi_phi, 
    std::vector<int> kho_hang, 
    std::vector<int> cua_hang);

// Hàm tìm phương án ban đầu bằng phương pháp xấp xỉ Vogel
std::vector<std::vector<int>> timPhuongAnBanDauVogel(
    const std::vector<std::vector<int>>& chi_phi, 
    std::vector<int> kho_hang, 
    std::vector<int> cua_hang);

#endif // INITIAL_SOLUTION_H
