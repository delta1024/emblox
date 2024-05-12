# Lox
An implementatian of lox sutable for embeding.

This is an implementation of the lox language. Eventaly I plan to morph it into my own language.

## How to build 
to build and install the program on arch linux run:
```sh
zig build dryrun
sudo zig build --prefix=/usr
```
to uninstall the program run:
```sh
sudo zig build uninstall --prefix=/usr
```

## Usage
to run the interpreter run 
```sh
lox 
```

embeding the runtime into your own program 
```c
#include <stddef.h>
#include <lox.h>

int main() {
    LoxState *state = lox_open_state();
    if (state == NULL) 
        return 1;
    lox_print_version();
    lox_close_state(state);
}
```
then when building use the following flags
```
cc -llox main.c -o print_lox_version
```
