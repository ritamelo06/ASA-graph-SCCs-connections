#include <iostream>
#include <vector>
#include <stack>
#include <list>

using namespace std;

enum Color { WHITE, GREY, BLACK };
//white- not visited
//grey - being visited
//black - visited

class Graph {

public:
    int vertices;
    vector<vector<int>> adjacencyList;

    Graph(int v) : vertices(v), adjacencyList(v + 1) {}

    void addConnection(int u, int v) {
        adjacencyList[u].push_back(v);
    }

    vector<int> DFS() {
        vector<Color> color(vertices + 1, WHITE);
        stack<int> vertexStack;
        vector<int> orderedVertices(vertices, 0);             // list to store vertices in descending order of time
        int numV = vertices - 1;

        for (int v = 1; v <= vertices; v++) {
            if (color[v] == WHITE) {
                vertexStack.push(v);

                while (!vertexStack.empty()) {
                    int currentVertex = vertexStack.top();
                    vertexStack.pop();

                    if (color[currentVertex] == WHITE) {
                        color[currentVertex] = GREY;
                        vertexStack.push(currentVertex);

                        for (int adjacentVertex : adjacencyList[currentVertex]) {
                            if (color[adjacentVertex] == WHITE) 
                                vertexStack.push(adjacentVertex);
                        }
                    } 
                    else if (color[currentVertex] == GREY) {
                        color[currentVertex] = BLACK;
                        orderedVertices[numV] = currentVertex;
                        numV--;
                    }
                }
            }
        }
        return orderedVertices;
    }
    
    int calculateMaxJump(Graph tpGraph){
        // do DFS in original graph
        vector<int> orderedVertices = DFS();                 // start DFS at vertex 1
        
        vector<Color> color(vertices + 1, WHITE);
        stack<int> vertexStack;
        vector<int> dp(vertices + 1, 0);                     // stores the max jumps for each subproblem (each vertex)   
        vector<int> vertex_scc(vertices + 1, 0);

        int maxJump = 0;
        int num_scc = 0;

        for (auto it = orderedVertices.rbegin(); it != orderedVertices.rend(); ++it) {
            const int& v = *it;
            num_scc ++;
            vertexStack.push(v);
            
            
            if (color[v] == WHITE) {
    
                while (!vertexStack.empty()) {
                    int currentVertex = vertexStack.top();
                    vertex_scc[currentVertex] = num_scc;

                    if (color[currentVertex] == WHITE) {
                        color[currentVertex] = GREY;

                        for (int adjacentVertex : this->adjacencyList[currentVertex]) {
                            if (color[adjacentVertex] == WHITE){
                                vertexStack.push(adjacentVertex);
                                vertex_scc[adjacentVertex] = num_scc;
                            }
                            if(vertex_scc[adjacentVertex] != vertex_scc[currentVertex])
                                dp[currentVertex] = max(dp[currentVertex], dp[adjacentVertex] + 1);
                            else{
                                dp[currentVertex] = max(dp[currentVertex], dp[adjacentVertex]);
                                dp[adjacentVertex] = dp[currentVertex];
                            }
                        }
                        maxJump = max(maxJump, dp[currentVertex]); 
                        
                    }

                    else if (color[currentVertex] == GREY) {
                        color[currentVertex] = BLACK;
                        vertexStack.pop();
                    }

                    else if (color[currentVertex] == BLACK) {
                        vertexStack.pop();
                    }
                }
            }        
        }         
        return maxJump;
    }
};



int main(){
    int numInd, numConnect;                                      // reads graph information
    scanf("%d %d", &numInd, &numConnect);                        // num_ind -> num of vertices; num_connect -> num of edges
    int maxJump = 0;

    if(numInd < 2 || numConnect < 0) {                           // checks arguments
        printf("%d\n", maxJump);
    }
    else{
        Graph TugaNet(numInd);                                   // create graph
        Graph transposeTugaNet(numInd);                          // create transpose graph
        
        for (int i = 0; i < numConnect; i++){          
            int u, v;
            scanf("%d %d", &u, &v);
            TugaNet.addConnection(u, v);                         // adds the edges to the graph
            transposeTugaNet.addConnection(v, u);                // adds reverse edges to transpose graph
        } 
        
        maxJump = TugaNet.calculateMaxJump(transposeTugaNet);
        printf("%d\n", maxJump);
    }
    return 0;
}