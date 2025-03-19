#include <iostream>
#include <vector>
#include <iomanip>
#include <string>
#include <tuple>
#include <functional>
#include <algorithm>

using namespace std;

// Hàm in ma trận vận tải
void inMaTranVanTai(const vector<vector<int>>& chi_phi, 
                    const vector<int>& kho_hang, 
                    const vector<int>& cua_hang,
                    const string& tieu_de,
                    const vector<vector<int>>& phuong_an = {},
                    const vector<string>& ten_kho = {},
                    const vector<string>& ten_cua_hang = {}) {
    
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

// Hàm cân bằng cung cầu
// Trả về tuple gồm: ma trận chi phí, vector cung, vector cầu, tên kho, tên cửa hàng sau khi cân bằng
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

// Hàm tìm phương án ban đầu sử dụng quy tắc góc Tây Bắc
vector<vector<int>> timPhuongAnBanDauGocTayBac(const vector<vector<int>>& chi_phi, 
                                              vector<int> kho_hang, 
                                              vector<int> cua_hang) {
    int m = kho_hang.size();    // Số kho
    int n = cua_hang.size();    // Số cửa hàng
    
    // Tạo ma trận phương án với tất cả các phần tử bằng 0
    vector<vector<int>> phuong_an(m, vector<int>(n, 0));
    
    // Bắt đầu từ góc Tây Bắc (ô [0,0])
    int i = 0, j = 0;
    
    while (i < m && j < n) {
        // Lấy lượng hàng nhỏ nhất giữa cung và cầu
        int luong_hang = min(kho_hang[i], cua_hang[j]);
        
        // Cập nhật phương án
        phuong_an[i][j] = luong_hang;
        
        // Cập nhật lượng hàng còn lại
        kho_hang[i] -= luong_hang;
        cua_hang[j] -= luong_hang;
        
        // Di chuyển đến ô tiếp theo
        if (kho_hang[i] == 0 && i < m - 1) {
            i++; // Chuyển sang kho tiếp theo
        } else if (cua_hang[j] == 0 && j < n - 1) {
            j++; // Chuyển sang cửa hàng tiếp theo
        } else if (i == m - 1 && j == n - 1) {
            break; // Đã đến ô cuối cùng
        }
    }
    
    return phuong_an;
}

// Hàm tìm phương án ban đầu với quy tắc cước phí nhỏ nhất
vector<vector<int>> timPhuongAnBanDau(const vector<vector<int>>& chi_phi, 
                                    vector<int> kho_hang, 
                                    vector<int> cua_hang) {
    int m = kho_hang.size();    // Số kho
    int n = cua_hang.size();    // Số cửa hàng
    
    // Tạo ma trận phương án với tất cả các phần tử bằng 0
    vector<vector<int>> phuong_an(m, vector<int>(n, 0));
    
    while (true) {
        // Tìm ô có chi phí thực nhỏ nhất (không tính kho ảo)
        int min_i = -1, min_j = -1;
        int min_chi_phi = INT_MAX;
        
        // Đầu tiên tìm chi phí nhỏ nhất trong các kho thực
        for (int i = 0; i < m - 1; i++) {  // m - 1 để bỏ qua kho ảo
            if (kho_hang[i] <= 0) continue;
            
            for (int j = 0; j < n; j++) {
                if (cua_hang[j] <= 0) continue;
                
                if (chi_phi[i][j] < min_chi_phi) {
                    min_chi_phi = chi_phi[i][j];
                    min_i = i;
                    min_j = j;
                }
            }
        }
        
        // Nếu không tìm thấy ô phù hợp trong các kho thực, xét đến kho ảo
        if (min_i == -1 && kho_hang[m-1] > 0) {
            min_i = m - 1;  // Chỉ số của kho ảo
            // Tìm cửa hàng còn nhu cầu để phân phối từ kho ảo
            for (int j = 0; j < n; j++) {
                if (cua_hang[j] > 0) {
                    min_j = j;
                    break;
                }
            }
        }
        
        // Nếu không tìm thấy ô phù hợp nào, kết thúc
        if (min_i == -1 || min_j == -1) break;
        
        // Lấy lượng hàng nhỏ nhất giữa cung và cầu
        int luong_hang = min(kho_hang[min_i], cua_hang[min_j]);
        
        // Cập nhật phương án
        phuong_an[min_i][min_j] = luong_hang;
        
        // Cập nhật lượng hàng còn lại
        kho_hang[min_i] -= luong_hang;
        cua_hang[min_j] -= luong_hang;
    }
    
    return phuong_an;
}

// Hàm tìm phương án ban đầu bằng phương pháp xấp xỉ Vogel
vector<vector<int>> timPhuongAnBanDauVogel(const vector<vector<int>>& chi_phi, 
                                          vector<int> kho_hang, 
                                          vector<int> cua_hang) {
    int m = kho_hang.size();    // Số kho
    int n = cua_hang.size();    // Số cửa hàng
    
    // Tạo ma trận phương án với tất cả các phần tử bằng 0
    vector<vector<int>> phuong_an(m, vector<int>(n, 0));
    
    // Tạo bản sao của ma trận chi phí để đánh dấu các ô đã xử lý
    vector<vector<int>> chi_phi_copy = chi_phi;
    
    // Tạo vector đánh dấu hàng và cột đã xử lý
    vector<bool> hang_da_xu_ly(m, false);
    vector<bool> cot_da_xu_ly(n, false);
    
    // Đếm số hàng và cột đã xử lý
    int so_hang_da_xu_ly = 0;
    int so_cot_da_xu_ly = 0;
    
    // Lặp cho đến khi tất cả các hàng và cột đều đã được xử lý
    while (so_hang_da_xu_ly < m && so_cot_da_xu_ly < n) {
        // Tính chênh lệch cho mỗi hàng (hiệu của hai chi phí nhỏ nhất)
        vector<int> chenh_lech_hang(m, 9999999);
        for (int i = 0; i < m; i++) {
            if (hang_da_xu_ly[i]) continue;
            
            int min1 = 9999999, min2 = 9999999;
            int so_cot_con_lai = 0;
            
            for (int j = 0; j < n; j++) {
                if (cot_da_xu_ly[j]) continue;
                
                so_cot_con_lai++;
                if (chi_phi_copy[i][j] < min1) {
                    min2 = min1;
                    min1 = chi_phi_copy[i][j];
                } else if (chi_phi_copy[i][j] < min2) {
                    min2 = chi_phi_copy[i][j];
                }
            }
            
            // Nếu chỉ còn 1 cột, chênh lệch là một giá trị rất lớn để ưu tiên
            if (so_cot_con_lai == 1) {
                chenh_lech_hang[i] = 9999999; // Giá trị lớn để ưu tiên
            } 
            // Nếu còn từ 2 cột trở lên, chênh lệch là hiệu của hai giá trị nhỏ nhất
            else if (so_cot_con_lai >= 2) {
                chenh_lech_hang[i] = min2 - min1;
            }
        }
        
        // Tính chênh lệch cho mỗi cột (hiệu của hai chi phí nhỏ nhất)
        vector<int> chenh_lech_cot(n, 9999999);
        for (int j = 0; j < n; j++) {
            if (cot_da_xu_ly[j]) continue;
            
            int min1 = 9999999, min2 = 9999999;
            int so_hang_con_lai = 0;
            
            for (int i = 0; i < m; i++) {
                if (hang_da_xu_ly[i]) continue;
                
                so_hang_con_lai++;
                if (chi_phi_copy[i][j] < min1) {
                    min2 = min1;
                    min1 = chi_phi_copy[i][j];
                } else if (chi_phi_copy[i][j] < min2) {
                    min2 = chi_phi_copy[i][j];
                }
            }
            
            // Nếu chỉ còn 1 hàng, chênh lệch là một giá trị rất lớn để ưu tiên
            if (so_hang_con_lai == 1) {
                chenh_lech_cot[j] = 9999999; // Giá trị lớn để ưu tiên
            } 
            // Nếu còn từ 2 hàng trở lên, chênh lệch là hiệu của hai giá trị nhỏ nhất
            else if (so_hang_con_lai >= 2) {
                chenh_lech_cot[j] = min2 - min1;
            }
        }
        
        // Tìm chênh lệch lớn nhất
        int max_chenh_lech = -1;
        bool chon_hang = true;  // true: chọn hàng, false: chọn cột
        int hang_hoac_cot = -1; // chỉ số hàng hoặc cột được chọn
        
        for (int i = 0; i < m; i++) {
            if (!hang_da_xu_ly[i] && chenh_lech_hang[i] > max_chenh_lech) {
                max_chenh_lech = chenh_lech_hang[i];
                chon_hang = true;
                hang_hoac_cot = i;
            }
        }
        
        for (int j = 0; j < n; j++) {
            if (!cot_da_xu_ly[j] && chenh_lech_cot[j] > max_chenh_lech) {
                max_chenh_lech = chenh_lech_cot[j];
                chon_hang = false;
                hang_hoac_cot = j;
            }
        }
        
        // Nếu không tìm thấy chênh lệch nào, thoát khỏi vòng lặp
        if (max_chenh_lech == -1) break;
        
        // Tìm ô có chi phí nhỏ nhất trong hàng hoặc cột được chọn
        int min_i = -1, min_j = -1;
        int min_chi_phi = 9999999;
        
        if (chon_hang) {
            // Tìm ô có chi phí nhỏ nhất trong hàng được chọn
            int i = hang_hoac_cot;
            for (int j = 0; j < n; j++) {
                if (!cot_da_xu_ly[j] && chi_phi_copy[i][j] < min_chi_phi) {
                    min_chi_phi = chi_phi_copy[i][j];
                    min_i = i;
                    min_j = j;
                }
            }
        } else {
            // Tìm ô có chi phí nhỏ nhất trong cột được chọn
            int j = hang_hoac_cot;
            for (int i = 0; i < m; i++) {
                if (!hang_da_xu_ly[i] && chi_phi_copy[i][j] < min_chi_phi) {
                    min_chi_phi = chi_phi_copy[i][j];
                    min_i = i;
                    min_j = j;
                }
            }
        }
        
        // Nếu không tìm thấy ô nào, thoát khỏi vòng lặp
        if (min_i == -1 || min_j == -1) break;
        
        // Lấy lượng hàng nhỏ nhất giữa cung và cầu
        int luong_hang = kho_hang[min_i] < cua_hang[min_j] ? kho_hang[min_i] : cua_hang[min_j];
        
        // Cập nhật phương án
        phuong_an[min_i][min_j] = luong_hang;
        
        // Cập nhật lượng hàng còn lại ở kho và nhu cầu còn lại ở cửa hàng
        kho_hang[min_i] -= luong_hang;
        cua_hang[min_j] -= luong_hang;
        
        // Đánh dấu hàng hoặc cột đã xử lý nếu đã hết hàng hoặc đủ hàng
        if (kho_hang[min_i] == 0) {
            hang_da_xu_ly[min_i] = true;
            so_hang_da_xu_ly++;
        }
        
        if (cua_hang[min_j] == 0) {
            cot_da_xu_ly[min_j] = true;
            so_cot_da_xu_ly++;
        }
    }
    
    return phuong_an;
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

// Hàm tìm chu trình trong phương án để cải tiến
vector<pair<int, int>> timChuTrinh(const vector<vector<int>>& phuong_an, int start_i, int start_j) {
    int m = phuong_an.size();
    int n = phuong_an[0].size();
    
    // Đánh dấu các ô cơ sở
    vector<vector<bool>> la_o_co_so(m, vector<bool>(n, false));
    for (int i = 0; i < m; i++) {
        for (int j = 0; j < n; j++) {
            if (phuong_an[i][j] > 0) {
                la_o_co_so[i][j] = true;
            }
        }
    }
    
    // Thêm điểm xuất phát (ô không phải cơ sở)
    la_o_co_so[start_i][start_j] = true;
    
    // Danh sách chu trình
    vector<pair<int, int>> chu_trinh;
    chu_trinh.push_back({start_i, start_j});
    
    // Mảng đánh dấu các chiều ngang/dọc đã sử dụng trong chu trình
    vector<bool> hang_da_dung(m, false);
    vector<bool> cot_da_dung(n, false);
    
    hang_da_dung[start_i] = true;
    cot_da_dung[start_j] = true;
    
    // Tìm chu trình
    while (chu_trinh.size() < m + n) {
        bool tim_thay = false;
        
        // Tìm ô tiếp theo trong chu trình
        for (int i = 0; i < m; i++) {
            if (!hang_da_dung[i]) continue;
            
            for (int j = 0; j < n; j++) {
                if (!cot_da_dung[j] && la_o_co_so[i][j]) {
                    chu_trinh.push_back({i, j});
                    cot_da_dung[j] = true;
                    tim_thay = true;
                    break;
                }
            }
            
            if (tim_thay) break;
        }
        
        if (!tim_thay) {
            // Tìm theo cột
            for (int j = 0; j < n; j++) {
                if (!cot_da_dung[j]) continue;
                
                for (int i = 0; i < m; i++) {
                    if (!hang_da_dung[i] && la_o_co_so[i][j]) {
                        chu_trinh.push_back({i, j});
                        hang_da_dung[i] = true;
                        tim_thay = true;
                        break;
                    }
                }
                
                if (tim_thay) break;
            }
        }
        
        if (!tim_thay) break;
    }
    
    return chu_trinh;
}

// Hàm tìm phương án tối ưu bằng phương pháp MODI
vector<vector<int>> timPhuongAnToiUu(const vector<vector<int>>& chi_phi, vector<vector<int>> phuong_an) {
    cout << "\n=== BAT DAU TIM PHUONG AN TOI UU ===\n" << endl;
    
    int m = phuong_an.size();    // Số kho
    int n = phuong_an[0].size(); // Số cửa hàng
    
    // Kiểm tra và in phương án ban đầu
    cout << "Phuong an ban dau:" << endl;
    for (int i = 0; i < m; i++) {
        for (int j = 0; j < n; j++) {
            cout << phuong_an[i][j] << "\t";
        }
        cout << endl;
    }
    
    // Lặp cho đến khi tìm được phương án tối ưu
    int vong_lap = 0;
    while (true) {
        vong_lap++;
        cout << "\n--- Vong lap cai tien " << vong_lap << " ---" << endl;
        
        // Khởi tạo mảng thế vị
        vector<int> u(m, -9999);  // Thế vị hàng (giá trị âm lớn để đánh dấu chưa gán)
        vector<int> v(n, -9999);  // Thế vị cột (giá trị âm lớn để đánh dấu chưa gán)
        
        // Tìm hàng hoặc cột có nhiều biến cơ sở nhất để gán u=0 hoặc v=0
        int max_bien_co_so = 0;
        int hang_duoc_chon = -1;
        int cot_duoc_chon = -1;
        
        for (int i = 0; i < m; i++) {
            int dem = 0;
            for (int j = 0; j < n; j++) {
                if (phuong_an[i][j] > 0) dem++;
            }
            if (dem > max_bien_co_so) {
                max_bien_co_so = dem;
                hang_duoc_chon = i;
                cot_duoc_chon = -1;
            }
        }
        
        for (int j = 0; j < n; j++) {
            int dem = 0;
            for (int i = 0; i < m; i++) {
                if (phuong_an[i][j] > 0) dem++;
            }
            if (dem > max_bien_co_so) {
                max_bien_co_so = dem;
                hang_duoc_chon = -1;
                cot_duoc_chon = j;
            }
        }
        
        // Gán giá trị 0 cho u hoặc v
        int u_da_gan = 0, v_da_gan = 0;
        
        if (hang_duoc_chon != -1) {
            u[hang_duoc_chon] = 0;
            u_da_gan++;
            cout << "Gan u[" << hang_duoc_chon << "] = 0" << endl;
        } else if (cot_duoc_chon != -1) {
            v[cot_duoc_chon] = 0;
            v_da_gan++;
            cout << "Gan v[" << cot_duoc_chon << "] = 0" << endl;
        } else {
            // Nếu không tìm được hàng/cột, gán u[0] = 0
            u[0] = 0;
            u_da_gan++;
            cout << "Gan u[0] = 0 (mac dinh)" << endl;
        }
        
        // Tính các giá trị u, v
        bool co_thay_doi = true;
        while (co_thay_doi) {
            co_thay_doi = false;
            
            for (int i = 0; i < m; i++) {
                for (int j = 0; j < n; j++) {
                    if (phuong_an[i][j] > 0) {
                        // Nếu đã biết u[i], tính v[j]
                        if (u[i] != -9999 && v[j] == -9999) {
                            v[j] = chi_phi[i][j] - u[i];
                            v_da_gan++;
                            co_thay_doi = true;
                            cout << "Tinh v[" << j << "] = " << chi_phi[i][j] << " - " << u[i] << " = " << v[j] << endl;
                        }
                        // Nếu đã biết v[j], tính u[i]
                        else if (u[i] == -9999 && v[j] != -9999) {
                            u[i] = chi_phi[i][j] - v[j];
                            u_da_gan++;
                            co_thay_doi = true;
                            cout << "Tinh u[" << i << "] = " << chi_phi[i][j] << " - " << v[j] << " = " << u[i] << endl;
                        }
                    }
                }
            }
            
            // Kiểm tra nếu đã tính đủ u và v
            if (u_da_gan == m && v_da_gan == n) {
                cout << "Da tinh du tat ca cac the vi u va v" << endl;
                break;
            }
            
            // Kiểm tra nếu không còn thay đổi nhưng chưa tính đủ, có thể phương án không phải phương án cơ sở
            if (!co_thay_doi && (u_da_gan < m || v_da_gan < n)) {
                cout << "CANH BAO: Khong the tinh du the vi (u_da_gan = " << u_da_gan 
                     << ", v_da_gan = " << v_da_gan << "). Phuong an co the khong phai phuong an co so!" << endl;
                
                // Gán giá trị mặc định cho các thế vị chưa tính được
                for (int i = 0; i < m; i++) {
                    if (u[i] == -9999) {
                        u[i] = 0;
                        u_da_gan++;
                        cout << "Gan gia tri mac dinh u[" << i << "] = 0" << endl;
                    }
                }
                
                for (int j = 0; j < n; j++) {
                    if (v[j] == -9999) {
                        v[j] = 0;
                        v_da_gan++;
                        cout << "Gan gia tri mac dinh v[" << j << "] = 0" << endl;
                    }
                }
                break;
            }
        }
        
        // In các thế vị u, v đã tính được
        cout << "The vi u: ";
        for (int i = 0; i < m; i++) {
            cout << u[i] << " ";
        }
        cout << endl;
        
        cout << "The vi v: ";
        for (int j = 0; j < n; j++) {
            cout << v[j] << " ";
        }
        cout << endl;
        
        // Tính delta cho các ô không phải biến cơ sở
        int min_delta = 0;
        int di = -1, dj = -1;
        
        cout << "Cac gia tri delta:" << endl;
        for (int i = 0; i < m; i++) {
            for (int j = 0; j < n; j++) {
                if (phuong_an[i][j] == 0) {
                    int delta = chi_phi[i][j] - u[i] - v[j];
                    cout << "delta[" << i << "][" << j << "] = " << chi_phi[i][j] << " - " << u[i] << " - " << v[j] << " = " << delta << endl;
                    
                    if (delta < min_delta) {
                        min_delta = delta;
                        di = i;
                        dj = j;
                    }
                }
            }
        }
        
        cout << "Delta nho nhat: " << min_delta << " tai vi tri [" << di << "][" << dj << "]" << endl;
        
        // Nếu tất cả delta >= 0, phương án đã tối ưu
        if (min_delta >= 0) {
            cout << "Khong con delta am => Phuong an da toi uu!" << endl;
            break;
        }
        
        // Xây dựng chu trình cải tiến
        vector<vector<bool>> loop(m, vector<bool>(n, false));
        vector<vector<char>> dau(m, vector<char>(n, ' '));
        
        // Đánh dấu ô có delta âm nhất
        loop[di][dj] = true;
        dau[di][dj] = '+';
        
        cout << "Xay dung chu trinh cai tien tu o [" << di << "][" << dj << "]" << endl;
        
        // Đánh dấu các ô biến cơ sở
        for (int i = 0; i < m; i++) {
            for (int j = 0; j < n; j++) {
                if (phuong_an[i][j] > 0) {
                    loop[i][j] = true;
                }
            }
        }
        
        // Loại bỏ các hàng và cột không liên quan
        co_thay_doi = true;
        while (co_thay_doi) {
            co_thay_doi = false;
            
            // Loại bỏ các hàng có duy nhất một biến cơ sở (trừ hàng của ô mới)
            for (int i = 0; i < m; i++) {
                if (i == di) continue;
                
                int lc = 0;
                for (int j = 0; j < n; j++) {
                    if (loop[i][j]) lc++;
                }
                
                if (lc == 1) {
                    for (int j = 0; j < n; j++) {
                        if (loop[i][j]) {
                            loop[i][j] = false;
                            co_thay_doi = true;
                        }
                    }
                }
            }
            
            // Loại bỏ các cột có duy nhất một biến cơ sở (trừ cột của ô mới)
            for (int j = 0; j < n; j++) {
                if (j == dj) continue;
                
                int lc = 0;
                for (int i = 0; i < m; i++) {
                    if (loop[i][j]) lc++;
                }
                
                if (lc == 1) {
                    for (int i = 0; i < m; i++) {
                        if (loop[i][j]) {
                            loop[i][j] = false;
                            co_thay_doi = true;
                        }
                    }
                }
            }
        }
        
        // Tìm chu trình bằng cách theo vết
        bool found_cycle = false;
        int current_i = di;
        int current_j = dj;
        char current_sign = '+';
        
        // Giới hạn số lần thử để tránh vòng lặp vô hạn
        int max_attempts = m * n;
        int attempts = 0;
        
        while (attempts < max_attempts) {
            attempts++;
            found_cycle = false;
            
            // Tìm điểm tiếp theo trên cùng hàng
            int next_j = -1;
            for (int j = 0; j < n; j++) {
                if (j != current_j && loop[current_i][j]) {
                    next_j = j;
                    current_sign = (current_sign == '+') ? '-' : '+';
                    dau[current_i][next_j] = current_sign;
                    current_j = next_j;
                    found_cycle = true;
                    cout << "Them vao chu trinh: [" << current_i << "][" << current_j << "] voi dau " << current_sign << endl;
                    break;
                }
            }
            
            if (!found_cycle) break;
            
            // Tìm điểm tiếp theo trên cùng cột
            int next_i = -1;
            for (int i = 0; i < m; i++) {
                if (i != current_i && loop[i][current_j]) {
                    next_i = i;
                    current_sign = (current_sign == '+') ? '-' : '+';
                    dau[next_i][current_j] = current_sign;
                    current_i = next_i;
                    found_cycle = true;
                    cout << "Them vao chu trinh: [" << current_i << "][" << current_j << "] voi dau " << current_sign << endl;
                    break;
                }
            }
            
            if (!found_cycle) {
                cout << "Khong tim thay o tiep theo trong chu trinh" << endl;
                break;
            }
            
            // Kiểm tra nếu đã quay về điểm bắt đầu
            if (current_i == di && current_j == dj) {
                cout << "Chu trinh da dong!" << endl;
                found_cycle = true;
                break;
            }
        }
        
        if (!found_cycle) {
            cout << "CANH BAO: Khong the tim thay chu trinh day du. Thu phuong phap khac." << endl;
            
            // Phương pháp đơn giản hơn để tìm chu trình
            for (int i = 0; i < m; i++) {
                for (int j = 0; j < n; j++) {
                    dau[i][j] = ' ';
                }
            }
            
            dau[di][dj] = '+';
            
            // Gán dấu - cho biến cơ sở gần nhất trên cùng hàng
            for (int j = 0; j < n; j++) {
                if (j != dj && phuong_an[di][j] > 0) {
                    dau[di][j] = '-';
                    
                    // Gán dấu + cho biến cơ sở khác trên cùng cột
                    for (int i = 0; i < m; i++) {
                        if (i != di && phuong_an[i][j] > 0) {
                            dau[i][j] = '+';
                            
                            // Gán dấu - cho biến cơ sở khác trên cùng hàng với dấu +
                            for (int j2 = 0; j2 < n; j2++) {
                                if (j2 != j && phuong_an[i][j2] > 0) {
                                    dau[i][j2] = '-';
                                }
                            }
                        }
                    }
                }
            }
        }
        
        // In ma trận dấu
        cout << "Ma tran dau:" << endl;
        for (int i = 0; i < m; i++) {
            for (int j = 0; j < n; j++) {
                cout << (dau[i][j] == ' ' ? "." : string(1, dau[i][j])) << " ";
            }
            cout << endl;
        }
        
        // Tìm theta (giá trị nhỏ nhất trong các ô có dấu -)
        int theta = INT_MAX;
        for (int i = 0; i < m; i++) {
            for (int j = 0; j < n; j++) {
                if (dau[i][j] == '-' && phuong_an[i][j] > 0 && phuong_an[i][j] < theta) {
                    theta = phuong_an[i][j];
                }
            }
        }
        
        if (theta == INT_MAX) {
            cout << "CANH BAO: Khong the tim theta hop le. Gan theta = 1" << endl;
            theta = 1;
        }
        
        cout << "Gia tri theta = " << theta << endl;
        
        // Cập nhật phương án
        for (int i = 0; i < m; i++) {
            for (int j = 0; j < n; j++) {
                if (dau[i][j] == '+') {
                    phuong_an[i][j] += theta;
                } else if (dau[i][j] == '-') {
                    phuong_an[i][j] -= theta;
                }
            }
        }
        
        // In phương án mới
        cout << "Phuong an moi:" << endl;
        for (int i = 0; i < m; i++) {
            for (int j = 0; j < n; j++) {
                cout << phuong_an[i][j] << "\t";
            }
            cout << endl;
        }
    }
    
    cout << "\n=== KET THUC TIM PHUONG AN TOI UU ===\n" << endl;
    return phuong_an;
}

int main() {
    int kho, shop;

    cout << "Nhap so kho hang:";
    cin >> kho;

    cout << "Nhap so cua hang:";
    cin >> shop;

    vector<int> kho_hang(kho);
    vector<int> cua_hang(shop);

    // Nhap luong hang cua kho hang
    for (int i = 1; i <= kho; i++) {
        cout << "Nhap luong hang cua kho hang A" << i << ": ";
        cin >> kho_hang[i-1];
    }

    // Nhap luong hang cua cua hang
    for (int i = 1; i <= shop; i++) {
        cout << "Nhap luong hang cua cua hang B" << i << ": ";
        cin >> cua_hang[i-1];
    }

    vector<vector<int>> chi_phi(kho, vector<int>(shop));

    // Nhap chi phi van chuyen tu kho hang den cua hang
    for (int i = 1; i <= kho; i++) {
        for (int j = 1; j <= shop; j++) {
            cout << "Nhap chi phi van chuyen tu kho hang A" << i << " den cua hang B" << j << ": ";
            cin >> chi_phi[i-1][j-1];
        }
    }
    
    // In ma trận gốc (không có phương án)
    inMaTranVanTai(chi_phi, kho_hang, cua_hang, "MA TRAN GOC TRUOC KHI CAN BANG");
    
    // Kiểm tra điều kiện cân bằng cung-cầu
    int tong_cung = 0, tong_cau = 0;
    for (int i = 0; i < kho; i++) {
        tong_cung += kho_hang[i];
    }
    for (int j = 0; j < shop; j++) {
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
        for (int i = 0; i < kho; i++) {
            ten_kho.push_back("A" + to_string(i+1));
        }
        
        for (int j = 0; j < shop; j++) {
            ten_cua_hang.push_back("B" + to_string(j+1));
        }
    }
    
    // Cho phép người dùng chọn phương pháp tìm phương án ban đầu
    int lua_chon;
    cout << "\n=== CHON PHUONG PHAP TIM PHUONG AN BAN DAU ===" << endl;
    cout << "1. Quy tac goc Tay Bac" << endl;
    cout << "2. Quy tac cuoc phi nho nhat" << endl;
    cout << "3. Phuong phap xap xi Vogel" << endl;
    cout << "Nhap lua chon cua ban (1-3): ";
    cin >> lua_chon;
    
    // Biến để lưu phương án ban đầu và tên phương pháp
    vector<vector<int>> phuong_an;
    string ten_phuong_phap;
    
    // Tìm phương án ban đầu theo phương pháp đã chọn
    switch (lua_chon) {
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
    
    // In ma trận với phương án ban đầu
    inMaTranVanTai(chi_phi_can_bang, kho_hang_can_bang, cua_hang_can_bang, 
                  "PHUONG AN BAN DAU (" + ten_phuong_phap + ")", 
                  phuong_an, ten_kho, ten_cua_hang);
    
    // Tính và hiển thị tổng chi phí vận chuyển ban đầu
    int tong_chi_phi_ban_dau = tinhTongChiPhi(chi_phi_can_bang, phuong_an);
    cout << "\nTong chi phi van chuyen ban dau: " << tong_chi_phi_ban_dau << endl;
    
    cout << "\n-------Bat dau tim phuong an toi uu-------" << endl;
    
    // Tìm phương án tối ưu - THÊM LOG ĐỂ THEO DÕI
    vector<vector<int>> phuong_an_toi_uu = timPhuongAnToiUu(chi_phi_can_bang, phuong_an);
    
    cout << "\n-------Da tim xong phuong an toi uu-------" << endl;
    
    // In ma trận với phương án tối ưu
    inMaTranVanTai(chi_phi_can_bang, kho_hang_can_bang, cua_hang_can_bang, 
                  "PHUONG AN TOI UU", 
                  phuong_an_toi_uu, ten_kho, ten_cua_hang);
    
    // Tính và hiển thị tổng chi phí vận chuyển tối ưu
    int tong_chi_phi_toi_uu = tinhTongChiPhi(chi_phi_can_bang, phuong_an_toi_uu);
    cout << "\nTong chi phi van chuyen toi uu: " << tong_chi_phi_toi_uu << endl;
    
    return 0;
}