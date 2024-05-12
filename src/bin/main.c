// #include <lox.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>

char *open_file(const char *path) {
    char *result = NULL;
    FILE *file = fopen(path, "r");
    if (file == NULL)
        return NULL;

    fseek(file, 0, SEEK_END);
    size_t f_len = ftell(file);
    rewind(file);

    char *buffer = malloc(sizeof(char) * (f_len + 1));
    if (buffer == NULL) {
        fclose(file);
        return NULL;
    }
    int read = fread(buffer, sizeof(char), f_len, file);
    buffer[read] = '\0';
    fclose(file);
    return buffer;
}
void run_repl() {
    char buff[1024];
    for (;;) {
        printf("> ");
        if (!fgets(buff, 1024, stdin)) {
            printf("\n");
            break;
        }
        printf("%s\n", buff);
    }
}
void run_file(const char *path) {
    char *f_contents = open_file(path);
    if (f_contents == NULL) {
        char buf[1024];
        snprintf(buf, 1024, "could not open file %s", path);
        perror(buf);
        exit(EXIT_FAILURE);
    }
    printf("%s\n", f_contents);
    free(f_contents);
    exit(EXIT_SUCCESS);
}

int main(int argc, char *argv[]) {
    if (argc == 2) {
        run_file(argv[1]);
    } else if (argc == 1) {
        run_repl();
    } else {
        fprintf(stderr, "usage: lox <file>\n");
        return EXIT_FAILURE;
    }
    return EXIT_SUCCESS;
}
