#!/bin/bash

LOCKFILE="/tmp/led.lockfile"
VERSION="1.0"
DELIMITER="#"
DESCRIPTION_COLUMN=0
REGEX_COLUMN=1

read_timeout="5"
pipe="/tmp/led.pipe"

trap "fuser $LOCKFILE > /dev/null 2>&1 || rm -f $LOCKFILE" EXIT

declare -A commands2values=(
["set-led-state"]="on or off$DELIMITER(on|off)"
["set-led-color"]="red, green or blue$DELIMITER(red|green|blue)"
["set-led-rate"]="an integer in range [0; 5]$DELIMITER[0-5]"
["get-led-state"]=""
["get-led-color"]=""
["get-led-rate"]="")

get_command_value() {
	if (( $# == 2 )) && (( $2 == $DESCRIPTION_COLUMN || $2 == $REGEX_COLUMN ));
	then
		IFS=$DELIMITER read -r -a v <<< "${commands2values[$1]}"
		echo ${v[$2]}
	fi	
}

print_usage() {
	sorted=()
	while IFS= read -rd '' key; do
	    sorted+=( "$key" )
	done < <(printf '%s\0' "${!commands2values[@]}" | sort -rz)

	for cmd in "${sorted[@]}";
	do 
		commands_list+="$cmd, "
		if [[ $cmd == set* ]];
		then
			desc=$(get_command_value $cmd $DESCRIPTION_COLUMN)
			padded_description=$(printf "%-30s" "$(get_command_value $cmd $DESCRIPTION_COLUMN)")
			values+="\n\t\t $padded_description\tfor $cmd command"
		fi
	done

	commands_list=${commands_list::-2}

	bold=$(tput bold)
	normal=$(tput sgr0)

	echo -e " LED Client. Script for controling camera's LED.\n\n Usage: led_client.sh -c command [-a argument] [-p file] [-t timeout]\n\n Parameters:\n\n $bold-c, --command$normal\tcommand to execute\n\t\tAllowed values:\n\t\t $commands_list\n\n $bold-a, --argument$normal\tvalue to set in case of 'set' command\n\t\tAllowed values:$values\n\n $bold-p, --pipe$normal\tnamed pipe for communicating with server\n\n $bold-t, --timeout$normal\ttimeout for reading from the pipe\n\n $bold-h, --help$normal\tdisplay this help and exit\n\n $bold-v, --version$normal\toutput version information and exit"

	exit 0
}

print_version() {
	echo -e " LED Client, version $VERSION"
	exit 0
}

print_error() {
	if (( $# == 1 ));
	then
		echo -e "\tError: $1"
	fi
	exit 1
}

set_read_timeout() {
	if (( $# == 1 ));
	then 
		if [[ $1 =~ ^[-0-9]+$ ]];
		then
			read_timeout=$1		
		else
			print_error "Invalid timeout: $1"
		fi
	fi
}

if (( $# == 0 ));
then
    print_usage
fi

while (( $# > 0 )); do case $1 in
	-c|--command) command=$2; shift;;
	-a|--argument) value=$2; shift;;
	-p|--pipe) pipe=$2; shift;;
	-t|--timeout) set_read_timeout $2; shift;;
	-h|--help) print_usage;;
	-v|--version) print_version;;
  	*) print_error "Unknown parameter passed: '$1'"
esac; shift; done

if [[ -z $command ]];
then
	print_error "Command is not provided"
fi

if ! [[ -v commands2values[$command] ]];
then
	print_error "Unknown command: '$command'"
fi

MESSAGE=$command

shopt -s nocasematch

if [[ $command == set* ]];
then
	if [[ -z $value ]];
	then
		print_error "Value is not provided"
	else
		if [[ $value =~ ^$(get_command_value $command $REGEX_COLUMN)$ ]];
		then
			MESSAGE+=" $value"			
		else
			print_error "Invalid value: '$value', must be $(get_command_value $command $DESCRIPTION_COLUMN)"
		fi	
	fi
fi

shopt -u nocasematch

if ! [[ -p $pipe ]];
then
	print_error "Pipe '$pipe' doesn't exist"
fi

if (( $read_timeout < 0 ));
then
	timeout_arg=""
else
	timeout_arg="-t $read_timeout"
fi
(
flock 200
echo ${MESSAGE,,} > $pipe
if read $timeout_arg response < $pipe;
then
	echo $response
else 
	print_error "Reading error. Errno: $?"
fi
) 200>$LOCKFILE

