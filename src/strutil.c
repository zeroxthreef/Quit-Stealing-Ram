#include "strutil.h"

void trim_leading(char* str) {
	int index, i, j;

	index = 0;

	/* Find last index of whitespace character */
	while(str[index] == ' ' || str[index] == '\t' || str[index] == '\n') {
		index++;
	}


	if(index != 0) {
		/* Shift all trailing characters to its left */
		i = 0;
		while(str[i + index] != '\0') {
			str[i] = str[i + index];
			i++;
		}
		str[i] = '\0'; // Make sure that string is NULL terminated
		
	}
}

void trim_trailing(char* str) {
	int index, i;

	index = -1;

	i = 0;
	while(str[i] != '\0') {
		if(str[i] != ' ' && str[i] != '\t' && str[i] != '\n') {
			index= i;
		}

		i++;
	}

	/* Mark next character to last non-white space character as NULL */
	str[index + 1] = '\0';
}

