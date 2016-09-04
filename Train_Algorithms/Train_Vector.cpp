// CS2134 Homework 2 Fall 2015 Programming Solution

#include <iostream>
#include <vector>
#include <fstream>
#include <sstream>
using namespace std;

class trainStopData {
public:
	trainStopData(const string& id, const string& name, double lat, double lon)
		: stop_id(id), stop_name(name), stop_lat(lat), stop_lon(lon) {}
	string get_id() 		const { return stop_id; }
	string get_stop_name() 	const { return stop_name; }
	double get_latitude() 	const { return stop_lat; }
	double get_longitude() 	const { return stop_lon; }
private:
	string stop_id;
	string stop_name;
	double stop_lat;
	double stop_lon;
};

void parseTrainData(vector<trainStopData>& trainStops) {
	ifstream stopFile("MTA_train_stop_data.txt");
	if (!stopFile) {
		cerr << "The file for train stop data cannot be found.\n";
		exit(1);
	}
	string line;
	getline(stopFile, line);
	while (getline(stopFile, line)) {
		string element;
		stringstream eachLine(line);
		vector<string> allElements;
		while (getline(eachLine, element, ',')) { allElements.push_back(element); }
		double lat = atof(allElements[4].c_str());
		double lon = atof(allElements[5].c_str());
		trainStops.push_back(trainStopData(allElements[0], allElements[2], lat, lon));
	}
	stopFile.close();
}

// You didn't need to print the train data for the homework
void printTrainData(const vector<trainStopData>& trainStops) {
	for (int i = 0; i < trainStops.size(); i++) {
		cout << "Stop ID: " << trainStops[i].get_id() << "     | Stop Name: " << trainStops[i].get_stop_name() << endl;
		cout << "Latitude: " << trainStops[i].get_latitude() << " | Longitude: " << trainStops[i].get_longitude() << endl << endl;
	}
}

int main() {
	vector<trainStopData> allTrains;
	parseTrainData(allTrains);
	printTrainData(allTrains);
}