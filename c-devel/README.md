Compile everything by running `./build`. This is a temporary hack; later I will write a makefile.

You can turn on debugging messages at compile time by one of the following means:

    0. Change the value of the `MPL_DEBUG_` macro in `config.h` to 1, or

    1. Pass the option `-DMPL_DEBUG_=1` to the build script; i.e. `./build -DMPL_DEBUG_=1`.
