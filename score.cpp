#include "char.h"
#include "string.h"
#include "score.h"

class Literal{
};

void addscore(const String &in){
	for(auto i = in.text.begin(); i != in.text.end(); ++i){
		std::cout << *i << std::endl;
	}
}
