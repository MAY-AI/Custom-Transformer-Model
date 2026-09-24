// Includes
/////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////

#include <chatbot.h>
#include <stdio.h>
#include <string.h>

// Global variables
/////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////

char filepath[] = "./data/token_embeddings.bin";

// Main
/////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////

int main(void) {
    // Start Algorithm
    printf("\n%s----------\n----------%s\n\n", BLUE, END);
    printf("%sStarting Algorithm:%s\n\n", GREEN, END);

    // Check token embeds status
    printf("%sChecking vocabulary embeds...%s\n", YELLOW, END);
    int8_t code = init_token_embed(filepath, VOCAB_SIZE, INPUT_LAYER_DIM);
    if (code == 0) {
        printf("%sToken embeds created...%s\n\n", GREEN, END);
    }
    else if (code == 1) {
        printf("%sToken embeds already exist...%s\n\n", GREEN, END);
    }
    else if (code == -1) {
        printf("%sError in creating token embeds:\n%s\n\n%s", RED, strerror(errno), END);
        printf("%s----------\n----------%s\n\n", BLUE, END);
        return 1;
    }

    // End algorithm
    printf("%s----------\n----------%s\n\n", BLUE, END);
    return 0;
}
