#include "Block.h"

typedef struct Blockchain {
    Block* blocks;        // Array of actual Block structures
    char** block_hashes;   // Array of hashes (optional, if needed)
    int size;
    int capacity;
} Blockchain;



// Function prototypes for blockchain operations
Blockchain* createBlockchain(int capacity);
void addBlock(Blockchain* blockchain, Block newBlock, char* newBlockHash);
void freeBlockchain(Blockchain* blockchain);
