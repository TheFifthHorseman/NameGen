#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include "stringops.h"
#include "wordlist.h"
#include "dictionary.h"
/* Buffer size for file names and output string. Should be larger than maximum expected length of either of these. */
#define NAMEGEN_BUFFER_SIZE 256
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
char* name_generator(unsigned int maximumOutputLength, char* manifestFile, char* patternFile)
{
	int retries, patternRetries, tokenLoops, redo, i, j, k, hardcodedIndicesLength=0;
	char *resultString, *appendPointer, *selectedPattern, *patternPosition, *fileNameBuffer;
	struct DICTIONARY dictionaries[256];
	struct WORDLIST manifest, patterns, wordListChoices[256];
	char *hardcodedWordListIndices;
	/* 23 chars printable plus null-terminator; tested to be the "sweet spot" for us */
	if (!maximumOutputLength) maximumOutputLength=24;
	for (i=0; i<256; ++i)
	{
		dictionaries[i]=blank_dictionary();
		wordListChoices[i]=blank_wordlist();
	}
	hardcodedWordListIndices=(char*)calloc(256,sizeof(char));
	manifest=load_wordlist(get_mod_file_path_2("Galaxy",manifestFile, fileNameBuffer=(char*)malloc(sizeof(char)*NAMEGEN_BUFFER_SIZE)));
	for (i=0; i<manifest.entryCount; ++i)
		if (strlen(manifest.entryList[i])>3)
		{
			j=(int)*(patternPosition=trim(manifest.entryList[i]));
			if (dictionaries[j].entryCount==0 && wordListChoices[j].entryCount==0)
				switch(*(patternPosition+=strspn (patternPosition+1,WhiteSpace)+1))
				{
					case '@':
						dictionary_from_file(get_mod_file_path_2("Galaxy",patternPosition+1, fileNameBuffer), &dictionaries[j]);
						break;
					default:
						wordlist_from_string(patternPosition, &wordListChoices[j]);
						hardcodedWordListIndices[hardcodedIndicesLength++]=j;
						wordListChoices[j].dataStart=NULL;
						for (k=0; k<wordListChoices[j].entryCount; ++k)
							if (!memcmp ( wordListChoices[j].entryList[k], "_", 2))
								wordListChoices[j].entryList[k][0]=0;
						break;
				}
		}
    for (i=0; i<256; ++i)
        prune_long_dictionary_entries( &(dictionaries[i]), maximumOutputLength );
	patterns=load_wordlist(get_mod_file_path_2("Galaxy",patternFile, fileNameBuffer));
    selectedPattern=random_wordlist_item(patterns);
	resultString=(char*)malloc(sizeof(char)*NAMEGEN_BUFFER_SIZE);
	do
	{
		patternRetries=MAXIMUM_RETRIES;
		do
		{
			patternPosition=(char*)memcpy((void*)fileNameBuffer, (void*)selectedPattern, sizeof(char)*NAMEGEN_BUFFER_SIZE);
			tokenLoops=MAXIMUM_TOKEN_DEPTH;
			do
			{
				for (i=0;i<256; ++i)
					if(strchr(hardcodedWordListIndices,i)==NULL)
						wordListChoices[i].entryCount=0;
				appendPointer=(char*)memset((void*)resultString,0,sizeof(char)*NAMEGEN_BUFFER_SIZE);
				while(*patternPosition)
					switch (*patternPosition)
					{
						case '$':
							i=(int)*(++patternPosition);
							++patternPosition;
							retries=MAXIMUM_RETRIES;
							if (dictionaries[i].entryCount>0 && strchr(hardcodedWordListIndices,i)==NULL)
								do
								{
									wordListChoices[i]=random_dictionary_item(dictionaries[i]);
									for (j=redo=0; j<256 && !redo; ++j)
										if(j!=i && wordListChoices[j].entryCount>0 && strchr(hardcodedWordListIndices,j)==NULL)
											if (wordlist_too_similar(wordListChoices[i], wordListChoices[j]))
												redo=1;
								} while (redo && --retries);
							if (wordListChoices[i].entryCount>0)
								appendPointer+=sprintf(appendPointer, "%s", random_wordlist_item(wordListChoices[i]));
							else
								fprintf(stderr, "No wordlist or dictionary for token $%c\n", i);
							break;
						default:
							*(appendPointer++)=*(patternPosition++);
							break;
					}
				if ((i=(int)strchr(resultString,'$')))
					patternPosition=(char*)memcpy((void*)fileNameBuffer, (void*)resultString, sizeof(char)*NAMEGEN_BUFFER_SIZE);
			} while(i && resultString[maximumOutputLength]==0 && --tokenLoops);
			if (!tokenLoops)
			{
				fprintf(stderr, "Failed namegen: %d token depth exceeded. Possibly infinite loop?\n", MAXIMUM_TOKEN_DEPTH);
				fflush(stderr);
			}
		} while ((j=strlen(resultString)>maximumOutputLength-1) && patternRetries--);
		if ((j=(j|| patternRetries<=0)))
		{
			fprintf(stderr, "Failed namegen [%s] after %d tries, changing pattern...\n", selectedPattern, MAXIMUM_RETRIES);
			fflush(stderr);
			remove_entry(&patterns, selectedPattern);
			if (patterns.entryCount==0)
            {
                fprintf(stderr, "Failed namegen: Cannot find a pattern that produces valid %d char output\n", maximumOutputLength-1);
                fflush(stderr);
                j=0;
                resultString="";
            }
            else
                selectedPattern=random_wordlist_item(patterns);
		}
	} while (j);
    for(appendPointer=resultString; *appendPointer; ++appendPointer)
        if (*appendPointer=='_')
            *appendPointer=' ';
    appendPointer=resultString;
    strcpy(resultString=(char*)malloc(maximumOutputLength),appendPointer);
	free (appendPointer);
	free (fileNameBuffer);
	for (i=0; i<256; ++i)
		free_dictionary(&dictionaries[i]);
	free_wordlist(&patterns);
	free_wordlist(&manifest);
	for (i=0; i<hardcodedIndicesLength; ++i)
		if (wordListChoices[j=hardcodedWordListIndices[i]].entryCount)
			free_wordlist(&wordListChoices[j]);
	free(hardcodedWordListIndices);
	return resultString;
}
