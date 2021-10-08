#include "common.h"
#include "mem.h"
#include "mem_os.h"
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>

void test_rand_blocs(int nb){
    int nbTest = nb;
    int blocs = round(rand() % 101);
    void *ptr[nbTest];

    for(int i = 0; i < nbTest; i++){
        blocs = rand() % 1001;
        blocs = round(blocs);
        ptr[i] = mem_alloc(blocs);
        if(ptr[i]) fprintf(stdout, "Allocation passée, bloc retourné %p\n", ptr[i]);
        else fprintf(stderr, "Alloc échouée, %p\n", ptr[i]);
    }

    for(int i = 0; i < nbTest; i++){
        mem_free(ptr[i]);
    }
}

void test_1_1(){
    void *ptr[4096];

    for(int i = 0; i < 4096; i++){
        ptr[i] = mem_alloc(1);
        if(ptr[i]) fprintf(stdout, "Allocation passée, bloc retourné %p\n", ptr[i]);
        else fprintf(stderr, "Alloc échouée, %p\n", ptr[i]);
    }

    for(int i = 0; i < 4096; i++){
        mem_free(ptr[i]);
    }
}

void test_hi(){
    void *ptr;
    ptr = malloc(5);
    if(ptr) fprintf(stdout, "Allocation passée, bloc retourné %p\n", ptr);
        else fprintf(stderr, "Alloc échouée, %p\n", ptr);

    mem_free(ptr + 5000);
}

void test_uzero(){
    void *ptr;
    ptr = malloc(5);
    if(ptr) fprintf(stdout, "Allocation passée, bloc retourné %p\n", ptr);
        else fprintf(stderr, "Alloc échouée, %p\n", ptr);

    mem_free(ptr - 5000);
}

int main(int argc, char *argv[]) {
    if(argc < 2){
        printf("Param missed: <prog_name> <test_nbr>");
        return -1;
    }

    int nb = atoi(argv[1]);
    printf("Test running");
    srand(time(NULL));
    
    mem_init();

    test_rand_blocs(nb);
    fprintf(stdout, "rand bloc OK\n");
    getchar();

    test_1_1();
    fprintf(stdout, "1_1 OK\n");
    getchar();

    test_uzero();
    fprintf(stdout, "under zero OK\n");
    getchar();

    test_hi();
    fprintf(stdout, "Hors interval OK\n");
    getchar();

    fprintf(stdout, "ALL TESTS OK\n");
    return 0;
}
