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
  - Supports Interactive and Non-interactive mode.
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
4. Exit the program by typing `exit` or pressing `CTRL-D`.

```bash
./minishell
minishell$> echo Hello, world!
Hello, world!
minishell$> ls | grep minishell > output.txt
minishell$> exit
```
### Non-interactive mode
1. Write the command to run by piping it to `./minishell`

```bash
echo 'ls' | ./minishell
img  includes  Makefile  minishell  obj  README.md  src
```

## New Notion

### Built-in commands
- Recreated the following commands:
	- echo with option -n
	- pwd with no options
	- env with no options or arguments
	- cd with only a relative or absolute path
	- export with no options
	- unset with no options
	- exit with no options

### Pipe
- Pipe redirects the output of a process to the input of another one.
- Handles the use of pipes (`|`) to chain commands together.
- Executes multiple pipes in the same command line.
- Executes parallel processes using forks logic.

### Waitpid
- Waits a process.
- Executed binaries must be waited in the execution flow by the main process.
- At the end of each pipeline, `waitpid` waits for all the processes created during it.
1. WIFEXITED(status): True if child exited normally.
	- WEXITSTATUS(status): Returns code when child exits.
2. WIFSIGNALED(status): True if child exited because a signal was not caught.
	- WTERMSIG(status): Returns code of the terminating signal.
3. WIFSTOPPED(status): True if child is stopped.
	- WSTOPSIG(status): Returns code of the stop signal.

### Readline
- Function that reads from the standard input.
- Keeps a history of the command entered in the program.
- Can use arrows to walk through the history.

### Binary trees
- Uses binary trees to structure commands execution and priorities.
- Implements priorities with parentheses(no sub-shells implemented).

### Redirection and Heredoc
- Uses `dup` and `dup2` to handle input/output redirection.
- Creates process to handle multi-heredoc.

## Code Structure
The `main` function operates as follows:
```txt
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

ls | grep e > outfile > outfile1
ls | grep e >> outfile >> outfile1
< infile grep a | grep e > outfile

- GOOD TO KNOW :
Redirections are processed in the order they appear, from left to right. 


EXIT STATUS :
- he return status of a simple command is its exit status as provided by the waitpid function, or 128+n if the command was terminated by signal n.
- The exit status of a pipeline is the exit status of the last command in the pipeline

- The return status of and and or lists is the exit status of the last command executed in the list



echo src*/		->src/
echo src*		->src

(echo a)(echo c)
