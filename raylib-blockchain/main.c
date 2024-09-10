#include <stdio.h>
#include <pthread.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <openssl/sha.h>
#include "User.h"

#define NONCE_SIZE 64
#define NUM_THREADS 50

// Shared data
pthread_mutex_t mutex;
int current_target_zeroes = 1;
bool solution_found = false;

void* thread_solution(void* arg) {
    User* u = (User*)arg;

    char* user_string;
    unsigned char hash[SHA256_DIGEST_LENGTH];

    // Timer variables specific to this thread
    clock_t start_time, end_time;
    start_time = clock(); // Start the timer when the thread starts

    while (current_target_zeroes <= 8) {
        pthread_mutex_lock(&mutex);
        if (solution_found && current_target_zeroes <= 8) {
            // Prepare for the next target zeroes search
            current_target_zeroes++;
            solution_found = false;
            // Restart the timer for the new target
            start_time = clock();
        }
        if (current_target_zeroes > 8) {
            pthread_mutex_unlock(&mutex);
            break;
        }
        pthread_mutex_unlock(&mutex);

        // Generate random nonce and user string
        generate_random_nonce(u->nonce, NONCE_SIZE);
        user_string = user_to_string(*u);

        // Compute SHA256 hash using OpenSSL
        SHA256_CTX ctx;
        SHA256_Init(&ctx);
        SHA256_Update(&ctx, (unsigned char*)user_string, strlen(user_string));
        SHA256_Final(hash, &ctx);

        free(user_string);

        // Count leading zeroes in the hash
        int leading_zeroes = 0;
        for (int i = 0; i < SHA256_DIGEST_LENGTH; i++) {
            if (hash[i] == 0x00) {
                leading_zeroes += 2;  // Each byte has 2 hex digits
            } else if ((hash[i] & 0xF0) == 0x00) {
                leading_zeroes += 1;  // Only upper nibble is zero
                break;
            } else {
                break;
            }
        }

        if (leading_zeroes >= current_target_zeroes) {
            pthread_mutex_lock(&mutex);
            if (!solution_found) {
                solution_found = true;
                end_time = clock(); // Stop the timer when a valid hash is found

                double time_taken = ((double)(end_time - start_time)) / CLOCKS_PER_SEC;
                printf("Time taken by thread %d to find %d leading zeroes: %f seconds\n", u->thread_id, current_target_zeroes, time_taken);
                printf("SHA256 Hash: ");
                for (int i = 0; i < SHA256_DIGEST_LENGTH; i++) {
                    printf("%02x", hash[i]);
                }
                printf("\n");
            }
            pthread_mutex_unlock(&mutex);
        }
    }

    free_user(*u);
    free(u);
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
