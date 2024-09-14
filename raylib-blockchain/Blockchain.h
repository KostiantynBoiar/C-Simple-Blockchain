#include "Block.h"

typedef struct Blockchain {
    char* block_hashes;       
    int size;            
    int capacity;        
} Blockchain;

// Function prototypes for blockchain operations
Blockchain* createBlockchain(int capacity);
void addBlock(Blockchain* blockchain, char* newBlockHash);
void freeBlockchain(Blockchain* blockchain);
