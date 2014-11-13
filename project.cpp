#include <cstdio>
#include <iostream>
#include <stdlib.h>
#include <vector>
#include <algorithm>
#include <math.h>
#include <queue>
#include <stack>
using namespace std;

typedef pair<int, double> id;
typedef pair<double, double> point;
typedef vector<int> vi;
typedef vector<id> vid;
typedef vector<vid> graph;

#define PI acos(-1.0)
#define EARTH_RAD (6371009) 

//-----------------------------------------
//	Helper Methods Signatures
//-----------------------------------------

double dist(point node1, point node2);
double greatCircleDistance(point node1, point node2);
double computeTourDistance(vi tour, vector<point> nodeList);
vi mutateTour(vi tour);
void print_vector(vi v);

//-----------------------------------------
//	Main Methods
//-----------------------------------------

// Returns a random graph in the form of an adjacency list
graph randomGraph(int n, double D, double p)
{
	vector<point> nodePosition;
	nodePosition.resize(n);

	for (int i = 0; i < n; ++i)
	{
		nodePosition[i].first = (double)rand() / RAND_MAX * D;
		nodePosition[i].second = (double)rand() / RAND_MAX * D;
	}

	graph randomResult; randomResult.resize(n);
	for (int i = 0; i < n; ++i)
	{
		for (int j = i + 1; j < n; ++j)
		{
			double d = dist(nodePosition[i],nodePosition[j]);
			//printf("%d %d %lf %lf %lf %lf %lf\n", i, j, nodePosition[i].first, nodePosition[i].second, nodePosition[j].first, nodePosition[j].second, d);
			randomResult[i].push_back(make_pair(j,d));
			randomResult[j].push_back(make_pair(i,d));
			
		}
	}
	return randomResult;
}

vector<point> randomGraphNodeList(int n, double D, double p)
{
	vector<point> nodePosition;
	nodePosition.resize(n);

	for (int i = 0; i < n; ++i)
	{
		nodePosition[i].first = (double)rand() / RAND_MAX * D;
		nodePosition[i].second = (double)rand() / RAND_MAX * D;
	}

	return nodePosition;
}

vi twoOptAlgorithm(vector<point> nodeList, int runTime){
	int startTime = time(NULL);
	int V = (int)nodeList.size();
	
	// Start with a tour from 0 to V-1
	vi tour;
	for(int i = 0; i < V; i++){
		tour.push_back(i);
	}

	double bestDistance = computeTourDistance(tour, nodeList);
	while(time(NULL) - startTime < runTime){
		//printf("here\n");
		vi currentTour = mutateTour(tour);
		for(int i = 0; i < V; i++){
  			for(int j = i+1; j < V; j++){
  				vi new_tour;
  				for(int k = 0; k < i; k++){
  					new_tour.push_back(currentTour[k]);
  				}
  				for(int k = i; k <= j; k++){
  					new_tour.push_back(currentTour[j-(k-i)]);
  				}
  				for(int k = j+1; k < V; k++){
  					new_tour.push_back(currentTour[k]);
  				}
  			//	print_vector(currentTour);
  			//	print_vector(new_tour);
  				double newDist = computeTourDistance(new_tour, nodeList);
			//	printf("%d\n", newDist);
  			//	printf("%lf\n", newDist);
  				if(newDist < bestDistance){
  					tour = new_tour;
  					bestDistance = newDist;
           			//print_vector(tour);
  				}
  			}
  		}
	}
	return tour;
}

vi nearestNeighbourHeuristic(graph adjList){
	int V = (int)adjList.size();
	vi tour;
	vi selected;
	selected.assign(V, 0);
	tour.push_back(0);
	selected[0] = 1;
	for(int i = 1; i < V; i++){
		int prev = tour[i-1];
		//printf("%d\n", prev);
		double best = 999999999;
		int bestIndex = 0;
		//print_vector(selected);
		for(int j = 0; j < adjList[prev].size(); j++){
			int node = adjList[prev][j].first;
			double currentWeight = adjList[prev][j].second;
			//printf("%d %lf\n", node, currentWeight);
			if(selected[node] == 0 && currentWeight < best){
				best = currentWeight;
				bestIndex = node;
			}
		}
		selected[bestIndex] = 1;
		tour.push_back(bestIndex);
	}
	return tour;
}

graph findMST(graph adjList) {
	graph MST;
	priority_queue<pair<double,pair<int,int> > > pq;
	vector<bool> visited;

	MST.resize(adjList.size());
	visited.assign(adjList.size(),false);
	pq.push(make_pair(0,make_pair(0,-1)));

	while (!pq.empty()) {
		double dist = pq.top().first;
		int vertex_now = pq.top().second.first;
		int vertex_prev = pq.top().second.second;
		pq.pop();
		if (visited[vertex_now]) continue;
		visited[vertex_now] = true;
		if (vertex_prev != -1) {
			MST[vertex_now].push_back(make_pair(vertex_prev,dist));
			MST[vertex_prev].push_back(make_pair(vertex_now,dist));
		}
		for (int i = 0; i < adjList[vertex_now].size(); ++i) {
			double dist = adjList[vertex_now][i].second;
			int vertex_next = adjList[vertex_now][i].first;
			pq.push(make_pair(dist,make_pair(vertex_next,vertex_now)));
		}
	}
	return MST;
}

vi twoApproxAlgorithm(graph adjList){
	vector<bool> visited;
	vector<int> tour;
	stack<int> DFS; //let's do DFS without recursion, shall we

	visited.assign(adjList.size(),false);
	graph MST = findMST(adjList);
	DFS.push(0);

	while (!DFS.empty()) {
		int vertex_now = DFS.top();
		DFS.pop();
		if (visited[vertex_now]) continue;
		visited[vertex_now] = true;
		tour.push_back(vertex_now);
		for (int i = 0; i < MST[vertex_now].size(); ++i) {
			int vertex_next = MST[vertex_now][i].first;
			DFS.push(vertex_next);
		}
	}

	return tour;
}

void outputGraphToFile(string fileName, vector<point> nodeList) 
{
	freopen(fileName.c_str(), "w", stdout);	
	for (int i = 0; i < nodeList.size(); ++i)
	{
		printf("%.6lf %.6lf\n",nodeList[i].first,nodeList[i].second);
	}
}

vector<point> inputGraphFromFile(string fileName) 
{
	freopen(fileName.c_str(), "r", stdin);	
	vector<point> graphResult;
	double x,y;
	while (scanf("%lf %lf",&x,&y)) {
		graphResult.push_back(make_pair(x,y));
	}
	return graphResult;
}

void generateRandomGraphs(void)
{
	outputGraphToFile("data_random/random1.txt",randomGraphNodeList(100,100,1));
	outputGraphToFile("data_random/random2.txt",randomGraphNodeList(1000,100,1));
	outputGraphToFile("data_random/random3.txt",randomGraphNodeList(5000,100,1));
	outputGraphToFile("data_random/random4.txt",randomGraphNodeList(10000,1,1));
	outputGraphToFile("data_random/random5.txt",randomGraphNodeList(10000,1000,1));
}

int main()
{
	//freopen("output.txt", "w", stdout);
	srand (time(NULL)); // Randomize seed
	//randomGraph(10,2,1);
	//print_vector(twoOptAlgorithm(randomGraphNodeList(10, 100, 1),1));
	//print_vector(nearestNeighbourHeuristic(randomGraph(10, 2, 1)));
	//print_vector(twoApproxAlgorithm(randomGraph(10, 2, 1)));
	generateRandomGraphs();
	
}

//-----------------------------------------
//	Helper Methods
//-----------------------------------------

double dist(point node1, point node2)
{
	return sqrt((node1.first - node2.first)*(node1.first - node2.first) + (node1.second - node2.second)*(node1.second - node2.second));
}

double greatCircleDistance(point node1, point node2){
	double pLat = node1.first;
	double pLong = node1.second;
	double qLat = node2.first;
	double qLong = node2.second;
	double radius = EARTH_RAD;
	pLat *= PI / 180; pLong *= PI / 180;
  	qLat *= PI / 180; qLong *= PI / 180;
  	return radius * acos(cos(pLat)*cos(pLong)*cos(qLat)*cos(qLong) +
                       cos(pLat)*sin(pLong)*cos(qLat)*sin(qLong) +
                       sin(pLat)*sin(qLat));
}

double computeTourDistance(vi tour, vector<point> nodeList){
	double result = 0.0;
	int V = (int)nodeList.size();
	for(int i = 0; i < V-1; i++){
		result += greatCircleDistance(nodeList[tour[i]], nodeList[tour[i+1]]);
	}
	result += greatCircleDistance(nodeList[V-1], nodeList[0]);
	//printf("%lf\n", result);
	return result;
}

vi mutateTour(vi tour){
	int V = (int)tour.size();
	vi finalTour = tour;
	for(int tt = 0; tt < 2; tt++){
		vi newTour;
		int x = rand() % V;
		int y = rand() % V;
		if(x > y) swap(x,y);
		for(int k = 0; k < x; k++){
			newTour.push_back(finalTour[k]);
		}
		for(int k = x; k <= y; k++){
			newTour.push_back(finalTour[y-(k-x)]);
		}
		for(int k = y+1; k < V; k++){
			newTour.push_back(finalTour[k]);
		}
		finalTour = newTour;
	}
	// print_vector(finalTour);
	return finalTour;
}

void print_vector(vi v){
  printf("[");
  for(int i = 0; i < (int)v.size(); i++){
    printf("%d ", v[i]);
  }
  printf("]");
  printf("\n");
}