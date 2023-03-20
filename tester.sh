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
	echo -n ""
	# input=$(echo -e "$1")
	# expected=$(echo -e "$1" | bash)
	# if [ "$input" == "$expected" ]; then
	# 	print_ok
	# 	if [ "$flag" == "-p" ]; then
	# 		echo -ne "\e[36mINPUT:\n""$input\n"
	# 		echo -ne "\e[94mEXPECTED:\n""$expected\n"
	# 		set_clear
	# 	fi
	# else
	# 	print_ko
	# 	if [ "$flag" == "-p" ]; then
	# 		echo -ne "\e[36mINPUT:\n""$input\n"
	# 		echo -ne "\e[94mEXPECTED:\n""$expected\n"
	# 		set_clear
	# 	fi
	# fi
	# error_expected=$(echo echo "$?" | bash)
	# echo ------
	# if [ "$error_intput" == "$error_expected" ]; then
	# 	print_ok
	# 	if [ "$flag" == "-p" ]; then
	# 		echo -ne "\e[36mEINPUT:\n""$error_intput\n"
	# 		echo -ne "\e[94mEEXPECTED:\n""$error_expected\n"
	# 		set_clear
	# 	fi
	# else
	# 	print_ko
	# 	if [ "$flag" == "-p" ]; then
	# 		echo -ne "\e[36mINPUT:\n""$error_intput\n"
	# 		echo -ne "\e[94mEXPECTED:\n""$error_expected\n"
	# 		set_clear
	# 	fi
	# fi
}

test()
{
	command="$1"
	# input=$(echo -en $command | ./minishell)
	input=$(echo -en $command | valgrind --error-exitcode=500 --log-file="/dev/null" ./minishell)
	error_input=$(echo $?)
	if [ "$flag" == "-p" ] || [ "$flag" == "-p2" ]; then
		set_yellow
		echo -n "Testing : \`$command\` "
	fi
	if [ $error_input == "$2" ]; then
		print_ok
		if [ "$flag" == "-p2" ]; then
			echo -ne "\e[36mINPUT   : $error_input\n"
			echo -ne "\e[94mEXPECTED: $2\n"
		fi
	else
		print_ko
		if [ "$flag" == "-p2" ]; then
			echo -ne "\e[36mINPUT   : $error_input\n"
			echo -ne "\e[94mEXPECTED: $2\n"
		fi
	fi
	set_clear
}

parsing_tests()
{
	test "\n" 0
	test " " 0
	test "           " 0
	test "echo a" 0
	test "\"" 2
	test "'" 2
	test "\"\"" 0
	test "''" 0
	test "echo a && echo b" 0
	test "&& echo b" 2
	test "&&" 2
	test "&|&" 0
	test "echo a|" 2
	test "|echo b" 2
	test "&|echo a" 0
	test "echo a|&" 0
	test "'&&'" 0
	test "echo a &&| echo b" 2
	test "  echo   	a		&&	  echo  b      " 0
	test "echo a &&|| echo b" 2
	test "echo a |&& echo b" 2
	test "echo a ||&& echo b" 2
	test "echo a&&echo b" 0
	test "echo a&&||echo b" 2
	test "echo a|echo c" 0
	test "echo a| |echo b" 2
	test "echo a|echo b|echo c" 0
	test "echo a|||echo b" 2
	test "echo a||||echo b" 2
	test "echo a||\"||\"echo b" 0
	test " && " 2
	test "'' && ''" 0
	test "()" 2
	test "'()'" 0
	test "echo a ()" 2
	test "()echo a" 2
	test "(echo a)" 0
	test "((((echo a))))" 0
	test "(((echo a))))" 2
	test "((((echo a)))" 2
	test "(echo a()()())" 2
	test "((((echo a)&&)))" 2
	test "(((&&(echo a))))" 2
	test "(echo a) && (echo b)" 0
	test "((((echo a)&&echo b)))" 0
	test "((echo a)\"&&\")" 0
	test "((((echo a) && echo b) || (echo c)))" 0
	test "(echo a && echo b) echo c" 2
	test "echo a (echo b && echo c)" 2
	test "echo a \"(echo b && echo c)\"" 0
	test "(echo a) && \"(echo b)\"" 0
	test "\"(echo a)\" && (echo b)" 0
}

flag=""

if [ $# -eq 0 ]; then
	prompt="$1"
	parsing_tests
elif [ "$1" == "-p" ]; then
	flag="-p"
	parsing_tests
elif [ "$1" == "-p2" ]; then
	flag="-p2"
	parsing_tests
else
	echo "Usage: $0 prompt [-p]"
	exit 1
fi
