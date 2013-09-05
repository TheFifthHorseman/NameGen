#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include "stringops.h"
#include "wordlist.h"

inline struct WORDLIST blank_wordlist()
{
	struct WORDLIST returnValue;
	returnValue.entryCount=0;
	returnValue.dataStart=0;
	returnValue.entryList=0;
	return returnValue;
}

inline void init_wordlist(struct WORDLIST* wordlist)
{
	(*wordlist).entryCount=0;
	(*wordlist).dataStart=0;
	(*wordlist).entryList=0;
}

inline struct WORDLIST load_wordlist(char* fileName)
{
	struct WORDLIST wordlist;
	wordlist.entryList=file_get_lines(fileName, &(wordlist.entryCount));
	wordlist.dataStart=&(wordlist.entryList[0][0]);
	clean_string_array(wordlist.entryList, &(wordlist.entryCount));
	return wordlist;
}

inline void wordlist_from_string(char* string, struct WORDLIST* wordlist)
{
	(*wordlist).dataStart=string;
	split_string_DESTRUCTIVE(string, &(*wordlist).entryCount, &(*wordlist).entryList, (char*)WhiteSpace);
	clean_string_array((*wordlist).entryList, &(*wordlist).entryCount);
}

inline int wordlist_too_similar(struct WORDLIST firstWordList, struct WORDLIST secondWordList)
{
	int i, j;
	for (i=0; i<firstWordList.entryCount; ++i)
		if(strchr(firstWordList.entryList[i],'$')==NULL)
			for (j=0; j<secondWordList.entryCount; ++j)
				if(strchr(secondWordList.entryList[j],'$')==NULL && too_similar(firstWordList.entryList[i],secondWordList.entryList[j]))
					return 1;
	return 0;
}

inline char* random_wordlist_item(struct WORDLIST wordList)
{
	return random_string(wordList.entryList, wordList.entryCount);
}

inline void free_wordlist(struct WORDLIST *wordList)
{
	if((*wordList).dataStart)	free((*wordList).dataStart);
	if((*wordList).entryList)	free((*wordList).entryList);
}

inline void remove_wordlist_item(struct WORDLIST *wordList, int i)
{
    (*wordList).entryList[i]=(*wordList).entryList[--(*wordList).entryCount];
}

inline void prune_long_wordlist_entries(struct WORDLIST *wordList, unsigned int truncateTo)
{
    int i;
    for (i=0; i<(*wordList).entryCount; ++i)
        if ( strlen((*wordList).entryList[i])>=truncateTo )
            remove_wordlist_item(wordList, --i);
}
