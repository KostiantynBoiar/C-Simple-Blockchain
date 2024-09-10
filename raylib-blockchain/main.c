#include <stdio.h>
#include "Block.h"
#include "base64.h"
#include "User.h"

int main(void) {
    // Declare a block
    Block b;
    User u;

    initialize_user(&u, "John", "Doe", "Michael", "Group A", "Computer Science", "Engineering", "MIT");
    printf("Answer: %s\n", solution(u));
    free_user(u);
    // Initialize the block with sample data
    /*
    initialize_block(&b, 1, "John", "Doe", "Michael", "Group A", "Computer Science", "Engineering", "MIT", "12345abc", "privateKey123", CreateCoin);

    char* encode = base64_encode(block_to_string(b));
    char* decode = base64_decode(encode);
    printf(decode);

    // Free the allocated memory
    free_block(&b);
*/
    return 0;
}
