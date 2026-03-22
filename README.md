# Minishell

## Description

`minishell` is a C project that implements a small Unix-like shell with parsing, expansion, redirections, pipelines, and command execution.

This codebase also parses and executes logical operators (`&&`, `||`) and parenthesized command groups.

## Features

- Interactive shell loop with `readline` history support.
- Non-interactive mode (commands can be piped to stdin).
- Builtins: `cd`, `pwd`, `echo`, `env`, `export`, `unset`, `exit`.
- Environment variables stored in an internal linked list.
- Variable expansion (`$VAR`, `$?`) and expansion inside double-quoted strings.
- Redirections: `<`, `>`, `>>`, `<<` (heredoc).
- Pipelines built around `fork`, `pipe`, `dup2`, and `execve`.
- Wildcard expansion for `*` against entries in the current directory (dotfiles only when the pattern starts with `.`).
- Logical operators `&&`, `||`, and parenthesized command groups.

## How it works

1. The main loop reads a line, initializes runtime state, and dispatches parsing/execution.
2. The parser tokenizes input into a linked list (`t_word`) and runs syntax checks (pipes, redirections, logic tokens, parenthesis).
3. Pre-execution steps handle heredocs, variable expansion, wildcard expansion, and token joins.
4. Execution chooses between builtin dispatch in-process and external commands via `fork` + `execve`, with redirections/pipes applied through fd duplication.
5. Exit status is tracked globally and reused by conditional execution.

## Installation / Build

### Requirements

- `cc` (C compiler)
- `make`
- `readline` development/runtime library

### Build commands

```bash
make
```

Other available targets:

```bash
make clean
make fclean
make re
```

The build produces the executable:

```bash
./minishell
```

## Usage

Interactive mode:

```bash
./minishell
```

Non-interactive mode:

```bash
printf 'echo hello_from_minishell\nexit\n' | ./minishell
```

## Example

```bash
./minishell
echo "User: $USER"
ls -la | grep src > out.txt
cat out.txt
exit
```

Tested non-interactive smoke example output:

```text
hello_from_minishell
```

## Project Structure

```text
include/
  minishell.h            # Core types, tokens, prototypes
libft/                   # Internal 42 utility library
src/
  main/                  # Shell loop, prompt, signal setup
  env/                   # Environment linked-list management
  parsing/               # Tokenization and syntax checks
  expand/                # Variable and wildcard expansion
  exec/                  # Heredoc prep, redirections, pipelines, execution
  builtins/              # Builtin command implementations
  free_and_exit/         # Cleanup and exit utilities
  git/                   # Prompt git branch/dirty-state helpers
tester/                  # Local test scripts/assets
Makefile
```

## What I learned

- Designing a shell around linked-list based token streams and explicit execution phases.
- Implementing process orchestration with `fork`, `execve`, pipes, redirections, and wait status handling.
- Building parsing/syntax checks for shell grammar edge cases.
- Managing shell state (environment, exit code, signals, heredoc behavior) consistently across parent and child processes.
