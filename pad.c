#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/fs.h>
#include <linux/uaccess.h>
 
#define DEVICE_NAME "virtual_keypad"
 
static int major_number;
static char message[256] = {0};  // Buffer to store the last command
static short size_of_message;

static ssize_t virtual_keypad_read(struct file *file, char *buffer, size_t len, loff_t *offset) {
    int error_count = 0;
    error_count = copy_to_user(buffer, message, size_of_message);
    if (error_count == 0) {
        int bytes_read = size_of_message;
        size_of_message = 0;
        return bytes_read;
    } else {
        return -EFAULT;
    }
}
 
// Write device method
static ssize_t virtual_keypad_write(struct file *file, const char *buffer, size_t length, loff_t *offset) {
    
    char input[length + 1];  // Temporary buffer to store input
    if (copy_from_user(input, buffer, length) != 0) {
        return -EFAULT;
    }
    input[length] = '\0'; // Ensure null-terminated string
 
    
    if (input[0] == '1') {  
        strcpy(message, "copy");  // Command for user space
        printk(KERN_INFO "copying message ");
    } else {
        // Handle other buttons or default case
        snprintf(message, 255, "button %c pressed", input[0]);
    }
    size_of_message = strlen(message);
 
    printk(KERN_INFO "VirtualKeypad: Received input %s, stored message: %s\n", input, message);
    return length;
}
 
static struct file_operations fops = {
    .read = virtual_keypad_read,
    .write = virtual_keypad_write,
};
 
static int __init virtual_keypad_init(void) {
    major_number = register_chrdev(0, DEVICE_NAME, &fops);
    if (major_number < 0) {
        printk(KERN_ALERT "Failed to register a major number\n");
        return major_number;
    }
    printk(KERN_INFO "Virtual keypad module loaded with major number %d\n", major_number);
    return 0;
}
 
static void __exit virtual_keypad_exit(void) {
    unregister_chrdev(major_number, DEVICE_NAME);
    printk(KERN_INFO "Virtual keypad module unloaded\n");
}
 
module_init(virtual_keypad_init);
module_exit(virtual_keypad_exit);
 
MODULE_LICENSE("GPL");
MODULE_AUTHOR("Your Name");
MODULE_DESCRIPTION("Virtual Keypad Module");