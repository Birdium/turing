#include <iostream>
#include <sstream>
#include <vector>
#include "parser.h"

using namespace std;

bool is_validID(char ch) {
	return 
		'A' <= ch && ch <= 'Z' || 
	    'a' <= ch && ch <= 'z' || 
		'0' <= ch && ch <= '9' || 
		ch == '_';
}

bool is_validID(const string & str) {
	if (str.length() == 0) return false;
	for (auto ch : str) {
		if (!is_validID(ch)) return false;
	}
	return true;
}

bool is_validSym(char ch) {
	static string str = " ,;{}*";
	for (auto invalid_ch : str) {
		if (ch == invalid_ch) return false;
	}
	return true;
}

bool is_validTranSym(char ch) {
	return ch == '*' || is_validSym(ch);
}

bool is_validDirection(char ch) {
	return ch == 'l' || ch == 'r' || ch == '*';
}

bool is_blank_ASCII(char ch) {
	return ch == ' ' || ch == '\t' || ch == '\n';
}

bool is_empty_str(const string &str) {
	for (auto ch : str) {
		if (!is_blank_ASCII(ch)) {
			return false;
		}
	}
	return true;
}

string remove_blank(const string &str) {
	string result = "";
	for (auto ch : str) {
		if (!is_blank_ASCII(ch)) {
			result += ch;
		}
	}
	return result;
}

void Parser::error(const string & msg) {
	if (_is_verbose) {
		cerr << "syntax error at line " << _lineno << ": " << msg << endl;
	}
	else {
		cerr << "syntax error" << endl;
	}
	exit(2);
}

set<State> Parser::parse_state_set(const string &str) {	
	set<State> result;
	int len = str.length();
	int start = 1;
	for (int i = start; i < len; i++) {
		char ch = str[i];
		if (ch == '}') {
			if (i == start) error(string("unexpected character \'") + ch + "\' in state set");
			result.emplace(State{str.substr(start, i - start)});
			break;
		}
		else if (ch == ',') {
			if (i == start) error(string("unexpected character \'") + ch + "\' in state set");
			result.emplace(State{str.substr(start, i - start)});
			start = i + 1;
		}
		else if (!is_validID(ch)) {
			error(string("unexpected character \'") + ch + "\' in state set");
		}
	}
	return result;
}

void Parser::parse_states(const string &str) {
	int len = str.length();
	if (len <= 2 || str[2] != '=') {
		error("missing character \'=\'.");
	}
	if (len <= 3 || str[3] != '{') {
		error("missing character \'{\'.");
	}
	if (str[len-1] != '}') {
		error("missing character \'}\'.");
	}
	_target->_states = parse_state_set(str.substr(3));
}

void Parser::parse_final_states(const string &str) {
	int len = str.length();
	if (len <= 2 || str[2] != '=') {
		error("missing character \'=\'.");
	}
	if (len <= 3 || str[3] != '{') {
		error("missing character \'{\'.");
	}
	if (str[len-1] != '}') {
		error("missing character \'}\'.");
	}
	_target->_final_states = parse_state_set(str.substr(3));
}

void Parser::parse_syms(const string & str, bool is_input) {
	int len = str.length();
	if (len <= 2 || str[2] != '=') {
		error("missing character \'=\'.");
	}
	if (len <= 3 || str[3] != '{') {
		error("missing character \'{\'.");
	}
	if (str[len-1] != '}') {
		error("missing character \'}\'.");
	}
	for (int i = 4; i < len - 1; i++) {
		char ch = str[i];
		if (ch == ',') {
			if (i % 2 == 0) {
				error("unexpected character \',\'");
			}
		}
		else if (!is_validSym(ch) || is_input && ch == '_') {
			error(string("unexpected character \'") + ch + '\'');
		}
		else {
			if (is_input) _target->_input_symbols.insert(ch);
			else _target->_tape_symbols.insert(ch);
		}
	}
}

void Parser::parse_start_state(const string & str){
	int len = str.length();
	if (len <= 3 || str[3] != '=') {
		error("missing character \'=\'.");
	}
	string state = str.substr(4);
	if (!is_validID(state)) {
		error("invalid start state.");
	}
	_target->_cur_state = State(state);
}

void Parser::parse_blank_sym(const string & str) {
	int len = str.length();
	if (len <= 2 || str[2] != '=') {
		error("missing character \'=\'.");
	}
	if (len == 3) {
		error("missing blank character.");
	}
	if (len > 4) {
		error(string("unexpected character \'") + str[4]);
	}
	char ch = str[3];
	_target->_blank_symbol = ch;
}

void Parser::parse_tape_num(const string & str) {
	int len = str.length();
	if (len <= 2 || str[2] != '=') {
		error("missing character \'=\'.");
	}
	if (len == 3) {
		error("missing tape num.");
	}
	string str_num = str.substr(3);
	int tape_num = -1;
	try {
		tape_num = stoi(str_num);
	} 
	catch (std::invalid_argument&) {
		error("invalid tape num");
	}
	char ch = str[3];
	_target->_tape_num = tape_num;
}

void Parser::parse_transition(const string &str) {
	istringstream iss(str);
	string current_state_str, current_tape_str, next_tape_str, direction_str, next_state_str;
	if (!(iss >> current_state_str)) return;
	if (!(iss >> current_tape_str)) error("missing current tape symbol of transition.");
	if (!(iss >> next_tape_str)) error("missing next tape symbol of transition.");
	if (!(iss >> direction_str)) error("missing direction of transition.");
	if (!(iss >> next_state_str)) error("missing next state of transition.");

	if (!is_validID(current_state_str)) error("invalid current state of transition.");
	if (!is_validID(next_state_str)) error("invalid next state of transition.");
	for (auto ch : current_tape_str) if (!is_validTranSym(ch)) error("invalid current tape sym of transition.");
	for (auto ch : next_tape_str) if (!is_validTranSym(ch)) error("invalid next tape sym of transition.");
	for (auto ch : direction_str) if (!is_validDirection(ch)) error("invalid directions of transition.");
	if (current_tape_str.length() != _target->_tape_num) error("invalid current tape sym of transition.");
	if (next_tape_str.length() != _target->_tape_num) error("invalid next tape sym of transition.");
	if (direction_str.length() != _target->_tape_num) error("invalid directions of transition.");

	State current_state(current_state_str);
	State next_state(next_state_str);
	vector<char> current_tape; for (auto ch : current_tape_str) current_tape.push_back(ch);
	vector<char> next_tape; for (auto ch : next_tape_str) next_tape.push_back(ch);
	vector<Direction> directions; for (auto ch : direction_str) {
		if (ch == 'l') directions.emplace_back(Direction::left);
		else if (ch == 'r') directions.emplace_back(Direction::right);
		else if (ch == '*') directions.emplace_back(Direction::stay);
	}
	_target->_transitions.emplace_back(Transition{current_state, next_state, current_tape, next_tape, directions});
}

void Parser::parse(const string &line) {
	string str = line;
	_lineno++;
	int semi = str.find(';');
	if (semi != string::npos) {
		str = str.substr(0, semi);
	}
	if (is_empty_str(str)) return;
	if (str[0] == '#') {
		str = remove_blank(str);
		if (str[1] == 'Q') parse_states(str);
		else if (str[1] == 'S') parse_syms(str, true);
		else if (str[1] == 'G') parse_syms(str, false);
		else if (str[1] == 'q' && str[2] == '0') parse_start_state(str);
		else if (str[1] == 'B') parse_blank_sym(str);
		else if (str[1] == 'F') parse_final_states(str);
		else if (str[1] == 'N') parse_tape_num(str);
		else {
			error("missing character after \'#\'");
		}
	}
	else {
		parse_transition(str);
	}
}