#include <stdio.h>
#include <windows.h>
#define MAX_SIZE 2
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

		for (i = 0; i < numCols; i++) {
			hdnode[i]->u.next->down = hdnode[i];

			for (i = 0; i < numHeads - 1; i++)
				hdnode[i]->u.next = hdnode[i + 1];
			hdnode[numHeads - 1]->u.next = node;
			node->right = hdnode[0];
		}


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
	printf("Matrix erased.\n");
}
void madd() {

}
void mmult() {

}
void mtranspose() {

}

int main(void) {
	int menu;
	int input;
	while (1) {
		system("cls");
		printf("%d : Read in a sparse matrix.\n", 1);
		printf("%d : Write out a sparse matrix.\n", 2);
		printf("%d : Erase a sparse matrix.\n", 3);
		printf("%d : Add two sparse matrix.\n", 4);
		printf("%d : Multiply two sparse matrix.\n", 5);
		printf("%d : Transpose a sparse matrix.\n", 6);
		printf("%d : Exit the program.\n", 7);
		printf("Type menu : ");
		scanf("%d", &menu);
		system("cls");
		switch (menu) {
		case 1:
			if (++matrixCounter > MAX_SIZE) {
				printf("There are too many matrices.\n");
				break;
			}
			printf("Type the number of position you want to save matrix : ");
			scanf("%d", &input);
			matrices[input] != NULL ? printf("Matrix already exists at the position %d.\n", input) : mread(input);
			break;
		case 2:
			printf("Type the number of matrix you want to print : ");
			scanf("%d", &input);
			matrices[input] == NULL ? printf("There is no matrix at that position.\n") : mwrite(matrices[input]);
			break;
		case 3:
			printf("Type the number of matrix you want to erase : ");
			scanf("%d", &input);
			matrices[input] == NULL ? printf("There is no matrix at that position.\n") : merase(&matrices[input]);
			break;
		case 4:
			madd();
			break;
		case 5:
			mmult();
			break;
		case 6:
			mtranspose();
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