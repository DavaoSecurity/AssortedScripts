// g++ -o user_logon_info userlogon.cpp     sudo chmod +x ./userlogon.cpp ./userlogon
// User Structure: A simple structure to hold user information, including the username, last logon time, and activity.
// getUsers Function: This function reads the /etc/passwd file to get a list of users. Each username is stored in a User object.
// getLastLogonAndActivity Function: This function uses the last command to get the last logon information for a specific user.
// It captures the output and parses it to extract the last logon time.
// Main Function: The main function retrieves the list of users and their last logon information, then prints it to the console.

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <cstdlib>

struct User {
    std::string username;
    std::string lastLogon;
    std::string activity;
};

std::vector<User> getUsers() {
    std::vector<User> users;
    std::ifstream passwdFile("/etc/passwd");
    std::string line;

    while (std::getline(passwdFile, line)) {
        std::istringstream iss(line);
        std::string username;
        std::getline(iss, username, ':'); // Get the username before the first ':'

        // Create a User object and add it to the vector
        users.push_back({username, "", ""});
    }

    return users;
}

void getLastLogonAndActivity(User& user) {
    std::string command = "last " + user.username + " | head -n 1";
    FILE* pipe = popen(command.c_str(), "r");
    if (!pipe) {
        user.lastLogon = "Error retrieving logon info";
        return;
    }

    char buffer[128];
    std::string result = "";
    while (fgets(buffer, sizeof(buffer), pipe) != nullptr) {
        result += buffer;
    }
    pclose(pipe);

    // Parse the result to extract last logon time and activity
    std::istringstream iss(result);
    std::string temp;
    if (iss >> temp) { // Skip the username
        std::getline(iss, user.lastLogon); // Get the rest of the line as last logon info
    } else {
        user.lastLogon = "No logon info found";
    }
}

int main() {
    std::vector<User> users = getUsers();

    for (auto& user : users) {
        getLastLogonAndActivity(user);
        std::cout << "User: " << user.username << "\n";
        std::cout << "Last Logon: " << user.lastLogon << "\n\n";
    }

    return 0;
}
