/*
  6-Player Quiz Buzzer System — Version B (Ranking System)
  ESP32 + OLED SSD1306 (128x64, I2C)

  Version B behaviour:
    - First press  → rank 1, buzzer sounds
    - Others press → rank 2–6 recorded
    - OLED shows full ranking live
    - Host RESET   → clears all, ready for next question

  Libraries (install via Arduino Library Manager):
    Adafruit GFX Library
    Adafruit SSD1306
*/

#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

// ── OLED ──────────────────────────────────────────────────────
#define SCREEN_WIDTH  128
#define SCREEN_HEIGHT  64
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);

// ── PINS ──────────────────────────────────────────────────────
const int playerButtons[6] = {13, 14, 16, 17, 18, 19};  // P1–P6
const int buzzerPin   = 4;
const int resetButton = 23;

// ── STATE ─────────────────────────────────────────────────────
int  ranking[6]          = {0};
int  rankCount           = 0;
unsigned long lastPressTime[6] = {0};
const unsigned long DEBOUNCE   = 150;


// ── SETUP ────────────────────────────────────────────────────
void setup() {
  Serial.begin(115200);

  for (int i = 0; i < 6; i++)
    pinMode(playerButtons[i], INPUT_PULLUP);

  pinMode(resetButton, INPUT_PULLUP);
  pinMode(buzzerPin, OUTPUT);
  digitalWrite(buzzerPin, LOW);

  Wire.begin(21, 22);   // SDA=21, SCL=22
  if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
    Serial.println("OLED not found — check SDA/SCL wiring");
    while (true);
  }

  display.clearDisplay();
  display.setTextColor(SSD1306_WHITE);
  showReadyScreen();
  Serial.println("Quiz Buzzer System ready.");
}


// ── LOOP ─────────────────────────────────────────────────────
void loop() {

  // Reset button
  if (digitalRead(resetButton) == LOW) {
    delay(30);
    if (digitalRead(resetButton) == LOW) {
      resetGame();
      while (digitalRead(resetButton) == LOW) delay(5);
    }
  }

  // Player buttons
  for (int i = 0; i < 6; i++) {
    if (digitalRead(playerButtons[i]) == LOW) {
      unsigned long now = millis();
      if (now - lastPressTime[i] > DEBOUNCE) {
        lastPressTime[i] = now;
        registerPlayer(i + 1);
        while (digitalRead(playerButtons[i]) == LOW) delay(5);
      }
    }
  }
}


// ── REGISTER PLAYER ──────────────────────────────────────────
void registerPlayer(int playerNumber) {

  // Already pressed?
  for (int i = 0; i < rankCount; i++)
    if (ranking[i] == playerNumber) return;

  if (rankCount >= 6) return;

  ranking[rankCount] = playerNumber;
  rankCount++;

  Serial.print("Rank "); Serial.print(rankCount);
  Serial.print(" = Player "); Serial.println(playerNumber);

  // Buzzer only on first press
  if (rankCount == 1) {
    digitalWrite(buzzerPin, HIGH);
    delay(300);
    digitalWrite(buzzerPin, LOW);
  }

  showRanking();
}


// ── READY SCREEN ─────────────────────────────────────────────
void showReadyScreen() {
  display.clearDisplay();

  display.setTextSize(2);
  display.setCursor(10, 5);
  display.println("QUIZ");
  display.setCursor(5, 28);
  display.println("BUZZER");

  display.setTextSize(1);
  display.setCursor(20, 52);
  display.println("Press your button!");

  display.display();
}


// ── RANKING SCREEN ───────────────────────────────────────────
void showRanking() {
  display.clearDisplay();
  display.setTextSize(1);

  display.setCursor(25, 0);
  display.println("QUIZ RANKING");
  display.drawLine(0, 10, 127, 10, SSD1306_WHITE);

  const char* suffix[] = {"st","nd","rd","th","th","th"};

  for (int i = 0; i < rankCount; i++) {
    display.setCursor(10, 13 + i * 9);
    display.print(i + 1);
    display.print(suffix[i]);
    display.print(" : PLAYER ");
    display.print(ranking[i]);
  }

  display.display();
}


// ── RESET ────────────────────────────────────────────────────
void resetGame() {
  rankCount = 0;
  for (int i = 0; i < 6; i++) { ranking[i] = 0; lastPressTime[i] = 0; }
  digitalWrite(buzzerPin, LOW);
  Serial.println("\n── NEW QUESTION ──");
  showReadyScreen();
}
