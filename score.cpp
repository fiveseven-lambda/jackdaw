#include "char.h"
#include "string.h"
#include "errmsg.h"
#include "score.h"

class Note{
public:
	double freq, vel;
	double attack, decay, sustain, release;
	double start, end;
	void wav(double *, int);
};

class Literal{
	enum class Type{ tbd, frac, dec } type;

public:
	void add(char);
};

void addscore(const String &in){
	for(auto i = in.text.begin(); i != in.text.end(); ++i){
		error_unexpected_character(*i);
	}
}
