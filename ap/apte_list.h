typedef struct _apte_handler{
    struct _apte_handler *next;
    char *name;
    int(*handler)();
}apte_handler;

extern apte_handler *apte_handler_list;
void apte_list_init(void);
apte_handler *apte_handler_register(char *name, int(*func)());