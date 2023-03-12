#!/bin/bash

set_yellow()
{
	echo -ne "\e[33m"
}

set_clear()
{
	echo -ne "\e[0m"
}

print_ok()
{
	echo -ne "\e[32m"
	echo -e "-> OK"
	set_clear
}

print_ko()
{
	echo -ne "\e[31m"
	echo -e "-> KO"
	set_clear
}

test_output()
{
	input=$(echo -e "$1")
	expected=$(echo -e "$2")
	if [ "$input" == "$expected" ]; then
		print_ok
		if [ "$flag" == "-p" ]; then
			echo -ne "\e[36mINPUT:\n""$input\n"
			echo -ne "\e[94mEXPECTED:\n""$expected\n"
			set_clear
		fi
	else
		print_ko
		if [ "$flag" == "-p" ]; then
			echo -ne "\e[36mINPUT:\n""$1\n"
			echo -ne "\e[94mEXPECTED:\n""$2\n"
			set_clear
		fi
	fi
}

test()
{
	set_yellow
	command="$1"
	input=$(echo -en $command | ./minishell)
	echo "Testing : '$command'"
	set_clear
	test_output "$input" "$prompt$2\n$prompt"
}

parsing_tests()
{
	test "\n" ""
	test "\"" "\""
	test "test" "test"
}

flag=""

if [ $# -lt 1 ]; then
	echo "Usage: $0 prompt [-p]"
	exit 1
elif [ $# -eq 1 ]; then
	prompt="$1"
	parsing_tests
elif [ "$2" == "-p" ]; then
	prompt="$1"
	flag="-p"
	parsing_tests
else
	echo "Usage: $0 prompt [-p]"
	exit 1
fi
