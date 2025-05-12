#include <iostream>
#include <string>
#include <iomanip>
#include <sys/shm.h>
#include <vector>
using namespace std;

const int MAX_TRANSACTIONS = 100;

struct Transaction {
    string type;
    double amount;
    string timestamp;
};

struct Account {
    string name;
    double balance;
    int transaction_count;
    Transaction transactions[MAX_TRANSACTIONS];
};

vector<Account> accounts;

void initializeAccounts() {
    accounts.push_back({"usman", 1000, 0});
    accounts.push_back({"nalain", 500, 0});
    accounts.push_back({"abdullah", 500, 0});
    accounts.push_back({"alishba", 500, 0});
}

void displayAccounts() {
    cout << "\nAvailable Accounts:" << endl;
    for (size_t i = 0; i < accounts.size(); i++) {
        cout << i + 1 << ". " << accounts[i].name << " (Balance: $" << accounts[i].balance << ")" << endl;
    }
}

void displayMenu() {
    cout << "\n1. Check balance" << endl;
    cout << "2. Deposit" << endl;
    cout << "3. Withdraw" << endl;
    cout << "4. Send money" << endl;
    cout << "5. Transaction history" << endl;
    cout << "6. Exit" << endl;
    cout << "Enter your choice: ";
}

double deposit(Account &account, double amount) {
    if (amount <= 0) {
        cout << "Invalid amount! Amount must be positive." << endl;
        return account.balance;
    }
    account.balance += amount;
    Transaction transaction = {"Deposit", amount, "Now"};
    account.transactions[account.transaction_count++] = transaction;
    cout << "Successfully deposited $" << amount << endl;
    return account.balance;
}

double withdraw(Account &account, double amount) {
    if (amount <= 0) {
        cout << "Invalid amount! Amount must be positive." << endl;
        return account.balance;
    }
    if (account.balance >= amount) {
        account.balance -= amount;
        Transaction transaction = {"Withdraw", amount, "Now"};
        account.transactions[account.transaction_count++] = transaction;
        cout << "Successfully withdrew $" << amount << endl;
    } else {
        cout << "Insufficient balance!" << endl;
    }
    return account.balance;
}

void sendMoney(Account &sender, Account &receiver, double amount) {
    if (amount <= 0) {
        cout << "Invalid amount! Amount must be positive." << endl;
        return;
    }
    if (sender.balance >= amount) {
        sender.balance -= amount;
        receiver.balance += amount;
        Transaction sender_transaction = {"Send", amount, "Now"};
        Transaction receiver_transaction = {"Receive", amount, "Now"};
        sender.transactions[sender.transaction_count++] = sender_transaction;
        receiver.transactions[receiver.transaction_count++] = receiver_transaction;
        cout << "Successfully sent $" << amount << " to " << receiver.name << endl;
    } else {
        cout << "Insufficient balance!" << endl;
    }
}

void printTransactionHistory(Account &account) {
    cout << "\nTransaction history for " << account.name << ":" << endl;
    cout << setw(15) << left << "Type" 
         << setw(15) << right << "Amount" 
         << setw(15) << right << "Time" << endl;
    cout << string(45, '-') << endl;
    
    for (int i = 0; i < account.transaction_count; i++) {
        cout << setw(15) << left << account.transactions[i].type
             << setw(15) << right << account.transactions[i].amount
             << setw(15) << right << account.transactions[i].timestamp << endl;
    }
}

int main() 
{
    cout << "\t\t-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*" << endl;
    cout << "\t\t\tWELCOME TO BANKING SYSTEM!" << endl;
    cout << "\t\t-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*" << endl;

    // Initialize shared memory
    int *shared_memory;
    int shmid = shmget((key_t)1122, sizeof(int), 0666);
    if (shmid == -1) {
        perror("shmget failed");
        return 1;
    }

    shared_memory = (int*)shmat(shmid, NULL, 0);
    if (shared_memory == (void*)-1) {
        perror("shmat failed");
        return 1;
    }

    int new_data = *shared_memory - 2;
    *shared_memory = new_data;

    if (shmdt(shared_memory) == -1) {
        perror("shmdt error");
        return 1;
    }

    initializeAccounts();
    int choice;
    double amount;
    int accountIndex;

    do {
        displayAccounts();
        cout << "\nSelect account number: ";
        cin >> accountIndex;
        
        if (accountIndex < 1 || accountIndex > static_cast<int>(accounts.size())) {
            cout << "Invalid account selection!" << endl;
            continue;
        }
        
        Account &currentAccount = accounts[accountIndex - 1];
        displayMenu();
        cin >> choice;

        switch (choice) {
            case 1:
                cout << "Current balance: $" << currentAccount.balance << endl;
                break;
            case 2:
                cout << "Enter deposit amount: $";
                cin >> amount;
                deposit(currentAccount, amount);
                break;
            case 3:
                cout << "Enter withdrawal amount: $";
                cin >> amount;
                withdraw(currentAccount, amount);
                break;
            case 4: {
                cout << "Select recipient account number: ";
                int recipientIndex;
                cin >> recipientIndex;
                if (recipientIndex < 1 || recipientIndex > static_cast<int>(accounts.size())) {
                    cout << "Invalid recipient account!" << endl;
                    break;
                }
                cout << "Enter amount to send: $";
                cin >> amount;
                sendMoney(currentAccount, accounts[recipientIndex - 1], amount);
                break;
            }
            case 5:
                printTransactionHistory(currentAccount);
                break;
            case 6: {
                int *shared_memory;
                int shmid = shmget((key_t)1122, sizeof(int), 0666);
                if (shmid == -1) {
                    perror("shmget failed");
                    return 1;
                }
                shared_memory = (int*)shmat(shmid, NULL, 0);
                if (shared_memory == (void*)-1) {
                    perror("shmat failed");
                    return 1;
                }
                int new_data = *shared_memory + 2;
                *shared_memory = new_data;
                if (shmdt(shared_memory) == -1) {
                    perror("shmdt error");
                    return 1;
                }
                cout << "Thank you for using our banking system!" << endl;
                return 0;
            }
            default:
                cout << "Invalid choice!" << endl;
        }
    } while (choice != 6);

    return 0;
}

