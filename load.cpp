#include <map>
#include "char.h"
#include "string.h"
#include "source.h"
#include "load.h"
#include "errmsg.h"
#include "score.h"

class Sentence{
public:
	String command;
	std::vector<String> arguments;
	void run();
};

std::map<const String, const String> macros;

void load(Source &source){
	Sentence sentence;

	for(;;){
		while(isspace(source.get(1))) source.get();

		Char tmp = source.get();

		if(source.eof()){
			sentence.run();
			return;
		}else if(tmp == '{'){
			String argument;
			for(;;){
				Char tmp = source.get();
				if(source.eof()) error_unexpected_end_of_file(tmp);
				else if(tmp == '}') break;
				else if(tmp == '{'){
					String buf;
					for(;;){
						Char tmp = source.get();
						if(tmp == '}') break;
						else buf.text.push_back(tmp);
					}
					auto it = macros.find(buf);
					if(it == macros.end()){
						error_undefined_macro(buf);
					}else{
						argument.append(it->second, tmp);
					}
				}else{
					argument.text.push_back(tmp);
				}
			}
			sentence.arguments.push_back(argument);
		}else if(isalpha(tmp)){
			if(!sentence.command.text.empty()){
				sentence.run();
				sentence.command.text.clear();
				sentence.arguments.clear();
			}
			sentence.command.text.push_back(tmp);
			while(isalpha(source.get(1))) sentence.command.text.push_back(source.get());
		}else{
			error_unexpected_character(tmp);
		}
	}

}

void Sentence::run(){
	if(command.match(std::string("message"))){
		if(arguments.size() == 1){
			std::cout << arguments[0];
		}
	}else if(command.match(std::string("define"))){
		if(arguments.size() == 2){
			auto it = macros.find(arguments[0]);
			if(it == macros.end()){
				macros.insert(std::make_pair(arguments[0], arguments[1]));
			}else{
				error_duplicate_macro(it->first, arguments[0]);
			}
		}
	}else if(command.match(std::string("score"))){
		if(arguments.size() == 1){
			addscore(arguments[0]);
		}
	}else if(command.match(std::string("import"))){
		if(arguments.size() == 1){
			Source source(arguments[0].c_str().get());
			load(source);
		}
	}else if(command.match(std::string("debug"))){
		if(arguments.size() == 1){
			for(auto i : arguments[0].text){
				error_unexpected_character(i);
			}
		}
	}else{
		error_unknown_command(command);
	}
}
