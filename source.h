#include <fstream>
#include <set>
#include <memory>

class Source{
	std::string filename;
	struct less{
		bool operator()(const std::unique_ptr<Source> &a, const std::unique_ptr<Source> &b){
			return a->filename < b->filename;
		}
	};
	static std::set<std::unique_ptr<Source>, less> list;
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

