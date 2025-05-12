#include <iostream>
#include <string>
#include <cstring>
#include <unistd.h>
#include <sys/shm.h>
using namespace std;

void clearScreen() {
    #ifdef _WIN32
        system("cls");
    #else
        system("clear");
    #endif
}

int main() {
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

    bool running = true;
    while(running) {
        clearScreen();
        cout << "\t\t-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*" << endl;
        cout << "\t\t\tSTRING LENGTH CALCULATOR" << endl;
        cout << "\t\t-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*" << endl;
        
        string input;
        cout << "\nEnter a string (or 'exit' to quit): ";
        getline(cin, input);

        if(input == "exit") {
            running = false;
            continue;
        }

        if(input.empty()) {
            cout << "Error: Empty string not allowed!" << endl;
            sleep(2);
            continue;
        }

        // Calculate length using different methods
        int length1 = input.length();  // Using string class method
        int length2 = input.size();    // Using string class method
        int length3 = strlen(input.c_str());  // Using C-style string

        cout << "\nResults:" << endl;
        cout << "----------------------------------------" << endl;
        cout << "Input string: \"" << input << "\"" << endl;
        cout << "Length using length(): " << length1 << " characters" << endl;
        cout << "Length using size(): " << length2 << " characters" << endl;
        cout << "Length using strlen(): " << length3 << " characters" << endl;
        
        // Additional information
        cout << "\nCharacter Analysis:" << endl;
        cout << "----------------------------------------" << endl;
        cout << "First character: '" << input[0] << "'" << endl;
        cout << "Last character: '" << input[input.length()-1] << "'" << endl;
        
        // Count spaces
        int spaces = 0;
        for(char c : input) {
            if(c == ' ') spaces++;
        }
        cout << "Number of spaces: " << spaces << endl;
        
        cout << "\nPress Enter to continue...";
        cin.get();
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

    cout << "Thank you for using the String Length Calculator!" << endl;
    return 0;
} 