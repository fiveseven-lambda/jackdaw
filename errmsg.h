class Char;
class String;

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

void error_open(const char []);
void error_open_twice(const char []);
void error_unterminated_comment(const Char &);
void error_unexpected_end_of_file(const Char &);
void error_unexpected_character(const Char &);
void error_unknown_command(const String &);
void error_null_macro_defined(const Char &);
void error_duplicate_macro(const String &, const String &);
void error_null_macro(const Char &);
void error_undefined_macro(const String &);
