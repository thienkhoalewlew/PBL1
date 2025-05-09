#ifndef UTILS_H
#define UTILS_H

#include <iostream>
#include <vector>
#include <iomanip>
#include <string>
#include <fstream>

// Hàm in ma trận vận tải
void inMaTranVanTai(const std::vector<std::vector<int>>& chi_phi, 
                    const std::vector<int>& kho_hang, 
                    const std::vector<int>& cua_hang,
                    const std::string& tieu_de,
                    const std::vector<std::vector<int>>& phuong_an = {},
                    const std::vector<std::string>& ten_kho = {},
                    const std::vector<std::string>& ten_cua_hang = {});

// Hàm đọc ma trận vận tải từ file
// Return: true nếu đọc thành công, false nếu có lỗi
// Format file: 
// - Dòng 1: số kho hàng (m) và số cửa hàng (n)
// - Dòng 2 đến m+1: lượng hàng của từng kho
// - Dòng m+2 đến m+n+1: nhu cầu của từng cửa hàng
// - Các dòng tiếp theo: ma trận chi phí (m x n)
bool docMaTranTuFile(const std::string& ten_file, 
                     std::vector<std::vector<int>>& chi_phi, 
                     std::vector<int>& kho_hang, 
                     std::vector<int>& cua_hang);

// Hàm tính tổng chi phí vận chuyển
int tinhTongChiPhi(const std::vector<std::vector<int>>& chi_phi, 
                  const std::vector<std::vector<int>>& phuong_an);

#endif // UTILS_H
