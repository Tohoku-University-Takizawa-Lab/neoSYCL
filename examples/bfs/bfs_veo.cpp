#include <stdio.h>
#include <string.h>
#include <math.h>
#include <stdlib.h>
#include <ve_offload.h>
#include <sys/time.h>

//#define NUM_THREAD 4
#define OPEN

FILE* fp;

// Structure to hold a node information
struct Node {
  int starting;
  int no_of_edges;
};

long get_time() {
  struct timeval tv;
  gettimeofday(&tv, NULL);
  return tv.tv_sec * 1000 + tv.tv_usec / 1000;
}

void BFSGraph(int argc, char** argv);

void Usage(int argc, char** argv) {

  fprintf(stderr, "Usage: %s <num_threads> <input_file>\n", argv[0]);
}
////////////////////////////////////////////////////////////////////////////////
// Main Program
////////////////////////////////////////////////////////////////////////////////
int main(int argc, char** argv) {
  BFSGraph(argc, argv);
}

////////////////////////////////////////////////////////////////////////////////
// Apply BFS on a Graph using CUDA
////////////////////////////////////////////////////////////////////////////////
void BFSGraph(int argc, char** argv) {
  int no_of_nodes    = 0;
  int edge_list_size = 0;
  char* input_f;
  int num_omp_threads;

  if (argc != 3) {
    Usage(argc, argv);
    exit(0);
  }

  num_omp_threads = atoi(argv[1]);
  input_f         = argv[2];

  printf("Reading File\n");
  // Read in Graph from a file
  fp = fopen(input_f, "r");
  if (!fp) {
    printf("Error Reading graph file\n");
    return;
  }

  struct veo_proc_handle* proc = veo_proc_create(0);
  if (proc == NULL) {
    perror("veo_proc_create");
    exit(1);
  }
  uint64_t handle = veo_load_library(proc, "./ve_kernel.so");
  printf("handle = %p\n", (void*)handle);

  long start_time = get_time();

  // create context
  struct veo_thr_ctxt* curr_ctx = veo_context_open(proc);

  int source = 0;

  fscanf(fp, "%d", &no_of_nodes);

  // allocate host memory

  Node* h_graph_nodes         = (Node*)malloc(sizeof(Node) * no_of_nodes);
  bool* h_graph_mask          = (bool*)malloc(sizeof(bool) * no_of_nodes);
  bool* h_updating_graph_mask = (bool*)malloc(sizeof(bool) * no_of_nodes);
  bool* h_graph_visited       = (bool*)malloc(sizeof(bool) * no_of_nodes);

  // allocate memory size
  uint64_t ve_h_graph_nodes;
  int rc = veo_alloc_mem(proc, &ve_h_graph_nodes, sizeof(Node) * no_of_nodes);
  if (rc != VEO_COMMAND_OK) {
    perror("allocate memory return bad code\n");
  }

  uint64_t ve_h_graph_mask;
  rc = veo_alloc_mem(proc, &ve_h_graph_mask, sizeof(bool) * no_of_nodes);
  if (rc != VEO_COMMAND_OK) {
    perror("allocate memory return bad code\n");
  }

  uint64_t ve_h_updating_graph_mask;
  rc = veo_alloc_mem(proc, &ve_h_updating_graph_mask,
                     sizeof(bool) * no_of_nodes);
  if (rc != VEO_COMMAND_OK) {
    perror("allocate memory return bad code\n");
  }

  uint64_t ve_h_graph_visited;
  rc = veo_alloc_mem(proc, &ve_h_graph_visited, sizeof(bool) * no_of_nodes);
  if (rc != VEO_COMMAND_OK) {
    perror("allocate memory return bad code\n");
  }

  int start, edgeno;
  // initalize the memory
  for (unsigned int i = 0; i < no_of_nodes; i++) {
    fscanf(fp, "%d %d", &start, &edgeno);
    h_graph_nodes[i].starting    = start;
    h_graph_nodes[i].no_of_edges = edgeno;
    h_graph_mask[i]              = false;
    h_updating_graph_mask[i]     = false;
    h_graph_visited[i]           = false;
  }

  // read the source node from the file
  fscanf(fp, "%d", &source);
  // source=0; //tesing code line

  // set the source node as true in the mask
  h_graph_mask[source]    = true;
  h_graph_visited[source] = true;

  fscanf(fp, "%d", &edge_list_size);

  int id, cost;
  int* h_graph_edges = (int*)malloc(sizeof(int) * edge_list_size);

  uint64_t ve_h_graph_edges;
  rc = veo_alloc_mem(proc, &ve_h_graph_edges, sizeof(int) * edge_list_size);
  if (rc != VEO_COMMAND_OK) {
    perror("allocate memory return bad code\n");
  }

  for (int i = 0; i < edge_list_size; i++) {
    fscanf(fp, "%d", &id);
    fscanf(fp, "%d", &cost);
    h_graph_edges[i] = id;
  }

  if (fp)
    fclose(fp);

  // allocate mem for the result on host side
  int* h_cost = (int*)malloc(sizeof(int) * no_of_nodes);

  uint64_t ve_h_cost;
  rc = veo_alloc_mem(proc, &ve_h_cost, sizeof(int) * no_of_nodes);
  if (rc != VEO_COMMAND_OK) {
    perror("allocate memory return bad code\n");
  }

  for (int i = 0; i < no_of_nodes; i++)
    h_cost[i] = -1;
  h_cost[source] = 0;

  printf("Start traversing the tree\n");

  long kernel_start_time = get_time();

  // do data copy
  rc = veo_write_mem(proc, ve_h_graph_nodes, h_graph_nodes,
                     sizeof(Node) * no_of_nodes);
  if (rc != VEO_COMMAND_OK) {
    perror("write memory return bad code\n");
  }

  rc = veo_write_mem(proc, ve_h_graph_mask, h_graph_mask,
                     sizeof(bool) * no_of_nodes);
  if (rc != VEO_COMMAND_OK) {
    perror("write memory return bad code\n");
  }

  rc = veo_write_mem(proc, ve_h_updating_graph_mask, h_updating_graph_mask,
                     sizeof(bool) * no_of_nodes);
  if (rc != VEO_COMMAND_OK) {
    perror("write memory return bad code\n");
  }

  rc = veo_write_mem(proc, ve_h_graph_visited, h_graph_visited,
                     sizeof(bool) * no_of_nodes);
  if (rc != VEO_COMMAND_OK) {
    perror("write memory return bad code\n");
  }

  rc = veo_write_mem(proc, ve_h_graph_edges, h_graph_edges,
                     sizeof(int) * edge_list_size);
  if (rc != VEO_COMMAND_OK) {
    perror("write memory return bad code\n");
  }

  rc = veo_write_mem(proc, ve_h_cost, h_cost, sizeof(int) * no_of_nodes);
  if (rc != VEO_COMMAND_OK) {
    perror("write memory return bad code\n");
  }

  struct veo_args* argp0 = veo_args_alloc();
  veo_args_set_i64(argp0, 0, ve_h_graph_nodes);
  veo_args_set_i64(argp0, 1, ve_h_graph_mask);
  veo_args_set_i64(argp0, 2, ve_h_updating_graph_mask);
  veo_args_set_i64(argp0, 3, ve_h_graph_visited);
  veo_args_set_i64(argp0, 4, ve_h_graph_edges);
  veo_args_set_i64(argp0, 5, ve_h_cost);
  veo_args_set_i64(argp0, 6, no_of_nodes);

  uint64_t sym = veo_get_sym(proc, handle, "kernel_func");
  uint64_t pid = veo_call_async(curr_ctx, sym, argp0);
  uint64_t retval;
  rc = veo_call_wait_result(curr_ctx, pid, &retval);
  if (rc != VEO_COMMAND_OK) {
    perror("body_force kernel return error\n");
  }

  // copy data back
  rc = veo_read_mem(proc, h_graph_mask, ve_h_graph_mask,
                    sizeof(bool) * no_of_nodes);
  if (rc != VEO_COMMAND_OK) {
    perror("read memory return bad code\n");
  }

  rc = veo_read_mem(proc, h_graph_visited, ve_h_graph_visited,
                    sizeof(bool) * no_of_nodes);
  if (rc != VEO_COMMAND_OK) {
    perror("read memory return bad code\n");
  }

  rc = veo_read_mem(proc, h_updating_graph_mask, ve_h_updating_graph_mask,
                    sizeof(bool) * no_of_nodes);
  if (rc != VEO_COMMAND_OK) {
    perror("read memory return bad code\n");
  }

  rc = veo_read_mem(proc, h_cost, ve_h_cost, sizeof(int) * no_of_nodes);
  if (rc != VEO_COMMAND_OK) {
    perror("read memory return bad code\n");
  }

  veo_args_free(argp0);

  veo_context_close(curr_ctx);

  long end_time = get_time();

  printf("Total cost: %ld ms\n", end_time - start_time);
  printf("Kernel cost: %ld ms\n", end_time - kernel_start_time);

  // Store the result into a file
  FILE* fpo = fopen("result.txt", "w");
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
