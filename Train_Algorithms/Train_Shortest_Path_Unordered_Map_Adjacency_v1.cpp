#include <iostream>
#include <fstream>
#include <string>
#include <cmath>
#include <new>
#include <vector>
#include <ctime>
#include <istream>
#include <algorithm>
#include <utility>
#include <functional>
#include <sstream>
#include <list>
#include <set>
#include <unordered_set>
#include <iterator>
#include <ostream>
#include <array>
#include <forward_list>
#include <stack>
#include <queue>
#include <map>
#include <exception>
#include <unordered_map>

// Warlon Zeng N11183332

using namespace std;

// PQ1 - using released homework solutions and files given.

class trainStopData { // FROM HWK 8 SOLUTIONS
public:
	trainStopData(const string& id, const string& name, double lat, double lon)
		: stop_id(id), stop_name(name), stop_lat(lat), stop_lon(lon) {}
	string get_id()         const { return stop_id; }
	string get_stop_name()  const { return stop_name; }
	double get_latitude()   const { return stop_lat; }
	double get_longitude()  const { return stop_lon; }
private:
	string stop_id;
	string stop_name;
	double stop_lat;
	double stop_lon;
};

// Fill up all the keys with the stop_ids from MTA_train_stop_data
void initializeMap(unordered_map < string, list<string> >& adjList) {
	// Open the MTA_train_stop_data.txt file
	ifstream stopFile("MTA_train_stop_data.txt");
	if (!stopFile) {
		cerr << "The file for train stop data cannot be found.\n";
		exit(1);
	}
	// Read the file line-by-line
	string line;
	getline(stopFile, line);

	// Note: I'm going to fill up a vector first, so that I can add adjacencies for stops on the same line
	// You don't have to do it this way, and this may not be the best approach
	vector<string> stops;

	while (getline(stopFile, line)) {
		// Get the stop_id, which is the first element on each line
		stringstream eachLine(line);
		string element;

		// We use comma-delimiting to get the stop_id, which is before the first comma
		// This will store the stop_id into element
		getline(eachLine, element, ',');

		// Get rid of the stops that have 'N' or 'S' after them
		// If you don't do this, it's totally fine, since it won't break the rest of our code

		// Note: I'm going to fill up a vector first, so that I can add adjacencies for stops on the same route
		// You don't have to do it this way, and this may not be the best approach
		if (element[element.size() - 1] != 'N' && element[element.size() - 1] != 'S')
			stops.push_back(element);
	}

	// Sort the vector of stops, so you ensure that all the stops on the same route are listed in order
	sort(stops.begin(), stops.end());

	// At this point, I have a vector that's sorted in order, so the stops on the same line that are one stop away 
	// from each other are listed together, in order.
	for (int i = 0; i < stops.size(); i++) {
		// Create the adjacency list for this stop
		list<string> adj;

		// For the first element, we can't check the element before us, so this is an edge case
		if (i == 0) {
			// Make sure that there's actually a next stop, and that the first character of that stop
			// is the same as the first character of this stop. If so, it's on the same route and we add
			// it to our adjacency list.
			if (stops.size() > 0 && stops[i][0] == stops[i + 1][0]) adj.push_back(stops[i + 1]);

			// For the last element, we can't check the element after us, so this is an edge case
		}
		else if (i == stops.size() - 1) {
			// Make sure that there's more than just one stop, and that the first character of the
			// last stop is the same as the first character of this stop (that means they're on the same route)
			if (stops.size() > 0 && stops[i - 1][0] == stops[i][0]) adj.push_back(stops[i - 1]);

			// Otherwise, check the element both before and after
		}
		else {
			// Add the previous stop if it's on the same route
			if (stops[i - 1][0] == stops[i][0]) adj.push_back(stops[i - 1]);

			// Add the following stop if it's on the same route
			if (stops[i][0] == stops[i + 1][0]) adj.push_back(stops[i + 1]);
		}

		// Insert this stop into the map
		adjList.insert({ stops[i], adj });
	}
	stopFile.close();
}

// Add the transfers to each adjacency list 
void addTransfers(unordered_map < string, list<string> >& adjList) {
	// Open the transfers.txt file
	ifstream transfersFile("transfers.txt");
	if (!transfersFile) {
		cerr << "The file for train stop data cannot be found.\n";
		exit(1);
	}
	// Read the file line-by-line
	string line;
	getline(transfersFile, line);
	while (getline(transfersFile, line)) {
		// Get the first two elements in each line
		stringstream eachLine(line);
		string fromStop;
		string toStop;
		// Get the from_stop_id
		getline(eachLine, fromStop, ',');
		// Get the to_stop_id
		getline(eachLine, toStop, ',');
		// Add the toStop to the adjacency list of the fromStop
		// Make sure that the two are different stops
		if (fromStop != toStop) adjList[fromStop].push_back(toStop);
	}
	transfersFile.close();
}

// end for importing code from homework solutions

// code obtained from file given & modified

const int DEFAULT_VAL = -1;      // must be less than 0

typedef vector<list<int> > Graph;        // The graph is given in an adjacency list.
// Vertices are indexed from 0 to V-1
// The indices of the vertices adjacent to vertex i
// are in the list Graph[i].
// Graph can be directed or undirected.

struct vertexInf {                 // Stores information for a vertex
	int dist;  // distance to vertex from the source
	string prev;  // previous node in BFS tree
};

// Breadth First Search
// The unweighted shortest path algorithm on the graph g, with vertex i as the source
// Prints the length (number of edges) of the shortest path from the source to every vertex
// in the graph

unordered_map<string, vertexInf> shortest_paths(unordered_map<string, list<string>>& adjList, string& starting_vertex, string& next_vertex) { // Modify the shortest path algorithm, shortestpaths, so that it now computes the shortest path to go from one stop_id to another stop_id and uses the graph you created.
	// i changed void to return an unordered_map
	queue<string> vertices;
	unordered_map<string, vertexInf> adjMap;

	for (auto& keys : adjList){ // using auto to iterate through the data, can also use iterator, 
		adjMap[keys.first].prev = ""; // initialize strings by leaving them blank
		adjMap[keys.first].dist = DEFAULT_VAL; // initialize default values
	}

	vertices.push(starting_vertex);
	adjMap[starting_vertex].dist = 0;

	while (!vertices.empty()) {
		string station = vertices.front();
		vertices.pop();
		for (list<string>::iterator begin = adjList[station].begin(); begin != adjList[station].end(); ++begin) { // using help of pointers to iterate and assign
			if (adjMap[*begin].dist == DEFAULT_VAL) {
				adjMap[*begin].prev = station;
				adjMap[*begin].dist = adjMap[station].dist + 1;
				if (*begin != next_vertex)
					vertices.push(*begin);
				else
					break;
			}
		}
	}

	return adjMap;
}

void print_path(string next_vertex, unordered_map<string, vertexInf>& adjMap) {
	stack<string> vertices; // container to fill in stations
	string station = next_vertex;

	while (adjMap[station].dist != 0){ // cannot be DEFAULT_VAL
		vertices.push(station);
		station = adjMap[station].prev; // initialize starting stop 
	}

	cout << "Starting stop: " << station << endl << endl;

	while (!vertices.empty()){
		if (vertices.size() == 1)
			cout << "Last stop: " << vertices.top() << endl << endl;
		else
			cout << "Next stop: " << vertices.top() << endl << endl;
		vertices.pop();
	}

	cout << "Distance traveled: " << adjMap[next_vertex].dist << " stations" << endl;
}

int main() { // files required: mta and transfers txts
	unordered_map < string, list<string> > adjList;
	initializeMap(adjList);
	addTransfers(adjList);

	// Some extra stuff to test that our code works // from HWK 8 SOLUTIONS. was pretesting solution code 
	//cout << "125: ";
	//printList(adjList.find("125")->second);

	//cout << "D19: ";
	//printList(adjList.find("D19")->second);

	//cout << "635: ";
	//printList(adjList.find("635")->second); 

	string vertex;
	string next_vertex;

	cout << "Please enter starting station: ";
	cin >> vertex;
	cout << "Please enter final station: ";
	cin >> next_vertex;

	cout << endl;
	unordered_map<string, vertexInf> adjMap = shortest_paths(adjList, vertex, next_vertex);
	print_path(next_vertex, adjMap);

	cin.get();
	cin.get();
}