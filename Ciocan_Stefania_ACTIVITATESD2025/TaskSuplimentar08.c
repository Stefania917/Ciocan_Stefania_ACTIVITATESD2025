#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//Sa se realizeze o aplicatie in limbajul C, care sa ajute personalul de la Unitatea de Primiri
//Urgente(UPU) in gestiunea pacientilor.
//Fiecare pacient care este adus la UPU are un grad prin care se specifica, cat de urgent si grav
//este.Pacientii care sunt adusi cu ambulanta au gradul de urgenta mult mai mare.deoarece
//sunt intr - o situatie foarte grava.Sa se realizeze structura Pacient, si pe langa informatiile
//specifice unui pacient sa adaugati si acest grad de urgenta.

struct Pacient {
	int id;
	char* nume;
	char* prenume;
	int varsta;
	char* diagnostic;
	int gradUrgenta; // cu cat gradul e mai mare cu atat e mai urgent

};
typedef struct Pacient Pacient;


struct Heap {
	int lungime;
	Pacient* vector;
	int nrPacienti;
};
typedef struct Heap Heap;

Pacient citirePacientDinFisier(FILE* file) {
	char buffer[100];
	char sep[3] = ",\n";
	fgets(buffer, 100, file);
	char* aux;
	Pacient p1;
	aux = strtok(buffer, sep);
	p1.id = atoi(aux);
	aux = strtok(NULL, sep);
	p1.nume = malloc(strlen(aux) + 1);
	strcpy_s(p1.nume, strlen(aux) + 1, aux);
	aux = strtok(NULL, sep);
	p1.prenume = malloc(strlen(aux) + 1);
	strcpy_s(p1.prenume, strlen(aux) + 1, aux);
	p1.varsta = atoi(strtok(NULL, sep));
	aux = strtok(NULL, sep);
	p1.diagnostic = malloc(strlen(aux) + 1);
	strcpy_s(p1.diagnostic, strlen(aux) + 1, aux);
	p1.gradUrgenta = atoi(strtok(NULL, sep));
	return p1;
}
void afisarePacient(Pacient pacient) {
	printf("Id: %d\n", pacient.id);
	printf("Nume: %s\n", pacient.nume);
	printf("Prenume: %s\n", pacient.prenume);
	printf("Varsta: %d\n", pacient.varsta);
	printf("Diagnostic: %s\n", pacient.diagnostic);
	printf("Grad de urgenta: %d\n\n", pacient.gradUrgenta);
}

Heap initializareHeap(int lungime) {
	Heap heap;
	heap.lungime = lungime;
	heap.nrPacienti = 0;
	heap.vector = (Pacient*)malloc(sizeof(Pacient) * lungime);
	return heap;
}

//Sa se implementeze modulul care sã permitã preluarea pacientilor cu grad de urgenta foarte
//mare chiar daca acestia au venit printre ultimele persoane.In acest mod personalul de la
//UPU poate sa se ocupe de situatiile foarte grave, iar pacientii care nu sunt in stare grava pot
//astepta pana cand sunt preluate situatiile urgente

void filtreazaHeap(Heap heap, int pozitieNod) {
	int pozFiuSt = 2 * pozitieNod + 1;
	int pozFiuDr = 2 * pozitieNod + 2;
	int pozMax = pozitieNod;
	if (pozFiuSt < heap.nrPacienti && heap.vector[pozFiuSt].gradUrgenta > heap.vector[pozMax].gradUrgenta) {
		pozMax = pozFiuSt;
	}
	if (pozFiuDr < heap.nrPacienti && heap.vector[pozFiuDr].gradUrgenta > heap.vector[pozMax].gradUrgenta) {
		pozMax = pozFiuDr;
	}
	if (pozMax != pozitieNod) {
		Pacient aux = heap.vector[pozitieNod];
		heap.vector[pozitieNod] = heap.vector[pozMax];
		heap.vector[pozMax] = aux;
		filtreazaHeap(heap, pozMax);
	}
}

Heap citireHeapDePacientiDinFisier(const char* numeFisier) {
	FILE* file = fopen(numeFisier, "r");
	Heap heap = initializareHeap(10);
	while (!feof(file)) {
		heap.vector[heap.nrPacienti++] = citirePacientDinFisier(file);
	}
	fclose(file);
	for (int i = (heap.nrPacienti - 2) / 2; i >= 0; i--) {
		filtreazaHeap(heap, i);
	}
	return heap;
}

Pacient extragePacient(Heap* heap) {
	if (heap->nrPacienti > 0) {
		Pacient aux = heap->vector[0];
		heap->vector[0] = heap->vector[heap->nrPacienti - 1];
		heap->vector[heap->nrPacienti - 1] = aux;
		heap->nrPacienti--;
		for (int i = (heap->nrPacienti - 2) / 2; i >= 0; i--) {
			filtreazaHeap(*heap, i);
		}
		return aux; //shallow copy
	}
}

void afisareHeap(Heap heap) {
	for (int i = 0; i < heap.nrPacienti; i++) {
		afisarePacient(heap.vector[i]);
	}
}

void dezalocareHeap(Heap heap) {
	for (int i = 0; i < heap.nrPacienti; i++) {
		free(heap.vector[i].nume);
		free(heap.vector[i].prenume);
		free(heap.vector[i].diagnostic);
	}
	free(heap.vector);
}

int main() {
	Heap heap = citireHeapDePacientiDinFisier("pacienti.txt");
	printf("Pacientii din heap:\n");
	afisareHeap(heap);

	printf("Extragem pacientul cu grad de urgenta foarte mare:\n");
	Pacient pacientExtrage = extragePacient(&heap);
	afisarePacient(pacientExtrage);

	printf("Pacientii din heap dupa extragere:\n");
	afisareHeap(heap);

	dezalocareHeap(heap);

	return 0;
}