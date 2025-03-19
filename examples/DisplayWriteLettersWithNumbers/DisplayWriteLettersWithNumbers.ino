#include <hw262.h>

char *TextArray[10] = { "ABCD", "EFGH", "IJKL", "MNOP", "QRST", "UVXY", "0123", "4567", "890-", "=-_-" };

uint16_t counter = 0;
uint64_t currentTime;
uint64_t elapsedTime = 0;
#define INTERVALTIME 100

/**
 * @brief Initialization function
 * @details Setup function which initializes the HW262 library by calling
 *          the HW262.begin() function. It also clears the display
 *          by calling the HW262.display.clear() function.
 */
void setup() {
  HW262.begin();  // Initialize HW262 library
  HW262.display.clear();  // Clear display
}

/**
 * @brief Rotates the text to the left by one character.
 * 
 * @param inputText The original text to be rotated.
 * @return The rotated text with the first character moved to the end.
 */
String rotateTextLeft(String inputText) {
  // Store the original text to retain the first character
  String originalText = inputText;
  
  // Shift each character to the left
  for (int i = 0; i < inputText.length() - 1; i++) {
    inputText[i] = inputText[i + 1];
  }
  
  // Move the first character to the end of the string
  inputText[inputText.length() - 1] = originalText[0];
  
  return inputText;
}

/**
 * @brief Rotate the text to the right by one character.
 * @details This function takes a string as an argument and shifts each character to the right.
 *          The last character of the string is moved to the first position.
 * @param inputText The original text to be rotated.
 * @return The rotated text with the last character moved to the first position.
 */
String rotateTextRight(String inputText) {
  // Store the original text to retain the last character
  String originalText = inputText;
  
  // Shift each character to the right
  for (int i = inputText.length() - 1; i > 0; i--) {
    inputText[i] = inputText[i - 1];
  }
  
  // Move the last character to the first position of the string
  inputText[0] = originalText[originalText.length() - 1];
  
  return inputText;
}

String abc = "   0123456789-,.[]Cc   ";

void loop() {
  abc = rotateTextLeft(abc);  // Rotate text to the left
  
  currentTime = millis();
  while ((elapsedTime = millis() - currentTime) < INTERVALTIME) {
    HW262.display.writeInteger(counter, true);  // Display the counter with leading zeros
    HW262.display.writeDot(1);  // Display a dot at position 1
  }
  
  elapsedTime = 0;
  if (++counter > 9999) {
    counter = 0;  // Reset counter if it exceeds 9999
  }
}
