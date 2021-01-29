#include <math.h>
#include <stdio.h>
#include <stdbool.h>

typedef struct {
  int starting;
  int no_of_edges;
} Node;

int kernel_func(Node *h_graph_nodes,
                bool *h_graph_mask,
                bool *h_updating_graph_mask,
                bool *h_graph_visited,
                int *h_graph_edges,
                int *h_cost,
                int no_of_nodes) {
  bool stop;
  do {
    stop = false;

    for (int tid = 0; tid < no_of_nodes; tid++) {
      if (h_graph_mask[tid] == true) {
        h_graph_mask[tid] = false;
        for (int i = h_graph_nodes[tid].starting; i < (h_graph_nodes[tid].no_of_edges + h_graph_nodes[tid].starting);
             i++) {
          int id = h_graph_edges[i];
          if (!h_graph_visited[id]) {
            h_cost[id] = h_cost[tid] + 1;
            h_updating_graph_mask[id] = true;
          }
        }
      }
    }

    for (int tid = 0; tid < no_of_nodes; tid++) {
      if (h_updating_graph_mask[tid] == true) {
        h_graph_mask[tid] = true;
        h_graph_visited[tid] = true;
        stop = true;
        h_updating_graph_mask[tid] = false;
      }
    }
  } while (stop);

}