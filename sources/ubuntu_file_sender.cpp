// Developed by CyberHorizon
// GitHub: https://github.com/cyberhorizonltd
// Website: https://cyberhorizon.hu
//
// This software is provided 'as-is', without any express or implied warranty.
// In no event will the authors be held liable for any damages arising from its use.
// Permission is granted to anyone to use this software for any purpose,
// including commercial applications, and to alter it and redistribute it freely,
// subject to the following restrictions:
//
// 1. The origin of this software must not be misrepresented; you must not claim
//    that you wrote the original software. If you use this software in a product,
//    an acknowledgment in the product documentation would be appreciated but is not required.
// 2. Altered source versions must be plainly marked as such and must not be misrepresented
//    as being the original software.
// 3. This notice may not be removed or altered from any source distribution.

#include <iostream>
#include <fstream>
#include <filesystem>
#include <thread>

namespace fs = std::filesystem;

const std::string script_dir = fs::absolute(fs::path(__FILE__)).parent_path().string();
const std::string VIRTUAL_DATA_LINE = script_dir + "/com/VIRTUAL_DATA_LINE";
const std::string VIRTUAL_CLOCK_LINE = script_dir + "/com/VIRTUAL_CLOCK_LINE";
const std::string VIRTUAL_ENABLE_LINE = script_dir + "/com/VIRTUAL_ENABLE_LINE";
const std::string VIRTUAL_NOTIFY_LINE = script_dir + "/com/VIRTUAL_NOTIFY_LINE";

void notif() {
    if (!fs::exists(VIRTUAL_NOTIFY_LINE)) {
        std::ofstream file(VIRTUAL_NOTIFY_LINE);
        file << "1";
    }
}

void stopnotif() {
    if (fs::exists(VIRTUAL_NOTIFY_LINE)) {
        fs::remove(VIRTUAL_NOTIFY_LINE);
    }
}

void write(bool state) {
    if (state) {
        if (!fs::exists(VIRTUAL_DATA_LINE)) {
            std::ofstream file(VIRTUAL_DATA_LINE);
            file << "1";
        }
    } else {
        if (fs::exists(VIRTUAL_DATA_LINE)) {
            fs::remove(VIRTUAL_DATA_LINE);
        }
    }
}

void clock(bool state) {
    if (state) {
        if (!fs::exists(VIRTUAL_CLOCK_LINE)) {
            std::ofstream file(VIRTUAL_CLOCK_LINE);
            file << "1";
        }
    } else {
        if (fs::exists(VIRTUAL_CLOCK_LINE)) {
            fs::remove(VIRTUAL_CLOCK_LINE);
        }
    }
}

void waitForEnab() {
    while (!fs::exists(VIRTUAL_ENABLE_LINE)) {
        std::this_thread::yield();
    }
}

void waitForDisab() {
    while (fs::exists(VIRTUAL_ENABLE_LINE)) {
        std::this_thread::yield();
    }
}

bool* char_to_bool_array(char c) {
    static bool bits[8];
    for (int i = 0; i < 8; ++i) {
        bits[i] = (c >> i) & 1;
    }
    return bits;
}

void sendChar(char c) {
    bool* bits = char_to_bool_array(c);
    for (int i = 0; i < 8; ++i) {
        write(false);
        clock(false);
        waitForEnab();
        write(bits[i]);
        clock(true);
        waitForDisab();
    }
    clock(false);
}

void sendString(const std::string& data) {
    std::string fullData = data + '\n';
    notif();
    for (char c : fullData) {
        sendChar(c);
    }
    stopnotif();
}

int main() {
    fs::create_directories(script_dir + "/com");
    sendString("This is a test.");
    fs::remove_all(script_dir + "/com");
    return 0;
}
