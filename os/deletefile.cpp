#include<iostream>
#include<cstring>
#include<fstream>
#include<stdio.h>
#include<unistd.h>
#include<sys/shm.h>
using namespace std;
int main()
{
        cout<<"\t\t-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*"<<endl;
	cout << "\t\t\tWELCOME TO FILE DELETION PROCCESS!"<<endl;
	cout<<"\t\t-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*"<<endl;
int choice;
int *shared_memory;
int shmid;
shmid=shmget((key_t)1122,sizeof(int),0666);

shared_memory=(int*)shmat(shmid,NULL,0);
int new_data =*shared_memory-3 ;
*shared_memory=new_data;

if (shmdt(shared_memory) == -1) {
        perror("shmdt error");
        exit(1);
    }

char in ,fn[35];
do
{
cout<<"Press 1 to delete file"<<endl;
cout<<"Press 0 to exit"<<endl;
cin>>choice;
if(choice==1)
{
    cout<<"Enter File Name: ";
    cin>>fn;
    if(!fopen(fn,"w"))
    {
      cout<<"Error in opening!"<<endl;
    }
    else
    {
      remove(fn);
      cout<<"File "<<fn<<" is Successfully deleted"<<endl;
    }
}
if(choice!=0 && choice!=1)
{
 choice=0;
}
}while(choice==1);
sleep(2);
if((choice==0)||(choice!=0 && choice!=1))
{
    int *shared_memory;
    int shmid;
    shmid=shmget((key_t)1122,sizeof(int),0666);
    shared_memory=(int*)shmat(shmid,NULL,0);
    int new_data =*shared_memory+3 ;
    *shared_memory=new_data;
    if (shmdt(shared_memory) == -1) 
    {
        perror("shmdt error");
        exit(1);
    }
    return 0;
    }

return 0;
}
