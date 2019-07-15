#include <getopt.h>
#include "char.h"
#include "source.h"
#include "load.h"
#include "score.h"

int verbosity;

int main(int argc, char *argv[]){
	const char *out_filename = "a.wav";
	[&]{ for(;;){
		int long_optind;
		static struct option long_options[] = {
			{
				.name = "out",
				.has_arg = required_argument,
				.flag = NULL,
				.val = 'o'
			},{
				.name = "version",
				.has_arg = no_argument,
				.flag = NULL,
				.val = 'v'
			},{
				.name = "verbose",
				.has_arg = no_argument,
				.flag = NULL,
				.val = 0
			}, {
				.name = "help",
				.has_arg = no_argument,
				.flag = NULL,
				.val = 'h'
			}
		};
		switch(getopt_long(argc, argv, "o:vh", long_options, &long_optind)){
			case -1:
				return;
			case 0:
				switch(long_optind){
					case 2:
						verbosity = 1;
						break;
				}
				break;
			case 'o':
				out_filename = optarg;
				break;
			case 'v':
				puts("jackdaw version 4.1");
				exit(0);
			case 'h':
				puts("usage: jackdaw [-v --version] [-h --help] [-o --out <filename>] [--verbose]");
				exit(0);
		}
	} }();

	for(;optind < argc; ++optind){
		load(*(new Source(argv[optind])));
	}

	wav(out_filename);
}
