#include <stdio.h>
#include <pthread.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <openssl/sha.h>
#include "Block.h"
#include "User.h"
#include "raylib.h"
#include "Blockchain.h"

#define NONCE_SIZE 64
#define NUM_THREADS 85
#define MAX_HASHES 1000  // Set an appropriate limit for the number of hashes

/*
typedef enum {MENU_MAIN, MENU_VIEW_HASHES, MENU_VIEW_GENERATING, MENU_VIEW_TIME_TAKEN} MenuState;

pthread_mutex_t mutex;
int current_target_zeroes = 1;
bool solution_found = false;

static char buffer[NUM_THREADS * 256];
static char hashHex[SHA256_DIGEST_LENGTH * 2 + 1];
unsigned char hashes[MAX_HASHES][SHA256_DIGEST_LENGTH];
int hash_count = 0;
Vector2 hash_positions[MAX_HASHES];  // Store positions for each hash

typedef struct {
    unsigned char hash[SHA256_DIGEST_LENGTH];
    char time[40];  // Adjust size if necessary
} TimeTaken;

// Store information about hashes being generated
typedef struct {
    User* user;
    unsigned char hash[SHA256_DIGEST_LENGTH];
} GeneratingHash;

GeneratingHash generating_hashes[NUM_THREADS];
int generating_hash_count = 0;

// Store current menu state
MenuState menu_state = MENU_MAIN;
clock_t raylib_start_time;
TimeTaken time_taken_array[NUM_THREADS];

// Initialize time_taken_array to zero
void initialize_time_taken_array() {
    for (int i = 0; i < NUM_THREADS; i++) {
        memset(&time_taken_array[i], 0, sizeof(TimeTaken));
    }
}

void* thread_solution(void* arg) {
    User* u = (User*)arg;

    char* user_string;
    unsigned char hash[SHA256_DIGEST_LENGTH];
    clock_t start_time, end_time;

    start_time = clock(); // Start the timer when the thread starts

    while (current_target_zeroes <= 9) {
        pthread_mutex_lock(&mutex);
        if (solution_found && current_target_zeroes <= 9) {
            // Prepare for the next target zeroes search
            current_target_zeroes++;
            solution_found = false;
            // Restart the timer for the new target
            start_time = clock();
        }
        if (current_target_zeroes > 9) {
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

        pthread_mutex_lock(&mutex);
        // Save information about the currently generating hash
        if (generating_hash_count < NUM_THREADS) {
            generating_hashes[generating_hash_count].user = u;
            memcpy(generating_hashes[generating_hash_count].hash, hash, SHA256_DIGEST_LENGTH);
            generating_hash_count++;
        }
        pthread_mutex_unlock(&mutex);

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
                if (hash_count < MAX_HASHES) {
                    // Save the hash and its position
                    memcpy(hashes[hash_count], hash, SHA256_DIGEST_LENGTH);
                    hash_positions[hash_count] = (Vector2){ 10, (float)(hash_count * 30 + 10) }; // Example position
                    hash_count++;
                }
                solution_found = true;
                end_time = clock(); // Stop the timer when a valid hash is found
                double time_taken = ((double)(end_time - start_time)) / CLOCKS_PER_SEC;
                printf("Time taken by thread %d to find %d leading zeroes: %f ticks\n", u->thread_id, current_target_zeroes, time_taken);
                printf("SHA256 Hash: ");
                for (int i = 0; i < SHA256_DIGEST_LENGTH; i++) {
                    printf("%02x", hash[i]);
                }
                printf("\n");

                // Save time taken to TimeTaken struct
                snprintf(time_taken_array[u->thread_id - 1].time, sizeof(time_taken_array[u->thread_id - 1].time), "%f ticks", time_taken);
                memcpy(time_taken_array[u->thread_id - 1].hash, hash, SHA256_DIGEST_LENGTH);
            }
            pthread_mutex_unlock(&mutex);
        }
    }

    free_user(*u);
    free(u);
    return NULL;
}

void draw_hashes() {
    for (int i = 0; i < hash_count; i++) {
        char hash_string[SHA256_DIGEST_LENGTH * 2 + 1];
        for (int j = 0; j < SHA256_DIGEST_LENGTH; j++) {
            sprintf(&hash_string[j * 2], "%02x", hashes[i][j]);
        }
        DrawText(hash_string, hash_positions[i].x, hash_positions[i].y, 20, RAYWHITE);
    }
}


int compare_time_taken(const void* a, const void* b) {
    // Cast to appropriate type
    const TimeTaken* time1 = (const TimeTaken*)a;
    const TimeTaken* time2 = (const TimeTaken*)b;

    // Convert time strings to double for comparison
    double time1_value = atof(time1->time);
    double time2_value = atof(time2->time);

    // Compare times
    if (time1_value < time2_value) return -1;
    if (time1_value > time2_value) return 1;
    return 0;
}


void draw_time_taken() {
    // Clear background or previous menu content
    ClearBackground(BLACK);

    // Calculate elapsed time since the Raylib window started
    clock_t current_time = clock();
    double elapsed_time = ((double)(current_time - raylib_start_time)) / CLOCKS_PER_SEC;

    // Draw header for time taken
    DrawText("Time Taken by Each Thread:", 10, 10, 20, RAYWHITE);

    // Draw elapsed time since Raylib window started
    char time_info[256];
    snprintf(time_info, sizeof(time_info), "Raylib Window Elapsed Time: %f ticks", elapsed_time);
    DrawText(time_info, 10, 40, 20, RAYWHITE);

    // Draw time taken for each thread
    int y_position = 70;
    for (int i = 0; i < NUM_THREADS; i++) {
        if (strlen(time_taken_array[i].time) > 0) {
            char display_text[256];
            char hash_string[SHA256_DIGEST_LENGTH * 2 + 1];
            for (int j = 0; j < SHA256_DIGEST_LENGTH; j++) {
                sprintf(&hash_string[j * 2], "%02x", time_taken_array[i].hash[j]);
            }

            snprintf(display_text, sizeof(display_text), "Thread %d: %s | Hash: %s", i + 1, time_taken_array[i].time, hash_string);
            DrawText(display_text, 10, y_position, 20, RAYWHITE);

            y_position += 30; // Move down for the next entry
        }
    }
}
*/
/*
int main(void) {
    pthread_t threads[NUM_THREADS];
    int rc;
    long t;

    // Initialize the mutex
    pthread_mutex_init(&mutex, NULL);

    // Seed the random number generator
    srand(time(NULL));

    // Initialize Raylib and create a window
    InitWindow(1200, 1200, "Hash Raylib");
    SetTargetFPS(60);

    // Initialize timer for Raylib window
    raylib_start_time = clock();

    // Initialize the time_taken_array
    initialize_time_taken_array();

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

    // Raylib window loop
    while (!WindowShouldClose()) {
        BeginDrawing();
        ClearBackground(BLACK);  // Set background to black

        // Display menu options
        if (menu_state == MENU_MAIN) {
            DrawText("Press 1 to view generated hashes", 10, 10, 20, RAYWHITE);
            DrawText("Press 3 to view time taken", 10, 40, 20, RAYWHITE);
            DrawText("Press 0 to hide current text", 10, 70, 20, RAYWHITE);
        } else if (menu_state == MENU_VIEW_HASHES) {
            draw_hashes();  // Draw all generated hashes
        } else if (menu_state == MENU_VIEW_TIME_TAKEN) {
            draw_time_taken();  // Draw time taken by threads and elapsed time
        }

        EndDrawing();

        // Handle menu input
        if (IsKeyPressed(KEY_ONE)) {
            menu_state = MENU_VIEW_HASHES;
        }
        if (IsKeyPressed(KEY_THREE)) {
            menu_state = MENU_VIEW_TIME_TAKEN;  // Show time taken menu
        }
        if (IsKeyPressed(KEY_ZERO)) {
            menu_state = MENU_MAIN;  // Switch to main menu
        }
    }

    // Close Raylib window
    CloseWindow();

    // Join the threads to wait for all to finish
    for (t = 0; t < NUM_THREADS; t++) {
        pthread_join(threads[t], NULL);
    }

    // Destroy the mutex
    pthread_mutex_destroy(&mutex);

    return 0;
}
*/

// Assume Block, Blockchain, User, and other related functions are already defined.

void solution() {
    // Initialize blockchain with capacity for 10 blocks
    Blockchain* blockchain = createBlockchain(10);

    // Initialize 10 users
    User users[10];
    initialize_user(&users[0], "John", "Doe", "A", "Group1", "Course1", "Faculty1", "University1");
    initialize_user(&users[1], "Jane", "Doe", "B", "Group2", "Course2", "Faculty2", "University2");
    initialize_user(&users[2], "Alice", "Smith", "C", "Group3", "Course3", "Faculty3", "University3");
    initialize_user(&users[3], "Bob", "Johnson", "D", "Group4", "Course4", "Faculty4", "University4");
    initialize_user(&users[4], "Charlie", "Brown", "E", "Group5", "Course5", "Faculty5", "University5");
    initialize_user(&users[5], "Eve", "Adams", "F", "Group6", "Course6", "Faculty6", "University6");
    initialize_user(&users[6], "Frank", "Miller", "G", "Group7", "Course7", "Faculty7", "University7");
    initialize_user(&users[7], "Grace", "Lee", "H", "Group8", "Course8", "Faculty8", "University8");
    initialize_user(&users[8], "Henry", "White", "I", "Group9", "Course9", "Faculty9", "University9");
    initialize_user(&users[9], "Ivy", "Williams", "J", "Group10", "Course10", "Faculty10", "University10");

    // Initialize 10 blocks
    Block blocks[10];
    char* prev_hash = NULL;  // To store the previous block's hash

    for (int i = 0; i < 10; i++) {
        // Convert user to string
        char user_str[256];
        sprintf(user_str, "%s %s %s %s %s %s %s", 
                users[i].firstName, users[i].secondName, users[i].middleName,
                users[i].groupName, users[i].course, users[i].faculty, users[i].universityName);

        char block_hash_input[512];

        // For the first block, hash is based on the user string alone
        if (i == 0) {
            strcpy(block_hash_input, user_str);
        } else {
            // For other blocks, hash is based on the previous hash + user string
            snprintf(block_hash_input, sizeof(block_hash_input), "%s%s", prev_hash, user_str);
        }

        // Generate hash (this assumes you have a hash generation function)
        char* block_hash = generate_block_random_nonce(block_hash_input, 32);

        // Initialize the block
        initialize_block(&blocks[i], i + 1, users[i].firstName, users[i].secondName, users[i].middleName,
                users[i].groupName, users[i].course, users[i].faculty, users[i].universityName, block_hash, "privateKey", CreateCoin);

        // Add block to the blockchain
        addBlock(blockchain, blocks[i], block_hash);

        // Store the current block's hash as previous hash for the next block
        prev_hash = block_hash;
    }

    // Print blockchain details
    printf("Blockchain contains %d blocks:\n", blockchain->size);
    for (int i = 0; i < blockchain->size; i++) {
        printBlock(&blockchain->blocks[i]);
    }

    // Free resources
    for (int i = 0; i < 10; i++) {
        free_block(&blocks[i]);
        free_user(&users[i]); 
    }
    freeBlockchain(blockchain);
}

int main() {
    solution();
    return 0;
}

