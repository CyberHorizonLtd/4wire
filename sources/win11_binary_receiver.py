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

import os

script_dir = os.path.dirname(os.path.abspath(__file__))
VIRTUAL_DATA_LINE = os.path.join(script_dir,"com", "VIRTUAL_DATA_LINE")
VIRTUAL_CLOCK_LINE = os.path.join(script_dir,"com", "VIRTUAL_CLOCK_LINE")
VIRTUAL_ENABLE_LINE = os.path.join(script_dir,"com", "VIRTUAL_ENABLE_LINE")
VIRTUAL_NOTIFY_LINE = os.path.join(script_dir,"com", "VIRTUAL_NOTIFY_LINE")

def wait_for_notif():
    while not os.path.exists(VIRTUAL_NOTIFY_LINE):
        pass

def enable():
    if not os.path.exists(VIRTUAL_ENABLE_LINE):
        with open(VIRTUAL_ENABLE_LINE, 'w') as f:
            f.write('1')

def disable():
    if os.path.exists(VIRTUAL_ENABLE_LINE):
        os.remove(VIRTUAL_ENABLE_LINE)

def read():
    return os.path.exists(VIRTUAL_DATA_LINE)

def wait_for_clock_high():
    while not os.path.exists(VIRTUAL_CLOCK_LINE):
        pass

def wait_for_clock_low():
    while os.path.exists(VIRTUAL_CLOCK_LINE):
        pass

def notif_stopped():
    return not os.path.exists(VIRTUAL_NOTIFY_LINE)

def bool_arr_to_char(bits):
    c = 0
    for i in range(8):
        c |= bits[i] << i
    return chr(c)

def read_char():
    bits = []
    for _ in range(8):
        enable()
        wait_for_clock_high()
        bits.append(read())
        disable()
        wait_for_clock_low()
    return bool_arr_to_char(bits)

def read_string(len = 0):
    received_data = ""
    wait_for_notif()
    if len == 0:
        while True:
            c = read_char()
            if c == '\n':
                break
            received_data += c
        return received_data
    else:
        for _ in range(len):
            c = read_char()
            received_data += c
        return received_data

if __name__ == "__main__":
    if not os.path.exists(os.path.join(script_dir,"com")):
        os.makedirs(os.path.join(script_dir,"com"))
    datalen = read_string()
    print(f"Received data length: {datalen}")
    print("Receiving data...")
    data = read_string(int(datalen))
    with open("received_image.png", 'wb') as f:
        f.write(data.encode('latin-1'))
    # delete everything in the com folder
    for filename in os.listdir(os.path.join(script_dir,"com")):
        file_path = os.path.join(script_dir,"com", filename)
        try:
            if os.path.isfile(file_path):
                os.remove(file_path)
        except Exception as e:
            print(f"Error deleting file {file_path}: {e}")
    if os.path.exists(os.path.join(script_dir,"com")):
        os.rmdir(os.path.join(script_dir,"com"))