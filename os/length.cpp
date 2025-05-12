#include<iostream>
#include<unistd.h>
#include<cstdlib>
#include<cstring>
#include<time.h>
#include<sys/shm.h>
using namespace std;
void* nofalphabets()
{
    char *array_point;
    char c1;
    int count=0, alp=0, digt=0, spcchr=0,oth=0;
    char string_array[100];    
    string str1;
    cout << " Enter a string: ";
    cin>>str1;    
    cout<<endl;
    strcpy(string_array, str1.c_str());
    for(array_point=string_array;*array_point!='\0';array_point++)
    {
        c1=*array_point;
        count++;
		if (isalpha(c1))
		{
		    alp++;
		}
		else if (isdigit(c1))
		{
		    digt++;
		}
        	else
        	{
            	oth++;;
        	}
    }   
        cout<<endl;  
        cout<<"The number of alphabets are: "<<alp<<endl;
        cout<<" The number of digits are: "<<digt<<endl; 
        cout<<" The number of other characters are: "<<oth<<endl<<endl;
        return NULL;
}
int main()
{
int choice;
       cout<<"\t\t-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*"<<endl;
	cout << "\t\t\tWELCOME TO STRING LENGTH PROCCESS!"<<endl;
	cout<<"\t\t-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*"<<endl;
      
int *shared_memory;
int shmid;
shmid=shmget((key_t)1122,sizeof(int),0666);

shared_memory=(int*)shmat(shmid,NULL,0);
int new_data =*shared_memory-11 ;
*shared_memory=new_data;

if (shmdt(shared_memory) == -1) {
        perror("shmdt error");
        exit(1);
    }
do
{
   cout<<"Press 1 to check length of string"<<endl;
   cout<<"Press 0 to exit"<<endl;
   cin>>choice;
   if(choice==1)
    {
      nofalphabets();
    }
    if(choice!=0 && choice!=1)
    {
      choice=0;
    }
}while(choice==1);
 
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
