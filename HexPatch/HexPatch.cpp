#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>
#define NOMINMAX // To avoid conflict with min max in windows.h
#include <windows.h>
#include <string>
#include <fcntl.h>
#include <io.h>

std::wstring openFileDialog() {
    OPENFILENAMEW ofn;
    wchar_t szFileName[MAX_PATH] = L"";

    ZeroMemory(&ofn, sizeof(ofn));
    ofn.lStructSize = sizeof(ofn);
    ofn.hwndOwner = NULL;
    ofn.lpstrFilter = L"Main game executable (alien.exe)\0alien.exe\0";
    ofn.lpstrFile = szFileName;
    ofn.nMaxFile = sizeof(szFileName) / sizeof(wchar_t);
    ofn.lpstrTitle = L"Select a File";
    ofn.Flags = OFN_DONTADDTORECENT | OFN_FILEMUSTEXIST;

    if (GetOpenFileNameW(&ofn))
        return szFileName;
    else
        return L""; // User canceled the dialog
}

std::string getHexSequenceFromUser() {
    std::string hexSequence;
    std::wcout << L"Choose the desired aspect ratio:\n1 - 16:9, 2 - 16:10, or 3 - 21:9 ): ";
    int choice;
    std::cin >> choice;

    switch (choice) {
    case 1:
        hexSequence = "398EE33F";
        break;
    case 2:
        hexSequence = "CDCCCC3F";
        break;
    case 3:
        hexSequence = "8EE31840";
        break;
    default:
        hexSequence = "";
        break;
    }
    return hexSequence;
}

void replaceHexSequence(const std::wstring& filePath, const std::string& searchHex, const std::string& replaceHex) {
    std::ifstream inputFile(filePath, std::ios::binary);
    if (!inputFile) {
        std::wcout << L"Error accessing the file " << filePath << L"\nMake sure the game is not running.\nIf the game is in Program Files, run the patcher as Administrator." << std::endl;
        return;
    }

    // Read the entire file into a vector
    std::vector<char> fileContents((std::istreambuf_iterator<char>(inputFile)), std::istreambuf_iterator<char>());
    inputFile.close();

    // Convert searchHex and replaceHex to vectors of bytes
    std::vector<char> searchBytes, replaceBytes;
    for (size_t i = 0; i < searchHex.length(); i += 2) {
        searchBytes.push_back(std::stoi(searchHex.substr(i, 2), nullptr, 16));
    }

    for (size_t i = 0; i < replaceHex.length(); i += 2) {
        replaceBytes.push_back(std::stoi(replaceHex.substr(i, 2), nullptr, 16));
    }

    // Search for the hex sequence in the file
    auto it = std::search(fileContents.begin(), fileContents.end(), searchBytes.begin(), searchBytes.end());

    if (it != fileContents.end()) {
        // Create the backup file by renaming and appending '.bak' to the original file name
        std::wstring backupPath = filePath + L".bak";

        if (MoveFileEx(filePath.c_str(), backupPath.c_str(), MOVEFILE_REPLACE_EXISTING) == 0) {
            std::wcout << L"Error accessing the file " << filePath << L"\nMake sure the game directory is writable, the game is not running and there's no 'alien.exe.bak' file with read-only permissions.\nIf the game is in Program Files, run the patcher as Administrator." << std::endl;
            return;
        }

        // Replace the found sequence with the new one
        std::copy(replaceBytes.begin(), replaceBytes.end(), it);

        // Write the modified content back to the file
        std::ofstream outputFile(filePath, std::ios::binary | std::ios::trunc);
        if (!outputFile) {
            std::wcout << L"Error accessing the file " << filePath << L"\nMake sure the game directory is writable, the game is not running and there's no 'alien.exe.bak' file with read-only permissions.\nIf the game is in Program Files, run the patcher as Administrator." << std::endl;
            return;
        }

        std::copy(fileContents.begin(), fileContents.end(), std::ostreambuf_iterator<char>(outputFile));
        outputFile.close();

        std::wcout << L"Hex sequence found and replaced successfully.\nBackup created: " << backupPath << std::endl;
    }
    else {
        std::wcout << L"Hex sequence not found in the file. No changes made. Exiting." << std::endl;
    }
}


int main() {
    SetConsoleOutputCP(CP_UTF8);
    _setmode(_fileno(stdout), _O_U16TEXT);
    std::string searchHex = "ABAAAA3F"; // Hex value for 4:3 aspect ratio - default in the game

    std::string replaceHex = getHexSequenceFromUser();

    if (replaceHex.empty()) {
        std::wcout << L"Incorrect choice. Exiting." << std::endl;
    }
    else {
        std::wstring filePath = openFileDialog(); // Open file dialog to get the file path
        if (filePath.empty()) {
            std::wcout << L"File selection canceled. Exiting." << std::endl;
        }
        else {
            replaceHexSequence(filePath, searchHex, replaceHex); 
        }
    }

    // Wait for user input before closing the console
    std::wcout << L"Press Enter to exit...";
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    std::cin.get();

    return 0;
}