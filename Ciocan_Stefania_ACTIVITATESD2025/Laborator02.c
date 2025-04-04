#include<stdio.h>
#include<malloc.h>

struct Telefon {
	int id;
	int RAM;
	char* producator;
	float pret;
	char serie;
};

struct Telefon initializare(int id, int ram,const char* producator, float pret, char serie) {
	struct Telefon t;
	t.id = id;
	t.RAM = ram;
	t.producator = (char*)malloc(sizeof(char) * (strlen(producator) + 1));
	strcpy_s(t.producator, strlen(producator) + 1, producator);
	t.pret = pret;
	t.serie = serie;
	return t;
}

void afisare(struct Telefon t) {
	if (t.producator != NULL) {
		printf("%d. Telefonul %s seria %c are %d GB RAM si costa %5.2f RON.\n",
			t.id, t.producator, t.serie, t.RAM, t.pret);
	}
	else {
		printf("%d. Telefonul din seria %c are %d GB RAM si costa %5.2f RON.\n",
			t.id, t.serie, t.RAM, t.pret);
	}
}
//Functia de afisare vector primeste un vector de telefoane si numarul de elemente
void afisareVector(struct Telefon* vector, int nrElemente) {
	//parcurgem for-ul si pt fiecare element din vector apelam functia de afisare
	for (int i = 0; i < nrElemente; i++) {
		afisare(vector[i]);
	}
}

//Functia de copiere a primelor n elemente dintr-un vector
// Functia primeste vectorul, numarul de elemente si numarul de elemente pe care vreau sa le copiez
struct Telefon* copiazaPrimeleNElemente(struct Telefon* vector, int nrElemente, int nrElementeCopiate){
	//initializez cu un vector NULL
	struct Telefon*vectorNou=NULL;
	//am alocat spatiu pentru vectorul nou
	vectorNou = (struct Telefon*)malloc(sizeof(struct Telefon) * nrElementeCopiate);
	//ca sa umplem pozitiiile din vector, parcurg vectorul
	for (int i = 0; i < nrElementeCopiate; i++) {
		//SHALLOW COPY
		vectorNou[i] = vector[i];
		//producator este un pointer care se afla in fiecare telefon, 
		// deci pt acest pointer trebuie sa alocam spatiu separat
		//DEEP COPY peste
		vectorNou[i].producator = (char*)malloc(strlen(vector[i].producator) + 1);
		strcpy_s(vectorNou[i].producator, strlen(vector[i].producator) + 1, vector[i].producator);
	}
	return vectorNou;
}

//Functia de dezalocare a vectorului

//dublu pointer pentru vector, copiez pe stiva de meorie adresele 
void dezalocare(struct Telefon** vector, int* nrElemente) {
	//parcurg tot vectorul; fac dereferentiere
	for (int i = 0; i < (*nrElemente); i++) {
		//daca vectorul este diferit de NULL atunci stergem(verificam ca nu este NULL)
		if ((*vector)[i].producator != NULL) {
			//stergem vectorul dereferentiat, si apoi indexam
			free((*vector)[i].producator);
		}
	}
	free(*vector);
	//derefentiem vectorul si il facem NULL ca sa nu mai retina nimic, daca nu am fi facut asta atunci el retinea o valoare
	*vector = NULL;
	*nrElemente = 0;
}

//FUNCTIE DE PROCESARE
void copiazaTelefoaneScumpe(struct Telefon* vector, char nrElemente, float pretMinim, struct Telefon** vectorNou, int* dimensiune) {
	//Prima data determinam dimensiunea, noi nu stim care este
	*dimensiune = 0;
	//parcurgem toate telefoanele
	for (int i = 0; i < nrElemente; i++) {
		//verificam daca pretul este mai mare decat minimul
		if (vector[i].pret >= pretMinim) {
			//atunci am mai gasit un telefon si crestem dimensiunea
			(*dimensiune)++;
			//vom stii cate telefoane indeplinesc conditia
		}
	}
	//prima data verificam daca vectorul este NULL, daca nu este NULL atunci il dezalocam
	//este o masura extra ca sa stim ca am sters memoria daca ar fi avut ceva inainte
	if ((*vectorNou) != NULL) {
		free(*vectorNou);
	}
	//alocam spatiu pentru vectorul nou
	*vectorNou = (struct Telefon*)malloc(sizeof(struct Telefon) * (*dimensiune));
	//acum trebuie sa aducem telefoanele care indeplinesc conditia in vectorul nou
	//k=0 este pozitia de unde voi copia
	int k = 0;
	//vectorul nou trebuie dereferntiat pt ca este un pointer transmis prin pointer (adresa la adresa)
	for (int i = 0; i < nrElemente; i++) {
		if (vector[i].pret >= pretMinim) {
			//aici am facut shallow copy
			(*vectorNou)[k] = vector[i];
			//acum vom face un deep copy
			(*vectorNou)[k].producator = (char*)malloc(strlen(vector[i].producator) + 1);
			strcpy_s((*vectorNou)[k].producator, strlen(vector[i].producator) + 1, vector[i].producator);
			//k trebuie sa creasca ca urmatorul telefon sa il aduca pe pozitia k+1
			k++;
		}
	}
}

struct Telefon getPrimulTelefonByProducator(struct Telefon* vector, int nrElemente, const char* producator) {
	struct Telefon t;
	t.producator = NULL;
	for (int i = 0; i < nrElemente; i++) {
		//verificam fiecare telefon, daca vectorul de i.producator este egal cu producatorul cautat
		//functia strcmp comnpara doua siruri de caractere
		if (strcmp(vector[i].producator, producator) == 0) {
			//se pune problema ce facem, shallow copy sau deep copy
			//vom face deep copy
			t = vector[i];
			t.producator = (char*)malloc(strlen(vector[i].producator) + 1);
			strcpy_s(t.producator, strlen(vector[i].producator) + 1, vector[i].producator);
			return t;
		}
	}
	return t;
}
	


int main() {

	struct Telefon* telefoane = NULL;
	int nrTelefoane = 3;
	telefoane = (struct Telefon*)malloc(sizeof(struct Telefon) * nrTelefoane);
	telefoane[0] = initializare(1, 256, "Samsung", 2000, 'S');
	telefoane[1] = initializare(2, 512, "Motorola", 1500, 'M');
	telefoane[2] = initializare(3, 256, "Apple", 2200, 'A');

	afisareVector(telefoane, nrTelefoane);

	struct Telefon* primeleTelefoane = NULL;
	int nrPrimeleTelefoane = 2;

	primeleTelefoane = copiazaPrimeleNElemente(telefoane, nrTelefoane, nrPrimeleTelefoane);
	printf("\n\nPrimele telefoane:\n");
	afisareVector(primeleTelefoane, nrPrimeleTelefoane);
	dezalocare(&primeleTelefoane, &nrPrimeleTelefoane);
	afisareVector(primeleTelefoane, nrPrimeleTelefoane);

	//copiaza telefoane scumpe
	struct Telefon* telefoaneScumpe = NULL;
	int nrTelefoaneScumpe = 0;
	copiazaTelefoaneScumpe(telefoane, nrTelefoane, 2000, &telefoaneScumpe, &nrTelefoaneScumpe);
	printf("\n\nTelefoane Scumpe:\n");
	afisareVector(telefoaneScumpe, nrTelefoaneScumpe);
	dezalocare(&telefoaneScumpe, &nrTelefoaneScumpe);

	struct Telefon telefon = getPrimulTelefonByProducator(telefoane, nrTelefoane, "Apple");
	printf("\n\nTelefonul gasit:\n");
	afisare(telefon);
	//verificam daca avem ce sa stergem si eliberam memoria
	if (telefon.producator != NULL) {
		free(telefon.producator);
		telefon.producator = NULL;
	}
	dezalocare(&telefoane, &nrTelefoane);
	return 0;
}