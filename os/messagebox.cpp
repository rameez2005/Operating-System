#include <iostream>
#include <string>
#include <cstring>
#include <vector>
#include <ctime>
#include <iomanip>
#include <sys/shm.h>
using namespace std;

const int MAX_MESSAGES = 100;

struct Message {
    string sender;
    string content;
    string timestamp;
};

struct User {
    string name;
    vector<Message> messages;
    bool online;
};

vector<User> users;

void initializeUsers() {
    users.push_back({"kamla", vector<Message>(), true});
    users.push_back({"laiba", vector<Message>(), true});
    users.push_back({"ayesha", vector<Message>(), true});
    users.push_back({"saad", vector<Message>(), true});
}

string getCurrentTime() {
    time_t now = time(0);
    tm timeinfo;
    
    #ifdef _WIN32
        localtime_s(&timeinfo, &now);  // Windows-safe version
    #else
        timeinfo = *localtime(&now);
    #endif
    
    // Format time in 12-hour format
    char buffer[80];
    strftime(buffer, sizeof(buffer), "%I:%M:%S %p", &timeinfo);
    return string(buffer);
}

void displayUsers() {
    cout << "\nAvailable Users:" << endl;
    cout << setw(15) << left << "Name" << setw(10) << "Status" << endl;
    cout << string(25, '-') << endl;
    for (const auto& user : users) {
        cout << setw(15) << left << user.name 
             << setw(10) << (user.online ? "Online" : "Offline") << endl;
    }
}

int findUser(const string& name) {
    for (size_t i = 0; i < users.size(); i++) {
        if (users[i].name == name) {
            return i;
        }
    }
    return -1;
}

void sendMessage(const string& sender, const string& receiver, const string& message) {
    int senderIndex = findUser(sender);
    int receiverIndex = findUser(receiver);

    if (senderIndex == -1 || receiverIndex == -1) {
        cout << "Error: One or both users not found." << endl;
        return;
    }

    if (!users[senderIndex].online || !users[receiverIndex].online) {
        cout << "Error: One or both users are offline." << endl;
        return;
    }

    Message newMessage = {sender, message, getCurrentTime()};
    users[receiverIndex].messages.push_back(newMessage);
    cout << "Message sent successfully!" << endl;
}

void readMessages(const string& user) {
    int userIndex = findUser(user);
    if (userIndex == -1) {
        cout << "Error: User not found." << endl;
        return;
    }

    if (!users[userIndex].online) {
        cout << "Error: User is offline." << endl;
        return;
    }

    if (users[userIndex].messages.empty()) {
        cout << "No messages for " << user << endl;
        return;
    }

    cout << "\nMessages for " << user << ":" << endl;
    cout << setw(15) << left << "From" 
         << setw(10) << "Time" 
         << "Message" << endl;
    cout << string(50, '-') << endl;

    for (const auto& msg : users[userIndex].messages) {
        cout << setw(15) << left << msg.sender
             << setw(10) << msg.timestamp
             << msg.content << endl;
    }
    users[userIndex].messages.clear();
}

void toggleUserStatus(const string& user) {
    int userIndex = findUser(user);
    if (userIndex == -1) {
        cout << "Error: User not found." << endl;
        return;
    }
    users[userIndex].online = !users[userIndex].online;
    cout << user << " is now " << (users[userIndex].online ? "online" : "offline") << endl;
}

void displayMenu() {
    cout << "\nMessage Box Menu:" << endl;
    cout << "1. Send Message" << endl;
    cout << "2. Read Messages" << endl;
    cout << "3. Toggle User Status" << endl;
    cout << "4. Display Users" << endl;
    cout << "5. Exit" << endl;
    cout << "Enter your choice: ";
}

int main() {
    cout << "\t\t-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*" << endl;
    cout << "\t\t\tWELCOME TO MESSAGE BOX!" << endl;
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

    initializeUsers();
    int choice;
    string sender, receiver, message, user;

    do {
        displayMenu();
        cin >> choice;
        cin.ignore(); // Clear the newline character

        switch (choice) {
            case 1:
                displayUsers();
                cout << "\nEnter sender name: ";
                getline(cin, sender);
                cout << "Enter receiver name: ";
                getline(cin, receiver);
                cout << "Enter message: ";
                getline(cin, message);
                sendMessage(sender, receiver, message);
                break;

            case 2:
                displayUsers();
                cout << "\nEnter user name to read messages: ";
                getline(cin, user);
                readMessages(user);
                break;

            case 3:
                displayUsers();
                cout << "\nEnter user name to toggle status: ";
                getline(cin, user);
                toggleUserStatus(user);
                break;

            case 4:
                displayUsers();
                break;

            case 5: {
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
                int new_data = *shared_memory + 30;
                *shared_memory = new_data;
                if (shmdt(shared_memory) == -1) {
                    perror("shmdt error");
                    return 1;
                }
                cout << "Thank you for using the Message Box!" << endl;
                return 0;
            }

            default:
                cout << "Invalid choice! Please try again." << endl;
        }
    } while (true);

    return 0;
}

