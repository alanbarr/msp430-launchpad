#Script will leave you on GDB prompt, 
#ready to run.

if [ $# -ne 1 ]
then
    echo "Needs one argument: the .elf file"
    exit
fi

if [ ! -f $1 ]
then
    echo "$1 doesn't exist"
    exit
fi


mspdebug rf2500 gdb &
msp430-gdb $1 -x gdb_commands

