# Bullet Barrage (Minigame)

## Thông tin

- **Full Name:** Nguyễn Duy Hưng
- **Student ID:** 22028264
- **Course:** 2324H_INT2215_70

## Giới thiệu game

**Bullet Barrage** là một minigame, nơi người chơi phải né tránh các làn đạn được bắn ra từ mọi hướng. Mục tiêu của trò chơi là sống sót lâu nhất có thể bằng cách di chuyển để tránh bị trúng đạn.

## Cách để chơi

1. **Starting the Game:** Trò chơi bắt đầu khi các viên đạn được bắn ra từ các cạnh của đấu trường. Người chơi sử dụng các phím điều khiển để di chuyển nhân vật và né tránh đạn.
2. **Controlling the Character:** Sử dụng các phím mũi tên để di chuyển nhân vật lên, trái hoặc phải.
3. **Winning the Game:** Trò chơi kết thúc khi người chơi hết 3 máu.
4. **Strategy:** Duy trì sự di chuyển liên tục và tìm kiếm các khoảng trống giữa các làn đạn để có thể sống sót lâu nhất.

## Bắt đầu game

Khi bắt đầu, chúng ta sẽ xuất phát với màn hình Menu. 
![image](https://github.com/user-attachments/assets/3be2d124-382c-4ca3-8bab-a29c9a0e2631)
Khi chọn Setting, chúng ta được điều chỉnh âm lượng và thay đổi nhạc nền. 
![image](https://github.com/user-attachments/assets/adff53cc-8c24-4d76-8d73-8c6d3f402fd6)
Khi chọn Score, chúng ta được xem top 10 điểm số cao nhất mà người chơi từng đạt được.
![image](https://github.com/user-attachments/assets/1cdb8628-5a0a-4335-9aec-ea44603370fd)
Khi chọn Play, chúng ta sẽ đến với màn chơi chính. Nơi nhân vật phải né toàn bộ những viên đạn bắn ra từ mọi phía để có thể sống sót.
![image](https://github.com/user-attachments/assets/892e684e-a02c-43fa-9c82-19dd82963842)

## Các thành phần trong game

-    Nhân vật chính <img src="https://github.com/user-attachments/assets/420b41b7-1c12-4d56-a217-a3d0a98b3bf6" width="30" height="60"> : Nhân vật chính của game.
-    Pause  <img src="https://github.com/user-attachments/assets/ae77960a-9b8d-4324-888a-2af1a2199501" width="30" height="30"> : Bấm để tạm dừng và thực hiện các điều chỉnh thông số.   
-    Home <img src="https://github.com/user-attachments/assets/f1699fc6-6e0d-48ae-b33a-586d39694f8a" width="30" height="30"> : Bấm để trở về màn hình chính.
-    Resume <img src="https://github.com/user-attachments/assets/b08332a9-49b6-4ca5-bb75-3f3d6361dc4a" width="30" height="30"> : Bấm để tiếp tục chơi khi đang pause.
-    Replay <img src="https://github.com/user-attachments/assets/f14b81a5-d871-4ebc-bbdd-3174ba50d479" width="30" height="30">: Bấm để chơi lại sau khi đã thua.
-    Bật/tắt âm thanh<img src="https://github.com/user-attachments/assets/a7f701a8-c989-4c2e-9653-b111a18773b0" width="30" height="30"> <img src="https://github.com/user-attachments/assets/13c7e797-6f29-4f3d-94c8-2141c4bb6633" width="30" height="30"> 
-    Máu của nhân vật <img src="https://github.com/user-attachments/assets/4b7ddfe2-30b3-40b8-bffd-181770a7f0c2" width="30" height="30"> : Lượng máu nhân vật còn lại.


## Cấu trúc game

Cấu trúc của trò chơi **Bullet Barrage** được tổ chức như sau:

### **Main Components**
- **`main.cpp & main.h`**: Tệp chính quản lý vòng lặp sự kiện, xử lý các màn hình khác nhau (MENU, GAME, SCORE, SETTING), và điều khiển hiển thị.

### **Menu Management**
- **`Menu.cpp & Menu.h`**: Quản lý giao diện menu, xử lý sự kiện và hiển thị các nút menu. Bao gồm các nút như "Play", "Score", "Setting", và "Exit".

### **Player Control**
- **`Player.cpp & Player.h`**: Định nghĩa hành vi của nhân vật chính, bao gồm di chuyển, nhảy, và xử lý sự kiện bàn phím. Quản lý các hiệu ứng âm thanh và trạng thái sức khỏe của nhân vật.

### **Background Management**
- **`Background.cpp & Background.h`**: Quản lý hình nền động, bao gồm việc tải và cập nhật các khung hình nền.

### **Collision Detection**
- **`Collision.cpp & Collision.h`**: Cung cấp chức năng kiểm tra va chạm giữa các đối tượng bằng cách xác định xem hai vật thể có giao nhau hay không.

### **Threat Management**
- **`Threat.cpp & Threat.h`**: Định nghĩa các mối đe dọa trong trò chơi như đạn và bom, bao gồm việc cập nhật và hiển thị chúng trên màn hình. Quản lý vị trí và vận tốc của các đối tượng này.

### **Score Management**
- **`Score.cpp & Score.h`**: Quản lý điểm số của người chơi, lưu trữ và tải điểm số từ tệp tin. Hiển thị bảng điểm cao trong trò chơi.

### **Setting Management**
- **`Setting.cpp & Setting.h`**: Quản lý các thiết lập của trò chơi như âm thanh, nhạc nền. Bao gồm các chức năng điều chỉnh âm lượng và thay đổi nhạc nền.

### **Play Screen Management**
- **`PlayScreen.cpp & PlayScreen.h`**: Quản lý màn hình chơi chính, bao gồm việc xử lý các sự kiện trong trò chơi, cập nhật và hiển thị các yếu tố trò chơi như nhân vật chính, mối đe dọa và điểm số. Quản lý các mẫu đạn bắn ra và xử lý va chạm.

### **Score Screen Management**
- **`ScoreScreen.cpp & ScoreScreen.h`**: Hiển thị bảng điểm cao và xử lý các sự kiện liên quan đến màn hình điểm số. Cho phép người chơi quay lại menu chính từ màn hình này.


## Cách tải game

1. **Download**: Tải tệp trò chơi về máy tính của bạn.
2. **Extract**: Giải nén nội dung của tệp.
3. **Run**: Tìm tệp `BulletBarrage.exe`, mở nó và thưởng thức trò chơi.
