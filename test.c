#include "tavltree.h"
#include <stdio.h>
#include <stdlib.h>

struct IntNode
{
    int n;
    TAVLTREE_DATAFIELD(IntNode);
};

int IntNodeCmp(struct IntNode* a, struct IntNode* b)
{
    return a->n - b->n;
}

void IntNodeCpy(struct IntNode* dst, const struct IntNode* src)
{
    dst->n = src->n;
}

TAVLTREE_GENERATE(IntNode, IntNodeCmp, IntNodeCpy);


void Dump(const struct IntNode* node)
{
    printf("%d ", node->n);
}

struct IntNode* CreateIntNode(int n)
{
    struct IntNode* node = (struct IntNode*)malloc(sizeof(struct IntNode));
    printf("malloc:%p\n", (int*)node);
    node->n = n;
    return node;
}

void DestroyIntNode(struct IntNode* node)
{
    printf("free:%p\n", (int*)node);
    if (NULL != node)
        free(node);
}

int main(int argc, char** argv)
{
    struct IntNode* root = NULL;
    struct IntNode* rm;
    struct IntNode tmp;
    int i;

    for (i = 0; i < 10; ++i)
    {
        TAVLTREE_INSERT(IntNode, &root, CreateIntNode(i));

        TAVLTREE_TRAVEL(IntNode, root, Dump);
        printf("\n");
    }

    for (i = 0; i < 10; ++i)
    {
        tmp.n = i;
        TAVLTREE_REMOVE(IntNode, &root, &tmp, &rm);
        DestroyIntNode(rm);

        TAVLTREE_TRAVEL(IntNode, root, Dump);
        printf("\n");
    }

    return 0;
}

