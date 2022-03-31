//
// Created by htomi on 3/28/2022.
//

#ifndef OPREND_BEAD_UTIL_H
#define OPREND_BEAD_UTIL_H

#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <stdlib.h>

struct nyuszi;

void init(struct nyuszi*, const char*, const char*, int);
void check_successful_alloc(const char*);
void write_to_file(const char*, struct nyuszi);
void check_file_pointer(FILE*);
void delete(struct nyuszi*);

#endif //OPREND_BEAD_UTIL_H
