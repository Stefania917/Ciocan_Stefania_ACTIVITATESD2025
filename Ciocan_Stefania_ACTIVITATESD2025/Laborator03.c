//se foloseste CRT SECURE NO WARNINGS pentru a putea folosi strcpy fara sa mai scriem _s
// sau scanf fara _s si avem nevoie pt strtok
#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//trebuie neaparat sa folosim fisierul txt cu masini

//structura pentru masina
struct StructuraMasina {
	int id;
	int nrUsi;
	float pret;
	char* model;
	char* numeSofer;
	unsigned char serie;
};
//am definit un alias pentru structura masina
// ca sa nu mai fim nevoiti sa scriem de fiecare data StructuraMasina scriem direct mainsa
//am definit acest alias ca sa scriem mai putin
//compilatorul scrie la fel
typedef struct StructuraMasina Masina;

//functia pentru afisarea unei masini
void afisareMasina(Masina masina) {
	printf("ID:%d\n", masina.id);
	printf("Numar usi:%d\n", masina.nrUsi);
	printf("Pret:%.2f\n", masina.pret);
	printf("Model:%s\n", masina.model);
	printf("Nume sofer:%s\n", masina.numeSofer);
	printf("Serie:%c\n\n", masina.serie);
}

//afisarea vectorului de masini
void afisareVectorMasini(Masina* masini, int nrMasini) {
	for (int i = 0; i < nrMasini; i++) {
		afisareMasina(masini[i]);
	}
}

//functie care adauga o masina in vectorul de masini
//vectorul de masini este un pointer la pointer
// dar si nr de masini este transmis prin pointer
//masinaNoua este masina care se adauga in vector
void adaugaMasinaInVector(Masina** masini, int* nrMasini, Masina masinaNoua) {
	//alocam spatiu pentru noul vectori
	//o sa am un vector de masini inmultiti cu cate masini aveam inainte +1
	//nr masini este dereferentiat si o prioritizam
	Masina* aux = (Masina*)malloc(sizeof(Masina) * ((*nrMasini) + 1));
	for (int i = 0; i < *nrMasini; i++) {
		aux[i] = (*masini)[i];
	}
	//facem shallow copy
	aux[(*nrMasini)] = masinaNoua;
	//stergem vechiul vector
	free(*masini);
	//initializam vechiul vector cu adresa din aux pe care a retinut o
	(*masini) = aux;
	//crestem numarul de masini cu +1
	//++ are prioritate mai mare decat dereferentierea
	(*nrMasini)++;
}

//functie care citeste o masina din fisier
//in fisier avem mai multe masini, vreau sa imi citeasca fiecare masina
//daca as deschide cu fopen atunci mi ar citi doar prima masina
//eu primesc un pointer la FILE ca sa nu deschid de fiecare data fisierul
Masina citireMasinaFisier(FILE* file) {
	//declaram o masina m
	Masina m;
	//am nevoie de un buffer in care sa citesc
	//buffer ul il aloc pe stiva de memorie a functiei citireMasinaFisier
	char buffer[100];
	//declaram o lista de posibile separoate sep
	char sep[4] = ",;\n";
	//fgets primeste buffer ul in care va citii,
	// cate elemente voi citii maxim si fisierul din care citim
	fgets(buffer, 100, file);
	//luam fiecare element din buffer
	//strtok returneaza un sir de caractere, tot ce gaseste pana la separator
	//avem un conflict de tipuri asa ca o sa folosim atoi (ascii to integer)...
	m.id=atoi(strtok(buffer, sep));
	//nu ii dau din nou buffer, ii dam NULL ca sa se uite la ultimul apel sa vada unde a ramas
	//daca ii dam buffer atunci o sa ne citeasca de la inceput de la 1 si nu vrem asta
	m.nrUsi = atoi(strtok(NULL, sep));
	m.pret = atof(strtok(NULL, sep));
	//pentru model noi trebuie sa ii oferim spatiu pt ca este un char*
	//luam un auxiliar aux declarat in stiva, cand functia se termina aux dispare
	char* aux;
	//strtok returneaza un char pointer deci asa se scrie
	aux = strtok(NULL, sep);
	//alocam spatiu pentru model
	m.model = (char*)malloc(strlen(aux) + 1);
	//copiem (destinatia, sursa)
	strcpy(m.model, aux);
	//pt numele soferului facem la fel
	aux = strtok(NULL, sep);
	//alocam spatiu pentru nume sofer
	m.numeSofer = (char*)malloc(strlen(aux) + 1);
	//copiem (destinatia, sursa)
	strcpy(m.numeSofer, aux);
	//pentru serie pe noi ne intereseaza ce avem pe pozitia 0 si il vom indexa
	m.serie = strtok(NULL, sep)[0];
	return m;
}

//functie care citeste un vector de masini din fisier
Masina* citireVectorMasiniFisier(const char* numeFisier, int* nrMasiniCitite) {
	//deschidem fisierul pe care il citim in mode read r
	FILE* file = fopen(numeFisier, "r");
	//declaram un vector de masini
	Masina* masini = NULL;
	//initializez dar trebuie sa dereferntiez nrMasiniCitite
	(*nrMasiniCitite) = 0;
	//trebuie sa apelez adaugaMasinaInVector de cate ori? pana la sfarsitul fisierului
	//luam un loop cat timp .... pana am ajuns la sfarsitul fisierului file 
	while (!feof(file)) {
		//vom citii o masina pe care o vom adauga in vector
		//trebuie sa ii dau adresa de masini: &masini
		adaugaMasinaInVector(&masini, nrMasiniCitite, citireMasinaFisier(file));
	}
	//!!!!!!!!!!ATENTIE!!!!! Sa nu uitam sa inchidem fisierul
	fclose(file);
	//returnam vectorul de masini
	return masini;
}

void dezalocareVectorMasini(Masina** vector, int* nrMasini) {
	//parcurgem vectorul si dereferentiem nrMasini
	for (int i = 0; i < *nrMasini; i++) {
		if ((*vector)[i].model != NULL) {
			//dezalocam zona de memorie dereferentiem vectorul si indexam
			free((*vector)[i].model);
		}
		if ((*vector)[i].numeSofer != NULL) {
			free((*vector)[i].numeSofer);
		}
	}
	free(*vector);
	//dereferentiem si initializam cu NULL si 0
	(*vector) = NULL;
	(*nrMasini) = 0;
	//nu mai avem masini si nici vector
}

int main() {
	int nrMasini = 0;
	Masina* masini = citireVectorMasiniFisier("masini.txt", &nrMasini);
	afisareVectorMasini(masini, nrMasini);
	dezalocareVectorMasini(&masini, &nrMasini);
	return 0;
}