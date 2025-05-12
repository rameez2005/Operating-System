#include<iostream>
#include<cstring>
#include<fstream>
#include<unistd.h>
#include<sys/shm.h>
using namespace std;
int main()
{
       cout<<"\t\t-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*"<<endl;
	cout << "\t\t\tWELCOME TO FILE CREATION PROCCESS!"<<endl;
	cout<<"\t\t-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*"<<endl;
int *shared_memory;
int shmid;
shmid=shmget((key_t)1122,sizeof(int),0666);

shared_memory=(int*)shmat(shmid,NULL,0);
int new_data =*shared_memory-6 ;
*shared_memory=new_data;

if (shmdt(shared_memory) == -1) {
        perror("shmdt error");
        exit(1);
    }
ofstream File;
int choice;
do
{
cout<<"Press 1 to create file"<<endl;
cout<<"Press 0 to exit"<<endl;
cin>>choice;
if(choice==1)
{
string filename;
cout<<"Enter Filename: ";
cin>>filename;
File.open(filename);
cout<<"File is successfully created!!"<<endl;
File.close();
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
   int new_data =*shared_memory-6 ;
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
