#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "mod.h"
#include "namegen.h"
char* ActiveMod;

int main(int argc, char** argv)
{
    unsigned int i, columns, j, lines;
    int padding;
    char*result;
    FILE* genlog=NULL;
    srand(time(NULL));
    genlog=fopen("names.log", "a");
    freopen( "stderr.log", "w", stderr );
    /*ActiveMod="AlienAssault2";*/
    lines=59;
    /* 23 chars printable plus null-terminator; */
    /* Sweet spot between short and boringly long names. */
    /* Note: The program WILL crash on v. low lengths - in test data, anything below 15 chars. Probably because that's shorter than most patterns are. */
/*    NameLength=15; */
/*    NameLength=19; */
    NameLength=24;

    i=0;
    if (argc>1) sscanf(argv[1], "%ud", &i);
    i=i/3+(i%3>0);
    if (i>0) lines=i;
    padding=80%(NameLength+1);
    columns=80/(NameLength+1);
    if (columns>1)
        padding/=columns-1;
    for (i=0;i<lines;++i)
    {
        for (j=0; j<columns; j++)
        {
            result=ship_alien_name();
            if (j<columns-1)
            {
                printf("%s%*s", result, NameLength-strlen(result)+padding,"");
                if (genlog) fprintf(genlog, "%s%*s", result, NameLength-strlen(result)+padding,"");
            }
            else
            {
                printf("%s\n", result);
                if (genlog) fprintf(genlog, "%s\n", result);
            }
            free(result);
        }
    }
	return 0;
}

