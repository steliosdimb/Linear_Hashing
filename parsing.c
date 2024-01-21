#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "global.h"
#include "parsing.h"
void parsing(int argc, char *argv[], int *bucketentries, char **filename, int *flagib)
{
    int i, flagf = 0, flagb = 0;
    for (i = 1; i < argc; i++)
    {

        if (strcmp(argv[i], "-f") == 0)
        {
            flagf = 1;
            if (argv[i + 1] != NULL)
            {
                *filename = argv[i + 1];
            }
            else
            {
                printf("Sorry Wrong Input!Try Again!\n"); // user need to provide a filename after flag -f
                exit(-1);
            }
        }
        else if (strcmp(argv[i], "-b") == 0)
        {
            flagb = 1;
            if (argv[i + 1] != NULL)
            {
                *bucketentries = atoi(argv[i + 1]);
            }
            else
            {
                printf("Sorry Wrong Input!Try Again!\n"); // user needs to provide a bucket entries after flag -b
                exit(-1);
            }
        }
        else if (strcmp(argv[i], "-ib") == 0)
        { // new flag that i add -ib=initial block size
            *flagib = 1;
            if (argv[i + 1] != NULL)
            {
                initial_bucket_size = atoi(argv[i + 1]);
            }
            else
            {
                printf("Sorry Wrong Input!Try Again!\n"); // user needs to provide inital bucket size for the initialization of the hashing table
                exit(-1);
            }
        }
    }
    if (flagb == 0 || flagf == 0 || *flagib == 0)
    {
        printf("Sorry Wrong Input!Try Again!\n"); // user needs to provide both flag -f and flag -b and flag -ib
        exit(-1);
    }
}