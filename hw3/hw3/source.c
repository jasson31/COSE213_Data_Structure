#include <stdio.h>

#define TRUE 1
#define FALSE 0
#define MAX_VERTICES 100
#define MAX_ELEMENT 100

int parentSet[MAX_VERTICES];
int countSet[MAX_VERTICES];

void initSet(int n) {
	int i;
	for (i = 0; i < n; i++) {
		parentSet[i] = -1;
		countSet[i] = 1;
	}
}

int findSet(int vertex) {
	int setIndex = 0, temp = 0;
	for (temp = vertex; parentSet[temp] >= 0; temp = parentSet[temp]) ;
	setIndex = temp;
	for (temp = vertex; parentSet[temp] >= 0; temp = parentSet[temp]) {
		parentSet[temp] = setIndex;
	}
	return temp;
}

void uniteSet(int set1, int set2) {
	if (countSet[set1] < countSet[set2]) {
		parentSet[set1] = set2;
		countSet[set2] += countSet[set1];
	}
	else {
		parentSet[set2] = set1;
		countSet[set1] += countSet[set2];
	}
}

typedef struct Edge {
	int cost;
	int right;
	int left;
} Edge;

typedef struct Graph {
	Edge edgeHeap[MAX_ELEMENT];
	int heapSize;
} Graph;

void initGraph(Graph *g) {
	g->heapSize = 0;
}

int isEmpty(Graph g) {
	return g.heapSize == 0 ? TRUE : FALSE;
}

void pushEdgeHeap(Graph *g, Edge edge) {
	int i = ++g->heapSize;
	while ((i != 1) && (edge.cost < g->edgeHeap[i / 2].cost)) {
		g->edgeHeap[i] = g->edgeHeap[i / 2];
		i /= 2;
	}
	g->edgeHeap[i] = edge;
}

Edge popEdgeHeap(Graph *g) {
	int parent = 1, child = 2;
	Edge item = g->edgeHeap[1], temp = g->edgeHeap[g->heapSize--];
	while (child <= g->heapSize) {
		if ((child < g->heapSize) && (g->edgeHeap[child].cost) > g->edgeHeap[child + 1].cost)
			child++;
		if (temp.cost <= g->edgeHeap[child].cost)
			break;
		g->edgeHeap[parent] = g->edgeHeap[child];
		parent = child;
		child *= 2;
	}
	g->edgeHeap[parent] = temp;
	return item;
}

void addNewEdge(Graph *g, int left, int right, int cost) {
	Edge e;
	e.left = left;
	e.right = right;
	e.cost = cost;
	pushEdgeHeap(g, e);
}

void insert_all_edges(Graph *g) {
	addNewEdge(g, 0, 1, 29);
	addNewEdge(g, 1, 2, 16);
	addNewEdge(g, 2, 3, 12);
	addNewEdge(g, 3, 4, 22);
	addNewEdge(g, 4, 5, 27);
	addNewEdge(g, 5, 0, 10);
	addNewEdge(g, 6, 1, 15);
	addNewEdge(g, 6, 3, 18);
	addNewEdge(g, 6, 4, 25);
}

void kruskal(int n) {
	int acceptedEdge = 0;
	Graph g;
	int leftSet, rightSet;
	Edge e;
	initGraph(&g);
	insert_all_edges(&g);
	initSet(n);
	while (acceptedEdge < (n - 1)) {
		e = popEdgeHeap(&g);
		leftSet = findSet(e.left);
		rightSet = findSet(e.right);
		if (leftSet != rightSet) {
			printf("(%d, %d) %d \n", e.left, e.right, e.cost);
			acceptedEdge++;
			uniteSet(leftSet, rightSet);
		}
	}
}


void main()
{
	printf("*COSE213 Assignment #3\n*2018320192 컴퓨터학과 손재민\n\n");
	/*while (1) {
		printf("**Kruskal's algorithm program\n");
		printf("*Type the menu.\n");

	}*/
	kruskal(7);
}