#include <fstream>

class Source{
	std::string filename;
	std::ifstream fs;
	int line, pos;
	struct Buffer{
		Char buf;
		bool isbuf;
		Buffer(const std::string &filename): buf(filename), isbuf(false) {}
	} buf_comment, buf_peek;
	void next(Char &ch);
public:
	bool eof();
	Source(const char []);
	Char get(bool = 0);
};
