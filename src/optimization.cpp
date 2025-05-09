#include "optimization.h"
#include <iostream>
#include <climits>

using namespace std;

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
