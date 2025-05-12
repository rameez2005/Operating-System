#include <iostream>
#include <ctime>
#include <cstdlib>
#include <unistd.h>
#include <sys/shm.h>
#include <thread>
#include <atomic>
using namespace std;

atomic<bool> running(true);

void checkForExit() {
    cout << "\nPress Enter to exit..." << endl;
    cin.get();
    running = false;
}

void clearScreen() {
    #ifdef _WIN32
        system("cls");
    #else
        system("clear");
    #endif
}

int main() {
    cout << "\t\t-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*" << endl;
    cout << "\t\t\tWELCOME TO DIGITAL CLOCK!" << endl;
    cout << "\t\t-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*" << endl;

    // Initialize shared memory
    int *shared_memory;
    int shmid = shmget((key_t)1122, sizeof(int), 0666);
    if (shmid == -1) {
        perror("shmget failed");
        return 1;
    }

    shared_memory = (int*)shmat(shmid, NULL, 0);
    if (shared_memory == (void*)-1) {
        perror("shmat failed");
        return 1;
    }

    int new_data = *shared_memory - 30;
    *shared_memory = new_data;

    if (shmdt(shared_memory) == -1) {
        perror("shmdt error");
        return 1;
    }

    // Start exit checker thread
    thread exitThread(checkForExit);
    exitThread.detach();

    int sec_prev = 0;
    while(running) {
        int seconds, minutes, hours;
        string str;
    
        // storing total seconds
        time_t total_seconds = time(0);
    
        // getting values of seconds, minutes and hours
        struct tm* ct;
        #ifdef _WIN32
            struct tm time_info;
            localtime_s(&time_info, &total_seconds);  // Windows-safe version
            ct = &time_info;
        #else
            ct = localtime(&total_seconds);
        #endif
    
        seconds = ct->tm_sec;
        minutes = ct->tm_min;
        hours = ct->tm_hour;
        
        // converting it into 12 hour format
        str = (hours >= 12) ? "PM" : "AM";
        hours = (hours % 12);
        // If hours is 0, set it to 12 (12 AM/PM format)
        if (hours == 0) hours = 12;
        
        // printing the result
        if (seconds == sec_prev + 1 || (sec_prev == 59 && seconds == 0)) {
            clearScreen();
            cout << "\t\t-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*" << endl;
            cout << "\t\t\tWELCOME TO DIGITAL CLOCK!" << endl;
            cout << "\t\t-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*" << endl;
            cout << "\t\t\t\t" << (hours < 10 ? "0" : "") << hours << ":"
                 << (minutes < 10 ? "0" : "") << minutes << ":"
                 << (seconds < 10 ? "0" : "") << seconds << " " << str << endl;
            cout << "\nPress Enter to exit..." << endl;
        }
        
        sec_prev = seconds;
        this_thread::sleep_for(chrono::milliseconds(100)); // Reduce CPU usage
    }

    // Cleanup shared memory
    shmid = shmget((key_t)1122, sizeof(int), 0666);
    if (shmid == -1) {
        perror("shmget failed");
        return 1;
    }

    shared_memory = (int*)shmat(shmid, NULL, 0);
    if (shared_memory == (void*)-1) {
        perror("shmat failed");
        return 1;
    }

    new_data = *shared_memory + 30;
    *shared_memory = new_data;

    if (shmdt(shared_memory) == -1) {
        perror("shmdt error");
        return 1;
    }

    cout << "Thank you for using the Digital Clock!" << endl;
    return 0;
}
