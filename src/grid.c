#include <stdlib.h>

typedef struct Cell {
    long x;
    long y;
} Cell;

typedef struct Node{
    Cell cellule;
    struct Node *suivante;
}Node;

typedef struct Grid{
    long capacite;
    Node **tableau;
    long cellules_alive;
}Grid;

void init_grid(Grid *grid){
    grid ->capacite = 100;
    grid -> tableau = calloc(grid -> capacite, sizeof(Node*));
    if (grid -> tableau == NULL){
        exit(EXIT_FAILURE);
    }
    grid -> cellules_alive = 0;
}

void free_grid(Grid *grid) {
    for (long i = 0; i < grid -> capacite; i++) {
        Node *courant = grid -> tableau[i];
        while (courant != NULL) {
            Node *suivant = courant -> suivante;
            free(courant);
            courant = suivant;
        }
    }
    free(grid -> tableau);
    grid->tableau = NULL;
    grid->capacite = 0;
    grid->cellules_alive = 0;
}

long hash(long x, long y, long capacite) {
    return (31 * (unsigned long)x + 17 * (unsigned long)y) % capacite;
}

int is_alive(Grid *grid, long x, long y){
    long index = hash(x, y, grid -> capacite);
    Node *courant = grid -> tableau[index];
    while (courant != NULL) {
        if (courant -> cellule.x == x && courant -> cellule.y == y) {
            return 1;
        }
        courant = courant -> suivante;
    }
    return 0; // Cell is dead
}

void add_cell(Grid *grid, long x, long y){
    if (is_alive(grid, x, y)) {
        return;
    }

    long index = hash(x, y, grid -> capacite);
    Node *nouvelle_cellule = malloc(sizeof(Node));
    if (nouvelle_cellule == NULL) {
        exit(EXIT_FAILURE);
    }
    nouvelle_cellule -> cellule.x = x;
    nouvelle_cellule -> cellule.y = y;
    nouvelle_cellule -> suivante = grid -> tableau[index];
    grid -> tableau[index] = nouvelle_cellule;
    grid -> cellules_alive++;
}

void remove_cell(Grid *grid, long x, long y){
    long index = hash(x, y, grid -> capacite);
    Node *courant = grid -> tableau[index];
    Node *precedent = NULL;

    while (courant != NULL) {
        if (courant -> cellule.x == x && courant -> cellule.y == y) {
            if (precedent == NULL) {
                grid -> tableau[index] = courant -> suivante;
            } else {
                precedent -> suivante = courant -> suivante;
            }
            free(courant);
            grid -> cellules_alive--;
            return;
        }
        precedent = courant;
        courant = courant -> suivante;
    }
}

