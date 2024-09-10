#include <stdio.h>
#include <pthread.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "User.h"
#include "SHA256.h"
#define NONCE_SIZE 64
#define NUM_THREADS 50

// Shared data
pthread_mutex_t mutex;
int current_target_zeroes = 1;
bool solution_found = false;

void* thread_solution(void* arg) {
    User* u = (User*)arg;

    char* user_string;
    BYTE hash[SHA256_BLOCK_SIZE];

    while (current_target_zeroes <= 8) {
        pthread_mutex_lock(&mutex);
        if (solution_found) {
            // Reset for the next target zeroes search
            current_target_zeroes++;
            solution_found = false;
        }
        if (current_target_zeroes > 8) {
            pthread_mutex_unlock(&mutex);
            break;
        }
        pthread_mutex_unlock(&mutex);

        // Generate random nonce and calculate the hash
        generate_random_nonce(u->nonce, NONCE_SIZE);
        user_string = user_to_string(*u);

        SHA256_CTX ctx;
        sha256_init(&ctx);
        sha256_update(&ctx, (BYTE*)user_string, strlen(user_string));
        sha256_final(&ctx, hash);
        free(user_string);

        // Check if the number of leading zeroes matches the current target
        int leading_zeroes = 0;
        for (int i = 0; i < SHA256_BLOCK_SIZE; i++) {
            if (ctx.data[i] == '0') {
                leading_zeroes++;
            } else {
                break;
            }
        }

        if (leading_zeroes >= current_target_zeroes) {
            pthread_mutex_lock(&mutex);
            if (!solution_found) {
                solution_found = true;
                printf("Thread %d found a hash with %d zeroes.\n", u->thread_id, current_target_zeroes);
                printf("SHA256 Hash: ");
                for (int i = 0; i < SHA256_BLOCK_SIZE; i++) {
                    printf("%c", ctx.data[i]);
                }
                printf("\n");
            }
            pthread_mutex_unlock(&mutex);
        }
    }

    free_user(*u);
    free(u);  // Free the allocated memory for the user object
    return NULL;
}

int main(void) {
    pthread_t threads[NUM_THREADS];
    int rc;
    long t;

    // Initialize the mutex
    pthread_mutex_init(&mutex, NULL);

    // Seed the random number generator
    srand(time(NULL));

    // Create and initialize users
    for (t = 0; t < NUM_THREADS; t++) {

        User* u = (User*)malloc(sizeof(User));
        if (u == NULL) {
            fprintf(stderr, "Failed to allocate memory for user.\n");
            exit(1);
        }

        char firstName[10];
        snprintf(firstName, 10, "User%ld", t + 1);
        initialize_user(u, firstName, "Doe", "Michael", "Group A", "Computer Science", "Engineering", "MIT");

        u->thread_id = t + 1;

        // Create the thread
        rc = pthread_create(&threads[t], NULL, thread_solution, (void*)u);
        if (rc) {
            printf("ERROR; return code from pthread_create() is %d\n", rc);
            exit(-1);
        }
    }

    // Join the threads to wait for all to finish
    for (t = 0; t < NUM_THREADS; t++) {
        pthread_join(threads[t], NULL);
    }

    // Destroy the mutex
    pthread_mutex_destroy(&mutex);

    return 0;
}