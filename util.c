//
// Created by htomi on 3/28/2022.
//
#include "util.h"

struct nyuszi {
    char* nev;
    char* lakoterulet;
    int hanyadik;
};

void init(struct nyuszi* nyul, const char* p_nev, const char* p_lakoterulet, int p_hanyadik) {

    size_t nev_hossz = strlen(p_nev);
    nyul->nev = (char*) malloc((nev_hossz) + 1 * sizeof(char));
    check_successful_alloc(nyul->nev);
    strcpy(nyul->nev, p_nev);

    nev_hossz = strlen(p_lakoterulet);
    nyul->lakoterulet = (char*) malloc((nev_hossz + 1) * sizeof(char));
    check_successful_alloc(nyul->lakoterulet);
    strcpy(nyul->lakoterulet, p_lakoterulet);

    nyul->hanyadik = p_hanyadik;
}

void check_successful_alloc(const char* allocated) {
    if (allocated == NULL) {
        printf("unsuccessful allocation");
        exit(1);
    }
}

void write_to_file(const char* path, struct nyuszi nyul) {
    FILE* fptr = fopen(path, "a");
    check_file_pointer(fptr);

    fprintf(fptr, "%s %s %d", nyul.nev, nyul.lakoterulet, nyul.hanyadik);
    fclose(fptr);
}

void check_file_pointer(FILE* fptr) {
    if (fptr == NULL) {
        printf("unsuccessful file open");
    }
}

void delete(struct nyuszi* nyul) {
    free(nyul->nev);
    free(nyul->lakoterulet);
    nyul->nev = NULL;
    nyul->lakoterulet = NULL;
}