#include<iostream>
#include<unistd.h>
#include<cstdlib>
#include<cstring>
#include<time.h>
#include<sys/shm.h>
using namespace std;
void* findprime()
{
    int i, n;
    bool isPrime = true;

    cout << "Enter a positive integer: ";
    cin >> n;

    // 0 and 1 are not prime numbers
    if (n == 0 || n == 1) {
        isPrime = false;
    }
    else {
        for (i = 2; i <= n / 2; ++i) {
            if (n % i == 0) {
                isPrime = false;
                break;
            }
        }
    }
    if (isPrime)
        cout << n << " is a prime number"<<endl;
    else
        cout << n << " is not a prime number"<<endl;
return NULL;
}
int main()
{
        cout<<"\t\t-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*"<<endl;
	cout << "\t\t\tWELCOME TO FINDING PRIME NUMBER PROCCESS!"<<endl;
	cout<<"\t\t-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*"<<endl;
int choice;
    int *shared_memory;
    int shmid;
    shmid=shmget((key_t)1122,sizeof(int),0666);
    shared_memory=(int*)shmat(shmid,NULL,0);
    int new_data =*shared_memory-20;
    *shared_memory=new_data;
    if (shmdt(shared_memory) == -1) 
    {
        perror("shmdt error");
        exit(1);
    }
    
do
{
    cout<<"Press 1 to start proccess"<<endl;
    cout<<"Press 0 to exit"<<endl;
    cin>>choice;
    if(choice==1)
      {
        findprime();  
      }
    if(choice!=0 && choice!=1)
      {
        choice=0;
      }
}while(choice==1);
 sleep(2);
    {
    int *shared_memory;
    int shmid;
    shmid=shmget((key_t)1122,sizeof(int),0666);
    shared_memory=(int*)shmat(shmid,NULL,0);
    int new_data =*shared_memory+20 ;
    *shared_memory=new_data;
    if (shmdt(shared_memory) == -1) 
    {
        perror("shmdt error");
        exit(1);
    }
    }
    return 0;
}
