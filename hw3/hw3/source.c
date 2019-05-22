#include <stdio.h>

#define MAX_VERTICES 100
#define MAX_ELEMENT 100

int parentSet[MAX_VERTICES];
int countSet[MAX_VERTICES];

typedef struct Edge {
	int cost;
	int right;
	int left;
} Edge;

typedef struct Graph {
	Edge edgeHeap[MAX_ELEMENT];
	int heapSize;
	int nodeCount;
} Graph;

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


void initGraph(Graph *g) {
	g->heapSize = 0;
}

int isEmpty(Graph g) {
	return g.heapSize == 0 ? 1 : 0;
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

void kruskal(Graph *g) {
	int acceptedEdge = 0;
	int leftSet, rightSet;
	Edge e;
	initSet(g->nodeCount);
	while (acceptedEdge < (g->nodeCount - 1)) {
		e = popEdgeHeap(g);
		leftSet = findSet(e.left);
		rightSet = findSet(e.right);
		if (leftSet != rightSet) {
			printf("  (%d, %d) %d \n", e.left, e.right, e.cost);
			acceptedEdge++;
			uniteSet(leftSet, rightSet);
		}
	}
}


void main()
{
	int nodeCount, edgeCount;
	int leftNode, rightNode, edgeCost;
	Graph g;
	printf("*COSE213 Assignment #3\n*2018320192 컴퓨터학과 손재민\n\n");
	printf("**Kruskal's algorithm program\n");
	printf("*Type the number of nodes in the graph : ");
	scanf("%d", &nodeCount);
	initGraph(&g);
	g.nodeCount = nodeCount;
	printf("*Type the number of edges in the graph : ");
	scanf("%d", &edgeCount);
	getchar();
	for (int i = 0; i < edgeCount; i++) {
		printf(" Type the edge in the form of (node node cost) : ");
		scanf("%d %d %d", &leftNode, &rightNode, &edgeCost);
		addNewEdge(&g, leftNode, rightNode, edgeCost);
		getchar();
	}
	printf("\n**Result\n");
	kruskal(&g);
}