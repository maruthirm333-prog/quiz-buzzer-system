# 6-Player Quiz Buzzer System ⚡🎯

**Version B — Ranking System**

> Press first. Get ranked. Smart & Simple.

![Project Overview](images/project-overview.png)

---

## How It Works

| Step | What happens |
|------|-------------|
| 1 | Host asks a question |
| 2 | First player presses → **buzzer sounds**, ranked **1st** |
| 3 | Others can still press → order recorded (2nd–6th) |
| 4 | OLED shows full ranking of all 6 players |
| 5 | Host presses RESET → clears everything, ready for next question |

---

## Hardware

| Component | Qty | Note |
|-----------|-----|------|
| ESP32 DevKit V1 | 1 | |
| Push buttons | 7 | 6 players + 1 host reset |
| OLED SSD1306 (128×64, I2C) | 1 | Shows ranking |
| Active buzzer (3.3V–5V) | 1 | Sounds on first press only |
| LEDs (optional) | 6 | One per player — Phase 2 |

---

## Pin Assignments

### Player Buttons → GND (INPUT_PULLUP — no resistors needed)

| Player | GPIO |
|--------|------|
| Player 1 | 13 |
| Player 2 | 14 |
| Player 3 | 16 |
| Player 4 | 17 |
| Player 5 | 18 |
| Player 6 | 19 |
| Host Reset | 23 |
| Buzzer (+) | 4 |

### OLED SSD1306 (I2C)

| OLED | ESP32 |
|------|-------|
| VCC | 3.3V |
| GND | GND |
| SDA | GPIO 21 |
| SCL | GPIO 22 |

---

## OLED Output

```
     QUIZ RANKING
──────────────────
1st : PLAYER 4
2nd : PLAYER 2
3rd : PLAYER 6
4th : PLAYER 1
5th : PLAYER 5
6th : PLAYER 3
```

---

## Firmware

| File | Status |
|------|--------|
| [firmware/quiz_buzzer_v2/quiz_buzzer_v2.ino](firmware/quiz_buzzer_v2/quiz_buzzer_v2.ino) | ✅ Working |

**Libraries needed (Arduino Library Manager):**
- Adafruit GFX Library
- Adafruit SSD1306

---

## Build Phases

| Phase | Feature | Status |
|-------|---------|--------|
| 1 | Buttons + OLED + buzzer (Version B) | ✅ Done |
| 2 | Individual LEDs per player (P1–P6) | 📋 Next |
| 3 | Large arcade buttons | 📋 Planned |
| 4 | Perfboard / PCB wiring | 📋 Planned |
| 5 | Enclosure build | 📋 Planned |
| 6 | Reaction time display (e.g. `P4: 1st — 0.82 sec`) | 📋 Planned |
| 7 | Score tracking across rounds — full Quiz Competition System | 📋 Future |

---

## Applications

- 🏆 Quiz competitions
- 🏫 Classroom activities
- 🎓 College events & exhibitions
- 👥 Team building games
- 🎮 Fun learning system

---

Part of [EC Simplified](https://github.com/maruthirm333-prog/ec-simplified) — Maruthi R M
