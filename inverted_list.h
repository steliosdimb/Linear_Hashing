
#include "voter.h"

struct NodeOfInvertedLinkedList
{
    int TK;
    struct ListOfVoters *LofV;
    struct NodeOfInvertedLinkedList *next;
};
struct ListOfVoters
{
    struct voter *v;
    struct ListOfVoters *next;
};
struct NodeOfInvertedLinkedList *init_inverted_list(void);
struct NodeOfInvertedLinkedList *fill_inverted_list(struct NodeOfInvertedLinkedList *, struct voter *, int);
void print_inverted_list(struct NodeOfInvertedLinkedList *);
void free_inverted_list(struct NodeOfInvertedLinkedList *);
