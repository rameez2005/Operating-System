#include <iostream>
#include <string>
#include <vector>
#include <cstdlib>
#include <ctime>
#include<sys/shm.h>
using namespace std;

const int MAX_WRONG = 8; // maximum number of incorrect guesses allowed

vector<string> words; // collection of possible words for the game
string current_word; // word to be guessed
string so_far; // word guessed so far
int wrong; // number of incorrect guesses so far

// function prototypes
void init_words();
void play();

// initialize the collection of possible words for the game
void init_words() {
    words.push_back("HELLO");
    words.push_back("WORLD");
    words.push_back("HANGMAN");
    words.push_back("COMPUTER");
    words.push_back("SCIENCE");
    words.push_back("PROGRAMMING");
    words.push_back("CPLUSPLUS");
}

// play a game of Hangman
void play() {
    // select a word randomly from the collection of possible words
    current_word = words[rand() % words.size()];
    so_far = string(current_word.length(), '-');
    wrong = 0;
    // play the game until the word is guessed or the maximum number of incorrect guesses is reached
    while (wrong < MAX_WRONG && so_far != current_word) {
        cout << "\n\nYou have " << MAX_WRONG - wrong << " incorrect guesses left.\n";
        cout << "\nYou've guessed the following letters:\n" << so_far << endl;
        char guess;
        cout << "\n\nEnter your guess: ";
        cin >> guess;
        guess = toupper(guess); // convert to uppercase
        if (current_word.find(guess) != string::npos) {
            cout << "That's right! " << guess << " is in the word.\n";
            // update the word guessed so far
            for (int i = 0; i < current_word.length(); ++i) {
                if (current_word[i] == guess) {
                    so_far[i] = guess;
                }
            }
        } else {
            cout << "Sorry, " << guess << " is not in the word.\n";
            ++wrong;
        }
    }
    // check whether the word was guessed or not
    if (so_far == current_word) {
        cout << "\nCongratulations! You guessed the word.\n";
    } else {
        cout << "\nSorry, you've run out of guesses. The word was " << current_word << ".\n";
    }
}
int main() 
{
        cout<<"\t\t-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*"<<endl;
	cout << "\t\t\tWELCOME TO HANGMAN GAME WORLD!"<<endl;
	cout<<"\t\t-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*"<<endl;
int *shared_memory;
int shmid;
shmid=shmget((key_t)1122,sizeof(int),0666);

shared_memory=(int*)shmat(shmid,NULL,0);
int new_data =*shared_memory-15 ;
*shared_memory=new_data;

if (shmdt(shared_memory) == -1) 
{
        perror("shmdt error");
        exit(1);
}
    srand(static_cast<unsigned int>(time(0))); // seed random number generator
    init_words();
    char play_again = 'y';
    while (play_again == 'y') {
        play();
        cout << "Do you want to play again? (y/n): ";
        cin >> play_again;
    }
    	{
	int *shared_memory;
int shmid;
shmid=shmget((key_t)1122,sizeof(int),0666);

shared_memory=(int*)shmat(shmid,NULL,0);
int new_data =*shared_memory+15 ;
*shared_memory=new_data;

if (shmdt(shared_memory) == -1) {
        perror("shmdt error");
        exit(1);
        cout<<"Game closed"<<endl;
    }
    }
    return 0;
}
