#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#define N 12

//!!!!
//ALGO PRIM - arbore minim acoperire iterativ?
//!!!!

typedef struct Node {
    int dest;
    int cost;
    struct Node* next;
} Node;

typedef struct Graph {
    Node* head[N];
} Graph;

typedef struct Edge {
    int src, dest, cost;
} Edge;

//Pentru coada pt BFS ca sa afisez arborele

int csize = 0;
int sizelim = 10;

typedef struct Queue {
    int front, rear, capacity;
    int* array;
} Queue;




Graph* createGraph(Edge edges[], int n) {
    Graph* graph = (Graph*)malloc(sizeof(Graph));
    for (int i = 0; i < N; i++) {
        graph->head[i] = NULL;
    }

    for (int i = 0; i < n; i++) {
        int src = edges[i].src;
        int dest = edges[i].dest;
        int cost = edges[i].cost;

        Node* newNode = (Node*)malloc(sizeof(Node));
        newNode->dest = dest;
        newNode->cost = cost;
        newNode->next = graph->head[src];
        graph->head[src] = newNode;

        newNode = (Node*)malloc(sizeof(Node));
        newNode->dest = src;
        newNode->cost = cost;
        newNode->next = graph->head[dest];
        graph->head[dest] = newNode;
    }
    return graph;
}

Graph* addEdge(Graph* graph2, Edge edge) {
    int src = edge.src;
    int dest = edge.dest;
    int cost = edge.cost;
  int nrElem=0;
    Node* newNode = (Node*)malloc(sizeof(Node));
    newNode->dest = dest;
    newNode->cost = cost;
    newNode->next = graph2->head[src];
    graph2->head[src] = newNode;

    newNode = (Node*)malloc(sizeof(Node));
    newNode->dest = src;
    newNode->cost = cost;
    newNode->next = graph2->head[dest];
    graph2->head[dest] = newNode;

    return graph2;
}

void printGraph(Graph* graph) {
    for (int i = 0; i < N; i++) {
        Node* ptr = graph->head[i];
        while (ptr != NULL) {
            printf("(%d -> %d, cost: %d)\t", i, ptr->dest, ptr->cost);
            ptr = ptr->next;
        }
        printf("\n");
    }
}


void sortEdges(Edge edges[], int n) {
    for (int i = 0; i < n - 1; i++) {
        for (int j = i + 1; j < n; j++) {
            if (edges[i].cost > edges[j].cost) {
                Edge temp = edges[i];
                edges[i] = edges[j];
                edges[j] = temp;
            }
        }
    }
}


Queue* createQueue(int capacity) {
    Queue* queue = (Queue*)malloc(sizeof(Queue));
    queue->capacity = capacity;
    queue->front = 0;
    queue->rear = -1;
    queue->array = (int*)malloc(queue->capacity * sizeof(int));
    return queue;
}

int isFull(Queue* queue) {
    return (queue->rear == queue->capacity - 1);
}

int isEmpty(Queue* queue) {
    return (queue->front > queue->rear);
}

void enqueue(Queue* queue, int item) {
    if (isFull(queue)) {
        printf("Queue is full\n");
        return;
    }
    queue->rear++;
    queue->array[queue->rear] = item;
}

int dequeue(Queue* queue) {
    if (isEmpty(queue)) {
        printf("Queue is empty\n");
        return -1;
    }
    int item = queue->array[queue->front];
    queue->front++;
    return item;
}

void BFS(Graph* graph, int start,FILE *f) {
    int visited[N];
    for (int i = 0; i < N; i++) {
        visited[i] = 0;
    }

    Queue* queue = createQueue(N);
    enqueue(queue, start);
    visited[start] = 1;

    printf("BFS de la %d: ", start);

    while (!isEmpty(queue)) {
        int currentVertex = dequeue(queue);
        printf("%d ", currentVertex);
        fprintf(f,"%d,",currentVertex);

        Node* node = graph->head[currentVertex];
        while (node != NULL) {
            int adj = node->dest;
            if (!visited[adj]) {
                visited[adj] = 1;
                enqueue(queue, adj);
            }
            node = node->next;
        }
    }
}

int inTree[30];
int costTotal;


Edge edges2[30];
int counter=0;

void algoPrim(Graph* graph, Edge edges[], int nrEdges, int initial, FILE *f) {

    
    int visited[N];
    for ( int i = 0 ; i < N ; i++){
        visited[i]=0;
    }

    //primul nod vizitat
    visited[initial] = 1;

    int edgeCount = 0;
 
    while (edgeCount < N - 1) {
        int minCost = 99;
        int indexEdge = -1;

        //caut cost min cu un capat in tree
        for (int i = 0; i < nrEdges; i++) {
            if (!inTree[i] && ((visited[edges[i].src] && !visited[edges[i].dest]) || (visited[edges[i].dest] && !visited[edges[i].src]))) {
                if (edges[i].cost < minCost) {
                    minCost = edges[i].cost;
                    indexEdge= i;
                }
            }
        }

       
        if (indexEdge != -1) {
            inTree[indexEdge] = 1;
            printf("%d -> %d, Cost: %d\n", edges[indexEdge].src, edges[indexEdge].dest, edges[indexEdge].cost);
            edges2[counter]=edges[indexEdge];
            fprintf(f,"%d,", edges[indexEdge].cost);
            counter++;
            edgeCount++;
            costTotal=costTotal + edges[indexEdge].cost;

            visited[edges[indexEdge].src] = 1;
            visited[edges[indexEdge].dest] = 1;
        } else {
            break; // fin
        }
    }

}

int main(void) {

    FILE *f = fopen("nodPlecare_si_muchii.csv","r");


    Edge *edges = malloc(sizeof(Edge));
    int n=0;

  
    int nodInitial;
    fscanf(f,"%d",&nodInitial);

    char c;
    fscanf(f,"%d%c%d%c%d",&edges[n].src, &c , &edges[n].dest, &c , &edges[n].cost);
    n++;
  

    while(!feof(f)){
        edges = realloc(edges,sizeof(Edge)*(n+1));
        fscanf(f,"%d%c%d%c%d",&edges[n].src, &c , &edges[n].dest, &c , &edges[n].cost);
        n++;
       
    }
    n--;



    fclose(f);

    //int n = sizeof(Edge) * i;

    // Edge edges[] = {
    //     {0, 1, 4}, {0, 2, 2}, {0, 3, 5}, {0, 4, 1}, {1, 2, 3},
    //     {1, 5, 7}, {1, 6, 8}, {2, 3, 6}, {2, 7, 2}, {3, 4, 4},
    //     {3, 8, 3}, {4, 5, 5}, {4, 9, 6}, {5, 6, 1}, {5, 10, 4},
    //     {6, 7, 9}, {6, 11, 2}, {7, 8, 7}, {8, 9, 3}
    // };

    //int n = sizeof(edges) / sizeof(edges[0]);
   
    Graph* graph = createGraph(edges, n);

    //printGraph(graph);

    //Graph* graph2 = createGraph(edges, 0); //graf pt arbore

    sortEdges(edges, n);

    printf("arbore minim al grafului: ");


    f = fopen("Rezultate.csv","w+");


    algoPrim(graph,edges,n, 0,f);
  
    printf("\n\n");

    printf("Cost total: %d", costTotal);
    printf("\n");


    Graph* graph2 = createGraph(edges2,counter);

    fprintf(f,"\n");

    
 

    BFS(graph2,0,f);
    printf("BFS pt ca asa e cel mai aproape de un arbore si nivelele sale\n\nSe considera doar o data la cost desi apar de 2 ori muchiile (e neorientat, dar functia folosita e si pt orientat)\n");
    
    printf("\n");


    fprintf(f,"\n");
    fprintf(f, "%d", costTotal);


    fclose(f);
    free(edges);


}
