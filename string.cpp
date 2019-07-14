#include "char.h"
#include "string.h"

bool String::match(const std::string &str) const{
	if(text.size() != str.size()) return false;
	for(size_t i = 0; i < text.size(); ++i) if(text[i] != str[i]) return false;
	return true;
}
void String::append(const String &str, const Char &ch){
	size_t size = text.size();
	text.resize(size + str.text.size(), ch);
	for(size_t i = 0; i < str.text.size(); ++i){
		text[size + i] = str.text[i];
		text[size + i].add_pos(ch);
	}
}
std::ostream &operator<<(std::ostream &os, const String &str){
	for(Char i : str.text) os << i;
	return os;
}
bool operator<(const String &l, const String &r){
	std::vector<Char>::const_iterator il = l.text.begin(), ir = r.text.begin();
	for(;; ++il, ++ir){
		if(ir == r.text.end()) return false;
		else if(il == l.text.end()) return true;
		else if(*il > *ir) return false;
		else if(*il < *ir) return true;
	}
}
