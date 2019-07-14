#include "char.h"
#include "source.h"
#include "errmsg.h"

bool Source::eof(){
	return fs.eof();
}

Source::Source(const char filename_cstr[]):
	filename(filename_cstr),
	fs(filename_cstr, std::ios::in),
	line(0),
	pos(0),
	buf_comment(filename),
	buf_peek(filename)
{
}

void Source::next(Char &ch){
	ch.set(fs.get(), line, pos);
	if(ch == '\n'){
		++line;
		pos = 0;
	}else{
		++pos;
	}
}

Char Source::get(bool peek){
	if(!buf_peek.isbuf){
		if(buf_comment.isbuf){
			buf_peek.buf = buf_comment.buf;
			buf_comment.isbuf = false;
		}else{
			next(buf_peek.buf);
		}
		if(buf_peek.buf == '/'){
			next(buf_comment.buf);
			if(buf_comment.buf == '+'){
				bool plus = false;
				for(;;){
					Char tmp = get();
					if(eof()) error_unterminated_comment(tmp);
					else if(tmp == '+') plus = true;
					else if(plus && tmp == '/') break;
					else plus = false;
				}
				return get(peek);
			}else{
				buf_comment.isbuf = true;
			}
		}
	}
	buf_peek.isbuf = peek;
	return buf_peek.buf;
}
