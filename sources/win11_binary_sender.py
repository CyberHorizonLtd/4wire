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


def notif():
    if not os.path.exists(VIRTUAL_NOTIFY_LINE):
        with open(VIRTUAL_NOTIFY_LINE, 'w') as f:
            f.write('1')

def stopnotif():
    if os.path.exists(VIRTUAL_NOTIFY_LINE):
        os.remove(VIRTUAL_NOTIFY_LINE)

def write(state):
    if state:
        if not os.path.exists(VIRTUAL_DATA_LINE):
            with open(VIRTUAL_DATA_LINE, 'w') as f:
                f.write('1')
    else:
        if os.path.exists(VIRTUAL_DATA_LINE):
            os.remove(VIRTUAL_DATA_LINE)

def clock(state):
    if state:
        if not os.path.exists(VIRTUAL_CLOCK_LINE):
            with open(VIRTUAL_CLOCK_LINE, 'w') as f:
                f.write('1')
    else:
        if os.path.exists(VIRTUAL_CLOCK_LINE):
            os.remove(VIRTUAL_CLOCK_LINE)

def waitForEnab():
    while not os.path.exists(VIRTUAL_ENABLE_LINE):
        pass

def waitForDisab():
    while os.path.exists(VIRTUAL_ENABLE_LINE):
        pass

def char_to_bool_array(c):
    bits = [False] * 8
    for i in range(8):
        bits[i] = (c >> i) & 1
    return bits

def sendChar(c):
    bits = char_to_bool_array(ord(c))
    # print(bits)
    for bit in bits:
        write(False)
        clock(False)
        waitForEnab()
        write(bit)
        clock(True)
        waitForDisab()
    clock(False)


def sendString(data:str):
    notif()
    for i,char in enumerate(data):
        if i % 100 == 0:
            print(f"Sending {i}/{len(data)} characters", end="\r")
        sendChar(char)
    stopnotif()

if __name__ == "__main__":
    # Example usage
    if not os.path.exists(os.path.join(script_dir,"com")):
        os.makedirs(os.path.join(script_dir,"com"))
    image = "example_image.png"  # Replace with your image path
    image_data = open(image, 'rb').read()
    data = image_data.decode('latin-1')
    sendString(str(len(data)) + "\n")
    sendString(data)  # Use 'latin-1' to handle binary data
    # delete everything in com folder
    for file in os.listdir(os.path.join(script_dir,"com")):
        file_path = os.path.join(script_dir,"com", file)
        try:
            os.remove(file_path)
        except Exception as e:
            print(f"Error deleting file {file_path}: {e}")
    if os.path.exists(os.path.join(script_dir,"com")):
        os.rmdir(os.path.join(script_dir,"com"))
