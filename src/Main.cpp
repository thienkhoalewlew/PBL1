// filepath: d:\PBL1\src\Main.cpp
#include <iostream>
#include <vector>
#include <string>
#include <tuple>
#include <limits>

#include "utils.h"
#include "transportationModel.h"
#include "initialSolution.h"
#include "optimization.h"
#include "ui.h"
#include "solver.h"

using namespace std;

int main() {
    vector<int> kho_hang;
    vector<int> cua_hang;
    vector<vector<int>> chi_phi;
    
    // Hiển thị menu chính
    int lua_chon_menu = hienThiMenuChinh();
    
    // Xử lý lựa chọn từ menu chính
    if (lua_chon_menu == 3) {
        cout << "Cam on ban da su dung chuong trinh!\n";
        return 0;
    } else if (lua_chon_menu == 2) {
        // Sử dụng dữ liệu mẫu
        if (!napDuLieuMau(chi_phi, kho_hang, cua_hang)) {
            cout << "Loi khi nap du lieu mau. Chuong trinh ket thuc.\n";
            return 1;
        }
    } else {
        // Cho người dùng chọn cách nhập dữ liệu
        int lua_chon_nhap = hienThiMenuNhapDuLieu();
        
        if (lua_chon_nhap == 2) {
            // Nhập từ file
            if (!nhapDuLieuTuFile(chi_phi, kho_hang, cua_hang)) {
                return 1;
            }
        } else {
            // Nhập từ bàn phím
            if (!nhapDuLieuTuBanPhim(chi_phi, kho_hang, cua_hang)) {
                return 1;
            }
        }
    }

    // Cho phép người dùng chọn phương pháp tìm phương án ban đầu
    int lua_chon_phuong_phap = hienThiMenuPhuongPhapBanDau();
    
    // Giải bài toán vận tải
    KetQuaVanTai ket_qua = giaiVanTai(chi_phi, kho_hang, cua_hang, lua_chon_phuong_phap);
    
    return 0;
}
