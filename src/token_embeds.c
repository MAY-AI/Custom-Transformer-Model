// Includes
/////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////

#include <chatbot.h>
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>
#include <stdlib.h>

// Function definitions
/////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////

static int8_t write_all(
    const int a,
    const void *buffer,
    size_t bytes
) {
    // Create iterable pointer
    const uint8_t *p = (const uint8_t *)buffer;

    // Loop until all bytes are written
    while (bytes > 0) {
        // Write bytes
        ssize_t written = write(a, p, bytes);
        if (written <= 0) {
            return -1;
        }

        // Increment pointer
        p += (size_t)written;

        // Decrement number of bytes left to write
        bytes -= (size_t)written;
    }

    // Return
    return 0;
}

int8_t init_token_embed(
    const char *filepath,
    const uint32_t vocab_size,
    const uint16_t model_size
) {
    // Open and/or create file
    int a = open(filepath, O_WRONLY | O_CREAT | O_EXCL, 0644);
    if (a < 0) {
        if (errno == EEXIST) {
            return 1;
        }
        return -1;
    }

    // Create header
    token_embed_header_t header = {
        .magic        = TOKEN_EMBED_MAGIC,
        .version      = TOKEN_EMBED_VERSION,
        .header_bytes = sizeof(token_embed_header_t),
        .vocab_size   = vocab_size,
        .model_size   = model_size,
        .float_bytes  = sizeof(float),
        .reserved     = 0
    };

    // Write header
    if (write_all(a, &header, sizeof(header)) != 0) {
        close(a);
        return -1;
    }

    // Allocate memory for a full token embed
    size_t row_bytes = (size_t)model_size * sizeof(float);
    float *row = malloc(row_bytes);
    if (row == NULL) {
        close(a);
        return -1;
    }

    // Fill file with random initialized values
    for (uint32_t i = 0; i < vocab_size; i++) {
        for (uint16_t d = 0; d < model_size; d++) {
            row[d] = ((float)rand() / (float)RAND_MAX) * 0.04f - 0.02f;
        }
        if (write_all(a, row, row_bytes) != 0) {
            free(row);
            close(a);
            return -1;
        }
    }
    free(row);

    // Close file and return
    if (close(a) != 0) {
        return -1;
    }
    return 0;
}
