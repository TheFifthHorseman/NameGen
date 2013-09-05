#include <stdio.h>
#include "mod.h"
inline char* get_mod_file_path(char* fileName, char* fileNameBuffer)
{
    if (ActiveMod)
        sprintf(fileNameBuffer, "Mods\\%s\\%s", ActiveMod, fileName);
    else
        sprintf(fileNameBuffer, "%s", fileName);
	return fileNameBuffer;
}
inline char* get_mod_file_path_2(char* subDir, char* fileName, char* fileNameBuffer)
{
    if (ActiveMod)
        sprintf(fileNameBuffer, "Mods\\%s\\%s\\%s", ActiveMod, subDir, fileName);
    else
        sprintf(fileNameBuffer, "%s\\%s", subDir, fileName);
	return fileNameBuffer;
}
