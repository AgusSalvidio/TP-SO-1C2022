#include "kernel_console_message_handler.h"
#include "kernel_process_image.h"

void handle_console_message(t_console_message *console_message) {
    create_process_image(console_message);
}