#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <stdlib.h>
#include "brute.h"
#include "dict.h"

//I feel ashamed for writing such a sloppy program...

char * lower = "abcdefghijklmnopqrstuvwxyz";
char * upper = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";
char * num   = "0123456789";
char * sym   = "~`!@#$%^&*()_-+={}[]|\\:;'<,>.?/\"";

//Convert a string representing hex numbers into an array of bytes
int build_cipher(unsigned char * in_string, unsigned char * out_cipher) {
	int		in_string_len = strlen(in_string);

	if ( in_string_len % 2 != 0 )
		return -1;

	for (int pos = 0; pos < in_string_len; ++pos ) {
		int shift = pos % 2 == 0 ? 4 : 0;
		if ( in_string[pos] >= 0x30 && in_string[pos] <= 0x39 ) 
			out_cipher[pos/2] += ( (in_string[pos] - 0x30) << shift );
		else if ( in_string[pos] >= 0x41 && in_string[pos] <= 0x46 )
			out_cipher[pos/2] += ( (in_string[pos] - 0x37) << shift);
		else if ( in_string[pos] >= 0x61 && in_string[pos] <= 0x66 )
			out_cipher[pos/2] += ( (in_string[pos] - 0x57) << shift);
		else
			return -1;
	}


	return 1;
}

//yeah, definitely need some help
void print_help() {
	printf("Usage: rc4crack -c HEXCIPHER {-b [-s luns][-m customcharset][-l lower upper] | -d -w /path/to/wordlist}[-h]\n");
	printf("\t\tLook, basically you input your cipher after -c then choose if you want a brute force (-b) or\n \
		dictionary (-d) attack. With a brute force attack you can choose what charsets you want to\n \
		include such as lower (l), upper (u), numeric (n), or symbols (s), just include them as one\n \
		string, no spaces. Or you can even specify a custom charset (-m), then you provide the upper\n \
		and lower limits (-l). Default charset is lun, default limits are 1 4. For a dictionary attack\n \
		just be sure to include a wordlist (-w).\n");
	exit(EXIT_SUCCESS);
}

//A slop trough of a main function
int main(int argc, char ** argv) {
	unsigned char 	* 	cipher = NULL;
	//no nuance, what is this? freshman programming class?
	char 		* 	charset = malloc(sizeof(char) * (strlen(lower) + strlen(upper) + strlen(num) + strlen(sym)));
	int			charset_len = 0;
	bool			brute_flag = true;
	bool			dict_flag = false;
	FILE		*	wordlist = NULL;
	int			lower_bound = 1;
	int			upper_bound = 4;

	if (argc == 1) 
		print_help();
	
	//Not how arguments should be handled.
	for (int arg = 1; arg < argc; ++arg) {
		if ( strcmp(argv[arg], "-c") == 0 ) {
			cipher = malloc(sizeof(char) * ((strlen(argv[arg+1])/2)));
			memset(cipher, 0, sizeof(char)*(strlen(argv[arg+1])/2));
			//Was too lazy to just pad out the string myself.
			if ( build_cipher(argv[arg+1], cipher) == -1 ) {
				fprintf(stderr, "That cipher is whack, only hex chars, maybe pad the beginning with a 0 or something. I dunno.\n");
				exit(EXIT_FAILURE);
			}
			++arg;			
		} else if ( strcmp(argv[arg], "-s") == 0 ) {
			for (int i = 0; i < strlen(argv[arg+1]); ++i) {
				switch(argv[arg+1][i]) {
					case 'l':
						charset_len += strlen(lower);
						strcat(charset, lower);
						break;
					case 'u':
						charset_len += strlen(upper);
						strcat(charset, upper);
						break;
					case 'n':
						charset_len += strlen(num);
						strcat(charset, num);
						break;
					case 's':
						charset_len += strlen(sym);
						strcat(charset, sym);
						break;
					default:
						fprintf(stderr, "That's not a charset option dummy. Only l, u, n, s.\n");
						exit(EXIT_FAILURE);
						break;
				}
			}
			++arg;
		} else if ( strcmp(argv[arg], "-m") == 0 ) {
			charset = malloc(sizeof(char) * strlen(argv[arg+1]));
			strcpy(charset, argv[arg+1]);
			++arg;
		} else if ( strcmp(argv[arg], "-b") == 0 ) {
			brute_flag = true;
			dict_flag = false;
		} else if ( strcmp(argv[arg], "-d") == 0 ) {
			dict_flag = true;
			brute_flag = false;
		} else if ( strcmp(argv[arg], "-w") == 0 ) {
			wordlist = fopen(argv[arg+1], "r");
			if (wordlist == NULL) {
				fprintf(stderr, "Yo, your file is messed up.\n");
				exit(EXIT_FAILURE);
			}
			++arg;
		} else if ( strcmp(argv[arg], "-l") == 0 ) {
			lower_bound = atoi(argv[arg+1]);
			upper_bound = atoi(argv[arg+2]);
			arg += 2;
		} else if ( strcmp(argv[arg], "-h") == 0 ) {
			print_help();
		}  else {
			fprintf(stderr, "Invalid argument: %s\n", argv[arg]);
		}
	}//So much can go wrong already, don't have the will to do anything about it. Just use the program correctly please.

	if ( cipher == NULL ) {
		fprintf(stderr, "Invalid cipher.\n");
		exit(EXIT_FAILURE);
	}
	if (brute_flag) {
		if ( charset_len == 0 ) { 
			charset_len = strlen(upper) + strlen(lower) + strlen(num);
			strcat(strcat(strcat(charset, upper), lower), num);
		}
		if ( lower_bound > upper_bound ) {
			fprintf(stderr, "You mixed up the lower and upper bound order. %d is bigger than %d...idiot.\n", lower_bound, upper_bound);
			exit(EXIT_FAILURE);
		}
		//Into the dungeon!
		brute(charset, charset_len, lower_bound, upper_bound, cipher);

	} else if (dict_flag) {
		dict(wordlist, cipher);
		fclose(wordlist);
	} else {
		fprintf(stderr, "I don't know how you did it, but you done messed up.\n");
	}

	//Oh thank god it's over. We made it. Please disregard the memory leaks. I'm sure you'll get them back someday.
	return EXIT_SUCCESS;
}
