/*
 * Developed by CyberHorizon
 * GitHub: https://github.com/cyberhorizonltd
 * Website: https://cyberhorizon.hu
 *
 * This software is provided 'as-is', without any express or implied warranty.
 * In no event will the authors be held liable for any damages arising from its use.
 * Permission is granted to anyone to use this software for any purpose,
 * including commercial applications, and to alter it and redistribute it freely,
 * subject to the following restrictions:
 *
 * 1. The origin of this software must not be misrepresented; you must not claim
 *    that you wrote the original software. If you use this software in a product,
 *    an acknowledgment in the product documentation would be appreciated but is not required.
 * 2. Altered source versions must be plainly marked as such and must not be misrepresented
 *    as being the original software.
 * 3. This notice may not be removed or altered from any source distribution.
 */

#include <string>
#include <Wire.h>

#define VIRTUAL_DATA_LINE 34
#define VIRTUAL_CLOCK_LINE 35
#define VIRTUAL_ENABLE_LINE 32
#define VIRTUAL_NOTIFY_LINE 33

void waitForNotif(){
  while (digitalRead(VIRTUAL_NOTIFY_LINE) == LOW) ;
}

void enable() {
  digitalWrite(VIRTUAL_ENABLE_LINE, HIGH);
}

void disable() {
  digitalWrite(VIRTUAL_ENABLE_LINE, LOW);
}

bool read() {
  return digitalRead(VIRTUAL_DATA_LINE);
}

void waitForClockHigh() {
  while (digitalRead(VIRTUAL_CLOCK_LINE) == LOW) ;
}

void waitForClockLow() {
  while (digitalRead(VIRTUAL_CLOCK_LINE) == HIGH) ;
}

void bool_arr_to_char(bool *bits, char *c)
{
    *c = 0;
    for (int i = 0; i < 8; i++)
    {
        *c |= bits[i] << i;
    }
}

char readChar() {
  bool bits[8];
  for (int i = 0; i < 8; i++)
  {
      enable();
      waitForClockHigh();
      bits[i] = read();
      disable();
      waitForClockLow();
  }
  char c;
  bool_arr_to_char(bits, &c);
  return c;
}

std::string readString() {
  std::string received_data;
    waitForNotif();
    while (true)
    {
        char c = readChar();
        if (c == '\n')
        {
            break;
        }
        received_data += c;
    }
    return received_data;
}