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

void wait_for_notif() {
    while (!fs::exists(VIRTUAL_NOTIFY_LINE)) {
        std::this_thread::yield();
    }
}

void enable() {
    if (!fs::exists(VIRTUAL_ENABLE_LINE)) {
        std::ofstream file(VIRTUAL_ENABLE_LINE);
        file << "1";
    }
}

void disable() {
    if (fs::exists(VIRTUAL_ENABLE_LINE)) {
        fs::remove(VIRTUAL_ENABLE_LINE);
    }
}

bool read() {
    return fs::exists(VIRTUAL_DATA_LINE);
}

void wait_for_clock_high() {
    while (!fs::exists(VIRTUAL_CLOCK_LINE)) {
        std::this_thread::yield();
    }
}

void wait_for_clock_low() {
    while (fs::exists(VIRTUAL_CLOCK_LINE)) {
        std::this_thread::yield();
    }
}

char bool_arr_to_char(const bool bits[8]) {
    char c = 0;
    for (int i = 0; i < 8; ++i) {
        c |= (bits[i] << i);
    }
    return c;
}

char read_char() {
    bool bits[8];
    for (int i = 0; i < 8; ++i) {
        enable();
        wait_for_clock_high();
        bits[i] = read();
        disable();
        wait_for_clock_low();
    }
    return bool_arr_to_char(bits);
}

std::string read_string() {
    std::string received_data;
    wait_for_notif();
    while (true) {
        char c = read_char();
        if (c == '\n') break;
        received_data += c;
    }
    return received_data;
}

int main() {
    fs::create_directories(script_dir + "/com");
    std::string data = read_string();
    std::cout << "Received data: " << data << std::endl;
    fs::remove_all(script_dir + "/com");
    return 0;
}
