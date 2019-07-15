#include "char.h"
#include "string.h"

bool String::match(const std::string &str) const{
	if(text.size() != str.size()) return false;
	for(size_t i = 0; i < text.size(); ++i) if(text[i] != str[i]) return false;
	return true;
}
void String::append(const String &str, const Char &ch){
	for(auto i : str.text){
		text.push_back(i);
		text.back().add_pos(ch);
	}
}
char *String::c_str(){
	char *ret = new char[text.size() + 1];
	for(size_t i = 0; i < text.size(); ++i) ret[i] = text[i];
	ret[text.size()] = '\0';
	return ret;
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
