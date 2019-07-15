#include <iostream>
#include "char.h"
#include "string.h"
#include "errmsg.h"

template<class T>
Pos<T> pos(const T &obj){
	return Pos<T>(obj);
}

std::ostream &operator<<(std::ostream &os, const Pos<Char> &pos){
	os << "[";
	for(auto i = pos.ch.pos.begin(); i != pos.ch.pos.end(); ++i){
		if(i != pos.ch.pos.begin()) os << "<";
		os << i->filename << ":" << i->line << "," << i->pos;
	}
	os << "]";
	return os;
}

std::ostream &operator<<(std::ostream &os, const Pos<String> &pos){
	return pos.str.text.empty() ? os << "[unknown]" : os << ::pos(pos.str.text.front()) << "-" << ::pos(pos.str.text.back());
}

void error_open(const char filename[]){
	std::cerr << "cannot open file \"" << filename << "\"" << std::endl;
	exit(-1);
}
void error_open_twice(const char filename[]){
	std::cerr << "file \"" << filename << "\" opened twice" << std::endl;
	exit(-1);
}
void error_unterminated_comment(const Char &ch){
	std::cerr << pos(ch) << " unterminated comment" << std::endl;
	exit(-1);
}
void error_unexpected_end_of_file(const Char &ch){
	std::cerr << pos(ch) << " unexpected end of file" << std::endl;
	exit(-1);
}
void error_unexpected_character(const Char &ch){
	std::cerr << pos(ch) << " unexpected character \'" << ch << "\'" << std::endl;
}
void error_unknown_command(const String &str){
	std::cerr << pos(str) << " unknown command \"" << str << "\"" << std::endl;
}
void error_command_arguments(const String &str, int n){
	std::cerr << pos(str) << " command \"" << str << "\" takes " << n << " argument";
	if(n != 1) std::cerr << 's';
	std::cerr << std::endl;
}
void error_undefined_macro(const String &str){
	std::cerr << pos(str) << " undefined macro \"" << str << "\"" << std::endl;
}
void error_duplicate_macro(const String &l, const String &r){
	std::cerr << pos(r) << " macro \"" << r << "\" is already defined at " << pos(l) << std::endl;
}

void error_null_macro_defined(const Char &ch){
	std::cerr << pos(ch) << " you cannot define macro {}" << std::endl;
}
void error_null_macro(const Char &ch){
	std::cerr << pos(ch) << " you cannot use macro {}" << std::endl;
}
void error_invalid_literal(const Char &ch){
	std::cerr << pos(ch) << " invalid literal \'" << ch << "\'" << std::endl;
}
