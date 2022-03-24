# ABI for the MPL programming language

The MPL programming language has a compiler which generates
TMA-16 assembly code. The TMA-16 is a very simple 16-bit RISC virtual machine.
[Follow this link for more information about using the TMA-16.](https://github.com/DanteFalzone0/tma-16)

Although the TMA-16 is a virtual machine, it is very low-level. Writing
assembly for the TMA-16 is essentially like writing baremetal code for
a minicomputer from the 1980s. The machine has very few standard
library subroutines written for it, and those that do exist are not loaded
into the runtime by default; you must #include them in your assembly
code. There is no support for graphics, nor for file I/O other than stdin
and stdout. The machine has four general-purpose registers and a
register for the instruction pointer, a 16-word cache which is called
the "stack" because it can only be pushed to and popped from, and
a single 16-bit address space with no segmentation, meaning that the
programmer is limited to 64KB for code and data.

Bytecode compiled from MPL programs has the extension "*.tmx"
and can be run on the TMA-16.

## Function calling conventions
Function arguments go on the stack in the order they appear in the
function declaration. To call a function, save the registers, save the
contents of the stack, push the return address (address to which to
return when the function exits), and then push the arguments in order
of the parameters.

For example, for this function invocation:
```
int x = foo(3, 2.7, "beans");
```
the call stack might look like this (top to bottom):
| Human-readable repr | Hexadecimal |
|---------------------|-------------|
| address of "beans"  |  `0x2CEF`   |
| 2.7                 |  `0x4166`   |
| 3                   |  `0x0003`   |
| return address      |  `0x01EB`   |

Return values are stored in `ra` before popping the return address
into `ip`.

## Binary layouts
The binary layouts are very simple. Integers are stored with one's
complement representation, floats are stored with the IEEE 754
16-bit standard representation, and strings are stored as pointers
to null-terminated char arrays.
