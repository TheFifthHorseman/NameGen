#ifndef STRING_FINGERPRINT_H
#define STRING_FINGERPRINT_H
inline char* get_5bit_hash_table ();
inline int string_fingerprint(char* str, char* hashMapping, int hashBits);
inline int get_hash_bits(unsigned char* hashTable);
#endif
