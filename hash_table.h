#include <stdio.h>

#include "voter.h" // Include voter.h
struct bucket_element
{
    int PIN;
    struct voter *v;
};
struct NodeOfLinkedList
{
    struct bucket_element *data; // this is a linked list in the hash table(each linked list represents one array cell)
    int size;                    // size of array                            //this list contains 2,3, or 4 (dynamically allocate them bucket_elements)
    struct NodeOfLinkedList *next;
};
struct NodeOfLinkedList **hash_table_insert(struct voter *, int, struct NodeOfLinkedList **, int);
int h_right_factor(int, int);
struct NodeOfLinkedList *create_overflow_bucket(int);
void print_hash_table(int, struct NodeOfLinkedList **);
int number_of_keys(int, struct NodeOfLinkedList **);
int key_capacity_non_overflow(int);
struct NodeOfLinkedList **rehash(struct NodeOfLinkedList **, int);
struct NodeOfLinkedList **initialization(int, int);
struct NodeOfLinkedList **fill_hashing_table(FILE *, struct NodeOfLinkedList **, int);
void free_hashing_array(struct NodeOfLinkedList **);
struct voter *search_by_PIN(struct NodeOfLinkedList **, int, int);
struct NodeOfLinkedList **new_bucket(int, int, struct NodeOfLinkedList **);
