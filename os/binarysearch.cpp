#include <iostream>
#include<sys/shm.h>
using namespace std;

int binarySearch(int arr[], int left, int right, int x) {
    while (left <= right) {
        int mid = left + (right - left) / 2;

        if (arr[mid] == x)
            return mid;

        if (arr[mid] < x)
            left = mid + 1;

        else
            right = mid - 1;
    }

    return -1;
}

int main() 
{
        cout<<"\t\t-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*"<<endl;
	cout << "\t\t\tWELCOME TO BINARY SEARCH PROCCESS!"<<endl;
	cout<<"\t\t-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*"<<endl;
{
 int *shared_memory;
int shmid;
shmid=shmget((key_t)1122,sizeof(int),0666);

shared_memory=(int*)shmat(shmid,NULL,0);
int new_data =*shared_memory-40 ;
*shared_memory=new_data;

if (shmdt(shared_memory) == -1) {
        perror("shmdt error");
        exit(1);
    }
    }


     int x,choice=0;
    int arr[] = {2, 5, 8, 12, 16, 23, 38, 56, 72, 91};
    int n = sizeof(arr) / sizeof(arr[0]);
    cout<<"Press 1 to find number in array"<<endl;
    cout<<"press 0 to exit"<<endl;
    cin>>choice;
    if(choice!=0 && choice!=1)
    {
     {
      cout<<"Wrong input"<<endl;
      cout<<"Proccess closed"<<endl;
      int *shared_memory;
      int shmid;
      shmid=shmget((key_t)1122,sizeof(int),0666);
      shared_memory=(int*)shmat(shmid,NULL,0);
      int new_data =*shared_memory+40 ;
      *shared_memory=new_data;
      if (shmdt(shared_memory) == -1) 
      {
        perror("shmdt error");
        exit(1);
      }
       return 0;
     }
      
    }
while(choice!=0)
{
    cout<<"Enter number to find int the array:  ";
    cin>>x;
    int result = binarySearch(arr, 0, n - 1, x);

    if (result == -1)
        cout << "Element not found"<<endl;
    else
    {
        cout << "Element found at index " << result<<endl;
    }
    cout<<"Press 1 to find number in array"<<endl;
    cout<<"press 0 to exit"<<endl;
    cin>>choice;
     if(choice!=0 && choice!=1)
    {
      cout<<"Wrong input"<<endl;
      cout<<"Proccess closed"<<endl;
      {
      int *shared_memory;
      int shmid;
      shmid=shmget((key_t)1122,sizeof(int),0666);
      shared_memory=(int*)shmat(shmid,NULL,0);
      int new_data =*shared_memory+40 ;
      *shared_memory=new_data;
      if (shmdt(shared_memory) == -1) 
      {
        perror("shmdt error");
        exit(1);
      }
    return 0;
    }
 }
 {
    int *shared_memory;
      int shmid;
      shmid=shmget((key_t)1122,sizeof(int),0666);
      shared_memory=(int*)shmat(shmid,NULL,0);
      int new_data =*shared_memory+40 ;
      *shared_memory=new_data;
      if (shmdt(shared_memory) == -1) 
      {
        perror("shmdt error");
        exit(1);
      }
  }
    return 0;
}
}

