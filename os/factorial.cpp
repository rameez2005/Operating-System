#include <iostream>
using namespace std;
#include<unistd.h>
#include<sys/shm.h>


int factorial(int n) {
    if(n==0 || n==1) {
        return 1;
    }
    return n*factorial(n-1);
}

int main() {
   cout<<"\t\t-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*"<<endl;
   cout << "\t\t\tWELCOME TO FACTORIAL FINDING PROCCESS!"<<endl;
   cout<<"\t\t-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*"<<endl;
int choice;

int *shared_memory;
int shmid;
shmid=shmget((key_t)1122,sizeof(int),0666);

shared_memory=(int*)shmat(shmid,NULL,0);
int new_data =*shared_memory-11;
*shared_memory=new_data;

if (shmdt(shared_memory) == -1) {
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
    int n;
    cout << "Enter a positive integer: ";
    cin >> n;
    cout << "Factorial of " << n << " is: " << factorial(n)<<endl;
}
if(choice!=0 && choice!=1)
{
 choice=0;
}
}while(choice==1);
    sleep(4);
    
    {
    int *shared_memory;
int shmid;
shmid=shmget((key_t)1122,sizeof(int),0666);

shared_memory=(int*)shmat(shmid,NULL,0);
int new_data =*shared_memory+11 ;
*shared_memory=new_data;

if (shmdt(shared_memory) == -1) {
        perror("shmdt error");
        exit(1);
    }
    }
    return 0;
}
