typedef enum {
    E_STATE_NONE = -1,
    E_STATE_CONTROL,
    E_STATE_GET_INPUT,
    E_STATE_DISPLAY,
    E_STATE_EXIT
} E_STATE_PROCESS;

static E_STATE_PROCESS state;

E_STATE_PROCESS ctrl_initState();