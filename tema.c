/* MUSTAFA Taner - 315CB*/

//Variabilele sunt denumite destul de sugestiv, 
//nu cred ca este nevoie de prea multe explicatii.

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "info.h"

typedef void (*TF)(void*);

void Afisare (Sala sala, FILE* out) { //evenimentul print
	TLista p, q, r;

	if (!sala->nrMese) {
		fprintf(out, "Sala este inchisa!\n");
		return;
	}

	for (p = sala->masa; p != NULL; p = p->urm) {
		fprintf (out, "%s: ", ((Masa)(p->info))->numeMasa);
		q = ((Masa)(p->info))->jucatori;

		for (r = q->urm; r->urm != q; r = r->urm) {

			fprintf (out, "%s - %d; ", 
			((Jucator)(r->info))->nume, 
			((Jucator)(r->info))->nrMaini);
		}

		fprintf (out, "%s - %d.", ((Jucator)(r->info))->nume,
		((Jucator)(r->info))->nrMaini);
		fprintf (out, "\n");
	}
}

void AfisareLista (TLista clas, FILE*out) { // afisare pt clasament
	TLista p = clas;

	while (p != NULL) {
		fprintf (out, "%s %d\n", ((Jucator)(p->info))->nume,
		((Jucator)(p->info))->nrMaini);
		p = p->urm;
	}
}

void AdaugaInSala (TLista *mese, TLista nou) { //adauga mese in sala
	TLista p = *mese;

	if ((*mese) == NULL) {
		(*mese) = nou;	
	} else {
		while (p->urm != NULL) {
			p = p->urm;		
		}
		p->urm = nou;
	}
}

void AdaugaInClasament (TLista *clas, TLista nou, TLista *u, int nrMaini)
	//creeaza lista pentru clasament
{	TLista ref, ant, ant2, p;
	ref = NULL;
	ant2 = NULL;

	if ((*clas) == NULL) {
		(*clas) = nou;
		(*u) = (*clas);	 //u = ultimul element
	} else {
		//gasim elementul inaintea caruia adaugam
		for (p = (*clas), ant = NULL; p != NULL; ant = p, p = p->urm){
			if(nrMaini > ((Jucator)(p->info))->nrMaini) {
				ref = p;
				ant2 = ant;
				break;
			} else {
				if((nrMaini == ((Jucator)(p->info))->nrMaini)
				&& (strcmp (((Jucator)(p->info))->nume,
				((Jucator)(nou->info))->nume) < 0)) {
					ref = p;
					ant2 = ant;
					break;
				}
			}
		}
		//adaugarea propriu-zisa
		if (ref == NULL){
			(*u)->urm = nou;
			(*u) = nou;	
		} else {
			nou->urm = ref;
			if (ant2 == NULL) {
				(*clas) = nou;		
			} else {
				ant2->urm = nou;		
			}	
		}
    	}
}

void AdaugaLaMasa (TLista *jucatori, TLista nou) { //adauga jucatori la o masa
	TLista p = *jucatori;

	if (((*jucatori) == NULL) || ((*jucatori)->urm == (*jucatori))) {
		if ((*jucatori) == NULL) {
			(*jucatori) = (TLista) malloc (sizeof(Celula));
		}
		(*jucatori)->urm = nou;
		nou->urm = (*jucatori);
	} else {
		while (p->urm != (*jucatori)) {
			p = p->urm;		
		}
		nou->urm = (*jucatori);
		p->urm = nou;
	}
}

TLista AlocCelula () { //aloca element de tip lista generica
	TLista nou;

	nou = (TLista) malloc (sizeof(Celula));
	if (!nou) {return NULL;}
	nou->urm = NULL;
	nou->info = NULL;

	return nou;
}

void noroc (char numeMasa[30], char numeJucator[30], //evenimentul noroc
int gradNoroc, Sala*sala, FILE*out) {
	TLista p, q, r;

	for (p = (*sala)->masa; p != NULL; p = p->urm) {
		//cauta masa
		if (!strcmp (((Masa)(p->info))->numeMasa, numeMasa)) {
			q = ((Masa)(p->info))->jucatori;

			for (r = q->urm; r != q; r = r->urm) {
				if (!strcmp (((Jucator)(r->info))->nume,
				numeJucator)) { //cauta jucatorul
					((Jucator)(r->info))->nrMaini =
					((Jucator)(r->info))->nrMaini 
					+ gradNoroc; //creste nr de maini
					return;
				}
			}

			fprintf (out, "Jucatorul %s nu exista la masa %s!\n",
			numeJucator, numeMasa); //se ajunge aici daca nu exista
			return;
		}
	}

	fprintf (out, "Masa %s nu exista!\n", numeMasa);
}

void ghinion (char numeMasa[30], char numeJucator[30], 
int gradNoroc, Sala*sala, FILE*out) { //evenimentul ghinion
	TLista p, q, r, ant, ant2;
	int locuri;

	for (ant2 = NULL, p = (*sala)->masa; p != NULL; ant2 = p, p = p->urm) {
		if ((!strcmp (((Masa)(p->info))->numeMasa, numeMasa))) {

			locuri = ((Masa)(p->info))->nrMaxJucatori;
			q = ((Masa)(p->info))->jucatori;

			for (ant = NULL, r = q->urm; r != q; ant = r, r = r->urm) {
				if (!strcmp (((Jucator)(r->info))->nume, numeJucator)) {
					((Jucator)(r->info))->nrMaini =
					((Jucator)(r->info))->nrMaini - gradNoroc;

					//verificam daca jucatorul trb eliminat
					if (((Jucator)(r->info))->nrMaini <= 0) {
						if (ant == NULL) {
							q->urm = r->urm;
						}
						else if (r->urm != q) {
							ant->urm = r->urm;
						}
						else if (r->urm == q) {
							ant->urm = q;
						}

						((Masa)(p->info))->nrCrtJucatori--;
						(*sala)->nrLocCrt--;

						//eliberam memoria jucatorului
						free (((Jucator)(r->info))->nume);
						free (r->info);
						free (r);

						//verificam dc masa se goleste
						if (((Masa)(p->info))->nrCrtJucatori < 1) {
							(*sala)->nrMese--;
							(*sala)->nrLocMax = 
							(*sala)->nrLocMax - locuri;

							//stergem santinela
							free (q);
							if (ant2 == NULL) {(*sala)->masa = p->urm;}
							else {ant2->urm = p->urm;}
							free (((Masa)(p->info))->numeMasa);
							free (p->info);
							free (p);

							return;
						}
					}

					return;
				}
			}

			fprintf (out, "Jucatorul %s nu exista la masa %s!\n",
			numeJucator, numeMasa); //aici se ajunge daca nu exista

			return;
		}
	}

	fprintf (out, "Masa %s nu exista!\n", numeMasa);
}

void ghinion2 (char numeMasa[30], Sala*sala, FILE*out) {//ghinion pt toata masa
	TLista p, q, r, ant, aux, ant2;
	int locuri;

	//se ia fiecare jucator de la numeMasa in parte
	for (ant2 = NULL, p = (*sala)->masa; p != NULL; ant2 = p, p = p->urm) {
		if ((!strcmp (((Masa)(p->info))->numeMasa, numeMasa))) {

			locuri = ((Masa)(p->info))->nrMaxJucatori;
			q = ((Masa)(p->info))->jucatori;
			r = q->urm;
			ant = NULL;

			while (r != q) {
				aux = r->urm;
				((Jucator)(r->info))->nrMaini =
				((Jucator)(r->info))->nrMaini - 1;

				if (((Jucator)(r->info))->nrMaini <= 0) {
					if (ant == NULL) {
						q->urm = r->urm;
					}
					else if (r->urm != q) {
						ant->urm = r->urm;
					}
					else if (r->urm == q) {
						ant->urm = q;
					}

					((Masa)(p->info))->nrCrtJucatori--;
					(*sala)->nrLocCrt--;

					free (((Jucator)(r->info))->nume);
					free (r->info);
					free (r);

					if (((Masa)(p->info))->nrCrtJucatori < 1) {
						(*sala)->nrMese--;
						(*sala)->nrLocMax = 
						(*sala)->nrLocMax - locuri;

						free (q);

						if (ant2 == NULL) {(*sala)->masa = p->urm;}
						else {ant2->urm = p->urm;}
						free (((Masa)(p->info))->numeMasa);
						free (p->info);
						free (p);

						return;
					}
				} else {
					ant = r;
				}
				r = aux;
			}

			return;
		}
	}
}

void roteste (Sala*sala, char numeMasa[30]) {
	TLista p, q, r, aux;

	//se roteste masa numeMasa
	for (p = (*sala)->masa; p != NULL; p = p->urm) {
		if ((!strcmp (((Masa)(p->info))->numeMasa, numeMasa))
		&& (((Masa)(p->info))->nrCrtJucatori > 1)) {

			q = ((Masa)(p->info))->jucatori;
			r = q->urm;

			//gasesc ultimul element dinainte de santinela
			while  (r->urm != q) {
				r = r->urm;
			}

			aux = q->urm;
			q->urm = q->urm->urm;
			aux->urm = q;
			r->urm = aux;

			return;
		}
		
	}
}

void tura (Sala*sala, char numeMasa[30], FILE*out) { //evenimentul tura
	TLista p, q, r;
	int prim;

	for (p = (*sala)->masa; p != NULL; p = p->urm) {
		prim = 0;
		if (!strcmp (((Masa)(p->info))->numeMasa, numeMasa)) {

			q = ((Masa)(p->info))->jucatori;
			r = q->urm;

			//verificam daca primul jucator de langa santinela
			//trebuie sters; daca da, nu se mai roteste masa
			if (((Jucator)(r->info))->nrMaini == 1)	{
				prim = 1;
			}

			ghinion2 (numeMasa, sala, out);

			if (!prim) {
				roteste (sala,  numeMasa);
			}

			return;
		}
	}

	fprintf (out, "Masa %s nu exista!\n", numeMasa);
	
}

void tura_completa (Sala*sala, FILE*out) { //evenimentul tura_completa
	TLista p, q;

	p = (*sala)->masa;
	while (p != NULL) {
		q = p;
		p = p->urm;

		//se aplica tura pt fiecare masa in parte		
		tura (sala, ((Masa)(q->info))->numeMasa, out);
	}
}

void eliberareClasament (TLista *clas) { //se elibereaza lista de clasament
	TLista p, aux;

	p = (*clas);
	while (p != NULL) {
		aux = p;
		p = p->urm;

		free (((Jucator)(aux->info))->nume);
		free (aux->info);
		free (aux);
	}
}

void clasament (Sala*sala, char numeMasa[30], FILE*out) { //evenimentul clasament
	TLista p, q, r, nou = NULL, clas = NULL, u = NULL;
	Jucator jucator;

	for (p = (*sala)->masa; p != NULL; p = p->urm) {
		if (!strcmp (((Masa)(p->info))->numeMasa, numeMasa)) {

			q = ((Masa)(p->info))->jucatori;
			r = q->urm;

			while (r != q) {
				//se aloca memorie pt lista generica cu info
				//de tip jucator
				nou = NULL;
				jucator = NULL;

				nou = AlocCelula();

				jucator = (Jucator) malloc (sizeof(struct jucator));
				if (!jucator) {
					printf("Eroare de alocare!\n");
					eliberareClasament (&clas);
					return;
				}

				jucator->nume = NULL;
				jucator->nume = (char*) malloc (15 * sizeof(char));
				if (!jucator->nume) {
					printf("Eroare de alocare!\n");
					eliberareClasament (&clas);
					return;
				}

				//se copiaza jucatorul in cel nou alocat
				strcpy (jucator->nume, ((Jucator)(r->info))->nume);
				jucator->nrMaini = ((Jucator)(r->info))->nrMaini;
				nou->info = jucator;

				r = r->urm;

				AdaugaInClasament (&clas, nou, &u, jucator->nrMaini);
			}

			fprintf (out, "Clasament %s:\n", numeMasa);
			AfisareLista (clas, out);	

			eliberareClasament (&clas);

			return;
		}
	}

	fprintf (out, "Masa %s nu exista!\n", numeMasa);
}

void inchide (Sala*sala, char numeMasa[30], FILE*out) { //evenimentul inchidere
	TLista p, q, r, aux, a, ant;
	int libere, locuri;
	
	for (ant = NULL, p = (*sala)->masa; p != NULL; ant = p, p = p->urm) {
		if (!strcmp (((Masa)(p->info))->numeMasa, numeMasa)) {

			locuri = ((Masa)(p->info))->nrMaxJucatori -
				 ((Masa)(p->info))->nrCrtJucatori;
			libere = (*sala)->nrLocMax -
				 (*sala)->nrLocCrt - locuri;
			locuri = ((Masa)(p->info))->nrMaxJucatori;

			//verificam daca sunt suficiente locuri
			if (libere < ((Masa)(p->info))->nrCrtJucatori) {
				fprintf (out, "Nu exista suficiente locuri in sala!\n");
				return;
			}

			q = ((Masa)(p->info))->jucatori;
			r = q->urm;

			while (r != q) {
				aux = r;
				r = r->urm;
				aux->urm = NULL;

				//cautam mese cu locuri libere la care adaugam
				for (a = (*sala)->masa; a != NULL; a = a->urm) {
					if (strcmp (((Masa)(a->info))->numeMasa,
					numeMasa) != 0) {
						if ( ((Masa)(a->info))->nrCrtJucatori <
						((Masa)(a->info))->nrMaxJucatori ) {
							AdaugaLaMasa (&((Masa)(a->info))->jucatori, aux);
							((Masa)(a->info))->nrCrtJucatori++;
							break;
						}
					}
				}
			}

			((Masa)(p->info))->nrCrtJucatori = 0;

			free (q);

			if (ant == NULL) {(*sala)->masa = p->urm;}
			else {ant->urm = p->urm;}
			free (((Masa)(p->info))->numeMasa);
			free (p->info);
			free (p);

			(*sala)->nrLocMax = (*sala)->nrLocMax - locuri;
			(*sala)->nrMese--;

			return;
		}
	}

	fprintf (out, "Masa %s nu exista!\n", numeMasa);
	
}

void evenimente (FILE*even, FILE*out, Sala sala) { //citirea evenimentelor
	char comanda[100], numeMasa[30], numeJucator[30];
	int gradNoroc;

	while (fscanf (even, "%s", comanda) != EOF) {
		if (!strcmp(comanda, "print")) {
			Afisare (sala, out);
		}
		else if (!strcmp(comanda, "noroc")) {
			fscanf (even, "%s %s %d", numeMasa, numeJucator, &gradNoroc);
			noroc (numeMasa, numeJucator, gradNoroc, &sala, out);
		}
		else if (!strcmp(comanda, "ghinion")) {
			fscanf (even, "%s %s %d", numeMasa, numeJucator, &gradNoroc);
			ghinion (numeMasa, numeJucator, gradNoroc, &sala, out);
		}
		else if (!strcmp(comanda, "tura")) {
			fscanf (even, "%s", numeMasa);
			tura (&sala, numeMasa, out);
		}
		else if (!strcmp(comanda, "tura_completa")) {
			tura_completa (&sala, out);
		}
		else if (!strcmp(comanda, "clasament")) {
			fscanf (even, "%s", numeMasa);
			clasament (&sala, numeMasa, out);
		}
		else if (!strcmp(comanda, "inchide")) {
			fscanf (even, "%s", numeMasa);
			inchide (&sala, numeMasa, out);
		}
	}
}

void elibMem (Sala*sala) {
	//eliberarea memoriei
	TLista p, q, r, aux, aux2;

	p = (*sala)->masa;
	while (p != NULL) {
		aux = p;
		p = p->urm;

		//se verifica daca masa e goala
		q = ((Masa)(aux->info))->jucatori;
		r = q->urm;
		while (r != q) {
			aux2 = r;
			r = r->urm;
			free (((Jucator)(aux2->info))->nume);
			free (aux2->info);
			free (aux2);
		}
		free (q);
		free (((Masa)(aux->info))->numeMasa);
		free (aux->info);
		free (aux);
	}
	free (*sala);
}

int main (int argc, char**argv)
{
	TLista nou;
	Masa masa;
	Sala sala;
	Jucator jucator;

	char config[30], evenim[30], fis[30];
	int i, j;

	//deschidem fisierele de input si output
	strcpy (config, argv[1]);
		FILE *conf = fopen (config, "r");
	if (!conf) {
		printf("Nu s-a putut deschide fisierul 1!\n");
		return 0;
	}

	strcpy (evenim, argv[2]);
	FILE *even = fopen (evenim, "r");
	if (!even) {
		printf("Nu s-a putut deschide fisierul 2!\n");
		return 0;
	}

	strcpy (fis, argv[3]);
	FILE *out = fopen (fis, "w");
	if (!out) {
		printf("Nu s-a putut deschide fisierul 3!\n");
		return 0;
	}
	
	//alocam memoria pentru sala si citim
	sala = (Sala) malloc (sizeof(struct sala));
	if (!sala) {
		printf ("Nu s-a putut aloca memoria pentru sala!\n");
		return 0;	
	}

	sala->masa = NULL;
	fscanf(conf, "%d", &sala->nrMese);
	sala->nrLocCrt = 0;
	sala->nrLocMax = 0;

	//citim mesele si jucatorii si alocam memorie
	for (i = 0; i < sala->nrMese; i++) {

		masa = (Masa) malloc (sizeof(struct masa));
		if (!masa) {
			printf ("Nu s-a putut aloca masa %d.\n", i);
			elibMem (&sala);
			return 0;
		}

		masa->numeMasa = NULL;
		masa->numeMasa = (char*) malloc (9 * sizeof(char));
		if (!masa->numeMasa) {
			printf("Nu s-a putut aloca masa %d.\n", i);
			elibMem (&sala);
		}

		masa->jucatori = NULL;

		fscanf (conf, "%s %d %d", masa->numeMasa, &masa->nrCrtJucatori, &masa->nrMaxJucatori);

		sala->nrLocCrt = sala->nrLocCrt + masa->nrCrtJucatori;
		sala->nrLocMax = sala->nrLocMax + masa->nrMaxJucatori;
		
		for (j = 0; j < masa->nrCrtJucatori; j++) {

			jucator = (Jucator) malloc (sizeof(struct jucator));
			if (!jucator) {
				printf ("Nu s-a putut aloca jucatorul %d.\n", j);
				elibMem (&sala);
				return 0;
			}

			jucator->nume = NULL;
			jucator->nume = (char*) malloc (15 * sizeof(char));
			if (!jucator->nume) {
				printf ("Nu s-a putut aloca jucatorul %d.\n", j);
				elibMem (&sala);
				return 0;
			}

			fscanf (conf, "%s %d", jucator->nume, &jucator->nrMaini);

			nou = AlocCelula ();
			nou->info = jucator;

			AdaugaLaMasa (&masa->jucatori, nou);
		}

		nou = AlocCelula ();
		nou->info = masa;

		AdaugaInSala (&sala->masa, nou);
	}

	evenimente(even, out, sala); //citirea & rularea evenimentelor

	elibMem (&sala); //eliberarea memoriei

	fclose (even);
	fclose (out);
	fclose (conf);
	return 0;
}
