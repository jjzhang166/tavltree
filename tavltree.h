/*
 * Copyright 2015 TaberWalker <taberwalker@gmail.com>
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

#define TAVLTREE_DATAFIELD(type)    \
    struct type* avltreeLeft;       \
    struct type* avltreeRight;      \
    int avltreeHeight;              \

#define TAVLTREE_INITIALIZER()      { NULL }

#define TAVLTREE_INIT(root)         \
    do                              \
    {                               \
        (root) = NULL;              \
    } while (0)

#define TAVLTREE_EMPTY(node)        (NULL == (node))
#define TAVLTREE_LEFT(node)         ((node)->avltreeLeft)
#define TAVLTREE_RIGHT(node)        ((node)->avltreeRight)
#define TAVLTREE_HEIGHT(node)       ((node)->avltreeHeight)

#define TAVLTREE_GENERATE(type, cmp, cpy)                                   \
    int type##_TAVLTREE_HEIGHT(struct type* node)                           \
    {                                                                       \
        return NULL == node ? 0 : TAVLTREE_HEIGHT(node);                    \
    }                                                                       \
    void type##_TAVLTREE_UPDATEHEIGHT(struct type* node)                    \
    {                                                                       \
        const int lh = type##_TAVLTREE_HEIGHT(TAVLTREE_LEFT(node));         \
        const int rh = type##_TAVLTREE_HEIGHT(TAVLTREE_RIGHT(node));        \
        TAVLTREE_HEIGHT(node) = 1 + (lh > rh ? lh : rh);                    \
    }                                                                       \
    void type##_TAVLTREE_LLROTATE(struct type** node)                       \
    {                                                                       \
        struct type* tmp = TAVLTREE_LEFT(*node);                            \
        TAVLTREE_LEFT(*node) = TAVLTREE_RIGHT(tmp);                         \
        TAVLTREE_RIGHT(tmp) = (*node);                                      \
        type##_TAVLTREE_UPDATEHEIGHT(*node);                                \
        type##_TAVLTREE_UPDATEHEIGHT(tmp);                                  \
        (*node) = tmp;                                                      \
    }                                                                       \
    void type##_TAVLTREE_RRROTATE(struct type** node)                       \
    {                                                                       \
        struct type* tmp = TAVLTREE_RIGHT(*node);                           \
        TAVLTREE_RIGHT(*node) = TAVLTREE_LEFT(tmp);                         \
        TAVLTREE_LEFT(tmp) = (*node);                                       \
        type##_TAVLTREE_UPDATEHEIGHT(*node);                                \
        type##_TAVLTREE_UPDATEHEIGHT(tmp);                                  \
        (*node) = tmp;                                                      \
    }                                                                       \
    void type##_TAVLTREE_LRROTATE(struct type** node)                       \
    {                                                                       \
        type##_TAVLTREE_RRROTATE(&TAVLTREE_LEFT(*node));                    \
        type##_TAVLTREE_LLROTATE(node);                                     \
    }                                                                       \
    void type##_TAVLTREE_RLROTATE(struct type** node)                       \
    {                                                                       \
        type##_TAVLTREE_LLROTATE(&TAVLTREE_RIGHT(*node));                   \
        type##_TAVLTREE_RRROTATE(node);                                     \
    }                                                                       \
    void type##_TAVLTREE_INSERT(struct type** node, struct type* newNode)   \
    {                                                                       \
        int comp;                                                           \
        if (NULL == (*node))                                                \
        {                                                                   \
            TAVLTREE_LEFT(newNode) = TAVLTREE_RIGHT(newNode) = NULL;        \
            (*node) = newNode;                                              \
        }                                                                   \
        else                                                                \
        {                                                                   \
            comp = (cmp)(newNode, *node);                                   \
            if (comp < 0)                                                   \
            {                                                               \
                type##_TAVLTREE_INSERT(&TAVLTREE_LEFT(*node), newNode);     \
                if (type##_TAVLTREE_HEIGHT(TAVLTREE_LEFT(*node)) ==         \
                    type##_TAVLTREE_HEIGHT(TAVLTREE_RIGHT(*node)) + 2)      \
                {                                                           \
                    if ((cmp)(newNode, TAVLTREE_LEFT(*node)) < 0)           \
                        type##_TAVLTREE_LLROTATE(node);                     \
                    else                                                    \
                        type##_TAVLTREE_LRROTATE(node);                     \
                }                                                           \
            }                                                               \
            else if (comp > 0)                                              \
            {                                                               \
                type##_TAVLTREE_INSERT(&TAVLTREE_RIGHT(*node), newNode);    \
                if (type##_TAVLTREE_HEIGHT(TAVLTREE_RIGHT(*node)) ==        \
                    type##_TAVLTREE_HEIGHT(TAVLTREE_LEFT(*node)) + 2)       \
                {                                                           \
                    if ((cmp)(newNode, TAVLTREE_RIGHT(*node)) < 0)          \
                        type##_TAVLTREE_RLROTATE(node);                     \
                    else                                                    \
                        type##_TAVLTREE_RRROTATE(node);                     \
                }                                                           \
            }                                                               \
        }                                                                   \
        type##_TAVLTREE_UPDATEHEIGHT(*node);                                \
    }                                                                       \
    void type##_TAVLTREE_REMOVE(struct type** node,                         \
        struct type* delNode, struct type** rm)                             \
    {                                                                       \
        int comp;                                                           \
        struct type* tmp;                                                   \
        *rm = NULL;                                                         \
        if (NULL == (*node))                                                \
            return;                                                         \
        comp = (cmp)(delNode, *node);                                       \
        if (comp < 0)                                                       \
        {                                                                   \
            type##_TAVLTREE_REMOVE(&TAVLTREE_LEFT(*node), delNode, rm);     \
        }                                                                   \
        else if (comp > 0)                                                  \
        {                                                                   \
            type##_TAVLTREE_REMOVE(&TAVLTREE_RIGHT(*node), delNode, rm);    \
        }                                                                   \
        else                                                                \
        {                                                                   \
            if (NULL != TAVLTREE_LEFT(*node))                               \
            {                                                               \
                for (tmp = TAVLTREE_LEFT(*node);                            \
                    NULL != TAVLTREE_RIGHT(tmp);                            \
                    tmp = TAVLTREE_RIGHT(tmp));                             \
                (cpy)((*node), (const struct type*)(tmp));                  \
                type##_TAVLTREE_REMOVE(&TAVLTREE_LEFT(*node), tmp, rm);     \
            }                                                               \
            else if (NULL != TAVLTREE_RIGHT(*node))                         \
            {                                                               \
                for (tmp = TAVLTREE_RIGHT(*node);                           \
                    NULL != TAVLTREE_LEFT(tmp);                             \
                    tmp = TAVLTREE_LEFT(tmp));                              \
                (cpy)((*node), (const struct type*)(tmp));                  \
                type##_TAVLTREE_REMOVE(&TAVLTREE_RIGHT(*node), tmp, rm);    \
            }                                                               \
            else                                                            \
            {                                                               \
                (*rm) = (*node);                                            \
                (*node) = NULL;                                             \
                return;                                                     \
            }                                                               \
        }                                                                   \
        comp = type##_TAVLTREE_HEIGHT(TAVLTREE_LEFT(*node)) -               \
            type##_TAVLTREE_HEIGHT(TAVLTREE_RIGHT(*node));                  \
        if (2 == comp)                                                      \
        {                                                                   \
            if (NULL != TAVLTREE_LEFT(TAVLTREE_LEFT(*node)))                \
                type##_TAVLTREE_LLROTATE(node);                             \
            else                                                            \
                type##_TAVLTREE_LRROTATE(node);                             \
        }                                                                   \
        else if (-2 == comp)                                                \
        {                                                                   \
            if (NULL != TAVLTREE_LEFT(TAVLTREE_RIGHT(*node)))               \
                type##_TAVLTREE_RLROTATE(node);                             \
            else                                                            \
                type##_TAVLTREE_RRROTATE(node);                             \
        }                                                                   \
        type##_TAVLTREE_UPDATEHEIGHT(*node);                                \
    }                                                                       \
    void type##_TAVLTREE_TRAVEL(struct type* node,                          \
        void (*func)(const struct type* node))                              \
    {                                                                       \
        if (NULL != node)                                                   \
        {                                                                   \
            type##_TAVLTREE_TRAVEL(TAVLTREE_LEFT(node), func);              \
            func(node);                                                     \
            type##_TAVLTREE_TRAVEL(TAVLTREE_RIGHT(node), func);             \
        }                                                                   \
    }


#define TAVLTREE_INSERT(type, node, newNode)        type##_TAVLTREE_INSERT(node, newNode)
#define TAVLTREE_REMOVE(type, node, delNode, rm)    type##_TAVLTREE_REMOVE(node, delNode, rm)
#define TAVLTREE_TRAVEL(type, node, func)           type##_TAVLTREE_TRAVEL(node, func)

#endif // !__TAVLTREE_H__

