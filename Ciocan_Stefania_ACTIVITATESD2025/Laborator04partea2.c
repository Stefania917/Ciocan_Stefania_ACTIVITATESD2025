//#define _CRT_SECURE_NO_WARNINGS
//#include <stdio.h>
//#include <stdlib.h>
//#include <string.h>
//
////trebuie sa avem fisierul masini.txt !!!!!
////ce inseamna lista simplu inlantuita? -> lista presupune legarea mai multor noduri intre ele
////lista nu o sa ocupe o zona contigua de memorie
////fiecare nod contine un camp cu informatie utila si pe langa acest camp mai avem unul cu adresa urmatorului nod
////ultimul nod o sa retina NULL
////ca sa am acces la toate nodurile trebuie sa am adresa primului nod (capul listei)
//
//
//struct StructuraMasina {
//    int id;
//    int nrUsi;
//    float pret;
//    char* model;
//    char* numeSofer;
//    unsigned char serie;
//};
//typedef struct StructuraMasina Masina;
//
////structura unui nod pentru lista simplu inlantuita
//struct Nod {
//    //informatia utila de tip MASINA
//    Masina info;
//    //a doua sectiune a nodului este un pointer
//    struct Nod* next;
//};
//typedef struct Nod Nod;
//
//Masina citireMasinaDinFisier(FILE* file) {
//    char buffer[100];
//    char sep[3] = ",\n";
//    fgets(buffer, 100, file);
//    char* aux;
//    Masina m1;
//    aux = strtok(buffer, sep);
//    m1.id = atoi(aux);
//    m1.nrUsi = atoi(strtok(NULL, sep));
//    m1.pret = atof(strtok(NULL, sep));
//    aux = strtok(NULL, sep);
//    m1.model = malloc(strlen(aux) + 1);
//    strcpy_s(m1.model, strlen(aux) + 1, aux);
//
//    aux = strtok(NULL, sep);
//    m1.numeSofer = malloc(strlen(aux) + 1);
//    strcpy_s(m1.numeSofer, strlen(aux) + 1, aux);
//
//    m1.serie = *strtok(NULL, sep);
//    return m1;
//}
//
//void afisareMasina(Masina masina) {
//    printf("Id: %d\n", masina.id);
//    printf("Nr. usi : %d\n", masina.nrUsi);
//    printf("Pret: %.2f\n", masina.pret);
//    printf("Model: %s\n", masina.model);
//    printf("Nume sofer: %s\n", masina.numeSofer);
//    printf("Serie: %c\n\n", masina.serie);
//}
//
////afisarea listei de masini
////daca avem adresa capului listei, putem accesa toate nodurile
//void afisareListaMasini(Nod* cap) {
//    while (cap != NULL) {
//        //folosesc -> pentru ca trebuie sa facem si dereferentierea
//        afisareMasina(cap->info);
//        cap = cap->next;
//    }
//}
//
////adaugarea unei masini in lista
////vrem sa adaugam o noua masina in lista, dar o adaugam la final
////pe langa masina trebuie sa adaugam si un pointer la urmatorul nod
////eu trebuie sa modific valoarea lui cap deci folosim dublu pointer
//void adaugaMasinaInLista(Nod** cap, Masina masinaNoua) {
//    //creem un nod nou ii alocam spatiu
//    Nod* nou = (Nod*)malloc(sizeof(Nod));
//    //ii setam informatiile
//    nou->info = masinaNoua; //shallow copy (daca apleman functia si nu o stergem sau nu o mai folosim atunci e ok shallow copy daca nu trebuie deep copy)
//    nou->next = NULL;
//    //verificam valoarea lui cap (trebuie dereferentiat!!!!!)
//    if (*cap) {
//        //luam un nou pointer p care sa retina adresa lui cap
//        Nod* p = *cap;
//        //ma deplasez cu p pana cand am unde sa ma deplasez
//        while (p->next) {
//            p = p->next;
//        }
//        //p o sa primeasca valoarea noua
//        p->next = nou;
//    }
//    else {
//        //daca cap e NULL, inseamna ca lista e goala
//        *cap = nou;
//    }
//}
//
////adaugarea unei masini la inceputul listei
////trebuie sa adaugam un nou nod la inceputul listei 
//void adaugaLaInceputInLista(Nod** cap, Masina masinaNoua) {
//    //creem un nod nou
//    Nod* nou = (Nod*)malloc(sizeof(Nod));
//    //ii setam informatiile
//    nou->info = masinaNoua;
//    nou->next = *cap;
//    //modificam capul listei
//    *cap = nou;
//}
//
////citirea listei de masini din fisier
////functia primeste numele fisierului
////returnam un nod(capul listei) deci returnam un pointer la nod Nod*
//Nod* citireListaMasiniDinFisier(const char* numeFisier) {
//    //declaram o lista si initializam cu NULL pt ca nu avem informatii
//    Nod* cap = NULL;
//    FILE* f = fopen(numeFisier, "r");
//    //daca exista f
//    if (f) {
//        //cat timp nu am ajuns la finalul fisierului
//        while (!feof(f)) {
//            //creem lista cu inserare la sfarsit astfel incat sa imi inserese exact in ordine
//            adaugaMasinaInLista(&cap, citireMasinaDinFisier(f)); //apel imbricat
//        }
//    }
//    //inchidem fisierul
//    fclose(f);
//    //returnam lista
//    return cap;
//}
//
////dezalocarea listei de masini
////cap este un pointer la pointer
//void dezalocareListaMasini(Nod** cap) {
//    //cat timp exista cap !!!!!!+trebuie sa fie dereferntiat!!!!!!!
//    while (*cap) {
//        //luam un  p pe care il initializam cu adresa pe care o contine cap
//        Nod* p = *cap;
//        //mutam pointerul cap la urmatoarea adresa !!!!!cap trebuie dereferentiat!!!!!!! -> dereferentiaza si ea pt ca este dublu pointer
//        (*cap) = p->next;
//        //stergem informatiile 
//        if (p->info.model) {
//            free(p->info.model);
//        }
//        if (p->info.numeSofer) {
//            free(p->info.numeSofer);
//        }
//        //stergem nodul intreg
//        free(p);
//    }
//}
//
////calcularea pretului mediu al masinilor din lista
////nu modificam lista deci nu avem nevoie de dublu pointer, pot sa ma plimb direct cu cap in lista
//float calculeazaPretMediu(Nod* cap) {
//	//calculam suma preturilor
//    float suma = 0;
//	//contorizez cate elemente am
//    int contor = 0;
//	//cat timp cap != NULL
//    while (cap) {
//        suma += cap->info.pret;
//        contor++;
//		//ca sa nu intre in bucla infinita trebuie sa plece cu cap->next
//        cap = cap->next;
//    }
//	//in cazul in care contor>0 , returnez suma/contor
//    if (contor > 0) {
//        return suma / contor;
//    }
//    //daca nu am masini  media este 0
//    return 0;
//}
//
////calculam pretul masinilor unui sofer adia calculam o suma si o returnam
//float calculeazaPretulMasinilorUnuiSofer(Nod* cap, const char* numeSofer) {
//    float suma = 0;
//    while (cap) {
//        //punem conditie ca soferul sa aiba un anumit nume
//		//le comparam iar daca sunt egale atunci adunam pretul
//        if (strcmp(cap->info.numeSofer, numeSofer) == 0) {
//            suma += cap->info.pret;
//        }
//        cap = cap->next;
//    }
//    return suma;
//}
//
////functia care sterge masina din lista care au seria data
//void stergeMasiniDinSeria(Nod** cap, char serieCautata) {
//	//cat timp exista cap si cap de info serie (!!!!!!CAP DEREFERENTIAT!!!!!!!) este exact acea serie
//    while ((*cap) && (*cap)->info.serie == serieCautata) {
//        //luam un pointer auxiliar
//        Nod* aux = *cap;
//        //mut cap la urmatoarea pozitie
//        (*cap) = aux->next;
//		//eliberam memoria
//        if (aux->info.numeSofer) {
//            free(aux->info.numeSofer);
//        }
//        if (aux->info.model) {
//            free(aux->info.model);
//        }
//        free(aux);
//    }
//	//daca cap != NULL
//    if ((*cap)) {
//        Nod* p = *cap;
//        while (p) {
//            //ma deplasez cat timp e diferit de serieCautat
//            while (p->next && p->next->info.serie != serieCautata) {
//                //fac deplasarea
//                p = p->next;
//            }
//            //putem sa iesim in 3 situatii
//			//verificam daca exista p->next
//            if (p->next) {
//				//luam un pointer auxiliar care trebuie sa ocoleasca nodul nostru
//                Nod* aux = p->next;
//                p->next = aux->next;
//				//eliberam memoria
//                if (aux->info.numeSofer) {
//                    free(aux->info.numeSofer);
//                }
//                if (aux->info.model) {
//                    free(aux->info.model);
//                }
//                free(aux);
//            }
//            else {
//               
//                p = NULL;
//            }
//        }
//    }
//
//}
//
//
//int main() {
//    Nod* cap = citireListaMasiniDinFisier("masini.txt");
//    afisareListaMasini(cap);
//
//
//    printf("Pretul mediu este:%.2f\n", calculeazaPretMediu(cap));
//
//    printf("Pretul masinilor unui Sofer este:%.2f\n",
//        calculeazaPretulMasinilorUnuiSofer(cap, "Gigel"));
//
//    printf("\nStergere seria A\n");
//    stergeMasiniDinSeria(&cap, 'A');
//    afisareListaMasini(cap);
//
//    printf("\nStergere seria B\n");
//    stergeMasiniDinSeria(&cap, 'B');
//    afisareListaMasini(cap);
//
//    dezalocareListaMasini(&cap);
//    return 0;
//}