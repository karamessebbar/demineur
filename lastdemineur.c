#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>

struct cases {
    char etat; // 'X' non dévoilée et non marquée, 'F' non dévoilée et marquée, ' ' dévoilée et vide, ou un chiffre
    int nb_mines_ad;
    bool mine;
};

struct table {
    int x, y;
    struct cases grille[100][100]; // Définissez une taille maximale pour la grille
    int nb_mines;
    int found;
};

struct table creer(int x, int y, int nb_mine) {
    struct table tb;
    tb.x = x;
    tb.y = y;
    tb.nb_mines = nb_mine;
    tb.found = 0;

    for (int i = 0; i < x; i++) {
        for (int j = 0; j < y; j++) {
            tb.grille[i][j].etat = 'X';
            tb.grille[i][j].nb_mines_ad = 0;
            tb.grille[i][j].mine = false;
        }
    }

    // Initialisation du générateur de nombres aléatoires avec une graine (seed)
    srand(time(NULL));

    // Positionnement aléatoire des mines
    for (int i = 0; i < nb_mine; i++) {
        int a, b;
        do {
            a = rand() % x;
            b = rand() % y;
        } while (tb.grille[a][b].mine); // Assure que la case n'a pas déjà une mine
        tb.grille[a][b].mine = true;
    }

    // Mise à jour du nombre de mines adjacentes
    for (int i = 0; i < x; i++) {
        for (int j = 0; j < y; j++) {
            if (tb.grille[i][j].mine) {
                // Incrémente le nombre de mines adjacentes pour les cases voisines
                for (int dx = -1; dx <= 1; dx++) {
                    for (int dy = -1; dy <= 1; dy++) {
                        int ni = i + dx;
                        int nj = j + dy;
                        if (ni >= 0 && ni < x && nj >= 0 && nj < y) {
                            tb.grille[ni][nj].nb_mines_ad++;
                        }
                    }
                }
            }
        }
    }

    return tb;
}

void afficher(struct table grille) {
    printf("  ");
    for (int j = 0; j < grille.y; j++) {
        printf("%2d ", j);
    }
    printf("\n");

    for (int i = 0; i < grille.x; i++) {
        printf("%2d|", i);
        for (int j = 0; j < grille.y; j++) {
            char c = grille.grille[i][j].etat;
            printf(" %c ", c);
        }
        printf("|\n");
    }
}

// Fonction pour dévoiler une case et gérer les règles du jeu
void devoiler_case(struct table *grille, int x, int y) {
    if (grille->grille[x][y].etat == 'X') {
        if (grille->grille[x][y].mine) {
            // La case contient une mine, l'utilisateur a perdu
            grille->grille[x][y].etat = '*';
            printf("BOOM! Vous avez perdu.\n");
        } else if (grille->grille[x][y].nb_mines_ad > 0) {
            // La case a des mines adjacentes, dévoilez la case
            grille->grille[x][y].etat = '0' + grille->grille[x][y].nb_mines_ad;
        } else {
            // La case est vide, dévoilez la case et répétez pour les cases adjacentes
            grille->grille[x][y].etat = ' ';
            grille->found++;
            if (grille->found == grille->x * grille->y - grille->nb_mines) {
                // Toutes les cases non minées ont été dévoilées, l'utilisateur a gagné
                printf("Félicitations, vous avez gagné!\n");
            } else {
                // Récursivement dévoiler les cases adjacentes vides
                for (int dx = -1; dx <= 1; dx++) {
                    for (int dy = -1; dy <= 1; dy++) {
                        int ni = x + dx;
                        int nj = y + dy;
                        if (ni >= 0 && ni < grille->x && nj >= 0 && nj < grille->y) {
                            devoiler_case(grille, ni, nj);
                        }
                    }
                }
            }
        }
    }
}

int main() {
    struct table grille = creer(5, 5, 5);
    afficher(grille);

    // Boucle de jeu
    while (1) {
        int x, y;
        printf("Choisissez une case (ligne colonne) : ");
        scanf("%d %d", &x, &y);
        if (x >= 0 && x < grille.x && y >= 0 && y < grille.y) {
            char action;
            printf("Posez un drapeau (F) ou dévoilez la case (D) : ");
            scanf(" %c", &action);
            if (action == 'F' || action == 'f') {
                if (grille.grille[x][y].etat == 'X') {
                    grille.grille[x][y].etat = 'F';
                } else if (grille.grille[x][y].etat == 'F') {
                    grille.grille[x][y].etat = 'X';
                }
            } else if (action == 'D' || action == 'd') {
                devoiler_case(&grille, x, y);
            }
            afficher(grille);
        } else {
            printf("Coordonnées invalides. Réessayez.\n");
        }
    }

    return 0;
}
