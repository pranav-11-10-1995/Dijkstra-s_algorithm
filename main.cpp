#include<iostream>
#include<fstream>
#include<string>
#include<cmath>
#include<climits>
#define MAX_SIZE 8 /* Update as per number of nodes*/
using namespace std;

std::string vertexList[MAX_SIZE];
int adjacencyMatrix[MAX_SIZE][MAX_SIZE];
int top = -1;

void push(std::string x){
	if(top == MAX_SIZE-1){
		cout<<"error: stack overflow"<<endl;
		return;
	}
	vertexList[++top] = x;
}

void pop(){
	if(top == -1){
		cout<<"error: no element to pop"<<endl;
		return;
	}
	top--;
}

void vertexListCreation(){
	std::string no_of_nodes;
	ifstream theFile("graph.node.txt");
	std::string nodeName;
	int countVal =0;
	while(theFile>>nodeName){
		if(countVal == 2){
			no_of_nodes = nodeName;
		}
        else if(countVal > 2){
			push(nodeName);
		}
		countVal++;
	}

	cout<<"Vertex List:"<<endl;
	cout<<"Number of Nodes : "<<no_of_nodes<<endl;
	for(int i=0;i<=top;i++){
		cout<<vertexList[i]<<endl;
	}
}

void adjacencyMatrixCreation(){
	int adjacencyMatrixRowIndex = 0;
	int adjacencyMatrixColumnIndex = 0;
	int flip = 0;
	int countVal =0;
	ifstream theFile("graph.nets.txt");
	std::string txtContent, no_of_connections;
	while(theFile>>txtContent){
        if(countVal == 2){
			no_of_connections = txtContent;
		}

        if(flip == 0){
            for(int i=0;i<=top;i++){
                if(txtContent == vertexList[i]){
                    adjacencyMatrixRowIndex = i;
                    flip = 1;
                }
            }
        }
        else if(flip == 1){
            for(int i=0;i<=top;i++){
                if(txtContent == vertexList[i]){
                    adjacencyMatrixColumnIndex = i;
                    flip = 2;
                }
            }
        }
        else{
            if(txtContent != "weight" && txtContent != ":"){
                adjacencyMatrix[adjacencyMatrixRowIndex][adjacencyMatrixColumnIndex] = stoi(txtContent);
                adjacencyMatrix[adjacencyMatrixColumnIndex][adjacencyMatrixRowIndex] = stoi(txtContent);
                flip = 0;
            }
        }
        countVal++;
	}

	cout<<"\nAdjacency Matrix:"<<endl;
	cout<<"No. of connections : "<<no_of_connections<<endl;
	for(int i=0;i<MAX_SIZE;i++){
		for(int j=0;j<MAX_SIZE;j++){
			cout<<adjacencyMatrix[i][j]<<"\t";
		}
		cout<<endl;
	}
}

void dijkstraAlgorithm(){
    int distance_from_A[MAX_SIZE];
    int tempSum[MAX_SIZE];
    int visited[MAX_SIZE]; /* initially all nodes are unvisited : 0 indicates unvisited, once visited flip 0 to 1*/
    std::string prevVertex[MAX_SIZE];
    int minVal, minIndexVal;
    cout<<"\nDijkstra's algorithm"<<endl;
    /*Init*/
    for(int i=0;i<MAX_SIZE;i++){
            distance_from_A[i] = INT_MAX; /*consider the distance from node A to any other node as max value*/
            tempSum[i] = 0;
            visited[i] = 0;
    }
    distance_from_A[0] = 0; /* distance from A to A is 0 */
    prevVertex[0]="A";

    for(int j=0;j<MAX_SIZE;j++){
        minVal = INT_MAX;
        /*1) Find min value and min index from distance_from_A[] array*/
        for(int i=0;i<MAX_SIZE;i++){
            if(distance_from_A[i] < minVal && visited[i] == 0){
                minVal = distance_from_A[i];
                minIndexVal = i;
            }
        }
        cout<<"Next node to be considered: "<<vertexList[minIndexVal]<<endl;
        /*2) In adjacency matrix, go to the row of min index and add min value to that row*/
        for(int i=0;i<MAX_SIZE;i++){
            if(adjacencyMatrix[minIndexVal][i] > 0){
                tempSum[i] = adjacencyMatrix[minIndexVal][i] + minVal;
            }
        }

        /*3) Compare tempSum[] with distance_from_A[], if tempSum[] is lower (we can traverse through lesser distance), then update  distance_from_A[]*/
        /*4) Lock the adjacency matrix's column with column index min index value*/
        for(int i=0;i<MAX_SIZE;i++){
            if(tempSum[i]< distance_from_A[i] && tempSum[i] > 0){
                distance_from_A[i] = tempSum[i];
                prevVertex[i]= vertexList[minIndexVal]; /*minIndexVal denotes the node which we are currently in. after updating the distance, update the node through which shortest path passes*/
            }
            adjacencyMatrix[i][minIndexVal] = INT_MAX; /*we lock the column by INT_MAX so that this column is not used for min_value calculation in next iteration */
        }

        /*5) Once visited, flip 0 to 1 so that the corresponding node is not considered for next iteration to find min value*/
        visited[minIndexVal] = 1;

        /* print distance_from_A[] for each iteration */
        cout<<"\ndistances from A in each iteration"<<endl;
        for(int i=0;i<MAX_SIZE;i++){
            cout<<distance_from_A[i]<<"\t";
        }
    }
    cout<<"\n\nFinal output"<<endl;
    cout<<"Vertex"<<'\t'<<"shortest distance from A"<<'\t'<<"Previous Vertex"<<endl;
    for(int i=0;i<MAX_SIZE;i++){
        cout<<vertexList[i]<<'\t'<<distance_from_A[i]<<'\t'<<'\t'<<'\t'<<'\t'<<prevVertex[i]<<endl;
    }
}

int main(){
	vertexListCreation();
	adjacencyMatrixCreation();
	dijkstraAlgorithm();
	return 0;
}
