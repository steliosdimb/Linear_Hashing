#include "hash_table.h"
#include <stdio.h>
#include "global.h"
#include <math.h>
#include <string.h>
#include <stdlib.h>
struct voter *search_by_PIN(struct NodeOfLinkedList **LinearHashingArray, int PIN, int bucketentries)
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
                    return curr_b->data[j].v;
                }
            }
            curr_b = curr_b->next;
        }
    }
    return NULL;
}

struct NodeOfLinkedList **hash_table_insert(struct voter *v, int key, struct NodeOfLinkedList **LinearHashingArray, int bucketentries)
{

    if (search_by_PIN(LinearHashingArray, key, bucketentries) != NULL)
    {
        return NULL; // voter with pin number already in the array
    }
    int bucket_position = key % h0;
    int flag = 0;
    int n_of_keys;
    double l;
    int key_capacity_no_overflow_buckets;

    if (bucket_position < p)
    {
        bucket_position = key % h1; // if bucket position found with hi function is less than the number of bucket the pointer points we need to use hi+1 function
    }
    if (LinearHashingArray[bucket_position]->next == NULL)
    {
        // there is no overflow bucket so we search for free space in this bucket

        for (int i = 0; i < LinearHashingArray[bucket_position]->size; i++)
        {
            if (LinearHashingArray[bucket_position]->data[i].PIN == -1)
            { // found an empty space in a bucket
                LinearHashingArray[bucket_position]->data[i].PIN = key;
                LinearHashingArray[bucket_position]->data[i].v = (struct voter *)malloc(sizeof(struct voter));
                if (LinearHashingArray[bucket_position]->data[i].v == NULL)
                {
                    printf("Error malloc\n");
                    exit(-1);
                }
                strcpy(LinearHashingArray[bucket_position]->data[i].v->name, v->name);
                strcpy(LinearHashingArray[bucket_position]->data[i].v->surname, v->surname);
                LinearHashingArray[bucket_position]->data[i].v->PIN = v->PIN;
                LinearHashingArray[bucket_position]->data[i].v->TK = v->TK;
                LinearHashingArray[bucket_position]->data[i].v->voted = 'N';
                flag = 1;
                break;
            }
        }
        if (flag == 0) // bucket is full
        {

            struct NodeOfLinkedList *overflow_bucket = create_overflow_bucket(LinearHashingArray[bucket_position]->size);
            // create new overflow bucket
            overflow_bucket->data[0].PIN = key; // add the key in the begining of the bucket
            strcpy(overflow_bucket->data[0].v->name, v->name);
            strcpy(overflow_bucket->data[0].v->surname, v->surname);
            overflow_bucket->data[0].v->PIN = v->PIN;
            overflow_bucket->data[0].v->TK = v->TK;
            overflow_bucket->data[0].v->voted = 'N';
            LinearHashingArray[bucket_position]->next = overflow_bucket;
            // connect the new linked list node with the previous one in the bucket posiiton of the linearhasingarray
        }
    }
    else
    {
        struct NodeOfLinkedList *next_ptr = LinearHashingArray[bucket_position]->next;
        if (next_ptr != NULL && LinearHashingArray[bucket_position]->data[bucketentries - 1].PIN != -1) // i use the second condiition to fix an error
        {
            while (next_ptr->next != NULL)
            {
                next_ptr = next_ptr->next; // find the last overflow bucket
            }
            for (int i = 0; i < next_ptr->size; i++)
            {
                if (next_ptr->data[i].PIN == -1)
                { // found an empty space in a bucket
                    next_ptr->data[i].PIN = key;
                    next_ptr->data[i].v = (struct voter *)malloc(sizeof(struct voter));
                    if (next_ptr->data[i].v == NULL)
                    {
                        printf("Error malloc\n");
                        exit(-1);
                    }
                    strcpy(next_ptr->data[i].v->name, v->name);
                    strcpy(next_ptr->data[i].v->surname, v->surname);
                    next_ptr->data[i].v->PIN = v->PIN;
                    next_ptr->data[i].v->TK = v->TK;
                    next_ptr->data[i].v->voted = 'N';
                    flag = 1;
                    break;
                }
            }
            if (flag == 0)
            { // bucket full

                struct NodeOfLinkedList *overflow_bucket = create_overflow_bucket(LinearHashingArray[bucket_position]->size);
                // create new overflow bucket
                overflow_bucket->data[0].PIN = key; // add the key in the begining of the bucket
                overflow_bucket->data[0].v = (struct voter *)malloc(sizeof(struct voter));
                if (overflow_bucket->data[0].v == NULL)
                {
                    printf("Error malloc\n");
                    exit(-1);
                }
                strcpy(overflow_bucket->data[0].v->name, v->name);
                strcpy(overflow_bucket->data[0].v->surname, v->surname);
                overflow_bucket->data[0].v->PIN = v->PIN;
                overflow_bucket->data[0].v->TK = v->TK;
                overflow_bucket->data[0].v->voted = 'N';
                next_ptr->next = overflow_bucket;
                // connect the new linked list node with the previous one in the bucket posiiton of the linearhasingarray
            }
        }
        else
        {
            // the error is that sometimes a bucket is not full but i create an overflow one and put the voter there
            for (int i = 0; i < LinearHashingArray[bucket_position]->size; i++)
            {
                if (LinearHashingArray[bucket_position]->data[i].PIN == -1)
                { // found an empty space in a bucket
                    LinearHashingArray[bucket_position]->data[i].PIN = key;
                    LinearHashingArray[bucket_position]->data[i].v = (struct voter *)malloc(sizeof(struct voter));
                    if (LinearHashingArray[bucket_position]->data[i].v == NULL)
                    {
                        printf("Error malloc\n");
                        exit(-1);
                    }
                    strcpy(LinearHashingArray[bucket_position]->data[i].v->name, v->name);
                    strcpy(LinearHashingArray[bucket_position]->data[i].v->surname, v->surname);
                    LinearHashingArray[bucket_position]->data[i].v->PIN = v->PIN;
                    LinearHashingArray[bucket_position]->data[i].v->TK = v->TK;
                    LinearHashingArray[bucket_position]->data[i].v->voted = 'N';
                    break;
                }
            }
        }
    }

    n_of_keys = number_of_keys(bucketentries, LinearHashingArray);
    key_capacity_no_overflow_buckets = key_capacity_non_overflow(bucketentries);
    l = (double)n_of_keys / (double)key_capacity_no_overflow_buckets;

    if (l > 0.75)
    {

        // split the buckets in the line where the number p is
        LinearHashingArray = (struct NodeOfLinkedList **)realloc(LinearHashingArray, (current_buckets_number + 1) * sizeof(struct NodeOfLinkedList *));
        // reallocated the size of the array adding one more bucket to it
        LinearHashingArray[current_buckets_number] = (struct NodeOfLinkedList *)malloc(sizeof(struct NodeOfLinkedList));
        if (LinearHashingArray[current_buckets_number] == NULL)
        {
            printf("Error malloc\n");
            exit(-1);
        }
        LinearHashingArray[current_buckets_number]->data = (struct bucket_element *)malloc(sizeof(struct bucket_element) * bucketentries);
        if (LinearHashingArray[current_buckets_number]->data == NULL)
        {
            printf("Error malloc\n");
            exit(-1);
        }
        LinearHashingArray[current_buckets_number]->size = bucketentries;
        for (int j = 0; j < bucketentries; j++)
        {
            LinearHashingArray[current_buckets_number]->data[j].PIN = -1;
        }
        LinearHashingArray[current_buckets_number]->next = NULL;
        // allocated the list that will be in this array element
        LinearHashingArray = rehash(LinearHashingArray, bucketentries);

        current_buckets_number++; // add one more bucket
        p = p + 1;

        // by spliting you redistribute each key with the hash function h1
        // check after spliting if buckets reached double the initial level
        // if not just increment p
        // if so p=0 and increment the level,find new hash functions
    }
    else
    {
        // do nothing just return
    }
    return LinearHashingArray;
}
int h_right_factor(int i, int m)
{
    return (int)pow((double)2, (double)i) * m;
}
struct NodeOfLinkedList *create_overflow_bucket(int bucketentries)
{
    struct NodeOfLinkedList *overflow_bucket = (struct NodeOfLinkedList *)malloc(sizeof(struct NodeOfLinkedList));
    if (overflow_bucket == NULL)
    {
        printf("Error malloc\n");
        exit(-1);
    }
    overflow_bucket->data = (struct bucket_element *)malloc(sizeof(struct bucket_element) * bucketentries);
    if (overflow_bucket->data == NULL)
    {
        printf("Error malloc\n");
        exit(-1);
    }
    // allocate space for new bucket
    overflow_bucket->size = bucketentries;
    overflow_bucket->next = NULL;
    for (int i = 0; i < bucketentries; i++)
    {
        overflow_bucket->data[i].PIN = -1;
        overflow_bucket->data[i].v = (struct voter *)malloc(sizeof(struct voter)); // Allocate memory for the voter struct
        if (overflow_bucket->data[i].v == NULL)
        {
            printf("Error malloc\n");
            exit(-1);
        }
        strcpy(overflow_bucket->data[i].v->name, "");
        strcpy(overflow_bucket->data[i].v->surname, "");
        overflow_bucket->data[i].v->PIN = -1;
        overflow_bucket->data[i].v->TK = -1;
        overflow_bucket->data[i].v->voted = 'N';
    }
    return overflow_bucket;
}
void print_hash_table(int bucketentries, struct NodeOfLinkedList **LinearHashingArray)
{
    for (int i = 0; i < current_buckets_number; i++)
    {
        for (int j = 0; j < bucketentries; j++)
        {
            if (LinearHashingArray[i]->data[j].PIN != -1)
            {
                printf("%d %d %d %d\n", i, j, LinearHashingArray[i]->data[j].PIN, LinearHashingArray[i]->data[j].v->PIN);
            }
        }
        struct NodeOfLinkedList *next_ptr = LinearHashingArray[i]->next;
        if (next_ptr != NULL)
        {
            while (next_ptr != NULL)
            {
                for (int j = 0; j < bucketentries; j++)
                {
                    if (next_ptr->data[j].PIN != -1)
                    {
                        printf("overflow %d %d %d %d\n", i, j, next_ptr->data[j].PIN, next_ptr->data[j].v->PIN);
                    }
                }
                next_ptr = next_ptr->next; // find the last overflow bucket
            }
        }
    }
}
int number_of_keys(int bucketentries, struct NodeOfLinkedList **LinearHashingArray)
{
    int key_counter = 0;
    for (int i = 0; i < current_buckets_number; i++)
    {
        for (int j = 0; j < bucketentries; j++)
        {
            if (LinearHashingArray[i]->data[j].PIN != -1)
            {
                key_counter++;
            }
        }
        struct NodeOfLinkedList *next_ptr = LinearHashingArray[i]->next;
        if (next_ptr != NULL)
        {
            while (next_ptr != NULL)
            {
                for (int j = 0; j < bucketentries; j++)
                {
                    if (next_ptr->data[j].PIN != -1)
                    {
                        key_counter++;
                    }
                }
                next_ptr = next_ptr->next; // find the last overflow bucket
            }
        }
    }
    return key_counter;
}
int key_capacity_non_overflow(int bucketentries)
{
    return bucketentries * current_buckets_number;
}
struct NodeOfLinkedList **rehash(struct NodeOfLinkedList **LinearHashingArray, int bucketentries)
{
    struct NodeOfLinkedList *currentBucket = LinearHashingArray[p]; // this will be the current bucket(can be an overflow one)
    struct NodeOfLinkedList *prevBucket = NULL;                     // this will be the previous bucket
    struct NodeOfLinkedList *firstBucket = LinearHashingArray[p];
    // go to the last overflow bucket (if it exists)
    while (currentBucket->next != NULL)
    {
        prevBucket = currentBucket;
        currentBucket = currentBucket->next;
    }
    int voters_not_to_move = 0;
    while (currentBucket != NULL)
    {
        int key;
        int hashed_key;
        int counter = 0;
        for (int i = 0; i < bucketentries; i++)
        {
            counter = 0;
            if (currentBucket->data[i].PIN != -1)
            {
                key = currentBucket->data[i].PIN;

                hashed_key = key % h1; // new hashed key

                if (hashed_key == p)
                {

                    if (currentBucket != firstBucket)
                    {
                        voters_not_to_move++;
                    }
                }

                else
                {
                    for (int j = 0; j < bucketentries; j++)
                    {
                        if (hashed_key > current_buckets_number)
                        { // in large files i get 1 or 2 keys that this happens
                            break;
                        }
                        if (LinearHashingArray[hashed_key] == NULL)
                        {

                            break;
                        }
                        if (LinearHashingArray[hashed_key]->data[j].PIN == -1) // found empty bucket space so we put the key here
                        {
                            LinearHashingArray[hashed_key]->data[j].PIN = key;
                            LinearHashingArray[hashed_key]->data[j].v = (struct voter *)malloc(sizeof(struct voter));
                            if (LinearHashingArray[hashed_key]->data[j].v == NULL)
                            {
                                printf("Error malloc\n");
                                exit(-1);
                            }
                            strcpy(LinearHashingArray[hashed_key]->data[j].v->name, currentBucket->data[i].v->name);
                            strcpy(LinearHashingArray[hashed_key]->data[j].v->surname, currentBucket->data[i].v->surname);
                            LinearHashingArray[hashed_key]->data[j].v->PIN = key;
                            LinearHashingArray[hashed_key]->data[j].v->TK = currentBucket->data[i].v->TK;
                            LinearHashingArray[hashed_key]->data[j].v->voted = 'N';
                            // copied key and voter to the new array position
                            currentBucket->data[i].PIN = -1;
                            // emptied the voter from the old bucket
                            break;
                        }
                        else
                        {
                            counter++;
                            continue;
                        }
                    }
                    if (counter == bucketentries) // bucket is full
                    {
                        if (LinearHashingArray[hashed_key]->next == NULL)
                        {
                            struct NodeOfLinkedList *overflow_bucket = create_overflow_bucket(LinearHashingArray[hashed_key]->size); // created an overflow bucket
                            LinearHashingArray[hashed_key]->next = overflow_bucket;
                            LinearHashingArray[hashed_key]->next->data[0].PIN = key;
                            strcpy(LinearHashingArray[hashed_key]->next->data[0].v->name, currentBucket->data[i].v->name);
                            strcpy(LinearHashingArray[hashed_key]->next->data[0].v->surname, currentBucket->data[i].v->surname);
                            LinearHashingArray[hashed_key]->next->data[0].v->PIN = key;
                            LinearHashingArray[hashed_key]->next->data[0].v->TK = currentBucket->data[i].v->TK;
                            LinearHashingArray[hashed_key]->next->data[0].v->voted = 'N';
                            // copy the voter and the key to the first place of the bucket
                            currentBucket->data[i].PIN = -1;
                            // emtpy the voter from the old bucket
                        }
                        else
                        { // there is already an overflow bucket
                            struct NodeOfLinkedList *next_ptr = LinearHashingArray[hashed_key]->next;
                            while (next_ptr->next != NULL)
                            {
                                next_ptr = next_ptr->next;
                            }
                            // go to the last overflow bucket
                            int counter1 = 0;
                            for (int j = 0; j < bucketentries; j++)
                            {
                                if (next_ptr->data->PIN == -1)
                                {
                                    next_ptr->data[j].PIN = key;
                                    next_ptr->data[j].v = (struct voter *)malloc(sizeof(struct voter));
                                    if (next_ptr->data[j].v == NULL)
                                    {
                                        printf("Error malloc\n");
                                        exit(-1);
                                    }
                                    strcpy(next_ptr->data[j].v->name, currentBucket->data[i].v->name);
                                    strcpy(next_ptr->data[j].v->surname, currentBucket->data[i].v->surname);
                                    next_ptr->data[j].v->PIN = key;
                                    next_ptr->data[j].v->TK = currentBucket->data[i].v->TK;
                                    next_ptr->data[j].v->voted = 'N';
                                    // copied key and voter to the new array position
                                    currentBucket->data[i].PIN = -1;
                                    // emptied the voter from the old bucket
                                    break;
                                }
                                else
                                {
                                    counter1++;
                                    continue;
                                }
                            }
                            if (counter1 == bucketentries)
                            {                                                                                      // overflow bucket full
                                struct NodeOfLinkedList *overflow_bucket = create_overflow_bucket(next_ptr->size); // created an overflow bucket
                                next_ptr->next = overflow_bucket;
                                next_ptr->next->data[0].PIN = key;
                                strcpy(next_ptr->data[0].v->name, currentBucket->data[i].v->name);
                                strcpy(next_ptr->data[0].v->surname, currentBucket->data[i].v->surname);
                                next_ptr->data[0].v->PIN = key;
                                next_ptr->data[0].v->TK = currentBucket->data[i].v->TK;
                                next_ptr->data[0].v->voted = 'N';
                                // copy the voter and the key to the first place of the bucket
                                currentBucket->data[i].PIN = -1;
                                // emtpy the voter from the old bucket
                            }
                            else
                            {
                            }
                        }
                    }
                    // in the place where i want to insert the new hashed key the bucket is full so i need to create a new overflow bucket
                }
            }
        }

        int countr = 0;
        for (int i = 1; i < bucketentries; i++) // move each non empty element in front of the list
        {
            if (currentBucket->data[i].PIN != -1 && currentBucket->data[countr].PIN == -1)
            {
                currentBucket->data[countr].PIN = currentBucket->data[i].PIN;
                strcpy(currentBucket->data[countr].v->name, currentBucket->data[i].v->name);
                strcpy(currentBucket->data[countr].v->surname, currentBucket->data[i].v->surname);
                currentBucket->data[countr].v->PIN = currentBucket->data[i].v->PIN;
                currentBucket->data[countr].v->TK = currentBucket->data[i].v->TK;
                currentBucket->data[countr].v->voted = 'N';
                currentBucket->data[i].PIN = -1;
                countr++;
            }
        }

        currentBucket = prevBucket;
        prevBucket = NULL;
        if (currentBucket != LinearHashingArray[p]) // if the current bucket isnt the initial bucket
        {
            prevBucket = LinearHashingArray[p];
            // go to the previous bucket of the current bucket (case when there are more than one overflow buckets)
            while (prevBucket->next != currentBucket)
            {
                prevBucket = prevBucket->next;
            }
        }
    }
    if (voters_not_to_move == 0)
    {
        return LinearHashingArray;
    }
    struct voter **voters = (struct voter **)malloc(voters_not_to_move * sizeof(struct voter *));
    if (voters == NULL)
    {
        printf("Error malloc\n");
        exit(-1);
    }
    int cc = 0;
    currentBucket = LinearHashingArray[p]->next;
    int temp = voters_not_to_move;
    while (voters_not_to_move > 0)
    {
        for (int i = 0; i < bucketentries; i++)
        {
            if (currentBucket->data[i].PIN != -1)
            {
                voters[cc] = currentBucket->data[i].v;
                cc++;
                voters_not_to_move--;
                currentBucket->data[i].PIN = -1;
            }
        }
        currentBucket = currentBucket->next;
    }

    currentBucket = LinearHashingArray[p];
    int counter = 0;
    voters_not_to_move = temp;
    while (temp > 0 && currentBucket != NULL)
    {
        for (int i = 0; i < bucketentries; i++)
        {
            if (currentBucket->data[i].PIN == -1)
            {
                if (counter == voters_not_to_move)
                {
                    break;
                }
                currentBucket->data[i].PIN = voters[counter]->PIN;

                currentBucket->data[i].v = voters[counter];
                temp--;
                counter++;
            }
        }
        currentBucket = currentBucket->next;
    }

    return LinearHashingArray;
}

struct NodeOfLinkedList **initialization(int flagib, int bucketentries)
{
    struct NodeOfLinkedList **LinearHashingArray;
    if (flagib == 0)
    {
        printf("Initial bucket size must be more than zero!Try Again\n");
        exit(-1);
    }
    else
    {
        LinearHashingArray = (struct NodeOfLinkedList **)malloc(initial_bucket_size * sizeof(struct NodeOfLinkedList *)); // create the hashing array each element is a linked list ,each node is a bucket that is an array with bucketentries number
        if (LinearHashingArray == NULL)
        {
            printf("Error malloc\n");
            exit(-1);
        }
    }
    if (bucketentries == 0)
    {
        printf("Bucket entries must be more than zero!Try Again\n");
        exit(-1);
    }
    else
    {

        for (int i = 0; i < initial_bucket_size; i++)
        {
            LinearHashingArray = new_bucket(i, bucketentries, LinearHashingArray);
        }

        // right now we initialized the linearhasingarray with two elements(two linked lists) and each list contains one node (one bucket) that is an array of bucketentries elements
    }
    return LinearHashingArray;
}
struct NodeOfLinkedList **fill_hashing_table(FILE *fptr, struct NodeOfLinkedList **LinearHashingArray, int bucketentries)
{

    int PIN, TK;
    char name[100], surname[100];
    int temp_bucket_initial = initial_bucket_size;
    int previous_bucket_size = initial_bucket_size;
    h0 = h_right_factor(level, temp_bucket_initial);
    h1 = h_right_factor(level + 1, temp_bucket_initial);
    current_buckets_number = initial_bucket_size;
    int counter = 0;
    while (fscanf(fptr, "%d %99s %99s %d", &PIN, name, surname, &TK) == 4)
    {
        counter++;
        // this is where we are going to start the process of puting elements in the hashing table
        struct voter v;
        strcpy(v.name, name);
        strcpy(v.surname, surname);
        v.PIN = PIN;
        v.TK = TK;
        LinearHashingArray = hash_table_insert(&v, PIN, LinearHashingArray, bucketentries);
        if (current_buckets_number == 2 * previous_bucket_size)
        {
            previous_bucket_size = current_buckets_number;
            level = level + 1;
            p = 0;
            h0 = h_right_factor(level, temp_bucket_initial);
            h1 = h_right_factor(level + 1, temp_bucket_initial);
        }
    }

    return LinearHashingArray;
}

void free_hashing_array(struct NodeOfLinkedList **LinearHashingArray)
{
    for (int i = 0; i < current_buckets_number; i++)
    {
        free(LinearHashingArray[i]);
    }
}
struct NodeOfLinkedList **new_bucket(int i, int bucketentries, struct NodeOfLinkedList **LinearHashingArray)
{
    LinearHashingArray[i] = (struct NodeOfLinkedList *)malloc(sizeof(struct NodeOfLinkedList));
    if (LinearHashingArray[i] == NULL)
    {
        printf("Error malloc\n");
        exit(-1);
    }
    LinearHashingArray[i]->data = (struct bucket_element *)malloc(sizeof(struct bucket_element) * bucketentries);
    if (LinearHashingArray[i]->data == NULL)
    {
        printf("Error malloc\n");
        exit(-1);
    }
    LinearHashingArray[i]->size = bucketentries;
    for (int j = 0; j < bucketentries; j++)
    {
        LinearHashingArray[i]->data[j].PIN = -1; // initialize each element in each array with -1 so we know that we have an empty space in the bucket
    }
    LinearHashingArray[i]->next = NULL;
    return LinearHashingArray;
}
