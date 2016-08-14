// CS2134 Fall 2015
// HW9 Programming Solution

#include <iostream>
#include <unordered_map>
#include <list>
#include <fstream>
#include <sstream>
#include <vector>
#include <queue>
#include <stack>
#include <algorithm>
using namespace std;

////////////////////////////////////////////////////////////////////////////////////
// This is code from the previous homework.                                       //
////////////////////////////////////////////////////////////////////////////////////

class trainStopData {
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

// This is for testing purposes, to print the adjacency list
void printList(const list<string>& ls) {
	for (list<string>::const_iterator a = ls.begin(); a != ls.end(); a++)
		cout << *a << " ";
	cout << endl;
}

////////////////////////////////////////////////////////////////////////////////////
// The following code is from Lisa Hellerstein, and modified by Wells Santo for   //
// the purpose of CS2134, Fall 2015.                                              //
////////////////////////////////////////////////////////////////////////////////////

const int DEFAULT_VAL = -1;
typedef unordered_map< string, list<string> > Graph; // Updated by Wells to make use of the unordered map from HW8

													 // Each stop in the graph should be represented by a vertexInf
struct vertexInf {
	vertexInf(int d = DEFAULT_VAL, string p = "") : dist(d), prev(p) {} // I added a default constructor
	int dist;
	string prev;   // Updated from int to string, since we're keeping track of stop id's
};

void printpath(const string& start, const string& end, const unordered_map<string, vertexInf>& vinfo) {
	cout << "Starting StopID: " << start << endl;
	cout << "Ending StopID:   " << end << endl;
	cout << "Distance:        " << vinfo.find(end)->second.dist << " transfers" << endl;
	cout << "Route:           ";

	// Use a stack to go from the ending stop to the starting stop
	// This is because our ending stop keeps track of its predecessor
	// and we want to print the list from start to end, not end to start
	stack<string> path;
	string current = end;
	while (current != start) {
		path.push(current);
		current = vinfo.find(current)->second.prev;
	}

	// Since we don't add the starting stop to the path, let's print it first
	cout << start;

	// Now print the rest of the path
	while (!path.empty()) {
		cout << " -> " << path.top();
		path.pop();
	}
	cout << endl;
}

void shortestpaths(const Graph& g, const string& start, const string& end) {
	// Test if the stops are valid
	// Notice this updates the provided code by looking for the keys in the
	// unordered map, rather than checking indices in a vector
	if (g.find(start) == g.end()) {
		cout << "The start train ID does not exist!" << endl;
		return;
	}

	if (g.find(end) == g.end()) {
		cout << "The end train ID does not exist!" << endl;
		return;
	}

	// Create a map of vertices, which maps stop ID's to vertexInf's
	// This way, we can keep track of the distance/predecessor for each train stop
	unordered_map<string, vertexInf> vertices;

	// Iterate through the existing adjacency list and populate the map of vertexInf's
	for (Graph::const_iterator itr = g.begin(); itr != g.end(); itr++) {
		// Notice this will use the default constructor for the vertexInf
		vertices.insert({ itr->first, vertexInf() });
	}

	// Initialize the distance to the starting stop as 0
	vertices[start].dist = 0;

	// Recall that the algorithm requires using a queue to see what node we visit next
	// We will use a queue of strings (stopIDs)
	queue<string> q;

	// We initialize the algorithm by adding the starting stop to the queue of nodes to visit
	q.push(start);

	// While the queue is not empty (aka we still have nodes to visit), we will continue our algorithm
	while (!q.empty()) {

		// Keep track of what stop ID we are currently visiting
		string v = q.front();
		// Pop this front stop off of the queue, since we are visiting it now
		q.pop();

		// Now, we must make use of the adjacency list for this specific stop
		// We do this by accessing the adjacency list that we created from last homework
		// In this function, this means we access the Graph g

		// Iterate through the adjacencies to the stop we are currently visiting
		// We will add these adjacencies to the visiting queue, and update
		// their distance/predecessor information as well
		for (list<string>::const_iterator w = g.find(v)->second.begin(); w != g.find(v)->second.end(); w++) {
			// If we haven't seen this stop before, we need to update its vertexInf information
			string adjacentStop = *w;
			if (vertices[adjacentStop].dist == DEFAULT_VAL) {
				vertices[adjacentStop].dist = vertices[v].dist + 1;
				vertices[adjacentStop].prev = v;
				q.push(adjacentStop);
			}
		}
	}

	// Print the path from the starting station to the ending station
	printpath(start, end, vertices);
}

int main() {
	// Initialize and fill up the adjacency list
	unordered_map < string, list<string> > adjList;
	initializeMap(adjList);
	addTransfers(adjList);

	// Some extra stuff to test that our adjacency list works
	cout << "\nTest some adjacency listings:\n";
	cout << "125: ";
	printList(adjList.find("125")->second);
	cout << "D19: ";
	printList(adjList.find("D19")->second);
	cout << "635: ";
	printList(adjList.find("635")->second);
	cout << endl;

	// Now test the shortest path algorithm
	cout << "Test a bad stop ID:\n";
	shortestpaths(adjList, "not valid", "D19");

	cout << "\nTest the shortest distance between two stops:\n";
	shortestpaths(adjList, "125", "D19");

	cout << endl;
}