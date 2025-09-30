#include <WiFi.h>
#include <ArtronShop_LineMessaging.h>
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>

// 📡 WiFi และ LINE configuration
const char* ssid = "{PHU}";
const char* password = "12341234";
#define LINE_TOKEN "Zi4hGawOxSLBckkbR6jqw8SljA8+8YG17ewVGu+BUwd6whDugj5R3simkPspsONMvdaY3RcsVr4XB4dtB83sxAFD4Tk8fODVpk5SoK19SdZJOPF/ge5HItTgDb9SdSUoR6olNixcdsprFhLhr4/fegdB04t89/1O/w1cDnyilFU="
#define GROUP_ID "C6752ff98b5b89c6009ac597322d478b3"

int a2Counter = 0;

// 🌡️ Task A1: สุ่มค่าทุก 2 วินาที
void taskReadA1(void *pvParameters) {
  while (true) {
    int a1Value = random(0, 1024);
    Serial.printf("🔹 A1 (Simulated): %d\n", a1Value);
    vTaskDelay(pdMS_TO_TICKS(2000));
  }
}

// 🧪 Task A2: สุ่มค่าทุก 5 วินาที และส่ง LINE เมื่อครบ 5 ครั้ง
void taskReadA2(void *pvParameters) {
  while (true) {
    int a2Value = random(0, 1024);
    Serial.printf("🔸 A2 (Simulated): %d\n", a2Value);
    a2Counter++;
    Serial.printf("📊 A2 Count: %d\n", a2Counter);

    if (a2Counter >= 5 && WiFi.status() == WL_CONNECTED) {
      LINE_Messaging_Massage_Option_t option;
      option.image.url = "https://img2.pic.in.th/pic/IMG20230921232431.jpeg";

      bool success = LINE.send(GROUP_ID, "✅ ส่งค่า A2 แบบสุ่มครบ 5 ครั้งแล้ว", &option);
      if (success) {
        Serial.println("📨 LINE ส่งข้อความเรียบร้อย");
      } else {
        Serial.printf("❌ ส่ง LINE ล้มเหลว (code: %d)\n", LINE.status_code);
      }

      a2Counter = 0; // reset
    }

    vTaskDelay(pdMS_TO_TICKS(5000));
  }
}

void setup() {
  Serial.begin(115200);
  Serial.println("🚀 เริ่มต้นระบบสุ่มค่า...");

  WiFi.begin(ssid, password);
  Serial.print("📶 กำลังเชื่อมต่อ WiFi...");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.printf("\n✅ เชื่อมต่อแล้ว: %s\n", WiFi.localIP().toString().c_str());

  LINE.begin(LINE_TOKEN);

  xTaskCreate(taskReadA1, "Task_A1", 2048, NULL, 1, NULL);
  xTaskCreate(taskReadA2, "Task_A2", 6144, NULL, 1, NULL);
}

void loop() {
  // ไม่ต้องใช้ loop เมื่อใช้ FreeRTOS
}