

#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//trebuie sa folositi fisierul masini.txt
//sau va creati un alt fisier cu alte date
// 
// 
//Se considera lista simplu inlantuita realizata la seminar.
//
//

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
    Masina info;
    struct Nod* next;
};

typedef struct Nod Nod;

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

void afisareListaMasini(Nod* cap) {
    while (cap != NULL) {
        afisareMasina(cap->info);
        cap = cap->next;
    }
}

void adaugaMasinaInLista(Nod** cap, Masina masinaNoua) {
    Nod* nou = (Nod*)malloc(sizeof(Nod));
    nou->info = masinaNoua;
    nou->next = NULL;
    if (*cap) {
        Nod* p = *cap;
        while (p->next) {
            p = p->next;
        }
        p->next = nou;
    }
    else {
        *cap = nou;
    }
}

void adaugaLaInceputInLista(Nod** cap, Masina masinaNoua) {
    Nod* nou = (Nod*)malloc(sizeof(Nod));
    nou->info = masinaNoua;
    nou->next = *cap;
    *cap = nou;
}

Nod* citireListaMasiniDinFisier(const char* numeFisier) {
    Nod* cap = NULL;
    FILE* f = fopen(numeFisier, "r");
    if (f) {
        while (!feof(f)) {
            //creem lista cu inserare la sfarsit
            adaugaMasinaInLista(&cap, citireMasinaDinFisier(f));
        }
    }
    fclose(f);
    return cap;
}

void dezalocareListaMasini(Nod** cap) {
    while (*cap) {
        Nod* p = *cap;
        (*cap) = p->next;
        if (p->info.model) {
            free(p->info.model);
        }
        if (p->info.numeSofer) {
            free(p->info.numeSofer);
        }
        free(p);
    }
}

float calculeazaPretMediu(Nod* cap) {
    float suma = 0;
    int contor = 0;
    while (cap) {
        suma += cap->info.pret;
        contor++;
        cap = cap->next;
    }
    if (contor > 0) {
        return suma / contor;
    }
    return 0;
}

float calculeazaPretulMasinilorUnuiSofer(Nod* cap, const char* numeSofer) {
    float suma = 0;
    while (cap) {
        if (strcmp(cap->info.numeSofer, numeSofer) == 0) {
            suma += cap->info.pret;
        }
        cap = cap->next;
    }
    return suma;
}
void stergeMasiniDinSeria(Nod** cap, char serieCautata) {
    while ((*cap) && (*cap)->info.serie == serieCautata) {
        Nod* aux = *cap;
        (*cap) = aux->next;
        if (aux->info.numeSofer) {
            free(aux->info.numeSofer);
        }
        if (aux->info.model) {
            free(aux->info.model);
        }
        free(aux);
    }
    if ((*cap)) {
        Nod* p = *cap;
        while (p) {
            while (p->next && p->next->info.serie != serieCautata) {
                p = p->next;
            }
            if (p->next) {
                Nod* aux = p->next;
                p->next = aux->next;
                if (aux->info.numeSofer) {
                    free(aux->info.numeSofer);
                }
                if (aux->info.model) {
                    free(aux->info.model);
                }
                free(aux);
            }
            else {
                p = NULL;
            }
        }
    }

}
//1. Implementati o functie care sterge un nod de pe o pozitie data ca parametru. Daca lista
// are mai putine noduri decat index - ul dat, nu se realizeaza stergerea.

void stergeNodPePozitie(Nod** cap, int pozitie) {
	if (pozitie < 0) {
		return;
	}
	if (pozitie == 0) {
		Nod* aux = *cap;
		*cap = aux->next;
		if (aux->info.model) {
			free(aux->info.model);
		}
		if (aux->info.numeSofer) {
			free(aux->info.numeSofer);
		}
		free(aux);
		return;
	}
	Nod* p = *cap;
	for (int i = 0; i < pozitie - 1 && p != NULL; i++) {
		p = p->next;
	}

	if (p == NULL || p->next == NULL) {
		return;
	}
	Nod* aux = p->next;
	p->next = aux->next;
	if (aux->info.model) {
		free(aux->info.model);
	}
	if (aux->info.numeSofer) {
		free(aux->info.numeSofer);
	}
	free(aux);
}




//2. Implementati o functie care sa insereze elementele in cadrul listei simplu inlantuite astfel
//incat acestea a fie sortate crescator dupa un camp la alegerea voastra.

void adaugaMasinaInListaSortata(Nod** cap, Masina masinaNoua) {
	Nod* nou = (Nod*)malloc(sizeof(Nod));
	nou->info = masinaNoua;
	nou->next = NULL;
	if (*cap == NULL || (*cap)->info.pret >= masinaNoua.pret) {
		nou->next = *cap;
		*cap = nou;
	}
	else {
		Nod* p = *cap;
		while (p->next != NULL && p->next->info.pret < masinaNoua.pret) {
			p = p->next;
		}
		nou->next = p->next;
		p->next = nou;
	}
}



//3. Implementati o functie care salveaza intr - un vector toate obiectele care indeplinesc o
//conditie stabilita de voi.Realizati deep copy, astfel incat elementele din vector sa fie
//diferentiate de cele din lista.Aveti grija la alocarea de spatiu, deoarece trebuie sa stabiliti
//pentru cate elemente veti aloca spatiu.

Masina* salvareMasiniInVector(Nod* cap, int* nrMasini) {
    *nrMasini = 0;
    Nod* p = cap;

    // 1. Parcurgere pentru a numãra câte masini indeplinesc conditia
    while (p) {
        if (p->info.pret > 10000) {  // <-- conditia
            (*nrMasini)++;
        }
        p = p->next;
    }

    // 2. Alocam vector doar pentru masinile care indeplinesc conditia
    Masina* vector = NULL;
    if (*nrMasini > 0) {
        vector = (Masina*)malloc((*nrMasini) * sizeof(Masina));
    }

    // 3. Parcurgere din nou pentru copiere (deep copy)
    p = cap;
    int index = 0;
    while (p) {
        if (p->info.pret > 10000) {  // <-- conditia
			vector[index].id = p->info.id;
			vector[index].nrUsi = p->info.nrUsi;
            vector[index].pret = p->info.pret;
            vector[index].model = malloc(strlen(p->info.model) + 1);
            strcpy(vector[index].model, p->info.model);
            vector[index].numeSofer = malloc(strlen(p->info.numeSofer) + 1);
            strcpy(vector[index].numeSofer, p->info.numeSofer);
			vector[index].serie = p->info.serie;
            index++;
        }
        p = p->next;
    }

    return vector;
}





//4. Implementati o functie care primeste lista si doua pozitii.In cadrul functiei trebuie sa
//interschimbati elementele de pe cele doua pozitii din lista primita.

void interschimbaElementePePozitii(Nod** cap, int pozitie1, int pozitie2) {
	if (pozitie1 == pozitie2) {
		return;
	}
	Nod* p1 = *cap;
	Nod* p2 = *cap;
	for (int i = 0; i < pozitie1 && p1 != NULL; i++) {
		p1 = p1->next;
	}
	for (int i = 0; i < pozitie2 && p2 != NULL; i++) {
		p2 = p2->next;
	}
	if (p1 == NULL || p2 == NULL) {
		return;
	}
	Masina temp = p1->info;
	p1->info = p2->info;
	p2->info = temp;
}


int main() {
    Nod* cap = citireListaMasiniDinFisier("masini.txt");
    afisareListaMasini(cap);
    printf("Pretul mediu este:%.2f\n", calculeazaPretMediu(cap));
    printf("Pretul masinilor unui Sofer este:%.2f\n",
        calculeazaPretulMasinilorUnuiSofer(cap, "Gigel"));

    printf("\nStergere seria A\n");
    stergeMasiniDinSeria(&cap, 'A');
    afisareListaMasini(cap);
    printf("\nStergere seria B\n");
    stergeMasiniDinSeria(&cap, 'B');
    afisareListaMasini(cap);

	printf("\nStergere masina de pe pozitia 2\n");
	stergeNodPePozitie(&cap, 2);
	afisareListaMasini(cap);

	printf("\nAdaugare masina la inceput\n");
	Masina masinaNoua;
	masinaNoua.id = 100;
	masinaNoua.nrUsi = 4;
	masinaNoua.pret = 15000;
	masinaNoua.model = malloc(20);
	strcpy_s(masinaNoua.model, 20, "BMW");
	masinaNoua.numeSofer = malloc(20);
	strcpy_s(masinaNoua.numeSofer, 20, "Ion");
	masinaNoua.serie = 'C';
	adaugaLaInceputInLista(&cap, masinaNoua);
	afisareListaMasini(cap);

	printf("\nAdaugare masina in lista sortata\n");
	Masina masinaNouaSortata;
	masinaNouaSortata.id = 101;
	masinaNouaSortata.nrUsi = 2;
	masinaNouaSortata.pret = 12000;
	masinaNouaSortata.model = malloc(20);
	strcpy_s(masinaNouaSortata.model, 20, "Audi");
	masinaNouaSortata.numeSofer = malloc(20);
	strcpy_s(masinaNouaSortata.numeSofer, 20, "Maria");
	masinaNouaSortata.serie = 'D';
	adaugaMasinaInListaSortata(&cap, masinaNouaSortata);
	afisareListaMasini(cap);

	int nrMasini = 0;
	Masina* vectorMasini = salvareMasiniInVector(cap, &nrMasini);
	printf("\nMasini salvate in vector:\n");
	for (int i = 0; i < nrMasini; i++) {
		afisareMasina(vectorMasini[i]);
	}


    dezalocareListaMasini(&cap);
    return 0;
}