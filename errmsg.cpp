#include <iostream>
#include "char.h"
#include "string.h"
#include "pos.h"
#include "errmsg.h"

void error_unterminated_comment(const Char &){}
void error_unexpected_end_of_file(const Char &){}
void error_unexpected_character(const Char &ch){
	std::cerr << Pos<Char>(ch) << ch << std::endl;
}
void error_unknown_command(const String &){}
void error_undefined_macro(const String &){}
void error_duplicate_macro(const String &, const String &){}
