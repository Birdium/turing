#ifndef __TAPE_H__
#define __TAPE_H__

#include <string>
#include <deque>

class State {

	std::string _name;

public: 

	State(const std::string &name): _name(name) {}

	std::string to_string();

	const bool operator < (const State &x) const {
		return _name < x._name;
	}

	const bool operator == (const State &x) const {
		return _name == x._name;
	}

	const bool operator != (const State &x) const {
		return _name != x._name;
	}
 
};

enum Direction {
	left, right, stay 
};


class Tape {

	char _blank_sym;

	int _num;

	int _abs_index;

	int _tape_index;

	std::deque<char> _tape;

	void push_blank();

	void pop_blank();

public: 

	char current();

	void execute(char ch, Direction direction);

	std::string result();

	std::string to_string();

	Tape(int num, char blank_sym): _num(num), _blank_sym(blank_sym), _abs_index(0), _tape_index(0), _tape(1, blank_sym) {}

	void set(const std::string & str);

};

#endif