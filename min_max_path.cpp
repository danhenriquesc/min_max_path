#include <iostream>
#include <limits.h>
#include <vector>
#include <random>

using namespace std;
 
#define V 1000
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
	int i = 0;

	if(s[0] != -1){
		cost += Graph[start][s[i]];

		// cout << "fit " << i << endl;
		while(s[i+1] != -1){
			// if(s[i] >=N || s[i+1] >= N){
			// 	cout << s[i] << " | " << s[i+1] << " | " << i << " | " << N << endl;

			// 	for(int j=0;j<N;j++){
			// 		cout << s[j] << " ";
			// 	}
			// 	cout << endl;

			// }
			cost += Graph[s[i]][s[i+1]];
			i++;
		}
		// cout << "fit " << i << " end" << endl;

		cost += Graph[s[i]][endd];
	}else{
		cost += Graph[start][endd];
	}

	// cout << "fit " << i << " end cost" << endl;

	return cost;
}

void initSolution(){
	int tmp;

	for(int i=0; i<N;i++){
		if(i != start && i != endd){
			solution.push_back(i);
		}
	}
	solution.push_back(-1);
}

void swap(){
	int i, j;

	random_device rd;     // only used once to initialise (seed) engine
	mt19937 rng(rd());    // random-number engine used (Mersenne-Twister in this case)
	uniform_int_distribution<int> uni(0,N-2); // guaranteed unbiased

	currentSolution = solution;

	i = -1;
	j = -1;
	
	while(i == j){
		i = uni(rng);
		j = uni(rng);
	}

	// cout << "a = " << i << endl;
	int tmp = currentSolution[i];
	// cout << "b = " << i << " | " << j << endl;
	currentSolution[i] = currentSolution[j];
	// cout << "c = " << j << endl;
	currentSolution[j] = tmp;
	// cout << "d" << endl;
}

void SA(){

	int nIter = 0;
	int maxIter = 100000;
	double T = 3;
	int delta;
	double alfa = 0.99;
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
			cout << "It: " << itTotal << "; Temp: " << T << "; BestFitness: "<<sm << "; Current: " << sg << endl;

			// cout << N << " ( " << start << " ";
	
			// int i = 0;	
			// while(currentSolution[i] != -1){
			// 	cout << currentSolution[i] << " ";
			// 	i++;
			// }
			
			// cout << endd << " ) " << sg << endl << endl;
		}

		T = T * alfa;

		nIter = 0;
	}

	cout << N << " ( " << start << " ";
	
	int i = 0;	
	while(bestSolution[i] != -1){
		cout << bestSolution[i] << " ";
		i++;
	}
	
	cout << endd << " ) " << sm << endl << endl;
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
