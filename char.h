#include <vector>
#include <string>
#include <iostream>

template<class> class Pos;

class Char{
	char val;
	struct Pos{
		const std::string &filename;
		int line, pos;
		Pos(const std::string &);
	};
	std::vector<Pos> pos;
public:
	Char(const std::string &filename);
	Char(const Char &) = default;
	void add_pos(const Char &);
	void set(char, int, int);
	Char &operator=(const Char &);
	operator char() const;
	friend std::ostream &operator<<(std::ostream &, const ::Pos<Char> &);
};

