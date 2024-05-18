#include <lox.h>
#include <stdlib.h>
int main(int argc, char *argv[])
{
	lox_state *state = lox_newstate(NULL, NULL);
	lox_close(state);
	return EXIT_SUCCESS;
}
