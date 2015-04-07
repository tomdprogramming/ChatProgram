/**************************************************
* This is the source code for my own input/output
* library, basically copied from the cs50.h file,
* which was seemingly copied from another library.
* I am writing this as a learning aid, not because
* I think I can improve on it
* 
***************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>

#include "tomIO.h"

char* GetString(void)
{
	//start by declaring a buffer into which the input can be read
	//this buffer will be expanded when necessary to accommodate
	//longer strings. It is declared as a NULL pointer to start with
	char* buffer = NULL;
	
	//an integer to store the capacity of the buffer, unsigned as it is
	//can never be negative
	unsigned int capacity = 0;
	
	//and integer to store the number of chars actually stored in the buffer
	unsigned int n = 0;
	
	//variable to store the character that is read in (or EOF marker)
	int c;
	
	//iteratively read in the chars from the standard input
	while ( (c=fgetc(stdin)) != '\n' && c!=EOF)
	{
		//first thing to do is check if the buffer can hold the next char
		// and if necessary, expand the buffer 
		if(n+1 > capacity)
		{
			//determine new capacity, start at 32, then double
			if(capacity == 0)
			{
				capacity = 32;
			}
			else if(capacity <= (UINT_MAX/2))
			{
				capacity *= 2; //shorthand for capacity = capacity*2;
			}
			else
			{
				free(buffer);
				return 0;
			}
			
			//if we still have the space, we can now expand the buffer's capacity
			char* temp = realloc(buffer, capacity*sizeof(char));
			
			//check that realloc worked, i.e that there was sufficient memory
			//for te expanded buffer
			if(temp==NULL)
			{
				free(buffer);
				return 0;
			}
			
			//now change buffer to point to the same place as temp
			buffer = temp;
		}
		
		//if we reach this point, buffer will definitely have enough space for the extra
		//char, since it either had space anyway and skipped the if condition, or it was 
		//expanded. NB n is incremented here
		buffer[n++] = c;
	}
	
	//returns NULL is user provided no input
	if(n==0 && c==EOF)
	{
		return NULL;
	}
	
	//now we need to minimise the buffer
	char* minimal = malloc((n+1)*sizeof(char));
	if(minimal==NULL)
	{
		free(buffer);
		return NULL;
	}
	strncpy(minimal, buffer, n);
	free(buffer);	
	minimal[n] = '\0';
	
	return minimal;
	
}




