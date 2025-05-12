#include<iostream> 
#include<string> 
#include<unistd.h> 
#include<stdlib.h>
#include<pthread.h>
#include <bits/stdc++.h>
#include<algorithm>
#include<string.h>
#include<sys/shm.h>
#include<stdio.h>
#include<queue>
#include<mutex>
#ifdef _WIN32
#include <windows.h>
#endif
using namespace std;

// Color definitions
#ifdef _WIN32
    #define COLOR_RED     "\033[31m"
    #define COLOR_GREEN   "\033[32m"
    #define COLOR_YELLOW  "\033[33m"
    #define COLOR_BLUE    "\033[34m"
    #define COLOR_MAGENTA "\033[35m"
    #define COLOR_CYAN    "\033[36m"
    #define COLOR_RESET   "\033[0m"
#else
    #define COLOR_RED     "\033[31m"
    #define COLOR_GREEN   "\033[32m"
    #define COLOR_YELLOW  "\033[33m"
    #define COLOR_BLUE    "\033[34m"
    #define COLOR_MAGENTA "\033[35m"
    #define COLOR_CYAN    "\033[36m"
    #define COLOR_RESET   "\033[0m"
#endif

// Mutex for protecting shared resources
mutex queueMutex;
mutex flagMutex;

queue<int> wqueue;
queue<int> rqueue;
bool flag = true;
int i;

// Global variables
int taskCount = 15;  // Initialize with default number of tasks

// Cross-platform clear screen function
void clearScreen() {
    #ifdef _WIN32
        system("cls");
    #else
        system("clear");
    #endif
}

// Thread-safe queue operations
void safePush(queue<int>& q, int value) {
    lock_guard<mutex> lock(queueMutex);
    q.push(value);
}

int safePop(queue<int>& q) {
    lock_guard<mutex> lock(queueMutex);
    if(q.empty()) return -1;
    int value = q.front();
    q.pop();
    return value;
}

void showq(queue<int> wqueue) {
    lock_guard<mutex> lock(queueMutex);
    queue<int> g = wqueue;
    while (!g.empty()) {
        cout << '\t' << g.front();
        g.pop();
    }
    cout << '\n';
}

pthread_t thrTask[15];///thread pool for our 2o tasks
int n=15;//to keep the value up to which our loops have to operate
int *currentIDArr = new int [15];///creating ids for each process
//array for setting the priority for each process
int *currentPriorityArr = new int[15];
int IDPosition = 0;//process index track
int positionPriority = 0;//priority array index track
int RAM;//variable in which we store the ram that we will store the ram given by the user
int *shared_memory;
struct Task
{//struct for kepping the values that are to identify and stored each process 
  int id;
  string name;
  int ramUse; //in MBs
  int priority; //Higher the number ,higher the  priority
};
Task t[15];

//the scheduling algorithm that we will be using for the process
void priorityScheduling(){
     for(int i=0;i<n-1;i++)
     {
      for(int j=0;j<n-i-1;j++)
       {
        if(t[j].priority < t[j+1].priority)
        {
         swap(t[j].id,t[j+1].id);
         swap(t[j].name,t[j+1].name); 
         swap(t[j].ramUse,t[j+1].ramUse);
         swap(t[j].priority,t[j+1].priority);
        }
      }
     }
  }
  
  ///function for showing each process with all its properties
void showAll(){
	for(int i=0;i<n;i++) {
    	cout<<t[i].id<<"\t"<<t[i].name<<"\t\t"<<t[i].ramUse<<"\t"<<t[i].priority<<endl; 
    }
}

//function that we will use if a process is stoped and the next is choosen 
//this is simply for contexting switching
int ContextSwitch(int ID, Task t[]){
    int pri = -1;
    for(int i = 0; i < n; i++){
        if(ID == t[i].id){
            pri = t[i].priority;
            break;
        }
    }
    if(pri == -1) {
        cout << "Error: Process ID " << ID << " not found!" << endl;
    }
    return pri;
}
void board()
{
    clearScreen();
    cout << COLOR_CYAN;
    cout << "\n\n\n\n\n\n\n\n";
    cout << "          :::::::::      :::.         ::::::::::.                               " << endl;
    cout << "          '`````;;;      ;;`;;         `;;;```.;;;                              " << endl;
    cout << "              .n[['     ,[[ '[[,        `]]nnn]]'                               " << endl;
    cout << "            ,$$P\"      c$$$cc$$$c        $$$\"                                  " << endl;
    cout << "          ,888bo,_      888   888,       888o                                   " << endl;
    cout << "           `\"\"*UMM      YMM   \"\"`        YMMMb                                  " << endl;
    cout << COLOR_RED;
    cout << "                                                              ...      .::::::. " << endl;
    cout << "                                                           .;;;;;;;.  ;;;`    ` " << endl;
    cout << "                                                          ,[[     \\[[,'[==/[[[[," << endl;
    cout << "                                                          $$$,     $$$  '''    $" << endl;
    cout << "                                                          \"888,_ _,88P 88b    dP" << endl;
    cout << "                                                            \"YMMMMMP\"   \"YMmMY\" " << endl;
    cout << COLOR_RESET;
    sleep(5);
    clearScreen();
}
/////proccess1////////
/////calculator/////////
bool executeProcess(const string& filename) {
    string command = "g++ " + filename + " -o " + filename.substr(0, filename.find(".cpp"));
    if(system(command.c_str()) != 0) {
        cout << "Error compiling " << filename << endl;
        return false;
    }
    
    command = "./" + filename.substr(0, filename.find(".cpp"));
    if(system(command.c_str()) != 0) {
        cout << "Error executing " << filename << endl;
        return false;
    }
    return true;
}

void *calculator(void *args) {
    sleep(1); // Reduced sleep time
    if(!executeProcess("calculator.cpp")) {
        cout << "Calculator process failed to start" << endl;
    }
    return NULL;
}
/////proccess2////////
/////Tic Tac Toe/////////
void *tictactoe(void *args) {
    sleep(1);
    if(!executeProcess("tictactoe.cpp")) {
        cout << "Tic Tac Toe process failed to start" << endl;
    }
    return NULL;
}
/////proccess3////////
/////binarysearch/////////
void *binar_search(void *args) {
    sleep(1);
    if(!executeProcess("binarysearch.cpp")) {
        cout << "Binary Search process failed to start" << endl;
    }
    return NULL;
}
/////proccess4////////
/////Banking System/////////
void *banking(void *args) {
    sleep(1);
    if(!executeProcess("banking.cpp")) {
        cout << "Banking process failed to start" << endl;
    }
    return NULL;
}
/////proccess5////////
/////Guessing Game/////////
void *Guessing_game(void *args) {
    sleep(1);
    if(!executeProcess("guessgame.cpp")) {
        cout << "Guessing Game process failed to start" << endl;
    }
    return NULL;
}
/////proccess6////////
/////Message Box/////////
void *message(void *args) {
    sleep(1);
    if(!executeProcess("messagebox.cpp")) {
        cout << "Message Box process failed to start" << endl;
    }
    return NULL;
}
/////proccess7////////
/////Create File/////////
void *createfile(void *args) {
    sleep(1);
    if(!executeProcess("createfile.cpp")) {
        cout << "Create File process failed to start" << endl;
    }
    return NULL;
}
/////proccess8////////
/////Delete File/////////
void *delete_file(void *args) {
    sleep(1);
    if(!executeProcess("deletefile.cpp")) {
        cout << "Delete File process failed to start" << endl;
    }
    return NULL;
}
/////proccess9////////
/////calender/////////
void *calender(void *args) {
    sleep(1);
    if(!executeProcess("calender.cpp")) {
        cout << "Calendar process failed to start" << endl;
    }
    return NULL;
}

/////proccess10////////
/////time/////////
void *factorial(void *args) {
    sleep(1);
    if(!executeProcess("factorial.cpp")) {
        cout << "Factorial process failed to start" << endl;
    }
    return NULL;
}
/////proccess11////////
/////time/////////
void *clock_function(void *args) {
    sleep(1);
    if(!executeProcess("time.cpp")) {
        cout << "Time process failed to start" << endl;
    }
    return NULL;
}
/////proccess12////////
/////Stop Watch/////////
void *stopwatch(void *args) {
    sleep(1);
    if(!executeProcess("stopwatch.cpp")) {
        cout << "Stopwatch process failed to start" << endl;
    }
    return NULL;
}
/////proccess13////////
/////Length of String/////////
void *length(void *args) {
    sleep(1);
    if(!executeProcess("stringlen.cpp")) {
        cout << "String Length process failed to start" << endl;
    }
    return NULL;
}
/////proccess14////////
/////Find Prime/////////
void *prime(void *args) {
    sleep(1);
    if(!executeProcess("findprime.cpp")) {
        cout << "Find Prime process failed to start" << endl;
    }
    return NULL;
}
/////proccess15////////
/////Hangman Game/////////
void *hangman_game(void *args) {
    sleep(1);
    if(!executeProcess("hangman.cpp")) {
        cout << "Hangman Game process failed to start" << endl;
    }
    return NULL;
}

///this function is used for telling which process or task with how much prority and ram useage is runnig.
//////23///////

void *TaskManager(void *args){

cout << "ID\tName\t\tRam"<<endl;
	int ID;
	for(int i = 0; i< IDPosition-1;i++){
		ID = currentIDArr[i];
		cout << ID << "\t" << t[ID].name << "\t" << t[ID].ramUse<<endl;
	}
sleep(8);
 return NULL;
}

// Fix the getCurrentTime function
string getCurrentTime() {
    time_t now;
    time(&now);  // Use time() with pointer to avoid ambiguity
    struct tm tstruct;
    char buf[80];
    
    #ifdef _WIN32
        localtime_s(&tstruct, &now);  // Windows-safe version
    #else
        tstruct = *localtime(&now);
    #endif
    
    // Format the time string with %I for 12-hour clock (01-12)
    strftime(buf, sizeof(buf), "%I:%M:%S %p", &tstruct);
    return string(buf);
}

// Add these validation functions after getCurrentTime()
bool isValidNumber(const string& input) {
    return !input.empty() && all_of(input.begin(), input.end(), ::isdigit);
}

int getValidNumberInput(const string& prompt, int min, int max) {
    string input;
    int number;
    
    while (true) {
        cout << prompt;
        getline(cin, input);
        
        // Check if input is empty
        if (input.empty()) {
            cout << "Input cannot be empty. Please try again." << endl;
            continue;
        }
        
        // Check if input is a valid number
        if (!isValidNumber(input)) {
            cout << "Please enter a valid number." << endl;
            continue;
        }
        
        // Convert to number and check range
        number = stoi(input);
        if (number < min || number > max) {
            cout << "Please enter a number between " << min << " and " << max << "." << endl;
            continue;
        }
        
        return number;
    }
}

string getValidStringInput(const string& prompt, int minLength = 1, int maxLength = 50) {
    string input;
    
    while (true) {
        cout << prompt;
        getline(cin, input);
        
        // Remove leading and trailing whitespace
        input = input.substr(input.find_first_not_of(" \t"), 
                           input.find_last_not_of(" \t") - input.find_first_not_of(" \t") + 1);
        
        if (input.length() < minLength) {
            cout << "Input must be at least " << minLength << " characters long." << endl;
            continue;
        }
        
        if (input.length() > maxLength) {
            cout << "Input must not exceed " << maxLength << " characters." << endl;
            continue;
        }
        
        return input;
    }
}

void userFunc() {
    bool cond1 = true;
    int choice1, pri;
    string confirm;
    
    // Clear any existing input buffer
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    
    while(cond1) {
        clearScreen();
        cout << COLOR_CYAN << "\t-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*" << endl;
        cout << "\t\t\t\tMAIN MENU" << endl;
        cout << "\t\t\tCurrent Time: " << COLOR_YELLOW << getCurrentTime() << COLOR_CYAN << endl;
        cout << "\t\t\tAvailable RAM: " << COLOR_YELLOW << *shared_memory << " MB" << COLOR_CYAN << endl;
        cout << "\t-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*" << endl;
        
        // Display menu with proper numbering
        for(int i = 0; i < taskCount; i++) {
            if(t[i].name != "") {
                cout << COLOR_GREEN << t[i].id << ".  " << COLOR_RESET << t[i].name << endl;
            }
        }
        cout << COLOR_GREEN << taskCount + 1 << ".  " << COLOR_GREEN << "Switch to Kernel Mode" << COLOR_RESET << endl;
        cout << COLOR_GREEN << taskCount + 2 << ".  " << COLOR_RED << "Shutdown" << endl;
        cout << COLOR_RESET;
        
        // Get valid menu choice
        choice1 = getValidNumberInput("\nEnter your choice (1-" + to_string(taskCount + 2) + "): ", 1, taskCount + 2);

        // Check if user wants to switch to kernel mode
        if(choice1 == taskCount + 1) {
            cout << "Switching to Kernel Mode..." << endl;
            sleep(1);
            return;
        }
        
        // Check if user wants to shutdown
        if(choice1 == taskCount + 2) {
            cout << "Are you sure you want to shutdown? (y/n): ";
            getline(cin, confirm);
            if (confirm == "y" || confirm == "Y") {
                cout << "Shutting down..." << endl;
                sleep(2);
                cond1 = false;
            }
            continue;
        }

        // Check RAM availability for the selected task
        if(t[choice1-1].ramUse > *shared_memory) {
            cout << "Insufficient RAM! Required: " << t[choice1-1].ramUse 
                 << "MB, Available: " << *shared_memory << "MB" << endl;
            cout << "Press Enter to continue...";
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            continue;
        }

        // Process the choice
        switch(choice1) {
            case 1:
                cout << COLOR_CYAN << "Opening the calculator..." << COLOR_RESET << endl;
                pthread_create(&thrTask[0], NULL, &calculator, NULL);
                pthread_join(thrTask[0], NULL);
                cout << COLOR_GREEN << "Calculator process completed" << COLOR_RESET << endl;
                RAM = RAM + t[choice1-1].ramUse;
                cout << COLOR_YELLOW << "Press Enter to continue..." << COLOR_RESET;
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                break;

            case 2:
                cout << COLOR_CYAN << "Opening Tic Tac Toe..." << COLOR_RESET << endl;
                pthread_create(&thrTask[1], NULL, &tictactoe, NULL);
                pthread_join(thrTask[1], NULL);
                cout << COLOR_GREEN << "Tic Tac Toe process completed" << COLOR_RESET << endl;
                RAM = RAM + t[choice1-1].ramUse;
                cout << COLOR_YELLOW << "Press Enter to continue..." << COLOR_RESET;
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                break;

            case 3:
                cout << COLOR_CYAN << "Opening Binary Search..." << COLOR_RESET << endl;
                pthread_create(&thrTask[2], NULL, &binar_search, NULL);
                pthread_join(thrTask[2], NULL);
                cout << COLOR_GREEN << "Binary Search process completed" << COLOR_RESET << endl;
                RAM = RAM + t[choice1-1].ramUse;
                cout << COLOR_YELLOW << "Press Enter to continue..." << COLOR_RESET;
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                break;

            case 4:
                cout << COLOR_CYAN << "Opening Banking System..." << COLOR_RESET << endl;
                pthread_create(&thrTask[3], NULL, &banking, NULL);
                pthread_join(thrTask[3], NULL);
                cout << COLOR_GREEN << "Banking System process completed" << COLOR_RESET << endl;
                RAM = RAM + t[choice1-1].ramUse;
                cout << COLOR_YELLOW << "Press Enter to continue..." << COLOR_RESET;
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                break;

            case 5:
                cout << COLOR_CYAN << "Opening Guessing Game..." << COLOR_RESET << endl;
                pthread_create(&thrTask[4], NULL, &Guessing_game, NULL);
                pthread_join(thrTask[4], NULL);
                cout << COLOR_GREEN << "Guessing Game process completed" << COLOR_RESET << endl;
                RAM = RAM + t[choice1-1].ramUse;
                cout << COLOR_YELLOW << "Press Enter to continue..." << COLOR_RESET;
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                break;

            case 6:
                cout << COLOR_CYAN << "Opening Message Box..." << COLOR_RESET << endl;
                pthread_create(&thrTask[5], NULL, &message, NULL);
                pthread_join(thrTask[5], NULL);
                cout << COLOR_GREEN << "Message Box process completed" << COLOR_RESET << endl;
                RAM = RAM + t[choice1-1].ramUse;
                cout << COLOR_YELLOW << "Press Enter to continue..." << COLOR_RESET;
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                break;

            case 7:
                cout << COLOR_CYAN << "Opening Create File..." << COLOR_RESET << endl;
                pthread_create(&thrTask[6], NULL, &createfile, NULL);
                pthread_join(thrTask[6], NULL);
                cout << COLOR_GREEN << "Create File process completed" << COLOR_RESET << endl;
                RAM = RAM + t[choice1-1].ramUse;
                cout << COLOR_YELLOW << "Press Enter to continue..." << COLOR_RESET;
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                break;

            case 8:
                cout << COLOR_CYAN << "Opening Delete File..." << COLOR_RESET << endl;
                pthread_create(&thrTask[7], NULL, &delete_file, NULL);
                pthread_join(thrTask[7], NULL);
                cout << COLOR_GREEN << "Delete File process completed" << COLOR_RESET << endl;
                RAM = RAM + t[choice1-1].ramUse;
                cout << COLOR_YELLOW << "Press Enter to continue..." << COLOR_RESET;
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                break;

            case 9:
                cout << COLOR_CYAN << "Opening Calendar..." << COLOR_RESET << endl;
                pthread_create(&thrTask[8], NULL, &calender, NULL);
                pthread_join(thrTask[8], NULL);
                cout << COLOR_GREEN << "Calendar process completed" << COLOR_RESET << endl;
                RAM = RAM + t[choice1-1].ramUse;
                cout << COLOR_YELLOW << "Press Enter to continue..." << COLOR_RESET;
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                break;

            case 10:
                cout << COLOR_CYAN << "Opening Clock..." << COLOR_RESET << endl;
                pthread_create(&thrTask[9], NULL, &clock_function, NULL);
                pthread_join(thrTask[9], NULL);
                cout << COLOR_GREEN << "Clock process completed" << COLOR_RESET << endl;
                RAM = RAM + t[choice1-1].ramUse;
                cout << COLOR_YELLOW << "Press Enter to continue..." << COLOR_RESET;
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                break;

            case 11:
                cout << COLOR_CYAN << "Opening Factorial Calculator..." << COLOR_RESET << endl;
                pthread_create(&thrTask[10], NULL, &factorial, NULL);
                pthread_join(thrTask[10], NULL);
                cout << COLOR_GREEN << "Factorial process completed" << COLOR_RESET << endl;
                RAM = RAM + t[choice1-1].ramUse;
                cout << COLOR_YELLOW << "Press Enter to continue..." << COLOR_RESET;
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                break;

            case 12:
                cout << COLOR_CYAN << "Opening String Length Calculator..." << COLOR_RESET << endl;
                pthread_create(&thrTask[11], NULL, &length, NULL);
                pthread_join(thrTask[11], NULL);
                cout << COLOR_GREEN << "String Length process completed" << COLOR_RESET << endl;
                RAM = RAM + t[choice1-1].ramUse;
                cout << COLOR_YELLOW << "Press Enter to continue..." << COLOR_RESET;
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                break;

            case 13:
                cout << COLOR_CYAN << "Opening Prime Number Finder..." << COLOR_RESET << endl;
                pthread_create(&thrTask[12], NULL, &prime, NULL);
                pthread_join(thrTask[12], NULL);
                cout << COLOR_GREEN << "Prime Number process completed" << COLOR_RESET << endl;
                RAM = RAM + t[choice1-1].ramUse;
                cout << COLOR_YELLOW << "Press Enter to continue..." << COLOR_RESET;
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                break;

            case 14:
                cout << COLOR_CYAN << "Opening Hangman Game..." << COLOR_RESET << endl;
                pthread_create(&thrTask[13], NULL, &hangman_game, NULL);
                pthread_join(thrTask[13], NULL);
                cout << COLOR_GREEN << "Hangman Game process completed" << COLOR_RESET << endl;
                RAM = RAM + t[choice1-1].ramUse;
                cout << COLOR_YELLOW << "Press Enter to continue..." << COLOR_RESET;
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                break;

            case 15:
                cout << COLOR_CYAN << "Opening Stop Watch..." << COLOR_RESET << endl;
                pthread_create(&thrTask[14], NULL, &stopwatch, NULL);
                pthread_join(thrTask[14], NULL);
                cout << COLOR_GREEN << "Stop Watch process completed" << COLOR_RESET << endl;
                RAM = RAM + t[choice1-1].ramUse;
                cout << COLOR_YELLOW << "Press Enter to continue..." << COLOR_RESET;
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                break;

            default:
                if(choice1 <= taskCount) {
                    cout << COLOR_CYAN << "Opening " << t[choice1-1].name << "..." << COLOR_RESET << endl;
                    cout << COLOR_GREEN << t[choice1-1].name << " process completed" << COLOR_RESET << endl;
                    RAM = RAM + t[choice1-1].ramUse;
                    cout << COLOR_YELLOW << "Press Enter to continue..." << COLOR_RESET;
                    cin.ignore(numeric_limits<streamsize>::max(), '\n');
                } else {
                    cout << COLOR_RED << "Invalid choice! Please try again." << COLOR_RESET << endl;
                    cout << COLOR_YELLOW << "Press Enter to continue..." << COLOR_RESET;
                    cin.ignore(numeric_limits<streamsize>::max(), '\n');
                }
                break;
        }
    }
}

void *initialize(void *args) {
    // Initialize tasks with proper counting
    t[0].id = 1; t[0].name = "Calculator";    
    t[0].ramUse = 10; t[0].priority = 1;
   
    t[1].id = 2; t[1].name = "TIC TAC TOE";   
    t[1].ramUse = 30; t[1].priority = 2;
   
    t[2].id = 3; t[2].name = "Binary Search"; 
    t[2].ramUse = 40; t[2].priority = 4;
   
    t[3].id = 4; t[3].name = "Banking System";     
    t[3].ramUse = 2; t[3].priority = 3;
   
    t[4].id = 5; t[4].name = "Guessing Game";     
    t[4].ramUse = 2; t[4].priority = 3;
   
    t[5].id = 6; t[5].name = "Message Box";
    t[5].ramUse = 30; t[5].priority = 6;
   
    t[6].id = 7; t[6].name = "Create File";    
    t[6].ramUse = 6; t[6].priority = 8;
   
    t[7].id = 8; t[7].name = "Delete File"; 
    t[7].ramUse = 3; t[7].priority = 7;
   
    t[8].id = 9; t[8].name = "Calender";    
    t[8].ramUse = 10; t[8].priority = 9;
   
    t[9].id = 10; t[9].name = "Time Task";  
    t[9].ramUse = 30; t[9].priority = 10;
   
    t[10].id = 11; t[10].name = "Find Factorial";  
    t[10].ramUse = 11; t[10].priority = 11;
   
    t[11].id = 12; t[11].name = "String Length";  
    t[11].ramUse = 11; t[11].priority = 12;
   
    t[12].id = 13; t[12].name = "Find prime";  
    t[12].ramUse = 20; t[12].priority = 13;
   
    t[13].id = 14; t[13].name = "Hangman Task";  
    t[13].ramUse = 15; t[13].priority = 14;
   
    t[14].id = 15; t[14].name = "Stop Watch";  
    t[14].ramUse = 11; t[14].priority = 15;
   
    pthread_exit(NULL);
}

void cleanup() {
    delete[] currentIDArr;
    delete[] currentPriorityArr;
    if(shared_memory != nullptr) {
        shmdt(shared_memory);
    }
}

int main(int argc, char** num)
{
	string r = string(num[1]);
	RAM = stoi(r);
	string c = string(num[2]);
	int Cores = stoi(c);
	string d = string(num[3]);
	int Disk = stoi(d);
	bool cond1=1;
	int choice1;
	pthread_t thr; 
	pthread_create(&thr,NULL,&initialize,NULL);  //loading tasks
	pthread_join(thr,NULL);
  	      	
	string pass;
	string kernalpass="1122";
	clearScreen();



int shmid;
shmid=shmget((key_t)1122,sizeof(int),0666|IPC_CREAT);
shared_memory=(int*)shmat(shmid,NULL,0);
int data=RAM;

*shared_memory=data;
board();
	
	system("gnome-terminal -e  'sh -c \"g++ calender.cpp && ./a.out\"'");
	
	system("gnome-terminal -e  'sh -c \"g++ time.cpp && ./a.out\"'");
	
	system("clear");

	cout << COLOR_CYAN << "\t\t-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*" << endl;
	cout << "\t\t\tWELCOME TO LOGIN PAGE!" << endl;
	cout << "\t\t-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*" << endl;
	cout << COLOR_RESET;
	cout << "Welcome! How do you want to Log In? \nPlease Select from the following options:\n";
	cout << COLOR_GREEN << "1. User.\n2. Kernel.\n" << COLOR_RESET;
	cout << "Enter the number of selected option:";
	cin>>choice1;
	clearScreen();
	
	while(true) {
		switch(choice1) {
			case 1:
				userFunc();
				// After returning from user mode, show login options again
				clearScreen();
				cout<<"\t\t-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*"<<endl;
				cout << "\t\t\t          LOGIN PAGE!"<<endl;
				cout<<"\t\t-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*"<<endl;
				cout<<"Welcome! How do you want to Log In? \nPlease Select from the following options:\n1. User.\n2. Kernal.\nEnter the number of selected option:";
				cin>>choice1;
				clearScreen();
				break;
			case 2:
		        {
		            // Initialize task array with proper bounds checking
		            const int MAX_TASKS = 50;
		            Task t1[MAX_TASKS];
		            
		            // Safely copy existing tasks
		            for(int i = 0; i < 15 && i < MAX_TASKS; ++i) {
		                t1[i].name = t[i].name;
		                t1[i].priority = t[i].priority;
		                t1[i].ramUse = t[i].ramUse;
		            }
		            
		            // Initialize remaining tasks
		            for(int i = 15; i < MAX_TASKS; ++i) {
		                t1[i].name = "";
		                t1[i].priority = 0;
		                t1[i].ramUse = 0;
		            }
		            
		            // Restore original tasks
		            for(int i = 0; i < 15 && i < MAX_TASKS; ++i) {
		                t[i].name = t1[i].name;
		                t[i].priority = t1[i].priority;
		                t[i].ramUse = t1[i].ramUse;
		            }    
		               
		            cout << "\t\t-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*" << endl;
		            cout << "\t\t\tPLEASE ENTER PASSWORD TO LOGIN!" << endl;
		            cout << "\t\t-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*" << endl;
		            cout << "Enter password of kernal: ";
		            cin >> pass;
		            
		            if(pass == kernalpass) {
		                cout << COLOR_GREEN << "Password is correct! Welcome to kernel mode." << COLOR_RESET << endl;
		            } else {
		                while(pass != kernalpass) {
		                    cout << COLOR_RED << "Incorrect password. Try again or press 0 to shutdown: " << COLOR_RESET;
		                    cin >> pass;
		                    if(pass == "0") {
		                        cout << COLOR_RED << "Shutting down..." << COLOR_RESET << endl;
		                        return 0;
		                    }
		                }
		            }
		            
		            int kChoice;
		            string taskName;
		            int priority;
		            int ram;
		            int deleteId;
		            bool found;
		            
		            do {
		                clearScreen();
		                cout << COLOR_CYAN << "\t\t-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*" << endl;
		                cout << "\t\t\tKERNEL MODE MENU" << endl;
		                cout << "\t\t-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*" << endl;
		                cout << COLOR_GREEN << "1. " << COLOR_RESET << "Add Task" << endl;
		                cout << COLOR_GREEN << "2. " << COLOR_RESET << "Delete Task" << endl;
		                cout << COLOR_GREEN << "3. " << COLOR_RESET << "View All Tasks" << endl;
		                cout << COLOR_GREEN << "4. " << COLOR_RESET << "Switch to User Mode" << endl;
		                cout << COLOR_GREEN << "5. " << COLOR_RED << "Shutdown" << endl;
		                cout << COLOR_YELLOW << "Enter your choice: " << COLOR_RESET;
		                
		                if (!(cin >> kChoice)) {
		                    cin.clear();
		                    cin.ignore(numeric_limits<streamsize>::max(), '\n');
		                    cout << COLOR_RED << "Invalid input. Please enter a number." << COLOR_RESET << endl;
		                    sleep(2);
		                    continue;
		                }
		                
		                switch(kChoice) {
		                    case 1:
		                        clearScreen();
		                        cout << COLOR_CYAN << "\t\t-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*" << endl;
		                        cout << "\t\t\tADD NEW TASK" << endl;
		                        cout << "\t\t-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*" << endl;
		                        
		                        if(taskCount >= MAX_TASKS) {
		                            cout << COLOR_RED << "Maximum number of tasks reached!" << COLOR_RESET << endl;
		                            sleep(2);
		                            break;
		                        }
		                        
		                        cin.ignore(numeric_limits<streamsize>::max(), '\n');
		                        
		                        cout << COLOR_YELLOW << "Enter the name of Task: " << COLOR_RESET;
		                        getline(cin, taskName);
		                        
		                        if(taskName.empty()) {
		                            cout << COLOR_RED << "Task name cannot be empty!" << COLOR_RESET << endl;
		                            sleep(2);
		                            break;
		                        }
		                        
		                        t[taskCount].name = taskName;
		                        
		                        cout << COLOR_YELLOW << "Enter the Priority of Task (1-20): " << COLOR_RESET;
		                        while (!(cin >> priority) || priority < 1 || priority > 20) {
		                            cin.clear();
		                            cin.ignore(numeric_limits<streamsize>::max(), '\n');
		                            cout << COLOR_RED << "Invalid priority. Please enter a number between 1 and 20: " << COLOR_RESET;
		                        }
		                        t[taskCount].priority = priority;
		                        
		                        cout << COLOR_YELLOW << "Enter the RAM usage of Task (in MB): " << COLOR_RESET;
		                        while (!(cin >> ram) || ram <= 0) {
		                            cin.clear();
		                            cin.ignore(numeric_limits<streamsize>::max(), '\n');
		                            cout << COLOR_RED << "Invalid RAM usage. Please enter a positive number: " << COLOR_RESET;
		                        }
		                        t[taskCount].ramUse = ram;
		                        
		                        t[taskCount].id = taskCount + 1;
		                        taskCount++;
		                        
		                        cout << COLOR_GREEN << "\nTask added successfully!" << COLOR_RESET << endl;
		                        cout << COLOR_CYAN << "\nCurrent Tasks:" << COLOR_RESET << endl;
		                        cout << COLOR_YELLOW << "ID\tName\t\tPriority\tRAM (MB)" << endl;
		                        cout << "----------------------------------------" << COLOR_RESET << endl;
		                        for(int i = 0; i < taskCount; i++) {
		                            cout << t[i].id << "\t" << t[i].name << "\t\t" << t[i].priority << "\t\t" << t[i].ramUse << endl;
		                        }
		                        sleep(3);
		                        break;
		                        
		                    case 2:
		                        clearScreen();
		                        cout << COLOR_CYAN << "\t\t-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*" << endl;
		                        cout << "\t\t\tDELETE TASK" << endl;
		                        cout << "\t\t-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*" << endl;
		                        
		                        cout << COLOR_YELLOW << "Current Tasks:" << endl;
		                        cout << "ID\tName\t\tPriority\tRAM (MB)" << endl;
		                        cout << "----------------------------------------" << COLOR_RESET << endl;
		                        for(int i = 0; i < taskCount; i++) {
		                            cout << t[i].id << "\t" << t[i].name << "\t\t" << t[i].priority << "\t\t" << t[i].ramUse << endl;
		                        }
		                        
		                        cout << COLOR_YELLOW << "\nEnter the ID of task to delete: " << COLOR_RESET;
		                        while (!(cin >> deleteId) || deleteId < 1 || deleteId > taskCount) {
		                            cin.clear();
		                            cin.ignore(numeric_limits<streamsize>::max(), '\n');
		                            cout << COLOR_RED << "Invalid task ID. Please enter a number between 1 and " << taskCount << ": " << COLOR_RESET;
		                        }
		                        
		                        found = false;
		                        for(int i = 0; i < taskCount; i++) {
		                            if(t[i].id == deleteId) {
		                                for(int j = i; j < taskCount - 1; j++) {
		                                    t[j] = t[j + 1];
		                                    t[j].id = j + 1;
		                                }
		                                taskCount--;
		                                found = true;
		                                cout << COLOR_GREEN << "Task deleted successfully!" << COLOR_RESET << endl;
		                                break;
		                            }
		                        }
		                        
		                        if(!found) {
		                            cout << COLOR_RED << "Task ID not found!" << COLOR_RESET << endl;
		                        }
		                        sleep(2);
		                        break;
		                        
		                    case 3:
		                        clearScreen();
		                        cout << COLOR_CYAN << "\t\t-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*" << endl;
		                        cout << "\t\t\tCURRENT TASKS" << endl;
		                        cout << "\t\t-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*" << endl;
		                        cout << COLOR_YELLOW << "ID\tName\t\tPriority\tRAM (MB)" << endl;
		                        cout << "----------------------------------------" << COLOR_RESET << endl;
		                        for(int i = 0; i < taskCount; i++) {
		                            cout << t[i].id << "\t" << t[i].name << "\t\t" << t[i].priority << "\t\t" << t[i].ramUse << endl;
		                        }
		                        cout << COLOR_YELLOW << "\nPress Enter to continue..." << COLOR_RESET;
		                        cin.ignore();
		                        cin.get();
		                        break;
		                        
		                    case 4:
		                        cout << COLOR_CYAN << "Switching to User Mode..." << COLOR_RESET << endl;
		                        sleep(1);
		                        userFunc();
		                        break;
		                        
		                    case 5:
		                        cout << COLOR_RED << "Shutting down..." << COLOR_RESET << endl;
		                        return 0;
		                        
		                    default:
		                        cout << COLOR_RED << "Invalid choice! Please try again." << COLOR_RESET << endl;
		                        sleep(1);
		                }
		            } while(true);
		        }
		        break;
		}
	}
	
	// Register cleanup function
	atexit(cleanup);
	
	return 0;
}

