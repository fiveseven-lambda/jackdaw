#include <iostream>
#include "char.h"
#include "string.h"
#include "pos.h"
#include "errmsg.h"

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
void error_undefined_macro(const String &str){
	std::cerr << pos(str) << " undefined macro \"" << str << "\"" << std::endl;
}
void error_duplicate_macro(const String &l, const String &r){
	std::cerr << pos(r) << " macro \"" << r << "\" is already defined at " << pos(l) << std::endl;
}
