#include <stdio.h>
#include <rpc/rpc.h>
#include "tp3.h"
main (int argc,char *argv[]){
	CLIENT *cl;
	char *server;
	char nom_repertoire[256];
	liste *result;
	liste *result_read;
	liste *clone;
	int choix;
	bool_t again=TRUE;
	arg_writte *args;
	


	if (argc!=2) {
		fprintf(stderr, "usage: %s <host> \n", argv[0]);
		exit(1);
	}
	server = argv[1];
	/* creation de la poignee client */
	cl = clnt_create(server, TP3, TP31, "tcp");
	if (cl==NULL) {
		/* impossible d'etablir la connexion sur le serveur,
		impression du message d'erreur et fin.*/
		clnt_pcreateerror(server);
		exit(1);
	}

	/* Sélectionner une identification */
	cl->cl_auth = authunix_create_default();
	
	/* Modification du timeout */
	struct timeval delai;
	
	delai.tv_sec = 60; /* nombre de secondes */
	delai.tv_usec = 0; /* nombre de microsecondes */
	clnt_control(cl, CLSET_TIMEOUT, (char *)&delai);



	while (again){
		//code du menu//
		printf("\n");
		printf("*MENU TP3************\n");
		printf("*rentrer votre choix*\n");
		printf("*********************\n");
		printf("*1-LS               *\n");
		printf("*2-READ             *\n");
		printf("*3-WRITTE           *\n");
		printf("*4-EXIT             *\n");
		printf("*********************\n");
		printf("\nsaisir votre choix (1/2/3/4):\n");
		scanf("%d", &choix);
		


		/* Appel de la procedure distante sur le
		serveur. */
		switch (choix) {
			case 1:
				printf("saisir le nom du repertoire:\n");
				scanf("%s",&nom_repertoire);
			

				result=ls_1(nom_repertoire, cl);
				if (*result==NULL) {
					/* une erreur a eu lieu lors de l'appel du serveur. Impression
					du message d'erreur et fin. */
					clnt_perror(cl, server);
					exit(1);
				}
				/* traitement du resultat  */
				printf("Liste recuperee \n");
			
				clone=result;
				while(*result !=NULL){
				
					printf("\n %s", (*result)->donnee);			
					*result=(*result)->suivant;
				}
				xdr_free((xdrproc_t)xdr_liste,(char*)clone);

				break; 



			case 2:
				printf("saisir le nom du fichier:\n");
				scanf("%s",&nom_repertoire);

				result_read=read_1(nom_repertoire, cl);
			
				/*clone=result_read;
				while(*result_read !=NULL){
					printf("\n %s", (*result_read)->donnee);			
					*result=(*result_read)->suivant;
				}*/
				xdr_free((xdrproc_t)xdr_liste,(char*)clone);

				break; 

			case 3:
				args=malloc(sizeof(arg_writte));
				printf("saisir le nom du fichier:\n");
				scanf("%s",&nom_repertoire);
				strcpy(args->nom_fichier,nom_repertoire);
				printf("Nom du fichier dans lequel write écrit : %s\n",args->nom_fichier);
					printf("ecraser les donnees? (1/0):\n");
					scanf("%d", &choix);	
					args->ecraser=choix;
				/* récupération du fichier de données par un read */
				args->donnees = malloc(sizeof(cellule));
				args->donnees = read_1("./lol", cl);
				if (args->donnees==NULL)
				{
				 /* une erreur a eu lieu lors de l'appel du serveur.
				  * Impression du message d'erreur et fin.
				  */
				 clnt_perror(cl, server);
				 exit(1);
			    }
				result=writte_1(args, cl);
				break;

			case 4:
				again=FALSE;
				break;

			default:
				printf("mauvaise entree \n");
			}

		choix=0;
		}
	clnt_perror(cl,"127.0.0.1");
	printf("\n");
	exit(0);
}
