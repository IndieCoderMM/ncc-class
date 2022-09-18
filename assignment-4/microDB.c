// Assignment - 4
// Hein Thant
// 9-18-2022

#include "stdio.h"
#include "stdlib.h"
#include "string.h"
#include "conio.h"

#define MAX_USERS 100
#define IDCODE 111 // Base number for user id

typedef struct User
{
    int id;
    char *name;
    char *password;
    char *city;
    int age;
    int balance;

} User;

void initDatabase(FILE *fptr, User users[], int *uid);
void loadUserData(User *user, char *strData);
void createNewUser(User *user, int uid);
int getUserByName(User users[], char *searchName, int userCount);
int getUserById(User users[], int searchId, int userCount);
void drawUserRow(User user);
void listUserData(User user);
void showUserDetail(User user);
void allocateMem(User *user);
void freeMem(User *user);

int main()
{
    FILE *fptr;
    User users[MAX_USERS];
    int uid = 0;
    int newId = 0;
    char cmd;
    char charIn;
    int searchId = 0;
    char searchName[30];
    int running = 1;

    // Load existing users' data
    fptr = fopen("user_data.txt", "r");
    if (fptr == NULL)
    {
        printf("[i]::Creating new file for database...");
    }
    else
    {
        initDatabase(fptr, users, &uid);
        fclose(fptr);
    }

    while (running == 1)
    {
        printf("\n\n{CMD}::[A]dd New User; [S]earch User; [L]ist All Users; [U]pdate User; [D]elete User; [Q]uit & Save;");
        printf("\nSelect Command: ");
        scanf(" %c", &cmd);
        switch (cmd)
        {
        case 'a':
        case 'A':
            // Get New User
            printf("\n// CREATE NEW ACCOUNT //");
            if (uid == 0)
                newId = IDCODE;
            else
                newId = users[uid - 1].id + 1;
            allocateMem(&users[uid]);
            createNewUser(&users[uid], newId);
            printf("\n[i]::Creating New User Account...");
            printf("\n[OK]::Created {%s's account} successfully!", users[uid].name);

            uid++;
            break;
        case 's':
        case 'S':
            printf("\n// SEARCH USER ACCOUNT //");
            printf("\n{CMD}::[I]d number; [N]ame;");
            printf("\nSelect search method: ");
            scanf(" %c", &charIn);
            if (charIn == 'n' || charIn == 'N')
            {
                printf("\nEnter Username: ");
                while (getchar() != '\n')
                {
                    // Clearing input text
                };
                gets(searchName);
                searchId = getUserByName(users, searchName, uid);
            }
            else
            {
                printf("\nEnter User ID: ");
                scanf("%d", &searchId); // 101
                searchId = getUserById(users, searchId, uid);
            }

            if (searchId < 0)
            {
                printf("\n[ERR0R]::USER NOT FOUND!");
                break;
            }
            showUserDetail(users[searchId]);
            break;
        case 'l':
        case 'L':
            printf("\n// All Accounts //");
            for (int i = 0; i < uid; i++)
            {
                listUserData(users[i]);
            }
            break;
        case 'u':
        case 'U':
            printf("\n// UPDATE USER DATA //");
            printf("\nEnter User ID: ");
            scanf("%d", &searchId); // 101
            searchId = getUserById(users, searchId, uid);
            if (searchId < 0)
            {
                printf("\n[ERR0R]::INVALID USER ID!");
                break;
            }
            showUserDetail(users[searchId]);
            allocateMem(&users[searchId]);
            createNewUser(&users[searchId], IDCODE + searchId);
            printf("\n[OK]::Updated {%s's account} successfully!", users[searchId].name);
            break;
        case 'd':
        case 'D':
            printf("\n// DELETE USER ACCOUNT //");
            printf("\nEnter User ID: ");
            scanf("%d", &searchId);                       // 101
            searchId = getUserById(users, searchId, uid); // get index
            if (searchId < 0)
            {
                printf("\n[ERR0R]::INVALID USER ID!");
                break;
            }
            showUserDetail(users[searchId]);
            printf("\nDo you want to delete this account (y/N)? ");
            scanf(" %c", &charIn);
            if (charIn == 'y' || charIn == 'Y')
            {
                users[searchId].id *= -1;
                printf("\n[OK]::Deleted {%s's account} successfully!", users[searchId].name);
            }
            else
                printf("\n[i]::Cancelled delete process!");
            break;
        case 'q':
        case 'Q':
        default:
            printf("\n>> Do you want to exit (y/N)? ");
            scanf(" %c", &charIn);
            if (charIn == 'y' || charIn == 'Y')
            {
                running = 0;
                printf("\n[i]::Saving the file...");
                fptr = fopen("user_data.txt", "w");
                for (int i = 0; i < uid; i++)
                {
                    if (users[i].id > 0)
                    {
                        fprintf(fptr, "%d,%s,%s,%s,%d,%d\n", users[i].id, users[i].name, users[i].password, users[i].city, users[i].age, users[i].balance);
                    }

                    freeMem(&users[i]);
                }
                printf("\n[OK]::File saved successfully!");
                fclose(fptr);
            }
        }
    }
    getch();
    return 0;
}

void initDatabase(FILE *fptr, User users[], int *uid)
{
    char strData[200];
    printf("\n+------+---------------------+---------------------+----------------+-----+------------+");
    printf("\n|  ID  |       Username      |       Password      |     Address    | Age |   Balance  |");
    printf("\n+------+---------------------+---------------------+----------------+-----+------------+");
    while (fgets(strData, 100, fptr) != NULL)
    {
        if (strlen(strData) < 10)
            continue;
        allocateMem(&users[*uid]);
        loadUserData(&users[*uid], strData);
        drawUserRow(users[*uid]);
        *uid += 1;
    }
    printf("\n+------+---------------------+---------------------+----------------+-----+------------+");
}

void loadUserData(User *user, char *strData)
{
    int index = 0;
    const char *data[10];
    char *word = strtok(strData, ",");
    while (word != NULL)
    {
        data[index] = (const char *)malloc(strlen(word) + 1);
        data[index] = word;
        word = strtok(NULL, ",");
        index++;
    }
    if (index > 5)
    {
        user->id = atoi(data[0]);
        strcpy(user->name, data[1]);
        strcpy(user->password, data[2]);
        strcpy(user->city, data[3]);
        user->age = atoi(data[4]);
        user->balance = atoi(data[5]);
    }
}

int getUserById(User users[], int searchId, int userCount)
{
    for (int i = 0; i < userCount; i++)
    {
        if (users[i].id == searchId)
            return i;
    }
    return -1;
}

int getUserByName(User users[], char *searchName, int userCount)
{
    for (int i = 0; i < userCount; i++)
    {
        if (strcmp(users[i].name, searchName) == 0)
            return i;
    }
    return -1;
}

void drawUserRow(User user)
{
    printf("\n| %4d |", user.id);
    printf("%20s |", user.name);
    // printf("%20s |", user.password);
    int spaces = 20 - strlen(user.password);
    for (int i = 0; i < spaces; i++)
        printf(" ");
    for (int j = 0; j < strlen(user.password); j++)
        printf("*");
    printf(" |");
    printf("%15s |", user.city);
    printf("%4d |", user.age);
    printf("$ ######### |");
}

void listUserData(User user)
{
    if (user.id > 0)
    {
        printf("\n>> [%d] ", user.id);
        printf("%s ", user.name);
        for (int i = 0; i < 20 - strlen(user.name); i++)
            printf(".");
        printf("(%2d) ", user.age);
        printf("@ %s", user.city);
    }
}

void showUserDetail(User user)
{
    printf("\n+------------------------------+");
    printf("\n|         Account Detail       |");
    printf("\n|  User ID : %3d", user.id);
    for (int i = 0; i < 15; i++)
        printf(" ");
    printf("|");
    printf("\n|  Username: %s", user.name);
    for (int i = 0; i < 18 - strlen(user.name); i++)
        printf(" ");
    printf("|");

    printf("\n|  Password: %s", user.password);
    for (int i = 0; i < 18 - strlen(user.password); i++)
        printf(" ");
    printf("|");
    printf("\n|  Address : %s", user.city);
    for (int i = 0; i < 18 - strlen(user.city); i++)
        printf(" ");
    printf("|");
    printf("\n|  Age     : %3d", user.age);
    for (int i = 0; i < 15; i++)
        printf(" ");
    printf("|");
    printf("\n|  Balance : %8d $", user.balance);
    for (int i = 0; i < 8; i++)
        printf(" ");
    printf("|");
    printf("\n+------------------------------+");
}

void createNewUser(User *user, int id)
{
    user->id = id;

    while (getchar() != '\n')
    {
        // Clearing input text
    };
    printf("\nEnter Username: ");
    gets(user->name);
    printf("\nEnter Password: ");
    gets(user->password);
    printf("\nEnter City: ");
    gets(user->city);

    do
    {
        printf("\nEnter Age: ");
        scanf("%d", &user->age);
        if (user->age <= 0)
            printf("[ERROR]::INVALID AGE!");
    } while (user->age <= 0);
    do
    {
        printf("\nEnter Balance: ");
        scanf("%d", &user->balance);
        if (user->balance < 0)
            printf("[ERROR]::INVALID BALANCE!");
    } while (user->balance < 0);
}

void allocateMem(User *user)
{
    user->name = (char *)malloc(sizeof(char) * 20);
    user->password = (char *)malloc(sizeof(char) * 20);
    user->city = (char *)malloc(sizeof(char) * 40);
}

void freeMem(User *user)
{
    free(user->name);
    free(user->password);
    free(user->city);
}