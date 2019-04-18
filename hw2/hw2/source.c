#include <stdio.h>
#include <windows.h>
#define MAX_SIZE 50
#define MALLOC(p, s) \
	if (!((p) = malloc(s))) { \
		fprintf(stderr, "  **Insufficient memory"); \
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
//The array for saving header nodes of the matrices.
matrixPointer matrices[MAX_SIZE];
//Count the number of saved matrices.
int matrixCounter = 0;

//Allocate memory for a new node.
matrixPointer newNode() {
	matrixPointer pointer = NULL;
	MALLOC(pointer, sizeof(*pointer));
	return pointer;
}
//Get matrix input and an empty position and save it to the position.
void mread(int pos) {
	int numRows, numCols, numTerms, numHeads, i;
	int row, col, value, currentRow;
	matrixPointer temp, last, node;
	printf("  Enter the number of rows, columns and number of nonzero terms(rows columns terms) : ");
	//Get the number of rows, columns, and nonzero terms.
	scanf("%d %d %d", &numRows, &numCols, &numTerms);
	//Set bigger one between the number of rows and the number of columns as the number of head nodes.
	numHeads = (numCols > numRows) ? numCols : numRows;
	if (numHeads > MAX_SIZE) {
		//If the number of head nodes exceed the 50(MAX_SIZE), then reject the input matrix.
		printf("\n  **Input matrix size exceeds the max size of matrix.\n\n");
		return;
	}
	if (numRows <= 0 || numCols <= 0 || numTerms <= 0) {
		//If the number of column, row or nonzero terms are less than 1, then reject the input matrix.
		printf("\n  **Inappropriate input for matrix.\n\n");
		return;
	}
	node = newNode();
	node->tag = entry;
	node->u.entry.row = numRows; 
	node->u.entry.col = numCols;
	node->u.entry.value = numTerms;
	printf("\n");
	if (!numHeads)
		node->right = node;
	else {
		//Make head nodes and connect them.
		for (i = 0; i < numHeads; i++) {
			temp = newNode();
			hdnode[i] = temp;
			hdnode[i]->tag = head;
			hdnode[i]->right = temp;
			hdnode[i]->u.next = temp;
		}
		currentRow = 0;
		//At first, make last node as the first head node.
		last = hdnode[0];
		for (i = 0; i < numTerms; i++) {
			printf("  Enter row, column and value(row column value) : ");
			//Get the row, column, and value.
			scanf("%d %d %d", &row, &col, &value);
			if (row > currentRow) {
				//If input row is bigger than current row, connect the previous head node to right of the last node.
				last->right = hdnode[currentRow];
				currentRow = row;
				//Make the last node as the head node of input row.
				last = hdnode[row];
			}
			//Make a new node with input row, column, and value.
			temp = newNode();
			temp->tag = entry;
			temp->u.entry.row = row;
			temp->u.entry.col = col;
			temp->u.entry.value = value;
			//Connect the new node to the right of the last node and make last node as the new node.
			last->right = temp;
			last = temp;
			//Connect the new node to the down of the head node of input column and make last node of the head node as the new node.
			hdnode[col]->u.next->down = temp;
			hdnode[col]->u.next = temp;
		}
		//Connect the head node of input row to right of the last node.
		last->right = hdnode[currentRow];
		//Connect each head nodes to the down of bottom nodes by column.
		for (i = 0; i < numCols; i++)
			hdnode[i]->u.next->down = hdnode[i];
		//Connect each head nodes.
		for (i = 0; i < numHeads - 1; i++)
			hdnode[i]->u.next = hdnode[i + 1];
		//Connect the last head node to the header node.
		hdnode[numHeads - 1]->u.next = node;
		node->right = hdnode[0];
		matrixCounter++;
		//Save the header node to the pos.
		matrices[pos] = node;
		printf("\n  Matrix added to %d.\n", pos);
	}
}
//Get the position of matrix and print it.
void mwrite(matrixPointer node) {
	int i;
	matrixPointer temp, head = node->right;
	//Print the numbers of rows and columns of the matrix.
	printf("\n  numRows = %d, numCols = %d\n", node->u.entry.row, node->u.entry.col);
	printf("  The matrix by row, column, and value : \n\n");
	//Print the row, column, and value of nonzero terms.
	//Print each terms by increasing order of columns, and rows.
	for (i = 0; i < node->u.entry.row; i++) {
		for (temp = head->right; temp != head; temp = temp->right)
			printf("  %5d%5d%5d \n", temp->u.entry.row, temp->u.entry.col, temp->u.entry.value);
		head = head->u.next;
	}
}
//Get the position of matrix and erase it.
void merase(matrixPointer* node) {
	matrixPointer x, y, head = (*node)->right;
	int i;
	//Free all nodes connected to the head nodes by the increasing order of row.
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
	//Free all head nodes.
	while (y != *node) {
		x = y;
		y = y->u.next;
		free(x);
	}
	//Free header node.
	free(*node);
	*node = NULL;
	matrixCounter--;
	printf("\n  Matrix erased.\n");
}
//Get the positions of two matrices and an empty position and save the sum of two matrices to the position.
void madd(matrixPointer a, matrixPointer b, int pos) {
	if (a->u.entry.col != b->u.entry.col || a->u.entry.row != b->u.entry.row) {
		//If the size of two matrices does not match, then reject.
		printf("\n  **Size of two matrices are not same.\n");
		return;
	}
	int numRows, numCols, numTerms = 0, numHeads, i;
	int row, col, value;
	matrixPointer temp, last, node;
	matrixPointer firstHead = a->right, secondHead = b->right;
	matrixPointer firstTemp, secondTemp;
	node = newNode();
	node->tag = entry;
	node->u.entry.row = a->u.entry.row;
	node->u.entry.col = a->u.entry.col;
	numCols = a->u.entry.col;
	numRows = a->u.entry.row;
	//Set bigger one between the number of rows and the number of columns as the number of head nodes.
	numHeads = (numRows > numCols) ? numRows : numCols;
	for (i = 0; i < numHeads; i++) {
		//Make head nodes and connect them.
		temp = newNode();
		hdnode[i] = temp;
		hdnode[i]->tag = head;
		hdnode[i]->right = temp;
		hdnode[i]->u.next = temp;
	}
	last = hdnode[0];
	firstTemp = firstHead->right;
	secondTemp = secondHead->right;
	//Add each nodes by the increasing order of from columns to rows.
	for(i = 0; i < numRows; i++) {
		firstTemp = firstHead->right;
		secondTemp = secondHead->right;
		//If there is no node at both of the matrix, then skip this row.
		if (firstTemp == firstHead && secondTemp == secondHead);
		//If there is no node at the second matrix, then add all nodes in current row of the first matrix.
		else if (secondTemp == secondHead) {
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
				numTerms++;
			}
			last->right = hdnode[row];
		}
		//If there is no node at the first matrix, then add all nodes in current row of the second matrix.
		else if (firstTemp == firstHead) {
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
				numTerms++;
			}
			last->right = hdnode[row];
		}
		//If there exists nodes at the both matrices, then add the sum of all nodes in current row of the both matrices.
		else if (firstTemp->u.entry.row == secondTemp->u.entry.row) {
			last = hdnode[firstTemp->u.entry.row];
			//Add all nodes in current row of the both matrices.
			while (firstTemp != firstHead || secondTemp != secondHead) {
				//If already checked all nodes in the second matrix, then set the node as remaining nodes in the first matrix.
				if (secondTemp == secondHead) {
					col = firstTemp->u.entry.col;
					value = firstTemp->u.entry.value;
					firstTemp = firstTemp->right;
				}
				//If already checked all nodes in the first matrix, then set the node as remaining nodes in the second matrix.
				else if (firstTemp == firstHead) {
					row = secondTemp->u.entry.row;
					col = secondTemp->u.entry.col;
					value = secondTemp->u.entry.value;
					secondTemp = secondTemp->right;
				}
				//If the column of the current node in the first matrix is smaller than the one of the second matrix, then set the node as the node of the first matrix.
				else if (firstTemp->u.entry.col < secondTemp->u.entry.col) {
					row = firstTemp->u.entry.row;
					col = firstTemp->u.entry.col;
					value = firstTemp->u.entry.value;
					firstTemp = firstTemp->right;
				}
				//If the column of the current node in the second matrix is smaller than the one of the first matrix, then set the node as the node of the second matrix.
				else if (firstTemp->u.entry.col > secondTemp->u.entry.col) {
					row = secondTemp->u.entry.row;
					col = secondTemp->u.entry.col;
					value = secondTemp->u.entry.value;
					secondTemp = secondTemp->right;
				}
				//If the column of the current nodes in the both matrices are same, then set the node as the sum of the nodes of the both matrix.
				else if (firstTemp->u.entry.col == secondTemp->u.entry.col) {
					row = firstTemp->u.entry.row;
					col = firstTemp->u.entry.col;
					value = firstTemp->u.entry.value + secondTemp->u.entry.value;
					firstTemp = firstTemp->right;
					secondTemp = secondTemp->right;
				}
				//If value is not 0, then add the node to the result matrix.
				//Do this process because it is possible that the term disappears by the sum.
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
					numTerms++;
				}
			}
		}
		firstHead = firstHead->u.next;
		secondHead = secondHead->u.next;
	}
	//Connect each head nodes to the down of bottom nodes by column.
	for (i = 0; i < numCols; i++)
		hdnode[i]->u.next->down = hdnode[i];
	//Connect each head nodes.
	for (i = 0; i < numHeads - 1; i++)
		hdnode[i]->u.next = hdnode[i + 1];
	//Connect the last head node to the header node.
	hdnode[numHeads - 1]->u.next = node;
	node->right = hdnode[0];
	node->u.entry.value = numTerms;
	matrixCounter++;
	//Save the header node to the pos.
	matrices[pos] = node;
	printf("\n  Result matrix added to %d.\n", pos);
}
//Get the positions of two matrices and an empty position and save the product of two matrices to the position.
void mmult(matrixPointer a, matrixPointer b, int pos) {
	if (a->u.entry.col != b->u.entry.row) {
		//If the number of columns of the first matrix and the number of rows of the second matrix does not match, then reject.
		printf("\n  **Size of two matrices does not match.\n");
		return;
	}
	int numRows, numCols, numTerms = 0, numHeads, i;
	int row, col, value;
	matrixPointer temp, last, node;
	matrixPointer firstHead = a->right, secondHead = b->right;
	matrixPointer firstTemp, secondTemp;
	node = newNode();
	node->tag = entry;
	numRows = a->u.entry.row;
	numCols = b->u.entry.col;
	node->u.entry.row = numRows;
	node->u.entry.col = numCols;
	//Set bigger one between the number of rows and the number of columns as the number of head nodes.
	numHeads = (numRows > numCols) ? numRows : numCols;
	for (i = 0; i < numHeads; i++) {
		//Make head nodes and connect them.
		temp = newNode();
		hdnode[i] = temp;
		hdnode[i]->tag = head;
		hdnode[i]->right = temp;
		hdnode[i]->u.next = temp;
	}
	last = hdnode[0];
	firstTemp = firstHead->right;
	secondTemp = secondHead->right;
	//Multiply each nodes by increasing order of row of the first matrix and of column of the second matrix.
	for (int j = 0; j < numRows; j++) {
		last = hdnode[j];
		firstTemp = firstHead->right;
		secondTemp = secondHead->down;
		for (i = 0; i < numCols; i++) {
			value = 0;
			//Multiply nodes until one of the matrices reach to the end.
			while (firstTemp != firstHead && secondTemp != secondHead) {
				//If the column of the term of the first matrix and the row of the term of the second matrix are same, then set the node as the product of the nodes of the both matrix.
				if (firstTemp->u.entry.col == secondTemp->u.entry.row) {
					value += (firstTemp->u.entry.value * secondTemp->u.entry.value);
					row = firstTemp->u.entry.row;
					col = secondTemp->u.entry.col;
					firstTemp = firstTemp->right;
					secondTemp = secondTemp->down;
				}
				//If the column of the term of the first matrix is smaller than the row of the term of the second matrix, then move the first matrix to the right node.
				else if (firstTemp->u.entry.col < secondTemp->u.entry.row)
					firstTemp = firstTemp->right;
				//If the row of the term of the second matrix is smaller than the column of the term of the first matrix, then move the second matrix to the down node.
				else if (firstTemp->u.entry.col > secondTemp->u.entry.row)
					secondTemp = secondTemp->down;
			}
			//If value is not 0, then add the node to the result matrix.
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
				numTerms++;
			}
			firstTemp = firstHead->right;
			secondHead = secondHead->u.next;
			secondTemp = secondHead->down;
		}
		//At the end of the row of the result matrix, move the head node of the first matrix to the next row and the head node of the second matrix to the first column.
		firstHead = firstHead->u.next;
		secondHead = b->right;
		last->right = hdnode[j];
	}
	//Connect each head nodes to the down of bottom nodes by column.
	for (i = 0; i < numCols; i++)
		hdnode[i]->u.next->down = hdnode[i];
	//Connect each head nodes.
	for (i = 0; i < numHeads - 1; i++)
		hdnode[i]->u.next = hdnode[i + 1];
	//Connect the last head node to the header node.
	hdnode[numHeads - 1]->u.next = node;
	node->right = hdnode[0];
	node->u.entry.value = numTerms;
	matrixCounter++;
	//Save the header node to the pos.
	matrices[pos] = node;
	printf("\n  Result matrix added to %d.\n", pos);
}
//Get the position of a matrix and an empty position and save the transpose of the matrix to the position.
void mtranspose(matrixPointer a, int pos) {
	int numRows, numCols, numTerms = 0, numHeads, i;
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
	//Set bigger one between the number of rows and the number of columns as the number of head nodes.
	numHeads = (numRows > numCols) ? numRows : numCols;
	for (i = 0; i < numHeads; i++) {
		//Make head nodes and connect them.
		temp = newNode();
		hdnode[i] = temp;
		hdnode[i]->tag = head;
		hdnode[i]->right = temp;
		hdnode[i]->u.next = temp;
	}
	last = hdnode[0];
	//Reverse each nodes by increasing order of column and row.
	for (int j = 0; j < numRows; j++) {
		last = hdnode[j];
		//Add all nodes connected to the head node of current row by the increasing order of row, downside.
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
			numTerms++;
		}
		//At the end of the row of the result matrix, move the head node of the original matrix to the next column.
		originalHead = originalHead->u.next;
		last->right = hdnode[j];
	}
	//Connect each head nodes to the down of bottom nodes by column.
	for (i = 0; i < numCols; i++)
		hdnode[i]->u.next->down = hdnode[i];
	//Connect each head nodes.
	for (i = 0; i < numHeads - 1; i++)
		hdnode[i]->u.next = hdnode[i + 1];
	//Connect the last head node to the header node.
	hdnode[numHeads - 1]->u.next = node;
	node->right = hdnode[0];
	node->u.entry.value = numTerms;
	matrixCounter++;
	//Save the header node to the pos.
	matrices[pos] = node;
	//Save the header node to the pos.
	printf("\n  Result matrix added to %d.\n", pos);
}
//Print the information of all matrices.
void printExisting() {
	printf("\n┌───────────────────┐\n");
	printf("│ Existing matrices │\n");
	printf("└───────────────────┘\n");
	int max = matrixCounter;
	int printCounter = 0;
	printf("┌──────────────────────────────────────────────────────────────────┐\n");
	//If more than one matrix exists, then print information of all matrices.
	if (matrixCounter) {
		for (int i = 0; i < max; i++) {
			if (matrices[i] != NULL) {
				//Print the information of the matrix at the position.
				printf("│ At position [%3d], Rows : %3d, Columns : %3d, Nonzero terms : %3d│\n", i, matrices[i]->u.entry.row, matrices[i]->u.entry.col, matrices[i]->u.entry.value);
			}
			else
				max++;
		}
	}
	//If there is no matrix at all, then print the message.
	else
		printf("│                        There is no matrix.                       │\n");
	printf("└──────────────────────────────────────────────────────────────────┘\n\n");
}

int main(void) {
	int menu, input1, input2, input3;
	while (1) {
		printf("*COSE213 Assignment #2\n*2018320192 컴퓨터학과 손재민\n");
		printExisting();
		printf("┌────────────────────────────────┐\n");
		printf("│ 1 : Read in a sparse matrix.   │\n");
		printf("│ 2 : Write out a sparse matrix. │\n");
		printf("│ 3 : Erase a sparse matrix.     │\n");
		printf("│ 4 : Add two sparse matrix.     │\n");
		printf("│ 5 : Multiply two sparse matrix.│\n");
		printf("│ 6 : Transpose a sparse matrix. │\n");
		printf("│ 7 : Exit the program.          │\n");
		printf("└────────────────────────────────┘\n\n");
		printf("  Type menu : ");
		//Get the input of menu.
		scanf("%d", &menu);
		system("cls");
		printExisting();
		switch (menu) {		
		case 1:
			//Read in a matrix.
			if (matrixCounter > MAX_SIZE) {
				//If there are already 50(MAX_SIZE) matrices saved, then reject.
				printf("\n  **There are too many matrices.\n");
				break;
			}
			printf("  Type the number of position you want to save the matrix : ");
			//Get the position to save the matrix.
			scanf("%d", &input1);
			if (input1 < 0 || input1 > 50)
				//If the input position is inappropriate, then reject.
				printf("\n  **Position of matrix should be between from 0 to 49.\n");
			else if (matrices[input1] != NULL)
				//If the input position is not empty, then reject.
				printf("\n  **Matrix already exists at that position.\n");
			else
				mread(input1);
			break;
		case 2:
			//Write out a matrix.
			printf("  Type the number of a matrix you want to print : ");
			//Get the position of matrix.
			scanf("%d", &input1);
			if (input1 < 0 || input1 > 50)
				//If the input position is inappropriate, then reject.
				printf("\n  **Position of matrix should be between from 0 to 49.\n");
			else if (matrices[input1] == NULL)
				//If the input position is empty, then reject.
				printf("\n  **There is no matrix at that position.\n");
			else
				mwrite(matrices[input1]);
			break;
		case 3:
			//Erase a matrix.
			printf("  Type the number of a matrix you want to erase : ");
			//Get the position of matrix.
			scanf("%d", &input1);
			if (input1 < 0 || input1 > 50)
				//If the input position is inappropriate, then reject.
				printf("\n  **Position of matrix should be between from 0 to 49.\n");
			else if (matrices[input1] == NULL)
				//If the input position is empty, then reject.
				printf("\n  **There is no matrix at that position.\n"); 
			else
				merase(&matrices[input1]);
			break;
		case 4:
			//Get the sum of two matrices.
			if (matrixCounter > MAX_SIZE) {
				//If there are already 50(MAX_SIZE) matrices saved, then reject.
				printf("\n  **There are too many matrices.\n");
				break;
			}
			printf("  Type the number of two matrices you want to add : ");
			//Get the position of matrix.
			scanf("%d %d", &input1, &input2);
			printf("  Type the number of position you want to save the result matrix : ");
			//Get the position to save the matrix.
			scanf("%d", &input3);
			if ((input1 < 0 || input1 > 50) || (input2 < 0 || input2 > 50) || (input3 < 0 || input3 > 50))
				//If the input position is inappropriate, then reject.
				printf("\n  **Position of matrix should be between from 0 to 49.\n");
			else if (matrices[input1] == NULL || matrices[input2] == NULL)
				//If the input position is empty, then reject.
				printf("\n  **There is no matrix at that position.\n");
			else if (matrices[input3] != NULL)
				//If the input position is not empty, then reject.
				printf("\n  **Matrix already exists at the position.\n");
			else
				madd(matrices[input1], matrices[input2], input3);
			break;
		case 5:
			//Get the product of two matrices.
			if (matrixCounter > MAX_SIZE) {
				//If there are already 50(MAX_SIZE) matrices saved, then reject.
				printf("\n  **There are too many matrices.\n");
				break;
			}
			printf("  Type the number of two matrices you want to multiply : ");
			//Get the position of matrix.
			scanf("%d %d", &input1, &input2);
			printf("  Type the number of position you want to save the result matrix : ");
			//Get the position to save the matrix.
			scanf("%d", &input3);
			if ((input1 < 0 || input1 > 50) || (input2 < 0 || input2 > 50) || (input3 < 0 || input3 > 50))
				//If the input position is inappropriate, then reject.
				printf("\n  **Position of matrix should be between from 0 to 49.\n");
			else if (matrices[input1] == NULL || matrices[input2] == NULL)
				//If the input position is empty, then reject.
				printf("\n  **There is no matrix at that position.\n");
			else if (matrices[input3] != NULL)
				//If the input position is not empty, then reject.
				printf("\n  **Matrix already exists at the position.\n");
			else
				mmult(matrices[input1], matrices[input2], input3);
			break;
		case 6:
			//Get the transpose of a matrix.
			if (matrixCounter > MAX_SIZE) {
				//If there are already 50(MAX_SIZE) matrices saved, then reject.
				printf("\n  **There are too many matrices.\n");
				break;
			}
			printf("  Type the number of a matrix you want to get transpose of : ");
			//Get the position of matrix.
			scanf("%d", &input1);
			printf("  Type the number of position you want to save the result matrix : ");
			//Get the position to save the matrix.
			scanf("%d", &input2);
			if ((input1 < 0 || input1 > 50) || (input2 < 0 || input2 > 50))
				//If the input position is inappropriate, then reject.
				printf("\n  **Position of matrix should be between from 0 to 49.\n");
			else if (matrices[input1] == NULL)
				//If the input position is empty, then reject.
				printf("\n  **There is no matrix at that position.\n");
			else if (matrices[input2] != NULL)
				//If the input position is not empty, then reject.
				printf("\n  **Matrix already exists at the position.\n");
			else
				mtranspose(matrices[input1], input2);
			break;
		case 7:
			return 0;
			break;
		}
		printf("\n  Press any key to proceed.");
		getch();
		system("cls");
	}
	return 0;
}