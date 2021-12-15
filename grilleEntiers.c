#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <MLV/MLV_all.h>

const int N = 20;

void echange(int* a, int* b){
	int temp = *a;
	*a = *b;
	*b = temp;
}

int test_start(int argc, char* argv[]){
	if(argc == 0 || argc == 1 || argc == 2 || argc > 3){
		return 0;
	}
	else if(argv[1][0] != '-' || (argv[1][1] != 'c' && argv[1][1] != 'l') || (argv[1][2] != 'a' && argv[1][2] != 'g')){
		return 0;
	}
	else{
		return 1;
	}
}

int lecture(FILE*fichier, char separateur){
	char intermed[5], lect;
	int i = 0;
	while((lect = fgetc(fichier)) != separateur){
		intermed[i] = lect;
		i += 1;
	}
	intermed[i] = '\0';
	return atoi(intermed);
}

void creation_tab_val_c(int taille_plat, int val_max, int tab_val[taille_plat][taille_plat]){
	srand(time(NULL));
	int i, j;
	for(i=0; i<taille_plat; i+=1){
		for(j=0; j<taille_plat; j+=1){
			tab_val[i][j] = rand() % val_max;
			if(rand()%2 == 0){
				tab_val[i][j] = -tab_val[i][j];
			}
		}
	}
}

void creation_tab_val_l(FILE* fichier, int taille_plat, int tab_val[taille_plat][taille_plat]){
	int i, j;
	for(i=0; i<taille_plat; i+=1){
		for(j=0; j<taille_plat-1; j+=1){
			tab_val[i][j] = lecture(fichier, ' ');
		};
		tab_val[i][j] = lecture(fichier, '\n');
	}
}

void sauvegarde(char* argv[3], int taille_plat, int tab_val[taille_plat][taille_plat], int* somme, int* coups, int val_max){
	int i, j;
	FILE* fichier = fopen(argv[2], "w");
	fprintf(fichier, "%d\n%d %d %d\n", taille_plat, *somme, *coups, val_max);
	for(i=0; i<taille_plat; i+=1){
		for(j=0; j<taille_plat-1; j+=1){
			fprintf(fichier, "%d ", tab_val[i][j]);
		}
		fprintf(fichier, "%d\n", tab_val[i][j]);
	}
	fclose(fichier);
}

void affichage_grille_ascii(int taille_plat, int val_max, int somme, int coups, int tab_val[taille_plat][taille_plat]){
	int i, j;
	printf("Somme : %d  Coups : %d", somme, coups);
	printf("\n        ");
	for(i=0; i<taille_plat; i+=1){
		printf("%d      ", i);
	}
	printf("\n       ");
	for(i=0; i<taille_plat; i+=1){
		if(i<10){
			printf("*******");
		}
		else{
			printf("********");
		}
	}
	for(i=0; i<taille_plat; i+=1){
		printf("\n");
		if(i<10){
			printf("  %d  *  ", i);
		}
		else{
			printf(" %d  *  ", i);
		}
		for(j=0; j<taille_plat; j+=1){
			printf("%d    ", tab_val[i][j]);
			if(j>=10){
				printf(" ");
			}
			if(tab_val[i][j]>=0){
				printf(" ");
			}
			if(abs(tab_val[i][j])<10){
				printf(" ");
			}
		}
	}
}

void jeu(int taille_plat, int tab_val[taille_plat][taille_plat], int coord[4], int* somme, int* coups, int val_max, char* argv[3]){
	int i, j, x1 = coord[0], y1 = coord[1], x2 = coord[2], y2 = coord[3];
	if((x1 == x2) && (y1 == y2)){
		if(argv[1][2] == 'a'){
			printf("Il faut selectionner deux cases différentes\n");
		}
		else if(argv[1][2] == 'g'){
			int width = MLV_get_window_width();
			int height = MLV_get_window_height();
			MLV_draw_text(((width*0.9)/2)-140, (height/2)-15, "Il faut selectionner deux cases différentes", MLV_COLOR_RED);
			MLV_actualise_window();
			MLV_wait_seconds(3);
		}
		*coups -= 1;
	}
	else if(x1 == x2){
		for(i = (y1 < y2) ? y1 : y2; i <= ((y1 > y2) ? y1 : y2); i+=1){
			*somme = *somme + tab_val[x1][i];
			tab_val[x1][i] = rand() % val_max;
			if(rand()%2 == 0){
				tab_val[x1][i] = -tab_val[x1][i];
			}
			for(j = ((x1 < x2) ? x1 : x2); j > 0; j-=1){
				echange(&tab_val[j][i], &tab_val[j-1][i]);
			}
		}
	}
	else if(y1 == y2){
		for(i = (x1 < x2) ? x1 : x2; i <= ((x1 > x2) ? x1 : x2); i+=1){
			*somme = *somme + tab_val[i][y1];
			tab_val[i][y1] = rand() % val_max;
			if(rand()%2 == 0){
				tab_val[i][y1] = -tab_val[i][y1];
			}
		}
		for(i=0; i < ((x1 < x2) ? x1 : x2); i+=1){
			echange(&tab_val[i][y1], &tab_val[abs(x1-x2)+i+1][y1]);
		}
	}
	else if(abs(x1-x2) == abs(y1-y2)){
		for(i = 0; i <= abs(x1-x2) ; i+=1){
			*somme = *somme + tab_val[x1 + ((x1 < x2) ? i : -i)][y1 + ((y1 < y2) ? i : -i)];
			tab_val[x1 + ((x1 < x2) ? i : -i)][y1 + ((y1 < y2) ? i : -i)] = rand() % val_max;
			if(rand()%2 == 0){
				tab_val[x1 + ((x1 < x2) ? i : -i)][y1 + ((y1 < y2) ? i : -i)] = -tab_val[x1 + ((x1 < x2) ? i : -i)][y1 + ((y1 < y2) ? i : -i)];
			}
			for(j = abs(x1 + ((x1 < x2) ? i : -i)); j > 0; j-=1){
				echange(&tab_val[j][y1 + ((y1 < y2) ? i : -i)], &tab_val[j-1][y1 + ((y1 < y2) ? i : -i)]);
			}
		}
	}
	else{
		if(argv[1][2] == 'a'){
			printf("La diagonale est impossible\n");
		}
		else if(argv[1][2] == 'g'){
			int width = MLV_get_window_width();
			int height = MLV_get_window_height();
			MLV_draw_text(((width*0.9)/2)-100, (height/2)-15, "La diagonale est impossible", MLV_COLOR_RED);
			MLV_actualise_window();
			MLV_wait_seconds(3);
		}
		*coups -= 1;
	}
}

void demande_ascii(int coord[4], int taille_plat, int tab_val[taille_plat][taille_plat], int* somme, int* coups, char* argv[3], int val_max){
	char choix;
	printf("\nQue voulez-vous faire ? (c/s/q) : ");
	scanf("\n%c", &choix);
	switch(choix){
		case 'c' :
		do{
		printf("Coordonnées point 1 : ");
		scanf("%d %d", &coord[0], &coord[1]);
		}while(coord[0] > taille_plat || coord[1] > taille_plat);
		do{
		printf("Coordonnées point 2 : ");
		scanf("%d %d", &coord[2], &coord[3]);
		}while(coord[2] > taille_plat || coord[3] > taille_plat);
		jeu(taille_plat, tab_val, coord, somme, coups, val_max, argv);
		*coups += 1;
		break;
		case 's': sauvegarde(argv, taille_plat, tab_val, somme, coups, val_max); break;
		case 'q': exit(0); break;
	}
}

 /*Partie contenant MLV*/

void bouton_score(int taille_plat,int somme){
	int width = MLV_get_window_width();
	int height = MLV_get_window_height();
	MLV_draw_text_box (width*0.905,height/15, 90, 80,"SCORE : %d ", 1, MLV_COLOR_WHITE, MLV_COLOR_GREEN, MLV_COLOR_BLACK, MLV_TEXT_LEFT,MLV_HORIZONTAL_CENTER,MLV_VERTICAL_CENTER,somme);
	MLV_draw_text_box (width*0.905,height/1.55, 90, 80,"Sauvegarder", 1, MLV_COLOR_WHITE, MLV_COLOR_BLACK, MLV_COLOR_GREY, MLV_TEXT_CENTER,MLV_HORIZONTAL_CENTER,MLV_VERTICAL_CENTER);
	MLV_draw_text_box (width*0.905,height/1.25, 90, 80,"Quitter", 1, MLV_COLOR_WHITE, MLV_COLOR_BLACK, MLV_COLOR_GREY, MLV_TEXT_CENTER,MLV_HORIZONTAL_CENTER,MLV_VERTICAL_CENTER);
}

void dessine_grille(int taille_plat, int tab_val[taille_plat][taille_plat], int somme){
	int width = MLV_get_window_width();
	int height = MLV_get_window_height();
	int i, j;
	char c[5];
	MLV_clear_window(MLV_COLOR_WHITE);
	bouton_score(taille_plat, somme);
	MLV_draw_rectangle(0, 0, width*0.9, height, MLV_COLOR_BLACK );
	for(i = 0; i< taille_plat; i++){
		for(j = 0; j< taille_plat; j++){
			int xcase = (width*0.9)/taille_plat;
			int ycase = (height)/taille_plat;
			sprintf(c, "%d",tab_val[i][j]);
			MLV_draw_text_box (xcase*j,ycase*i, xcase, ycase,c, 1, MLV_COLOR_BLACK, MLV_COLOR_BLACK, MLV_COLOR_GREY, MLV_TEXT_CENTER,MLV_HORIZONTAL_CENTER,MLV_VERTICAL_CENTER);
		}
	}
}

void clique_coord(int taille_plat, int tab_val[taille_plat][taille_plat], int* somme, int coord[4], int val_max, int* coups, char* argv[3]){
	int x, y, i;
	int width = MLV_get_window_width();
	int height = MLV_get_window_height();
	MLV_wait_mouse(&x, &y);
	if (x < width*0.9){ 
		for(i=0; i<2; i+=1){
			coord[1+2*i] = (x/((width*0.9)/taille_plat));
			coord[2*i] = (y/((height)/taille_plat));
			if(i==0){
				MLV_wait_mouse(&x, &y);
			}
		}
		jeu(taille_plat, tab_val, coord, somme, coups, val_max, argv);
		*coups += 1;
	}
	else if (x >= width*0.905 && x <= width*0.905 + 90 && y >= height/1.55 && y <= height/1.55 + 80){
		sauvegarde(argv, taille_plat, tab_val, somme, coups, val_max);
	}
	else if (x >= width*0.905 && x <= width*0.905 + 90 && y >= height/1.25 && y <= height/1.25 + 80){
		MLV_free_window();
		exit(0);
	}
	else{
		clique_coord(taille_plat, tab_val, somme, coord, val_max, coups, argv);
	}
}

int main(int argc, char* argv[]){
	if(test_start(argc, argv) == 0){
		printf("Paramètres %s non reconnus", argv[1]);
		return 0;
	}
	if(argv[1][1] == 'c'){
		int taille_plat, val_max, somme = 0, coups = 0;
		do{
			printf("Entrez la taille du plateau (inf à 20) : ");
			scanf("%d", &taille_plat);
		}
		while(taille_plat > N);
		int tab_val[taille_plat][taille_plat];
		printf("Entrez la valeur max : ");
		scanf("%d", &val_max);
		creation_tab_val_c(taille_plat, val_max, tab_val);
		int coord[4];
		if(argv[1][2] == 'a'){
			while(somme != 100){
				affichage_grille_ascii(taille_plat, val_max, somme, coups, tab_val);
				demande_ascii(coord, taille_plat, tab_val, &somme, &coups, argv, val_max);
			}
		}
		else if(argv[1][2] == 'g'){
			int a = 800, b = 1000;
			MLV_create_window("Mini projet", "projet", b, a);
			while(somme != 100){
				dessine_grille(taille_plat, tab_val, somme);
				MLV_actualise_window();
				clique_coord(taille_plat, tab_val, &somme, coord, val_max, &coups, argv);
			}
			MLV_free_window();
		}
	}
	else if(argv[1][1] == 'l'){
		FILE* fichier = fopen(argv[2], "r");
		int taille_plat = lecture(fichier, '\n');
		int somme = lecture(fichier, ' ');
		int coups = lecture(fichier, ' ');
		int val_max = lecture(fichier, '\n');
		int tab_val[taille_plat][taille_plat];
		creation_tab_val_l(fichier, taille_plat, tab_val);
		int coord[4];		
		if(argv[1][2] == 'a'){
			while(somme != 100){
				affichage_grille_ascii(taille_plat, val_max, somme, coups, tab_val);
				demande_ascii(coord, taille_plat, tab_val, &somme, &coups, argv, val_max);
			}
		}
		else if(argv[1][2] == 'g'){
			int a = 800, b = 1000;
			MLV_create_window("Mini projet", "projet", b, a);
			while(somme != 100){
				dessine_grille(taille_plat, tab_val, somme);
				MLV_actualise_window();
				clique_coord(taille_plat, tab_val, &somme, coord, val_max, &coups, argv);
			}
			MLV_free_window();
		}
	}
	return 0;
}
