#include "kernel/types.h"
#include "kernel/ptree.h"
#include "user/user.h"

#define MAX 64

void print_tree(struct ptreeinfo *buf, int n, int parent, int level){
    for(int i = 0; i < n; i++){
        if(buf[i].ppid == parent){

            // indent
            for(int j = 0; j < level; j++){
                printf("  ");
            }

            printf("%d %s state=%d mem=%lu\n",
                buf[i].pid,
                buf[i].name,
                buf[i].state,
                buf[i].memsize);

            // recursion
            print_tree(buf, n, buf[i].pid, level + 1);
        }
    }
}

int main(){
    struct ptreeinfo buf[MAX];

    int n = ptree(buf, MAX);

    if(n < 0){
        printf("ptree failed\n");
        exit(1);
    }

    // in init trước
    for(int i = 0; i < n; i++){
        if(buf[i].pid == 1){

            printf("%d %s state=%d mem=%lu\n",
                buf[i].pid,
                buf[i].name,
                buf[i].state,
                buf[i].memsize);

            print_tree(buf, n, 1, 1);
            break;
        }
    }

    exit(0);
}