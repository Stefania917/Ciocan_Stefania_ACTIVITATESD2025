#include<stdio.h>
#include<malloc.h>

//Considerati codul de la task-ul precedent.
struct Scuter {
		int cod;
		int putereKW;
		char* marca;
		float pret;
	};

//initializare
struct Scuter initializare(int cod, int putereKW, const char* marca, float pret) {
	struct Scuter s;
	s.cod = cod;
	s.putereKW = putereKW;
	//1.Alocare de memorie
	s.marca = (char*)malloc(sizeof(char) * (strlen(marca) + 1));
	//2.Copierea sirului de caractere
	strcpy_s(s.marca, strlen(marca) + 1, marca);
	//primul caracter este destinatia, al doilea este cate caractere copiez, al treilea este sursa

	s.pret = pret;
	return s;
	
};

//Afisare

void afisare(struct Scuter s) {
	if (s.marca != NULL) {
		printf("%d. Scuterul marca %s are puterea de %d KW si costa %5.2f RON.\n",
			s.cod, s.marca, s.putereKW, s.pret);
	}
	else {
		printf("%d. Scuterul are puterea de %d KW si costa %5.2f RON.\n",
			s.cod, s.putereKW, s.pret);
	}
}

//Realizati o functie care afiseaza un vector cu obiecte.
//Functia de afisare vector primeste un vector de scutere si numarul de elemente
void afisareVector(struct Scuter* vector, int nrElemente) {
	//parcurgem for-ul si pt fiecare element din vector apelam functia de afisare
	for (int i = 0; i < nrElemente; i++) {
		afisare(vector[i]);
	}
}
 
//Realizati o functie care va creea un nou vector în care va copia dintr-un vector primit ca parametru
// obiectele care indeplinesc o anumita conditie.
// Stabiliti voi conditia in functie de un atribut sau doua atribute.

void copiazaScuterePuterniceSiIeftine(struct Scuter* vector, int nrElemente, int putereMinima, float pretMaxim, struct Scuter** vectorNou, int* dimensiune) {
	//nu stim dimensiunea asa ca o initializam cu 0
	*dimensiune = 0;
	//parcurgem vectorul si vedem cate scutere indeplinesc conditiile
	for (int i = 0; i < nrElemente; i++) {
		if (vector[i].putereKW >= putereMinima && vector[i].pret <= pretMaxim) {
			(*dimensiune)++;
		}
	}
	//verificam daca vectorul este NULL, daca nu este NULL atunci il dezalocam
	if ((*vectorNou) != NULL) {
		free(*vectorNou);
	}
	//alocam spatiu pentru vectorul nou
	*vectorNou = (struct Scuter*)malloc(sizeof(struct Scuter) * (*dimensiune));
	int k = 0;
	for (int i = 0; i < nrElemente; i++) {
		if (vector[i].putereKW >= putereMinima && vector[i].pret <= pretMaxim) {
			(*vectorNou)[k] = vector[i];
			//fac deep copy
			(*vectorNou)[k].marca = (char*)malloc(sizeof(char) * (strlen(vector[i].marca) + 1));
			strcpy_s((*vectorNou)[k].marca, strlen(vector[i].marca) + 1, vector[i].marca);
			//k creste ca urmatorul scuter sa fie copiat pe pozitia k+1
			k++;
		}
	}
}

//Realizati o functie care muta intr-un nou vector obiectele care indeplinesc o alta conditie fata de cerinta precedenta.

void mutaScutereleScumpe(struct Scuter** vector, int* nrElemente, float pretMinim, struct Scuter** vectorNou, int* dimensiuneNoua) {
	//nu stim dimensiunea asa ca o initializam cu 0
	*dimensiuneNoua = 0;
	//parcurgem vectorul si vedem cate scutere indeplinesc conditiile
	for (int i = 0; i < *nrElemente; i++) {
		if ((*vector)[i].pret >= pretMinim) {
			(*dimensiuneNoua)++;
		}
	}
	//verificam daca vectorul este NULL, daca nu este NULL atunci il dezalocam
	if ((*vectorNou) != NULL) {
		free(*vectorNou);
	}
	//alocam spatiu pentru vectorul nou
	*vectorNou = (struct Scuter*)malloc(sizeof(struct Scuter) * (*dimensiuneNoua));
	int k = 0;
	for (int i = 0; i < *nrElemente; i++) {
		if ((*vector)[i].pret >= pretMinim) {
			(*vectorNou)[k] = (*vector)[i];
			//fac deep copy
			(*vectorNou)[k].marca = (char*)malloc(sizeof(char) * (strlen((*vector)[i].marca) + 1));
			strcpy_s((*vectorNou)[k].marca, strlen((*vector)[i].marca) + 1,(*vector)[i].marca);
			k++;
		}
	}
	//dezalocam vectorul vechi
	free(*vector);
	//facem vectorul vechi NULL ca sa nu mai retina nimic
	*vector = NULL;
	//si dimensiunea veche 0
	*nrElemente = 0;
}

//Realizati o functie care concateneaza doi vectori.

void concateneazaScutere(struct Scuter* vector1, int dimensiune1, struct Scuter* vector2, int dimensiune2, struct Scuter** vectorNou, int* dimensiuneNoua) {
	// Alocãm memorie pentru vectorul nou 
	*dimensiuneNoua = dimensiune1 + dimensiune2;
	*vectorNou = (struct Scuter*)malloc(sizeof(struct Scuter) * (*dimensiuneNoua));

	// Copiem elementele din primul vector în vectorul nou
	for (int i = 0; i < dimensiune1; i++) {
		(*vectorNou)[i] = vector1[i];
		(*vectorNou)[i].marca = (char*)malloc(strlen(vector1[i].marca) + 1);
		strcpy((*vectorNou)[i].marca, vector1[i].marca);
	}

	// Copiem elementele din al doilea vector în vectorul nou
	for (int i = 0; i < dimensiune2; i++) {
		(*vectorNou)[dimensiune1 + i] = vector2[i];
		(*vectorNou)[dimensiune1 + i].marca = (char*)malloc(strlen(vector2[i].marca) + 1);
		strcpy((*vectorNou)[dimensiune1 + i].marca, vector2[i].marca);
	}
}



//Functia de dezalocare a vectorului
//dublu pointer pentru vector, copiez pe stiva de meorie adresele 
void dezalocare(struct Scuter** vector, int* nrElemente) {
	//parcurg tot vectorul; fac dereferentiere
	for (int i = 0; i < (*nrElemente); i++) {
		//daca vectorul este diferit de NULL atunci stergem(verificam ca nu este NULL)
		if ((*vector)[i].marca != NULL) {
			//stergem vectorul dereferentiat, si apoi indexam
			free((*vector)[i].marca);
		}
	}
	free(*vector);
	//derefentiem vectorul si il facem NULL ca sa nu mai retina nimic, daca nu am fi facut asta atunci el retinea o valoare
	*vector = NULL;
	*nrElemente = 0;
}





int main() {

	//Creati in functia main un vector alocat dinamic cu cel putin 5 obiecte de tipul structurii voastre
	struct Scuter* scutere;
	int nrScutere = 5;
	scutere = (struct Scuter*)malloc(sizeof(struct Scuter) * nrScutere);
	scutere[0] = initializare(1, 256, "Yamaha", 2000.5);
	scutere[1] = initializare(2, 150, "Honda", 2500.0);
	scutere[2] = initializare(3, 200, "Suzuki", 3000.0);
	scutere[3] = initializare(4, 180, "Piaggio", 2800.0);
	scutere[4] = initializare(5, 220, "Kawasaki", 3500.0);

	afisareVector(scutere, nrScutere);

	struct Scuter* scuterePuterniceSiIeftine = NULL;
	int nrScuterePuterniceSiIeftine = 0;
	copiazaScuterePuterniceSiIeftine(scutere, nrScutere, 200, 3000.0, &scuterePuterniceSiIeftine, &nrScuterePuterniceSiIeftine);
	printf("\n\nScuterele puternice si ieftine sunt:\n");
	afisareVector(scuterePuterniceSiIeftine, nrScuterePuterniceSiIeftine);
	dezalocare(&scuterePuterniceSiIeftine, &nrScuterePuterniceSiIeftine);

	struct Scuter* scutereScumpe = NULL;
	int nrScutereScumpe = 0;
	mutaScutereleScumpe(&scutere, &nrScutere, 3000.0, &scutereScumpe, &nrScutereScumpe);
	printf("\n\nScuterele scumpe sunt:\n");
	afisareVector(scutereScumpe, nrScutereScumpe);
	dezalocare(&scutereScumpe, &nrScutereScumpe);

	struct Scuter* scutere2 = NULL;
	int nrScutere2 = 2;
	scutere2 = (struct Scuter*)malloc(sizeof(struct Scuter) * nrScutere2);
	scutere2[0] = initializare(6, 250, "Ducati", 4000.0);
	scutere2[1] = initializare(7, 300, "BMW", 4500.0);
	afisareVector(scutere2, nrScutere2);

	struct Scuter* scutereConcatenate = NULL;
	int nrScutereConcatenate = 0;
	concateneazaScutere(scutere, nrScutere, scutere2, nrScutere2, &scutereConcatenate, &nrScutereConcatenate);
	printf("\n\nScuterele concatenate sunt:\n");
	afisareVector(scutereConcatenate, nrScutereConcatenate);

	dezalocare(&scutereConcatenate, &nrScutereConcatenate);
	dezalocare(&scutere, &nrScutere);
	dezalocare(&scutere2, &nrScutere2);
	
};