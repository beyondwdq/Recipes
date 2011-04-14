#include <list>
#include <algorithm>

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

typedef std::list<Event> TEventList;
//End of implementation of Event

//global variables for simulation
TEventList g_event_list;
double g_current_time;

void enqueue(const Event& event)
{
    TEventList::iterator itr = std::upper_bound(g_event_list.begin(), g_event_list.end(), event);
    g_event_list.insert(itr, event);
}

int main(int argc, const char *argv[])
{
    return 0;
}
