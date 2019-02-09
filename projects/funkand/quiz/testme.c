/******************
 * Name: testme.c
 * Author (of inputChar & inputString functions): Andrew Funk
 * Date: 02-08-19
 * Description: The following program demonstrates random testing. Two 
 * 				functions are implmemented to generate complete coverage 
 * 				of all statements in the testme() function. This is
 * 				shown by reaching the error condition in the while loop.
 *****************/

#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<time.h>

char inputChar()
{
	char randLetter;

	// Generate a random character from 32 to 125 in the ASCII char set
	randLetter = 32 + rand() % 94;

	return randLetter;
}

char *inputString()
{
	static int STRING_SIZE = 7;
	int randInt, i;
	char* string;

	// Allocate space for a string
	string = malloc(STRING_SIZE * sizeof(char));
	// Clear the character array
	memset(string, '\0', STRING_SIZE * sizeof(char));

	// Set each character in string to a limited random value
	for(i = 0; i < STRING_SIZE - 1; i++)
	{
		// Generate random number from 0 to 5
		randInt = rand() % 6;
		if(randInt == 0){
			string[i] = 'r';
		}
		if(randInt == 1){
			string[i] = 'e';
		}
		if(randInt == 2){
			string[i] = 's';
		}
		if(randInt == 3){
			string[i] = 'e';
		}
		if(randInt == 4){
			string[i] = 't';
		}
		if(randInt == 5){
			string[i] = '\0';
		}
	}
	return string;
}

void testme()
{
	int tcCount = 0;
	char *s;
	char c;
	int state = 0;

	while (1)
	{
		tcCount++;
		c = inputChar();
		s = inputString();
		printf("Iteration %d: c = %c, s = %s, state = %d\n", tcCount, c, s, state);
		if (c == '[' && state == 0) state = 1;
		if (c == '(' && state == 1) state = 2;
		if (c == '{' && state == 2) state = 3;
		if (c == ' '&& state == 3) state = 4;
		if (c == 'a' && state == 4) state = 5;
		if (c == 'x' && state == 5) state = 6;
		if (c == '}' && state == 6) state = 7;
		if (c == ')' && state == 7) state = 8;
		if (c == ']' && state == 8) state = 9;
		if (s[0] == 'r' && s[1] == 'e'
			&& s[2] == 's' && s[3] == 'e'
			&& s[4] == 't' && s[5] == '\0'
			&& state == 9)
		{
			free(s);
			printf("error ");
			exit(200);
		}
		// Free allocated memory	
		free(s);
	}
}

int main(int argc, char *argv[])
{
	srand(time(NULL));
	testme();
	return 0;
}
