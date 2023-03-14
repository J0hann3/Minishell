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
	expected=$(echo -e "$1" | bash)
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
			echo -ne "\e[36mINPUT:\n""$input\n"
			echo -ne "\e[94mEXPECTED:\n""$expected\n"
			set_clear
		fi
	fi
	error_expected=$(echo echo "$?" | bash)
	echo ------
	if [ "$error_intput" == "$error_expected" ]; then
		print_ok
		if [ "$flag" == "-p" ]; then
			echo -ne "\e[36mEINPUT:\n""$error_intput\n"
			echo -ne "\e[94mEEXPECTED:\n""$error_expected\n"
			set_clear
		fi
	else
		print_ko
		if [ "$flag" == "-p" ]; then
			echo -ne "\e[36mINPUT:\n""$error_intput\n"
			echo -ne "\e[94mEXPECTED:\n""$error_expected\n"
			set_clear
		fi
	fi
}

test()
{
	set_yellow
	command="$1"
	input=$(echo -en $command | ./minishell)
	error_intput=$(echo $?)
	echo "Testing : '$command'"
	set_clear
	test_output "$input"
}

parsing_tests()
{
	test "\n"
	test "\""
	test "test"
}

flag=""
error_intput=""

if [ $# -eq 0 ]; then
	prompt="$1"
	parsing_tests
elif [ "$1" == "-p" ]; then
	prompt="$1"
	flag="-p"
	parsing_tests
else
	echo "Usage: $0 prompt [-p]"
	exit 1
fi
