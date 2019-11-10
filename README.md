# TeSH

It is a very basic command line interface written in C, for the purpose of Operating Systems course taken at the time.

It runs built-in commands as:

- cd
- exit
- pwd

But it also runs precompiled path-located binaries on the bin/ folder created on compilation. This means you may be able to dynamically allocate new commands by putting the binaries on a bin folder in the main directory. This feature was added by the latest commits. Preveious idea and also the idea used for the purpose of the course-project was for all commands to be compiled and link to a single binary file. Although this allows for faster execution, it does not provides a way to adds new programs without recompilation of the entire command line program.

TeSh supports pipes "|" and input-output redirection using "<" and ">" operators, and for a more general case, a combination of pipes and redirections.

As the project requires, the "ls", "grep" and "chmod" commands were also implemented using UNIX syscall and structures.


## Directory Structure

``` bash
    - src/
        |- tesh.c
        |- util.h
        |- ls.c #ls implementation
        |- ls.h
        |- chmod.c
        |- chmod.h
        |- grep.c
        |- grep.h
        |- Makefile

    - enunciado.pdf
    - LICENSE
    - Makefile
    - README.md

```

## How to run

To properly build and run all the code you may use the make utility:

```bash
$ make run
```

Otherwise, for just building the code you may use "make compile".
