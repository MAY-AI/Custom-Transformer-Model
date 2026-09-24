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
    const char *filepath,
    const uint32_t vocab_size,
    const uint16_t model_size
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
        .dimension    = vocab_size,
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
        // Fill row buffer with random numbers between [-0.02, 0.02]
        for (uint16_t j = 0; j < model_size; j++) {
            row[j] = ((float)rand() / (float)RAND_MAX) * 0.04f - 0.02f;
        }

        // Write row buffer
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
