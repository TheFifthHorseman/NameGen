#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include "stringops.h"
#include "wordlist.h"
#include "dictionary.h"
#include "namegen.h"
#include "string_fingerprint.h"
/* Buffer size for file names and output string. Should be larger than maximum expected length of either of these. */
#define NAMEGEN_BUFFER_SIZE 512
/* How many times to retry generation if:
    * The output is too long (change pattern if retries run out)
    * The word may be a synonym of one already used (keep last selection if retries run out)
    Larger values = slower generation, so be reasonable */
/* #define MAXIMUM_RETRIES 100 */
/*#define MAXIMUM_RETRIES 30*/
#define MAXIMUM_RETRIES 100
/* How many levels of token nesting are permitted until breaking generation as a potentially infinite loop */
/* #define MAXIMUM_TOKEN_DEPTH MAXIMUM_RETRIES */
#define MAXIMUM_TOKEN_DEPTH 5

char* hashMapping=NULL;
char* hashExists=NULL;
int hashTableSize=0;
int hashLength=0;
char bitPos[8]={ '\x01','\x02','\x04','\x08','\x10','\x20','\x40','\x80',  };
static void load_manifest_entry (char* manifestEntry, struct DICTIONARY* dictionaries, struct WORDLIST* wordListChoices, char* hardcodedWordListIndices)
{
    int j;
	char  *patternPosition, *fileNameBuffer;
	fileNameBuffer=(char*)calloc(NAMEGEN_BUFFER_SIZE,sizeof(char));
    if (strlen(manifestEntry)>3)
    {
        j=(int)*(patternPosition=trim(manifestEntry));
        if (dictionaries[j].entryCount==0 && wordListChoices[j].entryCount==0)
            switch(*(patternPosition+=strspn (patternPosition+1,WhiteSpace)+1))
            {
                case '@':
                    dictionary_from_files(patternPosition, &dictionaries[j]);
/*                    dictionary_from_file(get_mod_file_path_2("Galaxy",patternPosition+1, fileNameBuffer), &dictionaries[j]);*/
                    break;
                default:
					/*Is this the cause???*/
                    load_hardcoded_list(patternPosition, &wordListChoices[j]);
                    hardcodedWordListIndices[strlen(hardcodedWordListIndices)]=j;
                    break;
            }
    }
    free(fileNameBuffer);
}

static void dictionary_init(unsigned int maximumOutputLength, char* manifestFile, struct DICTIONARY* dictionaries, struct WORDLIST* wordListChoices, char** hardcodedIndicesOut)
{
    int i;
	char  *fileNameBuffer;
	struct WORDLIST manifest/*, *manifest_ptr*/;
	char *hardcodedWordListIndices;
    for (i=0; i<256; ++i)
	{
		dictionaries[i]=blank_dictionary();
		wordListChoices[i]=blank_wordlist();
	}
	hardcodedWordListIndices=(char*)calloc(256,sizeof(char));
	fileNameBuffer=(char*)calloc(NAMEGEN_BUFFER_SIZE, sizeof(char));
	manifest=load_wordlist(get_mod_file_path_2("Galaxy", manifestFile, fileNameBuffer));
/*	manifest_ptr=&manifest;*/
	for (i=0; i<manifest.entryCount; ++i)
	{
/*		fprintf(stderr, "[%s]\n", manifest.entryList[i]); */
        load_manifest_entry(manifest.entryList[i], dictionaries, wordListChoices, hardcodedWordListIndices);
	}
    for (i=0; i<256; ++i)
        prune_long_dictionary_entries( &(dictionaries[i]), maximumOutputLength );

    *hardcodedIndicesOut=hardcodedWordListIndices;
	free_wordlist(&manifest);
	free(fileNameBuffer);
}

static void get_non_colliding_wordlist(struct DICTIONARY* dictionaries, struct WORDLIST* wordListChoices, char* hardcodedWordListIndices, int i)
{
    int j, redo, retries=MAXIMUM_RETRIES;
    int* hashes;
    int hashCount;
    int t;
    if (dictionaries[i].entryCount>0 && strchr(hardcodedWordListIndices,i)==NULL)
    {
        for (j=hashCount=0; j<dictionaries[i].entryCount;++j)
            if (dictionaries[i].entryList[j].entryCount>hashCount)
                hashCount=dictionaries[i].entryList[j].entryCount;
        hashes=(int*)calloc(hashCount,sizeof(int));

        do
        {
            redo=0;
            wordListChoices[i]=random_dictionary_item(dictionaries[i]);
            memset(hashes,0,sizeof(int)*hashCount);
            for (j=0; j<wordListChoices[i].entryCount && !redo; ++j)
            {
                t=string_fingerprint(wordListChoices[i].entryList[j], hashMapping, hashLength);
                if ((hashExists[t>>3])&(bitPos[t&7]))
                    redo=1;
                hashes[j]=t;
            }
        } while (redo && --retries);
        /* if the selection ended up failing retries, fill in the missing hashes */
        if (!redo)
            for (j=0; j<wordListChoices[i].entryCount; ++j)
                hashes[j]=string_fingerprint(wordListChoices[i].entryList[j], hashMapping, hashLength);
        /* add the hashes to the existence array */
        for (j=0; j<wordListChoices[i].entryCount; ++j)
        {
            t=hashes[j];
            hashExists[t>>3]|=bitPos[t&7];
        }
        free(hashes);
    }
}

static void namegen_cleanup(char* appendPointer, char* fileNameBuffer, struct DICTIONARY* dictionaries, struct WORDLIST* patterns, struct WORDLIST* wordListChoices, char* hardcodedWordListIndices)
{
    int i,j;
	free (appendPointer);
	free (fileNameBuffer);
	for (i=0; i<256; ++i)
		free_dictionary(dictionaries+i);
	free_wordlist(patterns);
	for (i=0; i<(int)strlen(hardcodedWordListIndices); ++i)
		if (wordListChoices[j=hardcodedWordListIndices[i]].entryCount)
			free_wordlist(wordListChoices+j);
	free(hardcodedWordListIndices);
    free(hashMapping);
    free(hashExists);
	free(dictionaries);
	free(wordListChoices);
    hashExists=hashMapping=NULL;
}

static void substitute_tokens(char* patternPosition, char* resultString, struct DICTIONARY* dictionaries, struct WORDLIST* wordListChoices, char* hardcodedWordListIndices)
{
    char* appendPointer;
    int i;
    appendPointer=(char*)memset((void*)resultString,0,sizeof(char)*NAMEGEN_BUFFER_SIZE);
    while(*patternPosition)
        switch (*patternPosition)
        {
            case '$':
                i=(int)*(++patternPosition);
                ++patternPosition;
                get_non_colliding_wordlist(dictionaries, wordListChoices, hardcodedWordListIndices, i);
                if (wordListChoices[i].entryCount>0)
                    appendPointer+=sprintf(appendPointer, "%s", random_wordlist_item(wordListChoices[i]));
                else
                    fprintf(stderr, "No wordlist or dictionary for token $%c\n", i);
                break;
            default:
                *(appendPointer++)=*(patternPosition++);
                break;
        }
}

static void grammar_produce (struct DICTIONARY* dictionaries, struct WORDLIST* wordListChoices, char* hardcodedWordListIndices, char* selectedPattern, char* resultString, int maximumOutputLength)
{
    char* patternPosition;
    char* fileNameBuffer;
    int tokenLoops;
    int i;
    fileNameBuffer=(char*)calloc(NAMEGEN_BUFFER_SIZE, sizeof(char));
    patternPosition=strcpy(fileNameBuffer, selectedPattern);
    tokenLoops=MAXIMUM_TOKEN_DEPTH;
    memset(hashExists,0x00,hashTableSize);
    do
    {
        for (i=0;i<256; ++i)
            if(strchr(hardcodedWordListIndices,i)==NULL)
                wordListChoices[i].entryCount=0;
        substitute_tokens(patternPosition, resultString, dictionaries, wordListChoices, hardcodedWordListIndices);
        if ((i=(int)strchr(resultString,'$')))
            patternPosition=(char*)memcpy((void*)fileNameBuffer, (void*)resultString, sizeof(char)*NAMEGEN_BUFFER_SIZE);
    } while(i && resultString[maximumOutputLength]==0 && --tokenLoops);
    if (!tokenLoops)
        fprintf(stderr, "Failed namegen: %d token depth exceeded. Possibly infinite loop?\n", MAXIMUM_TOKEN_DEPTH);
    free (fileNameBuffer);
}

void init_hash_tables()
{
   	hashMapping=get_5bit_hash_table();
	hashLength=get_hash_bits((unsigned char*)hashMapping);
	hashTableSize=pow(2.0f,hashLength*3-3);
    hashExists=(char*)calloc(hashTableSize,1);
}

char* name_generator(unsigned int maximumOutputLength, char* manifestFile, char* patternFile)
{
	int patternRetries, j;
	char *resultString, *appendPointer, *selectedPattern, *fileNameBuffer;
	struct DICTIONARY* dictionaries;
	struct WORDLIST patterns;
	struct WORDLIST *wordListChoices;
	char *hardcodedWordListIndices;
	if (maximumOutputLength<=0)
	{
        fprintf(stderr, "Failed namegen: Impossible to produce a valid %d char output\n", maximumOutputLength);
        return (char*) calloc(1,1);
	}
    init_hash_tables();
	dictionaries=(struct DICTIONARY *)calloc(256, sizeof(struct DICTIONARY));
	wordListChoices=(struct WORDLIST *)calloc(256, sizeof(struct WORDLIST));
	dictionary_init(maximumOutputLength, manifestFile, dictionaries, wordListChoices, &hardcodedWordListIndices);
/*
	for (appendPointer=hardcodedWordListIndices; *appendPointer; ++appendPointer)
		for (j=0; j<wordListChoices[(int)*appendPointer].entryCount; ++j)
			fprintf(stderr, "[%s]\n", wordListChoices[(int)*appendPointer].entryList[j]);
*/
	fileNameBuffer=(char*)calloc(NAMEGEN_BUFFER_SIZE, sizeof(char));
	resultString=(char*)calloc(NAMEGEN_BUFFER_SIZE, sizeof(char));
	patterns=load_wordlist(get_mod_file_path_2("Galaxy",patternFile, fileNameBuffer));
    if( patterns.entryCount==0 )
    {
        fprintf(stderr, "Failed namegen: No patterns");
        return (char*) calloc(1,1);
    }
	do
	{
        selectedPattern=random_wordlist_item(patterns);
		patternRetries=MAXIMUM_RETRIES;
		do
            grammar_produce(dictionaries, wordListChoices, hardcodedWordListIndices, selectedPattern, resultString, maximumOutputLength);
		while ((j=strlen(resultString)>maximumOutputLength-1) && patternRetries--);
		if ((j=(j|| patternRetries<=0)))
		{
			fprintf(stderr, "Failed namegen [%s] after %d tries, changing pattern...\n", selectedPattern, MAXIMUM_RETRIES);
			remove_entry(&patterns, selectedPattern);
			if (patterns.entryCount==0)
            {
                fprintf(stderr, "Failed namegen: Cannot find a pattern that produces valid %d char output\n", maximumOutputLength-1);
                resultString[0]=j=0;
            }
		}
	} while (j);

    for(appendPointer=resultString; *appendPointer; ++appendPointer)
        if (*appendPointer=='_')
            *appendPointer=' ';
    appendPointer=resultString;
    strcpy(resultString=(char*)calloc(maximumOutputLength, sizeof(char)),appendPointer);
    namegen_cleanup(appendPointer, fileNameBuffer, dictionaries, &patterns, wordListChoices, hardcodedWordListIndices);
	return resultString;
}
