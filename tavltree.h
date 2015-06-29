/*
 * Copyright 2015 Taber Walker <taberwalker@gmail.com>
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE AUTHOR ``AS IS'' AND ANY EXPRESS OR
 * IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES
 * OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.
 * IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR ANY DIRECT, INDIRECT,
 * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT
 * NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
 * DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
 * THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF
 * THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#ifndef __TAVLTREE_H__
#define __TAVLTREE_H__

/* HOW-TO-USE
---------------------------------------------------------------------------------
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

---------------------------------------------------------------------------------
*/

#define TAVLTREE_DATAFIELD(type)                                                \
    struct type* avltreeParent;                                                 \
    struct type* avltreeLeft;                                                   \
    struct type* avltreeRight;                                                  \
    int avltreeHeight;

#define TAVLTREE_DECLARE(type)                                                  \
    struct type* type##_TAVLTREE_INSERT(struct type** root, struct type* node); \
    struct type* type##_TAVLTREE_REMOVE(struct type** root, struct type* node); \
    struct type* type##_TAVLTREE_FIND(struct type* root, struct type* node);    \
    struct type* type##_TAVLTREE_NEXT(struct type* node);                       \
    struct type* type##_TAVLTREE_PREV(struct type* node);                       \
    struct type* type##_TAVLTREE_MIN(struct type* root);                        \
    struct type* type##_TAVLTREE_MAX(struct type* root);
    
#define TAVLTREE_INSERT(type, root, node)   type##_TAVLTREE_INSERT(root, node)
#define TAVLTREE_REMOVE(type, root, node)   type##_TAVLTREE_REMOVE(root, node)
#define TAVLTREE_FIND(type, root, node)     type##_TAVLTREE_FIND(root, node)
#define TAVLTREE_NEXT(type, node)           type##_TAVLTREE_NEXT(node)
#define TAVLTREE_PREV(type, node)           type##_TAVLTREE_PREV(node)
#define TAVLTREE_MIN(type, node)            type##_TAVLTREE_MIN(node)
#define TAVLTREE_MAX(type, node)            type##_TAVLTREE_MAX(node)

#define TAVLTREE_FOREACH(type, root, tmp)                                       \
    for ((tmp) = TAVLTREE_MIN(type, root);                                      \
        NULL != (tmp);                                                          \
        (tmp) = TAVLTREE_NEXT(type, tmp))

#define TAVLTREE_FOREACH_FROM(type, from, tmp)                                  \
    for ((tmp) = (from);                                                        \
        NULL != (tmp);                                                          \
        (tmp) = TAVLTREE_NEXT(type, tmp))

#define TAVLTREE_FOREACH_REVERSE(type, root, tmp)                               \
    for ((tmp) = TAVLTREE_MAX(type, root);                                      \
        NULL != (tmp);                                                          \
        (tmp) = TAVLTREE_PREV(type, tmp))

#define TAVLTREE_FOREACH_REVERSE_FROM(type, from, tmp)                          \
    for ((tmp) = (from);                                                        \
        NULL != (tmp);                                                          \
        (tmp) = TAVLTREE_PREV(type, tmp))


#define TAVLTREE_PARENT(node)       ((node)->avltreeParent)
#define TAVLTREE_LEFT(node)         ((node)->avltreeLeft)
#define TAVLTREE_RIGHT(node)        ((node)->avltreeRight)
#define TAVLTREE_HEIGHT(node)       ((node)->avltreeHeight)

#define TAVLTREE_DEFINE(type, cmp)                                              \
    int type##_TAVLTREE_HEIGHT(struct type* node)                               \
    {                                                                           \
        return (NULL != node ? TAVLTREE_HEIGHT(node) : 0);                      \
    }                                                                           \
                                                                                \
    struct type* type##_TAVLTREE_LLROTATE(struct type* node)                    \
    {                                                                           \
        int lh, rh;                                                             \
        struct type* tmp = TAVLTREE_LEFT(node);                                 \
                                                                                \
        if (NULL != TAVLTREE_PARENT(node))                                      \
        {                                                                       \
            if (node == TAVLTREE_LEFT(TAVLTREE_PARENT(node)))                   \
                TAVLTREE_LEFT(TAVLTREE_PARENT(node)) = tmp;                     \
            else                                                                \
                TAVLTREE_RIGHT(TAVLTREE_PARENT(node)) = tmp;                    \
        }                                                                       \
        TAVLTREE_PARENT(tmp) = TAVLTREE_PARENT(node);                           \
                                                                                \
        TAVLTREE_LEFT(node) = TAVLTREE_RIGHT(tmp);                              \
        if (NULL != TAVLTREE_LEFT(node))                                        \
            TAVLTREE_PARENT(TAVLTREE_LEFT(node)) = node;                        \
        TAVLTREE_RIGHT(tmp) = node;                                             \
        TAVLTREE_PARENT(node) = tmp;                                            \
                                                                                \
        lh = type##_TAVLTREE_HEIGHT(TAVLTREE_LEFT(node));                       \
        rh = type##_TAVLTREE_HEIGHT(TAVLTREE_RIGHT(node));                      \
        TAVLTREE_HEIGHT(node) = 1 + (lh > rh ? lh : rh);                        \
                                                                                \
        lh = type##_TAVLTREE_HEIGHT(TAVLTREE_LEFT(tmp));                        \
        rh = type##_TAVLTREE_HEIGHT(TAVLTREE_RIGHT(tmp));                       \
        TAVLTREE_HEIGHT(tmp) = 1 + (lh > rh ? lh : rh);                         \
                                                                                \
        return tmp;                                                             \
    }                                                                           \
                                                                                \
    struct type* type##_TAVLTREE_RRROTATE(struct type* node)                    \
    {                                                                           \
        int lh, rh;                                                             \
        struct type* tmp = TAVLTREE_RIGHT(node);                                \
                                                                                \
        if (NULL != TAVLTREE_PARENT(node))                                      \
        {                                                                       \
            if (node == TAVLTREE_LEFT(TAVLTREE_PARENT(node)))                   \
                TAVLTREE_LEFT(TAVLTREE_PARENT(node)) = tmp;                     \
            else                                                                \
                TAVLTREE_RIGHT(TAVLTREE_PARENT(node)) = tmp;                    \
        }                                                                       \
        TAVLTREE_PARENT(tmp) = TAVLTREE_PARENT(node);                           \
                                                                                \
        TAVLTREE_RIGHT(node) = TAVLTREE_LEFT(tmp);                              \
        if (NULL != TAVLTREE_RIGHT(node))                                       \
            TAVLTREE_PARENT(TAVLTREE_RIGHT(node)) = node;                       \
        TAVLTREE_LEFT(tmp) = node;                                              \
        TAVLTREE_PARENT(node) = tmp;                                            \
                                                                                \
        lh = type##_TAVLTREE_HEIGHT(TAVLTREE_LEFT(node));                       \
        rh = type##_TAVLTREE_HEIGHT(TAVLTREE_RIGHT(node));                      \
        TAVLTREE_HEIGHT(node) = 1 + (lh > rh ? lh : rh);                        \
                                                                                \
        lh = type##_TAVLTREE_HEIGHT(TAVLTREE_LEFT(tmp));                        \
        rh = type##_TAVLTREE_HEIGHT(TAVLTREE_RIGHT(tmp));                       \
        TAVLTREE_HEIGHT(tmp) = 1 + (lh > rh ? lh : rh);                         \
                                                                                \
        return tmp;                                                             \
    }                                                                           \
                                                                                \
    struct type* type##_TAVLTREE_LRROTATE(struct type* node)                    \
    {                                                                           \
        node = type##_TAVLTREE_RRROTATE(TAVLTREE_LEFT(node));                   \
        return type##_TAVLTREE_LLROTATE(node);                                  \
    }                                                                           \
                                                                                \
    struct type* type##_TAVLTREE_RLROTATE(struct type* node)                    \
    {                                                                           \
        node = type##_TAVLTREE_LLROTATE(TAVLTREE_RIGHT(node));                  \
        return type##_TAVLTREE_RRROTATE(node);                                  \
    }                                                                           \
                                                                                \
    struct type* type##_TAVLTREE_BALANCE(struct type* node)                     \
    {                                                                           \
        const int lh = type##_TAVLTREE_HEIGHT(TAVLTREE_LEFT(node));             \
        const int rh = type##_TAVLTREE_HEIGHT(TAVLTREE_RIGHT(node));            \
                                                                                \
        TAVLTREE_HEIGHT(node) = 1 + (lh > rh ? lh : rh);                        \
                                                                                \
        if (lh == rh + 2)                                                       \
        {                                                                       \
            if (NULL != TAVLTREE_LEFT(TAVLTREE_LEFT(node)))                     \
                node = type##_TAVLTREE_LLROTATE(node);                          \
            else                                                                \
                node = type##_TAVLTREE_LRROTATE(node);                          \
        }                                                                       \
        else if (rh == lh + 2)                                                  \
        {                                                                       \
            if (NULL != TAVLTREE_RIGHT(TAVLTREE_RIGHT(node)))                   \
                node = type##_TAVLTREE_RRROTATE(node);                          \
            else                                                                \
                node = type##_TAVLTREE_RLROTATE(node);                          \
        }                                                                       \
                                                                                \
        return node;                                                            \
    }                                                                           \
                                                                                \
    struct type* type##_TAVLTREE_INSERT(struct type** root, struct type* node)  \
    {                                                                           \
        struct type* tmp = (*root);                                             \
        struct type* parent = NULL;                                             \
        int comp = 0;                                                           \
                                                                                \
        while (NULL != tmp)                                                     \
        {                                                                       \
            parent = tmp;                                                       \
            comp = (cmp)(node, tmp);                                            \
            if (comp < 0)                                                       \
                tmp = TAVLTREE_LEFT(tmp);                                       \
            else if (comp > 0)                                                  \
                tmp = TAVLTREE_RIGHT(tmp);                                      \
            else                                                                \
                return tmp;                                                     \
        }                                                                       \
                                                                                \
        TAVLTREE_PARENT(node) = parent;                                         \
        TAVLTREE_LEFT(node) = TAVLTREE_RIGHT(node) = NULL;                      \
        TAVLTREE_HEIGHT(node) = 1;                                              \
                                                                                \
        if (NULL != parent)                                                     \
        {                                                                       \
            if (comp < 0)                                                       \
                TAVLTREE_LEFT(parent) = node;                                   \
            else                                                                \
                TAVLTREE_RIGHT(parent) = node;                                  \
                                                                                \
            while (NULL != parent)                                              \
            {                                                                   \
                parent = type##_TAVLTREE_BALANCE(parent);                       \
                tmp = parent;                                                   \
                parent = TAVLTREE_PARENT(parent);                               \
            }                                                                   \
            (*root) = tmp;                                                      \
        }                                                                       \
        else                                                                    \
        {                                                                       \
            (*root) = node;                                                     \
        }                                                                       \
                                                                                \
        return NULL;                                                            \
    }                                                                           \
                                                                                \
    struct type* type##_TAVLTREE_REMOVE(struct type** root, struct type* node)  \
    {                                                                           \
        struct type* tmp = (*root);                                             \
        struct type* parent = NULL;                                             \
        int comp = 0;                                                           \
                                                                                \
        while (NULL != tmp)                                                     \
        {                                                                       \
            parent = tmp;                                                       \
            comp = (cmp)(node, tmp);                                            \
            if (comp < 0)                                                       \
                tmp = TAVLTREE_LEFT(tmp);                                       \
            else if (comp > 0)                                                  \
                tmp = TAVLTREE_RIGHT(tmp);                                      \
            else                                                                \
                break;                                                          \
        }                                                                       \
                                                                                \
        if (NULL == tmp)                                                        \
            return NULL;                                                        \
                                                                                \
        if (NULL != TAVLTREE_LEFT(tmp))                                         \
        {                                                                       \
            for (node = TAVLTREE_LEFT(tmp);                                     \
                NULL != TAVLTREE_RIGHT(node);                                   \
                node = TAVLTREE_RIGHT(node));                                   \
            parent = TAVLTREE_PARENT(node);                                     \
            if (node == TAVLTREE_LEFT(parent))                                  \
                TAVLTREE_LEFT(parent) = NULL;                                   \
            else                                                                \
                TAVLTREE_RIGHT(parent) = NULL;                                  \
                                                                                \
            TAVLTREE_LEFT(node) = TAVLTREE_LEFT(tmp);                           \
            TAVLTREE_RIGHT(node) = TAVLTREE_RIGHT(tmp);                         \
            if (NULL != TAVLTREE_PARENT(tmp))                                   \
            {                                                                   \
                if (tmp == TAVLTREE_LEFT(TAVLTREE_PARENT(tmp)))                 \
                    TAVLTREE_LEFT(TAVLTREE_PARENT(tmp)) = node;                 \
                else                                                            \
                    TAVLTREE_RIGHT(TAVLTREE_PARENT(tmp)) = node;                \
            }                                                                   \
            parent = TAVLTREE_PARENT(node) = TAVLTREE_PARENT(tmp);              \
            while (NULL != parent)                                              \
            {                                                                   \
                parent = type##_TAVLTREE_BALANCE(parent);                       \
                node = parent;                                                  \
                parent = TAVLTREE_PARENT(parent);                               \
            }                                                                   \
            (*root) = node;                                                     \
        }                                                                       \
        else if (NULL != TAVLTREE_RIGHT(tmp))                                   \
        {                                                                       \
            for (node = TAVLTREE_RIGHT(tmp);                                    \
                NULL != TAVLTREE_LEFT(node);                                    \
                node = TAVLTREE_LEFT(node));                                    \
            parent = TAVLTREE_PARENT(node);                                     \
            if (node == TAVLTREE_LEFT(parent))                                  \
                TAVLTREE_LEFT(parent) = NULL;                                   \
            else                                                                \
                TAVLTREE_RIGHT(parent) = NULL;                                  \
                                                                                \
            TAVLTREE_LEFT(node) = TAVLTREE_LEFT(tmp);                           \
            TAVLTREE_RIGHT(node) = TAVLTREE_RIGHT(tmp);                         \
            if (NULL != TAVLTREE_PARENT(tmp))                                   \
            {                                                                   \
                if (tmp == TAVLTREE_LEFT(TAVLTREE_PARENT(tmp)))                 \
                    TAVLTREE_LEFT(TAVLTREE_PARENT(tmp)) = node;                 \
                else                                                            \
                    TAVLTREE_RIGHT(TAVLTREE_PARENT(tmp)) = node;                \
            }                                                                   \
            parent = TAVLTREE_PARENT(node) = TAVLTREE_PARENT(tmp);              \
            while (NULL != parent)                                              \
            {                                                                   \
                parent = type##_TAVLTREE_BALANCE(parent);                       \
                node = parent;                                                  \
                parent = TAVLTREE_PARENT(parent);                               \
            }                                                                   \
            (*root) = node;                                                     \
        }                                                                       \
        else                                                                    \
        {                                                                       \
            if (NULL != TAVLTREE_PARENT(tmp))                                   \
            {                                                                   \
                if (tmp == TAVLTREE_LEFT(TAVLTREE_PARENT(tmp)))                 \
                    TAVLTREE_LEFT(TAVLTREE_PARENT(tmp)) = NULL;                 \
                else                                                            \
                    TAVLTREE_RIGHT(TAVLTREE_PARENT(tmp)) = NULL;                \
            }                                                                   \
            parent = TAVLTREE_PARENT(tmp);                                      \
            node = NULL;                                                        \
            while (NULL != parent)                                              \
            {                                                                   \
                parent = type##_TAVLTREE_BALANCE(parent);                       \
                node = parent;                                                  \
                parent = TAVLTREE_PARENT(parent);                               \
            }                                                                   \
            (*root) = node;                                                     \
        }                                                                       \
                                                                                \
        return tmp;                                                             \
    }                                                                           \
                                                                                \
    struct type* type##_TAVLTREE_FIND(struct type* root, struct type* node)     \
    {                                                                           \
        int comp = 0;                                                           \
        while (NULL != root)                                                    \
        {                                                                       \
            comp = (cmp)(node, root);                                           \
            if (comp < 0)                                                       \
                root = TAVLTREE_LEFT(root);                                     \
            else if (comp > 0)                                                  \
                root = TAVLTREE_RIGHT(root);                                    \
            else                                                                \
                return root;                                                    \
        }                                                                       \
        return NULL;                                                            \
    }                                                                           \
                                                                                \
    struct type* type##_TAVLTREE_NEXT(struct type* node)                        \
    {                                                                           \
        if (NULL != TAVLTREE_RIGHT(node))                                       \
        {                                                                       \
            node = TAVLTREE_RIGHT(node);                                        \
            while (NULL != TAVLTREE_LEFT(node))                                 \
                node = TAVLTREE_LEFT(node);                                     \
        }                                                                       \
        else                                                                    \
        {                                                                       \
            if (NULL != TAVLTREE_PARENT(node)                                   \
                && node == TAVLTREE_LEFT(TAVLTREE_PARENT(node)))                \
            {                                                                   \
                node = TAVLTREE_PARENT(node);                                   \
            }                                                                   \
            else                                                                \
            {                                                                   \
                while (NULL != TAVLTREE_PARENT(node)                            \
                    && node == TAVLTREE_RIGHT(TAVLTREE_PARENT(node)))           \
                    node = TAVLTREE_PARENT(node);                               \
                node = TAVLTREE_PARENT(node);                                   \
            }                                                                   \
        }                                                                       \
                                                                                \
        return node;                                                            \
    }                                                                           \
                                                                                \
    struct type* type##_TAVLTREE_PREV(struct type* node)                        \
    {                                                                           \
        if (NULL != TAVLTREE_LEFT(node))                                        \
        {                                                                       \
            node = TAVLTREE_LEFT(node);                                         \
            while (NULL != TAVLTREE_RIGHT(node))                                \
                node = TAVLTREE_RIGHT(node);                                    \
        }                                                                       \
        else                                                                    \
        {                                                                       \
            if (NULL != TAVLTREE_PARENT(node)                                   \
                && node == TAVLTREE_RIGHT(TAVLTREE_PARENT(node)))               \
            {                                                                   \
                node = TAVLTREE_PARENT(node);                                   \
            }                                                                   \
            else                                                                \
            {                                                                   \
                while (NULL != TAVLTREE_PARENT(node)                            \
                    && node == TAVLTREE_LEFT(TAVLTREE_PARENT(node)))            \
                    node = TAVLTREE_PARENT(node);                               \
                node = TAVLTREE_PARENT(node);                                   \
            }                                                                   \
        }                                                                       \
                                                                                \
        return node;                                                            \
    }                                                                           \
                                                                                \
    struct type* type##_TAVLTREE_MIN(struct type* root)                         \
    {                                                                           \
        if (NULL != root)                                                       \
        {                                                                       \
            while (NULL != TAVLTREE_LEFT(root))                                 \
                root = TAVLTREE_LEFT(root);                                     \
        }                                                                       \
        return root;                                                            \
    }                                                                           \
                                                                                \
    struct type* type##_TAVLTREE_MAX(struct type* root)                         \
    {                                                                           \
        if (NULL != root)                                                       \
        {                                                                       \
            while (NULL != TAVLTREE_RIGHT(root))                                \
                root = TAVLTREE_RIGHT(root);                                    \
        }                                                                       \
        return root;                                                            \
    }

#endif // !__TAVLTREE_H__

