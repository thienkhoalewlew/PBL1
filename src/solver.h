#ifndef SOLVER_H
#define SOLVER_H

#include <vector>
#include <string>
#include <tuple>

// Struct để lưu trữ kết quả giải bài toán vận tải
struct KetQuaVanTai {
    // Chi phí ban đầu và tối ưu
    int tong_chi_phi_ban_dau;
    int tong_chi_phi_toi_uu;
    
    // Phương án ban đầu và tối ưu
    std::vector<std::vector<int>> phuong_an_ban_dau;
    std::vector<std::vector<int>> phuong_an_toi_uu;
    
    // Dữ liệu ma trận sau khi cân bằng
    std::vector<std::vector<int>> chi_phi_can_bang;
    std::vector<int> kho_hang_can_bang;
    std::vector<int> cua_hang_can_bang;
    
    // Tên các kho và cửa hàng
    std::vector<std::string> ten_kho;
    std::vector<std::string> ten_cua_hang;
    
    // Tên phương pháp tìm phương án ban đầu
    std::string ten_phuong_phap;
};

// Hàm chính để giải bài toán vận tải
// Nó sẽ xử lý tất cả các bước từ kiểm tra suy biến, cân bằng,
// tìm phương án ban đầu và tối ưu hóa
KetQuaVanTai giaiVanTai(
    const std::vector<std::vector<int>>& chi_phi,
    const std::vector<int>& kho_hang,
    const std::vector<int>& cua_hang,
    int phuong_phap_ban_dau
);

// Hàm để hiển thị kết quả giải
void hienThiKetQua(const KetQuaVanTai& ket_qua);

#endif // SOLVER_H
