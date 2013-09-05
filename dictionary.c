#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include "stringops.h"
#include "wordlist.h"
#include "dictionary.h"

inline struct DICTIONARY blank_dictionary()
{
	struct DICTIONARY returnValue;
	returnValue.entryCount=0;
	returnValue.dataStart=0;
	returnValue.entryList=0;
	return returnValue;
}

inline void init_dictionary(struct DICTIONARY* dictionary)
{
	(*dictionary).entryCount=0;
	(*dictionary).dataStart=0;
	(*dictionary).entryList=0;
}

inline struct WORDLIST random_dictionary_item(struct DICTIONARY dictionary)
{
	switch (dictionary.entryCount)
	{
		case 0: return blank_wordlist();
		case 1: return dictionary.entryList[0];
		default:return dictionary.entryList[rand()%dictionary.entryCount];
	}
}

inline void dictionary_from_wordlist(struct WORDLIST wordList, struct DICTIONARY* targetDictionary)
{
	int i;
	(*targetDictionary).dataStart=wordList.dataStart;
	(*targetDictionary).entryCount=wordList.entryCount;
	(*targetDictionary).entryList=(struct WORDLIST*)malloc(sizeof(struct WORDLIST)*wordList.entryCount);
	for (i=0; i<wordList.entryCount; ++i)
	{
		wordlist_from_string(wordList.entryList[i], &((*targetDictionary).entryList[i]));
		(*targetDictionary).entryList[i].dataStart=0;
	}
	wordList.dataStart=0;
	free_wordlist(&wordList);
}


inline void dictionary_from_file(char* fileName, struct DICTIONARY* targetDictionary)
{
	struct WORDLIST wordList=load_wordlist(fileName);
	dictionary_from_wordlist(wordList,targetDictionary);
}

inline void free_dictionary(struct DICTIONARY* dictionary)
{
	if((*dictionary).dataStart!=0) 		free((*dictionary).dataStart);
	while ((*dictionary).entryCount>0) 	free_wordlist(&(*dictionary).entryList[--(*dictionary).entryCount]);
	if((*dictionary).entryList !=0) 	free((*dictionary).entryList);
}

inline void remove_dictionary_item(struct DICTIONARY* dictionary, int i)
{
    free_wordlist(&((*dictionary).entryList[i]));
    (*dictionary).entryList[i]=(*dictionary).entryList[--(*dictionary).entryCount];
}

inline void prune_long_dictionary_entries(struct DICTIONARY *dictionary, int truncateTo)
{
    int i;
    for (i=0; i<(*dictionary).entryCount; ++i)
    {
        prune_long_wordlist_entries(&((*dictionary).entryList[i]), truncateTo);
        if ( 0==(*dictionary).entryList[i].entryCount)
            remove_dictionary_item(dictionary, i--);
    }
}
