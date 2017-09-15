/*
 * File: WordLadder.cpp
 * --------------------
 * Name:Yang JianBang
 * Student ID:515030910223
 * This file is the starter project for the word ladder problem.
 *
 *Psuedo Code:
 *
 *Create an empty queue.
 *Add the start word to a new ladder.
 *Add the ladder to the queue.
 *while (the queue is not empty) {
 *    Dequeue the first ladder from the queue.
 *    if (the final word in this ladder is the destination word) {
 *        Return this ladder as the solution.
 *    }
 *    for (each word in the lexicon of English words that differs by one letter) {
 *        if (that word has not already been used in a ladder) {
 *            Create a copy of the current ladder.
 *            Add the new word to the end of the copy.
 *            Add the new ladder to the end of the queue.
 *            }
 *    }
 *}
 *Report that no word ladder exists.
 */

#include <iostream>
#include <fstream>
#include <string>
#include <set>
#include <queue>
#include <vector>
using namespace std;

//find the dest and init
void show_ans(vector<string>& ans);

//input and start function
void input_words(string& start, string& dest);

//read lexicon from file
void read_english(set<string>& english);

//implement of show_ans:shows the ladder if found
void show_ans(vector<string>& ans) {
	cout << "Found ladder: ";
	for (int i = 0; i < ans.size() - 1; i++) {
		cout << ans[i] << "->";
	}
	cout << ans[ans.size() - 1] << endl<<endl;                      //The "->" doesn't follow the last word
}

//implement of input_words
void input_words(string& start, string& dest) {
	cout << "Enter start word (enter ':q' to quit):";
	cin >> start;
	if (start == ":q") {
		exit(0);
	}
	cout << "Enter destination word:";
	cin >> dest;
}

//implement of read_english
void read_english(set<string>& english) {
	while (true) {
		cout << "Please enter filename containing source text:";
		string file_name;
		cin >> file_name;
		ifstream file_input(file_name.c_str());
		if (file_input.is_open()) {
			string temp_word;
			while (getline(file_input, temp_word)) {
				english.insert(temp_word);                      
			}
			break;
		}
	}
}

bool letter_differ(string comparable1, string comparable2) {
	int flag = 0;
	for (unsigned i = 0; i < comparable1.length(); i++) {
		if (comparable1[i] != comparable2[i])flag++;
		if (flag > 1)break;
	}

	if (flag == 1)return true;                          //return true only if there is only a letter different
	else return false;
}

set<string> trim_lexicon(set<string>&lexicon,int length){                                       //only need the words whose length equals the start(destination) word's length
	set<string> short_lexicon;
	for (set<string>::iterator it = lexicon.begin(); it != lexicon.end();)	{
		if ((*it).length() == length)short_lexicon.insert(*it);
		it++;
	}
	return short_lexicon;
}

void find_ladder(set<string>&lex) {
	//get start and destination words                                           //read start and destination words first because reading lexicon needs their length
	string start;
	string dest;
	input_words(start, dest);
	if (start.length() != dest.length()) {
		cout << "Sorry, no ladder found!" << endl;                          //words with different lengths never have a ladder
		return;
	}

	set<string> lexicon = trim_lexicon(lex,start.length());

	if (lexicon.find(start) == lexicon.end()) {                                 //check if the start or destination word exists in the lexicon
		cout << "There is no such start word in this lexicon." << endl;
		return;
	}
	if (lexicon.find(dest) == lexicon.end()) {
		cout << "There is no such detination word in this lexicon." << endl;
		return;
	}

	//create a new ladder and add the start word to it.
	queue<vector<string>>ladders;
	vector<string>ladder;                                                        //single ladder
	ladder.push_back(start);
	ladders.push(ladder);
	lexicon.erase(start);                                                        //erase used word

	int count = 0;
	while (!ladders.empty()) {
		vector<string>ladder = ladders.front();
		ladders.pop();

		string current_word = ladder.back();
		set<string>::iterator end = lexicon.end();
		int flag = 0;
		unsigned length;
		for (set<string>::iterator it = lexicon.begin(); it != end;) {
			flag = 0;                                    //add to queue if there is only a letter different 
			length = (*it).length();
			for (unsigned i = 0; i < length; i++) {
				if (current_word[i] != (*it)[i])flag++;
				if (flag > 1) break;
			}

			if (flag == 1) {                              //return true only if there is only a letter different
				vector<string> copy = ladder;
				copy.push_back(*it);
				if (letter_differ(*it, dest)) {           //if the last word of the ladder has only a word different from the destination words, it means a ladder found
					copy.push_back(dest);
					show_ans(copy);
					return;
				}
				ladders.push(copy);
				lexicon.erase(it++);
			}
			else it++;

		}
	}

	cout << "Sorry, no ladder found!" << endl;
	return;
}


int main() {
	//read words from file
	set<string>lexicon;
	read_english(lexicon);

	while (true) {
		find_ladder(lexicon);
	}
}