// CS2134 Fall 2015
// HW7 PQ1 Solution

#include <iostream>
#include <vector>
#include <fstream>
#include <sstream>
#include <map>
using namespace std;

#define ASCIIUPPEROFFSET 65
#define ASCIILOWEROFFSET 97

void initPointVector (vector<int>& alphabet) {
	ifstream alphaVal("Letter_point_value.txt");
	if (!alphaVal) {
		cerr << "No letter point file by that name.\n";
		exit(1);
	}
	string line;
	while ( getline(alphaVal, line) ) {
		stringstream lineParse(line);
		int point;
		lineParse >> point;
		char letter = ' ';
		lineParse >> letter >> letter;
		int asciival = (int)letter;
		if (asciival > 64 && asciival < 91) { alphabet[asciival-ASCIIUPPEROFFSET] = point; }
	}
}

int getWordPoint (const string& word, const vector<int>& alphabet) {
	int value = 0;
	for (int i = 0; i < word.length(); i++)
		if ( (int)word[i] > 96 && (int)word[i] < 123)
			value += alphabet[ (int)word[i] - ASCIILOWEROFFSET ];
	return value;
}

void initWords (map<string,int>& allWords, const vector<int>& alphabet) {
	ifstream enable("ENABLE.txt");
	if (!enable) {
		cerr << "No word list file by that name.\n";
		exit(1);
	}
	string line;
	int value;
	while (getline(enable, line)) {
		value = getWordPoint(line, alphabet);
		allWords.insert( pair<string,int>(line, value) );
	}
}

int main() {
	vector<int> alphabet(26,0);
	initPointVector(alphabet);
	map<string, int> allWords;
	initWords(allWords, alphabet);
	string testWord = "abacterial";
	if ( allWords.find(testWord) != allWords.end() ) {
		cout << "Point value of " << testWord << " is " << allWords[testWord] << ".\n";
	} else {
		cout << "This word was not in the map.\n";
	}
}
