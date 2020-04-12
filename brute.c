#include <string.h>
#include <stdio.h>
#include <stdbool.h>
#include "pos_stack.h"
#include "decrypt.h"
#include "brute.h"

//The monstrosity
int brute(char * charset, int charset_len, int min, int max, char * cipher) {	
	int cipher_len = strlen(cipher);

	//Loop through each key length option
	for ( int len = min; len <= max; ++len) {
		//Gotta set these up with null values else weird stuff happens
		char key[len+1];
		key[len] = '\0'; 
		char result[cipher_len+1];
		result[cipher_len] = '\0';
		pos_stack_t stack;
		create_pos_stack(len, &stack);

		/*
			This ugly beast does some iterative backtracking to make all the combinations of
			the specified length. Can't do it recursively else it'll blow up the stack frame. 
			She ain't the prettiest, but she gets the job done. 
		*/
		for ( int i = 0; i < len; ++i ) {
			int chst;
			bool possible = true;
			//If there's nothing in the key, fill it up from the beginning of the charset
			if (ps_size(&stack) == 0 || ps_top_res(&stack) == i-1) {
				chst = 0;
				ps_push(i, chst, &stack);
				key[i] = charset[chst];
			//If we're back to the position we're iterating through, then move up or pop completely
			} else if (ps_top_res(&stack) == i) {
				chst = ps_top_chst(&stack) + 1;
				ps_pop(&stack);
				if ( chst != charset_len) {
					ps_push(i, chst, &stack);
					key[i] = charset[chst]; 
				} 
			//This'll bring us back down to an earlier position after later positions have been exhausted
			} else if ( ps_top_res(&stack) != i && i != 0 ) {
				while (ps_top_res(&stack) != i)
					--i;
				--i;	
			}
			//If we're on the last position then we have a key to try
			if (i == len-1) {
				//If the last position hasn't exhaused the charset, try it out
				if (chst != charset_len) {
					decrypt(key, cipher, result);
					for (int pos = 0; pos < cipher_len; ++pos) {
						//Check for non-printing characters, we don't wanna see 'em
						if (result[pos] < 0x20 || result[pos] > 0x7E) {
							possible = false;
							break;
						}
					}
					if (possible)
						printf("Key: %s Possible Message: %s\n", key, result);
				}
				//Go back by one if the last position hasn't exhausted the charset
				if ( ps_top_res(&stack) == i ) {
					--i;
					continue;
				//If the stack is empty then every position has exhausted the charset
				} else if ( ps_size(&stack) == 0 ) 
					break;
				else
					i-=2;
			} 
		}
	}
	//Good lord this is some ugly code
	return 0;
}
