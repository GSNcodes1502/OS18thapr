#!/bin/bash

DEVICE_FILE="/dev/virtual_keypad"
BYTES_TO_READ=4

# Check if the device file exists
if [ ! -e "$DEVICE_FILE" ]; then
    echo "Device file $DEVICE_FILE not found."
    exit 1
fi

# Open the device file for reading
exec 3<"$DEVICE_FILE"

# Read 4 bytes of data from the device file
read -u 3 -N $BYTES_TO_READ data

# Display the read data
echo "Data read from the device: $data"

# Close the file descriptor
exec 3<&-

# Exit
exit 0
