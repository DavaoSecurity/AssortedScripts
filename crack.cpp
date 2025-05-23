// Password cracker written in C++ by Nathan W Jones nat@davaosecurity.com for ECE Credits
// loadPasswords: This function reads the passwords.txt file, which is expected to have lines in the format username:password. 
// It stores the usernames and passwords in an unordered map for quick lookup.
// loadUsers: This function reads the users.txt file, which is expected to contain one username per line.
// It checks if each username exists in the password map and writes any found passwords to found_passwords.txt.
// main: The main function orchestrates loading the passwords and users, and handles file operations.

#include <iostream>
#include <fstream>
#include <string>
#include <unordered_map>

void loadPasswords(const std::string& filename, std::unordered_map<std::string, std::string>& passwordMap) {
    std::ifstream file(filename);
    std::string line;

    while (std::getline(file, line)) {
        size_t delimiterPos = line.find(':');
        if (delimiterPos != std::string::npos) {
            std::string username = line.substr(0, delimiterPos);
            std::string password = line.substr(delimiterPos + 1);
            passwordMap[username] = password;
        }
    }
}

void loadUsers(const std::string& filename, const std::unordered_map<std::string, std::string>& passwordMap, std::ofstream& outputFile) {
    std::ifstream file(filename);
    std::string username;

    while (file >> username) {
        auto it = passwordMap.find(username);
        if (it != passwordMap.end()) {
            outputFile << "Found: " << username << " - " << it->second << std::endl;
        }
    }
}

int main() {
    std::unordered_map<std::string, std::string> passwordMap;
    loadPasswords("passwords.txt", passwordMap);

    std::ofstream outputFile("found_passwords.txt");
    if (!outputFile) {
        std::cerr << "Error opening output file!" << std::endl;
        return 1;
    }

    loadUsers("users.txt", passwordMap, outputFile);

    outputFile.close();
    std::cout << "Password cracking complete. Check found_passwords.txt for results." << std::endl;

    return 0;
}
