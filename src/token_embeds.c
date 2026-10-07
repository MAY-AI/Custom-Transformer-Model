// Includes
/////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////

#include <chatbot.h>
#include <unistd.h> // POSIX write() and close()
#include <fcntl.h>  // open() flags
#include <stdlib.h> // malloc(), free(), rand() and RAND_MAX
#include <errno.h>  // errno, EEXIST, etc...

// Function definitions
/////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////

int8_t init_token_embed(
    const char *filepath
) {
    // Open and/or create file
    #ifdef _WIN32
        int a = open(filepath, O_WRONLY | O_CREAT | O_EXCL | O_BINARY, 0644);
    #else
        int a = open(filepath, O_WRONLY | O_CREAT | O_EXCL, 0644);
    #endif
    if (a < 0) {
        if (errno == EEXIST) {
            return 1;
        }
        return -1;
    }

    // Create header
    file_header header = {
        .magic        = TOKEN_EMBED_MAGIC,
        .version      = TOKEN_EMBED_VERSION,
        .header_bytes = sizeof(file_header),
        .dimension    = VOCAB_SIZE,
        .model_size   = INPUT_LAYER_DIM,
        .float_bytes  = sizeof(float),
        .reserved     = 0
    };

    // Write header
    if (write_all(a, &header, sizeof(header)) != 0) {
        close(a);
        return -1;
    }

    // Allocate memory for a full token embed
    size_t bytes_per_token = (size_t)INPUT_LAYER_DIM * sizeof(float);
    float *token = malloc(bytes_per_token);
    if (token == NULL) {
        close(a);
        return -1;
    }

    // Fill file with random initialized values for each token
    for (uint32_t i = 0; i < VOCAB_SIZE; i++) {
        // Fill token buffer with random numbers between [-0.02, 0.02]
        for (uint16_t j = 0; j < INPUT_LAYER_DIM; j++) {
            token[j] = ((float)rand() / (float)RAND_MAX) * 0.04f - 0.02f;
        }

        // Write token buffer
        if (write_all(a, token, bytes_per_token) != 0) {
            free(token);
            close(a);
            return -1;
        }
    }
    free(token);

    // Close file and return
    if (close(a) != 0) {
        return -1;
    }
    return 0;
}
