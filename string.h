#include <vector>
#include <iostream>
#include <memory>

class String{
public:
	std::vector<Char> text;
	bool match(const std::string &str) const;
	void append(const String &, const Char &);
	std::unique_ptr<char> c_str();
	friend std::ostream &operator<<(std::ostream &, const String &);
	friend bool operator<(const String &l, const String &r);
};
