# DDU on Display
This is a school project entailing the use of an M5StickCPlus in order to make a game meant to teach others about the usage of an M5.

## Usage
Before you can use the code in this repo you must install and set up Arduino IDE

### Installation
**Arch Linux**\
`# pacman -S arduino`

**Windows**\
[https://wiki-content.arduino.cc/en/Guide/Windows](https://wiki-content.arduino.cc/en/Guide/Windows)

### Configuration
After intalling the Arduino IDE you must install the libraries and boards required to make the M5 work

**Boards**
* Arduino ESP32 Boards
* esp32

**Libraries**
 * M5StickCPlus (+ its dependencies)
 * FastLED

After installing the dependencies configure the board type to be M5Stick-C (Tools->Board->ESP32 Arduino->M5Stick-C)

### Flashing
After opening *m5stickcplus/m5stickcplus.ino* in the Arduino IDE you just have to press the Upload button (→) which will run the program.