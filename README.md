# C++ virtual machine version of Nori.IO

Mostly made to practice making a virtual machine.

There are no stability guarentees about the bytecode format.

| Command | Status | Description |
|-|-|-|
| `>` | ✅ | Push the value next to it |
| `<` | ✅ | Pop the last value |
| `N` | ❌ | Push the numeric user input |
| `I` | ✅ | Push the user input |
| `,` | ❌ | Push the user input as an ASCII value |
| `O` | ✅ | Output the last value to the console then pop it |
| `.` | ❌ | Output the last ASCII value to the console then pop it |
| `@` | ✅ | Swap the last two values |
| `$` | ✅ | Reverse the whole stack |
| `:` | ✅ | Duplicate the top value |
| `+` | ✅ | Add last two values together, leaving only the result |
| `-` | ✅ | Subtract last two values together, leaving only the result |
| `*` | ✅ | Multiply last two values together, leaving only the result |
| `/` | ✅ | Divide last two values together, leaving only the result |
| `^` | ✅ | Raise last two values together, leaving only the result |
| `z` | ✅ | Square root the last value, leaving only the result |
| `%` | ❓ | Modulo last two values together, leaving only the result |
| `c` | ✅ | Ceil the last number |
| `f` | ✅ | Floor the last number |
| `r` | ✅ | Push a random number |
| `b` | ✅ | Push a random bit (either 0 or 1) |
| `B` | ✅ | Push a random byte |
| `W` | ✅ | Set the IP position to 0 (wrap around the program) |