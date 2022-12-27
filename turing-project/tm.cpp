#include <iostream>
#include <fstream>
#include <cmath>
#include "tm.h"

using namespace std;

string TuringMachine::split_line = "---------------------------------------------";
string TuringMachine::run_line = "==================== RUN ====================";
string TuringMachine::end_line = "==================== END ====================";
string TuringMachine::err_line = "==================== ERR ====================";
TuringMachine::TuringMachine(const string &filename, bool is_verbose){
	_is_verbose = is_verbose;
	_parser = new Parser(this, is_verbose);
	ifstream in_file;
	in_file.open(filename);
	if (!in_file.is_open()) {
		cerr << "Failed to open TM file." << endl;
		exit(1);
	}
	string line;
	while (getline(in_file, line)) {
		_parser->parse(line);
	}
	in_file.close();
	for (int i = 0; i < _tape_num; i++) {
		_tapes.emplace_back(Tape(i, _blank_symbol));
	}
	_step = 0;
}

TuringMachine::~TuringMachine() {
	delete _parser;
}

bool TuringMachine::match(Transition &trans) {
	auto cur = current_head();
	auto &tran = trans.current_tape;
	if (_cur_state != trans.current_state) return false;
	for (int i = 0; i < _tape_num; i++) {
		if (!(cur[i] == tran[i] || cur[i] != _blank_symbol && tran[i] == '*')) return false;
	}
	return true;
}

bool TuringMachine::step() {
	_step++;
	for (auto &transition: _transitions) {
		if (match(transition)) {
			_cur_state = transition.next_state;
			for (int i = 0; i < _tape_num; i++) {
				char new_ch = transition.next_tape[i];
				if (new_ch == '*') new_ch = _tapes[i].current();
				_tapes[i].execute(new_ch, transition.directions[i]);
			}
			return true;	
		}
	}
	return false;
}

void TuringMachine::execute() {
	if (_is_verbose) print();
	while(step()) {
		if (_is_verbose) print();
	}
	if (_is_verbose) {
		cout << "Result: " << _tapes[0].result() << endl; 
		cout << end_line << endl;
	}
	else {
		cout << _tapes[0].result() << endl;
	}
}

void TuringMachine::print() {
	cout << "Step   : " << _step << endl;
	cout << "State  : " << _cur_state.to_string() << endl;
	for (auto &tape: _tapes) {
		cout << tape.to_string();
	}
	cout << TuringMachine::split_line << endl;
}

vector<char> TuringMachine::current_head(){
	vector<char> result;
	for (auto &tape : _tapes) {
		result.push_back(tape.current());
	}
	return result;
}

void TuringMachine::input(const string & str) {
	for (int i = 0; i < str.length(); i++) {
		char ch = str[i];
		if (_input_symbols.find(ch) == _input_symbols.end()) {
			if (_is_verbose) {
				cerr << "error: \'" << ch << "\' was not declared in the set of input symbols" << endl;
				cerr << "Input: " << str << endl;
				cerr << "       ";
				for (int j = 0; j < i; j++) cerr << " ";
				cerr << "^" << endl;
				cerr << err_line << endl;
			}
			else {
				cerr << "illegal input" << endl;
			}
			exit(3);
		}
	}
	if (_is_verbose) {
		cerr << "Input: " << str << endl;
		cerr << run_line << endl;
	}
	_tapes[0].set(str);
}
