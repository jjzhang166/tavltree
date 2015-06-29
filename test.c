#include "tavltree.h"
#include <stdio.h>

struct IntNode
{
    int n;
    TAVLTREE_DATAFIELD(IntNode);
};
TAVLTREE_DECLARE(IntNode);

int main(int argc, char** argv)
{
    struct IntNode cache[16];
    const int N = (int)(sizeof(cache) / sizeof(cache[0]));
    struct IntNode* root = NULL;
    struct IntNode* tmp;
    int i;
    struct IntNode x;

    for (i = 0; i < N; ++i)
    {
        cache[i].n = i;
        TAVLTREE_INSERT(IntNode, &root, &cache[i]);
        TAVLTREE_FOREACH(IntNode, root, tmp)
            printf("%d ", tmp->n);
        printf("\n");
    }
    for (i = 0; i < N; ++i)
    {
        x.n = i;
        tmp = TAVLTREE_REMOVE(IntNode, &root, &x);
        if (&cache[i] != tmp) // <--------------It will NOT occur!!!
            printf("memory mismatch when remove %d[%p, %p]\n", i, (int*)&cache[i], (int*)tmp);
        TAVLTREE_FOREACH(IntNode, root, tmp)
            printf("%d ", tmp->n);
        printf("\n");
    }
    return 0;
}

int IntNodeCompare(struct IntNode* a, struct IntNode* b) { return a->n - b->n; }
TAVLTREE_DEFINE(IntNode, IntNodeCompare);

