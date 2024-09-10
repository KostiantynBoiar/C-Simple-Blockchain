#pragma once
#include "User.h"
#include <stdlib.h>
#include <string.h>

typedef enum {
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
