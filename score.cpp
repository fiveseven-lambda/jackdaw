#include <array>
#include <vector>
#include "char.h"
#include "string.h"
#include "errmsg.h"
#include "score.h"

double end = 0;

struct Note{
	double freq, vel;
	double attack, decay, sustain, release;
	double start, end;
};

std::vector<Note> score;

class Literal{
	bool frac;
	class Val{
		bool is;
		double dec;
		double val;
		const Char *point;
	public:
		double get() const {
			return is ? val : 1;
		}
		operator bool() const {
			if(is) return true;
			else{
				if(point) error_invalid_literal(*point);
				return 0;
			}
		}
		Val(): is(false), dec(0), val(0), point(nullptr) {}
		void add(const Char &);
	} val[2];
public:
	Literal(): frac(false) {}
	void add(const Char&);
	operator bool() const {
		return frac || val[0];
	}
	double get(const double &standard){
		return frac ? standard * val[0].get() / val[1].get() : val[0].get();
	}
};

void addscore(const String &in){
	int index = 0;
	std::array<Literal *, 7> l{new Literal, new Literal, new Literal, nullptr, nullptr, nullptr, nullptr};
	double start = end, cursor = start;
	std::array<double, 3> standard{440, 1, 1};

	struct{
		std::vector<double> lens;
		size_t i;
		double next(const Char &ch){
			if(lens.empty()){
				error_note_length_unspecified(ch);
				return 0;
			}
			double ret = lens[i];
			++i;
			if(i == lens.size()) i = 0;
			return ret;
		}
	} rhythm;

	for(auto i = in.text.begin(); i != in.text.end(); ++i){
		switch(*i){
			case ' ':
			case '\n':
				break;
			case 'F':
			case 'f':
				index = 0;
				goto reset;
			case 'L':
			case 'l':
				index = 1;
				goto reset;
			case 'V':
			case 'v':
				index = 2;
				goto reset;
			case 'A':
			case 'a':
				index = 3;
				goto reset;
			case 'D':
			case 'd':
				index = 4;
				goto reset;
			case 'S':
			case 's':
				index = 5;
				goto reset;
			case 'R':
			case 'r':
				index = 6;
				goto reset;
			reset:
				delete l[index];
				l[index] = new Literal;
				break;
			case '0' ... '9':
			case '.':
			case '/':
				l[index]->add(*i);
				break;
			case ',':
				score.push_back([&](){
					double len = *l[1] ? l[1]->get(standard[1]) : rhythm.next(*i);
					Note ret;
					ret.freq = l[0]->get(standard[0]);
					ret.vel = l[2]->get(standard[2]);
					ret.attack = l[3] && *l[3] ? l[3]->get(len) : 0;
					ret.decay = l[4] && *l[4] ? l[4]->get(ret.vel) : 0;
					ret.sustain = l[5] && *l[5] ? l[5]->get(len) : 1;
					ret.release = l[6] && *l[6] ? l[6]->get(len) : 0;
					ret.start = cursor;
					ret.end = cursor += len;
					return ret;
				}());
				for(int i = 0; i < 3; ++i){
					delete l[i];
					l[i] = new Literal;
				}
				break;
			default:
				error_score_unexpected_character(*i);

		}
	}
	for(auto i : l) delete i;
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

void wav(char*){
	for(auto i : score){
		std::cout
		<< i.start << "-" << i.end
		<< " f" << i.freq
		<< " v" << i.vel
		<< " a" << i.attack
		<< " d" << i.decay
		<< " s" << i.sustain
		<< " r" << i.release
		<< std::endl;
	}
}
