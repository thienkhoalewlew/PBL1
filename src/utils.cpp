#include "utils.h"

using namespace std;

// Hàm in ma trận vận tải
void inMaTranVanTai(const vector<vector<int>>& chi_phi, 
                    const vector<int>& kho_hang, 
                    const vector<int>& cua_hang,
                    const string& tieu_de,
                    const vector<vector<int>>& phuong_an,
                    const vector<string>& ten_kho,
                    const vector<string>& ten_cua_hang) {
    
    cout << "\n=== " << tieu_de << " ===\n\n";
    
    // Tính tổng cung và cầu
    int tong_cung = 0;
    for (int i = 0; i < kho_hang.size(); i++) {
        tong_cung += kho_hang[i];
    }
    
    int tong_cau = 0;
    for (int j = 0; j < cua_hang.size(); j++) {
        tong_cau += cua_hang[j];
    }
    
    // In tiêu đề cột
    cout << setw(12) << " ";
    for (int j = 0; j < cua_hang.size(); j++) {
        string ten;
        if (j < ten_cua_hang.size() && !ten_cua_hang[j].empty()) {
            ten = ten_cua_hang[j];
        } else {
            ten = "B" + to_string(j+1);
        }
        cout << setw(12) << ten + "(" + to_string(cua_hang[j]) + ")";
    }
    cout << endl;
    
    // In ma trận chi phí và phương án (nếu có)
    for (int i = 0; i < kho_hang.size(); i++) {
        string ten;
        if (i < ten_kho.size() && !ten_kho[i].empty()) {
            ten = ten_kho[i];
        } else {
            ten = "A" + to_string(i+1);
        }
        cout << setw(12) << ten + "(" + to_string(kho_hang[i]) + ")";
        
        for (int j = 0; j < cua_hang.size(); j++) {
            if (phuong_an.empty()) {
                // Nếu chưa có phương án, hiển thị "none" và chi phí
                cout << setw(12) << "none(" + to_string(chi_phi[i][j]) + ")";
            } else {
                // Nếu có phương án, hiển thị lượng hàng và chi phí
                cout << setw(12) << to_string(phuong_an[i][j]) + "(" + to_string(chi_phi[i][j]) + ")";
            }
        }
        cout << endl;
    }
    
    // Hiển thị thông tin về tính cân bằng
    cout << "\nTong cung: " << tong_cung << ", Tong cau: " << tong_cau << endl;
    if (tong_cung == tong_cau) {
        cout << "Bai toan can bang: Tong cung = Tong cau" << endl;
    } else {
        cout << "Bai toan khong can bang: Tong cung " 
             << (tong_cung > tong_cau ? ">" : "<") << " Tong cau" << endl;
    }
}

// Hàm đọc ma trận vận tải từ file
bool docMaTranTuFile(const string& ten_file, 
                    vector<vector<int>>& chi_phi, 
                    vector<int>& kho_hang, 
                    vector<int>& cua_hang) {
    ifstream file(ten_file);
    if (!file.is_open()) {
        cout << "Khong mo duoc file " << ten_file << endl;
        return false;
    }
    
    int kho, shop;
    if (!(file >> kho >> shop)) {
        cout << "Loi: Khong doc duoc so kho va so cua hang." << endl;
        file.close();
        return false;
    }
    
    // Resize các vector để lưu trữ dữ liệu
    kho_hang.resize(kho);
    cua_hang.resize(shop);
    chi_phi.resize(kho, vector<int>(shop));
    
    // Đọc lượng hàng của các kho
    for (int i = 0; i < kho; i++) {
        if (!(file >> kho_hang[i])) {
            cout << "Loi: Khong doc duoc luong hang kho " << (i+1) << endl;
            file.close();
            return false;
        }
    }
    
    // Đọc nhu cầu của các cửa hàng
    for (int j = 0; j < shop; j++) {
        if (!(file >> cua_hang[j])) {
            cout << "Loi: Khong doc duoc nhu cau cua hang " << (j+1) << endl;
            file.close();
            return false;
        }
    }
    
    // Đọc ma trận chi phí
    for (int i = 0; i < kho; i++) {
        for (int j = 0; j < shop; j++) {
            if (!(file >> chi_phi[i][j])) {
                cout << "Loi: Khong doc duoc chi phi van chuyen [" << (i+1) << "," << (j+1) << "]" << endl;
                file.close();
                return false;
            }
        }
    }
    
    file.close();
    return true;
}

// Hàm tính tổng chi phí vận chuyển
int tinhTongChiPhi(const vector<vector<int>>& chi_phi, const vector<vector<int>>& phuong_an) {
    int total_cost = 0;
    int m = chi_phi.size();
    int n = chi_phi[0].size();
    
    for (int i = 0; i < m; i++) {
        for (int j = 0; j < n; j++) {
            total_cost += chi_phi[i][j] * phuong_an[i][j];
        }
    }
    
    return total_cost;
}
