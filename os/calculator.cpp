#include<iostream>
#include<unistd.h>
#include<cstdlib>
#include<cstring>
#include<sys/shm.h>
#include <cmath>
#include <string>
#include <limits>

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

using namespace std;
int n1,n2,n3=0;
int main()
{
    cout << COLOR_CYAN << "\n\t\t-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*" << endl;
    cout << "\t\t\t\tCALCULATOR" << endl;
    cout << "\t\t-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*" << COLOR_RESET << endl;
    
    int *shared_memory;
    int shmid;
    shmid=shmget((key_t)1122,sizeof(int),0666);
    shared_memory=(int*)shmat(shmid,NULL,0);
    int new_data =*shared_memory-10;
    *shared_memory=new_data;

    if (shmdt(shared_memory) == -1) {
        perror("shmdt error");
        exit(1);
    }
    
    int choice,chi;
    do {
        cout << COLOR_YELLOW << "\nPress 1 to start process" << endl;
        cout << "Press 0 to exit" << COLOR_RESET << endl;
        cin >> choice;
        
        if(choice==1) {
            cout << COLOR_CYAN << "\nAvailable Operations:" << endl;
            cout << COLOR_GREEN << "1. " << COLOR_RESET << "Addition" << endl;
            cout << COLOR_GREEN << "2. " << COLOR_RESET << "Subtraction" << endl;
            cout << COLOR_GREEN << "3. " << COLOR_RESET << "Multiplication" << endl;
            cout << COLOR_GREEN << "4. " << COLOR_RESET << "Division" << endl;
            
            cout << COLOR_YELLOW << "\nEnter operation number: " << COLOR_RESET;
            cin >> chi;
            
            cout << COLOR_YELLOW << "Enter first number: " << COLOR_RESET;
            cin >> n1;
            cout << COLOR_YELLOW << "Enter second number: " << COLOR_RESET;
            cin >> n2;

            if(chi==1) {
                n3=n1+n2;
                cout << COLOR_GREEN << "\nAddition of " << n1 << " and " << n2 << " = " << n3 << COLOR_RESET << endl;
            }
            else if(chi==2) {
                n3=n1-n2;
                cout << COLOR_GREEN << "\nSubtraction of " << n1 << " and " << n2 << " = " << n3 << COLOR_RESET << endl;
            }
            else if(chi==3) {
                n3=n1*n2;
                cout << COLOR_GREEN << "\nMultiplication of " << n1 << " and " << n2 << " = " << n3 << COLOR_RESET << endl;
            }
            else if(chi==4) {
                if(n2 != 0) {
                    n3=n1/n2;
                    cout << COLOR_GREEN << "\nDivision of " << n1 << " and " << n2 << " = " << n3 << COLOR_RESET << endl;
                } else {
                    cout << COLOR_RED << "\nError: Division by zero!" << COLOR_RESET << endl;
                }
            }
            else {
                cout << COLOR_RED << "\nInvalid operation selected!" << COLOR_RESET << endl;
                choice=0;
            }
        }
        if(choice!=0 && choice!=1) {
            cout << COLOR_RED << "\nInvalid choice!" << COLOR_RESET << endl;
            choice=0;
        }
    } while(choice==1);

    {
        int *shared_memory;
        int shmid;
        shmid=shmget((key_t)1122,sizeof(int),0666);
        shared_memory=(int*)shmat(shmid,NULL,0);
        int new_data =*shared_memory+10;
        *shared_memory=new_data;
        if (shmdt(shared_memory) == -1) {
            perror("shmdt error");
            exit(1);
        }
        cout << COLOR_CYAN << "\nThank you for using the calculator!" << COLOR_RESET << endl;
        return 0;
    }
    return 0;
}
