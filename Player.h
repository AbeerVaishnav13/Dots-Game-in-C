#ifndef PLAYER_H

#define PLAYER_H

// Functions for player stats
Player* Init_players(int);
VertexList* ownVertexToPlayer(VertexList*, int);
type RecentConnectedType(type, type);
bool AdjacentEdgesConnected(Graph*, int, int);
Player* BoxOwnership(Player*, Graph*, int, int, int);


// Function to initialize the players of the game
Player* Init_players(int num_of_players) {
    Player *P;

    P = (Player*) malloc(num_of_players * sizeof(Player));

    int i;
    for(i = 0; i < num_of_players; i++) {
        (P + i)->owned = NULL;
    }

    return P;
}


// Function to initialize the vertex list for each player for box ownership
VertexList* ownVertexToPlayer(VertexList *List, int vertex_index) {
    VertexList *newNode;
    newNode = (VertexList*) malloc(sizeof(VertexList));
    newNode->LT_vertex = vertex_index;
    newNode->next = NULL;

    if(List != NULL) {
        newNode->next = List;
    }

    printf("Current list for the player: ");
    while(newNode) {
    	printf("%d ", newNode->LT_vertex);
    	newNode = newNode->next;
    }
    printf("\n");

    return newNode;
}

// Function to return type of the recently connected vertices
type RecentConnectedType(type srcNodeType, type destNodeType) {
    type RCT;

     if(srcNodeType == t_edge || destNodeType == t_edge) {
        if(srcNodeType == tl_corner || destNodeType == tl_corner || (srcNodeType == t_edge && destNodeType == t_edge))
            RCT = t_edge;
    }

    else if(srcNodeType == b_edge || destNodeType == b_edge) {
        if(srcNodeType == br_corner || destNodeType == br_corner || (srcNodeType == b_edge && destNodeType == b_edge))
            RCT = b_edge;
    }

    else if(srcNodeType == l_edge || destNodeType == l_edge) {
        if(srcNodeType == bl_corner || destNodeType == bl_corner || (srcNodeType == l_edge && destNodeType == l_edge))
            RCT = l_edge;
    }

    else if(srcNodeType == r_edge || destNodeType == r_edge) {
        if(srcNodeType == tr_corner || destNodeType == tr_corner || (srcNodeType == r_edge && destNodeType == r_edge))
            RCT = r_edge;
    }

    else
        RCT = inside;


    return RCT;
}


bool* AllAdjacentEdgesConnected(Graph *graph, int src, int dest, bool *AllConnected) {
    type srcNodeType = graph->array[src].Type,  destNodeType = graph->array[dest].Type;

    type RCT = RecentConnectedType(srcNodeType, destNodeType);

    bool srcConn[] = {false, false}, destConn[] = {false, false}, interConn[] = {false, false};
    AllConnected[0] = AllConnected[1] = false;

    AdjListNode *srcNode, *destNode, *newNode[2];
    srcNode = graph->array[src].adjNode;
    destNode = graph->array[dest].adjNode;

    int Offset1, Offset2;

    if(RCT == t_edge) {
        newNode[0] = graph->array[src + graph->H].adjNode;
        Offset1 = graph->H;
    } 

    else if(RCT == r_edge) {
        newNode[0] = graph->array[src - 1].adjNode;
        Offset1 = -1;
    } 

    else if(RCT == l_edge) {
        newNode[0] = graph->array[src + 1].adjNode;
        Offset1 = 1;
    } 

    else if(RCT == b_edge) {
        newNode[0] = graph->array[src - graph->H].adjNode;
        Offset1 = -(graph->H);
    }

    else if(RCT == inside) {
        if(abs(src - dest) == graph->H) { // connected vertically
            newNode[0] = graph->array[src + 1].adjNode;
            newNode[1] = graph->array[src - 1].adjNode;
            Offset1 = -1;
            Offset2 = 1;
        }

        else if(abs(src - dest) == 1) { // connected horizontally
            newNode[0] = graph->array[src + graph->H].adjNode;
            newNode[1] = graph->array[src - graph->H].adjNode;
            Offset1 = graph->H;
            Offset2 = -(graph->H);
        }
    }


    if(RCT == t_edge || RCT == l_edge || RCT == r_edge || RCT == b_edge) {
        while(srcNode || destNode || newNode[0]) {
            if(srcNode->dest == (src + Offset1)) {
                srcConn[0] = true;
            }

            if(destNode->dest == (dest + Offset1)) {
                destConn[0] = true;
            }

            if(newNode[0]->dest == (dest + Offset1)) {
                interConn[0] = true;
            }

            if(srcConn[0] && destConn[0] && interConn[0]) {
                AllConnected[0] = true;
                break;
            }

            srcNode = srcNode->next;
            destNode = destNode->next;
            newNode[0] = newNode[0]->next;
        }
    }

    else if(RCT == inside) {
        while(srcNode || destNode || newNode[0] || newNode[1]) {
            if(srcNode->dest == (src + Offset1)) {
                srcConn[0] = true;
                if(srcNode->dest == (src + Offset2))
                    srcConn[1] = true;
            }

            if(destNode->dest == (dest + Offset1)) {
                destConn[0] = true;
                if(destNode->dest == (dest + Offset2))
                    destConn[1] = true;
            }

            if(newNode[0]->dest == (dest + Offset1)) {
                interConn[0] = true;
                if(newNode[1]->dest == (dest + Offset2))
                    interConn[1] = true;
            }

            srcNode = srcNode->next;
            destNode = destNode->next;
            newNode[0] = newNode[0]->next;
            newNode[1] = newNode[1]->next;
        }

        if(srcConn[0] && destConn[0] && interConn[0])
            AllConnected[0] = true;
        if(srcConn[1] && destConn[1] && interConn[1])
            AllConnected[1] = true;
    }

    return AllConnected;
}

// Function to store box ownership for each player
Player* BoxOwnership(Player *P, Graph *graph, int src, int dest, int player_chance) {
    bool *AllAdjEdgesConnected;
    AllAdjEdgesConnected = (bool*) malloc(2 * sizeof(bool));

    type srcNodeType = graph->array[src].Type, destNodeType = graph->array[dest].Type;

    AllAdjEdgesConnected = AllAdjacentEdgesConnected(graph, src, dest, AllAdjEdgesConnected);

    type RCT = RecentConnectedType(srcNodeType, destNodeType);

    int owned_box;

    if((RCT == t_edge || RCT == l_edge) && AllAdjEdgesConnected[0]) {
        if(src < dest)
            P[player_chance].owned = ownVertexToPlayer(P[player_chance].owned, src);
        else if(dest < src)
            P[player_chance].owned = ownVertexToPlayer(P[player_chance].owned, dest);
    }

    else if(RCT == r_edge && AllAdjEdgesConnected[0]) {
        if(src < dest)
            P[player_chance].owned = ownVertexToPlayer(P[player_chance].owned, src - 1);
        else if(dest < src)
            P[player_chance].owned = ownVertexToPlayer(P[player_chance].owned, dest - 1);
    }

    else if(RCT == b_edge && AllAdjEdgesConnected[0]) {
        if(src < dest)
            P[player_chance].owned = ownVertexToPlayer(P[player_chance].owned, src - graph->H);
        else if(dest < src)
            P[player_chance].owned = ownVertexToPlayer(P[player_chance].owned, dest - graph->H);
    }

    else if(RCT == inside) {
    	if(abs(src - dest) == 1) {
	        if(AllAdjEdgesConnected[0] && !AllAdjEdgesConnected[1]) {
        		if(src < dest)
            		P[player_chance].owned = ownVertexToPlayer(P[player_chance].owned, src);
        		else if(dest < src)
            		P[player_chance].owned = ownVertexToPlayer(P[player_chance].owned, dest);
	        }

	        else if(!AllAdjEdgesConnected[0] && AllAdjEdgesConnected[1]) {
	        	if(src < dest)
            		P[player_chance].owned = ownVertexToPlayer(P[player_chance].owned, src - graph->H);
        		else if(dest < src)
            		P[player_chance].owned = ownVertexToPlayer(P[player_chance].owned, dest - graph->H);
	        }

	        else if(AllAdjEdgesConnected[0] && AllAdjEdgesConnected[1]) {
	        	if(src < dest) {
            		P[player_chance].owned = ownVertexToPlayer(P[player_chance].owned, src);
            		P[player_chance].owned = ownVertexToPlayer(P[player_chance].owned, src - graph->H);
	        	}
        		else if(dest < src) {
            		P[player_chance].owned = ownVertexToPlayer(P[player_chance].owned, dest);
            		P[player_chance].owned = ownVertexToPlayer(P[player_chance].owned, dest - graph->H);
        		}
	        }
    	}
    }

    return P;
}

#endif