#include <iostream>
#include <chrono>
#include <thread>
#include <atomic>
#include<unistd.h>
#include <future>
#include<sys/shm.h>
using namespace std;

class Stopwatch {
public:
    Stopwatch() : start_time(std::chrono::high_resolution_clock::now()) {}

    void reset() {
        start_time = std::chrono::high_resolution_clock::now();
    }

    double elapsed_seconds() {
        auto now = std::chrono::high_resolution_clock::now();
        auto duration = std::chrono::duration_cast<std::chrono::microseconds>(now - start_time).count();
        return duration / 1e6;
    }

private:
    std::chrono::high_resolution_clock::time_point start_time;
};

void display_elapsed_time(std::atomic<bool>& running, Stopwatch& stopwatch) {
    while (running.load()) {
        std::cout << "Elapsed time (in seconds): " << stopwatch.elapsed_seconds() << std::endl;
        std::this_thread::sleep_for(std::chrono::seconds(1));
    }
}

int main() {
        cout<<"\t\t-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*"<<endl;
	cout << "\t\t\tWELCOME TO STOP WATCH PROCCESS!"<<endl;
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

    Stopwatch stopwatch;
    std::atomic<bool> running(true);

    std::cout << "Stopwatch started. Press Enter to stop." << std::endl;

    // Display the elapsed time every second in a separate thread
    std::thread display_thread(display_elapsed_time, std::ref(running), std::ref(stopwatch));

    // Wait for the user to press Enter
    std::cin.get();

    running.store(false);
    display_thread.join();

    std::cout << "Stopwatch stopped. Final elapsed time (in seconds): " << stopwatch.elapsed_seconds() << std::endl;
        sleep(2);
    
    {
    int *shared_memory;
int shmid;
shmid=shmget((key_t)1122,sizeof(int),0666);

shared_memory=(int*)shmat(shmid,NULL,0);
int new_data =*shared_memory+11;
*shared_memory=new_data;

if (shmdt(shared_memory) == -1) {
        perror("shmdt error");
        exit(1);
    }
    }

    return 0;
}

