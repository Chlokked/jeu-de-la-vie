//Fichier fait avec IA !!!!
#include <assert.h>
#include <stdio.h>
#include "grid.h"

void test_initialisation(void) {
    Grid grid;
    init_grid(&grid);

    assert(grid.capacite == 100);
    assert(grid.cellules_alive == 0);
    assert(grid.tableau != NULL);

    for (long i = 0; i < grid.capacite; i++) {
        assert(grid.tableau[i] == NULL);
    }

    free_grid(&grid);
    printf("[OK] Initialisation\n");
}

void test_insertion(void) {
    Grid grid;
    init_grid(&grid);

    add_cell(&grid, 1, 1);
    add_cell(&grid, 2, 2);

    assert(is_alive(&grid, 1, 1) == 1);
    assert(is_alive(&grid, 2, 2) == 1);
    assert(is_alive(&grid, 10, 10) == 0);
    assert(grid.cellules_alive == 2);

    free_grid(&grid);
    printf("[OK] Insertion et recherche\n");
}

void test_doublons(void) {
    Grid grid;
    init_grid(&grid);

    add_cell(&grid, 1, 1);
    add_cell(&grid, 1, 1);
    add_cell(&grid, 1, 1);

    assert(grid.cellules_alive == 1);
    assert(is_alive(&grid, 1, 1) == 1);

    free_grid(&grid);
    printf("[OK] Doublons\n");
}

void test_coordonnees(void) {
    Grid grid;
    init_grid(&grid);

    add_cell(&grid, -5, -8);
    add_cell(&grid, -10, 20);
    add_cell(&grid, 30, -40);
    add_cell(&grid, 0, 0);
    add_cell(&grid, 1000000, -1000000);

    assert(is_alive(&grid, -5, -8));
    assert(is_alive(&grid, -10, 20));
    assert(is_alive(&grid, 30, -40));
    assert(is_alive(&grid, 0, 0));
    assert(is_alive(&grid, 1000000, -1000000));
    assert(grid.cellules_alive == 5);

    free_grid(&grid);
    printf("[OK] Coordonnees negatives et eloignees\n");
}

void test_collisions(void) {
    Grid grid;
    init_grid(&grid);

    // Ces trois cellules ont le meme indice
    // avec notre fonction de hachage actuelle.
    assert(hash(0, 0, grid.capacite) ==
           hash(100, 0, grid.capacite));
    assert(hash(100, 0, grid.capacite) ==
           hash(200, 0, grid.capacite));

    add_cell(&grid, 0, 0);
    add_cell(&grid, 100, 0);
    add_cell(&grid, 200, 0);

    assert(is_alive(&grid, 0, 0));
    assert(is_alive(&grid, 100, 0));
    assert(is_alive(&grid, 200, 0));
    assert(grid.cellules_alive == 3);

    // Suppression au milieu de la liste
    remove_cell(&grid, 100, 0);

    assert(!is_alive(&grid, 100, 0));
    assert(is_alive(&grid, 0, 0));
    assert(is_alive(&grid, 200, 0));
    assert(grid.cellules_alive == 2);

    free_grid(&grid);
    printf("[OK] Collisions\n");
}

void test_suppression(void) {
    Grid grid;
    init_grid(&grid);

    add_cell(&grid, 0, 0);
    add_cell(&grid, 100, 0);
    add_cell(&grid, 200, 0);

    // Suppression en tete
    remove_cell(&grid, 200, 0);
    assert(!is_alive(&grid, 200, 0));
    assert(grid.cellules_alive == 2);

    // Suppression en fin
    remove_cell(&grid, 0, 0);
    assert(!is_alive(&grid, 0, 0));
    assert(grid.cellules_alive == 1);
    assert(is_alive(&grid, 100, 0));

    // Suppression d'une cellule inexistante
    remove_cell(&grid, 99, 99);
    assert(grid.cellules_alive == 1);

    // Suppression du dernier maillon
    remove_cell(&grid, 100, 0);
    assert(!is_alive(&grid, 100, 0));
    assert(grid.cellules_alive == 0);

    // Suppression dans une grille vide
    remove_cell(&grid, 5, 5);
    assert(grid.cellules_alive == 0);

    free_grid(&grid);
    printf("[OK] Suppression\n");
}

void test_redimensionnement(void) {
    Grid grid;
    init_grid(&grid);

    for (long i = 0; i < 100; i++) {
        add_cell(&grid, i, i);
    }

    assert(grid.capacite == 100);
    assert(grid.cellules_alive == 100);

    // Un doublon ne doit pas provoquer de redimensionnement
    add_cell(&grid, 0, 0);
    assert(grid.capacite == 100);

    // La 101e cellule declenche le redimensionnement
    add_cell(&grid, 100, 100);

    assert(grid.capacite == 200);
    assert(grid.cellules_alive == 101);

    // Verification de toutes les cellules apres rehashing
    for (long i = 0; i <= 100; i++) {
        assert(is_alive(&grid, i, i));
    }

    free_grid(&grid);
    printf("[OK] Redimensionnement automatique\n");
}

void test_redimensionnements_multiples(void) {
    Grid grid;
    init_grid(&grid);

    for (long i = 0; i < 10000; i++) {
        add_cell(&grid, i, -i);
    }

    assert(grid.cellules_alive == 10000);
    assert(grid.capacite == 12800);

    for (long i = 0; i < 10000; i++) {
        assert(is_alive(&grid, i, -i));
    }

    free_grid(&grid);
    printf("[OK] Redimensionnements multiples\n");
}

void test_liberation(void) {
    Grid grid;
    init_grid(&grid);

    add_cell(&grid, 1, 1);
    add_cell(&grid, 2, 2);

    free_grid(&grid);

    assert(grid.tableau == NULL);
    assert(grid.capacite == 0);
    assert(grid.cellules_alive == 0);

    printf("[OK] Liberation\n");
}

int main(void) {
    test_initialisation();
    test_insertion();
    test_doublons();
    test_coordonnees();
    test_collisions();
    test_suppression();
    test_redimensionnement();
    test_redimensionnements_multiples();
    test_liberation();

    printf("\nTous les tests sont passes !\n");

    return 0;
}