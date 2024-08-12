# Bullet Barrage (Minigame)

## About Me

- **Full Name:** Nguyễn Duy Hưng
- **Student ID:** 22028264
- **Course:** 2324H_INT2215_70

## Game Introduction

**Bullet Barrage** là một minigame, nơi người chơi phải né tránh các làn đạn được bắn ra từ mọi hướng. Mục tiêu của trò chơi là sống sót lâu nhất có thể bằng cách di chuyển để tránh bị trúng đạn.

## How to Play

1. **Starting the Game:** Trò chơi bắt đầu khi các viên đạn được bắn ra từ các cạnh của đấu trường. Người chơi sử dụng các phím điều khiển để di chuyển nhân vật và né tránh đạn.
2. **Controlling the Character:** Sử dụng các phím mũi tên để di chuyển nhân vật lên, trái hoặc phải.
3. **Winning the Game:** Trò chơi kết thúc khi người chơi hết 3 máu.
4. **Strategy:** Duy trì sự di chuyển liên tục và tìm kiếm các khoảng trống giữa các làn đạn để có thể sống sót lâu nhất.

## A few images of the game

[Include screenshots or visuals of the game here]

## Game Structure

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


## Installation Instructions

1. **Download**: Tải tệp trò chơi về máy tính của bạn.
2. **Extract**: Giải nén nội dung của tệp.
3. **Run**: Tìm tệp `BulletBarrage.exe`, mở nó và thưởng thức trò chơi.
