#include "solver.h"
#include "utils.h"
#include "transportationModel.h"
#include "initialSolution.h"
#include "optimization.h"
#include <iostream>

using namespace std;

// Hàm chính để giải bài toán vận tải
KetQuaVanTai giaiVanTai(
    const vector<vector<int>>& chi_phi,
    const vector<int>& kho_hang,
    const vector<int>& cua_hang,
    int phuong_phap_ban_dau
) {
    KetQuaVanTai ket_qua;
    
    // In ma trận gốc (không có phương án) trước khi kiểm tra suy biến
    inMaTranVanTai(chi_phi, kho_hang, cua_hang, "MA TRAN GOC TRUOC KHI KIEM TRA");
    
    // Kiểm tra trường hợp suy biến trước khi tiếp tục
    if (kiemTraSuyBien(chi_phi, kho_hang, cua_hang)) {
        cout << "\nDo bai toan thuoc truong hop suy bien, chuong trinh se ket thuc." << endl;
        return ket_qua; // Trả về kết quả trống
    }
    
    // In ma trận gốc (không có phương án)
    inMaTranVanTai(chi_phi, kho_hang, cua_hang, "MA TRAN GOC TRUOC KHI CAN BANG");
    
    // Kiểm tra điều kiện cân bằng cung-cầu
    int tong_cung = 0, tong_cau = 0;
    for (int i = 0; i < kho_hang.size(); i++) {
        tong_cung += kho_hang[i];
    }
    for (int j = 0; j < cua_hang.size(); j++) {
        tong_cau += cua_hang[j];
    }
    
    // Biến để lưu ma trận chi phí, vector cung, vector cầu sau khi cân bằng
    vector<vector<int>> chi_phi_can_bang = chi_phi;
    vector<int> kho_hang_can_bang = kho_hang;
    vector<int> cua_hang_can_bang = cua_hang;
    vector<string> ten_kho;
    vector<string> ten_cua_hang;
    
    // Thực hiện cân bằng nếu cần
    if (tong_cung != tong_cau) {
        // Gọi hàm cân bằng cung cầu
        tie(chi_phi_can_bang, kho_hang_can_bang, cua_hang_can_bang, ten_kho, ten_cua_hang) = 
            canBangCungCau(chi_phi, kho_hang, cua_hang);
        
        // Xác định loại cân bằng đã thực hiện
        string tieu_de;
        if (kho_hang_can_bang.size() > kho_hang.size()) {
            tieu_de = "MA TRAN SAU KHI CAN BANG (THEM KHO AO)";
        } else {
            tieu_de = "MA TRAN SAU KHI CAN BANG (THEM CUA HANG AO)";
        }
        
        // In ma trận sau khi cân bằng (không có phương án)
        inMaTranVanTai(chi_phi_can_bang, kho_hang_can_bang, cua_hang_can_bang, 
                      tieu_de, {}, ten_kho, ten_cua_hang);
    } else {
        // Nếu đã cân bằng, tạo tên cho các kho và cửa hàng
        for (int i = 0; i < kho_hang.size(); i++) {
            ten_kho.push_back("A" + to_string(i+1));
        }
        
        for (int j = 0; j < cua_hang.size(); j++) {
            ten_cua_hang.push_back("B" + to_string(j+1));
        }
    }
    
    // Lưu dữ liệu sau khi cân bằng vào kết quả
    ket_qua.chi_phi_can_bang = chi_phi_can_bang;
    ket_qua.kho_hang_can_bang = kho_hang_can_bang;
    ket_qua.cua_hang_can_bang = cua_hang_can_bang;
    ket_qua.ten_kho = ten_kho;
    ket_qua.ten_cua_hang = ten_cua_hang;
    
    // Biến để lưu phương án ban đầu và tên phương pháp
    vector<vector<int>> phuong_an;
    string ten_phuong_phap;
    
    // Tìm phương án ban đầu theo phương pháp đã chọn
    switch (phuong_phap_ban_dau) {
        case 1:
            phuong_an = timPhuongAnBanDauGocTayBac(chi_phi_can_bang, kho_hang_can_bang, cua_hang_can_bang);
            ten_phuong_phap = "QUY TAC GOC TAY BAC";
            break;
        case 2:
            phuong_an = timPhuongAnBanDau(chi_phi_can_bang, kho_hang_can_bang, cua_hang_can_bang);
            ten_phuong_phap = "QUY TAC CUOC PHI NHO NHAT";
            break;
        case 3:
            phuong_an = timPhuongAnBanDauVogel(chi_phi_can_bang, kho_hang_can_bang, cua_hang_can_bang);
            ten_phuong_phap = "PHUONG PHAP XAP XI VOGEL";
            break;
        default:
            cout << "Lua chon khong hop le. Su dung quy tac cuoc phi nho nhat." << endl;
            phuong_an = timPhuongAnBanDau(chi_phi_can_bang, kho_hang_can_bang, cua_hang_can_bang);
            ten_phuong_phap = "QUY TAC CUOC PHI NHO NHAT";
    }
    
    // Lưu phương án ban đầu và tên phương pháp vào kết quả
    ket_qua.phuong_an_ban_dau = phuong_an;
    ket_qua.ten_phuong_phap = ten_phuong_phap;
    
    // In ma trận với phương án ban đầu
    inMaTranVanTai(chi_phi_can_bang, kho_hang_can_bang, cua_hang_can_bang, 
                  "PHUONG AN BAN DAU (" + ten_phuong_phap + ")", 
                  phuong_an, ten_kho, ten_cua_hang);
    
    // Tính và hiển thị tổng chi phí vận chuyển ban đầu
    int tong_chi_phi_ban_dau = tinhTongChiPhi(chi_phi_can_bang, phuong_an);
    ket_qua.tong_chi_phi_ban_dau = tong_chi_phi_ban_dau;
    cout << "\nTong chi phi van chuyen ban dau: " << tong_chi_phi_ban_dau << endl;
    
    cout << "\n-------Bat dau tim phuong an toi uu-------" << endl;
    
    // Tìm phương án tối ưu
    vector<vector<int>> phuong_an_toi_uu = timPhuongAnToiUu(chi_phi_can_bang, phuong_an);
    ket_qua.phuong_an_toi_uu = phuong_an_toi_uu;
    
    cout << "\n-------Da tim xong phuong an toi uu-------" << endl;
    
    // In ma trận với phương án tối ưu
    inMaTranVanTai(chi_phi_can_bang, kho_hang_can_bang, cua_hang_can_bang, 
                  "PHUONG AN TOI UU", 
                  phuong_an_toi_uu, ten_kho, ten_cua_hang);
    
    // Tính và hiển thị tổng chi phí vận chuyển tối ưu
    int tong_chi_phi_toi_uu = tinhTongChiPhi(chi_phi_can_bang, phuong_an_toi_uu);
    ket_qua.tong_chi_phi_toi_uu = tong_chi_phi_toi_uu;
    cout << "\nTong chi phi van chuyen toi uu: " << tong_chi_phi_toi_uu << endl;
    
    return ket_qua;
}

// Hàm để hiển thị kết quả giải
void hienThiKetQua(const KetQuaVanTai& ket_qua) {
    // In phương án ban đầu
    inMaTranVanTai(ket_qua.chi_phi_can_bang, ket_qua.kho_hang_can_bang, ket_qua.cua_hang_can_bang, 
                  "PHUONG AN BAN DAU (" + ket_qua.ten_phuong_phap + ")", 
                  ket_qua.phuong_an_ban_dau, ket_qua.ten_kho, ket_qua.ten_cua_hang);
    
    // Hiển thị tổng chi phí vận chuyển ban đầu
    cout << "\nTong chi phi van chuyen ban dau: " << ket_qua.tong_chi_phi_ban_dau << endl;
    
    // In phương án tối ưu
    inMaTranVanTai(ket_qua.chi_phi_can_bang, ket_qua.kho_hang_can_bang, ket_qua.cua_hang_can_bang, 
                  "PHUONG AN TOI UU", 
                  ket_qua.phuong_an_toi_uu, ket_qua.ten_kho, ket_qua.ten_cua_hang);
    
    // Hiển thị tổng chi phí vận chuyển tối ưu
    cout << "\nTong chi phi van chuyen toi uu: " << ket_qua.tong_chi_phi_toi_uu << endl;
}
