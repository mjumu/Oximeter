#include <LiquidCrystal>

class Displayer {
  // LiquidCrystal LCD;
  LiquidCrystal LCD(2, 3, 4, 5, 6, 7);
  LCD.begin(16, 2);
  // Displayer(int rs, int enable, int d4, int d5, int d6, int d7) {
  //   LiquidCrystal LCD(rs, enable, d4, d5, d6, d7);
  //   LCD.begin(16, 2);
  // }

  bool firstRow(string text)) {
    LCD.setCursor(0, 0));
    LCD.print(text);
  }

  bool secondRow(string test) {
    LCD.setCursor(0, 1));
    LCD.print(text);
  }
}
