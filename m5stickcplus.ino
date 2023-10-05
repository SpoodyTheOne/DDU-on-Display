/*
 *  This sketch sends random data over UDP on a ESP32 device
 *
 */
#include <M5StickCPlus.h>
#include <FastLED.h>

#define Neopixel_PIN 32

#define LEDS_PER_STRIP 30
#define STRIPS 4
// 16ms
#define FPS 10

#define LEDS (STRIPS * LEDS_PER_STRIP + 20)

CRGB leds[LEDS];
CRGB screen_buffer[LEDS];

struct Enemy {
  int x = 0;
  int y = 0;
  int height = 0;
  int width = 0;
  int yd = 0;
  int xd = 0;
  int interval = 0;
  int speed = 1;
  int modulus = 2;
};

#define NUM_ENEMIES 10

Enemy enemies[NUM_ENEMIES];

int lost = 0;
int win = 0;
int playerPos = 0;

void gameOver() {
  lost = 1;
  for (int i = 0; i < LEDS; i++) {
    screen_buffer[i] = CRGB::Red;
  }
}

int winAnim = 0;

void gameWin() {
  lost = 1;
  win = 1;
  winAnim = 0;
  FastLED.setBrightness(30);
}

void fillEnemy(Enemy enemy, CRGB color, int check = 0) {
  for (int x = 0; x < enemy.width; x++) {
    for (int y = 0; y < enemy.height; y++) {
      int p = ( x + enemy.x ) + ( y + enemy.y + enemy.interval) * LEDS_PER_STRIP;
      if (p < 0 || p >= LEDS)
        continue;
      screen_buffer[p] = color;
    }
  }
}

void drawEnemy(Enemy enemy) {
  fillEnemy(enemy, CRGB::Red, 1);
}

void clearEnemy(Enemy enemy) {
  fillEnemy(enemy, CRGB::Black);
}

void tickEnemy(Enemy *enemy) {
  enemy->interval = enemy->interval + enemy->speed;

  if (enemy->interval <= 0 || enemy->interval >= enemy->yd) {
    enemy->speed *= -1;
  }
}

void setup(){
  // Initilize hardware serial:
  // Serial.begin(115200);
  
  M5.begin();
  M5.Lcd.fillScreen(BLACK);

  M5.Lcd.setRotation(1);

  FastLED.addLeds<WS2811, Neopixel_PIN, GRB>(leds, LEDS)
        .setCorrection(TypicalLEDStrip);
  FastLED.setBrightness(69);
  
  xTaskCreatePinnedToCore(FastLEDshowTask, "FastLEDshowTask", 2048, NULL, 2,
                            NULL, 1);
  
  enemies[0].x = 5;
  enemies[0].y = 0;
  enemies[0].width = 1;
  enemies[0].height = 1;
  enemies[0].yd = 2;
  enemies[0].modulus = 4;

  enemies[1].x = 18;
  enemies[1].y = -5;
  enemies[1].width = 1;
  enemies[1].height = 5;
  enemies[1].yd = 10;
  enemies[1].modulus = 1;

  enemies[2].x = 3;
  enemies[2].y = 4;
  enemies[2].width = 1;
  enemies[2].height = 1;
  enemies[2].yd = 1;
  enemies[2].modulus = 5;

  enemies[3].x = 6;
  enemies[3].y = 4;
  enemies[3].width = 1;
  enemies[3].height = 1;
  enemies[3].yd = 1;
  enemies[3].modulus = 4;

  enemies[4].x = 9;
  enemies[4].y = 4;
  enemies[4].width = 1;
  enemies[4].height = 1;
  enemies[4].yd = 1;
  enemies[4].modulus = 3;

  enemies[5].x = 12;
  enemies[5].y = 4;
  enemies[5].width = 1;
  enemies[5].height = 1;
  enemies[5].yd = 1;
  enemies[5].modulus = 2;

  enemies[6].x = 15;
  enemies[6].y = 4;
  enemies[6].width = 1;
  enemies[6].height = 1;
  enemies[6].yd = 1;
  enemies[6].modulus = 2;

  enemies[7].x = 26;
  enemies[7].y = 0;
  enemies[7].width = 2;
  enemies[7].height = 1;
  enemies[7].yd = 3;
  enemies[7].modulus = 2;
}

int tick = 0;

int btn = 0;
int pBtn = 0;

void loop() {
  btn = digitalRead(0);
  
  if (lost || win) {
    if (btn == 0 && pBtn) {
      lost = 0;
      playerPos = 0;

      FastLED.setBrightness(69);
      
      for (int i = 0; i < LEDS; i++) {
        screen_buffer[i] = CRGB::Black;
      }
    }
    pBtn = btn;
    return;
  }
 
  if (btn == 0) {
    M5.Lcd.fillScreen(GREEN);
    playerPos++;

    if (playerPos == LEDS) {
      gameWin();
    }
  } else {
    M5.Lcd.fillScreen(BLACK);
  }

  for (int i = 0; i < NUM_ENEMIES; i++) {

    if (tick % enemies[i].modulus == 0) {
      clearEnemy(enemies[i]);
      tickEnemy(&enemies[i]);
      drawEnemy(enemies[i]);
    }
  }

  tick++;
  
  pBtn = btn;
  
  delay(100);

}

void swap(CRGB *a, CRGB *b) {
  CRGB temp = *a;
  *a = *b;
  *b = temp;
}

int led = 0;

void FastLEDshowTask(void *pvParameters) {
  for (;;) {
    
    if (!win) {
      memcpy(leds,screen_buffer, sizeof(CRGB) * LEDS);
    
      for (int i = 0; i < STRIPS; i++) {
        // Is the strip even?
        if (i % 2 == 1) {
          // Copy strip into variable row
          CRGB row[LEDS_PER_STRIP];
          memcpy(row, &screen_buffer[LEDS_PER_STRIP * i], sizeof(CRGB) * LEDS_PER_STRIP);
      
          // Reverse order of row
          for (int index = 0; index < LEDS_PER_STRIP / 2; index++) {
            swap(row + index, row + LEDS_PER_STRIP - 1 - index);
          }

          // Copy row back into flipped_leds at the same position it was copied from
          memcpy(&leds[LEDS_PER_STRIP * i], row,
                 sizeof(CRGB) * LEDS_PER_STRIP);
        }
      }
    
      if (leds[playerPos] == CRGB::Red)
        gameOver();
      else
        leds[playerPos] = CRGB::Yellow;
    }

    FastLED.show();

    EVERY_N_MILLISECONDS(FPS) {
      if (win) {
        leds[winAnim++] = CRGB::Yellow;

        if (winAnim >= LEDS-1) {
          win = 0;
          playerPos = 0;
        }  
      } else
        led = (led + 1)%LEDS;
    }
  }
}