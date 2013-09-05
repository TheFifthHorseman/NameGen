#ifndef DICTIONARY_H
#define DICTIONARY_H
struct DICTIONARY
{
	char *dataStart;
	struct WORDLIST *entryList;
	int entryCount;
};

inline struct DICTIONARY blank_dictionary();
inline void init_dictionary(struct DICTIONARY* dictionary);
inline struct WORDLIST random_dictionary_item(struct DICTIONARY dictionary);
inline void dictionary_from_wordlist(struct WORDLIST wordList, struct DICTIONARY* targetDictionary);
inline void dictionary_from_file(char* fileName, struct DICTIONARY* targetDictionary);
inline void free_dictionary(struct DICTIONARY* dictionary);
inline void remove_dictionary_item(struct DICTIONARY* dictionary, int i);
inline void prune_long_dictionary_entries(struct DICTIONARY *dictionary, int truncateTo);
#endif
