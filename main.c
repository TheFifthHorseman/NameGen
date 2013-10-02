/*
    TODO:
    + Fix the mess with hardcoded mod paths!
    + Namegen should be called with the path as an argument!
    + BST??
    + Namegen validator
    + Increase buffer length!!! 256 = high risk of overflow. 16k?

    Dictionary TODOs:
    + Deverbal nouns
    + Verbs
    + Verbal Adjectives
    + The $X that <Verb>
    + Add some words specially for the "X: The Y" patterns!
    + Added Alliterative Appeal?
*/
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
    int NameLength=NAMEGEN_AA2_MAX_LENGTH;
    FILE* genlog=NULL;
    /*
    srand((unsigned int)21843769);
    srand(214783659);
    */
    srand(time(NULL));
    genlog=fopen("names.log", "a");
    freopen( "stderr.log", "w", stderr );
    /*ActiveMod="AlienAssault2";*/
    lines=59;

    i=0;
    if (argc>1) sscanf(argv[1], "%ud", &i);
    i=i/3+(i%3>0);
    if (i>0) lines=i;
    padding=80%(NameLength+1);
    columns=80/(NameLength+1);
    if (columns>1)
        padding/=columns-1;
    /*mem_log_out=stderr;*/

/*
    //start=time(NULL);
    for (i=0; i<10000; ++i)
    {
        result=ship_alien_name();
//		printf("%d ", i);
//        printf("%d %s\n", i,result);
        free(result);
//		mem_log(manager_struct, stderr);
//		fflush(stderr);
    }
//*/
/*
    result=ship_alien_name();
    //printf("%s%*s", result, NameLength-strlen(result)+padding,"");
 //   if (genlog) fprintf(genlog, "%s%*s", result, NameLength-strlen(result)+padding,"");
    free(result);
 /*/
/**/
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
/**/
/*    mem_log(manager_struct, stderr);
	mem_log(manager_struct, stdout); */
/*	system("PAUSE");*/
	return 0;
}

