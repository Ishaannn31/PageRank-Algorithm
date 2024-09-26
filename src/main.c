

#include <stdio.h>
#include <stdlib.h>
#include <getopt.h>
#include <string.h>
#include "utils.h"


typedef struct {
    char **name;
    int *out_degree;
    int node_count;
    int edge_count;
    int **adj_matrix;
} Graph;

void initialize_graph(Graph *graph) {
    graph->name = NULL;
    graph->out_degree = NULL;
    graph->node_count = 0;
    graph->edge_count = 0;
    graph->adj_matrix = NULL;
}

int find_node_index(Graph *graph, const char *name) {
    for (int i = 0; i < graph->node_count; i++) {
        if (strcmp(graph->name[i], name) == 0) {
            return i;
        }
    }
    return -1;
}

void add_node(Graph *graph, const char *name) {
    int index = find_node_index(graph, name);
    if (index == -1) {
        graph->name = realloc(graph->name, (graph->node_count + 1) * sizeof(char *));
        graph->name[graph->node_count] = malloc(strlen(name) + 1);
        strcpy(graph->name[graph->node_count], name);

        graph->out_degree = realloc(graph->out_degree, (graph->node_count + 1) * sizeof(int));
        graph->out_degree[graph->node_count] = 0;

        if (graph->node_count > 0) {
            for (int i = 0; i < graph->node_count; i++) {
                graph->adj_matrix[i] = realloc(graph->adj_matrix[i], (graph->node_count + 1) * sizeof(int));
                graph->adj_matrix[i][graph->node_count] = 0;
            }
        }

        graph->adj_matrix = realloc(graph->adj_matrix, (graph->node_count + 1) * sizeof(int *));
        graph->adj_matrix[graph->node_count] = calloc(graph->node_count + 1, sizeof(int));
        graph->node_count++;
    }
}

void add_edge(Graph *graph, const char *from, const char *to) {
    int from_index = find_node_index(graph, from);
    int to_index = find_node_index(graph, to);

    if (from_index != -1 && to_index != -1) {
        graph->adj_matrix[from_index][to_index]++;
        graph->out_degree[from_index]++;
        graph->edge_count++;
    }
}

void print_statistics(Graph *graph, const char *graph_id) {
    int min_in_degree = graph->node_count;
    int max_in_degree = 0;
    int min_out_degree = graph->node_count;
    int max_out_degree = 0;

    if (graph->node_count == 0) {
        printf("%s:\n", graph_id);
        printf("- num nodes: 0\n");
        printf("- num edges: 0\n");
        printf("- indegree: 0-0\n");
        printf("- outdegree: 0-0\n");
        return;
    } 

    for(int i = 0; i < graph->node_count; i++) {
        int in_degree = 0;
        for(int j = 0; j < graph->node_count; j++) {
            in_degree += graph->adj_matrix[j][i];
        }
        if(in_degree < min_in_degree) {
          min_in_degree = in_degree;
        }
        if(in_degree > max_in_degree) {
          max_in_degree = in_degree;
        }
        if(graph->out_degree[i] < min_out_degree) {
          min_out_degree = graph->out_degree[i];
        }
        if(graph->out_degree[i] > max_out_degree) {
          max_out_degree = graph->out_degree[i];
        }
    }

    printf("%s:\n", graph_id);
    printf("- num nodes: %d\n", graph->node_count);
    printf("- num edges: %d\n", graph->edge_count);
    printf("- indegree: %d-%d\n", min_in_degree, max_in_degree);
    printf("- outdegree: %d-%d\n", min_out_degree, max_out_degree);
    
}
void random_musafir(Graph *graph, double p, int steps, double *page_rank) {
    if (graph->node_count == 0 || p > 1 || p <= 0.0 || steps <= 0) {
        return;
    }

    int rand_node = randu(graph->node_count);
    int visits[graph->node_count];
    memset(visits, 0, graph->node_count * sizeof(int)); 

    for (int i = 0; i < steps; i++) {
          double randuProb = (double)randu(RAND_MAX)/ (double)RAND_MAX;
        if (randuProb < p || graph->out_degree[rand_node] == 0 || p == 1.0) {
            rand_node = randu(graph->node_count);
        } else {
            int next_rand_node = randu(graph->out_degree[rand_node]);
            int link_count = 0;
        
            for (int j = 0; j < graph->node_count; j++) {
                if (graph->adj_matrix[rand_node][j] > 0) {
                    if (link_count == next_rand_node) {
                        rand_node = j;
                        break;
                    }
                    link_count++;
                }
            }
        }
        visits[rand_node]++; 
    }

    for (int i = 0; i < graph->node_count; i++) {
        page_rank[i] = (double)visits[i] / steps;
    }
}

void print_pr(Graph *graph, double *page_rank) {
    if (graph->node_count == 0) {
        return;
    }
    if(graph->node_count == 1) {
      page_rank[0] = 1.0000000000;
      printf("%s %.10f\n", graph->name[0],page_rank[0]);
      return;
    }
    for (int i = 0; i < graph->node_count; i++) {
        printf("%s %.10f\n", graph->name[i], page_rank[i]);
    }
}

void markov_chain(Graph *graph, double p, int steps, double *page_rank) {
    if (graph->node_count == 0 || steps<=0 || p<=0.0 || p>1) {
        return;
    }

    double transition_matrix[graph->node_count][graph->node_count];
    double vector[graph->node_count];
    double current_vector[graph->node_count];

    for (int i = 0; i < graph->node_count; i++) {
        vector[i] = 1.0 / graph->node_count;
    }

    for (int i = 0; i < graph->node_count; i++) {
        for (int j = 0; j < graph->node_count; j++) {
            if (graph->out_degree[i] == 0) {
                transition_matrix[i][j] = 1.0 / graph->node_count;
            } else {
                transition_matrix[i][j] = p *(1.0/graph->node_count)+(1 - p)*((double)graph->adj_matrix[i][j]/graph->out_degree[i]);
            }
        }
    }

    memcpy(current_vector, vector, sizeof(double) * graph->node_count);

    for (int i = 0; i < steps; i++) {
        double next_vector[graph->node_count];
        memset(next_vector, 0, sizeof(double) * graph->node_count);

        for (int j = 0; j < graph->node_count; j++) {
            for (int i = 0; i < graph->node_count; i++) { 
             next_vector[j] += current_vector[i] * transition_matrix[i][j];
            }
            
        }

        memcpy(current_vector, next_vector, sizeof(double) * graph->node_count);
    }

    memcpy(page_rank, current_vector, sizeof(double) * graph->node_count);
}

void print_help() {
    printf("Usage: ./pagerank [OPTIONS] ... [FILENAME]\n");
    printf("Perform pagerank computations for a given file in the DOT format\n\n");
    printf("  -h     Print a brief overview of the available command line parameters\n");
    printf("  -r N   Simulate N steps of the random surfer and output the result\n");
    printf("  -m N   Simulate N steps of the Markov chain and output the result\n");
    printf("  -s     Compute and print the statistics of the graph as defined\n");
    printf("           in section 3.4\n");
    printf("  -p P   Set the parameter p to P%%. (Default: P = 10)\n");
}

int main(int argc, char *const *argv) {
   if (argc <= 1) {
        exit(1);
    }
     rand_init();
    int statistics = 0,rand_surf = 0,mark_c = 0,steps = 0;
    double p= 10/100.0;  // def p
    int args;

    while ((args = getopt(argc, argv, "hr:m:sp:")) != -1) {
        switch (args) {
            case 'h':
                print_help();
                exit(0);
            case 'r':
                rand_surf = 1;
                steps = atoi(optarg); // arg to int
                if(steps<=0) {
                    exit (1);
                }
                break;
            case 'm':
                mark_c = 1;
                steps = atoi(optarg);
                if(steps<=0) {
                    exit (1);
                }
                break;
            case 's':
                statistics = 1;
                break;
            case 'p':
                p = atoi(optarg) / 100.0;
                if(p<=0.0) {
                    exit (1);
                }
                break;
            default:
                print_help();
                exit(1);
        }
    }

      if(!rand_surf && !mark_c && !statistics) {
        print_help();
        exit(1);
      }

      if (optind >= argc) { //no filename
        exit(1);
      }

    const char *filename = argv[optind];
    if (strstr(filename, ".dot") == NULL || 
        strcmp(strstr(filename, ".dot"), ".dot") != 0) {
        exit(1);
    }

    FILE *file = fopen(filename, "r");
    if (file == NULL) { // file not opened
         exit(1);
    }

    Graph graph;
    initialize_graph(&graph);

    char line[520];
    char graph_id[257]; 
    int first_line = 1;
    char digraph[]= "digraph";
    char dig[8];
    char *ptr = NULL;
    char arrow[] = "->";

    while (fgets(line, sizeof(line), file)) {
      if(strlen(line)>519) {
        exit(1);
      }
        if (first_line) {
           sscanf(line, "digraph %[^ ] {\n", graph_id);
           sscanf(line,"%[^ ] idetifier {\n", dig);
        
           if(strlen(digraph) != strlen(dig)) {
            exit (1);
           }
          ptr = strchr(line, '{');
          if(ptr == NULL) {
            exit(1);
          }
            if(strlen(graph_id)>256) {
              exit(1);
            }
           if(!(graph_id[0] >= 'A' && graph_id[0] <= 'Z') && !(graph_id[0] >= 'a' && graph_id[0] <= 'z')) {
              exit(1);
           }
            first_line = 0;
        } else {
            char from[257], to[257],  arr[3];
            
            if (sscanf(line, "%[^ ] %[^ ] %[^;];\n", from,arr, to) == 3) {
                if(strlen(from)>256 || strlen(to)>256){
                exit(1);
              }
              if(strcmp(arr, arrow)!=0) {
                exit(1);
              }
               if(strstr(line,";")==NULL || strstr(line,"->")== NULL) {
                    exit(1);
                }
            if((!(from[0] >= 'A' && from[0] <= 'Z') && !(from[0] >= 'a' && from[0] <= 'z')) || (!(to[0] >= 'A' && to[0] <= 'Z') && !(to[0] >= 'a' && to[0] <= 'z'))) {
              exit(1);
           }
               
              
                add_node(&graph, from);
                add_node(&graph, to);
                add_edge(&graph, from, to);
            }
        }
    }

    fclose(file);
    if (strstr(line, "}") == NULL) {
        exit(1);
    }
    if (statistics) {
        print_statistics(&graph, graph_id);
    }

    if (rand_surf || mark_c) { 
        double *page_rank = malloc(graph.node_count * sizeof(double));
        if (rand_surf) {
            random_musafir(&graph, p, steps, page_rank);
            print_pr(&graph, page_rank);
        }
        if (mark_c) {
            markov_chain(&graph, p, steps, page_rank);
            print_pr(&graph, page_rank);
        }
        free(page_rank);
    }
    for (int i = 0; i < graph.node_count; i++) {
        free(graph.name[i]);
        free(graph.adj_matrix[i]);
    }
    free(graph.name);
    free(graph.out_degree);
    free(graph.adj_matrix);
    initialize_graph(&graph);
    // TODO: Implement me
    exit(0);
}
