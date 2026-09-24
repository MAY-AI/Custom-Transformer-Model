// Includes
/////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////

#include <chatbot.h>
#include <stdio.h>  // printf()
#include <errno.h>  // errno, EEXIST, etc...
#include <string.h> // strerror()

// Global variables
/////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////

char te_filepath[] = "./data/token_embeddings.bin";
char pe_filepath[] = "./data/positional_encoding.bin";

// Main
/////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////

int main(void) {
    // Start Algorithm
    printf("\n%s----------\n----------%s\n\n", BLUE, END);
    printf("%sStarting Algorithm:%s\n\n", GREEN, END);

    // Check token embeds status
    printf("%sChecking token embeds...%s\n", YELLOW, END);
    int8_t te_code = init_token_embed(te_filepath, VOCAB_SIZE, INPUT_LAYER_DIM);
    if (te_code == 0) {
        printf("%sToken embeds created...%s\n\n", GREEN, END);
    }
    else if (te_code == 1) {
        printf("%sToken embeds already exist...%s\n\n", GREEN, END);
    }
    else if (te_code == -1) {
        printf("%sError in creating token embeds:\n%s\n\n%s", RED, strerror(errno), END);
        printf("%s----------\n----------%s\n\n", BLUE, END);
        return 1;
    }

    // Check positional encodes
    printf("%sChecking positional encodes...%s\n", YELLOW, END);
    int8_t pe_code = init_positional_encoding(pe_filepath, MAX_SEQ_LEN, INPUT_LAYER_DIM);
    if (pe_code == 0) {
        printf("%sPositional encodes created...%s\n\n", GREEN, END);
    }
    else if (pe_code == 1) {
        printf("%sPositional encodes already exist...%s\n\n", GREEN, END);
    }
    else if (pe_code == -1) {
        printf("%sError in creating positional encodes:\n%s\n\n%s", RED, strerror(errno), END);
        printf("%s----------\n----------%s\n\n", BLUE, END);
        return 1;
    }

    // End algorithm
    printf("%s----------\n----------%s\n\n", BLUE, END);
    return 0;
}
