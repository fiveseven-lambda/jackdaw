#include "char.h"
#include "string.h"
#include "pos.h"

std::ostream &operator<<(std::ostream &os, const Pos<Char> &pos){
	os << "[";
	for(auto i = pos.ch.pos.begin(); i != pos.ch.pos.end(); ++i){
		if(i != pos.ch.pos.begin()) os << "<";
		os << i->filename << ":" << i->line << "," << i->pos;
	}
	os << "]";
	return os;
}
