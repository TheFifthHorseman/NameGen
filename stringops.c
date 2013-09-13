#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include "stringops.h"
inline int too_similar(char* a, char* b)
{
	int i;
	if (a==b) return 1;
	if(!a || !b) return 0;
	for (i=0; i<3; ++i)
		if (*a!=*b) return 0;
		else if (*(a++)==0 || *(b++)==0) return 1;
	return 1;
}

inline int count_character_occurences(char *string, char *charsToCount)
{
	int counter=0;
	if(string)
		while (*string)
			if (strchr(charsToCount, *(string++))!=NULL)
				++counter;
	return counter;
}

inline int split_string_DESTRUCTIVE(char* inputString, int* pieceCount, char*** ouputPieces, char* splitBy)
/* Split a char array into an array of char arrays using characters from a provided array as separators
 DESTRUCTIVE FOR INPUT ARGUMENT!!!*/
{
	char **pieces;
	int i;
	*pieceCount=count_character_occurences(inputString, splitBy)+1; /* Count how many substrings will result */
	*ouputPieces=pieces=(char **)malloc(*pieceCount*sizeof(char **));
	for (i=0;i<*pieceCount; i++) pieces[i]=strtok ((i?NULL:inputString), splitBy);
	return *pieceCount;
}

inline char* trim(char *t)
/* Pointer-safe inplace trim. */
{
	char *e;
	if (t==NULL) return NULL;
/*	if ((e=strstr(strings[i], "//")))	*e=0; */		 /* Remove any part of the string following a double-slash. */
	e=t+strlen(t);										 /* *e initially points to end of string */
	do *(e--)=0; while (strchr(WhiteSpace, *e) && e>=t); /* Find last char that is not \r\n\t */
	e=t+strspn (t,WhiteSpace);						     /* Find first char that is not \t */
	return (char*)(e>t?memmove(t,e,strlen(e)+1):t);		 /* memmove string contents and terminator */
}

inline void clean_string_array(char** strings, int* stringCount)
/* Trim strings in array, remove null-length entries and comments */
{
	int i;
	int blockComment;
	char *lineCommentStart, *blockCommentStart, *blockCommentEnd;
	blockComment=0;
	for (i=0; i<*stringCount; ++i)
	{
		if (trim(strings[i]))
		{
		    blockCommentStart=blockComment?strings[i]:strstr(strings[i], "/*");
		    blockCommentEnd=blockCommentStart?strstr(blockCommentStart, "*/"):NULL;
            lineCommentStart=strstr(strings[i], "//");
            while (blockCommentStart && blockCommentEnd)
            {
                blockComment=0;
                blockCommentEnd+=2;
                if (lineCommentStart && lineCommentStart<blockCommentEnd)
                {
                    *blockCommentStart=0;
                    lineCommentStart=NULL;
                }
                else
                {
                    memmove(blockCommentStart,blockCommentEnd,strlen(blockCommentEnd)+1);
                    blockComment=(int)(blockCommentStart=strstr(strings[i], "/*"));
                    blockCommentEnd=blockCommentStart?strstr(blockCommentStart, "*/"):NULL;
                }
            }
            if (lineCommentStart)
                *lineCommentStart=0;
            if ((blockComment=(int)(blockCommentStart=strstr(strings[i], "/*"))))
                *blockCommentStart=0;
		}
		if (strings[i]==NULL || strings[i][0]==0 /*|| strings[i][0]=='/'*/)
		{
			if (i<--*stringCount)
                memmove(&strings[i], &strings[i+1], sizeof(char*)*(*stringCount-i));
			i--;
		}
	}
}

inline char* random_string(char** stringArray, int stringCount)
{
	switch (stringCount)
	{
		case 0: return NULL;
		case 1: return stringArray[0];
		default:return stringArray[rand()%stringCount];
	}
}

inline char* file_get_text(char* fileName)
{
	FILE * sourceFile;
	char * loadedData;
	size_t fileLength;
	if ((sourceFile=fopen(fileName, "rb")))
	{
		fseek (sourceFile, 0, SEEK_END);
		fileLength=ftell(sourceFile);
		fseek (sourceFile, 0, SEEK_SET);
		loadedData=(char*)malloc((fileLength+1)*sizeof(char));
		loadedData[fread(loadedData, 1, fileLength, sourceFile)]=0; /* it's used as a string - ie must be null-terminated! */
		fclose(sourceFile);
		return loadedData;
	}
	else
	{
		fprintf(stderr, "Failed to read contents from [%s]\n", fileName);
		return NULL;
	}
}

inline char** file_get_lines(char* fileName, int* lineCount)
{
	char** lines;
	split_string_DESTRUCTIVE(file_get_text(fileName), lineCount, &lines, "\n");
	return lines;
}
