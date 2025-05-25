#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct StructuraMasina {
    int id;
    int nrUsi;
    float pret;
    char* model;
    char* numeSofer;
    unsigned char serie;
};
typedef struct StructuraMasina Masina;

struct Nod
{
    Masina info;
    struct Nod* dr;
    struct Nod* st;
};
typedef struct Nod Nod;

Masina citireMasinaDinFisier(FILE* file) {
    char buffer[100];
    char sep[3] = ",\n";
    Masina m1;
    m1.model = NULL;
    m1.numeSofer = NULL;

    if (!fgets(buffer, 100, file)) {
        m1.id = -1; // Semnal ca nu s-a citit nimic
        return m1;
    }

    char* aux;
    aux = strtok(buffer, sep);
    if (aux == NULL) { m1.id = -1; return m1; }
    m1.id = atoi(aux);

    aux = strtok(NULL, sep);
    if (aux == NULL) { m1.id = -1; return m1; }
    m1.nrUsi = atoi(aux);

    aux = strtok(NULL, sep);
    if (aux == NULL) { m1.id = -1; return m1; }
    m1.pret = atof(aux);

    aux = strtok(NULL, sep);
    if (aux == NULL) { m1.id = -1; return m1; }
    m1.model = malloc(strlen(aux) + 1);
    strcpy(m1.model, aux);

    aux = strtok(NULL, sep);
    if (aux == NULL) { free(m1.model); m1.id = -1; return m1; }
    m1.numeSofer = malloc(strlen(aux) + 1);
    strcpy(m1.numeSofer, aux);

    aux = strtok(NULL, sep);
    if (aux == NULL) { free(m1.model); free(m1.numeSofer); m1.id = -1; return m1; }
    m1.serie = *aux;

    return m1;
}

void afisareMasina(Masina masina) {
    printf("Id: %d\n", masina.id);
    printf("Nr. usi : %d\n", masina.nrUsi);
    printf("Pret: %.2f\n", masina.pret);
    printf("Model: %s\n", masina.model);
    printf("Nume sofer: %s\n", masina.numeSofer);
    printf("Serie: %c\n\n", masina.serie);
}

void adaugaMasinaInArbore(Nod** nod, Masina masinaNoua) {
    if ((*nod) != NULL)
    {
        if (masinaNoua.id > (*nod)->info.id)
        {
            adaugaMasinaInArbore(&(*nod)->dr, masinaNoua);
        }
        else if (masinaNoua.id < (*nod)->info.id)
        {
            adaugaMasinaInArbore(&(*nod)->st, masinaNoua);
        }
        else {
            // id duplicat, nu adaugam
            free(masinaNoua.model);
            free(masinaNoua.numeSofer);
        }
    }
    else
    {
        (*nod) = (Nod*)malloc(sizeof(Nod));
        (*nod)->info = masinaNoua;
        (*nod)->st = NULL;
        (*nod)->dr = NULL;
    }
}

Nod* citireArboreDeMasiniDinFisier(const char* numeFisier) {
    FILE* f = fopen(numeFisier, "r");
    if (!f) {
        printf("Fisierul nu poate fi deschis!\n");
        return NULL;
    }
    Nod* nod = NULL;
    while (1)
    {
        Masina m = citireMasinaDinFisier(f);
        if (m.id == -1) break; // Nu s-au mai citit masini
        adaugaMasinaInArbore(&nod, m);
    }
    fclose(f);
    return nod;
}

void afisarePreOrdineRSD(Nod* radacina) {
    if (radacina != NULL)
    {
        afisareMasina(radacina->info);
        afisarePreOrdineRSD(radacina->st);
        afisarePreOrdineRSD(radacina->dr);
    }
}

void afisareInOrdineSRD(Nod* radacina) {
    if (radacina != NULL)
    {
        afisareInOrdineSRD(radacina->st);
        afisareMasina(radacina->info);
        afisareInOrdineSRD(radacina->dr);
    }
}

void afisarePostOrdineSDR(Nod* radacina) {
    if (radacina != NULL)
    {
        afisarePostOrdineSDR(radacina->st);
        afisarePostOrdineSDR(radacina->dr);
        afisareMasina(radacina->info);
    }
}

void dezalocareArboreDeMasini(Nod* radacina) {
    if (radacina != NULL) {
        dezalocareArboreDeMasini(radacina->st);
        dezalocareArboreDeMasini(radacina->dr);
        free(radacina->info.model);
        free(radacina->info.numeSofer);
        free(radacina);
    }
}

Masina getMasinaByID(Nod* radacina, int id) {
    Masina m;
    m.id = -1;
    m.model = NULL;
    m.numeSofer = NULL;

    if (radacina != NULL) {
        if (id < radacina->info.id) {
            return getMasinaByID(radacina->st, id);
        }
        else if (id > radacina->info.id) {
            return getMasinaByID(radacina->dr, id);
        }
        else {
            return radacina->info;
        }
    }
    return m;
}

int determinaNumarNoduri(Nod* radacina) {
    if (radacina == NULL) return 0;
    return 1 + determinaNumarNoduri(radacina->st) + determinaNumarNoduri(radacina->dr);
}

int calculeazaInaltimeArbore(Nod* radacina) {
    if (radacina == NULL) return 0;
    int inaltimeSt = calculeazaInaltimeArbore(radacina->st);
    int inaltimeDr = calculeazaInaltimeArbore(radacina->dr);
    return (inaltimeSt > inaltimeDr ? inaltimeSt : inaltimeDr) + 1;
}

Nod* subarboreCuInaltimeMaiMare(Nod* radacina) {
    if (radacina == NULL) return NULL;
    int inaltimeSt = calculeazaInaltimeArbore(radacina->st);
    int inaltimeDr = calculeazaInaltimeArbore(radacina->dr);
    if (inaltimeSt >= inaltimeDr) {
        return radacina->st;
    }
    else {
        return radacina->dr;
    }
}

Nod* subarboreCuMaiMulteNoduri(Nod* radacina) {
    if (radacina == NULL) return NULL;
    int nrSt = determinaNumarNoduri(radacina->st);
    int nrDr = determinaNumarNoduri(radacina->dr);
    if (nrSt >= nrDr) {
        return radacina->st;
    }
    else {
        return radacina->dr;
    }
}

Nod* gasesteMinim(Nod* rad) {
    while (rad && rad->st != NULL) {
        rad = rad->st;
    }
    return rad;
}

Nod* stergeNod(Nod* rad, int id) {
    if (rad == NULL) return NULL;

    if (id < rad->info.id) {
        rad->st = stergeNod(rad->st, id);
    }
    else if (id > rad->info.id) {
        rad->dr = stergeNod(rad->dr, id);
    }
    else {
        // nodul de sters gasit
        if (rad->st == NULL && rad->dr == NULL) {
            // fara copii
            free(rad->info.model);
            free(rad->info.numeSofer);
            free(rad);
            return NULL;
        }
        else if (rad->st == NULL) {
            // un copil drept
            Nod* temp = rad->dr;
            free(rad->info.model);
            free(rad->info.numeSofer);
            free(rad);
            return temp;
        }
        else if (rad->dr == NULL) {
            // un copil stang
            Nod* temp = rad->st;
            free(rad->info.model);
            free(rad->info.numeSofer);
            free(rad);
            return temp;
        }
        else {
            // doi copii: gasim succesorul (cel mai mic nod din dreapta)
            Nod* temp = gasesteMinim(rad->dr);

            // eliberam stringurile radacinii curente
            free(rad->info.model);
            free(rad->info.numeSofer);

            // copiem info din succesor (copiere profunda)
            rad->info.id = temp->info.id;
            rad->info.nrUsi = temp->info.nrUsi;
            rad->info.pret = temp->info.pret;
            rad->info.model = malloc(strlen(temp->info.model) + 1);
            strcpy(rad->info.model, temp->info.model);
            rad->info.numeSofer = malloc(strlen(temp->info.numeSofer) + 1);
            strcpy(rad->info.numeSofer, temp->info.numeSofer);
            rad->info.serie = temp->info.serie;

            // stergem nodul succesor
            rad->dr = stergeNod(rad->dr, temp->info.id);
        }
    }
    return rad;
}

int main() {
    Nod* rad = citireArboreDeMasiniDinFisier("masini.txt");

    printf("Afisare preordine (RSD):\n");
    afisarePreOrdineRSD(rad);

    printf("Afisare in ordine (SRD):\n");
    afisareInOrdineSRD(rad);

    printf("\nMasina gasita cu ID 6:\n");
    Masina gasita = getMasinaByID(rad, 6);
    if (gasita.id != -1) {
        afisareMasina(gasita);
    }
    else {
        printf("Masina cu ID 6 nu a fost gasita.\n");
    }

    printf("Numar noduri in arbore: %d\n", determinaNumarNoduri(rad));

    // Exemplu stergere nod cu id = 6
    printf("Stergem nodul cu ID 6...\n");
    rad = stergeNod(rad, 6);

    printf("Afisare in ordine dupa stergere (SRD):\n");
    afisareInOrdineSRD(rad);

    // Subarbore cu inaltime mai mare
    Nod* subInaltime = subarboreCuInaltimeMaiMare(rad);
    printf("\nSubarbore cu inaltime mai mare:\n");
    if (subInaltime != NULL) {
        afisareMasina(subInaltime->info);
    }
    else {
        printf("Subarbore nul\n");
    }

    // Subarbore cu mai multe noduri
    Nod* subNoduri = subarboreCuMaiMulteNoduri(rad);
    printf("\nSubarbore cu mai multe noduri:\n");
    if (subNoduri != NULL) {
        afisareMasina(subNoduri->info);
    }
    else {
        printf("Subarbore nul\n");
    }

    dezalocareArboreDeMasini(rad);
    return 0;
}
