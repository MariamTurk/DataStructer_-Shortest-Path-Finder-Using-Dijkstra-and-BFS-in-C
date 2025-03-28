#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <limits.h>

#define MAX_LEN 100
#define CITY_NAME_LENGTH 50


typedef struct {
    char name[CITY_NAME_LENGTH];
    int distance;
} Neighbor;

typedef struct {
    char city[CITY_NAME_LENGTH];
    Neighbor neighbors[MAX_LEN];
    int neighbors_count;
} City;

City cities[MAX_LEN];
int cities_count = 0;

City *find_city(char *name) {
    for(int i = 0; i < cities_count; i++) {
        if(strcmp(cities[i].city, name) == 0)
            return &cities[i];
    }
    return NULL;
}

void add_neighbor(City *city, char *name, int distance) {
    strcpy(city->neighbors[city->neighbors_count].name, name);
    city->neighbors[city->neighbors_count].distance = distance;
    city->neighbors_count++;
}

void add_edge(char *city1, char *city2, int distance) {
    City *c1 = find_city(city1);
    if(c1 == NULL) {
        strcpy(cities[cities_count].city, city1);
        c1 = &cities[cities_count];
        cities_count++;
    }
    add_neighbor(c1, city2, distance);

    City *c2 = find_city(city2);
    if(c2 == NULL) {
        strcpy(cities[cities_count].city, city2);
        c2 = &cities[cities_count];
        cities_count++;
    }
    add_neighbor(c2, city1, distance);
}

void load_cities(char *filename) {
    FILE *file = fopen(filename, "r");
    if(file == NULL) {
        printf("Error opening file!\n");
        return;
    }

    char city1[CITY_NAME_LENGTH], city2[CITY_NAME_LENGTH];
    int distance;
    char line[MAX_LEN];

    while(fgets(line, MAX_LEN, file) != NULL) {
        sscanf(line, "%s %s %dkm", city1, city2, &distance);
        add_edge(city1, city2, distance);
    }

    fclose(file);
}

typedef struct {
    char city[CITY_NAME_LENGTH];
    int visited;
    int min_distance;
    char predecessor[CITY_NAME_LENGTH];
} DijkstraNode;

DijkstraNode dijkstra_nodes[MAX_LEN];

void initialize_dijkstra() {
    for(int i = 0; i < cities_count; i++) {
        strcpy(dijkstra_nodes[i].city, cities[i].city);
        dijkstra_nodes[i].visited = 0;
        dijkstra_nodes[i].min_distance = INT_MAX;
        strcpy(dijkstra_nodes[i].predecessor, "");
    }
}

DijkstraNode *find_dijkstra_node(char *name) {
    for(int i = 0; i < cities_count; i++) {
        if(strcmp(dijkstra_nodes[i].city, name) == 0)
            return &dijkstra_nodes[i];
    }
    return NULL;
}

DijkstraNode *find_unvisited_node_with_min_distance() {
    int min_distance = INT_MAX;
    DijkstraNode *node = NULL;
    for(int i = 0; i < cities_count; i++) {
        if(dijkstra_nodes[i].visited == 0 && dijkstra_nodes[i].min_distance < min_distance) {
            min_distance = dijkstra_nodes[i].min_distance;
            node = &dijkstra_nodes[i];
        }
    }
    return node;
}

void dijkstra(char *source) {
    initialize_dijkstra();
    DijkstraNode *source_node = find_dijkstra_node(source);
    source_node->min_distance = 0;

    for(int i = 0; i < cities_count; i++) {
        DijkstraNode *node = find_unvisited_node_with_min_distance();
        if(node == NULL)
            break;

        node->visited = 1;
        City *city = find_city(node->city);
        for(int j = 0; j < city->neighbors_count; j++) {
            DijkstraNode *neighbor_node = find_dijkstra_node(city->neighbors[j].name);
            if(neighbor_node->visited == 0) {
                int new_distance = node->min_distance + city->neighbors[j].distance;
                if(new_distance < neighbor_node->min_distance) {
                    neighbor_node->min_distance = new_distance;
                    strcpy(neighbor_node->predecessor, node->city);
                }
            }
        }
    }
}

typedef struct {
    char city[CITY_NAME_LENGTH];
    int visited;
    char predecessor[CITY_NAME_LENGTH];
} BFSNode;

BFSNode bfs_nodes[MAX_LEN];

void initialize_bfs() {
    for(int i = 0; i < cities_count; i++) {
        strcpy(bfs_nodes[i].city, cities[i].city);
        bfs_nodes[i].visited = 0;
        strcpy(bfs_nodes[i].predecessor, "");
    }
}

BFSNode *find_bfs_node(char *name) {
    for(int i = 0; i < cities_count; i++) {
        if(strcmp(bfs_nodes[i].city, name) == 0)
            return &bfs_nodes[i];
    }
    return NULL;
}

void bfs(char *source) {
    initialize_bfs();
    BFSNode *source_node = find_bfs_node(source);
    source_node->visited = 1;

    char queue[MAX_LEN][CITY_NAME_LENGTH];
    int front = 0;
    int back = 0;

    strcpy(queue[back], source);
    back++;

    while(front != back) {
        char *city_name = queue[front];
        front++;

        City *city = find_city(city_name);
        for(int i = 0; i < city->neighbors_count; i++) {
            BFSNode *neighbor_node = find_bfs_node(city->neighbors[i].name);
            if(neighbor_node->visited == 0) {
                neighbor_node->visited = 1;
                strcpy(neighbor_node->predecessor, city_name);
                strcpy(queue[back], city->neighbors[i].name);
                back++;
            }
        }
    }
}

void print_path(char *source, char *destination, char *algorithm) {
    printf("\n%s path from %s to %s: ", algorithm, source, destination);
    char path[MAX_LEN][CITY_NAME_LENGTH];
    int path_length = 0;

    if(strcmp(algorithm, "Dijkstra") == 0) {
        DijkstraNode *node = find_dijkstra_node(destination);
        while(node != NULL) {
            strcpy(path[path_length], node->city);
            path_length++;
            node = find_dijkstra_node(node->predecessor);
        }
    } else if(strcmp(algorithm, "BFS") == 0) {
        BFSNode *node = find_bfs_node(destination);
        while(node != NULL) {
            strcpy(path[path_length], node->city);
            path_length++;
            node = find_bfs_node(node->predecessor);
        }
    }

    int total_distance = 0;

    for(int i = path_length - 1; i >= 0; i--) {
        printf("%s", path[i]);
        if(i > 0) {
            City *city = find_city(path[i]);
            for(int j = 0; j < city->neighbors_count; j++) {
                if(strcmp(city->neighbors[j].name, path[i-1]) == 0) {
                    printf(" -> (%dkm) ", city->neighbors[j].distance);
                    total_distance += city->neighbors[j].distance;
                    break;
                }
            }
        }
    }

    printf("\tTotal distance: %dkm\n", total_distance);
}


void print_menu() {
    printf("\n********** Shortest Path Finder **********\n");
    printf("1. Load cities\n");
    printf("2. Enter source\n");
    printf("3. Enter destination\n");
    printf("4. Exit\n");
    printf("Enter your choice: ");
}

void write_to_file(char *source, char *destination) {
    FILE *file = fopen("shortest_distance.txt", "w");
    if(file == NULL) {
        printf("Error opening file!\n");
        return;
    }

    fprintf(file, "Dijkstra path from %s to %s: ", source, destination);
    char path[MAX_LEN][CITY_NAME_LENGTH];
    int path_length = 0;
    DijkstraNode *node = find_dijkstra_node(destination);
    while(node != NULL) {
        strcpy(path[path_length], node->city);
        path_length++;
        node = find_dijkstra_node(node->predecessor);
    }

    int total_distance = 0;

    for(int i = path_length - 1; i >= 0; i--) {
        fprintf(file, "%s", path[i]);
        if(i > 0) {
            City *city = find_city(path[i]);
            for(int j = 0; j < city->neighbors_count; j++) {
                if(strcmp(city->neighbors[j].name, path[i-1]) == 0) {
                    fprintf(file, " -> (%dkm) ", city->neighbors[j].distance);
                    total_distance += city->neighbors[j].distance;
                    break;
                }
            }
        }
    }

    fprintf(file, "\nTotal distance: %dkm\n", total_distance);

    fclose(file);
}


int main() {
    char filename[MAX_LEN];
    char source[CITY_NAME_LENGTH];
    char destination[CITY_NAME_LENGTH];
    int choice;

    while(1) {
        print_menu();
        scanf("%d", &choice);

        switch(choice) {
            case 1:
                printf("Enter filename: ");
                scanf("%s", filename);
                load_cities(filename);
                break;
            case 2:
                printf("Enter source city: ");
                scanf("%s", source);
                break;
            case 3:
                printf("Enter destination city: ");
                scanf("%s", destination);
                dijkstra(source);
                bfs(source);
                print_path(source, destination, "Dijkstra");
                print_path(source, destination, "BFS");
                break;
            case 4:
                write_to_file(source, destination);
                printf("Shortest path written to shortest_distance.txt\n");
                return 0;
            default:
                printf("Invalid choice!\n");
        }
    }

    return 0;
}

