//#define _CRT_SECURE_NO_WARNINGS
//#include<stdio.h>
//#include<stdlib.h>
//#include<string.h>
////Considerati codul de la task - ul precedent.
////Creati un fisier in care sa aveti minim 10 obiecte de tipul structurii create.Asezarea in fisier a
////elementelor o faceti la libera alegere.
//
//struct Scuter {
//		int cod;
//		int putereKW;
//		char* marca;
//		float pret;
//	};
//typedef struct Scuter Scuter;
//
//void afisareScuter(Scuter scuter) {
//	printf("Cod: %d\n", scuter.cod);
//	printf("Putere KW: %d\n", scuter.putereKW);
//	printf("Pret: %.2f\n", scuter.pret);
//	printf("Marca: %s\n", scuter.marca);
//	
//}
//
////afisarea vectorului de scutere
//void afisareVectorScutere(Scuter* scutere, int nrScutere) {
//	for (int i = 0; i < nrScutere; i++) {
//		afisareScuter(scutere[i]);
//	}
//}
//
////functia care adauga un scuter in vectorul de scutere
//void adaugaScuterInVector(Scuter** scutere, int* nrScutere, Scuter scuterNou){
//	//alocam spatiu pentru noul vector
//	Scuter* aux = (Scuter*)malloc(sizeof(Scuter) * ((*nrScutere) + 1));
//	for (int i = 0; i < *nrScutere; i++) {
//		aux[i] = (*scutere)[i];
//	}
//	//facem shallow copy
//	aux[(*nrScutere)] = scuterNou;
//	//stergem vechiul vector
//	free(*scutere);
//	//initializam vechiul vector cu adresa din aux pe care a retinut o
//	(*scutere) = aux;
//	//crestem numarul de scutere cu +1
//	(*nrScutere)++;
//
//}
////Scrieti intr - un program C functia care sa citeasca obiectele din fisier si sa le salveze intr - un
////vector.
//
//Scuter citireScuterFisier(FILE* file) {
//	//declaram un scuter
//	Scuter s;
//	//declaram un buffer pentru a citi din fisier
//	char buffer[100];
//	//declaram un vector de posibile separatoare sep
//	char sep[4] = ",;\n";
//	//fgets primeste buffer ul in care va citii,
//	// cate elemente voi citii maxim si fisierul din care citim
//	fgets(buffer, 100, file);
//	//luam fiecare element din buffer
//	//strtok returneaza un sir de caractere, tot ce gaseste pana la separator
//	//avem un conflict de tipuri asa ca o sa folosim atoi (ascii to integer)...
//	s.cod = atoi(strtok(buffer, sep));
//	s.putereKW = atoi(strtok(NULL, sep));
//	//pentru marca noi trebuie sa ii oferim spatiu pt ca este un char*
//	char* aux;
//	aux = strtok(NULL, sep);
//	s.marca = (char*)malloc(strlen(aux) + 1);
//	strcpy(s.marca, aux);
//
//	s.pret = atof(strtok(NULL, sep));
//	return s;
//}
////functia care citeste vectorul de scutere din fisier
//Scuter* citireVectorScutereFisier(const char* numeFisier, int* nrScutereCitite) {
//	FILE* file = fopen(numeFisier, "r");
//	Scuter* scutere = NULL;
//	(*nrScutereCitite) = 0;
//	while (!feof(file)) {
//		Scuter s = citireScuterFisier(file);
//		//adaugam scuterul in vector
//		adaugaScuterInVector(&scutere, nrScutereCitite, s);
//	}
//	fclose(file);
//	return scutere;
//
//}
//
////Scrieti o functie care va salva un obiect intr - un fisier text.
//void salvareScuterInFisier(const char* numeFisier, Scuter scuter) {
//	FILE* file = fopen(numeFisier, "w");
//	if (file != NULL) {
//		fprintf(file, "%d,%d,%s,%.2f\n",
//			scuter.cod,
//			scuter.putereKW,
//			scuter.marca,
//			scuter.pret);
//		fclose(file);
//	}
//}
//
//
//
////Scrieti o functie care va salva un vector de obiecte intr - un fisier text.
////functie care salveaza un vector de scutere intr-un fisier text
//void salvareVectorScutereInFisier(const char* numeFisier, Scuter* scutere, int nrScutere) {
//	FILE* file = fopen(numeFisier, "w"); // w = write, rescrie tot
//	if (file != NULL) {
//		for (int i = 0; i < nrScutere; i++) {
//			fprintf(file, "%d,%d,%s,%.2f\n",
//				scutere[i].cod,
//				scutere[i].putereKW,
//				scutere[i].marca,
//				scutere[i].pret);
//		}
//		fclose(file);
//	}
//}
//
//
////dezalocare vector
//void dezalocareVectorScutere(Scuter** vector, int* nrScutere) {
//	//parcurgem vectorul si dereferentiem nrScutere
//	for (int i = 0; i < *nrScutere; i++) {
//		if ((*vector)[i].marca != NULL) {
//			//dezalocam zona de memorie dereferentiem vectorul si indexam
//			free((*vector)[i].marca);
//		}
//	}
//	free(*vector);
//	//dereferentiem si initializam cu NULL si 0
//	(*vector) = NULL;
//	(*nrScutere) = 0;
//}
//
//
//	int main() {
//		int nrScutere = 0;
//		Scuter* scutere = citireVectorScutereFisier("scutere.txt", &nrScutere);
//		printf("Scuterele citite din fisier sunt:\n");
//		afisareVectorScutere(scutere, nrScutere);
//		//salvam un scuter in fisier
//		salvareScuterInFisier("scuter_unic.txt", scutere[0]);
//		printf("\nPrimul scuter a fost salvat separat in scuter_unic.txt.\n");
//
//		//salvam vectorul de scutere in fisier
//		salvareVectorScutereInFisier("scutere_salvate.txt", scutere, nrScutere);
//		printf("\nVectorul de scutere a fost salvat in scutere_salvate.txt.\n");
//		//dezalocam vectorul de scutere
//		dezalocareVectorScutere(&scutere, &nrScutere);
//		return 0;
//	}
