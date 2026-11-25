#include <stdio.h>
#include "config.h"

int charger_creux(const char *nom_fichier, Grid *grille){

    if(nom_fichier == NULL || grille == NULL || grille->tableau == NULL) return 0;

    FILE *fichier = fopen(nom_fichier, "r");
    if(fichier == NULL) return 0;

    char type;
    long nombre;

    if(fscanf(fichier, " %c %ld", &type, &nombre) != 2 || type != 'C' || nombre < 0) {
        fclose(fichier);
        return 0;
    }

    Grid nouvelle;
    init_grid(&nouvelle);
    int valide = 1;

    for(long i = 0; i < nombre; i++){
        long x, y;
        if(fscanf(fichier, "%ld %ld", &x, &y) != 2 || is_alive(&nouvelle, x, y)) {
            valide = 0;
            break;
        }
        add_cell(&nouvelle, x, y);
    }

    if(valide){
        char supplement;
        if (fscanf(fichier, " %c", &supplement) == 1) valide = 0;
    }

    if(ferror(fichier)) valide = 0;
    if(fclose(fichier) != 0) valide = 0;

    if(valide == 0){
        free_grid(&nouvelle);
        return valide;
    }

    free_grid(grille);
    *grille = nouvelle;
    return valide;
}

int sauvegarder_creux(const char *nom_fichier, const Grid *grille){

    if(nom_fichier == NULL || grille == NULL || grille->tableau == NULL) return 0;

    FILE *fichier = fopen(nom_fichier, "w");
    if(fichier == NULL) return 0;

    int valide = fprintf(fichier, "C %ld\n", grille->cellules_alive) >= 0;

    for(long i = 0; i < grille->capacite && valide; i++){
        const Node *courant = grille->tableau[i];
        while (courant != NULL) {
            if(fprintf(fichier, "%ld %ld\n", courant->cellule.x, courant->cellule.y) < 0) {
                valide = 0;
                break;
            }
            courant = courant->suivante;
        }
    }
    if(fclose(fichier) != 0){
        valide = 0;
    }
    return valide;
}