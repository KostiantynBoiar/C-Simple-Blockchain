#include "User.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define NONCE_SIZE 64

// Function to generate a random nonce
void generate_random_nonce(char* nonce, size_t length) {
    const char charset[] = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789";
    for (size_t i = 0; i < length - 1; i++) {
        int randomIndex = rand() % (sizeof(charset) - 1);
        nonce[i] = charset[randomIndex];
    }
    nonce[length - 1] = '\0';  // Null-terminate the nonce
}

// Function to convert User to a string
const char* user_to_string(User u) {
    char* result = (char*)malloc(2048);  // Allocate memory for the result

    if (!result) {
        return NULL;  // Handle memory allocation failure
    }

    snprintf(result, 2048,
        "First Name: %s\n"
        "Second Name: %s\n"
        "Middle Name: %s\n"
        "Group Name: %s\n"
        "Course: %s\n"
        "Faculty: %s\n"
        "University Name: %s\n"
        "Nonce: %s\n",
        u.firstName ? u.firstName : "N/A",
        u.secondName ? u.secondName : "N/A",
        u.middleName ? u.middleName : "N/A",
        u.groupName ? u.groupName : "N/A",
        u.course ? u.course : "N/A",
        u.faculty ? u.faculty : "N/A",
        u.universityName ? u.universityName : "N/A",
        u.nonce);

    return result;
}

// Function to initialize the User struct and allocate memory for its string fields
void initialize_user(User* u, const char* first, const char* second, const char* middle, const char* group, const char* course, const char* faculty, const char* university) {
    u->firstName = (char*)malloc(strlen(first) + 1); 
    strcpy(u->firstName, first);

    u->secondName = (char*)malloc(strlen(second) + 1);
    strcpy(u->secondName, second);

    u->middleName = (char*)malloc(strlen(middle) + 1);
    strcpy(u->middleName, middle);

    u->groupName = (char*)malloc(strlen(group) + 1);
    strcpy(u->groupName, group);

    u->course = (char*)malloc(strlen(course) + 1);
    strcpy(u->course, course);

    u->faculty = (char*)malloc(strlen(faculty) + 1);
    strcpy(u->faculty, faculty);

    u->universityName = (char*)malloc(strlen(university) + 1);
    strcpy(u->universityName, university);

    memset(u->nonce, '1', sizeof(u->nonce));
}

// Function to free the dynamically allocated memory in the User struct
void free_user(User u) {
    free(u.firstName);
    free(u.secondName);
    free(u.middleName);
    free(u.groupName);
    free(u.course);
    free(u.faculty);
    free(u.universityName);
}
