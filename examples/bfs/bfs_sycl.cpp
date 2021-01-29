#include <stdio.h>
#include <string.h>
#include <math.h>
#include <stdlib.h>
#include <sys/time.h>

#include <CL/sycl.hpp>

//#define NUM_THREAD 4
#define OPEN

using namespace cl::sycl;

FILE *fp;

//Structure to hold a node information
struct Node {
  int starting;
  int no_of_edges;
};

long get_time() {
  struct timeval tv;
  gettimeofday(&tv, NULL);
  return tv.tv_sec * 1000 + tv.tv_usec / 1000;
}

void BFSGraph(int argc, char **argv);

void Usage(int argc, char **argv) {

  fprintf(stderr, "Usage: %s <num_threads> <input_file>\n", argv[0]);

}
////////////////////////////////////////////////////////////////////////////////
// Main Program
////////////////////////////////////////////////////////////////////////////////
int main(int argc, char **argv) {
  BFSGraph(argc, argv);
}

////////////////////////////////////////////////////////////////////////////////
//Apply BFS on a Graph using CUDA
////////////////////////////////////////////////////////////////////////////////
void BFSGraph(int argc, char **argv) {
  int no_of_nodes = 0;
  int edge_list_size = 0;
  char *input_f;
  int num_omp_threads;

  if (argc != 3) {
    Usage(argc, argv);
    exit(0);
  }

  num_omp_threads = atoi(argv[1]);
  input_f = argv[2];

  printf("Reading File\n");
  //Read in Graph from a file
  fp = fopen(input_f, "r");
  if (!fp) {
    printf("Error Reading graph file\n");
    return;
  }

  long start_time = get_time();

  int source = 0;

  fscanf(fp, "%d", &no_of_nodes);

  // allocate host memory

  Node *h_graph_nodes = (Node *) malloc(sizeof(Node) * no_of_nodes);
  bool *h_graph_mask = (bool *) malloc(sizeof(bool) * no_of_nodes);
  bool *h_updating_graph_mask = (bool *) malloc(sizeof(bool) * no_of_nodes);
  bool *h_graph_visited = (bool *) malloc(sizeof(bool) * no_of_nodes);

  buffer<Node> h_graph_nodes_buf(h_graph_nodes, range<1>(no_of_nodes));
  buffer<bool> h_graph_mask_buf(h_graph_mask, range<1>(no_of_nodes));
  buffer<bool> h_updating_graph_mask_buf(h_updating_graph_mask, range<1>(no_of_nodes));
  buffer<bool> h_graph_visited_buf(h_graph_visited, range<1>(no_of_nodes));

  int start, edgeno;
  // initalize the memory
  for (unsigned int i = 0; i < no_of_nodes; i++) {
    fscanf(fp, "%d %d", &start, &edgeno);
    h_graph_nodes[i].starting = start;
    h_graph_nodes[i].no_of_edges = edgeno;
    h_graph_mask[i] = false;
    h_updating_graph_mask[i] = false;
    h_graph_visited[i] = false;
  }

  //read the source node from the file
  fscanf(fp, "%d", &source);
  // source=0; //tesing code line

  //set the source node as true in the mask
  h_graph_mask[source] = true;
  h_graph_visited[source] = true;

  fscanf(fp, "%d", &edge_list_size);

  int id, cost;
  int *h_graph_edges = (int *) malloc(sizeof(int) * edge_list_size);

  buffer<int> h_graph_edges_buf(h_graph_edges, range<1>(edge_list_size));

  for (int i = 0; i < edge_list_size; i++) {
    fscanf(fp, "%d", &id);
    fscanf(fp, "%d", &cost);
    h_graph_edges[i] = id;
  }

  if (fp)
    fclose(fp);


  // allocate mem for the result on host side
  int *h_cost = (int *) malloc(sizeof(int) * no_of_nodes);

  buffer<int> h_cost_buf(h_cost, range<1>(no_of_nodes));

  for (int i = 0; i < no_of_nodes; i++)
    h_cost[i] = -1;
  h_cost[source] = 0;

  printf("Start traversing the tree\n");

  long kernel_start_time = get_time();

  ve_queue q;

  buffer<int> n_buf(&no_of_nodes, range<1>(1));

  q.submit([&](handler &cgh) {
    auto h_graph_mask_buf_access = h_graph_mask_buf.get_access<access::mode::read_write>(cgh);
    auto h_graph_visited_buf_access = h_graph_visited_buf.get_access<access::mode::read_write>(cgh);
    auto h_updating_graph_mask_buf_access = h_updating_graph_mask_buf.get_access<access::mode::read_write>(cgh);
    auto h_graph_nodes_buf_access = h_graph_nodes_buf.get_access<access::mode::read>(cgh);

    auto h_graph_edges_buf_access = h_graph_edges_buf.get_access<access::mode::read>(cgh);
    auto h_cost_buf_access = h_cost_buf.get_access<access::mode::read_write>(cgh);

    auto no_of_nodes_access = n_buf.get_access<access::mode::read>(cgh);

    cgh.single_task<class traversing>([=]() {
      int k = 0;
      bool stop;
      do {
        //if no thread changes this value then the loop stops
        stop = false;
        for (int tid = 0; tid < no_of_nodes_access[0]; tid++) {
          if (h_graph_mask_buf_access[tid] == true) {
            h_graph_mask_buf_access[tid] = false;
            for (int i = h_graph_nodes_buf_access[tid].starting;
                 i < (h_graph_nodes_buf_access[tid].no_of_edges + h_graph_nodes_buf_access[tid].starting);
                 i++) {
              int id = h_graph_edges_buf_access[i];
              if (!h_graph_visited_buf_access[id]) {
                h_cost_buf_access[id] = h_cost_buf_access[tid] + 1;
                h_updating_graph_mask_buf_access[id] = true;
              }
            }
          }
        }

        for (int tid = 0; tid < no_of_nodes_access[0]; tid++) {
          if (h_updating_graph_mask_buf_access[tid] == true) {
            h_graph_mask_buf_access[tid] = true;
            h_graph_visited_buf_access[tid] = true;
            stop = true;
            h_updating_graph_mask_buf_access[tid] = false;
          }
        }
        k++;
      } while (stop);

    });

  });

  q.wait();

  long end_time = get_time();

  printf("Total cost: %ld ms\n", end_time - start_time);
  printf("Kernel cost: %ld ms\n", end_time - kernel_start_time);

  //Store the result into a file
  FILE * fpo = fopen("result.txt", "w");
  for (int i = 0; i < no_of_nodes; i++)
    fprintf(fpo, "%d) cost:%d\n", i, h_cost[i]);
  fclose(fpo);
  printf("Result stored in result.txt\n");


  // cleanup memory
  free(h_graph_nodes);
  free(h_graph_edges);
  free(h_graph_mask);
  free(h_updating_graph_mask);
  free(h_graph_visited);
  free(h_cost);

}

