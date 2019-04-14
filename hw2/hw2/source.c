#include <stdio.h>
#include <windows.h>
#define MAX_SIZE 50
#define MALLOC(p, s) \
	if (!((p) = malloc(s))) { \
		fprintf(stderr, "Insufficient memory"); \
		exit(EXIT_FAILURE); \
	}

typedef enum {head, entry} tagfield;
typedef struct matrixNode* matrixPointer;
typedef struct entryNode {
	int row;
	int col;
	int value;
}entryNode;

typedef struct matrixNode {
	matrixPointer down;
	matrixPointer right;
	tagfield tag;
	union {
		matrixPointer next;
		entryNode entry;
	}u;
}matrixNode;

matrixPointer hdnode[MAX_SIZE];

matrixPointer matrices[MAX_SIZE];
int matrixCounter = 0;

matrixPointer newNode() {
	matrixPointer pointer = NULL;
	MALLOC(pointer, sizeof(*pointer));
	return pointer;
}

void mread(int pos) {
	int numRows, numCols, numTerms, numHeads, i;
	int row, col, value, currentRow;
	matrixPointer temp, last, node;

	printf("Enter the number of rows, columns and number of nonzero terms : ");
	scanf("%d %d %d", &numRows, &numCols, &numTerms);
	numHeads = (numCols > numRows) ? numCols : numRows;

	if (numHeads > MAX_SIZE) {
		printf("Input matrix size exceeds the max size of matrix.\n");
		return;
	}
	
	node = newNode();
	node->tag = entry;
	node->u.entry.row = numRows;
	node->u.entry.col = numCols;

	if (!numHeads)
		node->right = node;
	else {
		for (i = 0; i < numHeads; i++) {
			temp = newNode();
			hdnode[i] = temp;
			hdnode[i]->tag = head;
			hdnode[i]->right = temp;
			hdnode[i]->u.next = temp;
		}
		currentRow = 0;
		last = hdnode[0];
		for (i = 0; i < numTerms; i++) {
			printf("Enter row, column and value : ");
			scanf("%d %d %d", &row, &col, &value);
			if (row > currentRow) {
				last->right = hdnode[currentRow];
				currentRow = row;
				last = hdnode[row];
			}
			temp = newNode();
			temp->tag = entry;
			temp->u.entry.row = row;
			temp->u.entry.col = col;
			temp->u.entry.value = value;
			last->right = temp;
			last = temp;
			
			hdnode[col]->u.next->down = temp;
			hdnode[col]->u.next = temp;
		}
		last->right = hdnode[currentRow];

		for (i = 0; i < numCols; i++)
			hdnode[i]->u.next->down = hdnode[i];

		for (i = 0; i < numHeads - 1; i++)
			hdnode[i]->u.next = hdnode[i + 1];
		hdnode[numHeads - 1]->u.next = node;
		node->right = hdnode[0];

		matrixCounter++;
		matrices[pos] = node;
		printf("Matrix added to %d.\n", pos);
	}
}
void mwrite(matrixPointer node) {
	int i;
	matrixPointer temp, head = node->right;

	printf("\n numRows = %d, numCols = %d\n", node->u.entry.row, node->u.entry.col);
	printf(" The matrix by row, column, and value : \n\n");
	for (i = 0; i < node->u.entry.row; i++) {

		for (temp = head->right; temp != head; temp = temp->right)
			printf("%5d%5d%5d \n", temp->u.entry.row, temp->u.entry.col, temp->u.entry.value);
		head = head->u.next;
	}
}
void merase(matrixPointer* node) {
	matrixPointer x, y, head = (*node)->right;
	int i, numHeads;

	for (i = 0; i < (*node)->u.entry.row; i++) {
		y = head->right;
		while (y != head) {
			x = y;
			y = y->right;
			free(x);
		}
		x = head;
		head = head->u.next;
		free(x);
	}

	y = head;
	while (y != *node) {
		x = y;
		y = y->u.next;
		free(x);
	}
	free(*node);
	*node = NULL;
	matrixCounter--;
	printf("Matrix erased.\n");
}
void madd(matrixPointer a, matrixPointer b, int pos) {
	if (a->u.entry.col != b->u.entry.col || a->u.entry.row != b->u.entry.row) {
		printf("Size of two matrices are not same.\n");
		return;
	}

	int numRows, numCols, numHeads, i;
	int row, col, value, currentRow;
	matrixPointer temp, last, node;
	matrixPointer firstHead = a->right, secondHead = b->right;
	matrixPointer firstTemp, secondTemp;

	node = newNode();
	node->tag = entry;
	node->u.entry.row = a->u.entry.row;
	node->u.entry.col = a->u.entry.col;
	numCols = a->u.entry.col;
	numRows = a->u.entry.row;
	numHeads = (numRows > numCols) ? numRows : numCols;

	for (i = 0; i < numHeads; i++) {
		temp = newNode();
		hdnode[i] = temp;
		hdnode[i]->tag = head;
		hdnode[i]->right = temp;
		hdnode[i]->u.next = temp;
	}
	currentRow = 0;
	last = hdnode[0];
	firstTemp = firstHead->right;
	secondTemp = secondHead->right;
	while (firstHead != a && secondHead != b) {
		if(firstTemp->tag == head)
			firstTemp = firstHead->right;
		if (secondTemp->tag == head)
			secondTemp = secondHead->right;
		if (firstTemp->u.entry.row < secondTemp->u.entry.row) {
			last = hdnode[firstTemp->u.entry.row];
			for (; firstTemp != firstHead; firstTemp = firstTemp->right) {
				row = firstTemp->u.entry.row;
				col = firstTemp->u.entry.col;
				value = firstTemp->u.entry.value;
				temp = newNode();
				temp->tag = entry;
				temp->u.entry.row = row;
				temp->u.entry.col = col;
				temp->u.entry.value = value;
				last->right = temp;
				last = temp;
				hdnode[col]->u.next->down = temp;
				hdnode[col]->u.next = temp;
			}
			last->right = hdnode[row];
		}
		else if (firstTemp->u.entry.row > secondTemp->u.entry.row) {
			last = hdnode[secondTemp->u.entry.row];
			for (; secondTemp != secondHead; secondTemp = secondTemp->right) {
				row = secondTemp->u.entry.row;
				col = secondTemp->u.entry.col;
				value = secondTemp->u.entry.value;
				temp = newNode();
				temp->tag = entry;
				temp->u.entry.row = row;
				temp->u.entry.col = col;
				temp->u.entry.value = value;
				last->right = temp;
				last = temp;
				hdnode[col]->u.next->down = temp;
				hdnode[col]->u.next = temp;
			}
			last->right = hdnode[row];
		}
		else if (firstTemp->u.entry.row == secondTemp->u.entry.row) {
			last = hdnode[firstTemp->u.entry.row];
			while (firstTemp != firstHead || secondTemp != secondHead) {
				if (secondTemp == secondHead) {
					col = firstTemp->u.entry.col;
					value = firstTemp->u.entry.value;
					firstTemp = firstTemp->right;
				}
				else if (firstTemp == firstHead) {
					row = secondTemp->u.entry.row;
					col = secondTemp->u.entry.col;
					value = secondTemp->u.entry.value;
					secondTemp = secondTemp->right;
				}
				else if (firstTemp->u.entry.col < secondTemp->u.entry.col) {
					row = firstTemp->u.entry.row;
					col = firstTemp->u.entry.col;
					value = firstTemp->u.entry.value;
					firstTemp = firstTemp->right;
				}
				else if (firstTemp->u.entry.col > secondTemp->u.entry.col) {
					row = secondTemp->u.entry.row;
					col = secondTemp->u.entry.col;
					value = secondTemp->u.entry.value;
					secondTemp = secondTemp->right;
				}
				else if (firstTemp->u.entry.col == secondTemp->u.entry.col) {
					row = firstTemp->u.entry.row;
					col = firstTemp->u.entry.col;
					value = firstTemp->u.entry.value + secondTemp->u.entry.value;
					firstTemp = firstTemp->right;
					secondTemp = secondTemp->right;
				}
				if (value) {
					temp = newNode();
					temp->tag = entry;
					temp->u.entry.row = row;
					temp->u.entry.col = col;
					temp->u.entry.value = value;
					last->right = temp;
					last = temp;
					hdnode[col]->u.next->down = temp;
					hdnode[col]->u.next = temp;
					last->right = hdnode[row];
				}
			}
		}
		firstHead = firstHead->u.next;
		secondHead = secondHead->u.next;
	}
	if (firstHead != a) {

		for (i = firstHead->u.entry.row; i < node->u.entry.row; i++) {

			for (firstTemp = firstHead->right; firstTemp != firstHead; firstTemp = firstTemp->right) {
				temp = newNode();
				temp->tag = entry;
				temp->u.entry.row = firstTemp->u.entry.row;
				temp->u.entry.col = firstTemp->u.entry.col;
				temp->u.entry.value = firstTemp->u.entry.value;
				last->right = temp;
				last = temp;
				hdnode[temp->u.entry.col]->u.next->down = temp;
				hdnode[temp->u.entry.col]->u.next = temp;
			}
			firstHead = firstHead->u.next;
			last->right = hdnode[row];
			last = hdnode[row];
		}
	}
	if (secondHead != b) {
		for (i = secondHead->u.entry.row; i < node->u.entry.row; i++) {

			for (secondTemp = secondHead->right; secondTemp != secondHead; secondTemp = secondTemp->right) {
				temp = newNode();
				temp->tag = entry;
				temp->u.entry.row = secondTemp->u.entry.row;
				temp->u.entry.col = secondTemp->u.entry.col;
				temp->u.entry.value = secondTemp->u.entry.value;
				last->right = temp;
				last = temp;
				hdnode[temp->u.entry.col]->u.next->down = temp;
				hdnode[temp->u.entry.col]->u.next = temp;
			}
			secondHead = secondHead->u.next;
			last->right = hdnode[row];
			last = hdnode[row];
		}
	}

	for (i = 0; i < numCols; i++)
		hdnode[i]->u.next->down = hdnode[i];

	for (i = 0; i < numHeads - 1; i++)
		hdnode[i]->u.next = hdnode[i + 1];
	hdnode[numHeads - 1]->u.next = node;
	node->right = hdnode[0];

	matrixCounter++;
	matrices[pos] = node;
	printf("Result matrix added to %d.\n", pos);
}
void mmult(matrixPointer a, matrixPointer b, int pos) {
	if (a->u.entry.col != b->u.entry.row) {
		printf("Size of two matrices does not match.\n");
		return;
	}

	int numRows, numCols, numHeads, i;
	int row, col, value, currentRow;
	matrixPointer temp, last, node;
	matrixPointer firstHead = a->right, secondHead = b->right;
	matrixPointer firstTemp, secondTemp;

	node = newNode();
	node->tag = entry;
	numRows = a->u.entry.row;
	numCols = b->u.entry.col;
	node->u.entry.row = numRows;
	node->u.entry.col = numCols;
	numHeads = (numRows > numCols) ? numRows : numCols;

	for (i = 0; i < numHeads; i++) {
		temp = newNode();
		hdnode[i] = temp;
		hdnode[i]->tag = head;
		hdnode[i]->right = temp;
		hdnode[i]->u.next = temp;
	}
	currentRow = 0;
	last = hdnode[0];
	firstTemp = firstHead->right;
	secondTemp = secondHead->right;
	for (int j = 0; j < numRows; j++) {
		last = hdnode[j];
		if (firstTemp->tag == head)
			firstTemp = firstHead->right;
		if (secondTemp->tag == head)
			secondTemp = secondHead->down;
		for (i = 0; i < numCols; i++) {
			value = 0;
			while (firstTemp != firstHead && secondTemp != secondHead) {
				if (firstTemp->u.entry.col == secondTemp->u.entry.row) {
					value += (firstTemp->u.entry.value * secondTemp->u.entry.value);
					row = firstTemp->u.entry.row;
					col = secondTemp->u.entry.col;
					firstTemp = firstTemp->right;
					secondTemp = secondTemp->down;
				}
				else if (firstTemp->u.entry.col < secondTemp->u.entry.row)
					firstTemp = firstTemp->right;
				else if (firstTemp->u.entry.col > secondTemp->u.entry.row)
					secondTemp = secondTemp->down;
			}
			if (value) {
				temp = newNode();
				temp->tag = entry;
				temp->u.entry.row = row;
				temp->u.entry.col = col;
				temp->u.entry.value = value;
				last->right = temp;
				last = temp;
				hdnode[col]->u.next->down = temp;
				hdnode[col]->u.next = temp;
				last->right = hdnode[row];
			}
			firstTemp = firstHead->right;
			secondHead = secondHead->u.next;
			secondTemp = secondHead->down;
		}
		firstHead = firstHead->u.next;
		secondHead = b->right;
		firstTemp = firstHead->right;
		secondTemp = secondHead->down;

		last->right = hdnode[row];
	}

	for (i = 0; i < numCols; i++)
		hdnode[i]->u.next->down = hdnode[i];

	for (i = 0; i < numHeads - 1; i++)
		hdnode[i]->u.next = hdnode[i + 1];
	hdnode[numHeads - 1]->u.next = node;
	node->right = hdnode[0];

	matrixCounter++;
	matrices[pos] = node;
	printf("Result matrix added to %d.\n", pos);

}
void mtranspose(matrixPointer a, int pos) {

	int numRows, numCols, numHeads, i;
	int row, col, value;
	matrixPointer temp, last, node;
	matrixPointer originalHead = a->right;
	matrixPointer originalTemp;

	node = newNode();
	node->tag = entry;
	numRows = a->u.entry.col;
	numCols = a->u.entry.row;
	node->u.entry.row = numRows;
	node->u.entry.col = numCols;
	numHeads = (numRows > numCols) ? numRows : numCols;

	for (i = 0; i < numHeads; i++) {
		temp = newNode();
		hdnode[i] = temp;
		hdnode[i]->tag = head;
		hdnode[i]->right = temp;
		hdnode[i]->u.next = temp;
	}
	last = hdnode[0];
	for (int j = 0; j < numRows; j++) {
		last = hdnode[j];
		for (originalTemp = originalHead->down; originalTemp != originalHead; originalTemp = originalTemp->down) {

			row = originalTemp->u.entry.col;
			col = originalTemp->u.entry.row;
			value = originalTemp->u.entry.value;
			temp = newNode();
			temp->tag = entry;
			temp->u.entry.row = row;
			temp->u.entry.col = col;
			temp->u.entry.value = value;
			last->right = temp;
			last = temp;
			hdnode[col]->u.next->down = temp;
			hdnode[col]->u.next = temp;
		}
		originalHead = originalHead->u.next;
		last->right = hdnode[j];
	}

	for (i = 0; i < numCols; i++)
		hdnode[i]->u.next->down = hdnode[i];

	for (i = 0; i < numHeads - 1; i++)
		hdnode[i]->u.next = hdnode[i + 1];
	hdnode[numHeads - 1]->u.next = node;
	node->right = hdnode[0];

	matrixCounter++;
	matrices[pos] = node;
	printf("Result matrix added to %d.\n", pos);
}

int main(void) {
	int menu, input1, input2, input3;
	int max, printCounter;
	while (1) {
		system("cls");
		printf("\n************************************\n");
		printf(" Existing matrices : ");
		max = matrixCounter;
		printCounter = 0;
		for (int i = 0; i < max; i++) {
			if (matrices[i] != NULL) {
				printCounter && printCounter % 5 == 0 ? printf("\n%23d ", i) : printf("%2d ", i);
				printCounter++;
			}
			else
				max++;
		}
		printf("\n************************************\n\n");
		printf("1 : Read in a sparse matrix.\n");
		printf("2 : Write out a sparse matrix.\n");
		printf("3 : Erase a sparse matrix.\n");
		printf("4 : Add two sparse matrix.\n");
		printf("5 : Multiply two sparse matrix.\n");
		printf("6 : Transpose a sparse matrix.\n");
		printf("7 : Exit the program.\n\n");
		printf("Type menu : ");
		scanf("%d", &menu);
		system("cls");
		switch (menu) {
		case 1:
			if (matrixCounter > MAX_SIZE) {
				printf("There are too many matrices.\n");
				break;
			}
			printf("Type the number of position you want to save the matrix : ");
			scanf("%d", &input1);
			matrices[input1] != NULL ? printf("Matrix already exists at the position %d.\n", input1) : mread(input1);
			break;
		case 2:
			printf("Type the number of a matrix you want to print : ");
			scanf("%d", &input1);
			matrices[input1] == NULL ? printf("There is no matrix at that position.\n") : mwrite(matrices[input1]);
			break;
		case 3:
			printf("Type the number of a matrix you want to erase : ");
			scanf("%d", &input1);
			matrices[input1] == NULL ? printf("There is no matrix at that position.\n") : merase(&matrices[input1]);
			break;
		case 4:
			printf("Type the number of two matrices you want to add : ");
			scanf("%d %d", &input1, &input2);
			printf("Type the number of position you want to save the result matrix : ");
			scanf("%d", &input3);
			madd(matrices[input1], matrices[input2], input3);
			break;
		case 5:
			printf("Type the number of two matrices you want to multiply : ");
			scanf("%d %d", &input1, &input2);
			printf("Type the number of position you want to save the result matrix : ");
			scanf("%d", &input3);
			mmult(matrices[input1], matrices[input2], input3);
			break;
		case 6:
			printf("Type the number of a matrix you want to get transpose of : ");
			scanf("%d", &input1);
			printf("Type the number of position you want to save the result matrix : ");
			scanf("%d", &input2);
			mtranspose(matrices[input1], input2);
			break;
		case 7:
			return 0;
			break;
		}
		printf("\nPress any key to proceed.");
		getch();
	}
	return 0;
}