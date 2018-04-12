// This example simulates a network of queues at a customs area
// using components (sources, queues, servers, sinks) that are 
// robust with respect to tie-breaking. That is, if a queue feeds
// into a server, and if both components undergo an event at the
// same time, the simulation should progress error-free regardless
// of which component's event is processed first.

#include <examples/research/customs/customs.h>

int main(int argc, const char* argv[])
{
    sydevs_examples::customs();
    return 0;
}
