// The "queueing" demonstration project provides a simple example of each of
// the four types of nodes.
//
// Atomic node example: queueing_node.h
// Function node example: plus_node.h
// Composite node example: two_stage_queueing_node.h
// Collection node example: parallel_queueing_node.h
//
// The atomic, composite, and collection node examples all have accompanying
// simulation nodes (e.g. queueing_simulation_node.h), which allow the nodes to
// be tested. The function node example is tested indirectly through its
// inclusion in the composite node example.

#include <examples/demo/queueing/queueing.h>

int main(int argc, const char* argv[])
{
    sydevs_examples::queueing();
    return 0;
}
