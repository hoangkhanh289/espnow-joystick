#include <WiFi.h>          // Thư viện Wi-Fi chuẩn cho ESP32
#include <esp_wifi.h>      // Thư viện ESP-IDF để truy cập các hàm Wi-Fi cấp thấp

void setup() {
  // Khởi tạo Serial Monitor với tốc độ baud 115200
  Serial.begin(115200);

  // Khởi tạo Wi-Fi ở chế độ Station (WIFI_STA)
  WiFi.mode(WIFI_STA);
  WiFi.begin();  // Kết nối với mạng Wi-Fi nếu có cấu hình trước đó
  delay(100);  // Đợi một chút để đảm bảo Wi-Fi được khởi tạo đầy đủ
  
  // Lấy địa chỉ MAC sử dụng thư viện Wi-Fi
  String macAddress = WiFi.macAddress();
  Serial.print("MAC Address using WiFi library: ");
  Serial.println(macAddress);

  // Lấy địa chỉ MAC sử dụng hàm esp_wifi_get_mac từ ESP-IDF
  uint8_t mac[6];
  esp_wifi_get_mac(WIFI_IF_STA, mac);  // Lấy địa chỉ MAC của giao diện Station
  Serial.print("MAC Address using esp_wifi_get_mac: ");
  for (int i = 0; i < 6; i++) {
    Serial.printf("%02X", mac[i]);  // In ra từng byte của địa chỉ MAC theo định dạng hexadecimal
    if (i < 5) Serial.print(":");   // Thêm dấu ":" giữa các byte
  }
  Serial.println();
}

void loop() {
  // Không làm gì trong hàm loop
}
