#include <iostream>
#include <fstream>
#include <sstream>
#include <cstdlib>
#include <ctime>
#include <thread>
#include <chrono>
#include <mutex>
#include <string>
#include <windows.h>
#include <shellapi.h>
#include <iomanip>
#include <iphlpapi.h>

using namespace std;

// Function prototypes
void runSpoofer();
void revertSpoofer();
void checkHWIDAndMAC();
void networkReset();
string generateRandomMAC();
string generateRandomGUID();
string generateRandomSerial();
string generateRandomPCName();
string generateRandomGPUName();
string generateRandomMonitorName();
void changeMACAddress(const string& mac);
void changeHWID(const string& hwid);
void changeSerial(const string& serial);
void changePCName(const string& pcName);
void changeGPUName(const string& gpuName);
void changeMonitorName(const string& monitorName);
string getCurrentMACAddress();
string getCurrentHWID();
string getCurrentSerial();
void showCredits();
void setConsoleTextColor(int color);
void logAction(const string& action);
bool isAdmin();
void restartAsAdmin();
bool changeRegistryValue(HKEY hKeyRoot, const string& subKey, const string& valueName, const string& newValue);

// Registry paths and value names
const string MAC_REGISTRY_BASE_PATH = "SYSTEM\\CurrentControlSet\\Control\\Class\\{4D36E972-E325-11CE-BFC1-08002BE10318}\\";
const string HWID_REGISTRY_PATH = "SYSTEM\\CurrentControlSet\\Control\\IDConfigDB\\Hardware Profiles\\0001";
const string SERIAL_REGISTRY_PATH = "SOFTWARE\\Microsoft\\Windows NT\\CurrentVersion";
const string GPU_REGISTRY_PATH = "SYSTEM\\CurrentControlSet\\Control\\Video";
const string MONITOR_REGISTRY_PATH = "SYSTEM\\CurrentControlSet\\Enum\\DISPLAY";

// Global variables
string originalMAC;
string originalHWID;
string originalSerial;
string originalPCName;
string originalGPUName;
string originalMonitorName;
bool isSpoofed = false;
mutex spoofMutex;

int main() {
    if (!isAdmin()) {
        cout << "This application needs administrator privileges. Restarting with admin rights...\n";
        restartAsAdmin();
        return 0;
    }

    srand(static_cast<unsigned int>(time(0))); // Seed for random number generation

    int option;
    bool running = true;

    while (running) {
#ifdef _WIN32
        system("cls");
#else
        system("clear");
#endif

        setConsoleTextColor(9); // Blue text
        cout << "==============================\n";
        cout << "          SpaceFN GUI\n";
        cout << "==============================\n";
        cout << "[1] Run Spoofer (Temp/free)\n";
        cout << "[2] Check HWID, MAC, and Serial\n";
        cout << "[3] Reset Spoofed Settings\n";
        cout << "[4] Network Reset\n";
        cout << "[5] Discord\n";
        cout << "[6] Website\n";
        cout << "[7] Credits\n";
        cout << "[8] Exit\n";
        cout << "==============================\n";
        cout << "Option: ";
        cin >> option;

        switch (option) {
        case 1:
            runSpoofer();
            break;
        case 2:
            checkHWIDAndMAC();
            break;
        case 3:
            revertSpoofer();
            break;
        case 4:
            networkReset();
            break;
        case 5:
            system("start https://discord.com/invite/spacefn");
            break;
        case 6:
            system("start https://spacefn.framer.website/");
            break;
        case 7:
            showCredits();
            break;
        case 8:
            if (isSpoofed) revertSpoofer();
            running = false;
            break;
        default:
            cout << "Invalid option. Please try again.\n";
            break;
        }

        if (running) {
            cout << "\nPress Enter to return to the menu...";
            cin.ignore();
            cin.get();
        }
    }
    return 0;
}

void runSpoofer() {
    lock_guard<mutex> guard(spoofMutex);
    if (isSpoofed) {
        cout << "Spoofer is already active.\n";
        return;
    }

    // Make the screen blank before spoofing
    system("cls");

    cout << "Spoofing...\n";

    // Save original values
    originalMAC = getCurrentMACAddress();
    originalHWID = getCurrentHWID();
    originalSerial = getCurrentSerial();
    originalPCName = "Original-PC"; // Placeholder
    originalGPUName = "Original-GPU"; // Placeholder
    originalMonitorName = "Original-Monitor"; // Placeholder

    // Generate random values
    string randomMAC = generateRandomMAC();
    string randomHWID = generateRandomGUID();
    string randomSerial = generateRandomSerial();
    string randomPCName = generateRandomPCName();
    string randomGPUName = generateRandomGPUName();
    string randomMonitorName = generateRandomMonitorName();

    // Display generated values
    cout << "New MAC: " << randomMAC << endl;
    cout << "New HWID: " << randomHWID << endl;
    cout << "New Serial: " << randomSerial << endl;
    cout << "New PC Name: " << randomPCName << endl;
    cout << "New GPU Name: " << randomGPUName << endl;
    cout << "New Monitor Name: " << randomMonitorName << endl;

    // Change values in the system
    changeMACAddress(randomMAC);
    changeHWID(randomHWID);
    changeSerial(randomSerial);
    changePCName(randomPCName);
    changeGPUName(randomGPUName);
    changeMonitorName(randomMonitorName);

    logAction("Spoofing activated.");
    isSpoofed = true;

    thread([]() {
        this_thread::sleep_for(chrono::hours(2));
        revertSpoofer();
        }).detach();

    cout << "Spoofing active for 2 hours.\n";
}

void showCredits() {
    cout << "Credits:\n";
    cout << "Spoofer created by: YourName\n";
    cout << "Contributors: Contributor1, Contributor2\n";
    cout << "Special thanks to the community!\n";
}

string generateRandomGUID() {
    stringstream guid;
    guid << hex << setw(8) << setfill('0') << rand() % 0xFFFFFFFF << "-"
        << hex << setw(4) << setfill('0') << rand() % 0xFFFF << "-"
        << hex << setw(4) << setfill('0') << rand() % 0xFFFF << "-"
        << hex << setw(4) << setfill('0') << rand() % 0xFFFF << "-"
        << hex << setw(12) << setfill('0') << rand() % 0xFFFFFFFFFFFF;
    return guid.str();
}

string generateRandomSerial() {
    const char chars[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789";
    string serial;
    for (int i = 0; i < 16; ++i) {
        serial += chars[rand() % (sizeof(chars) - 1)];
    }
    return serial;
}

string generateRandomMAC() {
    stringstream mac;
    for (int i = 0; i < 6; ++i) {
        mac << hex << uppercase << setw(2) << setfill('0') << (rand() % 256);
        if (i < 5) mac << "-";
    }
    return mac.str();
}

string generateRandomPCName() {
    return "PC-" + to_string(rand() % 10000); // Simple random PC name generator
}

string generateRandomGPUName() {
    return "GPU-" + to_string(rand() % 100); // Simple random GPU name generator
}

string generateRandomMonitorName() {
    return "Monitor-" + to_string(rand() % 100); // Simple random monitor name generator
}

void checkHWIDAndMAC() {
    cout << "Current HWID: " << getCurrentHWID() << endl;
    cout << "Current MAC: " << getCurrentMACAddress() << endl;
    cout << "Current Serial: " << getCurrentSerial() << endl;
}

bool changeRegistryValue(HKEY hKeyRoot, const string& subKey, const string& valueName, const string& newValue) {
    HKEY hKey;
    if (RegOpenKeyExA(hKeyRoot, subKey.c_str(), 0, KEY_SET_VALUE, &hKey) != ERROR_SUCCESS) {
        return false;
    }
    if (RegSetValueExA(hKey, valueName.c_str(), 0, REG_SZ, reinterpret_cast<const BYTE*>(newValue.c_str()), newValue.length() + 1) != ERROR_SUCCESS) {
        RegCloseKey(hKey);
        return false;
    }
    RegCloseKey(hKey);
    return true;
}

// Additional missing functions like `changeMACAddress`, `changeHWID`, etc. need to be defined as well.
