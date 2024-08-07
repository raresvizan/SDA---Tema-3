/* VISAN Rares-Stefan - 312CC */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// structura nod graf - include numele pentru cerinta1 si depth pt cerinta2
typedef struct node {
    int val;
    int depth;
    char name[20];
    struct node *next;
} Node, *GNode;

// structura muchie graf pentru a memora costul muchiei dintre 2 noduri
typedef struct edge {
    int cost;
    int node1, node2;
} Edge;

// structura graf reprezentat prin liste de adiacenta
typedef struct graph {
    int n;
    GNode a[31];
} Graph;

// initializare graf
void init_graph(Graph *g, int n) {
    int i;
    g->n = n;
    for (i = 0; i <= n; i++) {
        g->a[i] = NULL;
    }
}

// adaugare muchie graf cu nume pentru nod (cerinta1)
void insert_edge1(Graph *g, int x, int y) {
    GNode new = (GNode)calloc(1, sizeof(Node));
    new->val = y;
    new->next = g->a[x];
    strcpy(new->name, g->a[x]->name);
    g->a[x] = new;
}

// adaugare muchie graf (cerinta2)
void insert_edge2(Graph *g, int x, int y) {
    GNode new = (GNode)calloc(1, sizeof(Node));
    new->val = y;
    new->next = g->a[x];
    g->a[x] = new;
}

/*citire date pentru cerinta1 - verific daca apare o locatie noua sau nu pentru
    a nu introduce in graf 2 noduri pentru aceeasi locatie; adaug muchie intre
    nodurile citite si memorez costurile muchiilor */
void read_data_task1(Graph *g, Edge *edge, int m, FILE *fin) {
    int i, zone = 0, j;
    char citire[20];
    // memorez in x si y val nodurilor intre care trebuie inserata muchie
    for (i = 0; i < m; i++) {
        int found = 0, x, y;
        // citesc prima locatie - daca nu exista deja, ii aloc memorie
        fscanf(fin, "%s", citire);
        for (j = 1; j <= zone; j++) {
            if (strcmp(g->a[j]->name, citire) == 0) {
                found = 1;
                break;
            }
        }
        if (found == 0) {
            zone++;
            x = zone;
            g->a[x] = (GNode)calloc(1, sizeof(Node));
            strcpy(g->a[x]->name, citire);
        } else {
            x = j;
        }
        // citesc a doua locatie
        fscanf(fin, "%s", citire);
        found = 0;
        for (j = 1; j <= zone; j++) {
            if (strcmp(g->a[j]->name, citire) == 0) {
                found = 1;
                break;
            }
        }
        if (found == 0) {
            zone++;
            y = zone;
            g->a[y] = (GNode)calloc(1, sizeof(Node));
            strcpy(g->a[y]->name, citire);
        } else {
            y = j;
        }
        // inserez muchii intre locatii (graf neorientat)
        insert_edge1(g, x, y);
        insert_edge1(g, y, x);
        // citesc costul muchiei si il memorez in vectorul de muchii
        fscanf(fin, "%d", &edge[i].cost);
        edge[i].node1 = x;
        edge[i].node2 = y;
    }
}

/* citire date pentru cerinta2 - consider insula nodul 0 si corabie nodul n;
    Nod_x il consider nodul x */
void read_data_task2(Graph *g, Edge *edge, int n, int m, FILE *fin) {
    int i, k = 0, x, y;
    char citire1[20], citire2[20];
    for (i = 0; i < m; i++) {
        fscanf(fin, "%s%s%d", citire1, citire2, &edge[k].cost);
        // verific primul nod citit
        if (strcmp(citire1, "Insula") == 0) {
            x = 0;
        } else if (strcmp(citire1, "Corabie") == 0) {
            x = n;
        } else {
            // extrag din Nod_nr doar nr si il transform in numar
            char *p = strstr(citire1, "_");
            if (p != NULL) {
                x = atoi(p + 1);
            }
        }
        // verific al doilea nod citit
        if (strcmp(citire2, "Insula") == 0) {
            y = 0;
        } else if (strcmp(citire2, "Corabie") == 0) {
            y = n;
        } else {
            char *p = strstr(citire2, "_");
            if (p != NULL) {
                y = atoi(p + 1);
            }
        }
        // inserez muchie (graf orientat)
        insert_edge2(g, x, y);
        // memorez intre ce noduri am costul citit initial
        edge[k].node1 = x;
        edge[k].node2 = y;
        k++;
    }
    /* citesc primele 2 adancimi pentru Insula si Corabie
        daca inca nu am alocat memorie pt aceste noduri, aloc */
    if (g->a[0] == NULL) {
        g->a[0] = (GNode)calloc(1, sizeof(Node));
    }
    fscanf(fin, "%s%d", citire1, &g->a[0]->depth);
    if (g->a[n] == NULL) {
        g->a[n] = (GNode)calloc(1, sizeof(Node));
    }
    fscanf(fin, "%s%d", citire1, &g->a[n]->depth);
    // citesc restul adancimilor
    for (i = 1; i < n - 1; i++) {
        fscanf(fin, "%s%d", citire1, &x);
        char *p = strstr(citire1, "_");
        if (p != NULL) {
            y = atoi(p + 1);
        }
        if (g->a[y] != NULL) {
            g->a[y]->depth = x;
        }
    }
}

// algoritm de parcurgere graf prin dfs
void dfs(Graph g, int x, int *visit) {
    visit[x] = 1;
    GNode p = g.a[x];
    while (p->next != NULL) {
        if (visit[p->val] == 0) {
            dfs(g, p->val, visit);
        }
        p = p->next;
    }
}

// numara componentele conexe cu ajutorul parcurgerii dfs
void nr_conex_comp(Graph g, FILE *fout) {
    int visit[31] = {0}, conex = 0, i;
    for (i = 1; i <= g.n; i++) {
        if (visit[i] == 0) {
            conex++;
            dfs(g, i, visit);
        }
    }
    // afisez cate componente conexe contine graful
    fprintf(fout, "%d\n", conex);
}

/* gaseste nodul cu costul minim pana in acel moment;
    returneaza indexul nodului sau -1 daca nu exista;
    prin edge_cost intorc costul */
int min_edge_cost(int *cost, int *visit, int n, int *edge_cost) {
    int minim = 1000, index = -1, i;
    for (i = 1; i <= n; i++) {
        if (visit[i] == 0 && cost[i] < minim) {
            minim = cost[i];
            index = i;
        }
    }
    *edge_cost = minim;
    return index;
}

/* returneaza indexul din vectorul de muchii daca exista muchie intre 2 noduri
    sau -1 in caz contrar */
int exist_edge(Edge *edge, int m, int x, int y) {
    int i, index = -1;
    for (i = 0; i < m; i++) {
        if ((edge[i].node1 == x && edge[i].node2 == y) ||
            (edge[i].node1 == y && edge[i].node2 == x)) {
            index = i;
        }
    }
    return index;
}

// face algoritmul lui Prim pentru a gasi drumul cu cost minim din graf
void total_cost_minim(Graph g, Edge *edge, int m, FILE *fout) {
    int cost[100], visit[31] = {0}, total_cost[31] = {0};
    int i, j, k = 0;
    // initializez vectorul de cost cu o valoare mare
    for (i = 1; i <= g.n; i++) {
        cost[i] = 1000;
    }
    // parcurg nodurile din graf
    for (i = 1; i <= g.n; i++) {
        if (visit[i] == 0) {
            cost[i] = 0;
            for (j = 1; j <= g.n; j++) {
                // gasesc urmatorul nod catre care costul este cel mai mic
                int edge_cost = 0;
                int x = min_edge_cost(cost, visit, g.n, &edge_cost);
                if (x != -1) {
                    if (visit[x] == 0) {
                        // calculez costul total ce va fi afisat
                        total_cost[k] = total_cost[k] + edge_cost;
                        visit[x] = 1;
                        GNode p = g.a[x];
                        // calculez costurile fiecarui nod adiacent cu x
                        while (p->next != NULL) {
                            if (visit[p->val] == 0) {
                                int index = exist_edge(edge, m, x, p->val);
                                if (index != -1) {
                                    if (edge[index].cost < cost[p->val]) {
                                        cost[p->val] = edge[index].cost;
                                    }
                                }
                            }
                            p = p->next;
                        }
                    }
                }
            }
            /* am calculat costul total pentru o componenta conexa
                si trec la urmatoarea din graf */
            k++;
        }
    }
    // sortez crescator costul total al fiecarei comp conexe
    for (i = 0; i < k - 1; i++) {
        for (j = i + 1; j < k; j++) {
            if (total_cost[i] > total_cost[j]) {
                int aux = total_cost[i];
                total_cost[i] = total_cost[j];
                total_cost[j] = aux;
            }
        }
    }
    // afisez costul total
    for (i = 0; i < k; i++) {
        fprintf(fout, "%d\n", total_cost[i]);
    }
}

/* returneaza scorul dintre nodurile x-y sau 0;
    prin cost intorc costul muchiei x-y */
float calculate_score(Graph g, Edge *edge, int m, int x, int y, int *cost) {
    float score = 0;
    int i;
    // daca nu exista nodul y
    if (g.a[y] == NULL) {
        return score;
    }
    for (i = 0; i < m; i++) {
        if (edge[i].node1 == x && edge[i].node2 == y) {
            // aplic formula din cerinta
            score = (float)edge[i].cost / g.a[y]->depth;
            *cost = edge[i].cost;
            return score;
        }
    }
    return score;
}

// face alg lui Dijkstra pentru a afla cel mai bun drum intre 2 noduri
void dijkstra(int start, Graph g, Edge *edge, int m, int *pr, int *cost,
              FILE *fout) {
    int i, j, visit[31] = {0}, index;
    float score[31], minim, edge_score;
    // initializez scorul cu o val mare, vectorul de predecesori si cel de cost
    for (i = 0; i <= g.n; i++) {
        score[i] = 1000;
        pr[i] = -1;
        cost[i] = 0;
    }
    // start este nodul de plecare
    score[start] = 0;
    for (i = 0; i < g.n; i++) {
        minim = 1000;
        index = -1;
        // caut nodul cu scor minim
        for (j = 0; j <= g.n; j++) {
            if (visit[j] == 0 && score[j] < minim) {
                minim = score[j];
                index = j;
            }
        }
        if (index == -1) {
            break;
        }
        visit[index] = 1;
        GNode p = g.a[index];
        // calculez scorul pentru fiecare nod adiacent cu nodul cu scor minim
        while (p != NULL) {
            int edge_cost = 0;
            edge_score = calculate_score(g, edge, m, index, p->val, &edge_cost);
            // printf("%d %d %f\n",index,p->val,edge_score);
            if (edge_score != 0 && visit[p->val] == 0 && score[index] != 1000 &&
                score[index] + edge_score < score[p->val]) {
                score[p->val] = score[index] + edge_score;
                // calculez costul de la nodul start pana la toate celelalte
                cost[p->val] = cost[index] + edge_cost;
                pr[p->val] = index;
            }
            p = p->next;
        }
    }
}

/* afiseaza cel mai bun drum cu ajutorul vectorului pr obtinut prin Dijkstra;
    calculeaza adancimea minima a nodurilor parcurse */
void print_path(Graph g, int *pr, int node, int *depth, FILE *fout) {
    // daca am ajuns la nodul de start afisam Insula si ne oprim
    if (pr[node] == -1) {
        fprintf(fout, "Insula ");
        return;
    }
    // apelez recursiv pana ajung la nodul de start
    print_path(g, pr, pr[node], depth, fout);
    // daca am ajuns la nodul end afisez Corabie si ne oprim altfel afisez nodul
    if (node == g.n) {
        fprintf(fout, "Corabie");
        return;
    } else {
        fprintf(fout, "Nod_%d ", node);
        // calculez adancimea minima
        if (*depth > g.a[node]->depth) {
            *depth = g.a[node]->depth;
        }
    }
}

// rezolva si afiseaza toate taskurile cerintei 2
void resolve_task2(Graph g, Edge *edge, int m, int weight, FILE *fout) {
    int pr[31], cost[31];
    /* verific daca exista drum intre Corabie si Insula;
        cost[x] = costul pana la nodul x */
    dijkstra(g.n, g, edge, m, pr, cost, fout);
    if (cost[0] == 0) {
        fprintf(fout, "Echipajul nu poate ajunge la insula\n");
    } else {
        // verific daca exista drum intre Insula si Corabie
        dijkstra(0, g, edge, m, pr, cost, fout);
        if (cost[g.n] == 0) {
            fprintf(fout,
                    "Echipajul nu poate transporta comoara inapoi la "
                    "corabie\n");
        } else {
            /* daca exista afisez drumul parcurs, costul drumului,
                greutatea maxima a barcii <=> adancimea minima gasita
                si numarul de drumuri efectuate*/
            int depth = 1000;
            print_path(g, pr, g.n, &depth, fout);
            fprintf(fout, "\n");
            fprintf(fout, "%d\n", cost[g.n]);
            fprintf(fout, "%d\n", depth);
            int laps;
            if (weight % depth == 0) {
                laps = weight / depth;
            } else {
                laps = weight / depth + 1;
            }
            fprintf(fout, "%d", laps);
        }
    }
}

// elibereaza memoria alocata grafului
void free_graph(Graph *g) {
    int i;
    for (i = 0; i <= g->n; i++) {
        GNode p = g->a[i], next;
        while (p != NULL) {
            next = p->next;
            free(p);
            p = next;
        }
    }
}

int main(int argc, char *argv[]) {
    FILE *fin, *fout;
    fin = fopen("tema3.in", "r");
    fout = fopen("tema3.out", "w");
    // citesc nr noduri nr muchii din graf
    int n, m;
    fscanf(fin, "%d%d", &n, &m);
    // initializez graful de n noduri
    Graph g;
    init_graph(&g, n);
    Edge edge[100];
    // verific daca rezolv cerinta 1 sau 2
    if (strcmp(argv[1], "1") == 0) {
        read_data_task1(&g, edge, m, fin);
        nr_conex_comp(g, fout);
        total_cost_minim(g, edge, m, fout);
    } else {
        read_data_task2(&g, edge, n, m, fin);
        // citesc greutatea comorii
        int weight;
        fscanf(fin, "%d", &weight);
        resolve_task2(g, edge, m, weight, fout);
    }
    // eliberez memoria grafului
    free_graph(&g);
    fclose(fin);
    fclose(fout);
    return 0;
}