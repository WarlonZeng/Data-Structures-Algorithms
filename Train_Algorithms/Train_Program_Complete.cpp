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
#define M_PI 3.1415926535897932

// Warlon Zeng N11183332

using namespace std;

// Extra Credit

class trainStopData {

	struct vertexInf {                 // Stores information for a vertex
		int dist;  // distance to vertex from the source
		string prev;  // previous node in BFS tree
	};

private:
	string stop_id;
	string stop_name;
	double stop_lat;
	double stop_lon;
	vector<trainStopData> train_data;
	unordered_map<string, vertexInf> adjMap;
	unordered_map < string, list<string> > adjList;

public:
	trainStopData();
	trainStopData(const string& id, const string& name, double lat, double lon)
		: stop_id(id), stop_name(name), stop_lat(lat), stop_lon(lon) {}
	string get_id()         const { return stop_id; }
	string get_stop_name()  const { return stop_name; }
	double get_latitude()   const { return stop_lat; }
	double get_longitude()  const { return stop_lon; }
	void initialize_map_and_objects();//unordered_map < string, list<string> >& adjList, vector<trainStopData>& train_data);
	void add_transfers();// unordered_map < string, list<string> >& adjList);
	void shortest_paths(string& starting_stop, string& last_stop);// unordered_map<string, list<string>>& adjList, string& starting_vertex, string& next_vertex);
	void print_shortest_path(string& last_stop);// string next_vertex, unordered_map<string, vertexInf>& adjMap, vector<trainStopData>& train_data);
	string find_stop_id_from_stop_name(string& name);
	string find_stop_id_from_coordinates(double& lat, double& lon);
	void print(int& i);
	void print_this(string& stop);
	void print_same_train_line(string& last_stop);
	void print_nearby_stations(double& lat, double& lon);
};

trainStopData::trainStopData() {
	initialize_map_and_objects();
	add_transfers();
}

// Fill up all the keys with the stop_ids from MTA_train_stop_data
void trainStopData::initialize_map_and_objects() {//unordered_map < string, list<string> >& adjList, vector<trainStopData>& train_data) {
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

		//while (getline(stopFile, line)){
		int pos1 = line.find_first_of(',');
		string stop_id = line.substr(0, pos1);
		int pos2 = line.find_first_of(',', pos1 + 1);
		int pos3 = line.find_first_of(',', pos2 + 1);
		string stop_name = line.substr(pos2 + 1, pos3 - pos2 - 1);
		int pos4 = line.find_first_of(',', pos3 + 1);
		int pos5 = line.find_first_of(',', pos4 + 1);
		double stop_lat = (stod(line.substr(pos4 + 1, pos5 - pos4 - 1)));
		int pos6 = line.find_first_of(',', pos5 + 1);
		double stop_lon = (stod(line.substr(pos5 + 1, pos6 - pos5 - 1)));
		train_data.push_back(trainStopData(stop_id, stop_name, stop_lat, stop_lon));

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
void trainStopData::add_transfers() {//unordered_map < string, list<string> >& adjList) {
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

double degrad(double d) { return d * M_PI / 180; }

double haverdist_kilometers(double lat1, double longit1, double lat2, double longit2) {
	double r = 6371;
	double dlat = degrad(lat2 - lat1);
	double dlongit = degrad(longit2 - longit1);
	double a = sin(dlat / 2)*sin(dlat / 2) + cos(degrad(lat1))*cos(degrad(lat2))*sin(dlongit / 2)*sin(dlongit / 2);
	double c = 2 * atan2(sqrt(a), sqrt(1 - a));
	return r*c; // kilometers
}

double haverdist_miles(double lat1, double longit1, double lat2, double longit2) {
	double r = 3958.756;
	double dlat = degrad(lat2 - lat1);
	double dlongit = degrad(longit2 - longit1);
	double a = sin(dlat / 2)*sin(dlat / 2) + cos(degrad(lat1))*cos(degrad(lat2))*sin(dlongit / 2)*sin(dlongit / 2);
	double c = 2 * atan2(sqrt(a), sqrt(1 - a));
	return r*c; // miles
}

// from previous assignments 

void trainStopData::print(int& i) {
	cout << "Stop ID: " << train_data[i].get_id() << ", Stop Name: " << train_data[i].get_stop_name()
		<< ", Latitude: " << train_data[i].get_latitude() << ", Longitude: " << train_data[i].get_longitude() << endl;
}


void trainStopData::print_this(string& stop) {
	for (int i = 0; i < train_data.size(); i++)
		if (stop == train_data[i].get_id())
			print(i);
}


void trainStopData::print_same_train_line(string& last_stop) {
	for (int i = 0; i < train_data.size(); i++) {
		if ((train_data[i].get_id())[0] == last_stop[0]) // 1 = 112 example
			print(i);
	}
}


void trainStopData::print_nearby_stations(double& lat, double& lon) {
	string unit;
	double range;
	cout << "Find nearby stations by miles or kilometers?: ";
	cin >> unit;
	cout << "Within what radius?: ";
	cin >> range;
	if (unit == "m" || unit == "miles")
		for (int i = 0; i < train_data.size(); i++) {
			if (haverdist_miles(lat, lon, train_data[i].get_latitude(), train_data[i].get_longitude()) <= range)
				print(i);
		}
	else
		for (int i = 0; i < train_data.size(); i++)
			if (haverdist_kilometers(lat, lon, train_data[i].get_latitude(), train_data[i].get_longitude()) <= range)
				print(i);
}


string trainStopData::find_stop_id_from_stop_name(string& name) {
	for (int i = 0; i < train_data.size(); i++)
		if (name == train_data[i].get_stop_name())
			return train_data[i].get_id();
}


string trainStopData::find_stop_id_from_coordinates(double& lat, double& lon) {
	for (int i = 0; i < train_data.size(); i++)
		if (lat == train_data[i].get_latitude() && lon == train_data[i].get_longitude())
			return train_data[i].get_id();
}


void trainStopData::shortest_paths(string& start_stop, string& last_stop) {//unordered_map<string, list<string>>& adjList, string& starting_vertex, string& next_vertex) { // Modify the shortest path algorithm, shortestpaths, so that it now computes the shortest path to go from one stop_id to another stop_id and uses the graph you created.
	// i changed void to return an unordered_map
	// changed back to void, edits into class member variable instead
	queue<string> vertices;
	//unordered_map<string, vertexInf> adjMap;

	for (auto& keys : adjList){ // using auto to iterate through the data, can also use iterator, 
		adjMap[keys.first].prev = ""; // initialize strings by leaving them blank
		adjMap[keys.first].dist = DEFAULT_VAL; // initialize default values
	}

	vertices.push(start_stop);
	adjMap[start_stop].dist = 0;

	while (!vertices.empty()) {
		string station = vertices.front();
		vertices.pop();
		for (list<string>::iterator itr = adjList[station].begin(); itr != adjList[station].end(); ++itr) { // using help of pointers to iterate and assign
			if (adjMap[*itr].dist == DEFAULT_VAL) {
				adjMap[*itr].prev = station;
				adjMap[*itr].dist = adjMap[station].dist + 1;
				if (*itr != last_stop)
					vertices.push(*itr);
				else
					break;
			}
		}
	}
}

void trainStopData::print_shortest_path(string& last_stop) {//string next_vertex, unordered_map<string, vertexInf>& adjMap, vector<trainStopData>& train_data) {
	stack<string> vertices; // container to fill in stations
	stack<string> vertex_names;
	stack<double> lats;
	stack<double> longs;

	string station = last_stop;

	while (adjMap[station].dist != 0){ // cannot be DEFAULT_VAL
		vertices.push(station);
		station = adjMap[station].prev; // initialize starting stop 
	}

	cout << "Starting stop: " << station << endl;

	for (int i = 0; i < train_data.size(); i++)
		if (train_data[i].get_id() == station) {
			vertex_names.push(train_data[i].get_stop_name());
			lats.push(train_data[i].get_latitude());
			longs.push(train_data[i].get_longitude());
			break;
		}

	double tempLat = lats.top();
	double tempLong = longs.top();
	double total_meters = 0;
	double total_miles = 0;

	double startLat = tempLat;
	double startLong = tempLong;

	int stops_traveled = 0;
	string last_stop_name;

	//vector<trainStopData>::iterator itr_begin = train_data.begin();
	//vector<trainStopData>::iterator itr_end = train_data.end();
	//vector<trainStopData>::iterator itr;

	while (!vertices.empty()){
		for (int i = 0; i < train_data.size(); i++)
			//itr = lower_bound(itr_begin, itr_end, vertices.top()); // does not work because object != string const T& val comparison is imcompatitable
			//int i = itr - itr_begin;
			if (train_data[i].get_id() == vertices.top()) {
				vertex_names.push(train_data[i].get_stop_name());
				lats.push(train_data[i].get_latitude());
				longs.push(train_data[i].get_longitude());
				break;
			}
		if (vertices.size() == 1) {
			cout << "Last stop: " << vertices.top() << endl;
			last_stop_name = vertex_names.top();
		}
		else
			cout << "Next stop: " << vertices.top() << endl;

		cout << "Destination name: " << vertex_names.top() << endl;
		cout << "Latitude coordinate: " << lats.top() << endl;
		cout << "Longitude coordinate: " << longs.top() << endl;

		double meters = haverdist_kilometers(tempLat, tempLong, lats.top(), longs.top());
		double miles = haverdist_miles(tempLat, tempLong, lats.top(), longs.top());
		total_meters += meters;
		total_miles += miles;
		stops_traveled++;

		cout << "Stops traveled: " << stops_traveled << endl;
		cout << "Distance to travel to next stop: " << meters << " kilometers/ " << miles << "miles" << endl << endl;

		tempLat = lats.top();
		tempLong = longs.top();

		vertices.pop();
		vertex_names.pop();
		lats.pop();
		longs.pop();
	}

	double displacement_meters = haverdist_kilometers(startLat, startLong, tempLat, tempLong);
	double displacement_miles = haverdist_miles(startLat, startLong, tempLat, tempLong);

	cout << "------------------------------------Summary------------------------------------" << endl << endl;
	cout << "Last stop: " << last_stop_name << endl;
	cout << "Stops traveled: " << stops_traveled << endl;
	cout << "Distance traveled: " << total_meters << " kilometers/" << total_miles << " miles" << endl;
	cout << "Displacement: " << displacement_meters << " kilometers/" << displacement_miles << " miles" << endl;

}


int main() { // files required: mta and transfers txts

	trainStopData train_data;
	train_data.add_transfers();

	string start_stop;
	string last_stop;
	char input[100];
	string stop;
	double lat;
	double lon;
	double range;
	string choice;
	

	cout << "Choose an option: " << endl;
	cout << " 1. To find shortest path between two stops" << endl;
	cout << " 2. To find shortest path between two stop names" << endl;	
	cout << " 3. To find shortest path between two coordinates" << endl;
	cout << " 4. List all information on that stop" << endl;
	cout << " 5. List all stops on that line" << endl;
	cout << " 6. List all the train stops within that radius" << endl;
	cout << ">> ";
	//choice = cin.get();
	getline(cin, choice);
	char ch(choice[0]);

	switch (ch) {
	case('1') : {
		cout << "Enter starting stop id: ";
		cin >> start_stop;
		cout << "Enter final stop id: ";
		cin >> last_stop;
		cout << endl;
		train_data.shortest_paths(start_stop, last_stop);
		train_data.print_shortest_path(last_stop);
		break;
	}
	case('2') : {
		cout << "Enter starting stop name: ";
		getline(cin, stop);
		start_stop = train_data.find_stop_id_from_stop_name(stop);
		cout << "Enter final stop name: ";	
		getline(cin, stop);
		last_stop = train_data.find_stop_id_from_stop_name(stop);
		cout << endl;
		train_data.shortest_paths(start_stop, last_stop);
		train_data.print_shortest_path(last_stop);
		break;
	}
	case('3') : {
		cout << "Enter starting latitude coordinate: ";
		cin >> lat;
		cout << "Enter starting longitude coordinate: ";
		cin >> lon;
		start_stop = train_data.find_stop_id_from_coordinates(lat, lon);
		cout << "Enter final latitude coordinate: ";
		cin >> lat;
		cout << "Enter final longitude coordinate: ";
		cin >> lon;
		last_stop = train_data.find_stop_id_from_coordinates(lat, lon);
		cout << endl;
		train_data.shortest_paths(start_stop, last_stop);
		train_data.print_shortest_path(last_stop);
		break;
	}
	case('4') : {
		cout << "Enter stop id: ";
		cin >> stop;
		cout << endl;
		train_data.print_this(stop);
		break;
	}
	case('5') : {
		cout << "Enter stop id: ";
		cin >> stop;
		cout << endl;
		train_data.print_same_train_line(stop);
		break;
	}
	case('6') : {
		cout << "Enter latitude: ";
		cin >> lat;
		cout << "Enter longitude: ";
		cin >> lon;
		cout << endl;
		train_data.print_nearby_stations(lat, lon);
	}
	}

	cin.get();
	cin.get();
}