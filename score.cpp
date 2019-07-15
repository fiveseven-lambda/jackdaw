#include "char.h"
#include "string.h"
#include "errmsg.h"
#include "score.h"

class Literal{
};

void addscore(const String &in){
	for(auto i = in.text.begin(); i != in.text.end(); ++i){
		error_unexpected_character(*i);
	}
}
