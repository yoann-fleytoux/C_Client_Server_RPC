#include <stdio.h>
#include <rpc/rpc.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <dirent.h>
#include "tp3.h"
#include <stdlib.h>
#include <fcntl.h>

#include <unistd.h>




/* version distante des procédures          *
 
 * Rmq1 : toute procedure distante accepte  *
 * en entree un pointeur sur les arguments  *
 * qu'elle aurait eus en local              *
 
 * Rmq2 : toute procedure distante retourne *
 * un pointeur sur les resultats qu'elle    *
 * aurait retournes en local                *
 
 * Rmq3 : rpcgen convertit en minuscule le  *
 * nom de la procedure dans la definition   *
 * de programme, rajoute le caractere de    *
 * soulignement (_) et le numero de version */
 


/*****************************************
 * Procédure distante ls_1_svc           *
 ****************************************/
#define SIZE 1024

extern liste *ls_1_svc(char *nom_repertoire, struct svc_req *rqstp){




	
	// On teste d'abbord l'authentification
	if ( rqstp->rq_cred.oa_flavor == AUTH_NULL )
	{
		printf("Aucune identification n'est utilisee.\n");
	}
	else if ( rqstp->rq_cred.oa_flavor == AUTH_UNIX )
	{	
		printf("Type d'identification : UNIX.\n");

		// On déclare une structure
		struct authunix_parms* aup;
		
		// On récupère les paramètres d'identification
		aup = (struct authunix_parms*)rqstp->rq_clntcred;
		if ( (aup->aup_uid == getuid()) /* test de l'uid */ && (aup->aup_gid == getgid())/* test du groupe */ )
		{
			printf("Authentification reussie.\n");
		}
		else
		{
			printf("Echec de l'authentification.\n");
		}
	}
	else
	{
		printf("Erreur dans la reconnaissance de l'authentification.\n");
	}





	struct dirent * current;
	DIR* ptdir;
	int nb_result=0;

	static liste n_f;
	n_f = malloc(sizeof(cellule));
	liste last=NULL;
	
	// on ouvre le répertoire
	ptdir=opendir(nom_repertoire);
	if (!ptdir) 
	{ 	
		fprintf(stderr,"error opendir\n");
	}
	// On lit le premier nom de dossier
	current = readdir(ptdir);
	if ( current != NULL )
	{
		n_f->donnee = malloc(sizeof(char)*1024);
		strcpy(n_f->donnee, current->d_name);
	}
	
	last = n_f;
	current = readdir(ptdir);
	// on parcourt les fichiers
	// pour chaque fichier ou dossier
	while(current != NULL)
	{	
		last->suivant = malloc(sizeof(cellule));
		last = last->suivant;

		last->donnee =  malloc(sizeof(char)*1024);
		strcpy(last->donnee, current->d_name);
		nb_result+=1;
		printf("%s\n", last->donnee);
		printf("\n");
		current = readdir(ptdir);
	}
	
	closedir(ptdir);
	printf("%d",nb_result);
	printf("\n");

	// On vide le buffer
	fflush(stdout);
	fflush(stdin);

	return &n_f;
	

}



/*****************************************
 * Procédure distante read_1_svc         *
 ****************************************/
extern liste * read_1_svc(char *nom_repertoire, struct svc_req *rqstp){
	



	// On teste d'abbord l'authentification
	if ( rqstp->rq_cred.oa_flavor == AUTH_NULL )
	{
		printf("Aucune identification n'est utilisee.\n");
	}
	else if ( rqstp->rq_cred.oa_flavor == AUTH_UNIX )
	{	
		printf("Type d'identification : UNIX.\n");

		// On déclare une structure
		struct authunix_parms* aup;
		
		// On récupère les paramètres d'identification
		aup = (struct authunix_parms*)rqstp->rq_clntcred;
		if ( (aup->aup_uid == getuid()) /* test de l'uid */ && (aup->aup_gid == getgid())/* test du groupe */ )
		{
			printf("Authentification reussie.\n");
		}
		else
		{
			printf("Echec de l'authentification.\n");
		}
	}
	else
	{
		printf("Erreur dans la reconnaissance de l'authentification.\n");
	}

	/*char *str = malloc(sizeof(char)*256);
	strcpy(str,"lol.txt");*/

	FILE* fichierParcouru= NULL;
	// On ouvre le fichier
	
	
	fichierParcouru = fopen(/*str/"lol.txt"*/nom_repertoire, "r");

	//free(str);
	// Gestion de l'erreur
	if(fichierParcouru==NULL)
    	{
       		perror("Erreur dans l'ouverture du fichier\n");
		exit(1);
   	}else{
		printf("ouverture reussie");
	}

	// Allouer la premiere cellule
	static liste  n_f;
	n_f= malloc(sizeof(cellule));
	liste last=n_f;

	last->donnee = malloc(sizeof(char)*1024);
	last->suivant = NULL;
	
	int count;
	// Parcours du fichier
	// pour chaque bloc de 1024 octets
	char buffer[256];
	bzero(buffer,256);
	printf("\nVoici la liste des blocs lus par le read : \n\n\n");
	do 
	{
		// On vide le buffer
		fflush(stdout);
		fflush(stdin);
	
		//récupérer un bloc de 1024 octets et le stocker dans la liste
		count=fread(buffer,1,1024,fichierParcouru);
		printf("nb de blocs  %d\n",count);
		if (count<=0){
			fprintf(stderr,"erreur lecture\n");
			exit(9);
		}
		printf("%s\n",last->donnee);
		bcopy(buffer,last->donnee,1024);
		// allouer la cellule
		last->suivant = malloc(sizeof(cellule));
		
		// passer au suivant
		last = last->suivant;
		last->suivant = NULL;
		last->donnee = malloc(sizeof(char)*256);
	}while(!feof(fichierParcouru));

	fclose(fichierParcouru);
	return &n_f;
}



/*****************************************
 * Procédure distante write_1_svc        *
 ****************************************/
extern void * writte_1_svc(arg_writte *arg, struct svc_req *rqstp){
	// On teste d'abbord l'authentification
	if ( rqstp->rq_cred.oa_flavor == AUTH_NULL )
	{
		printf("Aucune identification n'est utilisee.\n");
	}
	else if ( rqstp->rq_cred.oa_flavor == AUTH_UNIX )
	{	
		printf("Type d'identification : UNIX.\n");

		// On déclare une structure
		struct authunix_parms* aup;
		
		// On récupère les paramètres d'identification
		aup = (struct authunix_parms*)rqstp->rq_clntcred;
		if ( (aup->aup_uid == getuid()) /* test de l'uid */ && (aup->aup_gid == getgid())/* test du groupe */ )
		{
			printf("Authentification reussie.\n");
		}
		else
		{
			printf("Echec de l'authentification.\n");
		}
	}
	else
	{
		printf("Erreur dans la reconnaissance de l'authentification.\n");
	}
	
	

	FILE* fichier = NULL;
	char mode[4];
	static char retour = 'a' ;
	
	
	if ( arg->ecraser == 1 )
	{
		strcpy(mode,"w"); // mode d'écriture et purge
	}
	else
	{
		strcpy(mode,"a"); // mode création et écriture
	}
	
	// On ouvre le fichier dans ce mode
	fichier = fopen(arg->nom_fichier,mode);
	
	// Gestion de l'erreur
	if ( fichier == NULL )
	{
		perror("Erreur dans l'ouverture du fichier.\n");
		
		// Sortie du programme
		retour = 'e';
		return (&retour);
	}
	else
	{
		// On écrit dans le fichier
		liste courant = (*arg->donnees);
		
		// Parcours des données
		while ( courant != NULL )
		{
			// On écrit les données dans le fichier
			fprintf(fichier,"%s",courant->donnee);
			
			// On passe au suivant
			courant = courant->suivant;
		}
	}
	
	// On ferme le fichier
	fclose(fichier);
	
	// Sortie du programme
	return (&retour);
}

