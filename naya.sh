#!/bin/bash

DEVICE="/dev/virtual_keypad"

if [[ ! -e "$DEVICE" ]]; then
    echo "Device $DEVICE not found!"
    exit 1
fi

# Read from the device file and capture the output into a variable
output=$(cat "$DEVICE")

# Use the captured output for further processing
echo "Output from device file: $output"

# Example: Check if the output contains a specific string
if [[ $output == *"copy"* ]]; then
    echo "The output contains 'copy'"
else
    echo "The output does not contain 'copy'"
fi

