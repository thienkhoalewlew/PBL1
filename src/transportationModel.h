#ifndef TRANSPORTATION_MODEL_H
#define TRANSPORTATION_MODEL_H

#include <vector>
#include <string>
#include <tuple>

// Hàm kiểm tra trường hợp suy biến
bool kiemTraSuyBien(const std::vector<std::vector<int>>& chi_phi, 
                    const std::vector<int>& kho_hang, 
                    const std::vector<int>& cua_hang);

// Hàm cân bằng cung cầu
// Trả về tuple gồm: ma trận chi phí, vector cung, vector cầu, tên kho, tên cửa hàng sau khi cân bằng
std::tuple<std::vector<std::vector<int>>, std::vector<int>, std::vector<int>, std::vector<std::string>, std::vector<std::string>> 
canBangCungCau(const std::vector<std::vector<int>>& chi_phi, 
               const std::vector<int>& kho_hang, 
               const std::vector<int>& cua_hang);

// Kiểm tra liệu một phương án có phải là phương án cơ sở không
bool laPhuongAnCoBan(const std::vector<std::vector<int>>& phuong_an);

// Thêm biến cơ sở bằng 0 nếu cần
std::vector<std::vector<int>> themBienCoSo(const std::vector<std::vector<int>>& chi_phi, 
                                          std::vector<std::vector<int>> phuong_an);

#endif // TRANSPORTATION_MODEL_H
