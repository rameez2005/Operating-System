#include <iostream>
#include <iomanip>
#include <ctime>
#include<sys/shm.h>
using namespace std;

int main()
{
        cout<<"\t\t-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*"<<endl;
	cout << "\t\t\tWELCOME TO CALENDER PROCCESS!"<<endl;
	cout<<"\t\t-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*"<<endl;
  int *shared_memory;
int shmid;
shmid=shmget((key_t)1122,sizeof(int),0666);
shared_memory=(int*)shmat(shmid,NULL,0);
int new_data =*shared_memory-10 ;
*shared_memory=new_data;
if (shmdt(shared_memory) == -1) {
        perror("shmdt error");
        exit(1);
    }


    // Get current year and month
    time_t now = time(0);
    tm *ltm = localtime(&now);
    int year = 1900 + ltm->tm_year;
    int month = 1 + ltm->tm_mon;

    // Input year and month if desired
    int choice=1;
    while(choice==1)
    {
    cout << "Enter year (0 to use current year): ";
    cin >> year;
    if (year == 0)
    {
        year = 1900 + ltm->tm_year;
    }
    cout << "Enter month (1-12, 0 to use current month): ";
    cin >> month;
    if (month == 0)
    {
        month = 1 + ltm->tm_mon;
    }

    // Determine number of days in month and starting day of week
    int daysInMonth, startDay;
    if (month == 2) // February
    {
        if ((year % 4 == 0 && year % 100 != 0) || year % 400 == 0) //leap year
        {
            daysInMonth = 29;
        }
        else
        {
            daysInMonth = 28;
        }
    }
    else if (month == 4 || month == 6 || month == 9 || month == 11)
    {
        daysInMonth = 30;
    }
    else
    {
        daysInMonth = 31;
    }

    // January 1st of input year
    tm jan1st = { 0, 0, 0, 1, 0, year - 1900 };
    time_t jan1stTime = mktime(&jan1st);

    // Starting day of input month
    tm start = *localtime(&jan1stTime);
    start.tm_mon = month - 1;
    mktime(&start);
    startDay = start.tm_wday;

    // Display calendar
    cout << endl << setw(17) << " " << month << "/" << year << endl;
    cout << "Sun Mon Tue Wed Thu Fri Sat" << endl;

    for (int i = 0; i < startDay; i++)
    {
        cout << "    ";
    }

    for (int i = 1; i <= daysInMonth; i++)
    {
        cout << setw(3) << i << " ";

        if ((i + startDay) % 7 == 0)
        {
            cout << endl;
        }
    }

    cout << endl;
    cout<<"Enter 1 to continue and 0 to exit: ";
    cin>>choice;
    if(choice!=1 && choice!=0)
    {
     choice=0;
    }
    }

    return 0;
}

