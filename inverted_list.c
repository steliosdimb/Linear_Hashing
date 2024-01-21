#include "inverted_list.h"
#include <stdio.h>
#include <stdlib.h>
void print_inverted_list(struct NodeOfInvertedLinkedList *InvertedList)
{
    struct NodeOfInvertedLinkedList *curr_node = InvertedList;
    int i = 0;
    int j = 0;
    while (curr_node != NULL)
    {
        if (curr_node->TK == -1)
        {
            break;
        }
        else
        {
            printf("%d %d i ", curr_node->TK, i);

            struct ListOfVoters *curr_list = curr_node->LofV;
            while (curr_list != NULL)
            {
                printf("%s %d j\n", curr_list->v->name, j);
                curr_list = curr_list->next;
                j++;
            }
        }
        curr_node = curr_node->next;
        i++;
        j = 0;
    }
}

void free_inverted_list(struct NodeOfInvertedLinkedList *InvertedList)
{
    struct NodeOfInvertedLinkedList *currentNode = InvertedList;
    while (currentNode != NULL)
    {
        struct NodeOfInvertedLinkedList *nextNode = currentNode->next;

        struct ListOfVoters *currentList = currentNode->LofV;
        while (currentList != NULL)
        {
            struct ListOfVoters *nextList = currentList->next;
            free(currentList); // free ListOfVoters node
            currentList = nextList;
        }

        free(currentNode); // free InvertedLinkedList node
        currentNode = nextNode;
    }
}
struct NodeOfInvertedLinkedList *fill_inverted_list(struct NodeOfInvertedLinkedList *InvertedList, struct voter *v, int TK)
{
    struct NodeOfInvertedLinkedList *originalInvertedList = InvertedList; // Store the original pointer
    while (InvertedList != NULL)
    {
        if (InvertedList->TK == -1) // there are no TK in this node so fill this
        {
            InvertedList->TK = TK;
            InvertedList->LofV = (struct ListOfVoters *)malloc(sizeof(struct ListOfVoters));
            if (InvertedList->LofV == NULL)
            {
                printf("Error malloc\n");
                exit(-1);
            }
            InvertedList->LofV->next = NULL;
            InvertedList->LofV->v = v;
            struct NodeOfInvertedLinkedList *new_node = init_inverted_list();
            InvertedList->next = new_node;
            break;
        }
        else if (InvertedList->TK == TK) // if the TK already exists in the inverted list
        {
            struct ListOfVoters *curr_lofv = InvertedList->LofV;
            while (curr_lofv->next != NULL) // find the last inserted voter with this TK
            {
                curr_lofv = curr_lofv->next;
            }
            struct ListOfVoters *new_lofv = (struct ListOfVoters *)malloc(sizeof(struct ListOfVoters));
            if (new_lofv == NULL)
            {
                printf("Error malloc\n");
                exit(-1);
            }
            curr_lofv->next = new_lofv;
            new_lofv->next = NULL;
            new_lofv->v = v;
            break;
        }
        InvertedList = InvertedList->next;
    }

    return originalInvertedList; // Return the original pointer
}
struct NodeOfInvertedLinkedList *init_inverted_list(void)
{
    struct NodeOfInvertedLinkedList *invertedList = (struct NodeOfInvertedLinkedList *)malloc(sizeof(struct NodeOfInvertedLinkedList));
    if (invertedList == NULL)
    {
        printf("Error malloc\n");
        exit(-1);
    }
    invertedList->TK = -1;
    invertedList->LofV = NULL;
    invertedList->next = NULL;
    return invertedList;
}