#ifndef STRINGOPS_H
#define STRINGOPS_H
#define WhiteSpace " \n\r\t"
/*int too_similar(char* a, char* b);*/
int count_character_occurences(char *string, char *charsToCount);
int split_string_DESTRUCTIVE(char* inputString, int* pieceCount, char*** ouputPieces, char* splitBy);
char* trim(char *t);
void clean_string_array(char** strings, int* stringCount);
char* random_string(char** stringArray, int stringCount);
char* file_get_text(char* fileName);
char* files_get_text(char* fileName, char* workingDirectory);
char** file_get_lines(char* fileName, int* lineCount);
char** files_get_lines(char* fileName, int* lineCount);
char* get_mod_file_path(char* fileName, char* fileNameBuffer);
char* get_mod_file_path_2(char* subDir, char* fileName, char* fileNameBuffer);
#endif
