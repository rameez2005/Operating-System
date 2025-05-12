#include<iostream>
#include<cstdlib>
#include<string>
#include<unistd.h> 
#include<cstring> 
#include <ctime>
#include<sys/shm.h>
using namespace std;
void *guess_game()
{
        int num, guess, tries = 0;
        char x;
	srand(time(0));        //seed random number generator
	num = rand() % 10 + 1; // random number between 1 and 10
	do
	{
		cout << "Enter a guess between 1 and 10 : ";
		cin >> guess;
		tries++;
		if (guess > num)
			cout << "Too high!\n\n";
		else if (guess < num)
			cout << "Too low!\n\n";
		else
			cout << "\nCorrect! You got it in " << tries << " guesses!\n";
	} while (guess != num);
	sleep(3);
	system("clear");
	return 0;
}
int main()
{
        cout<<"\t\t-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*"<<endl;
	cout << "\t\t\tWELCOME TO THE GUESS GAME WORLD!"<<endl;
	cout<<"\t\t-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*"<<endl;
	int choice;
	{
     int *shared_memory;
     int shmid;
     shmid=shmget((key_t)1122,sizeof(int),0666);
	 shared_memory=(int*)shmat(shmid,NULL,0);
     int new_data =*shared_memory-2 ;
     *shared_memory=new_data;
     if (shmdt(shared_memory) == -1) 
     {
        perror("shmdt error");
        exit(1);
     }
    }
 do{ 
     cout<<"Press 1 to start game"<<endl;
     cout<<"Press 0 to exit game"<<endl;
     cin>>choice;
     if(choice==1)
     {
       guess_game();
     }
    if((choice==0) || (choice!=1 && choice!=0))
    {
      	{
	       int *shared_memory;
           int shmid;
           shmid=shmget((key_t)1122,sizeof(int),0666);
           shared_memory=(int*)shmat(shmid,NULL,0);
           int new_data =*shared_memory+2 ;
           *shared_memory=new_data;
            if (shmdt(shared_memory) == -1) 
			{
              perror("shmdt error");
              exit(1);
              cout<<"Game closed"<<endl;
              choice=0;
            }
        }
    }
    }while(choice==1);
    return 0;

}

