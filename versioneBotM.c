#include <stdio.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <graphics.h> // Libreria SDL_bgi
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <time.h>

typedef struct {
    float carta;
    char segno[10];
    char immagine[30]; //Percorso immagine della carta
} setteMezzo;

// Funzioni del gioco
void generaMazzo(setteMezzo mazzo[]) {
    int c = 0;
    char semi[4][10] = {"Cuori", "Denari", "Fiori", "Picche"};

    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 10; j++) {
            strcpy(mazzo[c].segno, semi[i]);
            mazzo[c].carta = j + 1;
            if (j >= 7) {
                mazzo[c].carta = 0.5;
            }
            sprintf(mazzo[c].immagine, "carte/%s%d.png", semi[i], j + 1); // Nome immagine
            c++;
        }
    }
}

void mischiaMazzo(setteMezzo mazzo[]) {
    srand(time(NULL));
    int i, j;
    setteMezzo temp;
    for (i = 0; i < 40; i++) {
        j = rand() % 40;
        temp = mazzo[i];
        mazzo[i] = mazzo[j];
        mazzo[j] = temp;
    }
}

void AggiornaMazzo(setteMezzo mazzo[]) {
    for (int i = 0; i < 39; i++) {
        mazzo[i] = mazzo[i + 1];
    }
}

setteMezzo pescaCarta(setteMezzo mazzo[]) {
    setteMezzo cartaPescata = mazzo[0];
    AggiornaMazzo(mazzo);
    return cartaPescata;
}

setteMezzo pescataBot(setteMezzo mazzo[]) {
  setteMezzo cartaPescataBot = mazzo[0];
  AggiornaMazzo(mazzo);
  return cartaPescataBot;
}

// Funzione per visualizzare un messaggio con SDL_bgi
void visualizzaMessaggio(const char *messaggio, int x, int y, int colore) {
    setcolor(colore);
    outtextxy(x, y, (char *)messaggio);
}

int main(int argc, char *argv[]) {  

    //Inizializzazione SDL per testo 
    initwindow(500, 300);
    SDL_SetWindowTitle(bgi_window, "Sette e Mezzo - Messaggi");
 
   
    // Inizializzazione SDL
    if (SDL_Init(SDL_INIT_VIDEO) != 0) {
        printf("Errore SDL: %s\n", SDL_GetError());
        return 1;
    }

    if (!(IMG_Init(IMG_INIT_PNG) & IMG_INIT_PNG)) {
        printf("Errore SDL_image: %s\n", IMG_GetError());
        SDL_Quit();
        return 1;
    }

    // Creazione della finestra e del renderer
    SDL_Window *window = SDL_CreateWindow("Sette e Mezzo",
                                          SDL_WINDOWPOS_CENTERED,
                                          SDL_WINDOWPOS_CENTERED,
                                          1920, 1080,
                                          SDL_WINDOW_SHOWN);
    if (!window) {
        printf("Errore creazione finestra: %s\n", SDL_GetError());
        SDL_Quit();
        return 1;
    }

    SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if (!renderer) {
        printf("Errore creazione renderer: %s\n", SDL_GetError());
        SDL_DestroyWindow(window);
        SDL_Quit();
        return 1;
    }

    // Caricamento sfondo
    SDL_Surface *bgSurface = IMG_Load("sfondotintaunita.png");
    if (!bgSurface) {
        printf("Errore caricamento immagine sfondo: %s\n", IMG_GetError());
        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(window);
        SDL_Quit();
        return 1;
    }
    SDL_Texture *bgTexture = SDL_CreateTextureFromSurface(renderer, bgSurface);
    SDL_FreeSurface(bgSurface);
    
    SDL_Surface *superficieRetroCarta = IMG_Load("retrocarta.png");
    if (!superficieRetroCarta) {
        printf("Errore caricamento immagine: %s\n", IMG_GetError());
        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(window);
        SDL_Quit();
        return 1;
    }
    
    SDL_Texture *cartaRetroTexture = SDL_CreateTextureFromSurface(renderer, superficieRetroCarta);
    SDL_FreeSurface(superficieRetroCarta);

    setteMezzo mazzo[40];
    generaMazzo(mazzo);
    mischiaMazzo(mazzo);

    float somma = 0;
    bool exit = false;
      
        
    SDL_Rect* dstrect = malloc(sizeof(SDL_Rect));
	dstrect->x = 100;
	dstrect->y = 100;
	dstrect->w = 190;
	dstrect->h = 290;
        
        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255); // Colore bianco
        SDL_Rect rectInit = {100, 100, 210, 295};             // Rettangolo
        SDL_RenderFillRect(renderer, &rectInit);
        
        //disegno dello sfondo
    	SDL_RenderClear(renderer); 
    	SDL_RenderCopy(renderer, bgTexture, NULL, NULL);
    	SDL_RenderCopy(renderer, cartaRetroTexture, NULL, dstrect);

	refresh();
        SDL_RenderPresent(renderer);

    do {

        // Mostra messaggi con SDL_bgi
        cleardevice();
        char messaggio[50];
        sprintf(messaggio, "Somma corrente: %.1f", somma);
        visualizzaMessaggio(messaggio, 50, 50, WHITE);

        visualizzaMessaggio("Premi TASTO SINISTRO del mouse per pescare una carta", 50, 100, WHITE);
        visualizzaMessaggio("Premi TASTO DESTRO del mouse per fermarti", 50, 130, WHITE);

	refresh();
        SDL_RenderPresent(renderer);

	SDL_Event event; // Declare SDL event
	    bool click = true;
	    int scelta = 0;

	    while (click) {
		
		while (SDL_PollEvent(&event)) {
		    if (event.type == SDL_QUIT) {
		        click = false; 
		    }
		    if (event.type == SDL_MOUSEBUTTONDOWN) {
		        
		        if (event.button.button == SDL_BUTTON_LEFT) { 
		            int x = event.button.x;
		            int y = event.button.y;

		           
		            if (x >= 100 && x <= 290 && y >= 100 && y <= 390) {
		                scelta = 1;
		                click = false; 
		            }
		        } else if (event.button.button == SDL_BUTTON_RIGHT) { 
		            scelta = 2;
		            click = false; 
		        }
		    }
		}
	 }
        
        if (scelta == 1) {
            setteMezzo cartaPescata = pescaCarta(mazzo);
            somma += cartaPescata.carta;

            // Caricamento immagine della carta
            SDL_Surface *cardSurface = IMG_Load(cartaPescata.immagine);
            if (!cardSurface) {
                printf("Errore caricamento immagine carta: %s\n", IMG_GetError());
            } else {
                SDL_Texture *cardTexture = SDL_CreateTextureFromSurface(renderer, cardSurface);
                SDL_FreeSurface(cardSurface);
                
                SDL_RenderCopy(renderer, bgTexture, NULL, NULL);
                   
                SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255); // Colore bianco
	        SDL_Rect rect = {100, 100, 210, 295};             // Rettangolo
        	SDL_RenderFillRect(renderer, &rect);
                   
                SDL_RenderCopy(renderer, cartaRetroTexture, NULL, dstrect);
                
                SDL_Rect cardRect = {850, 500, 200, 300};
                SDL_RenderCopy(renderer, cardTexture, NULL, &cardRect);
                SDL_DestroyTexture(cardTexture);
            }
            // Mostra messaggio della carta pescata
            sprintf(messaggio, "Hai pescato: %.1f di %s", cartaPescata.carta, cartaPescata.segno);
            visualizzaMessaggio(messaggio, 50, 200, YELLOW);
            
            delay(500);

            if (somma > 7.5) {
                cleardevice();
                visualizzaMessaggio("Hai sballato! Hai perso.", 50, 120, RED);
                delay(1000);
                exit = true;
            }
            
        } else if (scelta == 2) {
            cleardevice();
            visualizzaMessaggio("Hai scelto di fermarti.", 50, 120, GREEN);
	    delay(1500);

	    setteMezzo primaCartaBot = mazzo[0];

	    float sommaBot = primaCartaBot.carta;
	    SDL_Surface *cardSurface = IMG_Load(primaCartaBot.immagine);
        	    if (!cardSurface) {
                	printf("Errore caricamento immagine carta: %s\n", IMG_GetError());
            	} else {
                	SDL_Texture *cardTexture = SDL_CreateTextureFromSurface(renderer, cardSurface);
			SDL_Rect cardRect = {850, 110, 200, 300};
                	SDL_RenderCopy(renderer, cardTexture, NULL, &cardRect);
                	SDL_DestroyTexture(cardTexture);
              	  	SDL_FreeSurface(cardSurface);
		}

		 refresh();
       		 SDL_RenderPresent(renderer);
      		 delay(1000);
	        AggiornaMazzo(mazzo);
        	bool sballatoBot = false;
        	int sceltaBot = 1;
	        while (sommaBot < 6) {
	          char messaggio2[50];
	          cleardevice();
                  sprintf(messaggio2, "Somma corrente del BANCO: %.1f", sommaBot);
                  visualizzaMessaggio(messaggio2, 50, 50, WHITE);
                  refresh();
                  if (sommaBot >= 2.5) {
        	    int sceltaBot = rand()%2 + 1;
        	  }
	          if (sceltaBot == 1) {

           	   setteMezzo cartaPescataBot;
          	   cartaPescataBot = pescataBot(mazzo);
           	   sommaBot += cartaPescataBot.carta;

	           SDL_Surface *cardSurface = IMG_Load(cartaPescataBot.immagine);
        	   if (!cardSurface) {
                	printf("Errore caricamento immagine carta: %s\n", IMG_GetError());
            	} else {
                    SDL_Texture *cardTexture = SDL_CreateTextureFromSurface(renderer, cardSurface);
              	    SDL_FreeSurface(cardSurface);
              	    SDL_Rect cardRect = {850, 110, 200, 300};
                    SDL_RenderCopy(renderer, cardTexture, NULL, &cardRect);
                    SDL_DestroyTexture(cardTexture);
              	}
              	refresh();
                SDL_RenderPresent(renderer);
                delay(1000);
                
            if (sommaBot == 7.5) {
              break;
            }

            if (sommaBot > 7.5) {
              visualizzaMessaggio("il Banco ha sballato", 50, 150, GREEN);
              sballatoBot = true;
              break;
            }
          }
          else if (sceltaBot == 2) {
            break;
          }
        }

	delay(1000);

        if(somma>=0 && somma<=7.5) {
          if (somma > sommaBot || sballatoBot) {
            visualizzaMessaggio("Hai vinto", 50, 160, GREEN);
          }

          else {
            visualizzaMessaggio("Hai perso, Il Banco vince", 50, 150, RED);
          }
	}
            exit = true;
        }

        // Aggiorna finestra
        refresh();
        SDL_RenderPresent(renderer);
        delay(1000);
    } while(!exit);
    
    delay(3000);
    
    // Pulizia risorse
    SDL_DestroyTexture(bgTexture);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    IMG_Quit();
    SDL_Quit();
    closegraph();
    return 0;
}
