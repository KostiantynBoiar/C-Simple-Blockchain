#pragma once
#include "User.h"
#include <stdlib.h>
#include <string.h>

typedef enum transactionType {
    CreateCoin,
    Pay
} transactionType;

typedef struct Block {
    int number;
    User blockchainUser;   
    char* hash;
    char* privateKey;
    transactionType transactionT;
    char nonce[256];
} Block;

const char* transactionTypeToString(transactionType type);
char* block_to_string(Block b);
void initialize_block(Block* b, int number, const char* first, const char* second, const char* middle, const char* group, const char* course, const char* faculty, const char* university, const char* hash, const char* privateKey, transactionType type);
void free_block(Block* b);
void printBlock(const Block* block);
void generate_block_random_nonce(Block* block);
// h1 => h1 + b2 => h2 + h3