#include "WiFi.h"         // Thư viện Wi-Fi của ESP32
#include "esp_now.h"      // Thư viện ESP-NOW để giao tiếp không dây

// Định nghĩa các chân analog mà joystick sẽ được kết nối
int x = 32;
int y = 33;

// Biến để lưu giá trị đã xử lý từ joystick
int X, Y;

// Biến dùng để theo dõi thời gian cho việc gửi dữ liệu định kỳ
unsigned long previousMillis = 0;

// Khoảng thời gian giữa các lần gửi dữ liệu (10ms)
const long interval = 10;

// Địa chỉ MAC của thiết bị nhận dữ liệu qua ESP-NOW
uint8_t broadcastAddress[] = { 0x08, 0xB6, 0x1F, 0xB8, 0xE3, 0x68 };

// Định nghĩa cấu trúc dữ liệu để gửi đi qua ESP-NOW
typedef struct struct_message {
  int val1;  // Giá trị X đã xử lý từ joystick
  int val2;  // Giá trị Y đã xử lý từ joystick
} struct_message;

// Biến chứa dữ liệu cần gửi
struct_message myData;

// Cấu trúc thông tin của thiết bị nhận trong ESP-NOW
esp_now_peer_info_t peerInfo;

void setup() {
  pinMode(x, INPUT);  // Cấu hình chân x là đầu vào để đọc giá trị joystick
  pinMode(y, INPUT);  // Cấu hình chân y là đầu vào để đọc giá trị joystick
  WiFi.mode(WIFI_STA);  // Thiết lập chế độ Wi-Fi ở chế độ station (WIFI_STA)
  esp_now_init();  // Khởi tạo ESP-NOW

  // Cấu hình thông tin của thiết bị nhận
  memcpy(peerInfo.peer_addr, broadcastAddress, 6);  // Sao chép địa chỉ MAC của thiết bị nhận
  peerInfo.channel = 0;  // Kênh truyền mặc định
  peerInfo.encrypt = false;  // Không mã hóa dữ liệu truyền

  // Thêm thiết bị nhận vào mạng ESP-NOW
  esp_now_add_peer(&peerInfo);
}

void loop() {
  sending();  // Gọi hàm gửi dữ liệu trong mỗi vòng lặp
}

void sending() {
  // Đọc giá trị từ các chân analog được kết nối với joystick
  int XVal = analogRead(x);
  int YVal = analogRead(y);

  // Xử lý giá trị X của joystick
  if ((999 <= XVal) && (XVal <= 1009)) {
    X = 0;  // Nếu giá trị nằm trong khoảng trung tính, đặt X = 0
  } else if (XVal > 1009) {
    X = map(XVal, 1607, 1009, -100, 0);  // Ánh xạ giá trị sang phạm vi -100 đến 0
    if (X < -100) {
      X = -100;  // Đảm bảo giá trị không vượt quá -100
    }
  } else if (XVal < 999) {
    X = map(XVal, 999, 429, 0, 100);  // Ánh xạ giá trị sang phạm vi 0 đến 100
    if (X > 100) {
      X = 100;  // Đảm bảo giá trị không vượt quá 100
    }
  }

  // Xử lý giá trị Y của joystick
  if ((1072 <= YVal) && (YVal <= 1082)) {
    Y = 0;  // Nếu giá trị nằm trong khoảng trung tính, đặt Y = 0
  } else if (YVal > 1082) {
    Y = map(YVal, 1082, 1599, 0, 100);  // Ánh xạ giá trị sang phạm vi 0 đến 100
    if (Y > 100) {
      Y = 100;  // Đảm bảo giá trị không vượt quá 100
    }
  } else if (YVal < 1072) {
    Y = map(YVal, 1072, 503, 0, -100);  // Ánh xạ giá trị sang phạm vi 0 đến -100
    if (Y < -100) {
      Y = -100;  // Đảm bảo giá trị không vượt quá -100
    }
  }

  // Cập nhật giá trị vào cấu trúc dữ liệu cần gửi
  myData.val1 = X;
  myData.val2 = Y;

  // Kiểm tra thời gian để gửi dữ liệu định kỳ
  unsigned long currentMillis = millis();
  if (currentMillis - previousMillis >= interval) {
    previousMillis = currentMillis;  // Cập nhật thời gian lần gửi cuối
    // Gửi dữ liệu qua ESP-NOW
    esp_err_t result = esp_now_send(broadcastAddress, (uint8_t *)&myData, sizeof(myData));
  }
}
