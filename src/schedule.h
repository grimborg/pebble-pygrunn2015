typedef struct Session {
    char *start;
    char *title;
    char *presenter;
    int room;
    struct Session *next;
    struct Session *prev;
} Session;

typedef struct {
    char *time;
    int is_common;
    char *title;
    int num_sessions;
    Session sessions[3];
} TimeSlot;


typedef struct {
    int num_slots;
    TimeSlot slots[];
} Schedule;

Schedule schedule;

void init_schedule();
