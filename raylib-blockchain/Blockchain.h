#include "Block.h"

typedef struct Blockchain {
    Block* blocks;       
    int size;            
    int capacity;        
} Blockchain;

// Function prototypes for blockchain operations
Blockchain* createBlockchain(int capacity);
void addBlock(Blockchain* blockchain, Block newBlock);
void freeBlockchain(Blockchain* blockchain);
