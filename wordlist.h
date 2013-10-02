#ifndef WORDLIST_H
#define WORDLIST_H
struct WORDLIST
{
	char *dataStart, **entryList;
	int entryCount;
};
struct WORDLIST blank_wordlist();
void init_wordlist(struct WORDLIST* wordlist);
struct WORDLIST load_wordlist(char* fileName);
struct WORDLIST load_wordlist_multi(char* fileNames);
void wordlist_from_string(char* string, struct WORDLIST* wordlist);
/*int wordlist_too_similar(struct WORDLIST firstWordList, struct WORDLIST secondWordList);*/
char* random_wordlist_item(struct WORDLIST wordList);
void free_wordlist(struct WORDLIST *wordList);
void remove_wordlist_item(struct WORDLIST *wordList, int i);
void prune_long_wordlist_entries(struct WORDLIST *wordList, unsigned int truncateTo);
void remove_entry(struct WORDLIST *wordList, char* pattern);
void load_hardcoded_list(char* source, struct WORDLIST* wordList);
#endif
