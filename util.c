//
// Created by htomi on 3/28/2022.
//
#include "util.h"

struct nyuszi {
    char* nev;
    char* lakoterulet;
    int hanyadik;
};

struct region_data {
    char** nyuszik;
    int count;
    int region;
};

struct nyuszi* init(const char* p_nev, const char* p_lakoterulet, int p_hanyadik) {
    struct nyuszi* nyul = (struct nyuszi*)malloc(sizeof(struct nyuszi));
    size_t nev_hossz = strlen(p_nev);
    nyul->nev = (char*)malloc((nev_hossz + 1) * sizeof(char));
    check_successful_alloc(nyul->nev);
    strcpy(nyul->nev, p_nev);

    nev_hossz = strlen(p_lakoterulet);
    nyul->lakoterulet = (char*)malloc(BUFSIZE * sizeof(char));
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

void check_successful_alloc2(const char** allocated) {
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
    char nev[BUFSIZE], lakoterulet[BUFSIZE], hanyadik_str[BUFSIZE];
    int hanyadik;

    fflush(stdin);
    //getchar();
    puts("Kerem adja meg a nevet:");
    fgets(nev, BUFSIZE, stdin);
    nev[strlen(nev) - 1] = '\0';

    fflush(stdin);
    puts("Kerem adja meg a lakoteruletet:");
    fgets(lakoterulet, BUFSIZE, stdin);

    lakoterulet[strlen(lakoterulet) - 1] = '\0';

    while (!check_lakoterulet(lakoterulet, 0)) {
        puts("nincs ilyen lakoterulet! \nkerem adja meg a lakoteruletet:");
        fflush(stdin);
        fgets(lakoterulet, BUFSIZE, stdin);
        lakoterulet[strlen(lakoterulet) - 1] = '\0';
    }

    puts("kerem adja meg a jelentkezes szamat:");

    fgets(hanyadik_str, BUFSIZE, stdin);
    hanyadik_str[strlen(hanyadik_str) - 1] = '\n';

    while ((hanyadik = atoi(hanyadik_str)) == 0) {
        puts("hibas bemenet \nkerem adja meg a jelentkezes szamat:");
        fgets(hanyadik_str, BUFSIZE, stdin);
        hanyadik_str[strlen(hanyadik_str) - 1] = '\n';
    }

    new_nyuszi = init(nev, lakoterulet, hanyadik);

    return new_nyuszi;
}

// region == 0 --- all
// region == 1 --- first region
// region == 2 --- second region
int check_lakoterulet(const char* lakoterulet, int region) {
    check_null_char_ptr(lakoterulet);
    if (region == 0 &&
        strcmp(lakoterulet, "Baratfa") == 0 ||
        strcmp(lakoterulet, "Lovas") == 0 ||
        strcmp(lakoterulet, "Szula") == 0 ||
        strcmp(lakoterulet, "Kigyos-patak") == 0 ||
        strcmp(lakoterulet, "Malom telek") == 0 ||
        strcmp(lakoterulet, "Paskom") == 0 ||
        strcmp(lakoterulet, "Kaposztas kert") == 0) {
        return 1;
    } else if (region == 1 &&
               strcmp(lakoterulet, "Baratfa") == 0 ||
               strcmp(lakoterulet, "Lovas") == 0 ||
               strcmp(lakoterulet, "Kigyos-patak") == 0 ||
               strcmp(lakoterulet, "Kaposztas kert") == 0) {
        return 1;
    } else if (region == 2 &&
               strcmp(lakoterulet, "Szula") == 0 ||
               strcmp(lakoterulet, "Malom telek") == 0 ||
               strcmp(lakoterulet, "Paskom") == 0) {
        return 1;
    }
    return 0;
}

int count_nyuszi(const char* path, int region) {
    check_null_char_ptr(path);

    FILE* fptr = fopen(path, "r");

    int count = 0;
    char line[BUFSIZE];

    if (fptr == NULL){
        puts("file nem letezik vagy valami hiba lepett fel a megnyitasa kozben");
        return -1;
    }

    while (fgets(line, BUFSIZE, fptr) != NULL) {
        if (region == 1 &&
            strstr(line, "Baratfa") == NULL &&
            strstr(line, "Lovas") == NULL &&
            strstr(line, "Kigyos-patak") == NULL &&
            strstr(line, "Kaposztas kert") == NULL) {
            continue;
        } else if (region == 2 &&
                 strstr(line, "Szula") == NULL &&
                 strstr(line, "Malom telek") == NULL &&
                 strstr(line, "Paskom") == NULL) {
            continue;
        }
        ++count;
    }

    fclose(fptr);

    return count;
}

char** read_all_nyuszi(const char* path, int region) {

    check_null_char_ptr(path);

    FILE* fptr = fopen(path, "r");

    int count = count_nyuszi(path, region);
    int curr_nyuszi = 0;

    char** ret_line = (char**)malloc(count * sizeof(char*));
    check_successful_alloc2((const char **) ret_line);

    for (int i = 0; i < count; ++i) {
        ret_line[i] = (char*)malloc(BUFSIZE * sizeof(char));
        check_successful_alloc(ret_line[i]);
    }

    if (fptr == NULL){
        puts("file nem letezik vagy valami hiba lepett fel a megnyitasa kozben");
        return NULL;
    }

    char line[BUFSIZE];
    while (fgets(line, BUFSIZE, fptr) != NULL) {
        if (region == 1 &&
            strstr(line, "Baratfa") == NULL &&
            strstr(line, "Lovas") == NULL &&
            strstr(line, "Kigyos-patak") == NULL &&
            strstr(line, "Kaposztas kert") == NULL) {
            continue;
        } else if (region == 2 &&
                   strstr(line, "Szula") == NULL &&
                   strstr(line, "Malom telek") == NULL &&
                   strstr(line, "Paskom") == NULL) {
            continue;
        }
        strcpy(ret_line[curr_nyuszi], line);
        ++curr_nyuszi;
    }

    fclose(fptr);

    return ret_line;
}

void list_nyuszi(const char* path, const char* place) {
    check_null_char_ptr(path);

    FILE* fptr = fopen(path, "r");
    //check_file_pointer(fptr);

    if (fptr == NULL){
        puts("file nem letezik vagy valami hiba lepett fel a megnyitasa kozben");
        return;
    }

    if (place != NULL && !check_lakoterulet(place, 0)) {
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

    while (!check_lakoterulet(lakoterulet, 0)) {
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

struct region_data* get_region_data(const char * path, int region) {
    struct region_data* ret_val = (struct region_data*) malloc(sizeof(struct region_data));
    int count = count_nyuszi(path, region);
    char** data = read_all_nyuszi(path, region);

    ret_val->region = region;
    ret_val->count = count;
    ret_val->nyuszik = data;

    return ret_val;
}

void delete_region_data(struct region_data* data) {

    for (int i = 0; i < data->count; ++i) {
        free(data->nyuszik[i]);
    }
    free(data->nyuszik);
    free(data);
    data = NULL;
}

void print_winner(char* data, int placement) {

    data[strlen(data) - 1] = '\0';
    printf("%s, %d tojassal\n", data, placement);
}

void run_easter(const char* path) {

    struct region_data* data1 = get_region_data(path, 1);
    struct region_data* data2 = get_region_data(path, 2);
    int fd1[2], fd2[2];
    pid_t pid;
    int eggs1[data1->count], eggs2[data2->count];

    for (int i = 0; i < 2; ++i) {

        if (pipe(fd1) == -1) {
            perror("pipe 1 fail");
            return;
        }
        if (pipe(fd2) == -1) {
            perror("pipe 2 fail");
            return;
        }
        if ((pid = fork()) < 0) {
            perror("fork fail");
            return;
        }

        if (0 < pid) {

            srand(pid);
            close(fd1[0]);

            if (i == 0) {
                write(fd1[1], data1, sizeof(struct region_data*));
            } else {
                write(fd1[1], data2, sizeof(struct region_data*));
            }

            wait(NULL);
            close(fd2[1]);

            if (i == 0) {
                for (int j = 0; j < data1->count; ++j) {
                    read(fd2[0], eggs1, data1->count * sizeof(int));
                }
            } else {
                for (int j = 0; j < data2->count; ++j) {
                    read(fd2[0], eggs2, data2->count * sizeof(int));
                }
            }

            close(fd2[0]);
        } else {

            close(fd1[1]);
            struct region_data* data;
            read(fd1[0], &data, sizeof(struct region_data*));

            close(fd1[0]);
            close(fd2[0]);

            int eggs[data->count];

            for (int j = 0; j < data->count; ++j) {
                int num = rand() % 100 + 1;
                eggs[j] = num;
            }

            write(fd2[1], eggs, data->count * sizeof(int));
            close(fd2[1]);

            exit(0);
        }
    }

    int max = 0, maxind = -1;
    bool first = true;

    for (int i = 0; i < data1->count; ++i) {
        if (max < eggs1[i]) {
            max = eggs1[i];
            maxind = i;
            first = true;
        }
    }

    for (int i = 0; i < data2->count; ++i) {
        if (max < eggs2[i]) {
            max = eggs2[i];
            maxind = i;
            first = false;
        }
    }

    puts("Locsolokiraly:");

    if (first) {
        print_winner(data1->nyuszik[maxind], max);
    } else {
        print_winner(data2->nyuszik[maxind], max);
    }

    delete_region_data(data1);
    delete_region_data(data2);
}

void menu_wrong_format() {
    puts("hibas formatum");
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

void menu_run_easter(const char* path) {
    run_easter(path);
}

void print_menu() {
    puts("-----------------------------------------------------");
    puts("-----------Husveti locsolokiraly valasztas-----------");
    puts("-----------------------------------------------------");
    puts("Kerem adja meg, mit szeretne tenni ------------------");
    puts("1 - kilepes -----------------------------------------");
    puts("2 - locsolo hozzaadasa ------------------------------");
    puts("3 - locsolo modositasa ------------------------------");
    puts("4 - locsolo torlese ---------------------------------");
    puts("5 - listazas ----------------------------------------");
    puts("6 - listazas teruleti alapon ------------------------");
    puts("7 - adatbazis torlese -------------------------------");
    puts("8 - locsoloverseny inditasa -------------------------");
    puts("-----------------------------------------------------");
}

int get_menu_point() {
    print_menu();

    int menu_point;

    char input[BUFSIZE];
    fgets(input, BUFSIZE, stdin);
    input[strlen(input) - 1] = '\0';

    if ((menu_point = atoi(input)) == 0) {
        puts("hibas bemenet");
        fgets(input, BUFSIZE, stdin);
        input[strlen(input) - 1] = '\0';
    }

    return menu_point;
}

void run(const char* path) {

    int menu_point = get_menu_point();

    while (menu_point != 1) {
        switch (menu_point) {
            case 1:
                menu_goodbye();
                break;
            case 2:
                menu_add(path);
                break;
            case 3:
                menu_modify(path);
                break;
            case 4:
                menu_delete(path);
                break;
            case 5:
                menu_list(path);
                break;
            case 6:
                menu_list_specific_place(path);
                break;
            case 7:
                menu_delete_database(path);
                break;
            case 8:
                menu_run_easter(path);
                break;
            default:
                menu_wrong_number();
                break;
        }

        menu_point = get_menu_point();
    }
}