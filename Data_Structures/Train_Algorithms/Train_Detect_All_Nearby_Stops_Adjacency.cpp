#define _CRT_SECURE_NO_WARNINGS // this is relatively old, can do strcpy_s instead otherwise
#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <cstring>
#include <cstdlib>
#include <math.h>
#include <vector>
#include <functional>
#include <limits>
#include <list>
#define M_PI 3.14159265358979323846

using namespace std;

double degrad(double d) {
	return d * M_PI / 180;
}


// Code below is adapted from http://www.movable-type.co.uk/scripts/latlong.html
// FYI: That website has an applet that computes the haversine distance. 
// It also has a link that will show the locations on a map,
// with a line between them.
double haverdist(double lat1, double longit1, double lat2, double longit2)
{
	double r = 6371;
	double dlat = degrad(lat2 - lat1);
	double dlongit = degrad(longit2 - longit1);
	double a = sin(dlat / 2)*sin(dlat / 2) +
		cos(degrad(lat1))*cos(degrad(lat2))*sin(dlongit / 2)*sin(dlongit / 2);
	double c = 2 * atan2(sqrt(a), sqrt(1 - a));
	return r*c;
}


class train_stop_data
{
private:
	//101,,Van Cortlandt Park - 242 St,,40.889248,-73.898583,,,1,

	string stop_id;    // id of train stop (1st token)
	string stop_name;  // name of station (4th token)
	double stop_lat;   // latitude of train stop location
	double stop_lon;   // longitude of train stop location
	list<string> adjacent;
public:
	train_stop_data(string id, string name, double lat, double lon)
		: stop_id(id), stop_name(name), stop_lat(lat), stop_lon(lon) {};


	const string & get_id() const { return stop_id; }
	const string & get_stop_name() const { return stop_name; }
	double get_latitude() const { return stop_lat; }
	double get_longitude() const { return stop_lon; }
	void add_adjacent_stop(const string s) { adjacent.push_front(s); }
};

void tokenizer2(char s[], vector<string> & tokens, const char delim[])
{
	tokens.clear();
	char * pch;
	pch = strtok(s, delim);
	while (pch != NULL)
	{
		string temp = pch;
		tokens.push_back(pch);
		pch = strtok(NULL, delim);
	}
}
//------------------------
void load_transfer_data_from_transfer_file(vector<train_stop_data> & train_stops)
{
	vector<train_stop_data>::iterator itr = train_stops.begin();
	ifstream infile_transfers;
	infile_transfers.open("transfers.txt", ifstream::in);

	string s;
	char delim_comma[] = ",";
	vector<string> tokens;

	if (infile_transfers.is_open()) {
		getline(infile_transfers, s);  // toss away first line of info.
		while (!infile_transfers.eof())
		{
			getline(infile_transfers, s);
			//string s1 = "101,101,2,180";

			if (infile_transfers.eof()) { cout << "-" << s << "-\n"; break; }  // toss away blank last line of code

			char * cstr = new char[s.size() + 1]; // to make a cstyle string out of a c++ string, you need to know the size
			strcpy(cstr, s.c_str()); // tokenizer requires c style strings

			tokenizer2(cstr, tokens, delim_comma);
			if (tokens[0] != tokens[1])
			{       // Many different approaches can be used to find the correct train_stop_data
				while (itr != train_stops.end() && (*itr).get_id() != tokens[0])
					++itr;

				if ((*itr).get_id() == tokens[0])
					(*itr).add_adjacent_stop(tokens[1]);
			}
			delete[] cstr;
		}
	}
	else
	{
		cout << "Couldn't open file of transfers.\n";
	}

	infile_transfers.close();
	return;

}
//-----------------------
void enter_stop_data(vector<train_stop_data> & train_stops)
{
	ifstream infile_stops;
	infile_stops.open("trainData.txt", ifstream::in);

	string s;
	char delim_comma[] = ",";
	vector<string> tokens;

	string prev_stop_id;
	string current_stop_id = " ";

	if (infile_stops.is_open()) {
		getline(infile_stops, s);  // toss away first line of info.
		while (!infile_stops.eof())
		{
			getline(infile_stops, s);
			//string s1 = "101,,Van Cortlandt Park - 242 St,,40.889248,-73.898583,,,1,";

			if (infile_stops.eof()) { cout << "-" << s << "-\n"; break; }  // toss away blank last line of code

			char * cstr = new char[s.size() + 1]; // to make a cstyle string out of a c++ string, you need to know the size
			strcpy(cstr, s.c_str()); // tokenizer requires c style strings

			tokenizer2(cstr, tokens, delim_comma);
			train_stops.push_back(train_stop_data(tokens[0], tokens[1], atof(tokens[2].c_str()), atof(tokens[3].c_str())));
			delete[] cstr;


			prev_stop_id = current_stop_id;
			current_stop_id = tokens[0]; // this is not needed, but I added it for readability.
			if (prev_stop_id[0] == current_stop_id[0]) // The two stops have the same route name
			{
				vector<train_stop_data>::iterator itr = train_stops.end() - 1;
				(*itr).add_adjacent_stop(prev_stop_id);
				--itr;
				(*itr).add_adjacent_stop(current_stop_id);
			}
		}

	}
	else
		cout << "couldn't open file of routes\n";

	infile_stops.close();
	return;
}
//------------------------
ostream & operator<< (ostream & out, const train_stop_data & a)
{

	out << left << "Stop " << a.get_id() << " is located at " << a.get_stop_name()
		<< ". The cooordinates are ( " << a.get_latitude() << ", " << a.get_longitude()
		<< " )" << endl;

	return out;
};
//----------------------
class isStopOnRoute
{

public:
	isStopOnRoute(char rout) :route(rout) {}
	bool operator()(train_stop_data prama)
	{
		string thing = prama.get_id();
		char comp = thing[0];
		return comp == route;
	}
private:

	char route;


};


//------------------------
class PrintSubwayStopInfo
{
public:
	void operator()(train_stop_data & thisStop)
	{
		cout << thisStop;
	}
};
//------------------------
class isSubwayStop
{
private:
	string stopId;
public:
	isSubwayStop(string id) :stopId(id) {}
	bool operator()(const train_stop_data & train_stop)
	{
		return stopId == train_stop.get_id();
	}
};
//------------------------
class isSubwayStopNearX
{
private:
	double latatude;
	double longatude;
	double dist;

public:
	isSubwayStopNearX(double lat, double lon, double d) :
		latatude(lat), longatude(lon), dist(d) {}

	bool operator()(train_stop_data & train_stop)
	{
		return haverdist(latatude, longatude, train_stop.get_latitude(), train_stop.get_longitude()) <= dist;
	}
};
//-----------------------
template<class ForwardIterator, class Predicate>
ForwardIterator select_item(ForwardIterator start, ForwardIterator end, Predicate pred)
{
	bool flag = false;
	ForwardIterator tmp_itr;
	ForwardIterator final_true;
	for (tmp_itr = start; tmp_itr != end; tmp_itr++)
	{

		if (pred(*tmp_itr))
		{
			flag = true;
			final_true = tmp_itr;
		}
	}
	if (flag == true)
	{
		return final_true;
	}
	else
		return end;

}
//------------------------
class closest_stop
{

private:
	double longitude;
	double latitude;
	double shortest_dist;
public:
	closest_stop(double longi, double lat) :longitude(longi), latitude(lat), shortest_dist(numeric_limits<double>::max()) {}

	bool operator()(train_stop_data stop)
	{
		if (haverdist(stop.get_latitude(), stop.get_longitude(), latitude, longitude)<shortest_dist)
		{
			shortest_dist = haverdist(stop.get_latitude(), stop.get_longitude(), latitude, longitude);
			return true;
		}
		else
			return false;
	}

};
//---------------------------------------
template<class ForwardIterator, class Predicate, class Operator>
int perform_if(ForwardIterator start, ForwardIterator pastEnd, Predicate pred, Operator op)
{
	int howMany = 0;
	while (start != pastEnd)
	{
		if (pred(*start))
		{
			op(*start);
			howMany++;
		}
		start++;
	}
	return howMany;
}
//--------------------------
template<class ForwardIterator, class Functor>
bool is_sorted(ForwardIterator start, ForwardIterator pastEnd, Functor comp)
{
	ForwardIterator prev;
	ForwardIterator current;
	bool flag = false;
	for (prev = start, current = prev++; current != pastEnd; prev++, current++)
	{
		if (!comp(*prev, *current))
		{
			flag = true;
		}
	}
	if (flag)
		return false;
	else
		return true;
}
//----------------------
class stop_idIsLessThan
{
public:
	bool operator()(train_stop_data previous, train_stop_data curr)
	{
		return previous.get_id() <= curr.get_id();
	}

};
//------------------------
void menu(vector<train_stop_data> & train_stops)
{
	int choice;
	string search_key;
	vector<train_stop_data>::const_iterator itr;

	while (true)
	{
		cout << "Choose an option or enter 0 to exit" << endl;
		cout << " Option 1. Given a subway stop id, return the location" << endl;
		cout << " Option 2. Given a route, list all the stops on that route" << endl;
		cout << " Option 3. Find all subway stations near longitude and latitude coordinates" << endl;
		cin >> choice;
		cin.clear();
		cin.ignore(numeric_limits<streamsize>::max(), '\n');

		switch (choice)
		{
		case(0): {return; }
		case(1):
		{
			cout << "Enter the stop id: ";
			getline(cin, search_key);
			isSubwayStop StopId(search_key);
			if (!perform_if(train_stops.begin(), train_stops.end(), StopId, PrintSubwayStopInfo()))
			{
				cout << search_key << "has no stop." << endl;
			}
			break;
		}
		case(2):
		{
			cout << "Enter the name of the subway route: ";
			getline(cin, search_key);
			isStopOnRoute  RouteStop(search_key[0]);
			cout << "The stops on subway route " << search_key[0] << ":" << endl;
			if (!perform_if(train_stops.begin(), train_stops.end(), RouteStop, PrintSubwayStopInfo()))
			{
				cout << "No stops were found on route " << search_key << "." << endl;
			}
			break;
		}
		case(3):
		{
			double longitude;
			double latitude;
			double dist;
			cout << "Enter the coordinates: ";
			cin >> longitude >> latitude;
			cout << "Enter the distance in kilometers from the coordinates: ";
			cin >> dist;
			cout << "The stops " << dist << " kilometers from (" << longitude << ", " << latitude << "):\n";
			isSubwayStopNearX IsNearby(longitude, latitude, dist);
			if (!perform_if(train_stops.begin(), train_stops.end(), IsNearby, PrintSubwayStopInfo()))
			{
				cout << "No stops found\n";
			}

			break;
		}
		case(4):
		{
			double longitude;
			double latitude;

			cout << "Enter the coordinates: ";
			cin >> longitude >> latitude;

			closest_stop select_stop(longitude, latitude);

			itr = select_item(train_stops.begin(), train_stops.end(), select_stop);
			if (itr == train_stops.end())
				cout << "Error: couldn't find any stops " << endl;
			else
			{
				cout << "The closest stop to coordinates (" << longitude << ", " << latitude << "):\n";
				cout << *itr;
			}
			break;
		}
		default: cout << "Menu option not found.Try again." << endl;
		}
		cout << endl;
	}
}
//------------------------------

int main()
{
	vector<train_stop_data> train_stops;
	enter_stop_data(train_stops); // enter initial data
	load_transfer_data_from_transfer_file(train_stops); // load and incorporate additional (transfer) data
	menu(train_stops);
	return 0;
}