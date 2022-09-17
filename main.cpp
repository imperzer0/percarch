#include "percarch.h"
#include "constants.hpp"
#include <getopt.h>
#include <cstdio>
#include <cstdlib>
#include <cstring>


static constexpr const char* short_args = "cdi:o:h";
static constexpr struct option long_args[] = {
		{ "compress",   no_argument,       nullptr, 'c' },
		{ "decompress", no_argument,       nullptr, 'd' },
		{ "input",      required_argument, nullptr, 'i' },
		{ "output",     required_argument, nullptr, 'o' },
		{ "help",       no_argument,       nullptr, 'h' },
		{ nullptr, 0,                      nullptr, 0 }
};


inline static void help()
{
	::printf(APPNAME " v" VERSION "\n");
	::printf("Usage: " APPNAME " [OPTIONS]...\n");
	::printf("Compresses file.\n");
	::printf("Options:\n");
	::printf(" --compress    | c          Compress file\n");
	::printf(" --decompress  | d          Decompress file\n");
	::printf(" --input       | i  <file>  Input file\n");
	::printf(" --output      | o  <file>  Output file\n");
	::printf(" --power       | p  <int>   Power of compression\n");
	::printf(" --help        | h          Show this help message.\n");
	::printf("\n");
	
	::exit(807);
}

int main(int argc, char** argv)
{
	char* input = nullptr, * output = nullptr;
	int option_index, option, compress = 0, decompress = 0;
	long power = 0;
	while ((option = ::getopt_long(argc, argv, short_args, long_args, &option_index)) > 0)
	{
		switch (option)
		{
			case 'c':
				compress = 1;
				break;
			case 'd':
				decompress = 1;
				break;
			case 'i':
				input = strdup(optarg);
				break;
			case 'o':
				output = strdup(optarg);
				break;
			case 'p':
				power = strtol(optarg, nullptr, 10);
				break;
			default:
				help();
		}
	}
	
	if (!(input && output && (compress ^ decompress)))
		help();
	
	if (compress)
		percarch_compress(input, output, power);
	//	else
	//		percarch_decompress(input, output, power);
	
	return 0;
}
