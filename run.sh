source ./build.sh
# Check if an argument is provided
if [ -n "$1" ]; then
    echo "trace_${1}.txt"
    ./bin/interrupts ./testcases/trace_${1}.txt vector_table.txt device_table.txt 
else
    echo "trace.txt"
    ./bin/interrupts trace.txt vector_table.txt device_table.txt 
fi
