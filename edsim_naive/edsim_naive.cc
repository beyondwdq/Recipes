#include <list>
#include <algorithm>

#include "edsim_naive.h"

typedef std::list<Event> TEventList;

TEventList g_event_list;
double g_current_time;

void enqueue(const Event& event)
{
    TEventList::iterator itr = std::upper_bound(g_event_list.begin(), g_event_list.end(), event);
    g_event_list.insert(itr, event);
}

void run_sim(double end_time)
{
	while(!g_event_list.empty() && g_current_time<end_time){
		Event event = g_event_list.front();
		g_event_list.pop_front();
		g_current_time = event.m_occur_time;
		(*event.m_handler)();
	}
}
