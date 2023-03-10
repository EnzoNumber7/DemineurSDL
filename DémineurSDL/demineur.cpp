#include <SDL.h>
#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <math.h>

#define windowWidth 900
#define windowHeight 900

void Error(const char* message);
void Destroy(SDL_Window* window, SDL_Renderer* renderer);
int verificationMine(int* mine, int lineChoice, int columnChoice, char* tab,int sizeTab,int sizeMine);
void mineProximity(char* tab, int* mine, int lineChoice, int columnChoice, int* verified, int sizeMine, int sizeTab);
int victory(int* verified, int sizeTab, int sizeMine);
int verificationTab(int* verified, int placement, int sizeTab);
void mineCreation( int lineChoice, int columnChoice, int numberLine, int sizeMine, int sizeTab, int* mine);
void putFlag(char* tab, int* mine, int lineChoice, int columnChoice, int* verifiedTab, int sizeTab);
void queryTextureAndRenderCopy(SDL_Texture* texture, SDL_Rect rect, SDL_Window* window, SDL_Renderer* renderer, int placementX, int placementY, int screen, int numberLine);

int main(int argc, char* argv[]) {
    SDL_Window* window = NULL;
    SDL_Renderer* renderer = NULL;

    // -- INITIALISATION DES VARIABLES C -- //
    int i = 0;
	int j = 0;
	int lose = 0;
	int clickWinLose = 0;

	int easyTab = 81;
	int medTab = 256;
	int hardTab = 529;
	int easyMine = 10; 
	int medMine = 40;
	int hardMine = 99;
    int lineChoice, columnChoice;
    int placement = 0;
	int win = 0;
    int round = 1;
	int sizeTab,sizeMine;

	int titleScreen = 1;
	int gameScreen = 0;
	int screenWin = 0;
	int screenLose = 0;

	char choice = 'c';
    sizeTab = 0;
	sizeMine = 0;

	char* tab = 0;
	int* mine = 0;
	int* verified = 0;
	int numberLine = 0;


    // -- INITIALISATION DES VARIABLES POUR L'AFFICHAGE -- //

    int run = 1;

    int placementX = 0;
    int placementY = 0;

	int nbFrame = 0;

    if (SDL_Init(SDL_INIT_VIDEO) != 0)
        Error("Initialisation SDL");


    // -- CREATION FENETRE ET RENDU -- //
    if (SDL_CreateWindowAndRenderer(windowWidth, windowHeight, 0, &window, &renderer) != 0) {
        Error("Creation fenetre et rendu echouee");
    }
    SDL_SetWindowTitle(window, "Demineur");

    // -- RECTANGLE UTILISE POUR LES IMAGE -- //
    SDL_Rect rectCase = { 0,0,100,100 };
    SDL_Rect rectFlag = { 0,0,50,100 };
	SDL_Rect rectWinLose = { 0,0,windowWidth,windowHeight };

    // -- CREATION DES SURFACES POUR LES IMAGES --//
    SDL_Surface* one = SDL_LoadBMP("img/one.bmp");
    SDL_Surface* two = SDL_LoadBMP("img/two.bmp");
    SDL_Surface* three = SDL_LoadBMP("img/three.bmp");
    SDL_Surface* four = SDL_LoadBMP("img/four.bmp");
    SDL_Surface* five = SDL_LoadBMP("img/five.bmp");
    SDL_Surface* six = SDL_LoadBMP("img/six.bmp");
    SDL_Surface* seven = SDL_LoadBMP("img/seven.bmp");
    SDL_Surface* eight = SDL_LoadBMP("img/eight.bmp");
    SDL_Surface* tnt = SDL_LoadBMP("img/tnt.bmp");
    SDL_Surface* flag = SDL_LoadBMP("img/flag.bmp");
    SDL_Surface* dirt = SDL_LoadBMP("img/dirt.bmp");
	SDL_Surface* loseScreen = SDL_LoadBMP("img/loseScreen.bmp");
    SDL_Surface* winScreen = SDL_LoadBMP("img/winScreen.bmp");

    // -- CREATION DES TEXTURES POUR LES IMAGES --//
    SDL_Texture* textureOne = SDL_CreateTextureFromSurface(renderer, one);
    SDL_Texture* textureTwo = SDL_CreateTextureFromSurface(renderer, two);
    SDL_Texture* textureThree = SDL_CreateTextureFromSurface(renderer, three);
    SDL_Texture* textureFour = SDL_CreateTextureFromSurface(renderer, four);
    SDL_Texture* textureFive = SDL_CreateTextureFromSurface(renderer, five);
    SDL_Texture* textureSix = SDL_CreateTextureFromSurface(renderer, six);
    SDL_Texture* textureSeven = SDL_CreateTextureFromSurface(renderer, seven);
    SDL_Texture* textureEight = SDL_CreateTextureFromSurface(renderer, eight);
    SDL_Texture* textureTnt = SDL_CreateTextureFromSurface(renderer, tnt);
    SDL_Texture* textureFlag = SDL_CreateTextureFromSurface(renderer, flag);
    SDL_Texture* textureDirt = SDL_CreateTextureFromSurface(renderer, dirt);
	SDL_Texture* textureLose = SDL_CreateTextureFromSurface(renderer, loseScreen);
	SDL_Texture* textureWin = SDL_CreateTextureFromSurface(renderer, winScreen);
    SDL_FreeSurface(one);
    SDL_FreeSurface(two);
    SDL_FreeSurface(three);
    SDL_FreeSurface(four);
    SDL_FreeSurface(five);
    SDL_FreeSurface(six);
    SDL_FreeSurface(seven);
    SDL_FreeSurface(eight);
    SDL_FreeSurface(tnt);
    SDL_FreeSurface(flag);
    SDL_FreeSurface(dirt);
	SDL_FreeSurface(loseScreen);
	SDL_FreeSurface(winScreen);

    // -- CREATION DE LA GRILLE -- //

	// METTRE LA GRID DU TITLE SCREEN PAR DEFAUT 
    SDL_Surface* title = SDL_LoadBMP("img/titleScreen.bmp");
    SDL_Texture* textureTitle = SDL_CreateTextureFromSurface(renderer, title);
    SDL_FreeSurface(title);
	SDL_Rect rectGrid = {0,0,windowWidth,windowHeight};
	queryTextureAndRenderCopy(textureTitle, rectGrid, window, renderer, 0, 0, 1, numberLine);
    SDL_RenderPresent(renderer);
	
	// -- BOUCLE PRINCIPALE DE JEU -- //
    while (run) {


        // -- EVENT -- //
        SDL_Event event;

        while (SDL_PollEvent(&event) != 0 ){
            switch (event.type) {
				case SDL_QUIT:
					run = 0;
					break;

				// -- CLICK SOURIS -- //
				case SDL_MOUSEBUTTONDOWN:
				
						// -- CLICK GAUCHE -- //
						if (event.button.button == SDL_BUTTON_LEFT) {

							// -- TITLE SCREEN EVENT -- //
							if (titleScreen == 1) {
								if (event.motion.x < 647 && event.motion.x >  195  && event.motion.y < 576  && event.motion.y > 442 ) {
									// -- GRILLE FACILE -- // 
									sizeTab = easyTab;
									sizeMine = easyMine;
								}
								else if (event.motion.x < 647 && event.motion.x > 195 && event.motion.y < 710 && event.motion.y > 583) {
									// -- GRILLE MOYENNE -- //
									sizeTab = medTab;
									sizeMine = medMine;
								}
								else if (event.motion.x < 648  && event.motion.x > 199 && event.motion.y < 849 && event.motion.y > 719) {
									// -- GRILLE DIFFICILE -- //
									sizeTab = hardTab;
									sizeMine = hardMine;
								}
							}

							// -- GAME EVENT -- //
							else if (gameScreen == 1) {
								placementX = (event.motion.x / (windowWidth / numberLine)) * (windowWidth / numberLine);
								placementY = (event.motion.y / (windowHeight / numberLine)) * (windowHeight / numberLine);
								lineChoice = (placementY / (windowHeight / numberLine));
								columnChoice = (placementX / (windowWidth / numberLine));

								if (victory(verified, sizeTab, sizeMine)) {
									win = 1;
								}

								if (victory(verified, sizeTab, sizeMine) == 0 && lose == 0) {
									if (round == 1) {
										mineCreation(lineChoice, columnChoice, numberLine, sizeMine, sizeTab, mine);
									}
									// On verifie si le joueur a cliqué sur une mine 
									if (verificationMine(mine, lineChoice, columnChoice, tab, sizeTab, sizeMine)) {
										lose = 1;
									}
									// Sinon, on verifie s'il y a des mines autour
									else {
										mineProximity(tab, mine, lineChoice, columnChoice, verified, sizeMine, sizeTab);
									}
									round += 1;
								}
								else {
									clickWinLose = 1;
								}
							}
							// -- WIN SCREEN EVENT -- //
							else if (screenWin == 1){

								// -- RESTART --//
								if (event.motion.x < 778 && event.motion.x > 539 && event.motion.y < 94  && event.motion.y > 16 ) {
									i=0;
									for (i = 0; i < sizeTab; i++) {
										tab[i] = '_';
									}
									for (i = 0; i < sizeTab; i++) {
										verified[i] = -1;
									}
									round = 1;
									titleScreen = 1;
									screenWin = 0;
									clickWinLose = 0;
									win = 0;
								}
								// -- TITLE SCREEN -- //
								if (event.motion.x < 776 && event.motion.x > 540 && event.motion.y < 178 && event.motion.y > 102) {
									sizeTab = 0;
									titleScreen = 1;
									screenWin = 0;
									clickWinLose = 0;
									round = 1;
									win = 0;
									queryTextureAndRenderCopy(textureTitle, rectGrid, window, renderer, 0, 0, 1, numberLine);
								}
							}

							// -- LOSE SCREEN EVENT -- //
							else if (screenLose == 1) {
								// -- RESTART -- //
								if (event.motion.x < 707 && event.motion.x > 205 && event.motion.y < 451 && event.motion.y > 403) {
									i=0;
									for (i = 0; i < sizeTab; i++) {
										tab[i] = '_';
									}
									for (i = 0; i < sizeTab; i++) {
										verified[i] = -1;
									}

									round = 1;
									screenLose = 0;
									lose = 0;
									clickWinLose = 0;
									titleScreen = 1;
								}
								// -- TITLE SCREEN -- //
								if (event.motion.x < 708 && event.motion.x > 205 && event.motion.y < 512 && event.motion.y > 465) {
									sizeTab = 0;
									screenLose = 0;
									round = 1;
									lose = 0;
									titleScreen = 1;
									clickWinLose = 0;
									queryTextureAndRenderCopy(textureTitle, rectGrid, window, renderer, 0, 0, 1, numberLine);
								}
							}

						}

						// -- CLICK DROIT -- //
						if (event.button.button == SDL_BUTTON_RIGHT){
							if (gameScreen == 1) {
								placementX = (event.motion.x / (windowWidth / numberLine)) * (windowWidth / numberLine);
								placementY = (event.motion.y / (windowHeight / numberLine)) * (windowHeight / numberLine);
								lineChoice = (placementY / (windowHeight / numberLine));
								columnChoice = (placementX / (windowWidth / numberLine));
								putFlag(tab, mine, lineChoice, columnChoice, verified, sizeTab);
							}
						}
			}	
        }

		// -- RENDER -- //

		// -- CREATION ET AFFICHAGE DES ECRAN PRINCIPAUX (MENU, JEU, ECRAN DE DEFAITE ET VICTOIRE) -- //

		if (titleScreen == 1 && sizeTab != 0) {
			// LA GRILLE AVEC LA DIFFICULTE CHOISI //
			// -- GRILLE FACILE -- //
			if (sizeTab == easyTab) {
				SDL_Surface* gridEasy = SDL_LoadBMP("img/gridEasy.bmp");
				SDL_Texture* textureGridEasy = SDL_CreateTextureFromSurface(renderer, gridEasy);
				SDL_FreeSurface(gridEasy);
				queryTextureAndRenderCopy(textureGridEasy, rectGrid, window, renderer, 0, 0, 1, numberLine);
			}

			// -- GRILLE MOYENNE -- //
			if (sizeTab == medTab) {
				SDL_Surface* gridMed = SDL_LoadBMP("img/gridMed.bmp");
				SDL_Texture* textureGridMed = SDL_CreateTextureFromSurface(renderer, gridMed);
				SDL_FreeSurface(gridMed);
				queryTextureAndRenderCopy(textureGridMed, rectGrid, window, renderer, 0, 0, 1, numberLine);
			}

			// -- GRILLE DIFFICILE  -- //
			if (sizeTab == hardTab) {
				SDL_Surface* gridHard = SDL_LoadBMP("img/gridHard.bmp");
				SDL_Texture* textureGridHard = SDL_CreateTextureFromSurface(renderer, gridHard);
				SDL_FreeSurface(gridHard);
				queryTextureAndRenderCopy(textureGridHard, rectGrid, window, renderer, 0, 0, 1 ,numberLine);
			}
			// -- VRAIABLE NESSECITANT sizeTab -- //
			tab = (char*)malloc(sizeof(char) * sizeTab);
			mine = (int*)malloc(sizeof(int) * sizeMine);
			verified = (int*)malloc(sizeof(int) * sizeTab);
			numberLine = (int)sqrt(sizeTab);

			// -- REMPLISSAGE DES TABLEAUX TEXTE -- // 
			for (i = 0; i < sizeTab; i++) {
				tab[i] = '_';
			}
			// Remplissage du tableau de vérification avec des -1
			for (i = 0; i < sizeTab; i++) {
				verified[i] = -1;
			}
			

			titleScreen = 0;
			gameScreen = 1;

		}
		else if (win == 1  && clickWinLose == 1) {
			queryTextureAndRenderCopy(textureWin, rectWinLose, window, renderer, 0, 0, 1, numberLine);
			screenWin = 1;
			gameScreen = 0;
		}
		else if (lose == 1 && clickWinLose == 1) {
				queryTextureAndRenderCopy(textureLose, rectWinLose, window, renderer, 0, 0, 1, numberLine);
				screenLose = 1;
				gameScreen = 0;
		}
		else if (gameScreen == 1) {
			// On affiche sur la fenêtre SDL les information du tableau texte
			i = 0;
			for (i = 0; i < sizeTab; i++) {
				placementX = ((i) % numberLine) * (windowWidth / numberLine);
				placementY = ((i) / numberLine) * (windowHeight / numberLine);
				

				if (tab[i] != '_' && tab[i] != 'M' && tab[i] != '$') { queryTextureAndRenderCopy(textureDirt, rectCase, window, renderer, placementX, placementY, 0, numberLine); }
				if (tab[i] == 'M') { queryTextureAndRenderCopy(textureTnt, rectCase, window, renderer, placementX, placementY, 0, numberLine); }
				else if (tab[i] == '1') { queryTextureAndRenderCopy(textureOne, rectCase, window, renderer, placementX, placementY, 0 ,numberLine); }
				else if (tab[i] == '2') { queryTextureAndRenderCopy(textureTwo, rectCase, window, renderer, placementX, placementY, 0 ,numberLine); }
				else if (tab[i] == '3') { queryTextureAndRenderCopy(textureThree, rectCase, window, renderer, placementX, placementY, 0 ,numberLine); }
				else if (tab[i] == '4') { queryTextureAndRenderCopy(textureFour, rectCase, window, renderer, placementX, placementY, 0 ,numberLine); }
				else if (tab[i] == '5') { queryTextureAndRenderCopy(textureFive, rectCase, window, renderer, placementX, placementY, 0 ,numberLine); }
				else if (tab[i] == '6') { queryTextureAndRenderCopy(textureSix, rectCase, window, renderer, placementX, placementY, 0 ,numberLine); }
				else if (tab[i] == '7') { queryTextureAndRenderCopy(textureSeven, rectCase, window, renderer, placementX, placementY, 0 ,numberLine); }
				else if (tab[i] == '8') { queryTextureAndRenderCopy(textureEight, rectCase, window, renderer, placementX, placementY, 0 ,numberLine); }
				else if (tab[i] == '$') { queryTextureAndRenderCopy(textureFlag, rectFlag, window, renderer, placementX, placementY, 0 ,numberLine); }
			}
		}

		SDL_RenderPresent(renderer);
    }

    SDL_DestroyRenderer(renderer);
    Destroy(window, renderer);
    return EXIT_SUCCESS;
}

void Error(const char* message){
    SDL_Log("ERREUR : %s > %s\n", message, SDL_GetError());
    SDL_Quit();
    exit(EXIT_FAILURE);
}

void Destroy(SDL_Window* window, SDL_Renderer* renderer) {
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
}
int verificationMine(int* mine, int lineChoice, int columnChoice, char* tab, int sizeTab, int sizeMine) {
	
	// Fonction qui prend en paramètre le tableau contenant le placement des mines, le tableau de jeu, la ligne et la colone séléctionner par le joueur
	int numberLine = (int)sqrt(sizeTab);
	int i = 0;
	int j = 0;
	int placement = lineChoice * numberLine + columnChoice;

	for (i = 0; i < sizeMine; i++) {
		if (placement == mine[i]) {
			for (j = 0;j<sizeMine;j++){tab[mine[j]] = 'M';}
				
			return 1;
		}
	}
	return 0;
}

void mineProximity(char* tab, int* mine, int lineChoice, int columnChoice, int* verifiedTab , int sizeMine , int sizeTab) {
	int numberLine = (int)sqrt(sizeTab);
	int i = 0;
	int nbMine = 0;
	int placement = lineChoice * numberLine + columnChoice;
	int verified;
	int isMine = 0;
	int top = numberLine - 1;
	int bot = sizeTab - numberLine;
	int left = 0;
	int right = numberLine - 1;
	int upLeft = numberLine + 1;
	int up = numberLine;
	int upRight = numberLine - 1;
	int downLeft = numberLine - 1;
	int down = numberLine;
	int downRight = numberLine + 1;


	verified = verificationTab(verifiedTab, placement,sizeTab);
	for (i = 0; i < sizeMine; i++) {
		if (placement == mine[i]) {
			isMine = 1;
		}
	}

	i = 0;
	if (isMine == 0) {
		for (i = 0; i < sizeMine; i++) {
			if (placement > top) {
				if (placement < bot) {
					if (placement % numberLine != left) {
						if (placement % numberLine != right) {
							if (placement - upRight == mine[i]) { nbMine += 1; }
							if (placement + 1 == mine[i] ) { nbMine += 1; }
							if (placement + downRight == mine[i] ) { nbMine += 1; }
							if (placement - 1 == mine[i]) { nbMine += 1; }
							if (placement - upLeft == mine[i]) { nbMine += 1; }
							if (placement + downLeft == mine[i]) { nbMine += 1; }
							if (placement + down == mine[i]) { nbMine += 1; }
							if (placement - up == mine[i]) { nbMine += 1; }
						}
						else {
							if (placement - 1 == mine[i]) { nbMine += 1; }
							if (placement - upLeft == mine[i]) { nbMine += 1; }
							if (placement + downLeft == mine[i]) { nbMine += 1; }
							if (placement + down == mine[i]) { nbMine += 1; }
							if (placement - up == mine[i]) { nbMine += 1; }
						}
					}
					else {
						if (placement + down == mine[i]) { nbMine += 1; }
						if (placement - up == mine[i]) { nbMine += 1; }
						if (placement - upRight == mine[i]) { nbMine += 1; }
						if (placement + 1 == mine[i]) { nbMine += 1; }
						if (placement + downRight == mine[i]) { nbMine += 1; }
					}
					
				}
				else {
					if (placement % numberLine != right) {
						if (placement % numberLine != left) {
							if (placement - up == mine[i]) { nbMine += 1; }
							if (placement - upRight == mine[i]) { nbMine += 1; }
							if (placement - upLeft == mine[i]) { nbMine += 1; }
							if (placement + 1 == mine[i]) { nbMine += 1; }
							if (placement - 1 == mine[i]) { nbMine += 1; }
						}
						else {
							if (placement - up == mine[i]) { nbMine += 1; }
							if (placement - upRight == mine[i]) { nbMine += 1; }
							if (placement + 1 == mine[i]) { nbMine += 1; }
						}
					}
					else {
						if (placement - up == mine[i]) { nbMine += 1; }
						if (placement - upLeft == mine[i]) { nbMine += 1; }
					}
				}
				
			}
			else {
				if (placement % numberLine != right) {
					if (placement % numberLine != left) {
						if (placement - 1 == mine[i]) { nbMine += 1; }
						if (placement + downLeft == mine[i]) { nbMine += 1; }
						if (placement + downRight == mine[i]) { nbMine += 1; }
						if (placement + 1 == mine[i]) { nbMine += 1; }
						if (placement + down == mine[i]) { nbMine += 1; }
					}
					else {
						if (placement + downRight == mine[i]) { nbMine += 1; }
						if (placement + 1 == mine[i]) { nbMine += 1; }
						if (placement + down == mine[i]) { nbMine += 1; }
					}
					
				}
				else{
					if (placement - 1 == mine[i]) { nbMine += 1; }
					if (placement + downLeft == mine[i]) { nbMine += 1; }
					if (placement + down == mine[i]) { nbMine += 1; }
				}
			}
		
		}
		
		tab[placement] = nbMine + 48;
	}

	
	if (nbMine == 0 && verified == 0 && placement >= 0 && placement <= (sizeTab - 1)) {
		
		if (placement > top){
			if (placement < bot) {
				if (placement % numberLine != left) {
					if (placement % numberLine != right) {
						return (mineProximity(tab, mine, lineChoice - 1, columnChoice, verifiedTab, sizeMine, sizeTab), mineProximity(tab, mine, lineChoice - 1, columnChoice + 1, verifiedTab, sizeMine, sizeTab), mineProximity(tab, mine, lineChoice, columnChoice + 1, verifiedTab, sizeMine, sizeTab), mineProximity(tab, mine, lineChoice + 1, columnChoice + 1, verifiedTab, sizeMine, sizeTab), mineProximity(tab, mine, lineChoice + 1, columnChoice, verifiedTab, sizeMine, sizeTab), mineProximity(tab, mine, lineChoice + 1, columnChoice - 1, verifiedTab, sizeMine, sizeTab), mineProximity(tab, mine, lineChoice, columnChoice - 1, verifiedTab, sizeMine, sizeTab), mineProximity(tab, mine, lineChoice - 1, columnChoice - 1, verifiedTab, sizeMine, sizeTab));
					}
					return (mineProximity(tab, mine, lineChoice - 1, columnChoice, verifiedTab, sizeMine, sizeTab), mineProximity(tab, mine, lineChoice + 1, columnChoice, verifiedTab, sizeMine, sizeTab), mineProximity(tab, mine, lineChoice + 1, columnChoice - 1, verifiedTab, sizeMine, sizeTab), mineProximity(tab, mine, lineChoice, columnChoice - 1, verifiedTab, sizeMine, sizeTab), mineProximity(tab, mine, lineChoice - 1, columnChoice - 1, verifiedTab, sizeMine, sizeTab));
				}
				return (mineProximity(tab, mine, lineChoice - 1, columnChoice, verifiedTab, sizeMine, sizeTab), mineProximity(tab, mine, lineChoice - 1, columnChoice + 1, verifiedTab, sizeMine, sizeTab), mineProximity(tab, mine, lineChoice, columnChoice + 1, verifiedTab, sizeMine, sizeTab), mineProximity(tab, mine, lineChoice + 1, columnChoice + 1, verifiedTab, sizeMine, sizeTab), mineProximity(tab, mine, lineChoice + 1, columnChoice, verifiedTab, sizeMine, sizeTab));
			}
			if (placement % numberLine != right) {
				if (placement % numberLine != left) {
					return (mineProximity(tab, mine, lineChoice - 1, columnChoice, verifiedTab, sizeMine, sizeTab), mineProximity(tab, mine, lineChoice - 1, columnChoice + 1, verifiedTab, sizeMine, sizeTab), mineProximity(tab, mine, lineChoice, columnChoice + 1, verifiedTab, sizeMine, sizeTab), mineProximity(tab, mine, lineChoice, columnChoice - 1, verifiedTab, sizeMine, sizeTab), mineProximity(tab, mine, lineChoice - 1, columnChoice - 1, verifiedTab, sizeMine, sizeTab));
				}
				return (mineProximity(tab, mine, lineChoice - 1, columnChoice, verifiedTab, sizeMine, sizeTab), mineProximity(tab, mine, lineChoice - 1, columnChoice + 1, verifiedTab, sizeMine, sizeTab), mineProximity(tab, mine, lineChoice, columnChoice + 1, verifiedTab, sizeMine, sizeTab));
			}
			return (mineProximity(tab, mine, lineChoice - 1, columnChoice, verifiedTab, sizeMine, sizeTab),  mineProximity(tab, mine, lineChoice, columnChoice - 1, verifiedTab, sizeMine, sizeTab), mineProximity(tab, mine, lineChoice - 1, columnChoice - 1, verifiedTab, sizeMine, sizeTab));
			
		}
		if (placement % numberLine != right) {
			if (placement % numberLine != left) {
				return (mineProximity(tab, mine, lineChoice, columnChoice + 1, verifiedTab, sizeMine, sizeTab), mineProximity(tab, mine, lineChoice + 1, columnChoice + 1, verifiedTab, sizeMine, sizeTab), mineProximity(tab, mine, lineChoice + 1, columnChoice, verifiedTab, sizeMine, sizeTab), mineProximity(tab, mine, lineChoice + 1, columnChoice - 1, verifiedTab, sizeMine, sizeTab), mineProximity(tab, mine, lineChoice, columnChoice - 1, verifiedTab, sizeMine, sizeTab));
			}
			return (mineProximity(tab, mine, lineChoice, columnChoice + 1, verifiedTab, sizeMine, sizeTab), mineProximity(tab, mine, lineChoice + 1, columnChoice + 1, verifiedTab, sizeMine, sizeTab), mineProximity(tab, mine, lineChoice + 1, columnChoice, verifiedTab, sizeMine, sizeTab));
		}
		return (mineProximity(tab, mine, lineChoice + 1, columnChoice, verifiedTab, sizeMine, sizeTab), mineProximity(tab, mine, lineChoice + 1, columnChoice - 1, verifiedTab, sizeMine, sizeTab), mineProximity(tab, mine, lineChoice, columnChoice - 1, verifiedTab, sizeMine, sizeTab));
	}

}

int verificationTab(int * verified, int placement, int sizeTab) {
	int i = 0;
	
	while (verified[i] != -1 && i < sizeTab) {
		if (verified[i] == placement) {
			return 1;
		}
		i++;
	}
	verified[i] = placement;
	return 0;
	
}

int victory(int * verified, int sizeTab, int sizeMine) {
	if (verified[sizeTab - (sizeMine+1)] == -1) {
		return 0;
	}
	return 1;
}

void mineCreation(int lineChoice, int columnChoice, int numberLine, int sizeMine, int sizeTab, int* mine) {
	int i = 0;
	int j = 0;
    int placement;
	time_t t;
	srand((unsigned)time(&t));

	placement = lineChoice  * numberLine + columnChoice;


	i = 0;
	for (i = 0; i < sizeMine; i++) {
		mine[i] = rand() % sizeTab - 1;
		// Une mine ne peut pas être la où le joueur à joué son premier tour
		if (mine[i] == placement || mine[i] == placement - 1 || mine[i] == placement + 1 || mine[i] == placement - numberLine || mine[i] == placement - numberLine - 1 || mine[i] == placement - numberLine + 1 || mine[i] == placement + numberLine || mine[i] == placement + numberLine - 1 || mine[i] == placement + numberLine + 1 || mine[i] <= 0 || mine[i] >= sizeTab - 1) {

			i -= 1;
		}
		if (i >= 1) {
			for (j = 0; j < i; j++) {
				// Deux mines ne peuvent pas être au même endroit
				if (mine[i] == mine[j]) {
					mine[j] = rand() % sizeTab - 1;
					j = 0;
				}
				if (mine[j] == placement || mine[j] == placement - 1 || mine[j] == placement + 1 || mine[j] == placement - numberLine || mine[j] == placement - numberLine - 1 || mine[j] == placement - numberLine + 1 || mine[j] == placement + numberLine || mine[j] == placement + numberLine - 1 || mine[j] == placement + numberLine + 1 || mine[j] <= 0 || mine[j] >= sizeTab - 1) {
					mine[j] = rand() % sizeTab - 1;
					j -= 1;
				}
			}
		}
	}
}

void putFlag (char * tab, int* mine, int lineChoice, int columnChoice, int* verifiedTab, int sizeTab){
	int numberLine = (int)sqrt(sizeTab);
	int i = 0;
	int placement = lineChoice * numberLine + columnChoice;
	int verified = 0;

	for (i=0;i<sizeTab;i++){
		if (verifiedTab[i] == placement){
			verified = 1;
		}
	}
	if (verified == 0){
		tab[placement] = '$';
	}
	else {
		printf("Erreur, choix imposible");
	}

}

void queryTextureAndRenderCopy(SDL_Texture* texture, SDL_Rect rect, SDL_Window* window, SDL_Renderer* renderer, int placementX, int placementY, int screen, int numberLine) {
	
	if (SDL_QueryTexture(texture, NULL, NULL, &rect.w, &rect.h) != 0) {
		Error("Impossible de charger la texture");
		Destroy(window, renderer);
	}
	rect.x = placementX;
	rect.y = placementY;

	if (screen) {
		rect.w = windowWidth;
		rect.h = windowHeight;
	}

	if (!screen) {
		rect.w = (windowWidth / numberLine);
		rect.h = (windowHeight / numberLine);
	}
	
	if (SDL_RenderCopy(renderer, texture, NULL, &rect) != 0) {
		Error("Impossible d'afficher l'image");
		Destroy(window, renderer);
	}
}
