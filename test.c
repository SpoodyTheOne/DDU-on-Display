#include <stdio.h>
#include <string.h>
#define LEDS_PER_STRIP 30
#define STRIPS 9
#define LEDS STRIPS *LEDS_PER_STRIP

#define ANSI_COLOR_RED "\x1b[31m"
#define ANSI_COLOR_GREEN "\x1b[32m"
#define ANSI_COLOR_YELLOW "\x1b[33m"
#define ANSI_COLOR_BLUE "\x1b[34m"
#define ANSI_COLOR_MAGENTA "\x1b[35m"
#define ANSI_COLOR_CYAN "\x1b[36m"
#define ANSI_COLOR_RESET "\x1b[0m"

#define T int

T leds[LEDS];

void printLed(T arr[], int index) {
  T led = arr[index];

  switch (led) {
  case 0:
    printf(".");
    break;
  case 1:
    printf(ANSI_COLOR_YELLOW "#");
    break;
  case 2:
    printf(ANSI_COLOR_RED "#");
    break;
  }

  printf(ANSI_COLOR_RESET);
}

void swap(T *a, T *b) {
  T temp = *a;
  *a = *b;
  *b = temp;
}

void printArray(T ledArray[]) {
  printf("[");
  for (int i = 0; i < LEDS; i++) {
    if (i > 0 && i % LEDS_PER_STRIP == 0)
      printf("]\n[");

    printLed(ledArray, i);
  }

  printf("]\n");
}

int main(int argc, char **argv) {

  leds[6] = 1;
  for (int i = 0; i < 5; i++) {
    leds[25 + LEDS_PER_STRIP * i] = 2;
  }

  int flipped_leds[LEDS];

  memcpy(flipped_leds, leds, sizeof(T) * LEDS);

  for (int i = 0; i < STRIPS; i++) {
    // Is the strip even?
    if (i % 2 == 1) {
      // Copy strip into variable row
      int row[LEDS_PER_STRIP];
      memcpy(row, &leds[LEDS_PER_STRIP * i], sizeof(T) * LEDS_PER_STRIP);

      // Reverse order of row
      for (int index = 0; index < LEDS_PER_STRIP / 2; index++) {
        swap(row + index, row + LEDS_PER_STRIP - 1 - index);
      }

      // Copy row back into flipped_leds at the same position it was copied from
      memcpy(&flipped_leds[LEDS_PER_STRIP * i], row,
             sizeof(T) * LEDS_PER_STRIP);
    }
  }

  printf("leds[]:\n");
  printArray(leds);
  printf("\n");

  printf("flipped_leds[]:\n");
  printArray(flipped_leds);
  printf("\n");

  return 0;
}