#ifndef __SITE
#define __SITE
typedef enum {
    PANIC_SWITCH, // In case the panic switch was pressed.
    REST,         // In case the worker needs a rest.
    START_UP,     //
    RUNNING,      // Everything is fine, construction sites works.
    RESET,        //
    DEBUG         // Diagnostic Functions.
} SiteState;
#endif // __SITE
