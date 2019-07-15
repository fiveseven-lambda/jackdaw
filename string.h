#include <vector>
#include <iostream>

class String{
public:
	std::vector<Char> text;
	String(){}
	String(const std::vector<Char> &text): text(text) {}
	bool match(const std::string &str) const;
	void append(const String &, const Char &);
	char *c_str();
	friend std::ostream &operator<<(std::ostream &, const String &);
	friend bool operator<(const String &l, const String &r);
};
