#ifndef __PARSER_H__
#define __PARSER_H__

class TuringMachine;

#include <string>
#include <set>
#include "tape.h"
#include "tm.h"


class Parser {

	int _lineno;

	bool _is_verbose;

	TuringMachine *_target;

	void error(const std::string & msg);

	void parse_states(const std::string & str);
	void parse_syms(const std::string & str, bool is_input);
	void parse_start_state(const std::string & str);
	void parse_blank_sym(const std::string & str);
	void parse_final_states(const std::string & str);
	void parse_tape_num(const std::string & str);
	void parse_transition(const std::string & str);

	std::set<State> parse_state_set(const std::string & str);

public:
	void parse(const std::string & str);
	Parser(TuringMachine *tm, bool is_verbose): _target(tm), _is_verbose(is_verbose), _lineno(0) {}
};

#endif