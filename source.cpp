#include "char.h"
#include "source.h"
#include "errmsg.h"

std::set<std::unique_ptr<Source>, Source::less> Source::list;

bool Source::eof(){
	return fs.eof();
}

Source::Source(const char filename_cstr[]):
	filename(filename_cstr),
	line(0),
	pos(0),
	buf_comment(filename),
	buf_peek(filename)
{
	auto ptr = std::unique_ptr<Source>(this);
	if(!list.insert(std::move(ptr)).second){
		error_open_twice(filename_cstr);
	}else{
		fs.open(filename_cstr, std::ios::in);
		if(!fs){
			error_open(filename_cstr);
		}
	}
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
