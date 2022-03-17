input=""
ctrl_c(){
	echo
	echo "Ctrl-C will not terminate $0"
}
ctrl_slash(){
	echo
	echo "Ctrl-C will not terminate $0"
}
ctrl_z(){
	echo
	echo "Ctrl-Z was pressed! $0"
}

run_script(){
while $true;
do
	trap ctrl_c INT
	trap ctrl_slash QUIT
	trap ctrl_z TSTP
	echo "Type a command:"
	read -t 30 input
	if [ $? -ne 0 ] || [ $input = "stop" ];
	then
	   echo "done"
	   exit
	fi
done
}
run_script
echo "done"
