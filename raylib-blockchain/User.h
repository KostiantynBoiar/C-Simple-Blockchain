#pragma once

#include <stdlib.h>
#include <string.h>
#include "base64.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
    char* firstName;
    char* secondName;
    char* middleName;
    char* groupName;
    char* course;
    char* faculty;
    char* universityName;
    char nonce[65];
} User;

void generate_random_nonce(char* nonce, size_t length);
const char* user_to_string(User u);
char* solution(User u);
void initialize_user(User* u, const char* first, const char* second, const char* middle, const char* group, const char* course, const char* faculty, const char* university);
void free_user(User u);