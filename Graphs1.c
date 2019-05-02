// A C Program to demonstrate adjacency list 
// representation of graphs
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

// User defined library for graph
#include "structs.h"
#include "graph.h"
#include "Player.h"


// Driver program to test above functions
int main() {
    // create the graph
    int L, B;

    // Number of players playing the game
    int num_of_players;

    // boolean to determine if a new edge was added
    bool edgeAdded;

    // Owned box to each player
    int *owned_box;
    owned_box = (int*) malloc(2 * sizeof(int));

    printf("Enter size of the grid (in LxB format): ");
    scanf("%dx%d", &L, &B);

    printf("Enter the number of players: ");
    scanf("%d", &num_of_players);

    Player *P;

    P = Init_players(num_of_players);

    Graph* graph = createGraph(L, B);

    int src, dest;
    
    int choice;

    int player_chance = 0;

    loop:
    printf("\n1: Add Edge \n2: Print Graph \n3: EXIT \nEnter your choice (Player %d): ", (player_chance + 1));
    scanf("%d", &choice);

    switch (choice) {
        case 1: printf("Enter src: "); scanf("%d", &src);
                printf("Enter dest: "); scanf("%d", &dest);
                edgeAdded = addEdge(graph, src, dest);
                owned_box = BoxOwnership(graph, src, dest, player_chance, owned_box);

                for(int i = 0; i < 2; i++) {
                    (P+player_chance)->owned = ownVertexToPlayer(P[player_chance].owned, owned_box[i]);
                }

                break;

        case 2: printGraph(graph, P, num_of_players);
                break;

        case 3: exit(0);

        default: printf("Enter a valid choice....\n");
    }

    if(edgeAdded) {
        player_chance = (player_chance + 1) % num_of_players;
        edgeAdded = false;
    }
    goto loop;
    return 0;
}