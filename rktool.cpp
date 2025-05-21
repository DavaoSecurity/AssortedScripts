// Basic Rootkit hunter by Nathan W Jones nat@davaosecurity.com for ECE Credits
// This code is a basic starting point and may not be effective against sophisticated rootkits.
// g++ -o rktool rktool.cpp     sudo chmod +x ./rktool.cpp   sudo ./rktool
// getProcessList: This function reads the /proc directory to get a list of process IDs (PIDs).
// It checks if the directory names are numeric, which indicates they are PIDs.
// checkHiddenProcesses: This function reads the current process's status to get a list of visible processes.
// It then compares this list with the list obtained from /proc to identify any hidden processes.
// main: The main function orchestrates the process of getting the list of processes and checking for hidden ones.

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <dirent.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>

std::vector<std::string> getProcessList() {
    std::vector<std::string> processList;
    DIR* dir = opendir("/proc");
    struct dirent* entry;

    if (dir == nullptr) {
        perror("opendir");
        return processList;
    }

    while ((entry = readdir(dir)) != nullptr) {
        if (entry->d_type == DT_DIR) {
            std::string dirName(entry->d_name);
            if (std::all_of(dirName.begin(), dirName.end(), ::isdigit)) {
                processList.push_back(dirName);
            }
        }
    }
    closedir(dir);
    return processList;
}

void checkHiddenProcesses(const std::vector<std::string>& processList) {
    std::ifstream procStatus("/proc/self/status");
    std::string line;
    std::vector<std::string> visibleProcesses;

    while (std::getline(procStatus, line)) {
        if (line.find("Name:") != std::string::npos) {
            visibleProcesses.push_back(line.substr(line.find(":") + 2));
        }
    }

    std::cout << "Detected Processes:\n";
    for (const auto& pid : processList) {
        std::string cmdlinePath = "/proc/" + pid + "/cmdline";
        std::ifstream cmdlineFile(cmdlinePath);
        std::string cmdline;

        if (cmdlineFile) {
            std::getline(cmdlineFile, cmdline);
            if (std::find(visibleProcesses.begin(), visibleProcesses.end(), cmdline) == visibleProcesses.end()) {
                std::cout << "Hidden Process Detected: PID " << pid << " CMD: " << cmdline << std::endl;
            }
        }
    }
}

int main() {
    std::vector<std::string> processList = getProcessList();
    checkHiddenProcesses(processList);
    return 0;
}
