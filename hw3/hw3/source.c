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

//Initiate and reset the set.
void initSet(int n) {
	int i;
	for (i = 0; i < n; i++) {
		//Set parent set to -1.
		parentSet[i] = -1;
		//Set count set to 1.
		countSet[i] = 1;
	}
}

//Find the parent of the vertex.
int findSet(int vertex) {
	int setIndex = 0, temp = 0;
	//Find the parent of the vertex.
	for (temp = vertex; parentSet[temp] >= 0; temp = parentSet[temp]) ;
	setIndex = temp;
	//Set all numbers' parents to the parent of the vertex.
	for (temp = vertex; parentSet[temp] >= 0; temp = parentSet[temp]) {
		parentSet[temp] = setIndex;
	}
	return temp;
}

//Unite set1 and set2.
//Make the set whose node is smaller parent of the set whose node is bigger.
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

//Initiate and reset the graph.
void initGraph(Graph *g) {
	g->heapSize = 0;
}

//Push edge to the heap of edges in g.
void pushEdgeHeap(Graph *g, Edge edge) {
	int i = ++g->heapSize;
	while ((i != 1) && (edge.cost < g->edgeHeap[i / 2].cost)) {
		g->edgeHeap[i] = g->edgeHeap[i / 2];
		i /= 2;
	}
	g->edgeHeap[i] = edge;
}

//Pop edge form the heap of edges in g.
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

//Make new edge and push to the heap of edges in g with the left, right, and cost.
void addNewEdge(Graph *g, int left, int right, int cost) {
	Edge e;
	e.left = left;
	e.right = right;
	e.cost = cost;
	pushEdgeHeap(g, e);
}

//Kruskal's algorithm.
void kruskal(Graph *g) {
	//The number of edges accepted to the spanning tree.
	int acceptedEdge = 0;
	int leftSet, rightSet;
	Edge e;
	//The array of edges accepted to the spanning tree.
	Edge *acceptedEdges = malloc((g->nodeCount - 1) * sizeof(Edge));
	initSet(g->nodeCount);
	//Check edges until sufficient edges are accepted.
	while (acceptedEdge < (g->nodeCount - 1)) {
		//If there are no more edges when accepted edges are insufficient, it means there is no spanning tree.
		if (g->heapSize == 0) {
			printf("**There is no spanning tree for this graph.\n");
			free(acceptedEdges);
			return;
		}
		e = popEdgeHeap(g);
		leftSet = findSet(e.left);
		rightSet = findSet(e.right);
		//If the edge doesn't make loop, then accept.
		if (leftSet != rightSet) {
			acceptedEdges[acceptedEdge] = e;
			acceptedEdge++;
			uniteSet(leftSet, rightSet);
		}
	}

	int totalCost = 0;
	printf("\n**The spanning tree of the graph\n");
	//Print all edges accepted to the spanning tree.
	for (int i = 0; i < acceptedEdge; i++) {
		printf("  (%d, %d) %d \n", acceptedEdges[i].left, acceptedEdges[i].right, acceptedEdges[i].cost);
		totalCost += acceptedEdges[i].cost;
	}
	printf("  The cost of the spanning tree : %d\n", totalCost);
	free(acceptedEdges);
}

void main()
{
	int nodeCount, edgeCount;
	int leftNode, rightNode, edgeCost;
	Graph g;
	printf("*COSE213 Assignment #3\n*2018320192 컴퓨터학과 손재민\n\n");
	printf("**Kruskal's algorithm program\n\n");
	//Get the number of nodes and edges of the graph.
	printf(" Enter the number of nodes and edges of the graph : ");
	scanf("%d %d", &nodeCount, &edgeCount);
	initGraph(&g);
	g.nodeCount = nodeCount;
	getchar();
	for (int i = 0; i < edgeCount; i++) {
		//Get the information of the edge.
		printf(" Enter the edge %d in the form of (node node cost) : ", i + 1);
		scanf("%d %d %d", &leftNode, &rightNode, &edgeCost);
		addNewEdge(&g, leftNode, rightNode, edgeCost);
		getchar();
	}
	//Start kruskal algorithm.
	kruskal(&g);
}