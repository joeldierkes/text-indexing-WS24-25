# Text Indexing WS 2024/25

My C implementation of the assignment for text indexing course for the
WS 2024/25. In brief, the task is to implement three trie variants.

## Development

`gcc` with a suitable version (in my case `13.3.1 20240614`) is
required. `make` is a handy addon to handle the compilation. The C
standard library is the only dependency.

To build the executable, run `make` in the project directory. The
executable `ti_program` is created and can be used as described by the
task. Run `make test` to run the test suite. `ti_programm` by itself
prints the help message:

```txt
Usage: ti_programm [-du] INPUT_FILE QUERY_FILE

The main entry point for the text indexing exercise 2024/25.

options:
-d   DOT_MODE,  displays the generated trie in a DOT readable format.
-u   DUMP_MODE, dumps the trie one word per line.
```

Note that some nice-to-have features are added and can be opted in via
the cli arguments.

### Tools

Some common tools are found under `tools`. They aim to support the
development process.

## ToDo

- [x] 🌲 Implement the general trie traversal routines
  - [x] Insert
  - [x] Contains
  - [x] Delete
- [x] 📋 Implement the fixed sized array version
  - [x] Insert
  - [x] Contains
  - [x] Delete
- [ ] 📏 Implement the variable sized array version
  - [ ] Insert
  - [ ] Contains
  - [ ] Delete
- [ ] 🗃️  Implement the hash table version
  - [ ] Insert
  - [ ] Contains
  - [ ] Delete
- [x] Add the code to handle the cli
- [x] Add the contruction from input file routine
- [x] Add the queries from input file routine
- [ ] Format the output according to the task
- [ ] Add a suitable test suite
- [ ] Controll if the query null terminator before the action is
      prefixed with a space
- [ ] Implement a cleanup (free) function
