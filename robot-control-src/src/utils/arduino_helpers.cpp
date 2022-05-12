#include "arduino_helpers.hpp"
#include <cstddef>

bool readFromSerial(SerialCharacter *const buffer, const std::size_t numberOfCharacters, Stream &iostream)
{
  static bool isQuestionPrinted = false;
  static std::size_t charToRead = 0;
  if (!isQuestionPrinted)
  {
    charToRead = (buffer != nullptr) ? numberOfCharacters : 0;
    iostream.printf("Please enter %u characters and press ENTER.\n", charToRead);
    isQuestionPrinted = true;
  }
  if (!iostream.available())
  {
    return true;
  }
  else
  {
    const SerialCharacter newChar = iostream.read();
    if (charToRead > 0)
    {
      if (buffer != nullptr && charToRead <= numberOfCharacters)
      {
        buffer[numberOfCharacters - charToRead] = newChar;
      }
      charToRead--;
      return true;
    }
    else
    {
      isQuestionPrinted = false;
      return false;
    }
  }
}
