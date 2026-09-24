// Includes
/////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////

#include <chatbot.h>
#include <unistd.h> // POSIX write() and close()
#include <fcntl.h>  // open() flags
#include <stdlib.h> // malloc(), free(), rand() and RAND_MAX
#include <errno.h>  // errno, EEXIST, etc...
#include <math.h>   // sin(), cos(), pow(), etc...

// Function definitions
/////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////

int8_t init_positional_encoding(
    const char *filepath,
    const uint32_t max_seq_len,
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
        .magic        = POS_ENCODE_MAGIC,
        .version      = POS_ENCODE_VERSION,
        .header_bytes = sizeof(file_header),
        .dimension    = max_seq_len,
        .model_size   = model_size,
        .float_bytes  = sizeof(float),
        .reserved     = 0
    };

    // Write header
    if (write_all(a, &header, sizeof(header)) != 0) {
        close(a);
        return -1;
    }

    // Allocate memory for one position's encoding row
    size_t row_bytes = (size_t)model_size * sizeof(float);
    float *row = malloc(row_bytes);
    if (row == NULL) {
        close(a);
        return -1;
    }

    // Fill file with sinusoidal positional encodings
    for (uint32_t i = 0; i < max_seq_len; i++) {
        // Fill row buffer with positional encoded values
        for (uint16_t j = 0; j < model_size; j += 2) {
            // Calculate angular value
            double exponent = (double)j / (double)model_size;
            double denom = pow(10000.0, exponent);
            double angle = (double)i / denom;

            // Fill even/odd elements
            row[j] = (float)sin(angle);
            if ((uint32_t)(j + 1) < model_size) {
                row[j + 1] = (float)cos(angle);
            }
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
