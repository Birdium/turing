#ifndef __TM_H__
#define __TM_H__

#include <set>
#include <vector>
#include <string>
#include <deque>

class Parser;

#include "tape.h"
#include "parser.h"

struct Transition{
	typedef std::vector<char> VecChar;
	typedef std::vector<Direction> VecDir;
	
	State current_state, next_state;

	std::vector<char> current_tape, next_tape;

	std::vector<Direction> directions;

	Transition(State _current_state, State _next_state, VecChar _current_tape, VecChar _next_tape, VecDir _directions): 
		current_state(_current_state), next_state(_next_state), current_tape(_current_tape), next_tape(_next_tape), directions(_directions) {}
};

class TuringMachine{

	friend class Parser;

	std::set<State> _states;

	std::set<char> _input_symbols;

	std::set<char> _tape_symbols;

	std::set<State> _final_states;

	State _cur_state{""};

	std::vector<Transition> _transitions;

	std::vector<Tape> _tapes;

	char _blank_symbol;

	int _step;

	int _tape_num;

	bool _is_verbose;

	static std::string split_line; 
	static std::string run_line;
	static std::string end_line;
	static std::string err_line;


	Parser *_parser;

	void check();

	void error(const std::string & msg);

	bool match(Transition &trans);

public:
	TuringMachine(const std::string &filename, bool is_verbose);

	~TuringMachine();

	void execute();

	bool step();

	void print();

	void input(const std::string & str);

	std::vector<char> current_head();
};

#endif