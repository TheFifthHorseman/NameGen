#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include "stringops.h"
#include "wordlist.h"

struct WORDLIST blank_wordlist()
{
	struct WORDLIST returnValue;
	returnValue.entryCount=0;
	returnValue.dataStart=0;
	returnValue.entryList=0;
	return returnValue;
}

void init_wordlist(struct WORDLIST* wordlist)
{
	(*wordlist).entryCount=0;
	(*wordlist).dataStart=0;
	(*wordlist).entryList=0;
}

struct WORDLIST load_wordlist(char* fileName)
{
    struct WORDLIST wordlist;
	wordlist.entryList=file_get_lines(fileName, &(wordlist.entryCount));
	wordlist.dataStart=&(wordlist.entryList[0][0]);
	clean_string_array(wordlist.entryList, &(wordlist.entryCount));
	return wordlist;
}

struct WORDLIST load_wordlist_multi(char* fileNames)
{
	struct WORDLIST wordlist;
	wordlist.entryList=files_get_lines(fileNames, &(wordlist.entryCount));
	wordlist.dataStart=&(wordlist.entryList[0][0]);
	clean_string_array(wordlist.entryList, &(wordlist.entryCount));
	return wordlist;
}

void wordlist_from_string(char* string, struct WORDLIST* wordlist)
{
	(*wordlist).dataStart=string;
	split_string_DESTRUCTIVE(string, &(*wordlist).entryCount, &(*wordlist).entryList, (char*)WhiteSpace);
	clean_string_array((*wordlist).entryList, &(*wordlist).entryCount);
}
/* Deprecated. Rewrite using fingerprints?
int wordlist_too_similar(struct WORDLIST firstWordList, struct WORDLIST secondWordList)
{
	int i, j;
	for (i=0; i<firstWordList.entryCount; ++i)
		if(strchr(firstWordList.entryList[i],'$')==NULL)
			for (j=0; j<secondWordList.entryCount; ++j)
				if(strchr(secondWordList.entryList[j],'$')==NULL && too_similar(firstWordList.entryList[i],secondWordList.entryList[j]))
					return 1;
	return 0;
}
*/
char* random_wordlist_item(struct WORDLIST wordList)
{
	return random_string(wordList.entryList, wordList.entryCount);
}

void free_wordlist(struct WORDLIST *wordList)
{
	if((*wordList).dataStart)	free((*wordList).dataStart);
	if((*wordList).entryList)	free((*wordList).entryList);
}

void remove_wordlist_item(struct WORDLIST *wordList, int i)
{
    (*wordList).entryList[i]=(*wordList).entryList[--(*wordList).entryCount];
}

void prune_long_wordlist_entries(struct WORDLIST *wordList, unsigned int truncateTo)
{
    int i;
    for (i=0; i<(*wordList).entryCount; ++i)
        if ( strlen((*wordList).entryList[i])>=truncateTo )
            remove_wordlist_item(wordList, i--);
}

void remove_entry(struct WORDLIST *wordList, char* entry)
{
    int i;
    for (i=0; i<(*wordList).entryCount; ++i)
        if ( strcmp((*wordList).entryList[i], entry )==0)
        {
            remove_wordlist_item(wordList, i--);
            return;
        }
}

void load_hardcoded_list(char* source, struct WORDLIST* wordList)
{
    int k;
    wordlist_from_string(_strdup(source), wordList);
/*    (*wordList).dataStart=NULL;*/
    for (k=0; k< (*wordList).entryCount; ++k)
        if (!memcmp ( (*wordList).entryList[k], "_", 2))
            (*wordList).entryList[k][0]=0;
}
