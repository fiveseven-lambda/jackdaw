class Char;
class String;
void error_unterminated_comment(const Char &);
void error_unexpected_end_of_file(const Char &);
void error_unexpected_character(const Char &);
void error_unknown_command(const String &);
void error_undefined_macro(const String &);
void error_duplicate_macro(const String &, const String &);
