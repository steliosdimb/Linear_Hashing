#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "global.h"
#include "parsing.h"
#include "hash_table.h"
#include "inverted_list.h"
int current_buckets_number = 0;
int initial_bucket_size;
int level = 0;
int p = 0;
int c1 = 0;
int h0;
int h1;
struct NodeOfInvertedLinkedList *mvote(struct NodeOfLinkedList **, struct NodeOfInvertedLinkedList *, int);

FILE *open_file(char *);

struct voter *set_to_voted(struct NodeOfLinkedList **, int, int, int *);
void clear_input_buffer(void);
int total_votes(struct NodeOfInvertedLinkedList *);
void print_by_TK(struct NodeOfInvertedLinkedList *, int);
void print_all_TK(struct NodeOfInvertedLinkedList *);
void swap(int *, int *);
int main(int argc, char *argv[])
{
    struct NodeOfLinkedList **LinearHashingArray;
    struct NodeOfInvertedLinkedList *InvertedList;
    int flagib = 0, bucketentries;
    char *filename;
    FILE *fptr;
    parsing(argc, argv, &bucketentries, &filename, &flagib);
    fptr = open_file(filename);
    if (fptr == NULL)
    {
        exit(-1);
    }
    LinearHashingArray = initialization(flagib, bucketentries);
    LinearHashingArray = fill_hashing_table(fptr, LinearHashingArray, bucketentries);
    InvertedList = init_inverted_list();
    while (InvertedList != NULL)
    {
        InvertedList = mvote(LinearHashingArray, InvertedList, bucketentries);
    }
}
struct NodeOfInvertedLinkedList *mvote(struct NodeOfLinkedList **LinearHashingArray, struct NodeOfInvertedLinkedList *InvertedList, int bucketentries)
{
    char option[2], surname[100], name[100];
    char filename[100];
    int PIN, TK;
    struct voter *voter;
    FILE *fptr;
    scanf("%s", option);
    int flag = 0;
    if (strcmp(option, "l") == 0)
    { // these option need a pin after the flag

        if (scanf("%d", &PIN) == 1)
        {
            voter = search_by_PIN(LinearHashingArray, PIN, bucketentries);
            if (voter != NULL)
            {
                printf("- %d %s %s %d %c\n", voter->PIN, voter->surname, voter->name, voter->TK, voter->voted);
            }
            else
            {
                printf("- Participant %d not in cohort\n", PIN);
            }
        }
        else
        {
            printf("- Malformed Pin\n");
            clear_input_buffer();
        }
    }
    else if (strcmp(option, "m") == 0)
    {
        if (scanf("%d", &PIN) == 1)
        {
            struct voter *v = (struct voter *)malloc(sizeof(struct voter));
            if (v == NULL)
            {
                printf("Error malloc\n");
                exit(-1);
            }
            v = set_to_voted(LinearHashingArray, PIN, bucketentries, &flag);
            if (flag == 1)
            {
                printf("%d Already Voted\n", PIN);
            }
            else
            {
                if (v == NULL)
                {
                    printf("%d does not exists\n", PIN);
                }
                else
                {
                    InvertedList = fill_inverted_list(InvertedList, v, v->TK);

                    printf("%d Marked voted\n", PIN);
                }
            }
            free(v);
        }
        else
        {
            printf("- Malformed Input\n");
            clear_input_buffer();
        }
    }
    else if (strcmp(option, "i") == 0)
    {
        if (scanf("%d", &PIN) == 1 && scanf("%s", surname) == 1 && scanf("%s", name) == 1 && scanf("%d", &TK) == 1)
        {
            struct voter *voter = (struct voter *)malloc(sizeof(struct voter));
            if (voter == NULL)
            {
                printf("Error malloc\n");
                exit(-1);
            }
            strcpy(voter->name, name);
            strcpy(voter->surname, surname);
            voter->PIN = PIN;
            voter->TK = TK;
            voter->voted = 'N';
            if (hash_table_insert(voter, PIN, LinearHashingArray, bucketentries) == NULL)
            {
                printf("%d already exist\n", PIN);
            }
            else
            {
            }
            printf("Inserted %d %s %s %d N\n", PIN, surname, name, TK);
            free(voter);
        }
        else
        {
            printf("- Malformed Input\n");
            clear_input_buffer();
        }
    }
    else if (strcmp(option, "bv") == 0)
    {
        scanf("%s", filename);
        fptr = open_file(filename);
        if (fptr == NULL)
        {
            return InvertedList;
        }
        else
        {
            while (fscanf(fptr, "%d", &PIN) == 1)
            {
                int flag = 0;
                struct voter *v = (struct voter *)malloc(sizeof(struct voter));
                if (v == NULL)
                {
                    printf("Error malloc\n");
                    exit(-1);
                }

                v = set_to_voted(LinearHashingArray, PIN, bucketentries, &flag);
                if (flag == 1)
                {
                    printf("%d Already Voted\n", PIN);
                }
                else
                {
                    if (v == NULL)
                    {
                        printf("%d does not exists\n", PIN);
                        continue;
                    }
                    else
                    {
                        InvertedList = fill_inverted_list(InvertedList, v, v->TK);
                        printf("%d Marked voted\n", PIN);
                    }
                }
                free(v);
            }
            if (feof(fptr))
            { // didnt read any malformed input
            }
            else
            {
                printf("- Malformed Input\n");
                clear_input_buffer();
            }
        }
    }
    else if (strcmp(option, "v") == 0)
    {
        printf("Voted So Far %d\n", total_votes(InvertedList));
    }
    else if (strcmp(option, "perc") == 0)
    {
        double percentage = ((double)total_votes(InvertedList) / (double)number_of_keys(bucketentries, LinearHashingArray)) * 100;
        printf("%f%% Voted So Far\n", percentage);
    }
    else if (strcmp(option, "o") == 0)
    {
        print_all_TK(InvertedList);
    }
    else if (strcmp(option, "z") == 0)
    {
        scanf("%d", &TK);
        print_by_TK(InvertedList, TK);
    }
    else if (strcmp(option, "exit") == 0)
    {
        free_hashing_array(LinearHashingArray);
        free_inverted_list(InvertedList);
        return NULL;
    }
    else
    {
        printf("Sorry option doesnt exists\n");
    }
    return InvertedList;
}

FILE *open_file(char *filename)
{
    FILE *fptr;
    if ((fptr = fopen(filename, "r")) == NULL)
    {
        printf("%s could not be opened\n", filename);
        return NULL;
    }
    return fptr;
}

struct voter *set_to_voted(struct NodeOfLinkedList **LinearHashingArray, int PIN, int bucketentries, int *flag)
{
    for (int i = 0; i < current_buckets_number; i++)
    {

        struct NodeOfLinkedList *curr_b = LinearHashingArray[i];
        while (curr_b != NULL)
        {
            for (int j = 0; j < bucketentries; j++)
            {
                if (curr_b->data[j].PIN == -1)
                {
                    continue;
                }
                if (curr_b->data[j].PIN == PIN)
                {
                    if (curr_b->data[j].v->voted == 'Y')
                    {
                        *flag = 1;
                        return NULL;
                    }
                    else
                    {
                        curr_b->data[j].v->voted = 'Y';
                        return curr_b->data[j].v;
                    }
                }
            }
            curr_b = curr_b->next;
        }
    }
    return NULL;
}

void clear_input_buffer() // clear the buffer because when entering a malformed pin in the next call of mvote the buffer still contains the malformed pin and reads it as int
{
    int c;
    while ((c = getchar()) != '\n' && c != EOF)
        ;
}
int total_votes(struct NodeOfInvertedLinkedList *InvertedList)
{
    int counter = 0;

    while (InvertedList != NULL)
    {
        if (InvertedList->TK != -1)
        {
            struct ListOfVoters *currentVoter = InvertedList->LofV;
            while (currentVoter != NULL)
            {
                if (currentVoter->v != NULL)
                {
                    counter++;
                }
                currentVoter = currentVoter->next;
            }
        }
        else
        {
            break;
        }
        InvertedList = InvertedList->next;
    }

    return counter;
}
void print_by_TK(struct NodeOfInvertedLinkedList *InvertedList, int TK)
{
    struct NodeOfInvertedLinkedList *curr_node = InvertedList;
    int counter = 0;
    while (curr_node != NULL)
    {
        if (curr_node->TK == -1)
        {
            break;
        }
        else if (curr_node->TK == TK)
        {

            struct ListOfVoters *curr_list = curr_node->LofV;
            while (curr_list != NULL)
            {
                curr_list = curr_list->next;
                counter++;
            }
            printf("%d voted in %d\n", counter, curr_node->TK);
            curr_list = curr_node->LofV;
            while (curr_list != NULL)
            {
                if (curr_list->v != NULL)
                {
                    printf("%d\n", curr_list->v->PIN);
                }
                curr_list = curr_list->next;
            }
            break;
        }
        curr_node = curr_node->next;
    }
}
void print_all_TK(struct NodeOfInvertedLinkedList *InvertedList)
{
    struct NodeOfInvertedLinkedList *curr_node = InvertedList;
    int counter1 = 0;
    while (curr_node != NULL)
    {
        if (curr_node->TK == -1)
        {
            break;
        }
        else
        {
            counter1++;
        }
        curr_node = curr_node->next;
    }
    int **arr = (int **)malloc(counter1 * sizeof(int *));
    if (arr == NULL)
    {
        printf("Error malloc\n");
        exit(-1);
    }
    for (int i = 0; i < counter1; i++)
    {
        arr[i] = (int *)malloc(2 * sizeof(int));

        if (arr[i] == NULL)
        {
            printf("Error malloc\n");
            exit(-1);
        }
    }
    // allocate an array with first column the TK and 2nd one the number of voters from this TK
    int i = 0;
    int counter = 0;
    curr_node = InvertedList;
    while (curr_node != NULL)
    {
        counter = 0;
        if (curr_node->TK == -1)
        {
            break;
        }
        else
        {

            struct ListOfVoters *curr_list = curr_node->LofV;
            while (curr_list != NULL)
            {
                curr_list = curr_list->next;
                counter++;
            }
            arr[i][0] = curr_node->TK;
            arr[i][1] = counter;
        }

        curr_node = curr_node->next;
        i++;
    }

    for (int i = 0; i < counter1 - 1; i++)
    {
        for (int j = 0; j < counter1 - i - 1; j++)
        {
            if (arr[j][1] < arr[j + 1][1])
            {
                // Swap the rows if the 2nd column is in the wrong order
                swap(&arr[j][0], &arr[j + 1][0]);
                swap(&arr[j][1], &arr[j + 1][1]);
            }
        }
    }
    for (int i = 0; i < counter1; i++)
    {
        printf("%d, %d\n", arr[i][0], arr[i][1]);
    }
    for (int i = 0; i < counter1; i++)
    {
        free(arr[i]);
    }

    free(arr);
}
void swap(int *a, int *b)
{
    int temp = *a;
    *a = *b;
    *b = temp;
}
