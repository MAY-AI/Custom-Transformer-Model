// Includes
/////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////

#include <chatbot.h>
#include <stdio.h>  // printf()
#include <errno.h>  // errno, EEXIST, etc...
#include <string.h> // strerror()

// Main
/////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////

int main(void) {
    // Start Algorithm
    printf("\n%s----------\n----------%s\n\n", BLUE, END);
    printf("%sStarting Algorithm:%s\n\n", BLUE, END);

    // Check token embeds status
    char te_filepath[] = TOK_EMB_DIR;
    printf("%sChecking token embeds...%s\n", YELLOW, END);
    int8_t te_code = init_token_embed(te_filepath);
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
    char pe_filepath[] = POS_ENC_DIR;
    printf("%sChecking positional encodes...%s\n", YELLOW, END);
    int8_t pe_code = init_positional_encoding(pe_filepath);
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
