#include "Block.h"

void initialize_block(Block* b, int number, const char* first, const char* second, const char* middle, const char* group, const char* course, const char* faculty, const char* university, const char* hash, const char* privateKey, enum transactionType type) {
    b->number = number;

    initialize_user(&b->blockchainUser, first, second, middle, group, course, faculty, university);

    b->hash = (char*)malloc(strlen(hash) + 1);
    strcpy(b->hash, hash);

    b->privateKey = (char*)malloc(strlen(privateKey) + 1);
    strcpy(b->privateKey, privateKey);

    b->transactionT = type;
}

const char* transactionTypeToString(transactionType type) {
    switch (type) {
    case CreateCoin:
        return "CreateCoin";
    case Pay:
        return "Pay";
    default:
        return "Unknown";
    }
}

char* block_to_string(Block b) {
    // Get the user string representation
    const char* userString = user_to_string(b.blockchainUser);

    if (!userString) {
        return NULL;  // Handle failure in user_to_string
    }

    // Allocate memory for the block string representation
    // Adjust size based on the expected length of the fields
    char* result = (char*)malloc(2048);  // Adjust size as needed

    if (!result) {
        free((void*)userString);  // Free user string memory on failure
        return NULL;  // Handle memory allocation failure
    }

    snprintf(result, 2048,
        "Block Number: %d\n"
        "User Details:\n%s"
        "Hash: %s\n"
        "Private Key: %s\n"
        "Transaction Type: %s\n"
        "Nonce: %s\n",
        b.number,
        userString,
        b.hash ? b.hash : "N/A",
        b.privateKey ? b.privateKey : "N/A",
        transactionTypeToString(b.transactionT),
        b.nonce);

    // Free the user string memory
    free((void*)userString);

    return result;
}

void free_block(Block* b) {
    free_user(b->blockchainUser);

    free(b->hash);
    free(b->privateKey);
}

void printBlock(const Block* block) {
    printf("Block Number: %d\n", block->number);
    printf("User: %s\n", user_to_string(block->blockchainUser));
    printf("Hash: %s\n", block->hash);
    printf("Private Key: %s\n", block->privateKey);
    printf("Transaction Type: %s\n", transactionTypeToString(block->transactionT));
    printf("Nonce: %s\n", block->nonce);
    printf("-----------------------------\n");
}

char* generate_block_random_nonce(char* nonce, size_t length) {
    const char charset[] = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789";
    for (size_t i = 0; i < length - 1; i++) {
        int randomIndex = rand() % (sizeof(charset) - 1);
        nonce[i] = charset[randomIndex];
    }
    nonce[length - 1] = '\0';  // Null-terminate the nonce
    return nonce;
}