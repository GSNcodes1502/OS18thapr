#include <gtk/gtk.h>
#include<fcntl.h>
#include<unistd.h>

GtkTextBuffer *clipboard_buffer = NULL;

// Callback function for numeric keypad buttons
void numeric_button_clicked(GtkWidget *widget, gpointer data)
{
    const gchar *button_label = gtk_button_get_label(GTK_BUTTON(widget));
    g_print("Numeric button clicked: %s\n", button_label);

    // Get the text buffer from the data pointer
    GtkTextBuffer *buffer = gtk_text_view_get_buffer(GTK_TEXT_VIEW(data));

    // Check if the buffer is valid
    if (!GTK_IS_TEXT_BUFFER(buffer)) {
        g_printerr("Error: Invalid GtkTextBuffer\n");
        return;
    }

    // Get the end iterator of the buffer
    GtkTextIter end_iter;
    gtk_text_buffer_get_end_iter(buffer, &end_iter);

    // Insert the button label into the buffer at the end
    gtk_text_buffer_insert(buffer, &end_iter, button_label, -1);
}

// Callback function for special buttons
void special_button_clicked(GtkWidget *widget, gpointer data)
{
    const gchar *button_label = gtk_button_get_label(GTK_BUTTON(widget));


    g_print("Special button clicked: %s\n", button_label);

    // Open the device file for writing
    int fd = open("/dev/virtual_keypad", O_WRONLY);
    if (fd < 0) {
        perror("Error opening device file");
        return;
    }

    // Write the button label to the device file
    g_print("Text: %s",button_label);
    ssize_t bytes_written = write(fd, button_label, strlen(button_label));
    if (bytes_written < 0) {
        perror("Error writing to device file");
    }

    // Close the device file
    close(fd);
}

// Callback function for copy button
void copy_button_clicked(GtkWidget *widget, GtkTextView *text_view)
{
    GtkTextBuffer *buffer = gtk_text_view_get_buffer(text_view);
    GtkTextIter start, end;
    gtk_text_buffer_get_selection_bounds(buffer, &start, &end);
    gchar *text = gtk_text_buffer_get_text(buffer, &start, &end, FALSE);
    if (clipboard_buffer != NULL) {
        g_object_unref(clipboard_buffer);
    }
    clipboard_buffer = gtk_text_buffer_new(NULL);
    gtk_text_buffer_set_text(clipboard_buffer, text, -1);
    g_free(text);
}

// Callback function for paste button
void paste_button_clicked(GtkWidget *widget, GtkTextView *text_view)
{
    if (clipboard_buffer != NULL) {
        GtkTextBuffer *buffer = gtk_text_view_get_buffer(text_view);
        GtkTextIter iter;
        gtk_text_buffer_get_end_iter(buffer, &iter);
        
        GtkTextIter start;
        gtk_text_buffer_get_start_iter(clipboard_buffer, &start);
        GtkTextIter end;
        gtk_text_buffer_get_end_iter(clipboard_buffer, &end);
        
        gchar *text = gtk_text_buffer_get_text(clipboard_buffer, &start, &end, FALSE);
        gtk_text_buffer_insert(buffer, &iter, text, -1);
        g_free(text);
    }
}

int main(int argc, char *argv[])
{
    GtkWidget *window;
    GtkWidget *grid;
    GtkWidget *button;
    GtkWidget *text_view;

    gtk_init(&argc, &argv);

    window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_title(GTK_WINDOW(window), "Virtual Keypad");

    g_signal_connect(window, "destroy", G_CALLBACK(gtk_main_quit), NULL);

    grid = gtk_grid_new();
    gtk_container_add(GTK_CONTAINER(window), grid);

    // Add numeric keypad buttons
    text_view = gtk_text_view_new();  // Create the text view first
    for (int i = 1; i <= 9; ++i) {
        char button_label[2];  // Buffer for button label
        sprintf(button_label, "%d", i);  // Convert integer to string
        button = gtk_button_new_with_label(button_label);
        g_signal_connect(button, "clicked", G_CALLBACK(numeric_button_clicked), text_view);
        gtk_grid_attach(GTK_GRID(grid), button, (i - 1) % 3, (i - 1) / 3, 1, 1);
    }

    // Add text view to display inputted text
    gtk_grid_attach(GTK_GRID(grid), text_view, 0, 3, 3, 1); // Span 3 columns for the text view

    // Add special buttons
    button = gtk_button_new_with_label("1");
    g_signal_connect(button, "clicked", G_CALLBACK(special_button_clicked), NULL);
    gtk_grid_attach(GTK_GRID(grid), button, 3, 0, 1, 1);

    button = gtk_button_new_with_label("2");
    g_signal_connect(button, "clicked", G_CALLBACK(special_button_clicked), NULL);
    gtk_grid_attach(GTK_GRID(grid), button, 3, 1, 1, 1);

    // Add copy button
    button = gtk_button_new_with_label("Copy");
    g_signal_connect(button, "clicked", G_CALLBACK(copy_button_clicked), text_view);
    gtk_grid_attach(GTK_GRID(grid), button, 3, 2, 1, 1);

    // Add paste button
    button = gtk_button_new_with_label("Paste");
    g_signal_connect(button, "clicked", G_CALLBACK(paste_button_clicked), text_view);
    gtk_grid_attach(GTK_GRID(grid), button, 3, 3, 1, 1);

    gtk_widget_show_all(window);

    gtk_main();

    return 0;
}
