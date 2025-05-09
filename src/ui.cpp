#include "ui.h"
#include "utils.h"
#include <iostream>
#include <limits>
#include <string>

using namespace std;

// Hàm hiển thị menu chính
int hienThiMenuChinh() {
    int lua_chon;
    
    cout << "\n==== CHUONG TRINH GIAI BAI TOAN VAN TAI ====\n";
    cout << "1. Nhap du lieu moi\n";
    cout << "2. Su dung du lieu mau\n";
    cout << "3. Thoat chuong trinh\n";
    cout << "Nhap lua chon cua ban (1-3): ";
    
    // Đảm bảo người dùng nhập số nguyên hợp lệ
    while (!(cin >> lua_chon) || lua_chon < 1 || lua_chon > 3) {
        cin.clear(); // Xóa trạng thái lỗi
        cin.ignore(numeric_limits<streamsize>::max(), '\n'); // Bỏ qua dữ liệu không hợp lệ
        cout << "Lua chon khong hop le. Vui long nhap lai (1-3): ";
    }
    
    return lua_chon;
}

// Hàm hiển thị menu nhập dữ liệu
int hienThiMenuNhapDuLieu() {
    int lua_chon;
    
    cout << "\n=== CHON CACH NHAP DU LIEU ===\n";
    cout << "1. Nhap tu ban phim\n";
    cout << "2. Doc tu file\n";
    cout << "Nhap lua chon cua ban (1-2): ";
    
    while (!(cin >> lua_chon) || lua_chon < 1 || lua_chon > 2) {
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        cout << "Lua chon khong hop le. Vui long nhap lai (1-2): ";
    }
    
    return lua_chon;
}

// Hàm hiển thị menu chọn phương pháp tìm phương án ban đầu
int hienThiMenuPhuongPhapBanDau() {
    int lua_chon;
    
    cout << "\n=== CHON PHUONG PHAP TIM PHUONG AN BAN DAU ===" << endl;
    cout << "1. Quy tac goc Tay Bac" << endl;
    cout << "2. Quy tac cuoc phi nho nhat" << endl;
    cout << "3. Phuong phap xap xi Vogel" << endl;
    cout << "Nhap lua chon cua ban (1-3): ";
    
    while (!(cin >> lua_chon) || lua_chon < 1 || lua_chon > 3) {
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        cout << "Lua chon khong hop le. Vui long nhap lai (1-3): ";
    }
    
    return lua_chon;
}

// Hàm nạp dữ liệu mẫu
bool napDuLieuMau(vector<vector<int>>& chi_phi, vector<int>& kho_hang, vector<int>& cua_hang) {
    // Dữ liệu mẫu đơn giản: 2 kho, 3 cửa hàng
    kho_hang = {20, 30};  // Cung
    cua_hang = {10, 28, 12};  // Cầu
    
    // Ma trận chi phí vận chuyển
    chi_phi = {
        {2, 3, 4},
        {5, 1, 3}
    };
    
    cout << "Da nap du lieu mau: 2 kho hang, 3 cua hang\n";
    return true;
}

// Hàm nhập dữ liệu từ bàn phím
bool nhapDuLieuTuBanPhim(vector<vector<int>>& chi_phi, vector<int>& kho_hang, vector<int>& cua_hang) {
    int kho, shop;
    
    cout << "Nhap so kho hang: ";
    cin >> kho;

    cout << "Nhap so cua hang: ";
    cin >> shop;

    kho_hang.resize(kho);
    cua_hang.resize(shop);

    // Nhập lượng hàng của kho hàng
    for (int i = 1; i <= kho; i++) {
        cout << "Nhap luong hang cua kho hang A" << i << ": ";
        cin >> kho_hang[i-1];
    }

    // Nhập lượng hàng của cửa hàng
    for (int i = 1; i <= shop; i++) {
        cout << "Nhap luong hang cua cua hang B" << i << ": ";
        cin >> cua_hang[i-1];
    }

    chi_phi.resize(kho, vector<int>(shop));

    // Nhập chi phí vận chuyển từ kho hàng đến cửa hàng
    for (int i = 1; i <= kho; i++) {
        for (int j = 1; j <= shop; j++) {
            cout << "Nhap chi phi van chuyen tu kho hang A" << i << " den cua hang B" << j << ": ";
            cin >> chi_phi[i-1][j-1];
        }
    }
    
    return true;
}

// Hàm nhập dữ liệu từ file
bool nhapDuLieuTuFile(vector<vector<int>>& chi_phi, vector<int>& kho_hang, vector<int>& cua_hang) {
    string ten_file;
    cout << "Nhap ten file (vd: input.txt): ";
    cin >> ten_file;
    
    if (!docMaTranTuFile(ten_file, chi_phi, kho_hang, cua_hang)) {
        cout << "Khong the doc du lieu tu file. Chuong trinh ket thuc." << endl;
        return false;
    }
    
    cout << "Doc du lieu tu file thanh cong!" << endl;
    cout << "So kho hang: " << kho_hang.size() << endl;
    cout << "So cua hang: " << cua_hang.size() << endl;
    
    return true;
}
