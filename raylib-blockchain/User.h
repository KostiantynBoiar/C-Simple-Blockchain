#pragma once

#include <stdlib.h>
#include <string.h>

#define NONCE_SIZE 64

typedef struct {
    char* firstName;
    char* secondName;
    char* middleName;
    char* groupName;
    char* course;
    char* faculty;
    char* universityName;
    char nonce[NONCE_SIZE];
    int thread_id;  // Thread ID to track which thread found the solution
} User;

void generate_random_nonce(char* nonce, size_t length);
const char* user_to_string(User u);
void initialize_user(User* u, const char* first, const char* second, const char* middle, const char* group, const char* course, const char* faculty, const char* university);
void free_user(User *u);
