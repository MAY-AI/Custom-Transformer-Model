// Includes
/////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////

#include <chatbot.h>
#include <unistd.h> // POSIX write() and close()

// Function definitions
/////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////

int8_t write_all(
    const int fd,
    const void *buffer,
    size_t bytes
) {
    // Create iterable pointer
    const uint8_t *p = (const uint8_t *)buffer;

    // Loop until all bytes are written
    while (bytes > 0) {
        // Write bytes
        ssize_t written = write(fd, p, bytes);
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
