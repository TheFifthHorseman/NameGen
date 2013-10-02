#ifndef DICTIONARY_H
#define DICTIONARY_H
struct DICTIONARY
{
	char *dataStart;
	struct WORDLIST *entryList;
	int entryCount;
};

struct DICTIONARY blank_dictionary();
void init_dictionary(struct DICTIONARY* dictionary);
struct WORDLIST random_dictionary_item(struct DICTIONARY dictionary);
void dictionary_from_wordlist(struct WORDLIST wordList, struct DICTIONARY* targetDictionary);
void dictionary_from_file(char* fileName, struct DICTIONARY* targetDictionary);
void dictionary_from_files(char* fileNames, struct DICTIONARY* targetDictionary);
void free_dictionary(struct DICTIONARY* dictionary);
void remove_dictionary_item(struct DICTIONARY* dictionary, int i);
void prune_long_dictionary_entries(struct DICTIONARY *dictionary, int truncateTo);
#endif
