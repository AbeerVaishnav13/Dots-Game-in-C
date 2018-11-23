#ifndef STRUCTS_H

#define STRUCTS_H

// Enumerated datatype for defining type of each node in the graph
typedef enum T {tl_corner = 0, tr_corner, bl_corner, br_corner, l_edge, t_edge, r_edge, b_edge, inside}type;

// String representation of type of node in the graph (just included for debugging)
const char *positions[] = {"tl_corner", "tr_corner", "bl_corner", "br_corner", "l_edge", "t_edge", "r_edge", "b_edge", "inside"};

// Enumerated datatype for declaring boolean expressions
typedef enum B {false = 0, true}bool;

// A structure to represent an adjacency list node
typedef struct ALN {
    int dest;
    struct ALN *next;
}AdjListNode;
 
// A structure to represent an adjacency list
typedef struct AL {
    AdjListNode *adjNode; 
    type Type;
}AdjList;
 
// A structure to represent a graph. A graph
// is an array of adjacency lists.
// Size of array will be N (number of vertices 
// in graph)
typedef struct G {
    int H;
    int V;
    int N;
    AdjList* array;
}Graph;

// List to store left top vertex of a particular player
typedef struct VL {
    int LT_vertex;
    struct VL *next;
}VertexList;

// Structure to store ownership of box to player
typedef struct P {
    VertexList *owned;
}Player;

#endif