#include <stdlib.h>
#include <string.h>
#include "Blockchain.h"

Blockchain* createBlockchain(int capacity) {
    Blockchain* blockchain = (Blockchain*)malloc(sizeof(Blockchain));
    blockchain->blocks = (Block*)malloc(capacity * sizeof(Block));
    blockchain->size = 0;
    blockchain->capacity = capacity;
    return blockchain;
}

void addBlock(Blockchain* blockchain, Block newBlock) {
    if (blockchain->size >= blockchain->capacity) {
        printf("Blockchain is full.\n");
        return;
    }
    blockchain->blocks[blockchain->size] = newBlock;
    blockchain->size++;
}

void freeBlockchain(Blockchain* blockchain) {
    free(blockchain->blocks);
    free(blockchain);
}