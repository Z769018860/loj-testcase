#include <stdlib.h>
#include <stdio.h>

typedef struct _queuenode{
    int val;
    int bn;
    struct _queuenode *next;
} QueueNode;

typedef struct _queue{
    QueueNode *head;
    QueueNode *tail;
    int sz;
} Queue;

Queue* create_queue(){
    Queue* tmp = malloc(sizeof(Queue));
    tmp->head = tmp->tail = NULL;
    tmp->sz = 0;
    return tmp;
}


void push(Queue* q, int val, int bn){
    if(q->head == NULL){
        q->head = q->tail = malloc(sizeof(QueueNode));
        q->head->val = val;
        q->head->next = NULL;
        q->head->bn = bn;
        q->sz++;
    } else {
        q->tail->next = malloc(sizeof(QueueNode));
        q->tail = q->tail->next;
        q->tail->val = val;
        q->tail->bn = bn;
        q->tail->next = NULL;
        q->sz++;
    }
}

void TLE(){
    TLE();
}

void pop(Queue* q){
    if(q->head == NULL) {
        TLE();
    } else {
        QueueNode* tmp = q->head;
        q->head = q->head->next;
        q->sz--;
        free(tmp);
    }
}

typedef struct _listnode{
    int val;
    int flow;
    struct _listnode* rev;
    struct _listnode* next;
} ListNode;

typedef struct _list{
    ListNode *head;
    ListNode *tail;
} List;

ListNode* new_node(int val, int flow){
    ListNode *tmp = (ListNode*) malloc(sizeof(ListNode));
    tmp->val = val;
    tmp->flow = flow;
    tmp->next = NULL;
    return tmp;
}

void init(List* ll){
    ll->head = ll->tail = NULL;
}

ListNode* push_back(List* ll, int val, int flow){
    if(ll->tail){
        ll->tail->next = new_node(val, flow);
        ll->tail = ll->tail->next;
    } else {
        ll->head = ll->tail = new_node(val, flow);
    }
    return ll->tail;
}

void add_edge(List adj_list[], int u, int v, int cap){
    ListNode* a = push_back(&adj_list[u], v, cap);
    ListNode* b = push_back(&adj_list[v], u, 0);
    a->rev = b;
    b->rev = a;
}

int min(int a, int b){
    return (a<b)?a:b;
}

int FFS(int node, int bn, int sink, List adj[], int visited[] ){
    if(visited[node]) return 0;
    if(node==sink) return bn;
    visited[node]=1;
    ListNode* ptr = adj[node].head;
    while(ptr){
        if(ptr->flow == 0) {
            ptr = ptr->next;
            continue;
        }
        int res = FFS(ptr->val, min(bn, ptr->flow), sink, adj, visited);
        if(res>0){
            ptr->flow -= res;
            ptr->rev->flow += res;
            return res;
        }
        ptr = ptr->next;
    }
    return 0;
}

void print_flow(List adj_list[], int n){
    int i;
    for(i=0; i<=n; i++){
        //if(i != 26 && i != 13) continue;
        printf("for node i: %d\n", i);
        ListNode *ptr = adj_list[i].head;
        while(ptr){
            printf("- %d (flow: %d)\n", ptr->val, ptr->flow);
            ptr = ptr->next;
        }
    }
}

int EMK(int node, int bn, int sink, List adj[], int n){
    //printf("\n he\n");
    int i;
    int visited[n]; for(int i=0; i<n; i++) visited[i] = 0;
    int par[n]; for(i=0; i<n; i++) par[i] = -1;
    ListNode* par_edg[n]; for(int i=0; i<n; i++) par_edg[i] = NULL;
    Queue* q = create_queue();
    push(q,node,bn);

    int res = 0;
    while(q->sz > 0){
        int c_node, c_bn;
        c_node = q->head->val;
        c_bn = q->head->bn;
        //printf("==%d, %d\n", c_node, c_bn);
        pop(q);
        if(visited[c_node]) continue;
        visited[c_node] = 1;
        if(c_node == sink) {
            res = c_bn;
            break;
        }
        ListNode* ptr = adj[c_node].head;
        while(ptr){
            //printf("%d\n", ptr);
            if(ptr->flow <= 0 || visited[ptr->val]) {
                ptr = ptr->next;
                continue;
            }
            //if(ptr->val == 13  && c_node == 26)printf("flow = %d", ptr->flow);
            if(par[ptr->val] == -1){
                //printf("going from %d\tto\t%d = %d\n", c_node, ptr->val, ptr->flow);
                push(q, ptr->val, min(c_bn, ptr->flow));
                par[ptr->val] = c_node;
                par_edg[ptr->val] = ptr;
            }
            ptr = ptr->next;
        }
        
    }
    //printf("res: %d\t", res);
    if(visited[sink]){
        int ptr = sink;
        while(par[ptr] != -1){
            //printf("%d ", ptr);
            //if(par_edg[ptr]==NULL) printf("hehe");
            par_edg[ptr]->rev->flow += res;
            par_edg[ptr]->flow -= res;
            ptr = par[ptr];
        }
    }

    return res;
    
}

int main(){
    int i,j;
    int n; scanf("%d", &n);
    List adj_list[n];
    int s,t,c; scanf("%d %d %d",  &c, &s, &t);
    s--; t--;
    for(i=0; i<n; i++) init(&adj_list[i]);
    for(i=0; i<c; i++){
        int u,v, cap; scanf("%d %d %d", &u, &v, &cap);
        if(u==14 && v==27 || u==27 && v ==14){
            //printf("found node %d %d: %d\n", u, v, cap);
        }
        add_edge(adj_list,u-1,v-1,cap);
    }
    int visited[n];
    //print_flow(adj_list, n-1);
    long long res;
    long long cnt = 0;
    do {
        res = EMK(s,2147483647,t,adj_list,n);
        cnt += (long long) res;
    } while (res > 0);

    ListNode* ptr = adj_list[0].head;

    printf("%lld", cnt);
}
