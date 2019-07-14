#include "char.h"

Char::Pos::Pos(const std::string &filename) : filename(filename){}
Char::Char(const std::string &filename) : pos(1, Pos(filename)) {}
void Char::set(char val, int line, int pos){
	this->val = val;
	this->pos.front().line = line;
	this->pos.front().pos = pos;
}
Char &Char::operator=(const Char &ch){
	val = ch.val;
	pos.front().line = ch.pos.front().line;
	pos.front().pos = ch.pos.front().pos;
	return *this;
}
Char::operator char() const {
	return val;
}
void Char::add_pos(const Char &ch){
	pos.push_back(ch.pos.front());
}
