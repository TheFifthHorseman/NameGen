#ifndef WORDLIST_H
#define WORDLIST_H
struct WORDLIST
{
	char *dataStart, **entryList;
	int entryCount;
};
inline struct WORDLIST blank_wordlist();
inline void init_wordlist(struct WORDLIST* wordlist);
inline struct WORDLIST load_wordlist(char* fileName);
inline void wordlist_from_string(char* string, struct WORDLIST* wordlist);
inline int wordlist_too_similar(struct WORDLIST firstWordList, struct WORDLIST secondWordList);
inline char* random_wordlist_item(struct WORDLIST wordList);
inline void free_wordlist(struct WORDLIST *wordList);
inline void remove_wordlist_item(struct WORDLIST *wordList, int i);
inline void prune_long_wordlist_entries(struct WORDLIST *wordList, unsigned int truncateTo);
inline void remove_entry(struct WORDLIST *wordList, char* pattern);
inline void load_hardcoded_list(char* source, struct WORDLIST* wordList);
#endif
