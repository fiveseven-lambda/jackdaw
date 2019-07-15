#include <fcntl.h>
#include <sys/mman.h>
#include <unistd.h>
#include <cmath>
#include <cstring>
#include <complex>
#include <array>
#include <vector>
#include "char.h"
#include "string.h"
#include "errmsg.h"
#include "score.h"

double end = 0;

extern int verbosity;

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

	for(auto i = in.text.begin();; ++i){
		char tmp = i != in.text.end() ? *i : ':';
		switch(tmp){
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
			case ':':
				score.push_back([&](){
					double len = *l[1] ? l[1]->get(standard[1]) : rhythm.next(*i);
					Note ret;
					ret.freq = *l[0] ? l[0]->get(standard[0]) : 0;
					ret.vel = *l[2] ? l[2]->get(standard[2]) : 0;
					ret.attack = l[3] && *l[3] ? l[3]->get(len) : 0;
					ret.decay = l[4] && *l[4] ? l[4]->get(ret.vel) : 0;
					ret.sustain = l[5] && *l[5] ? l[5]->get(len) : 1;
					ret.release = l[6] && *l[6] ? l[6]->get(len) : 0;
					ret.start = cursor;
					ret.end = cursor += len;
					return ret;
				}());
				for(int j = 0; j < 3; ++j){
					delete l[j];
					l[j] = new Literal;
				}
				if(tmp == ':') end = cursor;
				break;
			case '(':
				break;
			default:
				error_score_unexpected_character(*i);

		}
		if(i == in.text.end()){
			for(auto i : l) delete i;
			return;
		}
	}
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

int samplerate = 44100;
short channel = 2, bitdepth = 16;

void wav(const char *out_filename){
	if(verbosity > 0) for(auto i : score){
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

	int out = open(out_filename, O_CREAT | O_RDWR, S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH);
	int iend = samplerate * end;
	int filesize = bitdepth / 8 * channel * iend + 44;
	ftruncate(out, filesize);
	void *map = mmap(NULL, filesize, PROT_WRITE, MAP_SHARED, out, 0);
	if(map == MAP_FAILED) return;

	memcpy(map, "RIFF", 4);
	((unsigned int *)map)[1] = filesize - 8;
	memcpy((char *)map + 8, "WAVEfmt ", 8);
	((unsigned int *)map)[4] = 16;
	((unsigned short *)map)[10] = 1;
	((unsigned short *)map)[11] = channel;
	((unsigned int *)map)[6] = samplerate;
	((unsigned int *)map)[7] = samplerate * bitdepth / 8 * channel;
	((unsigned short *)map)[16] = channel * bitdepth / 8;
	((unsigned short *)map)[17] = bitdepth;
	memcpy((char *)map + 36, "data", 4);
	((unsigned int *)map)[10] = filesize - 44;

	double *data = new double[sizeof(double) * iend];

	for(auto i : score){
		const double t = 2 * M_PI * i.freq / samplerate;
		const std::complex<double> a(cos(t), sin(t));
		std::complex<double> x[2] = {1, 0};
		int istart = i.start * samplerate;
		int ilength = i.end * samplerate - istart;
		for(int j = 0; istart + j < iend; ++j){
			double tmp = x[j & 1].imag() * i.vel;
			if(j > ilength){
				tmp = 0;
			}
			data[istart + j] += tmp;
			x[~j & 1] = x[j & 1] * a;
		}
	}

	for(int i = 0; i < iend; ++i) for(int j = 0; j < channel; ++j) ((short *)map)[i * channel + j + 22] = data[i] * ((1 << 15) - 1);

	delete[] data;
	close(out);
	munmap(map, filesize);
}
