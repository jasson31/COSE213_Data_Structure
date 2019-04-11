#include <stdio.h>
#define MAX_SIZE 50
typedef enum {head, entry} tagfield;
typedef struct matrixNode* matrixPointer;
typedef struct{
	int row;
	int col;
	int value;
}entryNode;

typedef struct {
	matrixPointer down;
	matrixPointer right;
	tagfield tag;
	union {
		matrixPointer next;
		entryNode entry;
	};
}matrixNode;

matrixPointer hdNode[MAX_SIZE];
int matrixCounter = 0;

void mread() {
	int row, col;
	printf("Type the number of rows : ");
	scanf("%d", &row);
	printf("Type the number of columns : ");
	scanf("%d", &col);
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
		printf("Type menu\n");
		printf("%d : Read in a sparse matrix.\n", 1);
		printf("%d : Write out a sparse matrix.\n", 2);
		printf("%d : Erase a sparse matrix.\n", 3);
		printf("%d : Add two sparse matrix.\n", 4);
		printf("%d : Multiply two sparse matrix.\n", 5);
		printf("%d : Transpose a sparse matrix.\n", 6);
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
			mtrapnspose();
			break;
		}

	}
	return 0;
}