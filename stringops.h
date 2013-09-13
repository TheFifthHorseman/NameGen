#ifndef STRINGOPS_H
#define STRINGOPS_H
#define WhiteSpace " \n\r\t"
inline int too_similar(char* a, char* b);
inline int count_character_occurences(char *string, char *charsToCount);
inline int split_string_DESTRUCTIVE(char* inputString, int* pieceCount, char*** ouputPieces, char* splitBy);
inline char* trim(char *t);
inline void clean_string_array(char** strings, int* stringCount);
inline char* random_string(char** stringArray, int stringCount);
inline char* file_get_text(char* fileName);
inline char* files_get_text(char* fileName, char* workingDirectory);
inline char** file_get_lines(char* fileName, int* lineCount);
inline char** files_get_lines(char* fileName, int* lineCount);
inline char* get_mod_file_path(char* fileName, char* fileNameBuffer);
inline char* get_mod_file_path_2(char* subDir, char* fileName, char* fileNameBuffer);
#endif
