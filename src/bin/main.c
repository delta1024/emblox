#include <lox.h>
#include <stdlib.h>
int main(int argc, char *argv[]) {
    int exit_code    = EXIT_SUCCESS;
    lox_state *state = lox_newstate(NULL, NULL);
    lox_loadbuffer(state, "", 0, "test");
    lox_pcall(state, 0);
defer:
    lox_close(state);
    return exit_code;
}
