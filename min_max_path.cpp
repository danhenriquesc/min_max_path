#include <iostream>
#include <limits.h>
#include <vector>
#include <random>

using namespace std;
 
#define V 250
const double EulerConstant = std::exp(1.0);

string type;
int N;
int start, endd;
int Graph[V][V];

vector<int> solution, bestSolution, currentSolution;


//MIN CASE
int minDistance(int dist[], bool sptSet[])
{
    int min = INT_MAX, min_index;
 
    for (int v = 0; v < V; v++)
        if (sptSet[v] == false && dist[v] <= min)
            min = dist[v], min_index = v;
 
    return min_index;
}
 
void printPath(int parent[], int j, int size)
{
    // Base Case : If j is source
    if (parent[j]==-1){
    	cout << size << " (" << start << ",";
        return;
    }

    printPath(parent, parent[j], ++size);
 
    cout << j;

    if(j == endd){
    	cout << ") ";
    }else{
    	cout << ",";
    }
}
 
void dijkstra(int graph[V][V], int src)
{
    int dist[V]; 
    bool sptSet[V];
 
    int parent[V];
 
    for (int i = 0; i < V; i++)
    {
        parent[0] = -1;
        dist[i] = INT_MAX;
        sptSet[i] = false;
    }
 
    dist[src] = 0;
 
    for (int count = 0; count < V-1; count++)
    {
        int u = minDistance(dist, sptSet);
 
        sptSet[u] = true;
 
        for (int v = 0; v < V; v++)
 
            if (!sptSet[v] && graph[u][v] &&
                dist[u] + graph[u][v] < dist[v])
            {
                parent[v]  = u;
                dist[v] = dist[u] + graph[u][v];
            }  
    }
 
    printPath(parent, endd, 1);
    cout << dist[endd] << endl;
}

//MAX CASE
int fitness(vector<int> s){
	int cost = 0;

	for(int i=0; i<N-1;i++){
		cost += Graph[s[i]][s[i+1]];
	}

	if(s[0] != start) cost -= INFINITY;
	if(s[N-1] != endd) cost -= INFINITY;

	return cost;
}

void initSolution(){
	for(int i=0; i<N;i++){
		solution.push_back(i);
	}
}

void swap(){
	int i, j;

	random_device rd;     // only used once to initialise (seed) engine
	mt19937 rng(rd());    // random-number engine used (Mersenne-Twister in this case)
	uniform_int_distribution<int> uni(0,N-1); // guaranteed unbiased

	currentSolution = solution;

	i = -1;
	j = -1;
	
	while(i == j){
		i = uni(rng);
		j = uni(rng);
	}

	int tmp = currentSolution[i];
	currentSolution[i] = currentSolution[j];
	currentSolution[j] = tmp;
}

void SA(){

	int nIter = 0;
	int maxIter = 1000;
	double T = 1;
	int delta;
	double alfa = 0.90;
	double x;
	int sg, sm, ss;

	int accepted = 0;
	double perc = 0;

	initSolution();
	bestSolution = solution;

	ss = fitness(solution);
	sm = fitness(bestSolution);

	int itTotal = 0;

	while(T>0.001){
		accepted = 0;

		while(nIter < maxIter){
			nIter++;
			itTotal++;

			swap();

			sg = fitness(currentSolution);

			delta = sg - ss;

			if(delta > 0){
				solution = currentSolution;
				ss = sg;

				if(sg > sm){
					bestSolution = currentSolution;
					sm = sg;
				}

				accepted++;
			}else{
				x = ((double) rand() / (RAND_MAX));

				if(x < pow(EulerConstant, (double) (-delta)/T)){
					solution = currentSolution;
					ss = sg;
					accepted++;
				}
			}
			perc = (accepted*100)/nIter;
			cout << "It: " << itTotal << "; Temp: " << T << "; BestFitness: "<<sm << endl;
		}

		T = T * alfa;

		nIter = 0;
	}

	cout << N << " (";
	
	for(int i = 0; i<N; i++){
		cout << bestSolution[i] << " ";
	}
	
	cout << ") " << sm << endl << endl;;
}

int main(){
	cin >> type;
	cin >> N;
	cin >> start >> endd;

	for(int i=0;i<N;i++){
		for(int j=0;j<N;j++){
			cin >> Graph[i][j];
		}		
	}

	if(type == "min")
		dijkstra(Graph, start);
	else
		SA();
}