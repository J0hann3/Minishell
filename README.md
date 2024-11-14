<h1 align=center>💻 Minishell</h1>
<p align="center">
  <img src="img/minishell.png?raw=true"/>
</p>

## About
>This project is about creating a simple shell.
>Yes, your own little bash.
>You will learn a lot about processes and file descriptors.

[Subject Minishell Project 42](minishell.pdf)

The *Minishell* program aims to replicate the essential features of a Unix shell, allowing users to enter and execute commands as they would in a typical terminal session.

## Features
- **Description**: Provides an interactive terminal environment where you can execute shell commands.
- **Key Features**:
  - Supports command execution, input/output redirection, heredoc, pipes, conditional execution, variable expansion, wildcard and quoting.
  - Operates in both interactive and non-interactive modes.
  - Handles interruptions like `CTRL-C`, `CTRL-D` or `CTRL-\`.
  - Implements built-in commands including `echo`, `cd`, `pwd`, `export`, `unset`, `env`, and `exit`.
- **Usage**: Run `./minishell` and enter commands in the prompt as you would in a standard shell.

## Setup

**Clone the Repository**:

```bash
git clone https://github.com/J0hann3/Minishell.git
cd Minishell
make
```

## Usage

### Interactive mode

1. Run the program `./minishell` without any arguments.
2. Once launched, the program will display a prompt and wait for you to enter commands.
3. Type each command and press ENTER to execute.
4. `CTRL-C` stops the current command or displays a new prompt.
5. Exit the program by typing `exit` or pressing `CTRL-D`.

```bash
./minishell
minishell$> echo Hello, world!
Hello, world!
minishell$> ls | grep minishell > output.txt
minishell$> echo s*
src suppr.valgrind
minishell$> export HELLO="hello world"
minishell$> echo "$HELLO"
hello world
minishell$> echo '$HELLO'
$HELLO
minishell$> echo -n "hello " || echo bad && echo world
hello world
minishell$> exit
```
### Non-interactive mode
1. Run a command by piping it to `./minishell`.

```bash
echo 'ls' | ./minishell
img  includes  Makefile  minishell  obj  README.md  src
```

## New Notion

### Built-in commands
- Recreated the following built-in commands:
	- `echo` with option ``-n`
	- `pwd` with no options
	- `env` with no options or arguments
	- `cd` with only a relative or absolute path
	- `export` with no options
	- `unset` with no options
	- `exit` with no options

### Pipes
- Pipes (|) redirect the output of one command to the input of another.
- Handles the use of pipes (`|`) to chain commands together.
- Minishell supports multiple pipes in the same command line and uses fork processes to execute commands in parallel.

### Waitpid
- Waits a process.
- Executed binaries must be waited in the execution flow by the main process.
- At the end of each pipeline, `waitpid` waits for all processes created within that pipeline.
1. WIFEXITED(status): True if child exited normally.
	- WEXITSTATUS(status): Returns code when child exits.
2. WIFSIGNALED(status): True if child exited because a signal was not caught.
	- WTERMSIG(status): Returns code of the terminating signal.
3. WIFSTOPPED(status): True if child is stopped.
	- WSTOPSIG(status): Returns code of the stop signal.

### Exit Status
- Returns a command's exit status that is provided by the `waitpid`, or 128 + n if terminated by signal n.
- The exit status of a pipeline is the exit status of the last command.
- For commands using && and ||, the exit status is that of the last command executed.

### Readline
- Function that reads from the standard input.
- Keeps a history of the command entered in the program.
- Supports navigating through command history using the arrow keys.

### Binary trees
- Uses binary trees to structure command execution and establish command priorities.
- Implements priorities with parentheses (no sub-shells implemented).

### Redirection and Heredoc
- Use `dup` and `dup2` to manage input/output redirection.
- Create processes to handle multi-heredoc.
- Redirection are processed in the order they appear, from left to right. 

```bash
./minishell
minishell$> ls | grep src > outfile > outfile1
minishell$> ls | grep src >> outfile >> outfile1
minishell$> < infile grep obj | grep src > outfile
minishell$> cat <<hello <<world
> bad
> hello
> good
> world
good
```

## Code Structure
The `main` function operates as follows:
```
init env
while (!exit)
	readline
	if (heredoc)
		read heredoc
	history
	parse
	get PATH
	exec command
```