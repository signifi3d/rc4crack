#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include "decrypt.h"
#include "dict.h"

int dict(FILE * wordlist, unsigned char * cipher) {
	//The longest undisputed word in english is 1909 letters long, 2000 should be plenty
	int buffer_len = 2000;
	unsigned char buffer[buffer_len];
	unsigned char result[strlen(cipher)+1];
	result[strlen(cipher)] = '\0';

	while ( fgets(buffer, buffer_len, wordlist) != NULL ) {
		bool possible = true;
		//Don't judge me for how I get rid of a new line character
		for (int i = 0; i < buffer_len; ++i) {
			if (buffer[i] == '\n') {
				buffer[i] = '\0';
				break;
			}
			else if (buffer[i] == '\0')
				break;
		}
		decrypt(buffer, cipher, result);

		//You've called strlen(cipher) three times now, just save the damn result
		for (int i = 0; i < strlen(cipher); ++i) {
			if ( result[i] < 0x20 || result[i] > 0x7E ) {
				possible = false;
				break;
			} 
		}
		if (possible)
			printf("Key: %s Possible Message: %s\n", buffer, result);
	}

	return 1;
}
