//
// Assignment-5
// MicroDB - OOP
// Simple database system
// Hein Thant 9-25-2022
//

#include <iostream>
#include <conio.h>
#include <fstream>
#include <vector>
#include <time.h>

using namespace std;

#define MAX_USERS 100
#define ATTRIBUTES 6

class User
{
private:
    int id = 0;
    int balance = 0;
    bool admin = false;

public:
    string name;
    string password;
    string email;
    string city;

    void displayInfo()
    {
        cout << "\tID : " << id << endl;
        cout << "\tUsername : " << name << endl;
        cout << "\tEmail : " << email << endl;
        cout << "\tPassword : ";
        for (int i = 0; i < password.length(); i++)
            cout << "*";
        cout << "\n\tAddress : " << city << endl;
        cout << "\tBalance : " << balance << endl;
    }

    void makeAdmin()
    {
        admin = true;
    }

    bool isAdmin()
    {
        return admin;
    }

    void setId(int n)
    {
        id = n;
    }

    int getId()
    {
        return id;
    }

    void addBalance(int amount)
    {
        balance += amount;
    }

    bool cashOut(int amount)
    {
        if (amount > balance)
            return false;

        balance -= amount;
        return true;
    }

    int getBalance()
    {
        return balance;
    }
};

void getUserInfo(User *user);
void createNewUser(User *user, bool admin);
int loadUserData(User *user, string line);
bool isValidEmail(string text);
int generateRandNum(int min, int max);
int searchUserByName(User users[], string name, int totalUser);
int searchUserById(User users[], int id, int totalUser);
bool transferBalance(User *from_user, User *to_user, int amount);
void listAllUsers(User users[], int totalUser);
void checkPassword(User user);

int main()
{
    User users[MAX_USERS];
    string fileName = "user_data.txt";
    int uCount = 0;
    int uIndex = -1;
    bool running = true;
    int amount = 0;
    string inputStr;
    int tempInt;
    char cmd;
    srand(time(NULL));
    cout << "+---------------------+" << endl;
    cout << "| Welcome to Micro DB |" << endl;
    cout << "+---------------------+" << endl;
    cout << "After sign up or log in, you can add/withdraw balance, transfer money or update profile." << endl;
    cout << "Admin account is necessary to access into database." << endl;

    ifstream DataFile(fileName);
    if (DataFile.is_open())
    {
        // Load data into users array
        do
        {
            string line;
            getline(DataFile, line);
            // cout << line << endl;
            uCount += loadUserData(&users[uCount], line);
        } while (!DataFile.eof());
        DataFile.close();
    }
    if (uCount > 0)
        listAllUsers(users, uCount);
    cout << "Total Users: " << uCount << endl;

    bool logSuccess = false;
    // ---------------------
    // Log In or Sign Up
    // ---------------------
    do
    {
        cout << "<CMD/> [S]ign Up | [L]og in >> ";
        cin >> cmd;
        switch (cmd)
        {
        case 's':
        case 'S':
            // Create New User
            cout << "// Create New Account" << endl;
            cout << "Do you want to create Admin account (y/N)? ";
            cin >> cmd;
            if (cmd == 'y' || cmd == 'Y')
                createNewUser(&users[uCount], true);
            else
                createNewUser(&users[uCount], false);
            uIndex = uCount; // Remember current user
            uCount++;
            logSuccess = true;
            break;
        case 'l':
        case 'L':
            // Search by name and check password
            cout << "// Log In to Existing Account" << endl;
            cout << "Enter your username >> ";
            cin >> ws;
            getline(cin, inputStr);
            uIndex = searchUserByName(users, inputStr, uCount);
            if (uIndex != -1)
            {
                cout << "Username: " << users[uIndex].name << endl;
                checkPassword(users[uIndex]);
                logSuccess = true;
            }
            else
            {
                cout << "[not_found]: Please enter correct name or create new account!" << endl;
            }
            break;
        default:
            cout << "[invalid_command]: Please use the correct keyword!" << endl;
        }
    } while (!logSuccess);
    // -------------------------
    // Continue as logged User
    // -------------------------
    cout << "[i]: Logged in as " << users[uIndex].name << endl;
    while (running)
    {
        cout << "________________________________________" << endl;
        cout << "              PROFILE                   " << endl;
        cout << "----------------------------------------" << endl;
        users[uIndex].displayInfo();
        cout << "________________________________________" << endl;
        cout << "<CMD/> [A]dd balance | [T]ransfer balance   | [C]ash out" << endl;
        cout << "<CMD/> [U]pdate info | [L]ist all accounts* | [Q]uit & save" << endl;
        cout << "Enter command >> ";
        cin >> cmd;
        switch (cmd)
        {
        case 'a':
        case 'A':
            cout << "// Add Balance into Account" << endl;
            cout << "Enter amount to add >> ";
            cin >> amount;
            users[uIndex].addBalance(amount);
            cout << "[success]: Added $" << amount << " into balance." << endl;
            cout << "Press any key to continue..." << endl;
            getch();
            break;
        case 'c':
        case 'C':
            cout << "// Withdraw Cash from Account" << endl;
            cout << "Enter amount to withdraw >> ";
            cin >> amount;
            checkPassword(users[uIndex]);
            if (users[uIndex].cashOut(amount))
                cout << "[success]: Withdrew $" << amount << " from balance." << endl;
            else
                cout << "[low_balance]: Amount is greater than current balance!" << endl;
            cout << "Press any key to continue..." << endl;
            getch();
            break;
        case 't':
        case 'T':
            cout << "// Balance Transfer" << endl;
            do
            {
                cout << "Enter receiver's ID >> ";
                cin >> tempInt;
                tempInt = searchUserById(users, tempInt, uCount);
                if (tempInt == -1)
                    cout << "[not_found]: Please enter the correct ID!" << endl;
                if (tempInt == uIndex)
                    cout << "[error]: Cannot transfer into your own account!" << endl;
            } while (tempInt == -1 || tempInt == uIndex);

            cout << "Receiver : " << users[tempInt].name << endl;
            cout << "Enter amount to transfer >> ";
            cin >> amount;
            checkPassword(users[uIndex]);
            transferBalance(&users[uIndex], &users[tempInt], amount);
            cout << "Press any key to continue..." << endl;
            getch();
            break;
        case 'l':
        case 'L':
            if (!users[uIndex].isAdmin())
            {
                cout << "[denied]: Admin account is necessary to get access!" << endl;
            }
            else
            {
                cout << "// Accounts on Database" << endl;
                listAllUsers(users, uCount);
            }

            cout << "Press any key to continue..." << endl;
            getch();
            break;
        case 'u':
        case 'U':
            cout << "// Update User Profile" << endl;
            getUserInfo(&users[uIndex]);
            break;
        case 'q':
        case 'Q':
        default:
            cout << "Do you want to close the app (y/N)? ";
            cin >> cmd;
            if (cmd == 'y' || cmd == 'Y')
                running = false;
            break;
        }
    }
    // Save data to file

    ofstream File(fileName);
    for (int i = 0; i < uCount; i++)
    {
        File << users[i].getId() << "," << users[i].name << "," << users[i].email << "," << users[i].password << "," << users[i].city << "," << users[i].getBalance() << ",";
        if (users[i].isAdmin())
            File << "*," << endl;
        else
            File << "-," << endl;
    }
    File.close();
    cout << "[i]: Saved users' data into file." << endl;
    cout << "Press any key to close..." << endl;
    getch();
    return 0;
}

int loadUserData(User *user, string line)
{
    vector<string> info;
    string token;
    int pos;
    string delim = ",";

    while ((pos = line.find(delim)) != string::npos)
    {
        token = line.substr(0, pos);
        info.push_back(token);
        line.erase(0, pos + delim.length());
    }
    if (info.size() < 6)
        return 0;

    user->setId(stoi(info[0]));
    user->name = info[1];
    user->email = info[2];
    user->password = info[3];
    user->city = info[4];
    user->addBalance(stoi(info[5]));
    if (info[6] == "*")
        user->makeAdmin();
    return 1;
}

void getUserInfo(User *user)
{
    cout << "Enter Username >> ";
    cin >> ws; // clear cin
    getline(cin, user->name);
    do
    {
        cout << "Enter Email >> ";
        getline(cin, user->email);
        if (!isValidEmail(user->email))
        {
            cout << "[invalid_email]: Email must end with *@gmail.com!" << endl;
        }
    } while (!isValidEmail(user->email));

    do
    {
        cout << "Enter Password >> ";
        getline(cin, user->password);
        if (user->password.length() < 6)
        {
            cout << "[weak_password]: Password must be at least 6 characters!" << endl;
        }
    } while (user->password.length() < 6);

    cout << "Enter City >> ";
    getline(cin, user->city);
}

void createNewUser(User *user, bool admin)
{
    getUserInfo(user);
    // Setting ID and balance
    int newId = generateRandNum(1000, 9999);
    user->setId(newId);

    if (admin)
        user->makeAdmin();

    cout << "[success]: Created new user => " << user->name << " #" << user->getId() << endl;
}

void listAllUsers(User users[], int totalUser)
{
    cout << "+--------+------------------------------+----------------+" << endl;
    cout << "|   ID   |          Name                |     Balance    |" << endl;
    cout << "+--------+------------------------------+----------------+" << endl;
    for (int i = 0; i < totalUser; i++)
    {
        cout << "|  " << users[i].getId() << "  |  ";
        cout << users[i].name;
        if (users[i].isAdmin())
            cout << " * ";
        else
            cout << "   ";
        for (int k = 0; k < 25 - users[i].name.length(); k++)
            cout << " ";
        cout << "| $  " << users[i].getBalance();
        for (int k = 0; k < 12 - to_string(users[i].getBalance()).length(); k++)
            cout << " ";
        cout << "|" << endl;
    }
    cout << "+--------+------------------------------+----------------+" << endl;
    cout << "(==Users with * are admin accounts==)" << endl;
}

int generateRandNum(int min, int max)
{
    return rand() % (max - min + 1) + min;
}

bool isValidEmail(string text)
{
    string suffix = "@gmail.com";
    return text.find(suffix) != string::npos;
}

void checkPassword(User user)
{
    string inputStr;
    do
    {
        cout << "Enter your password >> ";
        cin >> ws;
        getline(cin, inputStr);
        if (user.password != inputStr)
        {
            cout << "[error]: Wrong password!" << endl;
        }
    } while (user.password != inputStr);
}

int searchUserByName(User users[], string name, int totalUser)
{
    for (int i = 0; i < totalUser; i++)
    {
        if (users[i].name == name)
            return i;
    }
    return -1;
}

int searchUserById(User users[], int id, int totalUser)
{
    for (int i = 0; i < totalUser; i++)
    {
        if (users[i].getId() == id)
            return i;
    }
    return -1;
}

bool transferBalance(User *from_user, User *to_user, int amount)
{
    if (amount > from_user->getBalance())
    {
        cout << "[low_balance]: Balance is insufficient to make transfer." << endl;
        return false;
    }

    from_user->addBalance(-amount);
    to_user->addBalance(amount);
    cout << "[success]: Transferred $" << amount << " to " << to_user->name << "'s account." << endl;
    return true;
}