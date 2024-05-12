#include <lox.h>
#include <stdio.h>
#include <stdlib.h>

void run_repl() {
    lox_state *state = lox_state_open();
    char buff[1024];
    for (;;) {
        printf("> ");
        if (!fgets(buff, 1024, stdin)) {
            printf("\n");
            break;
        }
        lox_dostring(state, buff);
    }
    lox_state_close(state);
}
void run_file(const char *path) {
    int exit_no = EXIT_SUCCESS;
    lox_state *state = lox_state_open();

    lox_error_t result = lox_dofile(state, path);
    switch (result) {
    case LOX_ERROR_INTERPRET:
        exit_no = 70;
        lox_state_printerr(state);
        break;
    case LOX_ERROR_COMPILE:
        exit_no = 65;
        lox_state_printerr(state);
        break;
    default:
        break;
    }
defer:
    lox_state_close(state);
    exit(exit_no);
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
