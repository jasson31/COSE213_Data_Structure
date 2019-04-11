//This program is designed to work for the descending sorted input.
#include <stdio.h>
//Maximum amount of terms is 100.
//If amount of terms exceeds 100, then program will finish.
#define MAX_TERMS 100

typedef struct {
	float coef;
	int exp;
} polynomial;
polynomial terms[MAX_TERMS];
int avail = 0;

//Print polynomial
void printPoly(int startPoly, int endPoly) {
	int i;
	for (i = startPoly; i < endPoly; i++) {
		printf("%.3fx^%d + ", terms[i].coef, terms[i].exp);
	}
	//Print last term without '+'
	if (terms[endPoly].exp)
		printf("%.3fx^%d\n", terms[endPoly].coef, terms[endPoly].exp);
	else
		printf("%.3f\n", terms[endPoly].coef);
}
//Add coefficient and exponent in terms array
void attachPoly(float coef, int exp) {
	if (avail >= MAX_TERMS) {
		fprintf(stderr, "Too many terms in the polynomial.\n");
		exit(-1);
	}
	terms[avail].coef = coef;
	terms[avail++].exp = exp;
}
//Get input of proper polynomial and save it in terms array
void newPoly(int* startPoly, int* endPoly) {
	*startPoly = avail;
	float coef, exp;
	do {
		if (!scanf("%f", &coef)) {
			//If input coefficient is not real number then error
			fprintf(stderr, "Invalid input.\nCoefficient should be real number.");
			exit(-1);
		}
		if (getchar() == '\n') {
			//If input coefficient and exponent does not paired then error 
			fprintf(stderr, "Invalid input.\nEach coefficient and exponent must be paired.");
			exit(-1);
		}
		scanf("%f", &exp);
		if (exp - (int)exp || exp < 0) {
			//If input coefficient is not integer then error
			fprintf(stderr, "Invalid input.\nExponents should be integer above 0.");
			exit(-1);
		}
		attachPoly(coef, exp);
	} while (getchar() != '\n'); //If input is '\n' then end getting input
	*endPoly = avail - 1;
}
//Add two polynomial and save it in terms array
void addPoly(int startA, int endA, int startB, int endB, int* startR, int* endR) {
	*startR = avail;
	float coefSum;
	//Check and add two polynomial until one polynomial ends
	while (startA <= endA && startB <= endB) {
		switch (terms[startA].exp > terms[startB].exp ? 1 : (terms[startA].exp < terms[startB].exp ? -1 : 0)) {
		case 1:
			//If exponent of A is bigger then add term of A
			attachPoly(terms[startA].coef, terms[startA].exp);
			startA++;
			break;
		case 0:
			//If exponent of A and B is same then add sum of A and B
			coefSum = terms[startA].coef + terms[startB].coef;
			//If sum of exponent is 0 then don't add it
			if (coefSum)
				attachPoly(coefSum, terms[startA].exp);
			startA++;
			startB++;
			break;
		case -1:
			//If exponent of B is bigger then add term of B
			attachPoly(terms[startB].coef, terms[startB].exp);
			startB++;
			break;
		}
	}
	//If there remains terms in A then add it
	for (; startA <= endA; startA++) {
		attachPoly(terms[startA].coef, terms[startA].exp);
	}
	//If there remains terms in B then add it
	for (; startB <= endB; startB++) {
		attachPoly(terms[startB].coef, terms[startB].exp);
	}
	*endR = avail - 1;
}

int main(void) {
	int startA, endA, startB, endB, startR, endR;
	printf("첫번째 다항식을 입력하세요.\n");
	newPoly(&startA, &endA);
	printf("두번째 다항식을 입력하세요.\n");
	newPoly(&startB, &endB);
	addPoly(startA, endA, startB, endB, &startR, &endR);
	printPoly(startR, endR);
}