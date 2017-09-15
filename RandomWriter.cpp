/*
 * File: RandomWriter.cpp
 * ----------------------
 * Name:Yang Jianbang
 * Student ID:515030910223
 * This file is the starter project for the random writer problem
 * on Assignment #2.
 *
 *Principle:
 *
 *For example, consider an order-2 Markov model built from this sentence from Franz Kafka¡¯s Metamorphosis:
 *As Gregor Samsa awoke one morning from uneasy dreams he found himself transformed in his bed into a gigantic insect.
 *
 *Here is how the first few characters might be chosen:
 *
 *(a)The most commonly occurring sequence is the string "in", which appears four times. This string therefore becomes the initial seed.
 *
 *(b)The next character is chosen based on the probability that it follows the seed "in" in the source.The source contains four 
 *   occurrences of "in", one followed by g, one followed by t, onefollowed by s, and one followed by a space. Thus, there should 
 *   be a 1/4 chance each ofchoosing g, t, s, or space. Suppose space is chosen this time.
 *
 *(c)The seed is updated to "n". The source contains one occurrence of "n", which is followedby h. Thus the next character chosen is h.
 *
 *(d)The seed is now "h". The source contains three occurrences of "h", once followed by e, andtwice followed by i. Thus, there is a 1/3 
 *   chance of choosing e and 2/3 for i. Imagine i is chosen this time.
 *
 *(e)The seed is now "hi" . The source contains two occurrences of "hi", once followed by m, theother by s. For the next character, there 
 *   is 1/2 chance of choosing m and 1/2 chance for s.
 */

#include <fstream>
#include <iostream>
#include <string>
#include <map>
#include <vector>
#include <ctime>
using namespace std;
#define MAX_CHAR_NUM 100

//report kinds of errors
class Error {
public:
	string context;
	Error(string s)
		:context(s)
	{}
	string what() { return context; };
};

//get filename
void read_file(ifstream& file_input);

//get order number
int read_order();

//build model
void model_read(ifstream& file_input, const int& order, map<string, vector<char> >& model);

//write
void random_write(const int& order, map<string, vector<char> >& model);

//implement of read_file
void read_file(ifstream& file_input) {
	cout << "Please enter the name of the source file(enter ':q' to quit):";
	string file_name;
	cin >> file_name;
	if (file_name == ":q")exit(0);
	file_input.open(file_name.c_str());
}

//implement of read_order
int read_order() {
	int order = 0;
	while (order < 1 || order>10) {
		cout << "Please enter the order number(from 1 to 10):";	                                     //order means the length of the sequence
		cin >> order;
	}
	return order;
}

//implement of model_read
void model_read(ifstream& file_input, const int& order, map<string, vector<char> >& model) {
	string seed = "";
	char temp_char;

	for (int i = 0; i < order; i++) {                     //read the first seed from the file
		if (file_input)seed += file_input.get();
		else throw Error("Not enough seed");              //error if there is no enough charaters for a seed with a length of order
	}

	if (file_input)temp_char = file_input.get();
	else throw Error("Not enough charaters");             //error if there is no charaters for the second seed because you can't write a eassy with only a word
	model[seed].push_back(temp_char);

	while (file_input) {                                  //read all the seeds until the end of the file
		seed = seed.substr(1, order - 1) + temp_char;
		temp_char = file_input.get();
		model[seed].push_back(temp_char);
	}
}

//implement of random_write
void random_write(const int& order, map<string, vector<char> >& model) {
	map<string, vector<char> >::iterator it_seed = model.begin();                                       //find the most occuring sequence
	for (map<string, vector<char> >::iterator it = model.begin(); it_seed != model.end(); ++it) {
		if (it == model.end())break;
		if (it->second.size() > it_seed->second.size()) it_seed = it;
		else if (it->second.size() == it_seed->second.size()) if(rand()%10<5)it_seed = it;               //if the two sequence have the same occuring time, choose either of them equally
	}

	for (int i = 0; i < MAX_CHAR_NUM-order; i++) {
		char temp = it_seed->second[rand()%(it_seed->second.size())];                                    //choose the following charater of the current seed according to their occuring chance 
		cout << temp;
		string seed = it_seed->first.substr(1, order - 1) + temp;                                        //get to the next seed
		it_seed = model.find(seed);
	}
	cout << endl;
}


int main() {
	try {
		while (true) {
			srand(time(NULL));           //get a random seed
			ifstream file_input;
			read_file(file_input);

			int order = read_order();
			map<string, vector<char> >model;

			cout << "I'm writting now, please wait!......" << endl<< "My essay is here:" << endl ;   //ask the user to wait
			model_read(file_input, order, model);

			random_write(order, model);
		}
		return 0;
	}
	catch(Error&e) {
		cout << e.what();
	}
	catch(...){
		cout << "run_time error";
	}
}
