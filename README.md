# C++ virtual machine version of [nori.io](https://github.com/mkukiro/nori.io)

Mostly made to practice making a virtual machine.

There are no stability guarentees about the bytecode format.

## Usage

### Compile a file to bytecode

```
nori build <file.nori>
```

Outputs into `file.nr`

### Run a bytecode file

```
nori run <file.nr>
```

### Compile and run (no output file)

Useful if you just want to experiment

```
nori exec
nori exec <file.nori>
```

If you don't pass a file to the command, it will read from standard input.

## Commands

A fuller description of syntax and other things are available at [nori.io](https://github.com/mkukiro/nori.io) and [the wiki](https://esolangs.org/wiki/Nori.io). 

| Command | Status | Description |
|-|-|-|
| `>` | ✅ | Push the value next to it |
| `<` | ✅ | Pop the last value |
| `N` | ✅ | Push the numeric user input |
| `I` | ✅ | Push the user input |
| `,` | ✅ | Push the user input as an ASCII value |
| `O` | ✅ | Output the last value to the console then pop it |
| `.` | ✅ | Output the last ASCII value to the console then pop it |
| `@` | ✅ | Swap the last two values |
| `$` | ✅ | Reverse the whole stack |
| `:` | ✅ | Duplicate the top value |
| `+` | ✅ | Add last two values together, leaving only the result |
| `-` | ✅ | Subtract last two values together, leaving only the result |
| `*` | ✅ | Multiply last two values together, leaving only the result |
| `/` | ✅ | Divide last two values together, leaving only the result |
| `^` | ✅ | Raise last two values together, leaving only the result |
| `z` | ✅ | Square root the last value, leaving only the result |
| `%` | ✅ | Modulo last two values together, leaving only the result |
| `c` | ✅ | Ceil the last number |
| `f` | ✅ | Floor the last number |
| `r` | ✅ | Push a random number |
| `b` | ✅ | Push a random bit (either 0 or 1) |
| `B` | ✅ | Push a random byte |
| `W` | ✅ | Set the IP position to 0 (wrap around the program) |
| `[` | ✅ | Jump to matching `]` if the top of the stack is zero |
| `]` | ✅ | Jump to matching `[` if the top of the stack is non-zero |