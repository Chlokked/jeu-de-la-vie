#include <stdlib.h>
#include <limits.h>
#include "grid.h"

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

    if (grid -> cellules_alive >= grid -> capacite) {
        resize_grid(grid);
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

void resize_grid(Grid *grid){
    if (grid->capacite > LONG_MAX / 2) {
        exit(EXIT_FAILURE);
    }

    long nouvelle_capacite = grid -> capacite * 2;
    Node **nouveau_tableau = calloc(nouvelle_capacite, sizeof(Node*));
    if (nouveau_tableau == NULL) {
        exit(EXIT_FAILURE);
    }

    for (long i = 0; i < grid -> capacite; i++) {
        Node *courant = grid -> tableau[i];
        while (courant != NULL) {
            long nouvel_index = hash(courant -> cellule.x, courant -> cellule.y, nouvelle_capacite);
            Node *suivant = courant -> suivante;

            courant -> suivante = nouveau_tableau[nouvel_index];
            nouveau_tableau[nouvel_index] = courant;

            courant = suivant;
        }
    }

    free(grid -> tableau);
    grid -> tableau = nouveau_tableau;
    grid -> capacite = nouvelle_capacite;
}
