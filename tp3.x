/*service*/
typedef struct cellule* liste;

const MAXNOM = 255;

typedef string type_nom<MAXNOM>;

struct cellule{
	type_nom donnee;
	liste suivant;};

struct arg_writte{
	type_nom nom_fichier;
	int ecraser;
	liste * donnees;};

program TP3{
	version TP31{
		liste LS(char nom_repertoire)=1;
		liste READ(char nom_repertoire)=2;
		void WRITTE(arg_writte arg)=3;
	}=1;
}=0x20000088;
