#include "User.h"
#include "SHA256.h"

const char* user_to_string(User u) {
    // Adjust size based on the expected string length (including large nonce)
    char* result = (char*)malloc(2048);  // Increased from 1024 to 2048

    if (!result) {
        return NULL; // Handle memory allocation failure
    }

    snprintf(result, 2048,  // Adjusted size to 2048
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


#define NONCE_SIZE 64  

void generate_random_nonce(char* nonce, size_t length) {
    const char charset[] = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789";
    for (size_t i = 0; i < length - 1; i++) {
        int randomIndex = rand() % (sizeof(charset) - 1);
        nonce[i] = charset[randomIndex];
    }
    nonce[length - 1] = '\0';  // Null-terminate the nonce
}

char* solution(User u) {
    srand((unsigned int)time(NULL));

    char* user_string;
    BYTE hash[SHA256_BLOCK_SIZE];
    int target_zeroes = 1;
    int attempt = 0;

    while (target_zeroes <= 8) {
        generate_random_nonce(u.nonce, NONCE_SIZE);
        //printf("Nonce: %s\n", u.nonce);

        user_string = user_to_string(u);

        SHA256_CTX ctx;
        sha256_init(&ctx);
        sha256_update(&ctx, (BYTE*)user_string, strlen(user_string));
        sha256_final(&ctx, hash);

        free(user_string);

        int leading_zeroes = 0;
        for (int i = 0; i < target_zeroes; i++) {
            if (hash[i] == 0x0) {
                leading_zeroes++;
            }
            else {
                break;
            }
        }

        if (leading_zeroes == target_zeroes) {
            printf("Hash with %d of zeroes has been found.\n", target_zeroes);
            printf("SHA256 Hash: ");
            for (int i = 0; i < SHA256_BLOCK_SIZE; i++) {
                printf("%01x", hash[i]);
            }
            printf("\n");
            target_zeroes++;
        }
        else {
            attempt++;
        }
    }

    return "Program complete!";
}




// Function to initialize the user struct and allocate memory for its string fields
void initialize_user(User* u, const char* first, const char* second, const char* middle, const char* group, const char* course, const char* faculty, const char* university)
{
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

void free_user(User u)
{
    // Free dynamically allocated memory for each string
    free(u.firstName);
    free(u.secondName);
    free(u.middleName);
    free(u.groupName);
    free(u.course);
    free(u.faculty);
    free(u.universityName);

}

