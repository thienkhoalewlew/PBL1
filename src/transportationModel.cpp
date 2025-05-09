#include "transportationModel.h"
#include <iostream>
#include <climits>

using namespace std;

// Hàm kiểm tra trường hợp suy biến
bool kiemTraSuyBien(const vector<vector<int>>& chi_phi, const vector<int>& kho_hang, const vector<int>& cua_hang) {
    int m = kho_hang.size();    // Số kho
    int n = cua_hang.size();    // Số cửa hàng
    
    // Kiểm tra trường hợp suy biến 1: Số hàng hoặc cột bằng 0
    if (m == 0 || n == 0) {
        cout << "\nBai toan thuoc truong hop suy bien: Khong co kho hang hoac cua hang." << endl;
        return true;
    }
    
    // Kiểm tra trường hợp suy biến 2: Có kho hoặc cửa hàng với lượng hàng bằng 0
    for (int i = 0; i < m; i++) {
        if (kho_hang[i] <= 0) {
            cout << "\nBai toan thuoc truong hop suy bien: Kho hang A" << i+1 << " co luong hang bang hoac nho hon 0." << endl;
            return true;
        }
    }
    
    for (int j = 0; j < n; j++) {
        if (cua_hang[j] <= 0) {
            cout << "\nBai toan thuoc truong hop suy bien: Cua hang B" << j+1 << " co nhu cau bang hoac nho hon 0." << endl;
            return true;
        }
    }
    
    // Kiểm tra trường hợp suy biến 3: Tổng cung bằng 0 hoặc tổng cầu bằng 0
    int tong_cung = 0, tong_cau = 0;
    for (int i = 0; i < m; i++) {
        tong_cung += kho_hang[i];
    }
    
    for (int j = 0; j < n; j++) {
        tong_cau += cua_hang[j];
    }
    
    if (tong_cung <= 0 || tong_cau <= 0) {
        cout << "\nBai toan thuoc truong hop suy bien: Tong cung hoac tong cau bang hoac nho hon 0." << endl;
        return true;
    }
    
    // Kiểm tra trường hợp suy biến 4: Ma trận chi phí không hợp lệ (có giá trị âm)
    for (int i = 0; i < m; i++) {
        for (int j = 0; j < n; j++) {
            if (chi_phi[i][j] < 0) {
                cout << "\nBai toan thuoc truong hop suy bien: Chi phi van chuyen am tai [A" << i+1 << ", B" << j+1 << "]." << endl;
                return true;
            }
        }
    }
    
    return false;  // Không phải trường hợp suy biến
}

// Hàm cân bằng cung cầu
tuple<vector<vector<int>>, vector<int>, vector<int>, vector<string>, vector<string>> 
canBangCungCau(const vector<vector<int>>& chi_phi, const vector<int>& kho_hang, const vector<int>& cua_hang) {
    
    // Tạo bản sao của dữ liệu gốc
    vector<vector<int>> chi_phi_can_bang = chi_phi;
    vector<int> kho_hang_can_bang = kho_hang;
    vector<int> cua_hang_can_bang = cua_hang;
    
    // Tạo tên cho các kho và cửa hàng
    vector<string> ten_kho;
    vector<string> ten_cua_hang;
    
    for (int i = 0; i < kho_hang.size(); i++) {
        ten_kho.push_back("A" + to_string(i+1));
    }
    
    for (int j = 0; j < cua_hang.size(); j++) {
        ten_cua_hang.push_back("B" + to_string(j+1));
    }
    
    // Tính tổng cung và cầu
    int tong_cung = 0, tong_cau = 0;
    for (int i = 0; i < kho_hang.size(); i++) {
        tong_cung += kho_hang[i];
    }
    for (int j = 0; j < cua_hang.size(); j++) {
        tong_cau += cua_hang[j];
    }
    
    // Nếu đã cân bằng, không cần điều chỉnh
    if (tong_cung == tong_cau) {
        return make_tuple(chi_phi_can_bang, kho_hang_can_bang, cua_hang_can_bang, ten_kho, ten_cua_hang);
    }
    
    // Trường hợp tổng cung < tổng cầu: thêm kho ảo
    if (tong_cung < tong_cau) {
        // Thêm một kho hàng ảo
        kho_hang_can_bang.push_back(tong_cau - tong_cung);
        chi_phi_can_bang.push_back(vector<int>(cua_hang.size(), 0)); // Chi phí 0 cho kho ảo
        ten_kho.push_back("A_ao");
    } 
    // Trường hợp tổng cung > tổng cầu: thêm cửa hàng ảo
    else {
        // Thêm một cửa hàng ảo
        cua_hang_can_bang.push_back(tong_cung - tong_cau);
        for (int i = 0; i < kho_hang.size(); i++) {
            chi_phi_can_bang[i].push_back(0); // Chi phí 0 cho cửa hàng ảo
        }
        ten_cua_hang.push_back("B_ao");
    }
    
    return make_tuple(chi_phi_can_bang, kho_hang_can_bang, cua_hang_can_bang, ten_kho, ten_cua_hang);
}

// Kiểm tra liệu một phương án có phải là phương án cơ sở không
bool laPhuongAnCoBan(const vector<vector<int>>& phuong_an) {
    int m = phuong_an.size();    // Số kho
    int n = phuong_an[0].size(); // Số cửa hàng
    
    // Để là phương án cơ sở, số biến cơ sở phải chính xác là m + n - 1
    int so_bien_co_so = 0;
    for (int i = 0; i < m; i++) {
        for (int j = 0; j < n; j++) {
            if (phuong_an[i][j] > 0) {
                so_bien_co_so++;
            }
        }
    }
    
    return so_bien_co_so == m + n - 1;
}

// Thêm biến cơ sở bằng 0 nếu cần
vector<vector<int>> themBienCoSo(const vector<vector<int>>& chi_phi, vector<vector<int>> phuong_an) {
    int m = phuong_an.size();    // Số kho
    int n = phuong_an[0].size(); // Số cửa hàng
    
    while (!laPhuongAnCoBan(phuong_an)) {
        // Tìm ô có chi phí nhỏ nhất trong các ô chưa là biến cơ sở
        int min_i = -1, min_j = -1;
        int min_cost = INT_MAX;
        
        for (int i = 0; i < m; i++) {
            for (int j = 0; j < n; j++) {
                if (phuong_an[i][j] == 0 && chi_phi[i][j] < min_cost) {
                    min_cost = chi_phi[i][j];
                    min_i = i;
                    min_j = j;
                }
            }
        }
        
        if (min_i != -1 && min_j != -1) {
            // Thêm biến cơ sở bằng 0 (epsilon)
            phuong_an[min_i][min_j] = 0;
        } else {
            break; // Không tìm thấy ô phù hợp
        }
    }
    
    return phuong_an;
}
