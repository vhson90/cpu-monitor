# Hệ Thống Theo Dõi Hiệu Suất Máy Tính bằng MQTT

## 📌 Giới thiệu
Dự án này xây dựng một hệ thống giám sát hiệu suất máy tính theo thời gian thực bằng cách thu thập dữ liệu từ cảm biến trên laptop, truyền qua giao thức **MQTT**, lưu trữ vào **MySQL**, và hiển thị trên web sử dụng **Node.js**, **Socket.io** và **Chart.js**.

## ⚡ Chức năng chính
- Đọc thông tin cảm biến trên laptop: 
  - 🔥 **Nhiệt độ CPU**
  - 💾 **Nhiệt độ SSD**
  - 🔋 **Dung lượng pin**
  - ⚡ **Điện áp pin**
- Truyền dữ liệu qua **MQTT** đến server.
- Lưu trữ dữ liệu vào **MySQL**.
- Gửi dữ liệu theo thời gian thực đến client bằng **Socket.io**.
- Hiển thị dữ liệu dưới dạng biểu đồ trên web.

## 🛠 Cài đặt và chạy hệ thống

### 1️⃣ Cài đặt môi trường
Trên Ubuntu, chạy các lệnh sau để cài đặt các thư viện cần thiết:

```sh
sudo apt update
sudo apt install mysql-server libpaho-mqtt-dev libmysqlclient-dev
```

Cài đặt Node.js và các thư viện:

```sh
npm install express mysql2 socket.io cors
```

### 2️⃣ Cấu hình MQTT Broker
Dự án sử dụng **HiveMQ** làm broker công khai, hoặc bạn có thể thiết lập broker riêng.

### 3️⃣ Chạy chương trình
#### 🚀 Gửi dữ liệu cảm biến lên MQTT:
```sh
gcc pub.c -o pub -lpaho-mqtt3c
./pub
```

#### 🛠 Nhận dữ liệu và lưu vào MySQL:
```sh
gcc sub.c -o sub -lpaho-mqtt3c -lmysqlclient
./sub
```

#### 🌐 Chạy server web:
```sh
node server.js
```

Mở file 'index.html' trên trình duyệt để xem dữ liệu cập nhật theo thời gian thực.

## 📂 Cấu trúc thư mục
```
📂 cpu-monitor
 ├── 📁 project
 │   ├── pub.c              # Chương trình gửi dữ liệu cảm biến lên MQTT
 │   ├── sub.c              # Chương trình nhận dữ liệu và lưu vào MySQL
 ├── 📁 web
 │   ├── server.js          # Server Node.js xử lý dữ liệu và gửi tới client
 │   ├── index.html         # Giao diện web hiển thị dữ liệu
 ├── README.md              # Tài liệu hướng dẫn

```

## 🔗 Liên kết
- **GitHub Repo**: [https://github.com/vhson90/cpu-monitor](https://github.com/vhson90/cpu-monitor)
- **Video Demo**: [https://youtu.be/GS-CXc_Z95Q](https://youtu.be/GS-CXc_Z95Q)
