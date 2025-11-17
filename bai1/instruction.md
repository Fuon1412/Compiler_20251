---

# 🛠️ **1 Cách biên dịch (Build)**

Chạy lệnh:

```bash
make
```

Lệnh này sẽ biên dịch `main.c` bằng gcc và tạo ra file thực thi:

```
indexer
```

---

# 🚀 **2. Cách chạy chương trình**

Chương trình nhận 3 tham số dòng lệnh:

```
./indexer <input_file> <stopword_file> <output_file>
```

Ví dụ:

```bash
./indexer alice30.txt stopw.txt index.txt
```

---

# ▶️ **3. Chạy bằng Makefile (truyền tham số)**

Makefile hỗ trợ truyền tên file trực tiếp:

```bash
make run input=alice30.txt stop=stopw.txt output=index.txt
```

Ý nghĩa:

* `input=`  → file văn bản cần xử lý
* `stop=`   → file stopword
* `output=` → file ghi kết quả

---

# 🧹 **4. Clean (xóa file build)**

```bash
make clean
```

Lệnh này xoá:

* file thực thi `indexer`
* các file `.o` nếu có

---

# 📦 **5. Đóng gói bài nộp**

Dùng:

```bash
make zip
```

Lệnh này sẽ tạo:

```
DoManhPhuong_20225660.zip
```

Bao gồm:

* `main.c`
* `Makefile`
* `stopw.txt`
* `README.md`

Không bao gồm các file build.
