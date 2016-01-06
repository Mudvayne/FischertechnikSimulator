#ifndef __SITE
#define __SITE
typedef enum {
    PANIC_SWITCH, 	// Panic switch was pressed, or sensor errors occured.
    DIAGNOSTIC,         // Diagnostic Functions.
    START,     		// State after Controller has been reset.
    RUNNING,      	// Operational mode.
    REST,         	// Worker needs a rest.
    STOP		// Worker is about to shutdown construction site.
} SiteState;
#endif // __SITE
