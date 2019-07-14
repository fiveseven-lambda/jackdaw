#include <ostream>

template<class T> class Pos{};

template<>
class Pos<Char>{
	const Char &ch;
public:
	explicit Pos(const Char &ch): ch(ch) {}
	friend std::ostream &operator<<(std::ostream &, const Pos &);
};
