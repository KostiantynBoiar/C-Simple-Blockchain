#include <stdlib.h>
#include <string.h>
#include <openssl/sha.h>

#include "Blockchain.h"

Blockchain* createBlockchain(int capacity) {
    Blockchain* blockchain = (Blockchain*)malloc(sizeof(Blockchain));
    blockchain->block_hashes = (Block*)malloc(capacity * sizeof(SHA256_DIGEST_LENGTH));
    blockchain->size = 0;
    blockchain->capacity = capacity;
    return blockchain;
}

void addBlock(Blockchain* blockchain, char* newBlockHash) {
    if (blockchain->size >= blockchain->capacity) {
        printf("Blockchain is full.\n");
        return;
    }
    blockchain->block_hashes[blockchain->size] = newBlockHash;
    blockchain->size++;
}

void freeBlockchain(Blockchain* blockchain) {
    free(blockchain->block_hashes);
    free(blockchain);
}