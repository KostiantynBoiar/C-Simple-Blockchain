#include <stdlib.h>
#include <string.h>
#include <openssl/sha.h>

#include "Blockchain.h"

Blockchain* createBlockchain(int capacity) {
    Blockchain* blockchain = (Blockchain*)malloc(sizeof(Blockchain));
    blockchain->blocks = (Block*)malloc(capacity * sizeof(Block));  // Allocate blocks
    blockchain->block_hashes = (char**)malloc(capacity * sizeof(char*)); // Allocate hashes (if needed)
    blockchain->size = 0;
    blockchain->capacity = capacity;
    return blockchain;
}



void addBlock(Blockchain* blockchain, Block newBlock, char* newBlockHash) {
    if (blockchain->size >= blockchain->capacity) {
        printf("Blockchain is full.\n");
        return;
    }
    blockchain->blocks[blockchain->size] = newBlock; // Store the block
    blockchain->block_hashes[blockchain->size] = strdup(newBlockHash); // Store the block hash
    blockchain->size++;
}


void freeBlockchain(Blockchain* blockchain) {
    for (int i = 0; i < blockchain->size; i++) {
        free(blockchain->block_hashes[i]); // Free each hash string
    }
    free(blockchain->block_hashes); // Free the array of pointers
    free(blockchain);
}