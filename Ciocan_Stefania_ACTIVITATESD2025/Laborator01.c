//#include<stdio.h>
//#include<stdlib.h>
//
////Aceasta este structura pe care vom lucra
//struct Telefon {
//	int id;
//	int RAM;
//	char* producator;
//	float pret;
//	char serie;
//};
//
////Initializarea seamana cu constructorul cu toti parametri din POO 
//// Mereu folosesc struct ca sa stie compilatorul ca e vorba de o structura
//struct Telefon initializare(int id, int ram, const char* producator, float pret, char serie) {
//	struct Telefon t;
//	//INITIALIZEZ TOATE ATRIBUTELE CU VALORI
//	t.id = id;
//	t.RAM = ram;
//	//FUNCTIA MALLOC PENTRU ALOCARE DINAMICA
//	// FUNCTIA MALLOC RETURNEAZA UN VOID POINTER
//	//SIZEOF RETURNEAZA NUMARUL DE OCTETI NECESARI PENTRU ALOCARE
//	//STRLEN RETURNEAZA NUMARUL DE CARACTERE 
//	t.producator = (char*)malloc(sizeof(char) * (strlen(producator) + 1)); // +1 PENTRU ULTIMUL CARACTER
//	strcpy_s(t.producator, strlen(producator) + 1, producator); //_S PENTRU CA TREBUIE SA FIE SECURE
//	//IN STRCPY PRIMUL PARAMETRU ESTE DESTINATIA (unde vreau sa copiez)
//	// AL DOILEA ESTE cate caractere voi copia, 
//	// iar al treilea este sursa
//	t.pret = pret;
//	t.serie = serie;
//	return t;
//}
//
//void afisare(struct Telefon t) {
//	//aici verific daca producatorul este NULL, doar atunci afisam producatorul
//	if (t.producator != NULL) {
//		printf("%d. Telefonul %s seria %c are %d GB RAM si costa %5.2f RON.\n",
//			t.id, t.producator, t.serie, t.RAM, t.pret);
//	}
//	else {
//		printf("%d. Telefonul din seria %c are %d GB RAM si costa %5.2f RON.\n",
//			t.id, t.serie, t.RAM, t.pret);
//	}
//}
//
////seamana cu un setter doar ca il facem sub forma de functie
////folosesc un pointer (in C nu avem referinte)  
////se face direct in stiva de memorie modificarea 
//void modificaPret(struct Telefon* t, float noulPret) {
//	if (noulPret > 0) {
//		//t->pret este echivalent cu (*t).pret
//		//ii dau o adresa de memorie
//		t->pret = noulPret;
//	}
//}
//
////ca la destructor
//void dezalocare(struct Telefon* t) {
//	if (t->producator != NULL) {
//		free(t->producator);
//		//dupa ce facem stergerea trebuie sa initializam 
//		t->producator = NULL;
//	}
//}
//
//int main() {
//	struct Telefon t;
//	t = initializare(1, 256, "Samsung", 2000.5, 'A');
//	//"" folosesc pt sir de caractere , '' pt un singur caracter
//	afisare(t);
//	//obtin adresa la t si o trimit ca parametru
//	//transmiterea s-a facut dirtect in memorie
//	modificaPret(&t, 1000);
//	afisare(t);
//
//	dezalocare(&t);
//	afisare(t);
//
//
//	return 0;
//}