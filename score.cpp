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
	bool frac;
	class Val{
		bool is;
		double dec;
		double val;
		const Char *point;
	public:
		operator double() const{
			if(is) return val;
			else if(point){
				error_invalid_literal(*point);
				return 1;
			}else return 1;
		}
		Val(): is(0), dec(0), val(0), point(nullptr) {}
		void add(const Char &);
	} val[2];
public:
	Literal(): frac(false) {}
	void add(const Char&);
	double get(double standard){
		return frac ? standard * val[0] / val[1] : val[0];
	}
};

void addscore(const String &in){
	Literal tmp;
	for(auto i = in.text.begin(); i != in.text.end(); ++i){
		switch(*i){
			case '0' ... '9':
			case '.':
			case '/':
				tmp.add(*i);
		}
	}
	std::cout << tmp.get(1000) << std::endl;
}

void Literal::Val::add(const Char &ch){
	if(ch == '.'){
		point = &ch;
		dec = 1;
	}else{
		is = true;
		if(point){
			val += (ch - '0') * (dec *= .1);
		}else{
			val = val * 10 + (ch - '0');
		}
	}
}
void Literal::add(const Char &ch){
	if(ch == '/'){
		frac = true;
	}else{
		val[frac].add(ch);
	}
}
