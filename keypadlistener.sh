#!/bin/bash

DEVICE="/dev/virtual_keypad"
BYTES_TO_READ=4

if [[ ! -e "$DEVICE" ]]; then
    echo "Device $DEVICE not found!"
    exit 1
fi

exec 3<"$DEVICE"

while true; do
    if read -u 3 -N $BYTES_TO_READ cmd; then
        
        case "$cmd" in
            "copy")
                echo "Received command: copy '$cmd'"
                echo "Performing copy operation..."
                if [ -e "hello.txt" ]; then
                   
                    echo "hello i am here" >> hello.txt
                else
                    t
                    echo "hello i am here" > hello.txt
                fi
             
                ;;
            *)
                echo "Received command: $cmd"
                
                
                ;;
        esac
    else
        echo "Failed to read from device"
        sleep 1
    fi
done
