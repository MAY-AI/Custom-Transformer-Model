// Includes
/////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////

#include <chatbot.h>
#include <stdio.h>

// Main
/////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////

int main(void) {
    char filepath[] = "./data/token_embeddings.bin";
    int8_t code = init_token_embed(filepath, VOCAB_SIZE, INPUT_LAYER_DIM);
    if (code == 0) {
        printf("Token embeds created...");
        return 0;
    }
    else if (code == 1) {
        printf("Token embeds already exist...");
        return 0;
    }
    else if (code == -1) {
        printf("Error in creating token embeds...");
        return 0;
    }
}
