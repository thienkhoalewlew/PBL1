<!-- filepath: README.md -->
# Bài Toán Vận Tải - Hướng dẫn biên dịch và chạy chương trình

## Giới thiệu
Chương trình này giải bài toán vận tải sử dụng các phương pháp tối ưu hóa như phương pháp góc Tây Bắc, phương pháp chi phí thấp nhất, và phương pháp xấp xỉ Vogel, kết hợp với thuật toán tối ưu MODI (Modified Distribution).

## Cấu trúc mã nguồn
Mã nguồn được tổ chức thành các module sau:
- `Main.cpp`: Module chính điều phối luồng chương trình
- `utils.h/cpp`: Các hàm tiện ích như đọc/ghi file, hiển thị ma trận
- `transportationModel.h/cpp`: Cấu trúc dữ liệu và thuật toán cho mô hình vận tải
- `initialSolution.h/cpp`: Các phương pháp tìm phương án ban đầu
- `optimization.h/cpp`: Các thuật toán tối ưu hóa
- `ui.h/cpp`: Các hàm giao diện người dùng
- `solver.h/cpp`: Logic giải quyết bài toán vận tải

## Yêu cầu hệ thống
- Trình biên dịch C++ hỗ trợ C++11 trở lên 
- Hệ điều hành: Windows, macOS, hoặc Linux
- Make (tùy chọn, để sử dụng Makefile)

## Biên dịch chương trình

### Sử dụng Makefile (Khuyến nghị)
Để biên dịch chương trình sử dụng Makefile, mở terminal và điều hướng đến thư mục `src`, sau đó chạy:

#### Windows (với MinGW):
```powershell
cd src
mingw32-make
```

#### Linux/macOS:
```bash
cd src
make
```

Chương trình sẽ được biên dịch và tạo file thực thi `Program.exe` trong thư mục `src`.

### Biên dịch thủ công
Nếu không sử dụng Makefile, bạn có thể biên dịch thủ công (chạy lệnh này từ thư mục gốc dự án):

```bash
g++ -std=c++11 -o Program.exe src/Main.cpp src/utils.cpp src/transportationModel.cpp src/initialSolution.cpp src/optimization.cpp src/ui.cpp src/solver.cpp
```

## Chạy chương trình

Sau khi biên dịch thành công, bạn có thể chạy chương trình:

#### Windows:
```powershell
# Từ thư mục gốc của dự án
.\Program.exe
```

hoặc:
```powershell
# Từ thư mục src
cd src
.\Program.exe
```

#### Linux/macOS:
```bash
# Từ thư mục gốc của dự án
./Program.exe
```

hoặc:
```bash
# Từ thư mục src
cd src
./Program.exe
```

## Các bước tính toán của chương trình

Chương trình thực hiện các bước sau để giải bài toán vận tải:

1. **Nhập dữ liệu**: 
   - Người dùng có thể chọn nhập dữ liệu từ bàn phím, đọc từ file, hoặc sử dụng dữ liệu mẫu.
   - Dữ liệu cần nhập gồm số lượng kho hàng, số lượng cửa hàng, khả năng cung cấp của mỗi kho, nhu cầu của mỗi cửa hàng và chi phí vận chuyển giữa các kho và cửa hàng.

2. **Kiểm tra và cân bằng cung-cầu**:
   - Kiểm tra tính hợp lệ của dữ liệu đầu vào.
   - Tổng hợp tổng cung và tổng cầu.
   - Nếu tổng cung ≠ tổng cầu, thực hiện cân bằng bằng cách:
     - Nếu tổng cung < tổng cầu: Thêm kho ảo.
     - Nếu tổng cung > tổng cầu: Thêm cửa hàng ảo.

3. **Tìm phương án ban đầu**:
   - Người dùng chọn một trong ba phương pháp:
     - **Phương pháp góc Tây Bắc**: Bắt đầu từ ô [0,0] và phân phối lượng hàng theo thứ tự từ trái sang phải, từ trên xuống dưới.
     - **Phương pháp chi phí thấp nhất**: Ưu tiên phân phối hàng cho các ô có chi phí vận chuyển thấp nhất.
     - **Phương pháp xấp xỉ Vogel**: Tính chênh lệch giữa hai chi phí nhỏ nhất trong mỗi hàng và cột, rồi phân phối hàng cho ô có chênh lệch lớn nhất.

4. **Kiểm tra và bổ sung biến cơ sở**:
   - Kiểm tra xem phương án ban đầu có phải là phương án cơ sở không (có đúng m+n-1 biến cơ sở).
   - Nếu không đủ biến cơ sở, thêm các biến cơ sở bằng 0 vào các vị trí phù hợp.

5. **Tìm phương án tối ưu bằng thuật toán MODI**:
   - Tính các thế vị u và v cho các hàng và cột.
   - Tính delta (chỉ số đánh giá) cho các ô không phải biến cơ sở.
   - Nếu tất cả delta ≥ 0, phương án hiện tại là tối ưu.
   - Nếu có delta < 0, xây dựng chu trình cải tiến và cập nhật phương án.
   - Lặp lại cho đến khi tìm được phương án tối ưu.

6. **Hiển thị kết quả**:
   - In ma trận phương án tối ưu.
   - Tính và hiển thị tổng chi phí vận chuyển.

## Định dạng tệp đầu vào

Tệp đầu vào nên được định dạng như sau:
```
m n          // Số kho hàng (m) và số cửa hàng (n)
a1 a2 ... am // Lượng hàng ở mỗi kho
b1 b2 ... bn // Nhu cầu ở mỗi cửa hàng
c11 c12 ... c1n // Chi phí vận chuyển từ kho 1 đến các cửa hàng
c21 c22 ... c2n // Chi phí vận chuyển từ kho 2 đến các cửa hàng
...
cm1 cm2 ... cmn // Chi phí vận chuyển từ kho m đến các cửa hàng
```

Ví dụ:
```
3 4    // 3 kho, 4 cửa hàng
10 20 30  // Lượng hàng tại mỗi kho
15 25 5 10  // Nhu cầu tại mỗi cửa hàng
2 3 4 1  // Chi phí vận chuyển từ kho 1
3 2 5 6  // Chi phí vận chuyển từ kho 2
1 4 5 2  // Chi phí vận chuyển từ kho 3
```
