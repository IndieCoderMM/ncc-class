// Shopping Cart Program
// Exam-3
// Hein Thant
// Sep-4-2022


#include <stdio.h>
#define SIZE 5

void displayProducts(char* item, int* price);
void displayWallet(char name[], char password[], int balance);
int displayCart(char cart[], char PRODUCTS[], int PRICES[]);
void printInfo(char label[], char info[], int width);
void printLine(char symbol, int width) ;
void printRow(int width, char a[], char b[], char c[]);
int printFruit(char fruit);
char deleteItem(char* pCart, int index);
void addItems(char* pcart, char* newItems);
int getPassword(char password[]);
int getStrLen(char string[]);
int getNumLen(int num);
char* intToStr(int num);


int main() {
	char PRODUCTS[SIZE+1] = {'o', 'a', 'p', 'm', 'b', '\0'};
	int PRICES[SIZE] = {200, 200, 300, 150, 100};

	char name[20];
	char password[20];
	int balance = 0;
	char cart[100];
	char command;
	int totalCost = 0;
	int getCmd = 1;
	int login = 0;
	int inumToDel;
	char yesNo;
	char newItems[100];

	printf("Shopping Cart Program");
	printf("\nAuthor: hthant | Lang: C | Date: Sep 4, 2022");

	printf("\n+------------------------------+");
	printf("\n|     ONLINE FRUITS STORE      |");
	printf("\n+------------------------------+");
	printf("\n[#] Welcome To Our Online Fruit Store.");
	printf("\n[i] Please Enter Your Wallet Card Information.\n");

	printf("\n>>Enter Your Name: ");
	scanf("%s", &name);
	printf("\n>>Enter Your Wallet Password: ");
	scanf("%s", &password);
	while (getStrLen(password) < 6) {
		printf("\n[!] Weak Password.");
		printf("\n[i] Password should contain at least 6 characters.");
		printf("\n>>Enter Your Wallet Password: ");
		scanf("%s", &password);
	}
	printf("\n>>Enter Your Balance: ");
	scanf("%d", &balance);
	while (balance <= 0) {
		printf("\n[!] Invalid Balance.");
		printf("\n>>Please Enter Correct Balance: ");
		scanf("%d", &balance);
	}
	displayWallet(name, password, balance);

	printf("\n*Available Fresh & Delicious Fruits*");
	displayProducts(PRODUCTS, PRICES);
	printf("\n[i] Use CODE to add into Shopping Cart.");
	printf("\n[i] e.g., aaabbmo");
	printf("\n>>Enter Your Shopping List: ");
	scanf("%s", &cart);
	
	do {
		totalCost = displayCart(cart, PRODUCTS, PRICES);
		if (totalCost == 0) printf("\n[i] Please add some items into cart.");
		printf("\n<< [R]emove [A]dd [P]roceed [C]ancel >>");
		printf("\n>>Select Next Action: ");
		scanf(" %c", &command);

		switch (command) {
			case 'r':
			case 'R':
				printf("\n>>Enter ITEM NO. to Remove: ");
				scanf("%d", &inumToDel);
				if (inumToDel > getStrLen(cart)) {
					printf("\n[!] Invalid ITEM NO. %d.", inumToDel);
				} else {
					printf("\n[!] Removed (Item %d : ", inumToDel);
					printFruit(deleteItem(&cart[0], inumToDel-1)); 
					printf(") from Shopping Cart.");
				}
				break;
			case 'a':
			case 'A':
				displayProducts(PRODUCTS, PRICES);
				printf("\n>>Enter ITEM CODES to Add: ");
				scanf("%s", &newItems);
				addItems(&cart[0], &newItems[0]);
				printf("\n[!] Added %d new items into cart.", getStrLen(newItems));
				break;
			case 'p':
			case 'P':
				printf("\n[%] Proceeding to next step...");
				printf("\n[%] Checking Your Balance...");
				if (totalCost > balance) {
					printf("\n[!] Sorry, You don't have enough money in your wallet.");
					printf("\n[i] Please remove some PRODUCTS you don't need.");
				} else {
					login = getPassword(password);
					if (login == 1) {
						printf("\n[%] Making payment...");
						printf("\n[%] %d - %d", balance, totalCost);
						balance -= totalCost;
						displayWallet(name, password, balance);
						printf("\n[!] Purchase Success.");
						displayCart(cart, PRODUCTS, PRICES);
						getCmd = 0;
					} else {
						printf("\n[%] Exiting from store...");
						getCmd = 0;
					}
				}
				break;
			default:
				printf("\n>>Do you want to exit (y/n)? ");
				scanf(" %c", &yesNo);
				switch (yesNo) {
					case 'n':
					case 'N':
						break;
					default:
						getCmd = 0;
				}
		} 
	} while (getCmd == 1);

	printf("\n[#] Thanks for visiting our store.");
	printf("\n[#] Have a nice day!");
	return 0;
}

char deleteItem(char* pCart, int index) {
	char deletedItem;
	for (int i = 0; i < index; i++) {
		// printf("\n%c at index %d", *pCart, i);
		pCart++;
	}
	deletedItem = *pCart;
	while (*pCart != '\0') {
		*pCart = *(pCart+1);
		pCart++;
	}
	return deletedItem;
}

void addItems(char* pCart, char* pNewItems){
	// Get pointer for last position
	while (*pCart != '\0') {
		pCart++;
	}
	// push new PRODUCTS
	while (*pNewItems != '\0') {
		// printf("%c ", *pNewItems);
		*pCart = *pNewItems;
		pCart++;
		pNewItems++;
	}
	*pCart = '\0';
}

int getPassword(char password[]) {
	int try = 4;
	int login = 0;
	char pwInput[100];
	while (try >= 0) {
		printf("\n>>Confirm Your Wallet Password: ");
		scanf("%s", &pwInput);
		login = 1;
		for (int i = 0; password[i] != '\0'; i++) {
			if (password[i] != pwInput[i]) login = 0;
		}
		if (login == 1) {
			printf("\n[!] Login Success.");
			return 1;
		} else {
			printf("\n[!] Wrong Password.");
			printf("\n[i] You can try (%d) more times.", try);
			try--;
		}
	}
	printf("\n[!] Login Failed.");
	return 0;
}


void displayWallet(char name[], char password[], int balance) {
	int cols = 32;
	int rows = 10;
	printf("\n");

	for (int r = 0; r < rows; r++) {
		for (int c = 0; c < cols; c++) {
			switch (r) {
				case 0:
				case 9:
					if (c == 0) {
						printf("+");
						printLine('-', cols-2);
						printf("+");
					} 
					break;
				case 1:
					if (c == 0) {
						printf("|");
						printLine(' ', (cols-16)/2);
						printf("MY WALLET CARD");
						printLine(' ', (cols-16)/2);
						printf("|");
					}
					break;
				case 2:
					if (c == 0) {
							printf("|");
							printLine(' ', (cols-16)/2);
							printLine('*', 14);
							printLine(' ', (cols-16)/2);
							printf("|");
						}
					break;
				case 3:
					if (c == 0) {
						printInfo("Name", name, cols);
					}
					break;
				case 5:
					if (c == 0) {
						printInfo("Password", password, cols);
					}
					break;
				case 7:
					if (c == 0) {
						char *pBalance = intToStr(balance);
						char balanceString[getNumLen(balance)];
						int index = 0;
						while (*pBalance != '\0') {
							balanceString[index] = *pBalance;
							pBalance++;
							index++;
						}
						balanceString[index] = '\0';
						
						printInfo("Balance", balanceString, cols);
					}
					break;
				default:
					if (c == 0 || c == cols -1) printf("|");
					else printf(" ");
			}
		}
		printf("\n");
	}
}

void displayProducts(char PRODUCTS[], int PRICES[]) {
	int width = 12;
	int len = 0;
	printf("\n+");
	printLine('-', width*2+6);
	printf("+");
	printf("\n");
	printf("| CODE |    FRUITS    | PRICE  |\n");
	printf("+");
	printLine('-', width*2+6);
	printf("+");
	printf("\n");
	for (int i = 0; PRODUCTS[i] != '\0'; i++) {
		printf("| %3c  | ", PRODUCTS[i]);
		len = printFruit(PRODUCTS[i]);
		printLine(' ', width-len);
		printf(" | $ %4d |\n", PRICES[i]);
		printf("+");
		printLine('-', width*2+6);
		printf("+");
		printf("\n");
	}
}

int displayCart(char cart[], char PRODUCTS[], int PRICES[]) {
	int totalCost = 0;
	int len = 0;
	for (int i = 0; cart[i] != '\0'; i++) {
		for (int j = 0; j < SIZE; j++) {
			if (PRODUCTS[j] == cart[i]) {
				printf("\nItem %2d - ", i+1);
				len = printFruit(PRODUCTS[j]);
				printLine('-', 12-len);
				printf("-- %5d", PRICES[j]);
				totalCost += PRICES[j];
			}
		}
	}
	printf("\n");
	printLine('_', 30);
	printf("\n");
	printf("Total Cost ");
	printLine('-', 11);
	printf(" $ %5d\n", totalCost);
	return totalCost;
}

void printInfo(char label[], char info[], int width) {
	int labelLen = getStrLen(label);
	int infoLen = getStrLen(info);
	int spaceLen = width - labelLen - infoLen - 6;
	printf("|");
	printf(" %s : ", label);
	printf("%s", info);

	for (int i = 0; i < spaceLen; i++) {
		printf(" ");
	}
	printf("|");
}

void printLine(char symbol, int length) {
	for (int i = 0; i < length; i++) {printf("%c", symbol);}
}

int printFruit(char fruit) {
	int len = 0;
	switch (fruit) {
		case 'o':
			len = getStrLen("Orange");
			printf("Orange");
			break;
		case 'a':
			len = getStrLen("Apple");
			printf("Apple");
			break;
		case 'm':
			len = getStrLen("Mango");
			printf("Mango");
			break;
		case 'b':
			len = getStrLen("Banana");
			printf("Banana");
			break;
		case 'p':
			len = getStrLen("Pineapple");
			printf("Pineapple");
			break;
		}
	return len;
}

void printRow(int width, char item1[], char item2[], char item3[]) {
	for (int c = 0; c < 3; c++) {
		printf("|");
		int len = 0;
		if (c == 0) {
			len = getStrLen(item1);
			printf("  %s", item1);
		} else if (c == 1) {
			len = getStrLen(item2);
			printf("  %s", item2);
		} else {
			len = getStrLen(item3);
			printf("  %s", item3);
		}
		for (int k = 0; k < width-len-4; k++) {
			printf(" ");
		}
		printf("|");
	}
	printf("\n");
}

char* intToStr(int num) {
	char digits[] = {'0', '1', '2', '3', '4', '5', '6', '7', '8', '9', '\0'};
	int length = getNumLen(num);
	int intArray[length];
	char numString[length+1];
	numString[length] = '\0';
	char *pNumString = &numString[0];
	int *ptr = &intArray[0];
	// push last digits to array
	while (num > 0) {
		*ptr = num % 10;
		ptr++;
		num /= 10;
	}
	// convert int to char
	for (int i = 0; i < length; i++) {
		int numIndex = intArray[length-1-i];
		numString[i] = digits[numIndex];
		// printf("%c", numString[i]);
	}
	return pNumString;
}

int getStrLen(char string[]) {
	int length = 0;
	for (int i = 0; string[i] != '\0'; i++) {length++;}
	return length;
}

int getNumLen(int num) {
	int length = 0;
	do {
		num = num / 10;
		length++;
	} while (num > 0);
	return length;
}