#include <stdio.h>
#include <stdlib.h>

int main(){
    int *p;
    int i;

    p = (int *) malloc(sizeof(int[10]));
    for (i=0; i < 10; i++)
        p[1] = 0;
    free(p);
}
