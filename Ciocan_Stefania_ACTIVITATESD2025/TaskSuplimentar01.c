#include<stdio.h>
#include <stdlib.h>

//Numele si prenumele: Ciocan Stefania -> C si S

//1.Sa se construiască un articol care să conțină cele două litere

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

//Realizati o functie cu ajutorul careia sa cititi de la tastatura 
// un articol de tipul construit de voi.Funcția returnează obiectul citit.
struct Scuter citireScuterDeLaTastatura() {
	struct Scuter s;

	printf("Introduceti codul scuterului: ");
	scanf_s("%d", &s.cod);

	printf("Introduceti puterea in KW: ");
	scanf_s("%d", &s.putereKW);

	char buffer[20];
	printf("Introduceti marca scuterului: ");
	scanf_s("%s", buffer, 20);

	s.marca = (char*)malloc(sizeof(char) * (strlen(buffer) + 1));
	strcpy_s(s.marca, strlen(buffer) + 1, buffer);

	printf("Introduceti pretul: ");
	scanf_s("%f", &s.pret);

	return s;
}

//Realizati o functie care calculeaza ceva pentru un obiect de tipul implementat de voi.
// (o medie sau o suma, un maxim, un minim...sau orice altceva)

float calculPretMediu(struct Scuter* scutere, int nrScutere) {
	if (nrScutere == 0) return 0;
	float suma = 0;
	for (int i = 0; i < nrScutere; i++) {
		suma += scutere[i].pret;
	}
	return suma / nrScutere;
}

//Realizati o functie care modifica pentru un obiect primit ca parametru, un anumit camp. 
// noua valoare este primita ca parametru.

void modificaPret(struct Scuter* s, float pretNou) {
	if (pretNou > 0) {
		s->pret = pretNou;
	}
}
	
	//Dezalocare de memorie
void dezalocare(struct Scuter* s) {
	if (s->marca != NULL) {
		free(s->marca);
		s->marca = NULL;
	}
}
//Realizați o funcție care afișează un obiect de tipul creat
// . Afișarea se face la console, și sunt afișate toate informațiile.

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

int main() {
	struct Scuter s;
	s = initializare(1, 256, "Yamaha", 2000.5);
	afisare(s);

	s = citireScuterDeLaTastatura();
	afisare(s);

	struct Scuter scutere[3] = {
		initializare(1, 150, "Honda", 2500.0),
		initializare(2, 200, "Suzuki", 3000.0),
		initializare(3, 180, "Piaggio", 2800.0)
	};

	// Calcul și afișare preț mediu
	printf("Pretul mediu este: %.2f RON\n", calculPretMediu(scutere, 3));

	modificaPret(&s, 3000.5);
	afisare(s);

	dezalocare(&s);
	afisare(s);


	return 0;
}