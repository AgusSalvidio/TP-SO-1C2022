#ifndef KISS_KERNEL_EVENT_H
#define KISS_KERNEL_EVENT_H

enum Event_types{
    PROCESS_READY_TO_EXECUTE,
    PROCESS_SWITCH,
    SEND_INTERRUPTION_SIGNAL,
    PROCESS_BLOCKED
};

typedef struct Subscription{
    int event_type;
    void (*callback_function) (void* event_argument);
}t_subscription;

void initialize_event_notifier();
void subscribe_to_event_doing(int event_type, void (*callback_function) (void* event_argument));
void notify(int event_type);
void notify_with_argument(int event_type, void* event_argument);
void free_event_notifier();

#endif //KISS_KERNEL_EVENT_H
