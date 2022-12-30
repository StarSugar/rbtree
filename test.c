#include <stdlib.h>
#include "rbtree.h"
#include <errno.h>
#include <stdio.h>

struct testtype {
        struct rb_node node;
        int val;
};

#define testtype_of(n) (container_of((n), struct testtype, node))

struct rb_root tree;

void init(void)
{
        tree.rb_node = NULL;
}

static inline struct rb_node *findnode(int val)
{
        struct rb_node *node = tree.rb_node;
        while(node)
        {
                if(testtype_of(node)->val < val)
                        node = node->rb_left;
                else if(testtype_of(node)->val > val)
                        node = node->rb_right;
                else
                        return node;
        }
        return NULL;
}

int exist(int val)
{
        return findnode(val) ? 1 : 0;
}

void insert(int val)
{
        struct rb_node **node_p, *parent = NULL;
        struct testtype *x;
        node_p = &tree.rb_node;
        while(*node_p)
        {
                parent = *node_p;
                if(testtype_of(parent)->val < val)
                        node_p = &parent->rb_left;
                else if(testtype_of(parent)->val > val)
                        node_p = &parent->rb_right;
                else
                        return;
        }
        x = malloc(sizeof(struct testtype));
        if(x == NULL) exit(ENOMEM);
        x->val = val;
        rb_link_node(&x->node, parent, node_p);
        rb_insert_color(&x->node, &tree);
}

void erase(int val)
{
        struct rb_node *node;
        struct testtype *x;
        node = findnode(val);
        if(node != NULL)
        {
                x = testtype_of(node);
                rb_erase(node, &tree);
                free(x);
        }
}

int cmp(const void *a1, const struct rb_node* a2){
        return 0;
}

int main()
{
        int i;
        struct rb_node *node;
        init();
        for(i = -500;i < 500;i++)
        {
                insert(i);
        }
        rb_for_each(node, NULL, &tree, cmp){
                printf("%d\n", testtype_of(node)->val);
        }
        return 0;
}
