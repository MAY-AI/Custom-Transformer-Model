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
        .magic        = POS_ENCODE_MAGIC,
        .version      = POS_ENCODE_VERSION,
        .header_bytes = sizeof(file_header),
        .dimension    = MAX_SEQ_LEN,
        .model_size   = INPUT_LAYER_DIM,
        .float_bytes  = sizeof(float),
        .reserved     = 0
    };

    // Write header
    if (write_all(a, &header, sizeof(header)) != 0) {
        close(a);
        return -1;
    }

    // Allocate memory for one position's encoding row
    size_t bytes_per_encode = (size_t)INPUT_LAYER_DIM * sizeof(float);
    float *pos_enc = malloc(bytes_per_encode);
    if (pos_enc == NULL) {
        close(a);
        return -1;
    }

    // Fill file with sinusoidal positional encodings for each positional encode
    for (uint32_t i = 0; i < MAX_SEQ_LEN; i++) {
        // Fill positional encode buffer with positional encoded values
        for (uint16_t j = 0; j < INPUT_LAYER_DIM; j += 2) {
            // Calculate angular value
            double exponent = (double)j / (double)INPUT_LAYER_DIM;
            double denom = pow(10000.0, exponent);
            double angle = (double)i / denom;

            // Fill even/odd elements
            pos_enc[j] = (float)sin(angle);
            if ((uint32_t)(j + 1) < INPUT_LAYER_DIM) {
                pos_enc[j + 1] = (float)cos(angle);
            }
        }

        // Write row buffer
        if (write_all(a, pos_enc, bytes_per_encode) != 0) {
            free(pos_enc);
            close(a);
            return -1;
        }
    }
    free(pos_enc);

    // Close file and return
    if (close(a) != 0) {
        return -1;
    }
    return 0;
}
