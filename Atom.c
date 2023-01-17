/**
* Projet de info4A_2020
* Auteur: Dornier Simon et Charles-Adele Desormeaux
* Type de projet: J
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

typedef unsigned int piece;
typedef unsigned char bool;
#define true 1
#define false 0

void draw(piece** tableau, int taille)
{   int x = taille;
    int y = taille;
    printf("   ");
    for (int i = 0; i < x; i++)
        printf(" %c  ", (i+'A'));
    printf("\n");
    printf("  ");
    for (int i = 0; i < x-1; i++)
        printf("----");
    printf("-----\n");

    printf(" |");
    for (int i = 0; i < x; i++)
        printf("    ");
    printf(" |\n");
    for (int j = 0; j < y; j++)
    {
        printf(" | ");
        for (int i = 0; i < x; i++)
        {
            char* c = ((char*)&tableau[j][i]);
            printf(" %c  ", c[0] + 'A');
        }
        printf("|\n");
        printf("%d| ", (j+1));
        for (int i = 0; i < x; i++)
        {
            char* c = ((char*)&tableau[j][i]);
            printf("%c#%c ", c[3] + 'A', c[1] + 'A');
        }
        printf("|\n");
        printf(" | ");
        for (int i = 0; i < x; i++)
        {
            char* c = ((char*)&tableau[j][i]);
            printf(" %c  ", c[2] + 'A');
        }
        if (j < y-1) {
            printf("|\n |");
            for (int i = 0; i < x; i++)
                printf("    ");
            printf(" |\n");
        }
        else {
            printf("|\n");
            printf(" |");
            for (int i = 0; i < x; i++)
                printf("    ");
            printf(" |\n");
        }
    }
    printf("  ");
    for (int i = 0; i < x; i++)
    printf("----");
    printf("-\n");
}

void drawP(piece p)
{
    printf(" %c \n%c#%c\n %c ", ((char*)&p)[0] + 'A', ((char*)&p)[1] + 'A', ((char*)&p)[2] + 'A', ((char*)&p)[3] + 'A');
}

piece randomPiece()
{
    piece p = 0x00000000;
    for (int i = 0; i < 4; i++)
        p |=  (rand() % 4) << (8 * i);
    return p;
}

unsigned char getLettre(piece p, int position)
{
    return ((p & (255 << (position * 8))) >> (position * 8))+'A';
}
void setLettre(piece* p, int position, unsigned char lettre)
{
    *p = (*p & ~(255 << (position * 8))) | ((lettre - 'A') << (position * 8));
}

bool bienPlace(piece** t, int x1, int y1, int x2, int y2)
{
    if (x1 != x2 && y1 != y2)
        return false;
    piece p1 = t[y1][x1];
    piece p2 = t[y2][x2];
    if (x1 != x2)
    {
        if (x1 < x2)
            return getLettre(p1, 1) == getLettre(p2, 3);
        else
            return getLettre(p1, 3) == getLettre(p2, 1);
    }
    else
    {
        if (y1 < y2)
            return getLettre(p1, 2) == getLettre(p2, 0);
        else
            return getLettre(p1, 0) == getLettre(p2, 2);
    }
    return false;
}

int collisions(piece** t, int taille)
{
  int col = 0;
    int x = taille;
    int y = taille;
    for (int i = 0; i < x-1; i++)
    {
        for (int j = 0; j < y-1; j++)
        {
            if (!bienPlace(t, i, j, i + 1, j) || !bienPlace(t, i, j, i, j + 1))
            {
                col++;
            }
        }
    }
    for (int i = 0; i < x; i++)
    {
        if (getLettre(t[y-1][i], 2) != getLettre(t[0][i], 0))//!bienPlace(t, i, y-1, i, 0))
        {
            col++;
        }
    }
    for (int j = 0; j < y; j++)
    {
        if (getLettre(t[j][x-1], 1) != getLettre(t[j][0], 3))//!bienPlace(t, x-1, j, 0, j))
        {
            col++;
        }
    }
    return col;
}

bool resolu(piece** t, int taille)
{
  return collisions(t, taille) == 0;
}

void swap(piece** t, int x1, int y1, int x2, int y2)
{
    piece dt = t[x1][y1];
    t[x1][y1] = t[x2][y2];
    t[x2][y2] = dt;
}

void rotate(piece** t, int x, int y, int rot) {
    rot = rot % 4;
    t[x][y] = t[x][y] << (rot * 8) | t[x][y] >> ((4 - rot) * 8);
}

piece** generate(int taille)
{
    int x = taille;
    int y = taille;
    piece** res = (piece**)calloc(y, sizeof(piece*));
    for (int j = 0; j < y; j++)
    {
        res[j] = (piece*)calloc(x, sizeof(piece));
    }
    return res;
}

piece** generateRandomFinis(int taille)
{
    int x = taille;
    int y = taille;
    piece** p = generate(taille);
    for (int j = 0; j < y; j++)
        for (int i = 0; i < x; i++)
            p[j][i] = randomPiece();

    for (int i = 1; i < x; i++)
    {
        setLettre(&p[0][i], 3, getLettre(p[0][i - 1], 1));
    }
    setLettre(&p[0][x - 1], 1, getLettre(p[0][0], 3));

    for (int j = 1; j < y; j++)
    {
        setLettre(&p[j][0], 0, getLettre(p[j - 1][0], 2));
    }
    setLettre(&p[y - 1][0], 2, getLettre(p[0][0], 0));

    for (int j = 1; j < y - 1; j++)
    {
        for (int i = 1; i < x; i++)
        {
            setLettre(&p[j][i], 3, getLettre(p[j][i-1], 1));
            setLettre(&p[j][i], 0, getLettre(p[j-1][i], 2));
        }
        setLettre(&p[j][x-1], 1, getLettre(p[j][0], 3));
    }

    for (int i = 1; i < x; i++)
    {
        setLettre(&p[y - 1][i], 3, getLettre(p[y - 1][i - 1], 1));
        setLettre(&p[y - 1][i], 0, getLettre(p[y - 2][i], 2));
        setLettre(&p[y - 1][i], 2, getLettre(p[0][i], 0));
    }
    setLettre(&p[y - 1][x - 1], 1, getLettre(p[y - 1][0], 3));

    return  p;
}

piece** generateRandomFinissable(int taille, int iterations)
{
    int x = taille;
    int y = taille;
    piece** t = generateRandomFinis(taille);
    draw(t, taille);
    for (int i = 0; i < iterations; i++)
    {
        if (rand() % 2)
        {
            swap(t, rand() % x, rand() % y, rand() % x, rand() % y);
        }
        else
        {
            rotate(t, rand() % x, rand() % y, (rand() % 3)+1);
        }
    }
    return t;
}

piece** copieTab(int taille, piece** tab)
{
    piece** tab2 = generate(taille);
    for (int i=0; i<taille; i++)
        for (int j=0; j<taille; j++)
            tab2[i][j]=tab[i][j];
    return tab2;
}

///////// IA_DEBUT
void rndMod(piece** tab, int taille)
{
  int x = taille;
  int y = taille;
  if (rand() % 2)
  {
      swap(tab, rand() % x, rand() % y, rand() % x, rand() % y);
  }
  else
  {
      rotate(tab, rand() % x, rand() % y, (rand() % 3)+1);
  }
}
void deleteTab(piece** t, int taille)
{
  for (int i = 0; i < taille; i++)
    free(t[i]);
  free(t);
}
void printt(int* t, int l)
{
  printf("[ ");
  for (int i = 0; i < l; i++)
    printf("%d ", t[i]);
  printf("]\n");
}

piece** resolve(piece** tab, int taille, int numTabs)
{
  piece*** tabs = (piece***)malloc(numTabs * sizeof(piece**));
  int* collision = (int*)malloc(numTabs * sizeof(int));
  int* indexes = (int*)malloc(numTabs * sizeof(int));
  for (int i = 0; i < numTabs; i++) {
    tabs[i] = copieTab(taille, tab);
    rndMod(tabs[i], taille);
    collision[i] = -1;
    indexes[i] = i;
  }
  while (1) {
    for (int i = 0; i < numTabs; i++)
    {
      collision[i] = collisions(tabs[i], taille);
      indexes[i] = i;
      //Tri par insertion
      for (int j = i - 1; j >= 0; j--)
      {
        if (collision[j + 1] > collision[j])
          break;
        int c = collision[j];
        collision[j] = collision[j + 1];
        collision[j + 1] = c;
        int k = indexes[j];
        indexes[j] = indexes[j + 1];
        indexes[j + 1] = k;
      }
    }

    if (collision[0] == 0)
      break;
    int m = 0;
    for (int i = 0; i < numTabs; i++)
    {
      tabs[indexes[numTabs - 1 - i]] = copieTab(taille, tabs[indexes[m]]);
      rndMod(tabs[indexes[numTabs - 1 - i]], taille);
      if ((i+1) % (numTabs / (2 + m) + 1) == 0)
        m++;
    }
  }
  piece** res = copieTab(taille, tabs[indexes[0]]);
  free(collision);
  free(indexes);
  for (int i = 0; i < numTabs; i++)
    deleteTab(tabs[i], taille);
  return res;
}

///////// IA_FIN

int jeu()
{
    int taille;
    int diff;
    int compteurCoup=0;
    printf("Quel taille de tableau voulez-vous (entre 1 et 9)?\n");
    scanf("%d",&taille);
    printf("Quel difficulte voulez-vous ?\n");
    scanf("%d",&diff);
    printf("La taille de tableau choisi est: %dx%d\n", taille,taille);
    printf("Plateau solution: \n");
    piece** tab = generateRandomFinissable(taille, diff);
    piece** saveTab = copieTab(taille ,tab);
    printf("Plateau actuel: \n");
    draw(tab, taille);

    while(!resolu(tab,taille))
    {
        int choix;
        printf("Entrez (0,1,2 ou 3) pour:\n1.Pour que l'IA resolve le puzzle\n2.Tourner une piece\n3.Pour echanger deux pieces\n0.Pour recommencer ce plateau du debut\n");
        scanf("%d", &choix);
        switch (choix)
        {
            case 0:
            {
                printf("Et on y retourne ;)\n");
                tab=copieTab(taille ,saveTab);
                break;
            }
            case 1:
            {
              printf("Attendez je cherche...\n");
              piece** res = resolve(tab, taille, taille * taille * taille);
              printf("Plateau resolu: \n");
              draw(res, taille);
              break;
            }
            case 2:
            {
                printf("Pour echanger deux pieces, entrez leurs coordonnees respectives (ligne puis colonne(en majuscule)): \n");
                int x1,x2;
                char y1,y2;
                scanf("%d %c %d %c", &x1, &y1, &x2, &y2);
                int coordY1=y1-'a';
                int coordY2=y2-'a';
                if(x1>taille || coordY1>taille || x2>taille || coordY2>taille ) printf("Valeurs incorrects...\n");
                else swap(tab,x1-1,coordY1,x2-1,coordY2);
                compteurCoup++;
                break;
            }
            case 3:
            {
                printf("Pour tourner une piece, entrez ses coordonnees(ligne puis colonne(en majuscule))\net le nombre de rotation dans le sens horaire: \n");
                int x,r;
                char y;
                scanf("%d %c %d", &x, &y, &r);
                int coordY=y-'a';
                if(x>taille || coordY>taille) printf("Valeurs incorrects...\n");
                else rotate(tab,x-1,coordY,r);
                compteurCoup++;
                break;
            }
            default:
            {
                printf("Choix invalide, recommence...\n");
                break;
            }
        }
        printf("Plateau actuel: (score actuel: %d) \n", compteurCoup);
        draw(tab, taille);
        printf("\n");
    }
    return compteurCoup;
}

int main() //Le main va initaliser "time" pour les randoms puis appeller la méthode jeu qui va lui renvoyer un score et on l'affiche
{
    srand(time(0));
    int score=jeu();
    printf("Bien jouez champion, tu as reussi en: %d coups\n", score);
}
