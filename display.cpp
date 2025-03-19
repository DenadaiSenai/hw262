
#include "hw262.h"

Mc74hc595a Display::ic(LCHCLK_PIN, SFTCLK_PIN, SDI_PIN);

// const byte* Display::numbers[10]
// {
//   DISPLAY_0,
//   DISPLAY_1,
//   DISPLAY_2,
//   DISPLAY_3,
//   DISPLAY_4,
//   DISPLAY_5,
//   DISPLAY_6,
//   DISPLAY_7,
//   DISPLAY_8,
//   DISPLAY_9,
// };

#define numbers(DIGIT) Display::ASCII[DIGIT + 48]

const byte *Display::ASCII[256] {
    0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,                                                       // 8
    0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,                                                       // 16
    0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,                                                       // 24
    0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,                                                       // 32
    0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,                                                       // 40
    DISPLAY_LEFTBRACKET, DISPLAY_RIGHTBRACKET, 0xFF, 0xFF, DISPLAY_DOT, DISPLAY_MINUS, DISPLAY_DOT, 0xFF, // 48
    DISPLAY_0, DISPLAY_1, DISPLAY_2, DISPLAY_3, DISPLAY_4, DISPLAY_5, DISPLAY_6, DISPLAY_7,               // 56
    DISPLAY_8, DISPLAY_9, 0xFF, 0xFF, 0xFF, DISPLAY_EQUAL, 0xFF, 0xFF,                                    // 64
    0xFF, DISPLAY_A, DISPLAY_B, DISPLAY_C, DISPLAY_D, DISPLAY_E, DISPLAY_F, DISPLAY_G,                    // 72
    DISPLAY_H, DISPLAY_I, 0xFF, 0XFF, DISPLAY_L, 0xFF, DISPLAY_N, DISPLAY_O,                              // 80
    DISPLAY_P, DISPLAY_Q, DISPLAY_R, DISPLAY_S, DISPLAY_T, DISPLAY_U, 0xFF, 0xFF,                         // 88
    0xFF, DISPLAY_Y, 0xFF, DISPLAY_LEFTBRACKET, 0xFF, DISPLAY_RIGHTBRACKET, 0xFF, DISPLAY_UNDERSCORE,     // 96
    0xFF, DISPLAY_A, DISPLAY_B, DISPLAY_C, DISPLAY_D, DISPLAY_E, DISPLAY_F, DISPLAY_G,                    //
    DISPLAY_H, DISPLAY_I, 0xFF, 0XFF, DISPLAY_L, 0xFF, DISPLAY_N, DISPLAY_O,                              // 112
    DISPLAY_P, DISPLAY_Q, DISPLAY_R, DISPLAY_S, DISPLAY_T, DISPLAY_U, 0xFF, 0xFF,                         // 120
    0xFF, DISPLAY_Y, 0xFF, DISPLAY_LEFTBRACKET, 0xFF, DISPLAY_RIGHTBRACKET, 0xFF, 0xFF,                   // 128
    0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,                                                       // 136
    0xFF, 0xFF, 0xFF, 0xFF, DISPLAY__N, DISPLAY__N, 0xFF, DISPLAY_DEGRESS,                                // 144
    0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,                                                       // 152
    0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,                                                       // 176
    0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,                                                       // 184
    0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,                                                       // 192
    0xFF, 0xFF, 0xFF, 0xFF, DISPLAY_MINUS, 0xFF, 0xFF, 0xFF,                                              // 200
    0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,                                                       // 208
    0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,                                                       // 216
    0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,                                                       // 224
    0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,                                                       // 232
    0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,                                                       // 240
    0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,                                                       // 248
    0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF                                                        // 256
};

/**
 * @brief Return a byte that can be showed into
 * 7-segments-display-4-digits from a char
 * @param c character to convert
 * @return byte
 */
byte Display::convertCharToSegments(char c)
{
    return Display::ASCII[static_cast<uint8_t>(c)];
}

/**
 * @brief Clear display
 *
 * This function sends a blank byte to the display to clear the actual number
 * that is shown on the display.
 */
void Display::clear()
{
  for (uint8_t i = 0; i < NUMBER_DIGITS_ON_DISPLAY; ++i)
  {
    const register uint8_t position = MAX_DISPLAY_POSITION - i;
    ic.SendByte(DISPLAY_BLANK); // Send blank byte to clear display
    ic.SendByte(0x10 << position); // Send position byte to set position to the right
    ic.End(); // Send end byte to finish the transmission
  }
}

/**
 * @brief Return a byte from an int16_t number
 * @param number number to convert
 * @param leading_zeros number of leading zeros
 * @return char*
 */
char *Display::convertIntToBytes(const int16_t number, bool leading_zeros)
{
  uint32_t factor;
  uint8_t digit;
  bool startNumber = false;
  char *charNumber = new char[NUMBER_DIGITS_ON_DISPLAY];
  uint8_t position;
  byte segmentDigit;
  uint16_t absNumber = abs(number);

  for (int8_t i = 3; i > -1; i--)
  {
    position = (MAX_DISPLAY_POSITION - i);
    factor = (i > 1) ? round(pow(10, i)) : pow(10, i); // correction for pow method
    digit = absNumber / factor;
    absNumber -= digit * factor;
    if (digit != 0 && startNumber == false)
    {
      if (number < 0)
        charNumber[position - 1] = DISPLAY_MINUS;
      startNumber = true;
    }
    segmentDigit = numbers(digit);
    charNumber[position] = (startNumber == true || (digit == 0 && i == 0) || leading_zeros == true) ? segmentDigit : DISPLAY_BLANK;
  }
  return charNumber;
}

/*******************************************************************************/
/**
 * @brief Write a const char*\/number\/symbol to display
 */
void Display::write(const char *text)
{
  register uint8_t position;
  register byte c;
  register uint8_t len = strlen(text);

  if (len > NUMBER_DIGITS_ON_DISPLAY)
  {
    Serial.println(MESSAGE_INVALID_LENGHT_TEXT);
    return;
  }

  for (int i = 0; i < len; i++)
  {
    position = (MAX_DISPLAY_POSITION - i);
    c = convertCharToSegments(text[i]);
    ic.SendByte(c);
    ic.SendByte(0x10 << position);
    ic.End();
  }
}

/**
 * @brief Write an integer to the display with optional leading zeros and offset.
 *
 * This function delegates to writeInteger() to handle the actual display logic.
 * It allows specifying if leading zeros should be displayed and a positional offset.
 *
 * @param number The integer number to display.
 * @param leading_zeros Whether to pad the number with leading zeros.
 * @param offset Offset position on the display.
 */
void Display::write(const int16_t number, bool leading_zeros, uint8_t offset)
{
    // Delegate to writeInteger to perform the display operation
    writeInteger(number, leading_zeros, offset);
}

/**
 * @brief Write a symbol to a specific position on the display.
 *
 * This function sends the provided symbol to be displayed at a specified position
 * on the 7-segment display.
 *
 * @param symbol The byte representing the symbol to display.
 * @param position The position on the display to write the symbol (0-3).
 */
void Display::write(byte symbol, uint8_t position)
{
  // Send the byte corresponding to the symbol to the display
  ic.SendByte(symbol);
  
  // Shift the position bit to the correct location and send it to the display
  ic.SendByte(0x10 << position);
  
  // End the current transmission
  ic.End();
}
/*******************************************************************************/

/*
uint8_t Display::getDotPosition(const float number)
{
  if(number < 10) return MAX_DISPLAY_POSITION;
  else if(number < 100) return 2;
  else if(number < 1000) return 1;
  else return 0;
}
*/

/**
 * @brief
 *
 * @param number
 */
/*
void Display::write(const float number)
{
 byte* numberDigits = new byte[NUMBER_DIGITS_ON_DISPLAY];
 byte byteDigit;
 uint8_t digitPosition;
 uint8_t dotPosition;
 int32_t numberMils;

 if(IS_NOT_VALID_NUMBER(number))
 {
   Serial.println(MESSAGE_INVALID_NUMBER);
   delete[] numberDigits;
   return;
 }

 dotPosition = getDotPosition(number);
 numberMils = number*round(pow(10, dotPosition));

 strcpy(numberDigits, convertIntToBytes(numberMils));

 for(uint8_t i=0; i<NUMBER_DIGITS_ON_DISPLAY; i++)
 {
   digitPosition = MAX_DISPLAY_POSITION-i;
   byteDigit = (i != dotPosition) ? numberDigits[i] : (numberDigits[i] - 1);

   ic.SendByte(byteDigit);
   ic.SendByte((byte) (0x10 << digitPosition) );
   ic.End();
 }
 delete[] numberDigits;
}
*/

/**
 * @brief Write an integer to the display with optional leading zeros and offset.
 *
 * @param number The integer number to display.
 * @param leadingZeros Whether to pad the number with leading zeros.
 * @param offset Offset position on the display.
 */
void Display::writeInteger(const int16_t number, const bool leadingZeros, const uint8_t offset)
{
    if ((offset > MAX_DISPLAY_POSITION) || IS_NOT_VALID_NUMBER(number)) {
        return;
    }

    char* const segments = convertIntToBytes(number, leadingZeros);

    for (uint8_t i = 0; i < NUMBER_DIGITS_ON_DISPLAY; ++i) {
        const uint8_t position = (MAX_DISPLAY_POSITION - i) + offset;
        ic.SendByte(segments[i]);
        ic.SendByte(0x10 << position);
        ic.End();
    }

    delete[] segments;
}

/**
 * @brief Write a single digit to a specified position on the display.
 *
 * This function will activate the display to show a digit character at the given position.
 * The digit should be a value between 0 and 9, inclusive.
 * The position should be a value between 0 and 3, inclusive, where 0 is the leftmost position.
 *
 * @param digit The digit to display (0-9).
 * @param position The position on the display to write the digit (0-3).
 */
void Display::writeOne(uint8_t digit, uint8_t position)
{
    // Send the byte corresponding to the digit character to the display
    ic.SendByte(numbers(digit));

    // Shift the position bit to the correct location and send it to the display
    ic.SendByte(0x10 << position);

    // End the current transmission
    ic.End();
}

/**
 * @brief Write a 4-digit number to the display.
 *
 * @param number The number to display (0-9999).
 *
 * The number is split into its individual digits and then written to the display
 * in the correct order.
 */
void Display::writeNumber(uint16_t number)
{
    uint8_t digits[NUMBER_DIGITS_ON_DISPLAY] = { 0 };
    uint8_t position = NUMBER_DIGITS_ON_DISPLAY - 1;

    // Split the number into its individual digits
    do
    {
        digits[position--] = number % 10;
        number /= 10;
    }
    while (number > 0);

    // Write the digits to the display
    for (uint8_t i = 0; i < NUMBER_DIGITS_ON_DISPLAY; ++i)
    {
        ic.SendByte(numbers(digits[i]));
        ic.SendByte(0x10 << position);
        ic.End();
        --position;
    }
}

/**
 * @brief Write a dot to the display at a specified position.
 *
 * This function will activate the display to show a dot character at the given position.
 * The position should be a value between 0 and 3, inclusive, where 0 is the leftmost position.
 *
 * @param position The position on the display to write the dot (0-3).
 */
void Display::writeDot(uint8_t position)
{
  // Send the byte corresponding to the dot character to the display
  ic.SendByte(DISPLAY_DOT);
  
  // Shift the position bit to the correct location and send it to the display
  ic.SendByte(0x10 << position);
  
  // End the current transmission
  ic.End();
}
