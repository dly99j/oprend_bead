//
// Created by htomi on 3/28/2022.
//
#include "util.h"
#include <stdio.h>
#include <stdlib.h>

struct nyuszi {
    char* nev;
    char* lakoterulet;
    int hanyadik;
};

struct nyuszi* init(const char* p_nev, const char* p_lakoterulet, int p_hanyadik) {
    struct nyuszi* nyul = (struct nyuszi*)malloc(sizeof(struct nyuszi*));
    size_t nev_hossz = strlen(p_nev);
    nyul->nev = (char*)malloc((nev_hossz)+1 * sizeof(char));
    check_successful_alloc(nyul->nev);
    strcpy(nyul->nev, p_nev);

    nev_hossz = strlen(p_lakoterulet);
    nyul->lakoterulet = (char*)malloc((nev_hossz + 1) * sizeof(char));
    check_successful_alloc(nyul->lakoterulet);
    strcpy(nyul->lakoterulet, p_lakoterulet);

    nyul->hanyadik = p_hanyadik;

    return nyul;
}

void check_successful_alloc(const char* allocated) {
    if (allocated == NULL) {
        fprintf(stderr, "unsuccessful allocation\n");
        exit(1);
    }
}

void append_to_file(const char* path, struct nyuszi* nyul) {
    FILE* fptr = fopen(path, "a+");
    check_file_pointer(fptr);

    fprintf(fptr, "%s %s %d\n", nyul->nev, nyul->lakoterulet, nyul->hanyadik);
    fflush(fptr);
    fclose(fptr);
}

void check_file_pointer(FILE* fptr) {
    if (fptr == NULL) {
        fprintf(stderr, "unsuccessful file open\n");
        exit(1);
    }
}

void check_null_char_ptr(const char* str) {
    if (str == NULL) {
        fprintf(stderr, "character literal is null\n");
        exit(1);
    }
}

void delete_nyuszi(struct nyuszi* nyul) {
    free(nyul->nev);
    free(nyul->lakoterulet);
    nyul->nev = NULL;
    nyul->lakoterulet = NULL;
    free(nyul);
    nyul = NULL;
}

struct nyuszi* get_nyuszi_from_user() {
    struct nyuszi* new_nyuszi;
    char nev[BUFSIZE], lakoterulet[BUFSIZE];
    int hanyadik;

    fflush(stdin);
    //getchar();
    puts("Kerem adja meg a nevet:");
    fgets(nev, BUFSIZE, stdin);

    fflush(stdin);
    puts("Kerem adja meg a lakoteruletet:");
    fgets(lakoterulet, BUFSIZE, stdin);

    nev[strlen(nev) - 1] = '\0';
    lakoterulet[strlen(lakoterulet) - 1] = '\0';

    while (!check_lakoterulet(lakoterulet)) {
        puts("nincs ilyen lakoterulet! \nkerem adja meg a lakoteruletet:");
        fflush(stdin);
        fgets(lakoterulet, BUFSIZE, stdin);
        lakoterulet[strlen(lakoterulet) - 1] = '\0';
    }

    puts("kerem adja meg a jelentkezes szamat:");

    while (scanf( "%d", &hanyadik) == 0) {
        puts("hibas formatum");
    }

    new_nyuszi = init(nev, lakoterulet, hanyadik);

    return new_nyuszi;
}


int check_lakoterulet(const char* lakoterulet) {
    check_null_char_ptr(lakoterulet);
    if (strcmp(lakoterulet, "Baratfa") == 0 ||
        strcmp(lakoterulet, "Lovas") == 0 ||
        strcmp(lakoterulet, "Szula") == 0 ||
        strcmp(lakoterulet, "Kigyos-patak") == 0 ||
        strcmp(lakoterulet, "Malom telek") == 0 ||
        strcmp(lakoterulet, "Paskom") == 0 ||
        strcmp(lakoterulet, "Kaposztas kert") == 0) {
        return 1;
    }
    return 0;
}

void list_nyuszi(const char* path, const char* place) {
    check_null_char_ptr(path);

    FILE* fptr = fopen(path, "r");
    check_file_pointer(fptr);

    if (place != NULL && !check_lakoterulet(place)) {
        puts("nincs ilyen lakoterulet");
        return;
    }

    char line[BUFSIZE];
    while (fgets(line, BUFSIZE, fptr) != NULL) {
        if (place != NULL && strstr(line, place) == NULL) {
            continue;
        }
        printf("%s", line);
    }

    fclose(fptr);
}

void copy_back_temp(const char* path) {
    check_null_char_ptr(path);

    FILE* fptr = fopen(path, "w+");
    FILE* temp_fptr = fopen("temp.txt", "r");

    check_file_pointer(fptr);
    check_file_pointer(temp_fptr);

    char ch;
    while ((ch = fgetc(temp_fptr)) != EOF)
        fputc(ch, fptr);

    fflush(fptr);
    fclose(fptr);
    fclose(temp_fptr);
    remove("temp.txt");
}

void modify_entry_in_file(const char* path, const char* name) {
    check_null_char_ptr(path);
    check_null_char_ptr(name);

    FILE* fptr = fopen(path, "r");
    FILE* temp_fptr = fopen("temp.txt", "w+");

    check_file_pointer(fptr);
    check_file_pointer(temp_fptr);

    char line[BUFSIZE];
    struct nyuszi* modified = NULL;
    bool found = false;
    while (fgets(line, BUFSIZE, fptr) != NULL) {
        if (strstr(line, name) != NULL) {
            puts("kerem adja meg a modositott adatokat:");
            modified = get_nyuszi_from_user();
            fprintf(temp_fptr, "%s %s %d\n", modified->nev, modified->lakoterulet, modified->hanyadik);
            delete_nyuszi(modified);
            found = true;
        } else {
            fprintf(temp_fptr, "%s", line);
        }
    }

    if (!found) {
        puts("nyuszi nem talalhato az adatbazisban");
    }

    fflush(temp_fptr);
    fclose(fptr);
    fclose(temp_fptr);

    copy_back_temp(path);
}

void delete_from_file(const char* path, const char* name) {
    check_null_char_ptr(path);
    check_null_char_ptr(name);

    FILE* fptr = fopen(path, "r");
    FILE* temp_fptr = fopen("temp.txt", "w+");

    check_file_pointer(fptr);
    check_file_pointer(temp_fptr);

    char line[BUFSIZE];
    bool is_in_file = false;
    while (fgets(line, BUFSIZE, fptr) != NULL) {
        if (strstr(line, name) != NULL) {
            is_in_file = true;
        } else {
            fprintf(temp_fptr, "%s", line);
        }
    }

    if (!is_in_file) {
        puts("nyuszi nem talalhato az adatbazisban");
    }

    fflush(temp_fptr);
    fclose(fptr);
    fclose(temp_fptr);

    copy_back_temp(path);
}

char* get_name() {
    char* nev = (char*)malloc(BUFSIZE * sizeof(char));
    puts("Kerem adja meg a nevet:");
    fflush(stdin);
    getchar();
    fgets(nev, BUFSIZE, stdin);
    nev[strlen(nev) - 1] = '\0';
    return nev;
}

char* get_place() {
    char* lakoterulet = (char*)malloc(BUFSIZE * sizeof(char));

    fflush(stdin);
    puts("Kerem adja meg a lakoteruletet:");
    fgets(lakoterulet, BUFSIZE, stdin);
    lakoterulet[strlen(lakoterulet) - 1] = '\0';

    while (!check_lakoterulet(lakoterulet)) {
        puts("nincs ilyen lakoterulet! \nkerem adja meg a lakoteruletet:");
        fflush(stdin);
        fgets(lakoterulet, BUFSIZE, stdin);
        lakoterulet[strlen(lakoterulet) - 1] = '\0';
    }

    return lakoterulet;
}

void clear_database(const char* path) {
    remove(path);
}

void menu_goodbye() {
    puts("Viszont latasra!");
}

void menu_wrong_number() {
    puts("Nincs ilyen menupont!");
}

void menu_add(const char* path) {
    struct nyuszi* new_nyuszi = get_nyuszi_from_user();
    append_to_file(path, new_nyuszi);
    delete_nyuszi(new_nyuszi);
}

void menu_modify(const char* path) {
    char* nev = get_name();
    modify_entry_in_file(path, nev);
    free(nev);
}

void menu_delete(const char* path) {
    char* nev = get_name();
    delete_from_file(path, nev);
    free(nev);
}

void menu_list(const char* path) {
    list_nyuszi(path, NULL);
}

void menu_list_specific_place(const char* path) {
    char* lakoterulet = get_place();
    list_nyuszi(path, lakoterulet);
    free(lakoterulet);
}

void menu_delete_database(const char* path) {
    remove(path);
}

void print_menu() {
    puts("-----------------------------------------------------");
    puts("-----------Husveti locsolokiraly valasztas-----------");
    puts("-----------------------------------------------------");
    puts("Kerem adja meg, mit szeretne tenni ------------------");
    puts("0 - kilepes -----------------------------------------");
    puts("1 - locsolo hozzaadasa ------------------------------");
    puts("2 - locsolo modositasa ------------------------------");
    puts("3 - locsolo torlese ---------------------------------");
    puts("4 - listazas ----------------------------------------");
    puts("5 - listazas teruleti alapon ------------------------");
    puts("6 - adatbazis torlese -------------------------------");
    puts("-----------------------------------------------------");
}

int get_menu_point() {
    print_menu();

    int menu_point;
    while (fscanf(stdin, "%d", &menu_point) != 1) {
        puts("hibas formatum");
    }

    return menu_point;
}

void run(const char* path) {

    int menu_point = get_menu_point();

    while (menu_point) {
        switch (menu_point) {
            case 0:
                menu_goodbye();
                break;
            case 1:
                menu_add(path);
                break;
            case 2:
                menu_modify(path);
                break;
            case 3:
                menu_delete(path);
                break;
            case 4:
                menu_list(path);
                break;
            case 5:
                menu_list_specific_place(path);
                break;
            case 6:
                menu_delete_database(path);
                break;
            default:
                menu_wrong_number();
                break;
        }

        menu_point = get_menu_point();
    }
}