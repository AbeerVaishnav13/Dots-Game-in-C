#ifndef GRAPH_H

#define GRAPH_H

// Functions definition prototypes

// Functions for graph implementation
AdjListNode* newAdjListNode(int);
type assignNodeType(int, int, int);
Graph* createGraph(int, int);
void printNodeType(Graph*);
bool checkAdjacency(int, type, int, int);
bool alreadyConnected(AdjList*, int, int);
bool addEdge(Graph*, int, int);
void printGraph(Graph*, Player*, int);

 
// A utility function to create a new adjacency list node
AdjListNode* newAdjListNode(int dest) {
    AdjListNode* newNode = (AdjListNode*) malloc(sizeof(AdjListNode));
    newNode->dest = dest;
    newNode->next = NULL;
    return newNode;
}

// To assign type to a node
type assignNodeType(int H, int N, int i) {
    type temp;

    if (i == 0)
        temp = tl_corner;
    else if (i == H - 1)
        temp = tr_corner;
    else if (i == (N - H))
        temp = bl_corner;
    else if (i == N - 1)
        temp = br_corner;

    else if (i > 0 && i < H - 1)
        temp = t_edge;
    else if (i % H == 0 && i != (N - H))
        temp = l_edge;
    else if (i % H == H - 1 && i != H - 1 && i != N - 1)
        temp = r_edge;
    else if (i > (N - H) && i < N - 1)
        temp = b_edge;

    else
        temp = inside;

    return temp;
}
 
// A utility function that creates a graph of N vertices
Graph* createGraph(int H, int V) {
    Graph* graph = (Graph*) malloc(sizeof(Graph));
    graph->H = H;
    graph->V = V;
    graph->N = H*V;
 
    // Create an array of adjacency lists.  Size of 
    // array will be V
    graph->array = (AdjList*) malloc(graph->N * sizeof(AdjList));

    //
 
    // Initialize each adjacency list as empty by 
    // making adjNode as NULL
    int i;
    for (i = 0; i < graph->N; ++i) {
        graph->array[i].adjNode = NULL;
        graph->array[i].Type = assignNodeType(graph->H, graph->N, i);
    }
 
    return graph;
}

// Printing the type of the node
void printNodeType(Graph* graph) {
    int i;
    printf("\n");
    for(i = 0; i < graph->N; i++)
        printf("Node: %d is '%s' node\n", i, positions[graph->array[i].Type]);
}


// To Check adjacency of the nodes
bool checkAdjacency(int H, type Type, int src, int dest) {
    if (Type == tl_corner && (dest == src+1 || dest == (src + H))) {
        return true;
    }
    else if (Type == tr_corner && (dest == src-1 || dest == (src + H))) {
        return true;
    }
    else if (Type == bl_corner && (dest == src+1 || dest == (src - H))) {
        return true;
    }
    else if (Type == br_corner && (dest == src-1 || dest == (src - H))) {
        return true;
    }

    else if (Type == t_edge && (dest == src-1 || dest == src+1 || dest == (src + H))) {
        return true;
    }
    else if (Type == l_edge && (dest == (src - H) || dest == src+1 || dest == (src + H))) {
        return true;
    }
    else if (Type == r_edge && (dest == src-1 || dest == (src - H) || dest == (src + H))) {
        return true;
    }
    else if (Type == b_edge && (dest == src-1 || dest == src+1 || dest == (src - H))) {
        return true;
    }

    else if (Type == inside && (dest == src-1 || dest == src+1 || dest == (src - H) || dest == (src + H))) {
        return true;
    }

    return false;
}


// To check if some particult nodes are already connected
bool alreadyConnected(AdjList *array, int src, int dest) {
    int i;
    AdjListNode *srcNode = array[src].adjNode;

    while (srcNode) {
        if (srcNode->dest == dest)
            return true;
        else
            srcNode = srcNode->next;
    }

    return false;
}
 
// Adds an edge to an undirected graph
bool addEdge(Graph* graph, int src, int dest) {
    if(checkAdjacency(graph->H, graph->array[src].Type, src, dest)) {
        if(alreadyConnected(graph->array, src, dest)) {
            printf("\nThis edge already exists...\n\n");
            return false;
        }
        else {
            // Add an edge from src to dest.  A new node is 
            // added to the adjacency list of src.  The node
            // is added at the begining
            AdjListNode* newNode = newAdjListNode(dest);
            newNode->next = graph->array[src].adjNode;
            graph->array[src].adjNode = newNode;
         
            // Since graph is undirected, add an edge from
            // dest to src also
            newNode = newAdjListNode(src);
            newNode->next = graph->array[dest].adjNode;
            graph->array[dest].adjNode = newNode;

            return true;
        }
    }
    else {
        printf("\nInvalid move...\n");
        return false;
    }
}

/*
    PRINTED GRAPH LOOKS LIKE THIS DEPENDING UPON THE CONNECTED AND NON-CONNECTED DOTS
                                    +---+   +---+
                                        |   | 1 |
                                    +   +---+---+
                                    |       |
                                    +---+   +---+
                                    | 2 |       |
                                    +---+---+   +
*/

void printGraph(Graph *graph, Player *P, int num_of_players) {
    int n = 0;
    int i, j, k;
    bool connected = false;

    printf("\n");
    for (i = 0; i < graph->V; i++) {
        for (j = 0; j < graph->H; j++) {
            printf("+");
            int index = i*graph->H + j;
            AdjListNode *newNode = graph->array[index].adjNode;
            int ext_index = index + 1;
            while (newNode) {
                if (newNode->dest == ext_index)
                    connected = true;

                newNode = newNode->next;
            }

            if (connected)
                printf("---");
            else if (!connected)
                printf("   ");

            connected = false;
        }
        printf("\n");
        for (j = 0; j < graph->H; j++) {
            int index = i*graph->H + j;
            AdjListNode *newNode = graph->array[index].adjNode;
            int ext_index = index + graph->H;
            while (newNode) {
                if (newNode->dest == ext_index)
                    connected = true;

                newNode = newNode->next;
            }
        

            if(connected)
                printf("|");
            else if (!connected)
                printf(" ");

            bool index_owned = false;
            for(k = 0; k < num_of_players; k++) {
                VertexList *OwnedList = P[k].owned;
                // printf("entered the for loop\n");

                while(OwnedList != NULL) {
                    printf("entered the while loop\n");
                    if(OwnedList->LT_vertex == index) {
                        // printf("entered if condition\n");
                        printf(" %d ", i+1);
                        index_owned = true;
                        break;
                    }
                    else {
                        // printf("entered else condition\n");
                        OwnedList = OwnedList->next;
                    }
                }
            }
            if(index_owned == false)
                printf("   ");
            connected = false;
        }
        printf("\n");
    }
}

#endif
