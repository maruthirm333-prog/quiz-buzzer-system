/*
  6-Player Quiz Buzzer System — Version B (Ranking System)
  ESP32 + OLED SSD1306

  Press first  → ranked 1st, buzzer sounds
  Others press → order recorded (2nd–6th)
  Host RESET   → clears everything, ready for next question

  Libraries:
    Adafruit SSD1306
    Adafruit GFX
*/

#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

// ── OLED ──────────────────────────────────────────────────────
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_RESET   -1
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

// ── PINS ──────────────────────────────────────────────────────
const int PLAYER_PINS[6] = {13, 14, 27, 26, 25, 33};  // Players 1–6
const int RESET_PIN      = 32;                          // Host reset
const int BUZZER_PIN     = 23;                          // Active buzzer

// ── STATE ─────────────────────────────────────────────────────
int  rankOrder[6];     // rankOrder[0] = first player to press, etc.
bool pressed[6];       // has this player pressed?
int  pressCount = 0;   // how many have pressed so far
bool roundActive = true;

// ── DEBOUNCE ──────────────────────────────────────────────────
unsigned long lastDebounce[6] = {0};
const int DEBOUNCE_MS = 50;


// ── DISPLAY: IDLE SCREEN ──────────────────────────────────────
void showIdle() {
  display.clearDisplay();
  display.setTextColor(SSD1306_WHITE);

  display.setTextSize(1);
  display.setCursor(15, 0);
  display.println("Quiz Buzzer System");

  display.drawLine(0, 10, 127, 10, SSD1306_WHITE);

  display.setTextSize(1);
  display.setCursor(10, 18);
  display.println("Waiting for players");
  display.setCursor(20, 32);
  display.println("Press your button!");

  display.display();
}


// ── DISPLAY: RANKING SCREEN ───────────────────────────────────
void showRanking() {
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);

  display.setCursor(15, 0);
  display.println("Quiz Buzzer System");
  display.drawLine(0, 10, 127, 10, SSD1306_WHITE);

  const char* suffixes[] = {"st", "nd", "rd", "th", "th", "th"};

  for (int i = 0; i < pressCount; i++) {
    display.setCursor(0, 13 + i * 9);
    display.print(i + 1);
    display.print(suffixes[i]);
    display.print(" : Player ");
    display.println(rankOrder[i] + 1);
  }

  display.display();
}


// ── BUZZER: SHORT BEEP ────────────────────────────────────────
void beep(int ms = 200) {
  digitalWrite(BUZZER_PIN, HIGH);
  delay(ms);
  digitalWrite(BUZZER_PIN, LOW);
}


// ── RESET ────────────────────────────────────────────────────
void resetRound() {
  pressCount  = 0;
  roundActive = true;

  for (int i = 0; i < 6; i++) {
    pressed[i]      = false;
    rankOrder[i]    = -1;
    lastDebounce[i] = 0;
  }

  // Double beep = reset confirmed
  beep(100); delay(80); beep(100);

  showIdle();
  Serial.println("── RESET — Ready for next question ──");
}


// ── SETUP ────────────────────────────────────────────────────
void setup() {
  Serial.begin(115200);

  // Player buttons + reset
  for (int i = 0; i < 6; i++)
    pinMode(PLAYER_PINS[i], INPUT_PULLUP);
  pinMode(RESET_PIN, INPUT_PULLUP);

  // Buzzer
  pinMode(BUZZER_PIN, OUTPUT);
  digitalWrite(BUZZER_PIN, LOW);

  // OLED
  if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
    Serial.println("OLED not found — check wiring");
    while (1);
  }
  display.clearDisplay();

  // Init state
  resetRound();

  Serial.println("Quiz Buzzer System ready.");
}


// ── LOOP ─────────────────────────────────────────────────────
void loop() {

  // ── Host RESET button ──
  if (digitalRead(RESET_PIN) == LOW) {
    delay(50);
    if (digitalRead(RESET_PIN) == LOW) {
      resetRound();
      while (digitalRead(RESET_PIN) == LOW) delay(10);
    }
  }

  // ── Player buttons ──
  for (int i = 0; i < 6; i++) {

    if (pressed[i]) continue;  // already registered this player

    if (digitalRead(PLAYER_PINS[i]) == LOW) {

      // Debounce
      if (millis() - lastDebounce[i] < DEBOUNCE_MS) continue;
      lastDebounce[i] = millis();

      // Register press
      pressed[i]              = true;
      rankOrder[pressCount]   = i;
      pressCount++;

      Serial.print("Player ");
      Serial.print(i + 1);
      Serial.print(" pressed — ranked ");
      Serial.println(pressCount);

      // Buzzer only on first press
      if (pressCount == 1) beep(300);

      // Update display
      showRanking();

      // Wait for button release
      while (digitalRead(PLAYER_PINS[i]) == LOW) delay(10);
    }
  }
}
