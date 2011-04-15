#include <list>
#include <algorithm>

#include "edsim_naive.h"

typedef std::list<Event> TEventQueue;

TEventQueue g_event_queue;
double g_current_time;

void enqueue(const Event& event)
{
    TEventQueue::iterator itr = 
		std::upper_bound(g_event_queue.begin(), g_event_queue.end(), event);
    g_event_queue.insert(itr, event);
}

void run_sim(double end_time)
{
	while(!g_event_queue.empty() && g_current_time<end_time){
		Event event = g_event_queue.front();
		g_event_queue.pop_front();
		g_current_time = event.m_occur_time;
		(*event.m_handler)();
	}
}
