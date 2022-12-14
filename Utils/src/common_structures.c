#include <common_structures.h>
#include <stdlib.h>
#include <general_logs.h>
#include <garbage_collector.h>
#include <signal.h>
#include <commons/string.h>
#include <unistd.h>
#include <commons/temporal.h>
#include <time.h>
#include <pthread_wrapper.h>

void handler(){
    char* goodbye_message = "\nOK. Me vas a matar. :( Pero antes voy a liberar tooodaaa la memoria que utilicé! :D\n\n";
    write(STDOUT_FILENO, goodbye_message, string_length(goodbye_message));
    free_system();
}

void sigpipe_handler(){
    log_syscall_error_with_errno_description("Se produjo una señal SIGPIPE");
}

void handle_signal(int signal_number, void (*handler_function) ()){

    struct sigaction signal_action = {.sa_handler = handler_function};

    if(sigaction(signal_number, &signal_action, NULL) == -1){
        log_syscall_error_with_errno_description("Error en la creación de una acción de signals");
        free_system();
    }
}

void initialize_signal_handler(){
    handle_signal(SIGINT, handler);
    handle_signal(SIGTERM, handler);
    handle_signal(SIGPIPE, sigpipe_handler);
}

void* safe_malloc(size_t size){
    void* pointer = malloc(size);
    if(pointer == NULL && size != 0){
        log_syscall_error("Error al guardar espacio de memoria con safe_malloc");
        free_system();
    }
    return pointer;
}

unsigned long long hash(char* value){
    unsigned long long hash = 0;
    int value_length = string_length(value);

    for(int index = 0; index < value_length; index++) {
        unsigned char c = value[index];
        hash += c;
        hash += (hash << 10);
        hash ^= (hash >> 6);
    }
    hash += (hash << 3);
    hash ^= (hash >> 11);
    hash += (hash << 15);

    return hash;
}

char* process_description_for(char* process_name, t_list* strings_to_hash){
    unsigned long long hash_sum = 0;

    for(int i = 0; i < list_size(strings_to_hash); i++){
        char* string = list_get(strings_to_hash, i);
        hash_sum += hash(string);
    }

    return string_from_format("%s-%llu", process_name, hash_sum);
}

void free_char_array(char** char_array){
    //itero hasta que lo que apunta char array sea null, en cuyo caso null evalua como false
    for (int i = 0; char_array[i]; i++ ){
        free( char_array[i] );
    }
    free( char_array );
}

void safe_sem_initialize(sem_t* semaphore){

    if(sem_init(semaphore, false, 0) == -1){
        log_syscall_error_with_errno_description("Error al inicializar un semáforo");
        free_system();
    }
}

void safe_sem_initialize_with_value(sem_t* semaphore, uint32_t value) {
    if(sem_init(semaphore, false, value) == -1){
        log_syscall_error_with_errno_description("Error al inicializar un semáforo");
        free_system();
    }
}

void safe_sem_wait(sem_t* semaphore){

    if(sem_wait(semaphore) == -1){
        log_syscall_error_with_errno_description("Error al intentar tomar el lock del semáforo");
        free_system();
    }
}

void safe_sem_post(sem_t* semaphore){

    if(sem_post(semaphore) == -1){
        log_syscall_error_with_errno_description("Error al intentar liberar el lock del semáforo");
        free_system();
    }
}

void safe_sem_destroy(sem_t* semaphore){

    safe_sem_post(semaphore); // Si el lock está tomado, lo libero para poder destruirlo

    if(sem_destroy(semaphore) == -1){
        log_syscall_error_with_errno_description("Error al destruir un semáforo");
        free_system();
    }
}

uint32_t current_time_in_milliseconds(){

    char* time = temporal_get_string_time("%H:%M:%S:%MS");
    char** time_splitted = string_split(time, ":");
    char* hours = time_splitted[0];
    char* minutes = time_splitted[1];
    char* seconds = time_splitted[2];
    int milliseconds = atoi(time_splitted[3]);

    int hours_in_milliseconds = atoi(hours) * 3600000;
    int minutes_in_milliseconds = atoi(minutes) * 60000;
    int seconds_in_milliseconds = atoi(seconds) * 1000;

    free(time);
    free_char_array(time_splitted);

    return hours_in_milliseconds + minutes_in_milliseconds + seconds_in_milliseconds + milliseconds;
}

void assert_only_one_in(t_list* self){
    if(list_size(self) != 1){
        log_expected_to_have_only_one_element_error();
        free_system();
    }
}

void sleep_for(int reconnection_time_in_seconds){
    struct timespec deadline;
    deadline.tv_sec = reconnection_time_in_seconds;
    deadline.tv_nsec = 0;
    if(clock_nanosleep(CLOCK_MONOTONIC, 0, &deadline, NULL) != 0){
        log_syscall_error("Se produjo un error al intentar dormir el hilo.");
        free_system();
    }
}

int minimum_integer_between(int number, int another_number){
    return number < another_number ? number : another_number;
}

void* free_system_debugging_thread(void* seconds){

    int cast_seconds = (*(int*) seconds);
    free(seconds);
    sleep_for(cast_seconds);
    free_system();
    return NULL;
}

void free_system_debugging_thread_alive_for(int seconds){

    int* seconds_pointer = safe_malloc(sizeof(int));
    *seconds_pointer = seconds;

    default_safe_thread_create(free_system_debugging_thread, (void*) seconds_pointer);
}