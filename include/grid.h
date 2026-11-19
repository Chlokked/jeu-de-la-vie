#ifndef _GRID_H
#define _GRID_H



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

void init_grid(Grid *grid);
void free_grid(Grid *grid);
long hash(long x, long y, long capacite);
int is_alive(Grid *grid, long x, long y);
void add_cell(Grid *grid, long x, long y);
void remove_cell(Grid *grid, long x, long y);
void resize_grid(Grid *grid);

#endif