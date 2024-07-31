#include <WiFi.h>
#include <esp_now.h>
#include <TFT_eSPI.h>
#include "Image1.h"
#include <math.h>

// Khởi tạo màn hình TFT
TFT_eSPI tft = TFT_eSPI();

float h, w, distance;
int h1, w1;

// Định nghĩa cấu trúc dữ liệu để nhận từ ESP-NOW
typedef struct struct_message {
  int val1;
  int val2;
} struct_message;

struct_message myData;

unsigned long previousMillis = 0;  
const long interval = 100;         

// Hàm callback khi nhận dữ liệu từ ESP-NOW
void OnDataRecv(const uint8_t *mac, const uint8_t *incomingData, int len) {
  memcpy(&myData, incomingData, sizeof(myData));
  unsigned long currentMillis = millis();
  if (currentMillis - previousMillis >= interval) {
    previousMillis = currentMillis; 
    backgroud();
  }
  poin(myData.val1, myData.val2);
}

void setup() {
  Serial.begin(115200);
  tft.begin();            // Khởi tạo màn hình TFT
  tft.init();
  tft.setSwapBytes(true);
  tft.setRotation(1);     // Xoay màn hình
  backgroud();            // Vẽ nền ban đầu

  WiFi.mode(WIFI_STA);    // Thiết lập chế độ Wi-Fi ở chế độ station (WIFI_STA)
  esp_now_init();         // Khởi tạo ESP-NOW

  // Đăng ký hàm callback khi nhận dữ liệu từ ESP-NOW
  esp_now_register_recv_cb(esp_now_recv_cb_t(OnDataRecv));
}

void loop() {
  // Hàm loop để trống vì mọi thứ được xử lý trong callback OnDataRecv
}

// Hàm để vẽ các hình ảnh lên màn hình TFT dựa trên giá trị width và height
void poin(int width, int height) {
  if (width > 0) {
    tft.fillRect(162, 10, width, 10, TFT_BLUE); // Vẽ hình chữ nhật màu xanh
  } else if (width < 0) {
    tft.fillRect(160 + width, 10, -width, 10, TFT_BLUE); // Vẽ hình chữ nhật màu xanh
  }
  if (height > 0) {
    tft.fillRect(10, 120 - height, 10, height, TFT_BLUE); // Vẽ hình chữ nhật màu xanh
  } else if (height < 0) {
    tft.fillRect(10, 122, 10, -height, 10, TFT_BLUE); // Vẽ hình chữ nhật màu xanh
  }

  // Tính khoảng cách Euclidean
  distance = sqrt(width * width + height * height);

  if (distance <= 100) {
    tft.fillCircle(160 + width, 130 - height, 5, TFT_RED); // Vẽ hình tròn màu đỏ
  } else {
    h = (height * 100) / distance;
    w = (width * 100) / distance;
    h1 = (int)h;
    w1 = (int)w;
    tft.fillCircle(160 + w1, 130 - h1, 5, TFT_RED); // Vẽ hình tròn màu đỏ
  }
}

// Hàm để vẽ nền ban đầu lên màn hình TFT
void backgroud() {
  tft.pushImage(0, 0, 320, 240, image); // Hiển thị hình ảnh nền
  tft.drawCircle(160, 130, 100, TFT_BLACK); // Vẽ hình tròn màu đen

  tft.drawLine(160, 125, 160, 135, TFT_BLACK); // Vẽ đường thẳng màu đen
  tft.drawLine(155, 130, 165, 130, TFT_BLACK); // Vẽ đường thẳng màu đen

  tft.drawRect(58, 9, 204, 12, TFT_BLACK); // Vẽ hình chữ nhật màu đen
  tft.fillRect(160, 10, 2, 10, TFT_BLACK); // Vẽ hình chữ nhật màu đen
  tft.drawRect(9, 19, 12, 204, TFT_BLACK); // Vẽ hình chữ nhật màu đen
  tft.fillRect(10, 120, 10, 2, TFT_BLACK); // Vẽ hình chữ nhật màu đen
}
