# Notes on the C implementation of NCNDI (still under construction)

Compile everything by running `./build`. This is a temporary hack; later I will write a makefile.

You can turn on debugging messages at compile time by one of the following means:

- Change the value of the `MPL_DEBUG_` macro in `config.h` to 1, or

- Pass the option `-DMPL_DEBUG_=1` to the build script; i.e. `./build -DMPL_DEBUG_=1`.

To get rid of object files and executables in this directory, run `./clean`.