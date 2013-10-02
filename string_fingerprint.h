#ifndef STRING_FINGERPRINT_H
#define STRING_FINGERPRINT_H
char* get_5bit_hash_table ();
int string_fingerprint(char* str, char* hashMapping, int hashBits);
int get_hash_bits(unsigned char* hashTable);
#endif
