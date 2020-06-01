#ifndef QUADTREE_H
#define QUADTREE_H

#include <stdio.h>

struct Arvore {
    Linha linhas[];
    float xmin,xmax,ymin,ymax;
    bool folha;
    struct Arvore* subArvore1;            // ponteiros para os filhos, se houver
    struct Arvore* subArvore2;
};

typedef struct Arvore Nodo;

Nodo* GeraArvore();
Nodo* NovoNodo(float xmin, float xmax, float ymin, float ymax, bool folha;);

#endif // QUADTREE_H
