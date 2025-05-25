//#define _CRT_SECURE_NO_WARNINGS
//#include <stdio.h>
//#include <stdlib.h>
//#include <string.h>
//
//struct Cladire {
//    int idUnic;
//    int nrEtaje;
//    int anulConstruirii;  // corect peste tot
//    char* adresa;
//    float suprafata;
//};
//typedef struct Cladire Cladire;
//
//struct Nod {
//    Cladire cladire;
//    struct Nod* next;
//};
//typedef struct Nod Nod;
//
//struct HashTable {
//    int dim;
//    Nod** tabela;
//};
//typedef struct HashTable HashTable;
//
//Cladire citireCladireDinFisier(FILE* file) {
//    char buffer[200];
//    char sep[3] = ",\n";
//    if (fgets(buffer, 200, file) == NULL) {
//        // Returnam o cladire "goala" daca nu mai sunt linii
//        Cladire c1 = { 0, 0, 0, NULL, 0.0f };
//        return c1;
//    }
//    char* aux;
//    Cladire c1;
//    aux = strtok(buffer, sep);
//    if (!aux) return (Cladire) { 0, 0, 0, NULL, 0.0f };
//    c1.idUnic = atoi(aux);
//    aux = strtok(NULL, sep);
//    c1.nrEtaje = aux ? atoi(aux) : 0;
//    aux = strtok(NULL, sep);
//    c1.anulConstruirii = aux ? atoi(aux) : 0;
//    aux = strtok(NULL, sep);
//    if (!aux) {
//        c1.adresa = NULL;
//    }
//    else {
//        c1.adresa = malloc(strlen(aux) + 1);
//        strcpy(c1.adresa, aux);
//    }
//    aux = strtok(NULL, sep);
//    c1.suprafata = aux ? atof(aux) : 0.0f;
//    return c1;
//}
//
//void afisareCladire(Cladire cladire) {
//    printf("ID: %d\n", cladire.idUnic);
//    printf("Nr. Etaje: %d\n", cladire.nrEtaje);
//    printf("Anul construirii: %d\n", cladire.anulConstruirii);
//    printf("Adresa: %s\n", cladire.adresa);
//    printf("Suprafata: %.2f\n\n", cladire.suprafata);
//
//}
//
////1. Sa se creeze o tabela de dispersie pentru stocarea cladirilor din Bucuresti (structura
////Cladire contine ce atribute vreti voi dar va contine obligatoriu anul construirii si un id unic).
////campul folosit pentru clusterizare este anul construirii cladirii
//
//HashTable creareTabela(int dim) {
//    HashTable ht;
//    ht.dim = dim;
//    ht.tabela = (Nod**)malloc(sizeof(Nod*) * dim);
//    for (int i = 0; i < dim; i++) {
//        ht.tabela[i] = NULL;
//    }
//    return ht;
//}
//
//int functieHash(int anulConstruirii, int dim) {
//    return anulConstruirii % dim;
//}
//
//void inserareInTabela(HashTable* ht, Cladire cladire) {
//    int index = functieHash(cladire.anulConstruirii, ht->dim);
//    Nod* nouNod = (Nod*)malloc(sizeof(Nod));
//    nouNod->cladire = cladire;
//    nouNod->next = ht->tabela[index];
//    ht->tabela[index] = nouNod;
//}
//
////2. Implementati o functie care afiseaza cladirile dintr - un cluster(construite intr - un an primit
////    ca parametru).
//
//void afisareCladiriDinCluster(HashTable ht, int an) {
//    int index = functieHash(an, ht.dim);
//    Nod* p = ht.tabela[index];
//    if (p == NULL) {
//        printf("Nu exista cladiri construite in anul %d.\n", an);
//        return;
//    }
//    printf("Cladiri construite in anul %d:\n", an);
//    while (p) {
//        if (p->cladire.anulConstruirii == an) // verificam exact anul
//            printf("ID: %d, Nr. Etaje: %d, Adresa: %s, Suprafata: %.2f\n",
//                p->cladire.idUnic, p->cladire.nrEtaje, p->cladire.adresa, p->cladire.suprafata);
//        p = p->next;
//    }
//}
//
////3. Implementati o functie care sterge o cladire pentru care se primeste id - ul si anul
////construirii.
//
//void stergeCladire(HashTable* ht, int id, int an) {
//    int index = functieHash(an, ht->dim);
//    Nod* p = ht->tabela[index];
//    Nod* prev = NULL;
//    while (p != NULL && (p->cladire.idUnic != id || p->cladire.anulConstruirii != an)) {
//        prev = p;
//        p = p->next;
//    }
//    if (p == NULL) {
//        printf("Cladirea cu ID %d si anul construirii %d nu a fost gasita.\n", id, an);
//        return;
//    }
//    if (prev == NULL) {
//        ht->tabela[index] = p->next;
//    }
//    else {
//        prev->next = p->next;
//    }
//    free(p->cladire.adresa);
//    free(p);
//    printf("Cladirea cu ID %d si anul construirii %d a fost stearsa.\n", id, an);
//}
//
////4. Implementati o functie care sterge o cladire pentru care se primeste doar id - ul cladirii.
//
//void stergeCladireDupaId(HashTable* ht, int id) {
//    for (int i = 0; i < ht->dim; i++) {
//        Nod* p = ht->tabela[i];
//        Nod* prev = NULL;
//        while (p != NULL && p->cladire.idUnic != id) {
//            prev = p;
//            p = p->next;
//        }
//        if (p != NULL) {
//            if (prev == NULL) {
//                ht->tabela[i] = p->next;
//            }
//            else {
//                prev->next = p->next;
//            }
//            free(p->cladire.adresa);
//            free(p);
//            printf("Cladirea cu ID %d a fost stearsa.\n", id);
//            return;
//        }
//    }
//    printf("Cladirea cu ID %d nu a fost gasita.\n", id);
//}
//
////6. Implementati o functie care salveaza intr - un vector toate cladirile dintr - un an primit ca
////parametru.Realizati deep copy, astfel incat elementele din vector sa fie diferentiate de cele
////din tabela de dispersie.Aveti grija la alocarea de spatiu, deoarece trebuie sa stabiliti pentru
////cate elemente veti aloca spatiu.
//
//Cladire* cladiriDinAn(HashTable ht, int an, int* numarCladiri) {
//    int index = functieHash(an, ht.dim);
//    Nod* p = ht.tabela[index];
//    int count = 0;
//    while (p) {
//        if (p->cladire.anulConstruirii == an)
//            count++;
//        p = p->next;
//    }
//    if (count == 0) {
//        *numarCladiri = 0;
//        return NULL;
//    }
//    Cladire* cladiri = (Cladire*)malloc(count * sizeof(Cladire));
//    p = ht.tabela[index];
//    int i = 0;
//    while (p) {
//        if (p->cladire.anulConstruirii == an) {
//            cladiri[i].idUnic = p->cladire.idUnic;
//            cladiri[i].nrEtaje = p->cladire.nrEtaje;
//            cladiri[i].anulConstruirii = p->cladire.anulConstruirii;
//            cladiri[i].suprafata = p->cladire.suprafata;
//            cladiri[i].adresa = (char*)malloc(strlen(p->cladire.adresa) + 1);
//            strcpy(cladiri[i].adresa, p->cladire.adresa);
//            i++;
//        }
//        p = p->next;
//    }
//    *numarCladiri = count;
//    return cladiri;
//}
//
////7. Implementati o functie care modifica anul construirii unei cladiri identificate prin ID.
////Functia primeste ca parametri : id - ul cladirii, vechiul an de constructie si noul an de
////constructie.Aveti grija se modifica valoarea atributului folosit pentru clusterizare.
//
//void modificaAnConstructie(HashTable* ht, int id, int vechiAn, int nouAn) {
//    int indexVechi = functieHash(vechiAn, ht->dim);
//    Nod* p = ht->tabela[indexVechi];
//    Nod* prev = NULL;
//    while (p != NULL && p->cladire.idUnic != id) {
//        prev = p;
//        p = p->next;
//    }
//    if (p == NULL) {
//        printf("Cladirea cu ID %d nu a fost gasita in anul %d.\n", id, vechiAn);
//        return;
//    }
//
//    p->cladire.anulConstruirii = nouAn;
//    int indexNou = functieHash(nouAn, ht->dim);
//    if (indexNou != indexVechi) {
//        if (prev == NULL) {
//            ht->tabela[indexVechi] = p->next;
//        }
//        else {
//            prev->next = p->next;
//        }
//        p->next = ht->tabela[indexNou];
//        ht->tabela[indexNou] = p;
//    }
//    printf("Anul construirii cladirii cu ID %d a fost modificat de la %d la %d.\n", id, vechiAn, nouAn);
//}
//
//void dezalocareTabela(HashTable* ht) {
//    for (int i = 0; i < ht->dim; i++) {
//        Nod* p = ht->tabela[i];
//        while (p) {
//            Nod* temp = p;
//            p = p->next;
//            free(temp->cladire.adresa);
//            free(temp);
//        }
//    }
//    free(ht->tabela);
//    ht->tabela = NULL;
//    ht->dim = 0;
//}
//
//int main() {
//    HashTable ht = creareTabela(10);
//
//    FILE* f = fopen("cladiri.txt", "r");
//    if (!f) {
//        printf("Eroare la deschiderea fisierului!\n");
//        return 1;
//    }
//
//    while (1) {
//        Cladire c = citireCladireDinFisier(f);
//        if (c.adresa == NULL || strlen(c.adresa) == 0) {
//            if (c.adresa) free(c.adresa);
//            break;
//        }
//        inserareInTabela(&ht, c);
//    }
//    fclose(f);
//
//    afisareCladiriDinCluster(ht, 2000);
//
//    stergeCladire(&ht, 2, 2001);
//    stergeCladireDupaId(&ht, 1);
//
//    int numarCladiri = 0;
//    Cladire* vectorCladiri = cladiriDinAn(ht, 2000, &numarCladiri);
//    if (vectorCladiri != NULL) {
//        printf("Cladiri din anul 2000:\n");
//        for (int i = 0; i < numarCladiri; i++) {
//            afisareCladire(vectorCladiri[i]);
//            free(vectorCladiri[i].adresa);
//        }
//        free(vectorCladiri);
//    }
//
//    modificaAnConstructie(&ht, 3, 2000, 2002);
//    afisareCladiriDinCluster(ht, 2002);
//
//    dezalocareTabela(&ht);
//
//    return 0;
//}
