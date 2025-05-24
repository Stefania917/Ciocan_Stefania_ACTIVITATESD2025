#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//Se considera lista dublu inlantuita realizata la seminar

struct StructuraMasina {
	int id;
	int nrUsi;
	float pret;
	char* model;
	char* numeSofer;
	unsigned char serie;
};
typedef struct StructuraMasina Masina;

struct Nod {
	Masina masina;
	struct Nod* next;
	struct Nod* prev;
};
typedef struct Nod Nod;

struct ListaDubla {
	Nod* first;
	Nod* last;
	int nrNoduri;
};
typedef struct ListaDubla ListaDubla;

Masina citireMasinaDinFisier(FILE* file) {
	char buffer[100];
	char sep[3] = ",\n";
	fgets(buffer, 100, file);
	char* aux;
	Masina m1;
	aux = strtok(buffer, sep);
	m1.id = atoi(aux);
	m1.nrUsi = atoi(strtok(NULL, sep));
	m1.pret = atof(strtok(NULL, sep));
	aux = strtok(NULL, sep);
	m1.model = malloc(strlen(aux) + 1);
	strcpy_s(m1.model, strlen(aux) + 1, aux);

	aux = strtok(NULL, sep);
	m1.numeSofer = malloc(strlen(aux) + 1);
	strcpy_s(m1.numeSofer, strlen(aux) + 1, aux);

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

void afisareListaMasiniDeLaInceput(ListaDubla lista) {
	Nod* p = lista.first;
	while (p) {
		afisareMasina(p->masina);
		p = p->next;
	}
}

void afisareListaMasiniDeLaSfarsit(ListaDubla lista) {
	Nod* p = lista.last;
	while (p) {
		afisareMasina(p->masina);
		p = p->prev;
	}
}
void adaugaMasinaInLista(ListaDubla* lista, Masina masinaNoua) {
	Nod* nou = (Nod*)malloc(sizeof(Nod));
	nou->masina = masinaNoua; //shallow copy
	nou->next = NULL;
	nou->prev = lista->last;
	if (lista->last != NULL) {
		lista->last->next = nou;
	}
	else {
		lista->first = nou;
	}
	lista->last = nou;
	lista->nrNoduri++;
}

void adaugaLaInceputInLista(ListaDubla* lista, Masina masinaNoua) {
	Nod* nou = (Nod*)malloc(sizeof(Nod));
	nou->masina = masinaNoua; //shallow copy
	nou->next = lista->first;
	nou->prev = NULL;
	if (lista->first != NULL) {
		lista->first->prev = nou;
	}
	else {
		lista->last = nou;
	}
	lista->first = nou;
	lista->nrNoduri++;
}

ListaDubla citireLDMasiniDinFisier(const char* numeFisier) {
	//functia primeste numele fisierului, il deschide si citeste toate masinile din fisier
	//prin apelul repetat al functiei citireMasinaDinFisier()
	//ATENTIE - la final inchidem fisierul/stream-ul
	FILE* f = fopen(numeFisier, "r");
	ListaDubla lista;
	lista.first = NULL;
	lista.last = NULL;
	lista.nrNoduri = 0;
	while (!feof(f)) {
		adaugaMasinaInLista(&lista, citireMasinaDinFisier(f));
	}
	fclose(f);
	return lista;
}

void dezalocareLDMasini(ListaDubla* lista) {
	Nod* p = lista->first;
	while (p) {
		Nod* aux = p;
		p = p->next;
		if (aux->masina.model) {
			free(aux->masina.model);
		}
		if (aux->masina.numeSofer) {
			free(aux->masina.numeSofer);
		}
		free(aux);
	}
	lista->first = NULL;
	lista->last = NULL;
	lista->nrNoduri = 0;
}

float calculeazaPretMediu(ListaDubla lista) {
	if (lista.nrNoduri > 0) {
		float suma = 0;
		Nod* p = lista.first;
		while (p) {
			suma += p->masina.pret;
			p = p->next;
		}
		return suma / lista.nrNoduri;
	}
	return 0;
}

void stergeMasinaDupaID(ListaDubla* lista, int id) {
	if (lista->first == NULL) {
		return;
	}
	Nod* p = lista->first;
	while (p != NULL && p->masina.id != id) {
		p = p->next;
	}
	if (p == NULL) {
		return;
	}
	if (p->prev == NULL) {
		lista->first = p->next;
		if (lista->first) {
			lista->first->prev = NULL;
		}
	}
	else {
		p->prev->next = p->next;
	}
	if (p->next != NULL) {
		p->next->prev = p->prev;
	}
	else {
		lista->last = p->prev;
	}
	if (p->masina.model) {
		free(p->masina.model);
	}
	if (p->masina.numeSofer) {
		free(p->masina.numeSofer);
	}
	free(p);
	lista->nrNoduri--;
}

char* getNumeSoferMasinaScumpa(ListaDubla lista) {
	if (lista.first) {
		Nod* max = lista.first;
		Nod* p = lista.first->next;
		while (p) {
			if (p->masina.pret > max->masina.pret) {
				max = p;
			}
			p = p->next;
		}
		char* nume = (char*)malloc(strlen(max->masina.numeSofer) + 1);
		strcpy_s(nume, strlen(max->masina.numeSofer) + 1, max->masina.numeSofer);
		return nume;
	}
	else {
		return NULL;
	}
}

//1. Implementati o functie care sterge un nod de pe o pozitie data ca parametru. Daca lista
//are mai putine noduri decat index - ul dat, nu se realizeaza stergerea;

void stergeMasinaDupaPozitie(ListaDubla* lista, int index) {
	if (index < 0 || index >= lista->nrNoduri) {
		return; // Index invalid
	}
	Nod* p = lista->first;
	for (int i = 0; i < index; i++) {
		p = p->next;
	}
	if (p->prev == NULL) {
		lista->first = p->next;
		if (lista->first) {
			lista->first->prev = NULL;
		}
	}
	else {
		p->prev->next = p->next;
	}
	if (p->next != NULL) {
		p->next->prev = p->prev;
	}
	else {
		lista->last = p->prev;
	}
	if (p->masina.model) {
		free(p->masina.model);
	}
	if (p->masina.numeSofer) {
		free(p->masina.numeSofer);
	}
	free(p);
	lista->nrNoduri--;
}

//2. Implementati o functie care sa insereze elementele in cadrul listei dublu inlantuite astfel
//incat acestea a fie sortate crescator dupa un camp la alegerea voastra;
void insereazaMasinaSortata(ListaDubla* lista, Masina masinaNoua) {
	Nod* nou = (Nod*)malloc(sizeof(Nod));
	nou->masina = masinaNoua; //shallow copy
	nou->next = NULL;
	nou->prev = NULL;
	if (lista->first == NULL) {
		// Lista este goala
		lista->first = nou;
		lista->last = nou;
		lista->nrNoduri++;
		return;
	}
	Nod* p = lista->first;
	while (p != NULL && p->masina.pret < masinaNoua.pret) {
		p = p->next;
	}
	if (p == lista->first) {
		// Inserare la inceput
		nou->next = lista->first;
		lista->first->prev = nou;
		lista->first = nou;
	}
	else if (p == NULL) {
		// Inserare la sfarsit
		nou->prev = lista->last;
		lista->last->next = nou;
		lista->last = nou;
	}
	else {
		// Inserare in mijloc
		nou->prev = p->prev;
		nou->next = p;
		p->prev->next = nou;
		p->prev = nou;
	}
	lista->nrNoduri++;
}

//3. Realizati parcurgerile pentru lista dublu inlantuita, astfel incat sa afiseze elementele
//sortate crescator dar si sortate descrescator;
void afisareListaMasiniSortateCrescator(ListaDubla lista) {
	Nod* p = lista.first;
	while (p) {
		afisareMasina(p->masina);
		p = p->next;
	}
}

//4. Implementati o functie care salveaza intr - o lista simplu inlantuita toate obiectele care
//indeplinesc o conditie stabilita de voi.Realizati deep copy, astfel incat elementele din
//listasimplu inlantuita sa fie diferite de cele din lista dublu inlantuita.Observati diferenta
//dintre utilizarea de lista inlantuita si vector(task - ul precedent);
ListaDubla filtreazaMasiniDupaPret(ListaDubla lista, float pretMinim) {
	ListaDubla listaFiltrata;
	listaFiltrata.first = NULL;
	listaFiltrata.last = NULL;
	listaFiltrata.nrNoduri = 0;
	Nod* p = lista.first;
	while (p) {
		if (p->masina.pret >= pretMinim) {
			Masina masinaNoua;
			masinaNoua.id = p->masina.id;
			masinaNoua.nrUsi = p->masina.nrUsi;
			masinaNoua.pret = p->masina.pret;
			masinaNoua.model = malloc(strlen(p->masina.model) + 1);
			strcpy_s(masinaNoua.model, strlen(p->masina.model) + 1, p->masina.model);
			masinaNoua.numeSofer = malloc(strlen(p->masina.numeSofer) + 1);
			strcpy_s(masinaNoua.numeSofer, strlen(p->masina.numeSofer) + 1, p->masina.numeSofer);
			masinaNoua.serie = p->masina.serie;
			adaugaMasinaInLista(&listaFiltrata, masinaNoua);
		}
		p = p->next;
	}
	return listaFiltrata;
}

//5. Implementati o functie care primeste lista dublu inlantuita si doua pozitii.In cadrul
//functiei trebuie sa interschimbati elementele de pe cele doua pozitii din lista primita.
//Interschimbati informatiile utile din cele doua noduri.

void interschimbaElemente(ListaDubla* lista, int index1, int index2) {
	if (index1 < 0 || index1 >= lista->nrNoduri || index2 < 0 || index2 >= lista->nrNoduri) {
		return; // Indici invalidi
	}
	if (index1 == index2) {
		return; // Nu este necesar sa schimbam acelasi element
	}
	Nod* nod1 = lista->first;
	Nod* nod2 = lista->first;
	for (int i = 0; i < index1; i++) {
		nod1 = nod1->next;
	}
	for (int i = 0; i < index2; i++) {
		nod2 = nod2->next;
	}
	Masina temp = nod1->masina;
	nod1->masina = nod2->masina;
	nod2->masina = temp;
}





int main() {
	ListaDubla lista = citireLDMasiniDinFisier("masini.txt");
	afisareListaMasiniDeLaInceput(lista);
	printf("----------------------------\n");
	stergeMasinaDupaID(&lista, 10);
	stergeMasinaDupaID(&lista, 9);
	stergeMasinaDupaID(&lista, 8);
	stergeMasinaDupaID(&lista, 7);
	stergeMasinaDupaID(&lista, 6);
	stergeMasinaDupaID(&lista, 5);
	stergeMasinaDupaID(&lista, 4);
	stergeMasinaDupaID(&lista, 3);
	stergeMasinaDupaID(&lista, 2);
	stergeMasinaDupaID(&lista, 1);
	afisareListaMasiniDeLaSfarsit(lista);

	printf("\nPretul mediu al masinilor este %.2f.\n", calculeazaPretMediu(lista));

	char* numeSofer = getNumeSoferMasinaScumpa(lista);
	printf("Soferul cu cea mai scumpa masina este: %s\n", numeSofer);
	if (numeSofer) {
		free(numeSofer);
	}
	
	dezalocareLDMasini(&lista);

	// Testare stergere dupa pozitie
	ListaDubla lista2 = citireLDMasiniDinFisier("masini.txt");
	afisareListaMasiniDeLaInceput(lista2);
	stergeMasinaDupaPozitie(&lista2, 2); // Sterge al treilea element (index 2)
	afisareListaMasiniDeLaInceput(lista2);
	dezalocareLDMasini(&lista2);

	// Testare inserare sortata
	Masina masinaNoua;
	masinaNoua.id = 11;
	masinaNoua.nrUsi = 4;
	masinaNoua.pret = 15000.0;
	masinaNoua.model = malloc(20);
	strcpy_s(masinaNoua.model, 20, "Dacia Logan");
	masinaNoua.numeSofer = malloc(20);
	strcpy_s(masinaNoua.numeSofer, 20, "Ion Popescu");
	masinaNoua.serie = 'D';
	insereazaMasinaSortata(&lista, masinaNoua);
	afisareListaMasiniSortateCrescator(lista);

	// Testare filtrare masini dupa pret
	ListaDubla listaFiltrata = filtreazaMasiniDupaPret(lista, 10000.0);
	afisareListaMasiniDeLaInceput(listaFiltrata);
	dezalocareLDMasini(&listaFiltrata);

	// Testare interschimbare elemente
	interschimbaElemente(&lista, 0, 1); // Interschimbam primele doua masini
	afisareListaMasiniDeLaInceput(lista);
	dezalocareLDMasini(&lista);


	

	return 0;
}
