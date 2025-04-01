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

#include <Wire.h>
#include <string>

#define VIRTUAL_DATA_LINE 37
#define VIRTUAL_CLOCK_LINE 46
#define VIRTUAL_ENABLE_LINE 45
#define VIRTUAL_NOTIFY_LINE 42

void notif() {
    digitalWrite(VIRTUAL_NOTIFY_LINE, 1);
}

void stopnotif() {
    digitalWrite(VIRTUAL_NOTIFY_LINE, 0);
}

void write(bool bit)
{
    digitalWrite(VIRTUAL_DATA_LINE, bit);
}

void clock(bool state)
{
    digitalWrite(VIRTUAL_CLOCK_LINE, state);
}

void waitForEnab() {
    while (digitalRead(VIRTUAL_ENABLE_LINE) == LOW) ;
}
void waitForDisab() {
    while (digitalRead(VIRTUAL_ENABLE_LINE) == HIGH) ;
}

void char_to_bool_array(char c, bool *bits)
{
    for (int i = 0; i < 8; i++)
    {
        bits[i] = (c >> i) & 1;
    }
}

void sendChar(char c)
{
    bool bits[8];
    char_to_bool_array(c, bits);
    for (int j = 0; j < 8; j++)
    {
        write(false);
        clock(false);
        waitForEnab();
        write(bits[j]);
        clock(true);
        waitForDisab();
    }
    clock(false);
}

void sendString(std::string data)
{
    data += '\n';
    notif();
    for (int i = 0; i < data.length(); i++)
    {
        sendChar(data[i]);
    }
    stopnotif();
}