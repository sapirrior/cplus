# CLI & REPL Usage

The C+ executable (`cplus`) offers several ways to run and interact with the language.

## Running Script Files
To execute a source file ending in `.cp`, provide the filename as an argument:
```bash
./cplus examples/hello_world.cp
```

## Interactive REPL
Running `cplus` without any arguments starts the Read-Eval-Print Loop (REPL). This allows you to type code and see results immediately.
```bash
./cplus
> PRINT 1 + 2
3
> exit
```
Type `exit` to quit the REPL.

## Global Flags
- `-version`: Displays the current version of the C+ interpreter as defined in `config.json`.
- `-help`: Shows a quick summary of these command-line options.
