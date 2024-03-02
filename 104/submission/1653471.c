#include <stdlib.h>
#include <stdio.h>
#include <limits.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <stdbool.h>
#include <ctype.h>

void* generic_max(void* lhs, void* rhs, int (*compare)(void* lhs, void* rhs)) {
    if (compare(lhs, rhs) > 0) { /* lhs > rhs */
        return lhs;
    } else {
        return rhs;
    }
}

void* generic_min(void* lhs, void* rhs, int (*compare)(void* lhs, void* rhs)) {
    if (compare(lhs, rhs) > 0) { /* lhs > rhs */
        return rhs;
    } else {
        return lhs;
    }
}

enum RedBlackTreeColor {
    RBT_BLACK,
    RBT_RED
};

struct RedBlackTreeNode {
    /* Basic data field */
    struct RedBlackTreeNode* child[2];
    void* key;
    struct RedBlackTreeNode* p;
    enum RedBlackTreeColor color;
    /* Argumented data field */
    int size; /* Subtree size */
    int cnt;  /* Duplicate element count */
};

struct RedBlackTree {
    struct RedBlackTreeNode* root;
    struct RedBlackTreeNode* nil;

    /* Compare function:
     * if lhs < rhs, return negative;
     * if lhs == rhs, return 0;
     * otherwise, return positive.
     */
    int (*compare)(void* lhs, void* rhs);
    void* MAX;
    void* MIN;
};

/* Create a new Red Black Tree node */
struct RedBlackTreeNode* redblacktree_node_init(void) {
    struct RedBlackTreeNode* node = malloc(sizeof(struct RedBlackTreeNode));
    /* Basic data field init */
    node -> child[0] = NULL;
    node -> child[1] = NULL;
    node -> color = RBT_BLACK;
    node -> p = NULL;
    node -> key = NULL;
    /* Argumented data field init */
    node -> size = 0;
    node -> cnt = 0;
    return node;
}
/* Create a new Red Black Tree node with initial value */
struct RedBlackTreeNode*
redblacktree_node_init2(void* key,
                        int size,
                        struct RedBlackTreeNode* left,
                        struct RedBlackTreeNode* right,
                        struct RedBlackTreeNode* p,
                        enum RedBlackTreeColor color) {
    struct RedBlackTreeNode* node = malloc(sizeof(struct RedBlackTreeNode));
    /* Basic data field init */
    node -> child[0] = left;
    node -> child[1] = right;
    node -> color = color;
    node -> p = p;
    node -> key = malloc(size);
    memcpy(node -> key, key, size);
    /* Argumented data field init */
    node -> size = 1;
    node -> cnt = 1;
    return node;
}
void redblacktree_node_deinit(struct RedBlackTreeNode* node) {
    free(node -> key);
    free(node);
}

/* Create a new empty Red Black Tree */
void redblacktree_init(struct RedBlackTree* tree,
                       int (*compare)(void* lhs, void* rhs),
                       void* MAX,
                       void* MIN) {
    tree -> nil = redblacktree_node_init();
    /* Argumented data field init */
//    tree -> nil -> size = 0;
//    tree -> nil -> cnt = 0;

    tree -> root = tree -> nil;
    tree -> root -> p = tree -> nil;

    tree -> compare = compare;
    tree -> MAX = MAX;
    tree -> MIN = MIN;
}

/* Red Black Tree Rotation */
/*
 *        |                               |
 *       [y]       left_rotate(x)        [x]
 *      /   \      <--------------      /   \
 *    [x]    c                         a    [y]
 *   /   \         -------------->         /   \
 *  a     b        right_rotate(y)        b     c
 */
void redblacktree_rotate(struct RedBlackTree* tree,
                         struct RedBlackTreeNode* x,
                         char is_right) {
    struct RedBlackTreeNode* y = x -> child[is_right ^ 1];
    x -> child[is_right ^ 1] = y -> child[is_right];
    if (y -> child[is_right] != tree -> nil) {
        y -> child[is_right] -> p = x;
    }
    y -> p = x -> p;
    if (x -> p == tree -> nil) {
        tree -> root = y;
    } else {
        x -> p -> child[x == x -> p -> child[1] ? 1 : 0] = y;
    }
    y -> child[is_right] = x;
    x -> p = y;
    /* Maintain argumented data */
    y -> size = x -> size;
    x -> size = x -> child[0] -> size + x -> child[1] -> size + x -> cnt;
}

/* Case 1: z's uncle y is red, {[A]: red node, (A): black node}
 *
 *           |                                   |
 *          (C)                                 [z] <--~ {new z}
 *         /   \                               /   \
 *        /     \                             /     \
 *       /       \                           / _old z\
 *      /         \                         / /       \
 *    [A]         [y]    ------------>    (A) |       (y)
 *   /   \       /   \                   /   \|      /   \
 *  a    [z]    d     e                 a    [z]    d     e
 *      /   \                               /   \
 *     b     c                             b     c
 *
 * Because z.p.p (in here: C) is black, we can color both z.p (A) and y black,
 * thereby fixing the problem of z and z.p (A) both being red, and we can color
 * z.p.p (C) red, thereby maintaining property 5. We then repeat the while loop
 * with z.p.p (C) as the new node z. The pointer z moves up two levels in the
 * tree.
 */
void redblacktree_insert_fixup(struct RedBlackTree* tree,
                               struct RedBlackTreeNode* node) {
    struct RedBlackTreeNode* p;
    struct RedBlackTreeNode* gp;
    struct RedBlackTreeNode* y;
    struct RedBlackTreeNode* z = node;
    char is_left;
    while (z -> p -> color == RBT_RED) {
        p = z -> p;
        gp = p -> p;
        is_left = p == gp -> child[0] ? 1 : 0;
        y = gp -> child[is_left];
        if (y -> color == RBT_RED) { /* Case 1 */
            y -> color = RBT_BLACK;
            p -> color = RBT_BLACK;
            gp -> color = RBT_RED;
            z = gp;
        } else {
            if (z == p -> child[is_left]) { /* Case 2 */
                z = p;
                redblacktree_rotate(tree, z, is_left ^ 1);
            }
            /* Case 3 */
            z -> p -> color = RBT_BLACK;
            z -> p -> p -> color = RBT_RED;
            redblacktree_rotate(tree, gp, is_left);
        }
    }
    tree -> root -> color = RBT_BLACK;
}

void redblacktree_insert(struct RedBlackTree* tree, void* key, int size) {
    struct RedBlackTreeNode* x = tree -> root;
    struct RedBlackTreeNode* y = tree -> nil;
    struct RedBlackTreeNode* z = redblacktree_node_init2(key,
                                                         size,
                                                         tree -> nil,
                                                         tree -> nil,
                                                         tree -> nil,
                                                         RBT_RED);
    /* Binary Search Tree Insertion */
    while (x != tree -> nil) { /* find the position to insert */
        y = x;
        y -> size += 1;
        /* If exists, add `cnt` by 1. */
        if (tree -> compare(x -> key, key) == 0) {
            x -> cnt += 1;
            return;
        }
        x = x -> child[(tree -> compare(x -> key, key) < 0) ? 1 : 0];
    }
    z -> p = y;
    if (y == tree -> nil) {
        tree -> root = z;
    } else {
        y -> child[(tree -> compare(y -> key, key) < 0) ? 1 : 0] = z;
    }
    redblacktree_insert_fixup(tree, z);
}

struct RedBlackTreeNode* redblacktree_minimum(struct RedBlackTree tree,
                                              struct RedBlackTreeNode* node) {
    while (node -> child[0] != tree.nil) {
        node = node -> child[0];
    }
    return node;
}

void redblacktree_transplant(struct RedBlackTree* tree,
                struct RedBlackTreeNode* u,
                struct RedBlackTreeNode* v) {
    v -> p = u -> p;
    if (u -> p == tree -> nil) {
        tree -> root = v;
    } else {
        u -> p -> child[(u == u -> p -> child[1]) ? 1 : 0] = v;
    }
}

void redblacktree_delete_fixup(struct RedBlackTree* tree,
                 struct RedBlackTreeNode* node) {
    struct RedBlackTreeNode* x = node;
    struct RedBlackTreeNode* p;
    struct RedBlackTreeNode* w;
    char is_left = 0;
    while (x != tree -> root && x -> color == RBT_BLACK) {
        p = x -> p;
        is_left = x == x -> p -> child[0] ? 1 : 0;
        w = p -> child[is_left];
        if (w -> color == RBT_RED) {
            p -> color = RBT_RED;
            w -> color = RBT_BLACK;
            redblacktree_rotate(tree, p, is_left ^ 1);
            w = p -> child[is_left];
        }
        if (w -> child[0] -> color == RBT_BLACK &&
            w -> child[1] -> color == RBT_BLACK) {
            w -> color = RBT_RED;
            x = x -> p;
        } else {
            if (w -> child[is_left] -> color == RBT_BLACK) {
                w -> color = RBT_RED;
                w -> child[is_left ^ 1] -> color = RBT_BLACK;
                redblacktree_rotate(tree, w, is_left);
                w = p -> child[is_left];
            }
            w -> color = p -> color;
            p -> color = RBT_BLACK;
            w -> child[is_left] -> color = RBT_BLACK;
            redblacktree_rotate(tree, w -> p, is_left ^ 1);
            x = tree -> root;
        }
    }
    x -> color = RBT_BLACK;
}

void redblacktree_delete(struct RedBlackTree* tree, void* key) {
    struct RedBlackTreeNode* z = tree -> root;
    struct RedBlackTreeNode* w = tree -> nil;
    struct RedBlackTreeNode* y;
    struct RedBlackTreeNode* x;
    struct RedBlackTreeNode* delta;
    enum RedBlackTreeColor old_color;
    while (z != tree -> nil) {
        w = z;
        w -> size -= 1;
        if (tree -> compare(key, z -> key) == 0) {
            break;
        }
        z = z -> child[tree -> compare(z -> key, key) < 0 ? 1 : 0];
    }
    if (z != tree -> nil) {
        if (z -> cnt > 1) {
            z -> cnt -= 1;
            return;
        }
        y = z;
        old_color = y -> color;
        if (z -> child[0] == tree -> nil) {
            x = z -> child[1];
            redblacktree_transplant(tree, z, z -> child[1]);
        } else if (z -> child[1] == tree -> nil) {
            x = z -> child[0];
            redblacktree_transplant(tree, z, z -> child[0]);
        } else {
            y = redblacktree_minimum(*tree, z -> child[1]);
            old_color = y -> color;
            x = y -> child[1];
            if (y -> p == z) {
                x -> p = y;
            } else {
                delta = y;
                while (delta != z) {
                    delta -> size -= y -> cnt;
                    delta = delta -> p;
                }
                redblacktree_transplant(tree, y, y -> child[1]);
                y -> child[1] = z -> child[1];
                y -> child[1] -> p = y;
            }
            redblacktree_transplant(tree, z, y);
            y -> child[0] = z -> child[0];
            y -> child[0] -> p = y;
            y -> color = z -> color;
            y -> size = y -> child[0] -> size +
                        y -> child[1] -> size + y -> cnt;
        }
        if (old_color == RBT_BLACK) {
            redblacktree_delete_fixup(tree, x);
        }
        redblacktree_node_deinit(z);
    } else {
        while (w != tree -> nil) {
            w -> size += 1;
            w = w -> p;
        }
    }
}

struct RedBlackTreeNode* redblacktree_search(struct RedBlackTree tree,
                                             void* key) {
    struct RedBlackTreeNode* x = tree.root;
    while (x != tree.nil) {
        if (tree.compare(x -> key, key) == 0) {
            return x;
        }
        x = x -> child[tree.compare(x -> key, key) < 0 ? 1 : 0];
    }
    return tree.nil;
}

struct RedBlackTreeNode* redblacktree_maximum(struct RedBlackTree tree,
                                              struct RedBlackTreeNode* node) {
    while (node -> child[1] != tree.nil) {
        node = node -> child[1];
    }
    return node;
}

void* redblacktree_successor(struct RedBlackTree tree,
                             struct RedBlackTreeNode* x,
                             void* key) {
    if (x == tree.nil) {
        return tree.MAX;
    } else if (tree.compare(x -> key, key) <= 0) {
        return redblacktree_successor(tree, x -> child[1], key);
    } else {
        return generic_min(redblacktree_successor(tree, x -> child[0], key),
                           x -> key, tree.compare);
    }
}


void* redblacktree_predecessor(struct RedBlackTree tree,
                               struct RedBlackTreeNode* x,
                               void* key) {
    if (x == tree.nil) {
        return tree.MIN;
    } else if (tree.compare(x -> key, key) >= 0) {
        return redblacktree_predecessor(tree, x -> child[0], key);
    } else {
        return generic_max(redblacktree_predecessor(tree, x -> child[1], key),
                           x -> key, tree.compare);
    }
}

void* redblacktree_select(struct RedBlackTree tree, int i) {
    struct RedBlackTreeNode* x = tree.root;
    while (x != tree.nil) {
        if (x -> child[0] -> size + 1 <= i &&
            x -> child[0] -> size + x -> cnt >= i) {
            return x -> key;
        } else {
            if (x -> child[0] -> size + x -> cnt < i) {
                i -= x -> child[0] -> size + x -> cnt;
                x = x -> child[1];
            } else {
                x = x -> child[0];
            }
        }
    }
    return NULL;
}

int redblacktree_rank(struct RedBlackTree tree, void* key) {
    struct RedBlackTreeNode* x = tree.root;
    int rank = 0;

    while (x != tree.nil) {
        if (tree.compare(x -> key, key) < 0) {
            rank += x -> child[0] -> size + x -> cnt;
            x = x -> child[1];
        } else {
            x = x -> child[0];
        }
    }
    return rank;
}



int cmp(void* lhs, void* rhs) {
    return (*(int*)lhs) - (*(int*)rhs);
}

void
redblacktree_test_inorder_tree_traversal(struct RedBlackTree tree,
                                         struct RedBlackTreeNode* node) {
    if (node != tree.nil) {
        redblacktree_test_inorder_tree_traversal(tree, node -> child[0]);
        printf("(%d|%d|%c) ", *(int*)(node -> key),
                              node -> size,
                              node -> color == RBT_RED ? 'R' : 'B');
        redblacktree_test_inorder_tree_traversal(tree, node -> child[1]);
    }
}

char *buf, *p;
off_t size;
void io_init(void) {
    int fd = fileno(stdin);
    struct stat sb;
    fstat(fd, &sb);
    size = sb.st_size;
    buf = mmap(0, size, PROT_READ, MAP_PRIVATE, fileno(stdin), 0);
    p = buf;
}
char io_next_char(void) {
    return (p == buf + size || *p == -1) ? -1 : *p++;
}

void io_read_int(int* x) {
    bool iosig;
    static char c;
    for (iosig = false, c = io_next_char(); !isdigit(c); c = io_next_char()) {
        if (c == '-') iosig = true;
        if (c == -1) return;
    }
    for (*x = 0; isdigit(c); c = io_next_char())
        *x = ((*x + (*x << 2)) << 1) + (c ^ '0');
    if (iosig) *x = -(*x);
}

const int OUT_LEN = 10000000;
char obuf[OUT_LEN], *oh = obuf;

void print(char c) {
    if (oh == obuf + OUT_LEN) {
        fwrite(obuf, 1, OUT_LEN, stdout);
        oh = obuf;
    }
    *oh++ = c;
}

void print_int(int x) {
    static int buf[30], cnt;
    if (x == 0) {
        print('0');
    } else {
        if (x < 0) {
            print('-');x = -x;
        }
        for (cnt = 0; x; x /= 10) buf[++cnt] = x % 10 + 48;
        while (cnt) print((char)buf[cnt--]);
    }
}

void io_flush(void) {
    fwrite(obuf, 1, oh - obuf, stdout);
}

int main() {

    io_init();
    /* Prevent overflow */
    int __ = 10000000 + 1;
    int ___ = -10000000 - 1;
    struct RedBlackTree tree;
//    printf("%d\n", ___);
    redblacktree_init(&tree, cmp, &__, &___);

    int n = 0;
//    scanf("%d", &n);
    io_read_int(&n);

    int opt = 0;
    int opd = 0;
//    int A[100000];
//    int len = 0;

//    int AA[] = {1, 5, 4, 3, 5, 1, 1, 3, 3, 3, 1, 5, 1, 2, 2, 6, 1, 1, 2, 1};
//    int BB[] = {106465, 1, 317721, 460929, 644985, 84185, 89851, 81968, 492737, 493598};
    // for (int _ = 0; _ < n; _ += 1) {
        while (n--) {
//        scanf("%d %d", &opt, &opd);
        io_read_int(&opt);
        io_read_int(&opd);
//        opt = AA[_];
//        opd = BB[_];
        switch(opt){
        // if (opt == 1) {
        case 1:
            redblacktree_insert(&tree, &opd, sizeof(int));
            break;

        // } else if (opt == 2) {
case 2:
            redblacktree_delete(&tree, &opd);
break;
        // } else if (opt == 3) {
            case 3:
            print_int(redblacktree_rank(tree, &opd) + 1);
            print('\n');
            break;
            // printf("%d\n", redblacktree_rank(tree, &opd) + 1);

        // } else if (opt == 4) {
            case 4:
            print_int(*(int*)(redblacktree_select(tree, opd)));
            print('\n');
            break;
            // printf("%d\n", *(int*)(redblacktree_select(tree, opd)));
        // } else if (opt == 5) {
            case 5:
            print_int(*(int*)(redblacktree_predecessor(tree,
                                                            tree.root,
                                                            &opd)));
            print('\n');
            break;
            // printf("%d\n", *(int*)(redblacktree_predecessor(tree,
            //                                                 tree.root,
            //                                                 &opd)));
            case 6:
        // } else if (opt == 6) {
            print_int(*(int*)(redblacktree_successor(tree,
                                                          tree.root,
                                                          &opd)));
            print('\n');  
            break;
            // printf("%d\n", *(int*)(redblacktree_successor(tree,
                                                        //   tree.root,
                                                        //   &opd)));

        // }
        }
    }
    io_flush();
//    redblacktree_test_inorder_tree_traversal(tree, tree.root);

    return 0;
}
