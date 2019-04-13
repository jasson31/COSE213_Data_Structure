#include <stdio.h>
#include <windows.h>
#define MAX_SIZE 50
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
	matrixPointer node = (matrixPointer)malloc(sizeof(matrixPointer));
	return node;
}

void mread() {
	int numRows, numCols, numTerms, numHeads, i;
	int row, col, value, currentRow;
	matrixPointer temp, last, node;

	printf("Enter the number of rows, columns and number of nonzero terms : ");
	scanf("%d %d %d", &numRows, &numCols, &numTerms);
	numHeads = (numCols > numRows) ? numCols : numRows;

	if (numHeads > MAX_SIZE) {
		printf("Input matrix size exceeds the max size of matrix.\n");
		Sleep(1000);
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
		if(matrixCounter <= MAX_SIZE)
			matrices[matrixCounter++] = node;
		else {
			fprintf(stderr, "asdf");
			return;
		}
	}
}
void mwrite() {

}
void merase() {

}
void madd() {

}
void mmult() {

}
void mtranspose() {

}

int main(void) {
	int menu;
	while (1) {
		system();
		printf("Type menu\n");
		printf("%d : Read in a sparse matrix.\n", 1);
		printf("%d : Write out a sparse matrix.\n", 2);
		printf("%d : Erase a sparse matrix.\n", 3);
		printf("%d : Add two sparse matrix.\n", 4);
		printf("%d : Multiply two sparse matrix.\n", 5);
		printf("%d : Transpose a sparse matrix.\n", 6);
		printf("%d : Exit the program.\n", 7);
		scanf("%d", &menu);
		switch (menu) {
		case 1:
			mread();
			break;
		case 2:
			mwrite();
			break;
		case 3:
			merase();
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
	}
	return 0;
}