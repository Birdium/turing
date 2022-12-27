#include "tape.h"

using namespace std;

int num_len(int x) {
	x = abs(x);
	if (x == 0) return 1;
	int result = 0;
	while (x) {
		result ++;
		x /= 10;
	}
	return result;
}

string State::to_string() {return _name;}

char Tape::current() {
	return _tape[_tape_index];
}

void Tape::push_blank() {
	_tape.push_front(_blank_sym);
	_tape_index ++;
	_tape.push_back(_blank_sym);
}

void Tape::pop_blank() {
	while (_tape.size() > _tape_index + 1 && _tape.back() == '_') {
		_tape.pop_back();
	}
	while (_tape_index > 0 && _tape.front() == '_') {
		_tape.pop_front();
		_tape_index --;
	}
}

void Tape::execute(char ch, Direction direction) {
	push_blank();
	_tape[_tape_index] = ch;
	if(direction == Direction::left) {
		_tape_index --;
		_abs_index --;
	}
	else if (direction == Direction::right) {
		_tape_index ++;
		_abs_index ++;
	}
	pop_blank();
}

void Tape::set(const string & str) {
	_abs_index = _tape_index = 0;
	_tape.clear();
	for (auto ch : str) {
		_tape.push_back(ch);
	}
}

string Tape::result() {
	string result;
	for (auto ch : _tape) {
		result += ch;
	}  
	return result;
}

string Tape::to_string() {
	string result = "";
	result += "Index" + std::to_string(_num) + " :"; 
	for (int i = 0; i < _tape.size(); i++) {
		int index = _abs_index - _tape_index + i;
		result += " ";
		result += std::to_string(abs(index));
	}
	result += "\n";
	result += "Tape" + std::to_string(_num) + "  :"; 
	for (int i = 0; i < _tape.size(); i++) {
		int index = _abs_index - _tape_index + i;
		result += " ";
		result += _tape[i];
		for (int j = 1; j < num_len(index); j ++) {
			result += " ";
		}
	}
	result += "\n";
	result += "Head" + std::to_string(_num) + "  :"; 
	for (int i = 0; i < _tape.size(); i++) {
		int index = _abs_index - _tape_index + i;
		result += " ";
		result += i == _tape_index ? "^" : " ";
		for (int i = 1; i < num_len(index); i ++) {
			result += " ";
		}
	}
	result += "\n";
	return result;
}