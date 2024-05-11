#include <sys/stat.h>
#include "hal.h"
extern void _default_err_handler(void) __attribute__((noreturn));

int _close (int file) {
    uart_send("\n***E: Calling close***\n");
    _default_err_handler();
}

int _lseek (int file, int ptr, int dir) {
    uart_send("\n***E: Calling lseek***\n");
    _default_err_handler();
}

int _write (int file, char *ptr, int len) {
    uart_send("\n***E: Calling write***\n");
    _default_err_handler();
}

int _read (int file, char *ptr, int len) {
    uart_send("\n***E: Calling read***\n");
    _default_err_handler();
}

int _sbrk (int incr) {
    uart_send("\n***E: Calling sbrk***\n");
    _default_err_handler();
}
