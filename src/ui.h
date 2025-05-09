#ifndef UI_H
#define UI_H

#include <vector>
#include <string>
#include <limits>
#include <iostream>

// Hàm hiển thị menu chính
int hienThiMenuChinh();

// Hàm hiển thị menu nhập dữ liệu
int hienThiMenuNhapDuLieu();

// Hàm hiển thị menu chọn phương pháp tìm phương án ban đầu
int hienThiMenuPhuongPhapBanDau();

// Hàm nạp dữ liệu mẫu
bool napDuLieuMau(std::vector<std::vector<int>>& chi_phi, 
                 std::vector<int>& kho_hang, 
                 std::vector<int>& cua_hang);

// Hàm nhập dữ liệu từ bàn phím
bool nhapDuLieuTuBanPhim(std::vector<std::vector<int>>& chi_phi, 
                         std::vector<int>& kho_hang, 
                         std::vector<int>& cua_hang);

// Hàm nhập dữ liệu từ file
bool nhapDuLieuTuFile(std::vector<std::vector<int>>& chi_phi, 
                     std::vector<int>& kho_hang, 
                     std::vector<int>& cua_hang);

#endif // UI_H
