// Header definition
/////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////

#ifndef CHATBOT_H
#define CHATBOT_H

// Includes
/////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////

#include <stdint.h> // Fixed-width integer types: uint8_t, int8_t, etc...

// Type definitions
/////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////

typedef struct {
    char magic[8] __attribute__((nonstring));
    uint32_t version;
    uint32_t header_bytes;
    uint32_t dimension;
    uint32_t model_size;
    uint32_t float_bytes;
    uint32_t reserved;
} file_header;
_Static_assert(
    sizeof(file_header) == 32,
    "File headers must be 32 bytes"
);

// Global variables
/////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////

// Terminal printing
#define RED    "\x1b[31m"
#define GREEN  "\x1b[32m"
#define YELLOW "\x1b[33m"
#define BLUE   "\x1b[34m"
#define END    "\x1b[0m"

// Data directory locations
#define TOK_EMB_DIR "./data/token_embeddings.bin"
#define POS_ENC_DIR "./data/positional_encodings.bin"

// NN params
#define NUM_LAYERS      96u
#define INPUT_LAYER_DIM 12288u
#define NUM_HEADS       96u
#define HEAD_DIM        INPUT_LAYER_DIM / NUM_HEADS
#define FF_DIM          4 * INPUT_LAYER_DIM
#define VOCAB_SIZE      75000u
#define MAX_SEQ_LEN     8192u
_Static_assert(
    INPUT_LAYER_DIM % NUM_HEADS == 0u,
    "Incompatable dimensions, ensure 'INPUT_LAYER_DIM modulo NUM_HEADS = 0'"
);

// Data params
#define TOKEN_EMBED_MAGIC   "TOKEMB01"
#define TOKEN_EMBED_VERSION 1u
#define POS_ENCODE_MAGIC    "POSENC01"
#define POS_ENCODE_VERSION  1u

// Function declarations
/////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////

int8_t write_all(
    const int fd,
    const void *buffer,
    size_t bytes
);
int8_t init_token_embed(
    const char *filepath
);
int8_t init_positional_encoding(
    const char *filepath
);
int8_t init_weight_bias(
    const char *filepath
);

// End of header definiton
/////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////

#endif
