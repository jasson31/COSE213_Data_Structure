/*#include <stdio.h>
#define MAX_DEGREE 101

typedef struct polynomial {
	float coef;
	int exp;
	struct polynomial *next;
} polynomial;

void printPoly(polynomial startPoly) {
	for (; startPoly.next != NULL; startPoly = *startPoly.next) {
		printf("%.3fx^%d + ", startPoly.coef, startPoly.exp);
	}
	if (startPoly.exp)
		printf("%.3fx^%d", startPoly.coef, startPoly.exp);
	else
		printf("%.3f", startPoly.coef);
}
polynomial newPoly() {
	polynomial* headPoly = (polynomial*)malloc(sizeof(polynomial));
	headPoly->next = NULL;
	float coef, exp;
	do {
		if (!scanf("%f", &coef)) {
			fprintf(stderr, "Invalid input.\nCoefficient should be real number.");
			exit(-1);
		}
		scanf("%f", &exp);
		if (exp - (int)exp || exp < 0) {
			fprintf(stderr, "Invalid input.\nExponents should be integer above 0.");
			exit(-1);
		}
		for (polynomial* temp = headPoly; ; temp = temp->next) {
			if (temp->next == NULL || temp->next->exp < exp) {
				polynomial* curPoly = (polynomial*)malloc(sizeof(polynomial));
				curPoly->coef = coef;
				curPoly->exp = (int)exp;
				curPoly->next = temp->next;
				temp->next = curPoly;
				break;
			}
			else if (temp->next->exp == (int)exp) {
				temp->next->coef += coef;
				break;
			}
		}
	} while (getchar() != '\n');
	headPoly = headPoly->next;
	return *headPoly;
}

polynomial addPoly(polynomial _firstPoly, polynomial _secondPoly) {
	polynomial* firstPoly = &_firstPoly, *secondPoly = &_secondPoly;
	polynomial* headPoly = (polynomial*)malloc(sizeof(polynomial));
	headPoly->next = NULL;
	polynomial* tempPoly = headPoly;
	while (firstPoly != NULL && secondPoly != NULL) {
		polynomial* curPoly = (polynomial*)malloc(sizeof(polynomial));
		if (firstPoly->exp == secondPoly->exp) {
			curPoly->coef = firstPoly->coef + secondPoly->coef;
			curPoly->exp = firstPoly->exp;
			curPoly->next = tempPoly->next;
			tempPoly->next = curPoly;
			firstPoly = firstPoly->next;
			secondPoly = secondPoly->next;
			tempPoly = tempPoly->next;
		}
		else if (firstPoly->exp > secondPoly->exp) {
			curPoly->coef = firstPoly->coef;
			curPoly->exp = firstPoly->exp;
			curPoly->next = tempPoly->next;
			tempPoly->next = curPoly;
			firstPoly = firstPoly->next;
			tempPoly = tempPoly->next;
		}
		else if (firstPoly->exp < secondPoly->exp) {
			curPoly->coef = secondPoly->coef;
			curPoly->exp = secondPoly->exp;
			curPoly->next = tempPoly->next;
			tempPoly->next = curPoly;
			secondPoly = secondPoly->next;
			tempPoly = tempPoly->next;
		}
	}
	for (; firstPoly != NULL; firstPoly = firstPoly->next) {
		polynomial* curPoly = (polynomial*)malloc(sizeof(polynomial));
		curPoly->coef = firstPoly->coef;
		curPoly->exp = firstPoly->exp;
		curPoly->next = tempPoly->next;
		tempPoly->next = curPoly;
		tempPoly = tempPoly->next;
	}
	for (; secondPoly != NULL; secondPoly = secondPoly->next) {
		polynomial* curPoly = (polynomial*)malloc(sizeof(polynomial));
		curPoly->coef = secondPoly->coef;
		curPoly->exp = secondPoly->exp;
		curPoly->next = tempPoly->next;
		tempPoly->next = curPoly;
		tempPoly = tempPoly->next;
	}
	headPoly = headPoly->next;
	printPoly(*headPoly);
	return*headPoly;
}

int main(void) {
	printf("첫번째 다항식을 입력하세요.\n");
	polynomial firstPoly = newPoly();
	printf("두번째 다항식을 입력하세요.\n");
	polynomial secondPoly = newPoly();
	polynomial resultPoly = addPoly(firstPoly, secondPoly);
}*/