#include <string.h>
#include <stdlib.h>
#include "string_fingerprint.h"
/*
    Based on Baudot code.
*/
char* get_5bit_hash_table ()
{
    char* hashTable;
    char* mapptr;
    char* mappings[32]={   " _\t\r\n", "Aa", "Bb", "Cc", "Dd", "Ee", "Ff", "Gg", "Hh", "Ii", "Jj", "Kk", "Ll", "Mm", "Nn", "Oo",
                                 "Pp", "Qq", "Rr", "Ss", "Tt", "Uu", "Ww", "Vv", "Xx", "Yy", "Zz", "0123456789",
                            "'\"`\\/", "-~", ":;,.!?",   "" };
    int i;
    hashTable=(char*)calloc(1, 256);
    memset(hashTable,0x00,0x20);
    memset(hashTable+0x20,0x1F,0x100-0x20);
    hashTable[0x7F]=00;
    for (i=0; i<32; ++i)
        for (mapptr=mappings[i]; *mapptr; ++mapptr)
            hashTable[(int)*mapptr]=i;
    return hashTable;
}

int get_hash_bits(unsigned char* hashTable)
{
    int m=0;
    int i;
    for (i=0; i<256; ++i)
    {
        if (m<hashTable[i])
            m|=hashTable[i];
        else
        if (m&0x80) return 8;
    }
    if (m&0x40) return 7;
    if (m&0x20) return 6;
    if (m&0x10) return 5;
    if (m&0x8)  return 4;
    if (m&0x4)  return 3;
    if (m&0x2)  return 2;
                return 1;
}

/*
    The fingerprint is a representation of the string's first three characters
    converted into 5 bit encoding and packed into an int
*/
int string_fingerprint(char* str, char* hashMapping, int hashBits)
{
    int i=0;
    int t;
    if (!str)
        return 0;
    if ((t=*str))
    {
        t=hashMapping[t];
        if (!t)
            return 0;
        i=t;
        ++str;
        if ((t=*str))
        {
            t=hashMapping[t];
            if (!t)
                return i;
            i<<=hashBits;
            i+=t;
            ++str;
            if ((t=*str))
            {
                t=hashMapping[t];
                if (!t)
                    return i;
                i<<=hashBits;
                i+=t;
            }
        }
    }
    return i;
}

