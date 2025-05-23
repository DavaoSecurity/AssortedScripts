// find Windows users by Nathan W Jones nat@davaosecurity.com for ECE Credits
// Include Necessary Headers: The code includes headers for Windows API functions and JSON handling.
// Enumerate Sessions: The WTSEnumerateSessions function retrieves information about the sessions on the server.
// Query User Information: For each session, it queries the user name and domain name using WTSQuerySessionInformation.
// Create JSON Output: It constructs a JSON object for each user and appends it to a JSON array.
// Write to File: Finally, it writes the JSON array to a specified output file.
// Dependencies-You will need a JSON library for C++. In this example, I used jsoncpp.
// You can install it via vcpkg or include it manually in your project.
// Compilation -Make sure to link against Wtsapi32.lib when compiling your program.

#include <windows.h>
#include <wtsapi32.h>
#include <iostream>
#include <fstream>
#include <string>
#include <json/json.h> // Make sure to include a JSON library, e.g., jsoncpp

#pragma comment(lib, "Wtsapi32.lib")

void ListLoggedOnUsers(const std::string& outputFile) {
    // Create a JSON object to hold the user data
    Json::Value usersJson(Json::arrayValue);

    // Get the session information
    PWTS_SESSION_INFO pSessionInfo = nullptr;
    DWORD sessionCount = 0;
    DWORD bytesReturned = 0;

    if (WTSEnumerateSessions(WTS_CURRENT_SERVER_HANDLE, 0, 1, &pSessionInfo, &sessionCount)) {
        for (DWORD i = 0; i < sessionCount; ++i) {
            // Get the user name for each session
            char* userName = nullptr;
            char* domainName = nullptr;
            DWORD userNameLen = 0;
            DWORD domainNameLen = 0;

            if (WTSQuerySessionInformation(WTS_CURRENT_SERVER_HANDLE, pSessionInfo[i].SessionId, WTSUserName, &userName, &userNameLen)) {
                if (WTSQuerySessionInformation(WTS_CURRENT_SERVER_HANDLE, pSessionInfo[i].SessionId, WTSDomainName, &domainName, &domainNameLen)) {
                    // Create a JSON object for the user
                    Json::Value userJson;
                    userJson["sessionId"] = (int)pSessionInfo[i].SessionId;
                    userJson["userName"] = std::string(domainName) + "\\" + std::string(userName);
                    userJson["state"] = pSessionInfo[i].State == WTSActive ? "Active" : "Disconnected";

                    // Add the user JSON object to the array
                    usersJson.append(userJson);

                    // Free the memory allocated for the user name and domain name
                    WTSFreeMemory(userName);
                    WTSFreeMemory(domainName);
                }
            }
        }
        WTSFreeMemory(pSessionInfo);
    }

    // Write the JSON data to a file
    std::ofstream file(outputFile);
    if (file.is_open()) {
        file << usersJson.toStyledString();
        file.close();
        std::cout << "User data written to " << outputFile << std::endl;
    } else {
        std::cerr << "Failed to open file for writing: " << outputFile << std::endl;
    }
}

int main() {
    const std::string outputFile = "logged_on_users.json";
    ListLoggedOnUsers(outputFile);
    return 0;
}
