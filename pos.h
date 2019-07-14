#include <ostream>

template<class T> class Pos{};

template<>
class Pos<Char>{
	const Char &ch;
public:
	explicit Pos(const Char &ch): ch(ch) {}
	friend std::ostream &operator<<(std::ostream &, const Pos &);
};

template<>
class Pos<String>{
	const String &str;
public:
	explicit Pos(const String &str): str(str) {}
	friend std::ostream &operator<<(std::ostream &, const Pos &);
};

template<class T>
Pos<T> pos(const T &obj){
	return Pos<T>(obj);
}
