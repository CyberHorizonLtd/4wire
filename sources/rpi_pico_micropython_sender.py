"""
Developed by CyberHorizon
GitHub: https://github.com/cyberhorizonltd
Website: https://cyberhorizon.hu

This software is provided 'as-is', without any express or implied warranty.
In no event will the authors be held liable for any damages arising from its use.
Permission is granted to anyone to use this software for any purpose,
including commercial applications, and to alter it and redistribute it freely,
subject to the following restrictions:

1. The origin of this software must not be misrepresented; you must not claim
   that you wrote the original software. If you use this software in a product,
   an acknowledgment in the product documentation would be appreciated but is not required.
2. Altered source versions must be plainly marked as such and must not be misrepresented
   as being the original software.
3. This notice may not be removed or altered from any source distribution.
"""



from machine import Pin

VIRTUAL_NOTIFY_LINE = Pin(5, Pin.OUT)
VIRTUAL_DATA_LINE = Pin(2, Pin.OUT)
VIRTUAL_CLOCK_LINE = Pin(3, Pin.OUT)
VIRTUAL_ENABLE_LINE = Pin(4, Pin.IN)

def notif():
    VIRTUAL_NOTIFY_LINE.value(1)

def stopnotif():
    VIRTUAL_NOTIFY_LINE.value(0)

def write(bit: bool):
    VIRTUAL_DATA_LINE.value(bit)

def clock(state: bool):
    VIRTUAL_CLOCK_LINE.value(state)

def wait_for_enab():
    while VIRTUAL_ENABLE_LINE.value() == 0:
        pass

def wait_for_disab():
    while VIRTUAL_ENABLE_LINE.value() == 1:
        pass

def char_to_bool_array(c: str):
    bits = [(ord(c) >> i) & 1 for i in range(8)]
    return bits

def send_char(c: str):
    bits = char_to_bool_array(c)
    for bit in bits:
        write(False)
        clock(False)
        wait_for_enab()
        write(bit)
        clock(True)
        wait_for_disab()
    clock(False)

def send_vuart_string(data: str):
    data += '\n'
    notif()
    for c in data:
        send_char(c)
    stopnotif()