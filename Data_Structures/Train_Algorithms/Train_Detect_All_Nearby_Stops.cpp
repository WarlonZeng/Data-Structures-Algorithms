// CS2134 Homework 3B Fall 2015
// Programming Question 1 Solution
#include <iostream>
#include <vector>
#include <fstream>
#include <sstream>
#include <math.h>       // need this for haverdist to work
using namespace std;

//////////////////////////////////////////////////////////////////////////////
//
// PROVIDED AND PREVIOUS CODE
//
/////////////////////////////////////////////////////////////////////////////

// The haverdist function and degrad (provided)
double degrad(double d) { return d * M_PI / 180; }

double haverdist(double lat1, double longit1, double lat2, double longit2) {
	double r = 6371;
	double dlat = degrad(lat2 - lat1);
	double dlongit = degrad(longit2 - longit1);
	double a = sin(dlat / 2)*sin(dlat / 2) + cos(degrad(lat1))*cos(degrad(lat2))*sin(dlongit / 2)*sin(dlongit / 2);
	double c = 2 * atan2(sqrt(a), sqrt(1 - a));
	return r*c;
}

// TrainStopData from HW2

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

//////////////////////////////////////////////////////////////////////////////
//
// NEW CODE FOR THIS QUESTION
//
/////////////////////////////////////////////////////////////////////////////

// The perform_if function
template <class Itr, class Pred, class Op>
int perform_if(Itr begin, Itr end, Pred pred, Op op) {
	// We need to keep track of how many times 'pred' evaluates to true
	int how_many = 0;
	// We need a loop to go through all elements in the collection
	// In this case, that's all train stops in the vector
	for (; begin != end; begin++) {
		// Notice that this part is taken directly from the PDF
		if (pred(*begin)) {
			op(*begin);
			how_many++;
		}
	}
	return how_many;
}

// The four functors:

class isStopOnRoute {
public:
	isStopOnRoute(char route) : route(route) {}
	// Will return true if the first char in rhs's id matches the route
	bool operator() (const trainStopData& rhs) {
		string stopRoute = rhs.get_id();
		return stopRoute[0] == route;
	}
private:
	char route;
};

class isSubwayStop {
public:
	isSubwayStop(const string& stopId) : stopId(stopId) {}
	// Will return true if rhs has the given stopId
	bool operator() (const trainStopData& rhs) {
		return rhs.get_id() == stopId;
	}
private:
	string stopId;
};

class isSubwayStopNearX {
public:
	isSubwayStopNearX(double longitude, double latitude, double d) : longitude(longitude), latitude(latitude), d(d) {}
	bool operator() (const trainStopData& rhs) {
		return (haverdist(latitude, longitude, rhs.get_latitude(), rhs.get_longitude())) <= d;
	}
private:
	double longitude;
	double latitude;
	double d;
};

class printTrainStopInfo {
public:
	void operator() (const trainStopData& rhs) {
		cout << "Stop ID: " << rhs.get_id() << ", Stop Name: " << rhs.get_stop_name() << ", Latitude: " << rhs.get_latitude()
			<< ", Longitude: " << rhs.get_longitude() << endl;
	}
};

void menu(vector<trainStopData>& tsd) {
	string choice;
	string search_key;
	vector<trainStopData>::iterator itrStart = tsd.begin();
	vector<trainStopData>::iterator itrEnd = tsd.end();

	while (true) {
		cout << "\nChoose an option or enter 0 to exit:\n";
		cout << " 1. Given a route, list all the stops on that route\n";
		cout << " 2. Given a subway stop id, print out its info\n";
		cout << " 3. Given the coordinates and a certain distance, list all the train stops within that distance\n";

		cout << "> ";
		cin >> choice;
		cin.ignore();
		printTrainStopInfo print;
		char ch = choice[0];

		switch (ch) {
		case('0'): { return; }
		case('1'): {
			cout << "Enter the name of the subway route: ";
			char route;
			cin >> route;
			isStopOnRoute compare(route);
			int amount = perform_if(itrStart, itrEnd, compare, print);
			if (amount == 0)
				cout << "Route Not Found!\n";
			break;
		}
		case('2'): {
			cout << "Enter the stop id: ";
			string id;
			cin >> id;
			isSubwayStop compare(id);
			int amount = perform_if(itrStart, itrEnd, compare, print);
			if (amount == 0)
				cout << "Stop Not Found!\n";
			break;
		}
		case('3'): {
			cout << "Enter latitude: ";
			double lat, lon, d;
			cin >> lat;
			cout << "Enter longitude: ";
			cin >> lon;
			cout << "Enter the distance: ";
			cin >> d;
			isSubwayStopNearX compare(lon, lat, d);
			int amount = perform_if(itrStart, itrEnd, compare, print);
			if (amount == 0)
				cout << "No Stops Found Within Distance Given!\n";
			break;
		}
		default: cout << "Menu option not found. Try again.\n\n";
		}
	}
	cout << endl;
}

//////////////////////////////////////////////////////////////////////////////
//
// MAIN(): NOTICE THAT THIS MAINLY JUST CALLS FUNCTIONS WRITTEN ABOVE
//
/////////////////////////////////////////////////////////////////////////////

int main() {
	vector<trainStopData> allTrains;
	parseTrainData(allTrains);
	menu(allTrains);
}