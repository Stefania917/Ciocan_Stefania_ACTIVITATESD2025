#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Structura masina
struct StructuraMasina {
	int id;
	int nrUsi;
	float pret;
	char* model;
	char* numeSofer;
	unsigned char serie;
};
typedef struct StructuraMasina Masina;

// Nod arbore binar de cautare
struct nod {
	Masina info;
	struct nod* stanga;
	struct nod* dreapta;
};
typedef struct nod nod;

// Nod lista simplu inlantuita
struct nodLSI {
	Masina info;
	struct nodLSI* next;
};
typedef struct nodLSI nodLSI;

// Nod lista dublu inlantuita
struct nodLDI {
	Masina info;
	struct nodLDI* next;
	struct nodLDI* prev;
};
typedef struct nodLDI nodLDI;

// Citire masina din fisier
Masina citireMasinaDinFisier(FILE* file) {
	char buffer[100];
	char sep[3] = ",\n";
	if (fgets(buffer, 100, file) == NULL) {
		Masina m = { 0,0,0,NULL,NULL,0 };
		return m; // EOF
	}
	char* aux;
	Masina m1;
	aux = strtok(buffer, sep);
	m1.id = atoi(aux);
	m1.nrUsi = atoi(strtok(NULL, sep));
	m1.pret = atof(strtok(NULL, sep));
	aux = strtok(NULL, sep);
	m1.model = malloc(strlen(aux) + 1);
	strcpy(m1.model, aux);
	aux = strtok(NULL, sep);
	m1.numeSofer = malloc(strlen(aux) + 1);
	strcpy(m1.numeSofer, aux);
	m1.serie = *strtok(NULL, sep);
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

int calculeazaInaltimeArbore(nod* arbore) {
	if (arbore != NULL) {
		int inaltimeStanga = calculeazaInaltimeArbore(arbore->stanga);
		int inaltimeDreapta = calculeazaInaltimeArbore(arbore->dreapta);
		return (inaltimeStanga > inaltimeDreapta ? inaltimeStanga : inaltimeDreapta) + 1;
	}
	return 0;
}

int calculeazaGE(nod* arbore) {
	if (arbore == NULL) return 0;
	return calculeazaInaltimeArbore(arbore->stanga) - calculeazaInaltimeArbore(arbore->dreapta);
}

void rotireStanga(nod** arbore) {
	nod* aux = (*arbore)->dreapta;
	(*arbore)->dreapta = aux->stanga;
	aux->stanga = (*arbore);
	(*arbore) = aux;
}

void rotireDreapta(nod** arbore) {
	nod* aux = (*arbore)->stanga;
	(*arbore)->stanga = aux->dreapta;
	aux->dreapta = (*arbore);
	(*arbore) = aux;
}

void adaugaMasinaInArboreEchilibrat(nod** arbore, Masina masinaNoua) {
	if ((*arbore) != NULL)
	{
		if ((*arbore)->info.id < masinaNoua.id)
		{
			adaugaMasinaInArboreEchilibrat(&((*arbore)->dreapta), masinaNoua);
		}
		else
		{
			adaugaMasinaInArboreEchilibrat(&((*arbore)->stanga), masinaNoua);
		}
		int gradEchilibru = calculeazaGE(*arbore);
		if (gradEchilibru == 2)
		{
			if (calculeazaGE((*arbore)->stanga) >= 0)
			{
				rotireDreapta(arbore);
			}
			else
			{
				rotireStanga(&((*arbore)->stanga));
				rotireDreapta(arbore);
			}
		}
		else if (gradEchilibru == -2)
		{
			if (calculeazaGE((*arbore)->dreapta) <= 0)
			{
				rotireStanga(arbore);
			}
			else
			{
				rotireDreapta(&((*arbore)->dreapta));
				rotireStanga(arbore);
			}
		}
	}
	else
	{
		(*arbore) = (nod*)malloc(sizeof(nod));
		(*arbore)->info = masinaNoua;
		(*arbore)->stanga = NULL;
		(*arbore)->dreapta = NULL;
	}
}

nod* citireArboreDeMasiniDinFisier(const char* numeFisier) {
	FILE* f = fopen(numeFisier, "r");
	if (!f) return NULL;
	nod* radacina = NULL;
	while (!feof(f))
	{
		Masina m = citireMasinaDinFisier(f);
		if (m.id != 0) 
			adaugaMasinaInArboreEchilibrat(&radacina, m);
	}
	fclose(f);
	return radacina;
}

// Afisare preordine (RSD)
void afisarePreOrdineRSD(nod* radacina) {
	if (radacina != NULL)
	{
		afisareMasina(radacina->info);
		afisarePreOrdineRSD(radacina->stanga);
		afisarePreOrdineRSD(radacina->dreapta);
	}
}
// Afisare inordine (SRD)
void afisareInOrdineSRD(nod* radacina) {
	if (radacina != NULL)
	{
		afisareInOrdineSRD(radacina->stanga);
		afisareMasina(radacina->info);
		afisareInOrdineSRD(radacina->dreapta);
	}
}
// Afisare postordine (SDR)
void afisarePostOrdineSDR(nod* radacina) {
	if (radacina != NULL)
	{
		afisarePostOrdineSDR(radacina->stanga);
		afisarePostOrdineSDR(radacina->dreapta);
		afisareMasina(radacina->info);
	}
}

// Cautare masina dupa id
Masina* getMasinaByID(nod* radacina, int id) {
	if (radacina == NULL)
		return NULL;
	if (radacina->info.id == id)
		return &(radacina->info);
	if (id < radacina->info.id)
		return getMasinaByID(radacina->stanga, id);
	return getMasinaByID(radacina->dreapta, id);
}

// Functii pentru vector
// Dimensiunea vectorului trebuie sa fie suficienta, se poate apela mai intai cu NULL ptr a determina nr noduri
int determinaNumarNoduri(nod* radacina) {
	if (radacina == NULL)
		return 0;
	return 1 + determinaNumarNoduri(radacina->stanga) + determinaNumarNoduri(radacina->dreapta);
}

int indexVector = 0;

void salvareInVectorPreordine(nod* radacina, Masina* vector) {
	if (radacina != NULL)
	{
		vector[indexVector++] = radacina->info;
		salvareInVectorPreordine(radacina->stanga, vector);
		salvareInVectorPreordine(radacina->dreapta, vector);
	}
}

void salvareInVectorInordine(nod* radacina, Masina* vector) {
	if (radacina != NULL)
	{
		salvareInVectorInordine(radacina->stanga, vector);
		vector[indexVector++] = radacina->info;
		salvareInVectorInordine(radacina->dreapta, vector);
	}
}

void salvareInVectorPostordine(nod* radacina, Masina* vector) {
	if (radacina != NULL)
	{
		salvareInVectorPostordine(radacina->stanga, vector);
		salvareInVectorPostordine(radacina->dreapta, vector);
		vector[indexVector++] = radacina->info;
	}
}

// Functii pentru lista simplu inlantuita
void adaugaNodLSI(nodLSI** cap, Masina m) {
	nodLSI* nou = malloc(sizeof(nodLSI));
	nou->info = m;
	nou->next = NULL;
	if (*cap == NULL) {
		*cap = nou;
	}
	else {
		nodLSI* temp = *cap;
		while (temp->next) temp = temp->next;
		temp->next = nou;
	}
}

void salvareInListaSimpluInlantuitaPreordine(nod* radacina, nodLSI** cap) {
	if (radacina != NULL)
	{
		adaugaNodLSI(cap, radacina->info);
		salvareInListaSimpluInlantuitaPreordine(radacina->stanga, cap);
		salvareInListaSimpluInlantuitaPreordine(radacina->dreapta, cap);
	}
}

void salvareInListaSimpluInlantuitaInordine(nod* radacina, nodLSI** cap) {
	if (radacina != NULL)
	{
		salvareInListaSimpluInlantuitaInordine(radacina->stanga, cap);
		adaugaNodLSI(cap, radacina->info);
		salvareInListaSimpluInlantuitaInordine(radacina->dreapta, cap);
	}
}

void salvareInListaSimpluInlantuitaPostordine(nod* radacina, nodLSI** cap) {
	if (radacina != NULL)
	{
		salvareInListaSimpluInlantuitaPostordine(radacina->stanga, cap);
		salvareInListaSimpluInlantuitaPostordine(radacina->dreapta, cap);
		adaugaNodLSI(cap, radacina->info);
	}
}

// Functii pentru lista dublu inlantuita
void adaugaNodLDI(nodLDI** cap, nodLDI** coada, Masina m) {
	nodLDI* nou = malloc(sizeof(nodLDI));
	nou->info = m;
	nou->next = NULL;
	nou->prev = NULL;

	if (*cap == NULL) {
		*cap = nou;
		*coada = nou;
	}
	else {
		(*coada)->next = nou;
		nou->prev = *coada;
		*coada = nou;
	}
}

void salvareInListaDubluInlantuitaPreordine(nod* radacina, nodLDI** cap, nodLDI** coada) {
	if (radacina != NULL)
	{
		adaugaNodLDI(cap, coada, radacina->info);
		salvareInListaDubluInlantuitaPreordine(radacina->stanga, cap, coada);
		salvareInListaDubluInlantuitaPreordine(radacina->dreapta, cap, coada);
	}
}

void salvareInListaDubluInlantuitaInordine(nod* radacina, nodLDI** cap, nodLDI** coada) {
	if (radacina != NULL)
	{
		salvareInListaDubluInlantuitaInordine(radacina->stanga, cap, coada);
		adaugaNodLDI(cap, coada, radacina->info);
		salvareInListaDubluInlantuitaInordine(radacina->dreapta, cap, coada);
	}
}

void salvareInListaDubluInlantuitaPostordine(nod* radacina, nodLDI** cap, nodLDI** coada) {
	if (radacina != NULL)
	{
		salvareInListaDubluInlantuitaPostordine(radacina->stanga, cap, coada);
		salvareInListaDubluInlantuitaPostordine(radacina->dreapta, cap, coada);
		adaugaNodLDI(cap, coada, radacina->info);
	}
}

// Afisare lista simplu inlantuita
void afisareListaSimpluInlantuita(nodLSI* cap) {
	nodLSI* temp = cap;
	while (temp) {
		afisareMasina(temp->info);
		temp = temp->next;
	}
}

// Afisare lista dublu inlantuita
void afisareListaDubluInlantuita(nodLDI* cap) {
	nodLDI* temp = cap;
	while (temp) {
		afisareMasina(temp->info);
		temp = temp->next;
	}
}

// Dezalocare arbore
void dezalocareArboreDeMasini(nod* arbore) {
	if (arbore != NULL) {
		dezalocareArboreDeMasini(arbore->stanga);
		dezalocareArboreDeMasini(arbore->dreapta);
		free(arbore->info.model);
		free(arbore->info.numeSofer);
		free(arbore);
	}
}

// Dezalocare lista simplu inlantuita
void dezalocareListaSimpluInlantuita(nodLSI* cap) {
	while (cap) {
		nodLSI* temp = cap;
		free(temp->info.model);
		free(temp->info.numeSofer);
		cap = cap->next;
		free(temp);
	}
}

// Dezalocare lista dublu inlantuita
void dezalocareListaDubluInlantuita(nodLDI* cap) {
	while (cap) {
		nodLDI* temp = cap;
		free(temp->info.model);
		free(temp->info.numeSofer);
		cap = cap->next;
		free(temp);
	}
}




int main() {
	nod* arbore = citireArboreDeMasiniDinFisier("Masini.txt");

	printf("Afisare arbore preordine:\n");
	afisarePreOrdineRSD(arbore);

	printf("\nCautare masina cu id=3:\n");
	Masina* m = getMasinaByID(arbore, 3);
	if (m) afisareMasina(*m);
	else printf("Masina nu a fost gasita!\n");

	int nrNoduri = determinaNumarNoduri(arbore);
	printf("\nNumar noduri arbore: %d\n", nrNoduri);

	// Salvare in vector (preordine)
	Masina* vectorMasini = malloc(sizeof(Masina) * nrNoduri);
	indexVector = 0;
	salvareInVectorPreordine(arbore, vectorMasini);
	printf("\nAfisare vector masini (preordine):\n");
	for (int i = 0; i < nrNoduri; i++) {
		afisareMasina(vectorMasini[i]);
	}
	free(vectorMasini);

	// Salvare in lista simplu inlantuita (inordine)
	nodLSI* listaSimplu = NULL;
	salvareInListaSimpluInlantuitaInordine(arbore, &listaSimplu);
	printf("\nAfisare lista simplu inlantuita (inordine):\n");
	afisareListaSimpluInlantuita(listaSimplu);

	// Salvare in lista dublu inlantuita (postordine)
	nodLDI* listaDubla = NULL;
	nodLDI* coadaDubla = NULL;
	salvareInListaDubluInlantuitaPostordine(arbore, &listaDubla, &coadaDubla);
	printf("\nAfisare lista dublu inlantuita (postordine):\n");
	afisareListaDubluInlantuita(listaDubla);


	// Dezalocari
	dezalocareArboreDeMasini(arbore);
	dezalocareListaSimpluInlantuita(listaSimplu);
	dezalocareListaDubluInlantuita(listaDubla);

	return 0;
}
