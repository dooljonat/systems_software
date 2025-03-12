#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <unistd.h>

#define USAGE "USAGE: ./mycp source_file target_fie\n"
#define SOURCE_REG_FILE "Source is not a regular file\n"
#define TARGET_REG_FILE "Target is not a regular file\n"
#define TARGET_IS_DIR "Target is a directory\n"

int main(int argc, char **argv) {
    // ERR: Incorrect # of arguments was passed
    if (argc != 3) {
        fprintf(stderr, USAGE);
        exit(3);
    }

    // Get file source from args
    char *source_path = argv[1];
    struct stat source_stat;
    // ...Retrieve file stats
    if (stat(source_path, &source_stat) == -1) {
        perror("stat");
        exit(4);
    }
    // ...Check if it is a regular file
    if (!S_ISREG(source_stat.st_mode)) {
        fprintf(stderr, SOURCE_REG_FILE);
        exit(5);
    }

    // Get target from args
    char *target_path = argv[2];
    struct stat target_stat;
    // ...Check if target and source are the samae
    if (stat(target_path, &target_stat) != -1 && 
        source_stat.st_ino == target_stat.st_ino) {
        fprintf(stderr, "Source and target are the same.\n");
        exit(EXIT_FAILURE);
    }
    // ...Check if target is a directory
    if (S_ISDIR(target_stat.st_mode)) {
        fprintf(stderr, TARGET_IS_DIR);
        exit(1);
    }

    // Allocate buffer
    char *buffer = malloc(sizeof(*buffer) *source_stat.st_size);
    if (buffer == NULL) {
        perror("malloc");
        exit(EXIT_FAILURE);
    }

    // Open source file descriptor
    int source_file_descriptor = open(source_path, O_RDONLY);
    if (source_file_descriptor == -1) {
        perror("open");
        free(buffer);
        exit(4);
    }

    // Read source file
    if (read(source_file_descriptor, buffer, source_stat.st_size) == -1) {
        perror("read");
        free(buffer);
        exit(EXIT_FAILURE);
    }

    // Close source file descriptor
    if (close(source_file_descriptor) == -1) {
        perror("close");
        free(buffer);
        exit(EXIT_FAILURE);
    }

    // Open target file descriptor
    int target_file_descriptor = open(target_path, O_WRONLY | O_CREAT | O_TRUNC , S_IRUSR | S_IWUSR);
    if (target_file_descriptor == -1) {
        perror("open");
        free(buffer);
        exit(6);
    }

    // Write to target
    if (write(target_file_descriptor, buffer, source_stat.st_size) == -1) {
        perror("write");
        free(buffer);
        exit(EXIT_FAILURE);
    }

    if(close(target_file_descriptor) == -1) {
        perror("close");
        free(buffer);
        exit(EXIT_FAILURE);
    }

    free(buffer);
    return EXIT_SUCCESS;
}