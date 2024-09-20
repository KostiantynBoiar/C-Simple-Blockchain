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
    // We will combine the user string and block fields into a single string
    char* user_str = (char*)user_to_string(b.blockchainUser); // Convert User to string
    const char* transaction_type_str = transactionTypeToString(b.transactionT); // Get transaction type string
    
    // Calculate the buffer size (tweak this size if necessary)
    size_t buffer_size = strlen(user_str) + strlen(b.hash) + strlen(b.privateKey) + strlen(transaction_type_str) + strlen(b.nonce) + 256;
    
    // Allocate memory for the final string
    char* result = (char*)malloc(buffer_size);
    if (result == NULL) {
        return NULL; // Memory allocation failed
    }
    
    // Format the Block fields into a string
    snprintf(result, buffer_size, 
             "Block Number: %d\n"
             "User Info: %s\n"
             "Hash: %s\n"
             "Private Key: %s\n"
             "Transaction Type: %s\n"
             "Nonce: %s\n", 
             b.number, 
             user_str, 
             b.hash, 
             b.privateKey, 
             transaction_type_str, 
             b.nonce);
    
    // Free the user string as it was dynamically allocated
    free(user_str);
    
    return result; // Caller is responsible for freeing this memory
}

void free_block(Block* b) {
    free_user(&b->blockchainUser);

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

void generate_block_random_nonce(Block* block) {
    const char charset[] = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789";
    
    // Ensure length doesn't exceed the fixed size of the nonce array
    size_t length = sizeof(block->nonce);

    for (size_t i = 0; i < length - 1; i++) {
        int randomIndex = rand() % (sizeof(charset) - 1);
        block->nonce[i] = charset[randomIndex];
    }
    
    block->nonce[length - 1] = '\0';  // Null-terminate the nonce
}