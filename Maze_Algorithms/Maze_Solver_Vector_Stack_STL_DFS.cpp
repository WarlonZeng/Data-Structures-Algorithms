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

// Warlon Zeng N11183332

using namespace std;

enum { START = 's', EXIT = 'e', PASSAGE = '.', USED = 'O', DOWN = 'D', UP = 'U', LEFT = 'L', RIGHT = 'R', ANY = 'A' };

void open_input_file(ifstream& ifs, string& file_name){ // standard file opener, now with recursion style!
	cout << "Please enter maze name: ";
	cin >> file_name;
	file_name = file_name + ".txt";
	ifs.open(file_name);
	if (!ifs) {
		cout << "Bad filename" << endl;
		ifs.clear();
		open_input_file(ifs, file_name);
	}
	cout << endl;
}

string get_algorithm_choice() {
	string method;
	cout << "Available methods to solve the maze: " << endl;
	cout << endl;
	cout << "1. Vector" << endl;
	cout << "2. Stack" << endl;
	cout << endl;
	cout << "How would you like to solve the maze?: ";
	cin >> method;
	cout << endl;
	if (method != "1" && method != "vector" && method != "Vector"
		&& method != "2" && method != "stack" && method != "Stack") {
		cout << "Invalid choice!" << endl << endl;
	}
	return method;
}


class maze {
public:
	ifstream ifs; // initialized
	//ofstream strategy_file; // derived initialized
	//ofstream solution_file; // derived initialized
	vector<vector<char>> the_maze; // iniitialized
	vector<vector<char>> saved_maze; // derived initialized
	string file_name; // initialized
	string strategy_by_method; // derived initialized
	string solution_by_method; // derived initialized
	int x_loc; // initialized
	int y_loc; // initialized
	int x_save; // derived initialized
	int y_save; // derived initialized
	char split_move;
	char crash_move;

	string return_file_name() {
		return file_name;
	}

	string return_strategy_by_method() {
		return strategy_by_method;
	}
	
	string return_solution_by_method() {
		return solution_by_method;
	}

	void initialize_maze() {
		string line;
		vector<char> temp;
		open_input_file(ifs, file_name);
		while (ifs >> line) {
			for (int i = 0; i < line.size(); i++)
				temp.push_back(line[i]);
			the_maze.push_back(temp);
			temp.clear();
		}
		ifs.close();
	}

	void initialize_start_coordinates() {
		for (int x = 0; x < the_maze.size(); x++)
			for (int y = 0; y < the_maze[0].size(); y++)
				if (the_maze[x][y] == START) {
					x_loc = x;
					y_loc = y;
				}
	}

	maze() { initialize_maze(); initialize_start_coordinates(); } // constructor

	void print_strategy(ofstream& strategy_file) { // work print
		strategy_file.open(strategy_by_method + file_name, ios::app);
		for (int i = 0; i < the_maze.size(); i++) {
			for (int j = 0; j < the_maze[0].size(); j++) {
				strategy_file << the_maze[i][j];
			}
			strategy_file << endl;
		}
		strategy_file << endl;
	}

	void print_maze() { // solution print
		ofstream solution_file;
		solution_file.open(solution_by_method + file_name, ios::app);
		for (int i = 0; i < the_maze.size(); i++) {
			for (int j = 0; j < the_maze[0].size(); j++) {
				solution_file << the_maze[i][j];
			}
			solution_file << endl;
		}
		solution_file << endl;
	}

	void reset_files() {
		ofstream strategy_file;
		ofstream solution_file;
		strategy_file.open(strategy_by_method + file_name, ofstream::trunc);
		solution_file.open(solution_by_method + file_name, ofstream::trunc);
	}

	void cout_maze(){
		for (int i = 0; i < the_maze.size(); i++) {
			for (int j = 0; j < the_maze[0].size(); j++) {
				cout << the_maze[i][j];
			}
			cout << endl;
		}
		cout << endl;
	}

	void save() {
		saved_maze.clear();
		vector<char> temp;
		for (int x = 0; x < the_maze.size(); x++) {
			for (int y = 0; y < the_maze[0].size(); y++)
				temp.push_back(the_maze[x][y]);
			saved_maze.push_back(temp);
			temp.clear();
		}
	}

	void reload() {
		the_maze = saved_maze;
	}

	char here(int &a, int &b) {
		return (the_maze[a][b]);
	}

};


class maze_vector : public maze { // higher classing
public:

	maze_vector() : maze() {
		strategy_by_method = "[Strategy by vector] ";
		solution_by_method = "[Solution by vector] ";
	} // constructor

	bool up_open() {
		if (x_loc == 0)
			return false;
		return ((the_maze[x_loc - 1][y_loc] == PASSAGE) || (the_maze[x_loc - 1][y_loc] == EXIT));
	}

	bool down_open() {
		if (x_loc == the_maze.size() - 1)
			return false;
		return ((the_maze[x_loc + 1][y_loc] == PASSAGE) || (the_maze[x_loc + 1][y_loc] == EXIT));
	}

	bool left_open() {
		if (y_loc == 0)
			return false;
		else
			return ((the_maze[x_loc][y_loc - 1] == PASSAGE) || (the_maze[x_loc][y_loc - 1] == EXIT));
	}

	bool right_open() {
		if (y_loc == the_maze[0].size() - 1)
			return false;
		return ((the_maze[x_loc][y_loc + 1] == PASSAGE) || (the_maze[x_loc][y_loc + 1] == EXIT));

	}
	void go_up() {
		x_loc--;
	}

	void go_down() {
		x_loc++;
	}

	void go_left() {
		y_loc--;
	}

	void go_right() {
		y_loc++;
	}

	void mark_path() {
		the_maze[x_loc][y_loc] = USED;
	}

	bool two_way() {
		if (up_open())
			if ((down_open() || left_open() || right_open()))
				return true;
		if (down_open())
			if ((up_open() || left_open() || right_open()))
				return true;
		if (left_open())
			if ((up_open() || down_open() || right_open()))
				return true;
		if (right_open())
			if ((up_open() || down_open() || left_open()))
				return true;
	}


	void return_to_save_point_vector() {
		x_loc = x_save;
		y_loc = y_save;
	}

	bool got_fucked() {
		return (!up_open() && !down_open() && !left_open() && !right_open());
	}

	bool success(){
		return (the_maze[x_loc][y_loc] == EXIT);
	}

	void recursive_vector() {
		ofstream strategy_file;
		//ofstream solution_file;
		if (here(x_loc, y_loc) == START) {
			reset_files();
		}

		if (two_way()) { // base case 1
			x_save = x_loc;
			y_save = y_loc;
			save();
			print_strategy(strategy_file);
			strategy_file.close();
			if (down_open() && crash_move != DOWN) {
				go_down();
				split_move = DOWN;
				crash_move = NULL;
				if (success()) {
					print_maze();
					return;
				}
				mark_path();
				recursive_vector();
			}

			if (up_open() && crash_move != UP) {
				go_up();
				split_move = UP;
				crash_move = NULL;
				if (success()){
					print_maze();
					return;
				}
				mark_path();
				recursive_vector();
			}

			if (left_open() && crash_move != LEFT) {
				go_left();
				split_move = LEFT;
				crash_move = NULL;
				if (success()){
					print_maze();
					return;
				}
				mark_path();
				recursive_vector();
			}

			if (right_open() && crash_move != RIGHT) {
				go_right();
				split_move = RIGHT;
				crash_move = NULL;
				if (success()){
					print_maze();
					return;
				}
				mark_path();
				recursive_vector();
			}
		}

		if (got_fucked()) { // base case 2
			if (success()) {
				return;
			}
			print_strategy(strategy_file);
			crash_move = split_move;
			return_to_save_point_vector();
			reload();
			recursive_vector();
		}

		if (down_open()) {
			go_down();
			print_strategy(strategy_file);
			if (success()){
				print_maze();
				return;
			}
			mark_path();
			recursive_vector();
		}

		if (up_open()) {
			go_up();
			print_strategy(strategy_file);
			if (success()){
				print_maze();
				return;
			}
			mark_path();
			recursive_vector();
		}

		if (left_open()) {
			go_left();
			print_strategy(strategy_file);
			if (success()){
				print_maze();
				return;
			}
			mark_path();
			recursive_vector();
		}

		if (right_open()) {
			go_right();
			print_strategy(strategy_file);
			if (success()){
				print_maze();
				return;
			}
			mark_path();
			recursive_vector();
		}
	}
};


class maze_stack : public maze { // x_loc, y_loc = initialized; x_save, y_save = uninitialized; saved_maze = initialized; file_name = initialized; the_maze = initialized;
public:
	stack<int> row;
	stack<int> column;

	void initialize_start_coordinates() {
		row.push(0);
		column.push(0);
		while (row.top() != x_loc) {
			if (row.top() > x_loc)
				row.pop();
			else
				row.push(row.top() + 1);
		}
		while (column.top() != y_loc) {
			if (column.top() > y_loc)
				column.pop();
			else
				column.push(column.top() + 1);
		}
	}

	maze_stack() : maze() {
		strategy_by_method = "[Strategy by stack] ";
		solution_by_method = "[Solution by stack] ";
		initialize_start_coordinates();
	}

	bool up_open() {
		if (row.top() == 0)
			return false;
		return ((the_maze[row.top() - 1][column.top()] == PASSAGE) || (the_maze[row.top() - 1][column.top()] == EXIT)); // can be done 
	}

	bool down_open() {
		if (row.top() == the_maze.size() - 1)
			return false;
		return ((the_maze[row.top() + 1][column.top()] == PASSAGE) || (the_maze[row.top() + 1][column.top()] == EXIT));
	}

	bool left_open() {
		if (column.top() == 0)
			return false;
		else
			return ((the_maze[row.top()][column.top() - 1] == PASSAGE) || (the_maze[row.top()][column.top() - 1] == EXIT));
	}

	bool right_open() {
		if (column.top() == the_maze[0].size() - 1)
			return false;
		return ((the_maze[row.top()][column.top() + 1] == PASSAGE) || (the_maze[row.top()][column.top() + 1] == EXIT));
	}

	void go_up() {
		row.pop();
	}

	void go_down() {
		row.push(row.top() + 1);
	}

	void go_left() {
		column.pop();
	}

	void go_right() {
		column.push(column.top() + 1);
	}

	bool two_way() {
		if (up_open())
			if ((down_open() || left_open() || right_open()))
				return true;
		if (down_open())
			if ((up_open() || left_open() || right_open()))
				return true;
		if (left_open())
			if ((up_open() || down_open() || right_open()))
				return true;
		if (right_open())
			if ((up_open() || down_open() || left_open()))
				return true;
	}

	bool got_fucked() {
		return (!up_open() && !down_open()
			&& !left_open() && !right_open());
	}

	void return_to_save_point() {
		while (row.top() != x_save) {
			if (row.top() > x_save)
				row.pop();
			else
				row.push(row.top() + 1);
		}
		while (column.top() != y_save) {
			if (column.top() > y_save)
				column.pop();
			else
				column.push(column.top() + 1);
		}
	}

	bool success(){
		return (the_maze[row.top()][column.top()] == EXIT);
	}

	void mark_path() {
		the_maze[row.top()][column.top()] = USED;
	}

	void stack_solve() {

		while (here(row.top(), column.top()) != EXIT) { // not on exit
			ofstream strategy_file;
			//ofstream solution_file;
			if (here(row.top(), column.top()) == START) {
				reset_files();
			}
			if (two_way()) { // base case 1, save pivot
				x_save = row.top();
				y_save = column.top();
				save();
				print_strategy(strategy_file);
				strategy_file.close();
				if (down_open() && crash_move != DOWN) {
					go_down();
					split_move = DOWN;
					crash_move = NULL;
					if (success()){
						print_maze();
						break;
					}
					mark_path();
				}

				else if (up_open() && crash_move != UP) {
					go_up();
					split_move = UP;
					crash_move = NULL;
					if (success()){
						print_maze();
						break;
					}
					mark_path();
				}

				else if (left_open() && crash_move != LEFT) {
					go_left();
					split_move = LEFT;
					crash_move = NULL;
					if (success()){
						print_maze();
						break;
					}
					mark_path();
				}

				else if (right_open() && crash_move != RIGHT) {
					go_right();
					split_move = RIGHT;
					crash_move = NULL;
					if (success()){
						print_maze();
						break;
					}
					mark_path();
				}
			}

			if (got_fucked()) { // base case 2, return pivot
				if (success()) {
					return;
				}
				print_strategy(strategy_file);
				crash_move = split_move;
				return_to_save_point();
				reload();
			}

			else if (down_open()) {
				go_down();
				print_strategy(strategy_file);
				if (success()){
					print_maze();
					break;
				}
				mark_path();
			}

			else if (up_open()) {
				go_up();
				print_strategy(strategy_file);
				if (success()){
					print_maze();
					break;
				}
				mark_path();
			}

			else if (left_open()) {
				go_left();
				print_strategy(strategy_file);
				if (success()){
					print_maze();
					break;
				}
				mark_path();
			}

			else if (right_open()) {
				go_right();
				print_strategy(strategy_file);
				if (success()){
					print_maze();
					break;
				}
				mark_path();
			}
			//print_strategy(strategy_file);
		}
	}
};


int main() {

	string choice = get_algorithm_choice();
	if (choice == "1" || choice == "vector" || choice == "Vector") {
		maze_vector extra_credit;
		extra_credit.cout_maze();
		extra_credit.recursive_vector();
		extra_credit.cout_maze();
		cout << endl;
		cout << "All locations traversed by exhaustive search is documented in " << endl
			<< extra_credit.return_strategy_by_method() + extra_credit.return_file_name() << endl << endl;
		cout << "Quick and simple solution is documented in " << endl
			<< extra_credit.return_solution_by_method() + extra_credit.return_file_name() << endl;
	}
	else if (choice == "2" || choice == "stack" || choice == "Stack") {
		//stack<int> row;
		//stack<int> column;
		maze_stack extra_credit;
		extra_credit.cout_maze();
		extra_credit.stack_solve();
		extra_credit.cout_maze();
		cout << "All locations traversed by exhaustive search is documented in " << endl
			<< extra_credit.return_strategy_by_method() + extra_credit.return_file_name() << endl << endl;
		cout << "Quick and simple solution is documented in " << endl
			<< extra_credit.return_solution_by_method() + extra_credit.return_file_name() << endl;
	}

	cout << endl;

	cin.get();
	cin.get();
}