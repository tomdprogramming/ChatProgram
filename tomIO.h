/**************************************************
* header file for my own input functions. Basically copied from the cs50.h
* file, which was seemingly copied from another library
* 
***************************************************/

#ifndef _TOMIO_H //standard include guards
#define _TOMIO_H

/*
 * Function GetString(), reads a line of text from standard input
 * and returns it as a string (char*), without trailing newline
 * character (so, if user inputs only "\n", returns "", not NULL.
 * Returns NULL upon error or no input whatsoever (i.e. just EOF).
 * Leading and trailing whitespace is not ignored. Stores string
 * on heap (via malloc() and realloc());
 * N.B. MEMORY MUST BE FREED BY CALLER TO AVOID LEAK.
 */
 char* GetString(void);


#endif
