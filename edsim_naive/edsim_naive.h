#ifndef __EDSIM_NAIVE_H
#define __EDSIM_NAIVE_H


//Implementation of Event
typedef void (*TEventHandler) (void);

struct Event{
    Event(double occur_time, TEventHandler handler)
    : m_occur_time(occur_time)
    , m_handler(handler)
    {}

    bool operator < (const Event& rhs) const{
        return m_occur_time < rhs.m_occur_time;
    }

    double m_occur_time;
    TEventHandler m_handler;
};

void enqueue(const Event& event);
void run_sim(double end_time);

#endif
