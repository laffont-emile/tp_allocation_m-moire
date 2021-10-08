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
        if(! ptr[i]) fprintf(stderr, "Alloc échouée, %p\n", ptr[i]);
    }

    for(int i = 0; i < nbTest; i++){
        mem_free(ptr[i]);
    }
}

void test_1_1(){
    void *ptr[4096];

    for(int i = 0; i < 4096; i++){
        ptr[i] = mem_alloc(1);
        if(!ptr[i])fprintf(stderr, "Alloc échouée, %p\n", ptr[i]);
    }

    for(int i = 0; i < 4096; i++){
        mem_free(ptr[i]);
    }
}

void test_hi(){
    void *ptr;
    ptr = malloc(5);
    if(!ptr) fprintf(stderr, "Alloc échouée, %p\n", ptr);

    mem_free(ptr + 5000);
}

void test_uzero(){
    void *ptr;
    ptr = malloc(5);
    if(!ptr)  fprintf(stderr, "Alloc échouée, %p\n", ptr);

    mem_free(ptr - 5000);
}

int main(int argc, char *argv[]) {

    int nb = 500;
    printf("Test running");
    srand(time(NULL));
    
    mem_init();

    test_rand_blocs(nb);
    fprintf(stdout, "rand bloc OK\n");

    test_1_1();
    fprintf(stdout, "1_1 OK\n");


    test_uzero();
    fprintf(stdout, "under zero OK\n");

    test_hi();
    fprintf(stdout, "Hors interval OK\n");

    fprintf(stdout, "ALL TESTS OK\n");
    return 0;
}
