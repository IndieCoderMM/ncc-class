// Calculator App
// Exam-1
// Hein Thant
// Aug-19-2022


#include <stdio.h>

void drawCalculator(int x, int y, int ans, char opr);
void drawButtons(char a, char b, char c, char d, int col);
void drawDisplay(int x, int y, char opr, int width);
void showAnswer(int ans, int width);
int getNumLength(int n);
int calculateOperation(int x, int y, char opr);
int calculateValue(int x, char opr);
int add(int x, int y);
int substract(int x, int y);
int multiply(int x, int y);
int divide(int x, int y);
int mod(int x, int y);
int square(int x);
int squareRoot(int x);
int cube(int x);
int power(int base, int power);


int main() {
	int firstNum = 0;
	int secNum = 0;
	int result = 0;
	int run = 1;
	char operation = '+';
	char yesNo = 'y';

	printf(" Welcome to Calculator PRO");
	printf("\n***************************");
	printf("\nAuthor::Hthant | Lang::C\n");
	printf("\nThis app can do basic maths and some calculations (square, cube, power, root).");
	printf("\n(i):: Does not support Floating Point Numbers.");
	printf("\n[!]:: Entering 2 or more characters exits the app.");

	while (run == 1) {
		drawCalculator(firstNum, secNum, result, operation);

		printf("\n Enter First Number => ");
		scanf("%d", &firstNum);
		drawCalculator(firstNum, secNum, result, operation);

		printf("\nOptions:\n[+  -  *  / % ] [(s)quare  (c)ube (p)ower sq(r)t]");
		printf("\n Select Operation => ");
		scanf(" %c", &operation);
		drawCalculator(firstNum, secNum, result, operation);

		switch (operation) {
			case 's':
			case 'c':
			case 'r':
				result = calculateValue(firstNum, operation);
				break;
			case '+':
			case '-':
			case '*':
			case '/':
			case '%':
			case 'p':
				printf("\n Enter Second Number => ");
				scanf("%d", &secNum);
				result = calculateOperation(firstNum, secNum, operation);
				break;
			case '=':
				result = firstNum;
				break;
			default:
				printf("\n[!!!]::INVALID INPUT!");
		}
		drawCalculator(firstNum, secNum, result, operation);
		printf("\n Do you want to do another calculation (y/n)? ");
		scanf(" %c", &yesNo);
		switch (yesNo) {
			case 'y':
			case 'Y':
				firstNum = 0;
				secNum = 0;
				result = 0;
				operation = '+';
				break;
			case 'n':
			default:
				printf("\nCalculator App is closing...");
				run = 0;
		}
	}
	printf("\nThanks for using Calculator PRO. BYE!");
	return 0;
}

void drawCalculator(int x, int y, int ans, char opr) {
	int width = 17;
	int height = 18;
	for (int row = 0; row < height; row++) {
		for (int col = 0; col < width; col++) {
			if (col == 0) {
				if (row == 0) {printf("\n  ");}
				else if (row == height-1) {printf(" \\");}
				else {printf(" |");}
			}

			switch (row) {
				case 2:
					if (col == 1) {drawDisplay(x, y, opr, width);}
					break;
				case 4:
					if (col == 0) {printf(" Ans: ");}
					else if (col < width - 5) {printf(" ");}
					break;
				case 5:
					if (col == 1) {showAnswer(ans, width);}
					break;
				case 7:
					drawButtons('s', 'r', 'p', 'c', col);
					break;
				case 9:
					drawButtons('7', '8', '9', '+', col);
					break;
				case 11:
					drawButtons('4', '5', '6', '-', col);
					break;
				case 13:
					drawButtons('1', '2', '3', '*', col);
					break;
				case 15:
					drawButtons('/', '0', '%', '=', col);
					break;
				case 0:
				case 3:
				case 16:
				case 17:
					printf("_");
					break;
				case 6:
				case 8:
				case 10:
				case 12:
				case 14:
					printf("-");
					break;
				default:
					printf(" ");
			}

			if (col == width-1) {
				if (row == 0) {printf("\n");}
				else if (row == 1) {printf("|\\\n");}
				else if (row == height-1) {printf("\\|\n");}
				else {printf("||\n");}
			} 
		}
	}
}

void drawButtons(char a, char b, char c, char d, int col) {
	if (col == 0) {printf(" %c |", a); } 
	else if (col == 3) {printf(" %c |", b);} 
	else if (col == 8) {printf(" %c |", c);} 
	else if (col == 13) {printf("  %c  ", d);}
}

void drawDisplay(int x, int y, char opr, int width) {
	int firstLen = getNumLength(x);
	int secLen = getNumLength(y);
	int totalLen = 0;
	switch (opr) {
		case 's':
		case 'r':
		case 'c':
			totalLen = firstLen + 7;
			for (int i = 0; i < width-totalLen; i++) {printf(" ");} 
			if (opr == 's') {printf("(%d) ^ 2 ", x);}
			if (opr == 'c') {printf("(%d) ^ 3 ", x);}
			if (opr == 'r') {printf("sqrt(%d) ", x);}
			break;
		case 'p':
			totalLen = firstLen + secLen + 6;
			for (int i = 0; i < width-totalLen; i++) {printf(" ");} 
			printf("(%d) ^ %d ", x, y);
			break;
		case '+':
		case '-':
		case '*':
		case '/':
		case '%':
			totalLen = firstLen + secLen + 4;
			for (int i = 0; i < width-totalLen; i++) {printf(" ");}
			printf("%d %c %d ", x, opr, y);
			break;
		case '=':
			totalLen = firstLen + 1;
			for (int i = 0; i < width-totalLen; i++) {printf(" ");}
			printf("%d ", x);
			break;
		default:
			totalLen = 10;
			for (int i = 0; i < width-totalLen; i++) {printf(" ");}
			printf("ERROR! :( ");
		}
}

void showAnswer(int ans, int width) {
	int len = getNumLength(ans)+1;
	for (int i = 0; i < width - len; i++) {
		printf(" ");
	}
	printf("%d ", ans);
}

int getNumLength(int n) {
	int len = 0;
	if (n < 0) {
		len = 1;
		n = multiply(n, -1);
	}
	do {
		n = n / 10;
		len++;
	} while (n > 0);
	return len;
}

int calculateOperation(int x, int y, char opr) {
	int result = 0;
	switch (opr) {
		case '+':
			result = add(x, y);
			break;
		case '-':
			result = substract(x, y);
			break;
		case '*':
			result = multiply(x, y);
			break;
		case '/':
			if (y==0) {
				result = -999999;
				printf("\n[!!!]::ZeroDivisionError!");
			}
			else {result = divide(x, y);}
			break;
		case '%':
			result = mod(x, y);
			break;
		case 'p':
			if (y < 0) {result = 0;}
			else {result = power(x, y);}
			break;
		default:
			printf("\n[!!!]::UnexpectedError!");
	}
	return result;
}

int calculateValue(int x, char opr) {
	int result = 0;
	switch (opr) {
		case 's':
			result = square(x);
			break;
		case 'c':
			result = cube(x);
			break;
		case 'r':
			if (x <= 0) {
				result = 0;
				printf("\n[!!!]::ValueError!");
			}
			else {result = squareRoot(x);}
			break;
		default:
			printf("\n[!!!]::UnexpectedError!");
	}
	return result;
}

int add(int x, int y) {
	return x + y;
}

int substract(int x, int y) {
	return x - y;
}

int multiply(int x, int y) {
	return x * y;
}

int divide(int x, int y) {
	return x / y;
}

int mod(int x, int y) {
	return x % y;
}

int square(int x) {
	return x * x;
}

int cube(int x) {
	return x * x * x;
}

int squareRoot(int x) {
	int num = 0;
	int sqValue = 0;
	int diff = 0;
	do {
		num++;
		sqValue = square(num);
		diff = substract(x, sqValue);
		// printf("%d by %d, diff %d\n", x, num, diff);
	} while (diff > num);
	return num;
}

int power(int base, int power) {
	int result = 1;
	for (int i = 0; i < power; i++) {
		result = multiply(result, base);
		// printf("%d x %d\n", result, base);
	}
	return result;
}
