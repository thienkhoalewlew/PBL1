#include "initialSolution.h"
#include <algorithm>
#include <climits>

using namespace std;

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
