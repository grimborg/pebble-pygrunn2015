#include <stdlib.h>
#include "schedule.h"

Schedule schedule = {.num_slots = 15, .slots = {
    {.time = "09:00-09:50", .title = "Reception", .is_common = 1},
    {.time = "09:50-10:00", .title = "Welcome speech", .is_common = 1},
    {.time = "10:15-10:45", .is_common = 0, .num_sessions = 3, .sessions = {
        {.start = "10:15", .title = "Leveraging Procedural Knowledge", .presenter = "K Rain Leander", .room = 2},
        {.start = "10:15", .title = "Orchestrating Python projects using CoreOS", .presenter = "Òscar Vilaplana", .room = 3},
        {.start = "10:15", .title = "Leveraging RPython For Efficient Real-Time Audio Processing", .presenter = "Emil Loer", .room = 4}
    }},
    {.time = "11:00-11:30", .is_common = 0, .num_sessions = 3, .sessions = {
        {.start = "11:00", .title = "Blosc and friends", .presenter = "Valentin Haenel", .room = 1},
        {.start = "11:00", .title = "Python in Processes, data, contracts", .presenter = "Herman Balsters", .room = 2},
        {.start = "11:00", .title = "IPython and MongoDB as big data scratchpads", .presenter = "Jens de Smit", .room = 4}
    }},
    {.time = "11:45-12:15", .is_common = 0, .num_sessions = 3, .sessions =  {
        {.start = "11:45", .title = "Python, WebRTC and you", .presenter = "Saúl Ibarra Corretgé", .room = 1},
        {.start = "11:45", .title = "Contract languages based on Python", .presenter = "Henk Doornbos", .room = 2},
        {.start = "15:30", .title = "Tornado and IO in Python 3", .presenter = "Sylvain Viollon", .room = 3}

    }},
    {.time = "12:15-13:00", .title = "Lunch", .is_common = 1},
    {.time = "13:00-13:30", .is_common = 0, .num_sessions = 3, .sessions = {
        {.start = "13:00", .title = "GIS: Solutions and architecture trade-offs", .presenter = "Ivor Bosloper", .room = 1},
        {.start = "13:00", .title = "Web11: Implementing a redecentralized Internet", .presenter = "", .room = 2},
        {.start = "13:00", .title = "Reliable distributed task scheduling", .presenter = "Niels Hageman", .room = 4}
    }},
    {.time = "13:45-12:15", .is_common = 0, .num_sessions = 3, .sessions = {
        {.start = "13:45", .title = "Web11: Implementing a redecentralized Internet", .presenter = "", .room = 2},
        {.start = "13:45", .title = "Implement Gmail api in our CRM system", .presenter = "Bob Voorneveld", .room = 3},
        {.start = "13:45", .title = "Data acquisition with Vlermv database", .presenter = "Thomas Levine", .room = 4}
    }},
    {.time = "14:30-15:00", .is_common = 0, .num_sessions = 3, .sessions = {
        {.start = "14:30", .title = "Towards a web framework for distributed apps", .presenter = "Laurence de Jong", .room = 1},
        {.start = "14:30", .title = "Computational neuroscience", .presenter = "Ot de Wiljes", .room = 2},
        {.start = "14:30", .title = "Generators", .presenter = "Erik Groeneveld", .room = 3}
    }},
    {.time = "15:00-15:30", .title = "Break", .is_common = 1},
    {.time = "15:30-16:00", .is_common = 0, .num_sessions = 2, .sessions = {
        {.start = "15:30", .title = "Advanced REST APIs", .presenter = "Lars de Ridder", .room = 1},
        {.start = "15:30", .title = "Microservices: The decentralized future of Python and friends?", .presenter = "Berco Beute", .room = 2}
    }},
    {.time = "16:15-16:45", .is_common = 0, .num_sessions = 1, .sessions = {
        {.start = "16:15", .title = "ElasticSearch", .presenter = "Boaz Leskes", .room = 2}
    }},
    {.time = "17:00-17:30", .is_common = 0, .num_sessions = 1, .sessions = {
        {.start = "17:00", .title = "ZeroMQ", .presenter = "Pieter Hintjens", .room = 2}
    }},
    {.time = "17:45-18:30", .title = "Drinks", .is_common = 1},
    {.time = "19:00-late", .title = "Speakers Dinner", .is_common = 1}
}};

void init_schedule() {
    Session *prev = NULL;
    for(int ts_idx=0; ts_idx<schedule.num_slots; ts_idx++) {
        TimeSlot *ts = &schedule.slots[ts_idx];
        if (ts->is_common) {
            continue;
        }
        for(int s_idx=0; s_idx<ts->num_sessions; s_idx++) {
            Session *s = &ts->sessions[s_idx];
            s->prev = prev;
            s->next = NULL;
            if (prev) {
                prev->next = s;
            }
            prev = s;
        }
    }
}
