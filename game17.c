//BAT HUNTER
//Trabalho de computação I - 2016.1
//Jogo de Mariana Mendes e Thalia Ferreira
//POWERFUL DEVELOPERS ©

#include <stdio.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_mixer.h>

#define MAX_BULLETS 1000
#define MAX_ENEMIES 50

const int SCREEN_WIDTH = 1000;
const int SCREEN_HEIGHT = 600;

typedef struct
{
	float x, y, w, h;
	int life;
	char *nome;
	int shooting, alive, facingleft, facingright, facingup;
} GIRL;

typedef struct 
{
	float x, y, w, h;
	int alive;
} Enemy;

typedef struct
{
	int x, y, vel;
} Bullet;

typedef struct 
{
	GIRL girl;

	//Enemy enemies[4];

	//logo
	SDL_Texture *logo;

	//menu
	SDL_Texture *menu;

	//instrucoes
	SDL_Texture *instrucoes;

	//creditos
	SDL_Texture *creditos;

	//background
	SDL_Texture *background;

	//background game over
	SDL_Texture *background_gameover;

	//background you win
	SDL_Texture *background_youwin;

	//bullet
	SDL_Texture *bulletTexture;

	//player
	SDL_Texture *girlImg;
	SDL_Rect srcRect;		
	
	//enemy
	SDL_Texture *enemyImg;
	
	//renderer
	SDL_Renderer *renderer;

	//musicadomenu
	Mix_Chunk *mus_menu;

} GameState;


	Bullet *bullets[MAX_BULLETS] = {NULL};

	Enemy *enemies0[MAX_ENEMIES] = {NULL};
	Enemy *enemies1[MAX_ENEMIES] = {NULL};
	Enemy *enemies2[MAX_ENEMIES] = {NULL};
	Enemy *enemies3[MAX_ENEMIES] = {NULL};

	int troca=0, opcao=0; //explode=0;
	int qtdeEnemies=4;
	int time = 1;


void addBullet(int x, int y, int vel)
{
  int found = -1;
  for(int i = 0; i < MAX_BULLETS; i++)
  {
    if(bullets[i] == NULL)
    {
      found = i;
      break;
    }
  }
    
  if(found >= 0)
  {
    int i = found;
    bullets[i] = malloc(sizeof(Bullet));
    bullets[i]->x = x;
    bullets[i]->y = y;
    bullets[i]->vel = vel;  
  }
}

void removeBullet(int i)
{
  if(bullets[i])
  {
    free(bullets[i]);
    bullets[i] = NULL;
  }
}

void addEnemy(Enemy *enemy[MAX_ENEMIES], int x, int y, int w, int h)
{
  int found = -1;
  for(int i = 0; i < MAX_ENEMIES; i++)
  {
    if(enemy[i] == NULL)
    {
      found = i;
      break;
    }
  }
    
  if(found >= 0)
  {
    int i = found;
    enemy[i] = malloc(sizeof(Enemy));
    enemy[i]->x = x;
    enemy[i]->y = y;
    enemy[i]->w = w;
    enemy[i]->y = h;
    enemy[i]->alive = 1;  
  }
}

void removeEnemy(Enemy *enemy[MAX_ENEMIES], int i)
{
  if(enemy[i])
  {
    free(enemy[i]);
    enemy[i] = NULL;
  }
}


void LoadGame(GameState *game)
{
	SDL_Surface *surface = NULL;

	surface = IMG_Load("logomarca.jpg");
	if(surface == NULL)
	{
		printf("Can't find logomarca.jpg!\n");
		SDL_Quit();
		exit(1);
	}

	game->logo = SDL_CreateTextureFromSurface(game->renderer, surface);
	SDL_FreeSurface(surface);

	//img menu
	surface = IMG_Load("menu.jpg");
	if(surface == NULL)
	{
		printf("Can't find menu.jpg!\n");
		SDL_Quit();
		exit(1);
	}

	game->menu = SDL_CreateTextureFromSurface(game->renderer, surface);
	SDL_FreeSurface(surface);

	//instrucoes
	surface = IMG_Load("instrucoes.jpg");
	if(surface == NULL)
	{
		printf("Can't find instrucoes.jpg!\n");
		SDL_Quit();
		exit(1);
	}

	game->instrucoes = SDL_CreateTextureFromSurface(game->renderer, surface);
	SDL_FreeSurface(surface);

	//creditos
	surface = IMG_Load("creditos.jpg");
	if(surface == NULL)
	{
		printf("Can't find creditos.jpg!\n");
		SDL_Quit();
		exit(1);
	}

	game->creditos = SDL_CreateTextureFromSurface(game->renderer, surface);
	SDL_FreeSurface(surface);

	//fundo
	surface = IMG_Load("background.jpg");
	if(surface == NULL)
	{
		printf("Can't find background.jpg!\n");
		SDL_Quit();
		exit(1);
	}

	game->background = SDL_CreateTextureFromSurface(game->renderer, surface);
	SDL_FreeSurface(surface);

	//fundo game over
	surface = IMG_Load("gameover.jpg");
	if(surface == NULL)
	{
		printf("Can't find gameover.jpg!\n");
		SDL_Quit();
		exit(1);
	}

	game->background_gameover = SDL_CreateTextureFromSurface(game->renderer, surface);
	SDL_FreeSurface(surface);

	//fundo you win
	// surface = IMG_Load("YouWin.png");
	// if(surface == NULL)
	// {
	// 	printf("Can't find YouWin.png!\n");
	// 	SDL_Quit();
	// 	exit(1);
	// }

	// game->background_youwin = SDL_CreateTextureFromSurface(game->renderer, surface);
	// SDL_FreeSurface(surface);

	//player
	game->srcRect.x = 0;
	game->srcRect.y = 0;
	game->srcRect.w = game->girl.w;
	game->srcRect.h = game->girl.h;

	surface = IMG_Load("girl.png");
	if(surface == NULL)
	{
		printf("Can't find girl.png!\n");
		SDL_Quit();
		exit(1);
	}

	game->girlImg = SDL_CreateTextureFromSurface(game->renderer, surface);
	SDL_FreeSurface(surface);

	//enemy
	surface = IMG_Load("enemy.gif");
	if(surface == NULL)
	{
		printf("Can't find enemy.gif!\n");
		SDL_Quit();
		exit(1);
	}
	
	game->enemyImg = SDL_CreateTextureFromSurface(game->renderer, surface);
	SDL_FreeSurface(surface);

	//bullet
	surface = IMG_Load("bullet.png");
	if(surface == NULL)
	{
		printf("can't find bullet.png\n");
		SDL_Quit();
		exit(1);
	}

	game->bulletTexture = SDL_CreateTextureFromSurface(game->renderer, surface);
	SDL_FreeSurface(surface);
}

int enemyMoviment(Enemy *enemy[MAX_ENEMIES], GameState *game, int speedE)
{
	float j;
	int i, gameRunning = 1;

	for(i=0; i<MAX_ENEMIES; i++)
   	{
		if(enemy[i]->x < game->girl.x)
	    {
	    	for(j=0.5; j <= speedE; j++)
	    	{
	    		enemy[i]->x += j;
	    		if(enemy[i]->x == game->girl.x)
	    		{
	    			speedE = 1;
	    		}
	    		
	    	}
	    }
	    if(enemy[i]->x > game->girl.x)
	    {
	    	for(j=0.5; j <= speedE; j++)
	    	{
	    		enemy[i]->x -= j;
	    		if(enemy[i]->x == game->girl.x)
	    		{
	    			speedE = 1;
	    		}
	    		
	    	}
	    }
	    if(enemy[i]->y < game->girl.y)
	    {
	    	for(j=0.5; j <= speedE; j++)
	    	{
	    		enemy[i]->y += j;
	    		if(enemy[i]->y == game->girl.y)
	    		{
	    			speedE = 1;
	    		}
	    		
	    	}
	    }
	    if(enemy[i]->y > game->girl.y)
	    {
	    	for(j=0.5; j <= speedE; j++)
	    	{
	    		enemy[i]->y -= j;
	    		if(enemy[i]->y == game->girl.y)
	    		{
	    			speedE = 1;
	    		}
	    		
	    	}
	    }
		if(enemy[i]->x < game->girl.x && enemy[i]->y < game->girl.y)
		{
			for(j=0.5; j <= speedE; j++)
			{
				enemy[i]->x += j;
				enemy[i]->y += j;
				if(enemy[i]->x == game->girl.x || enemy[i]->y == game->girl.y)
				{
	    			speedE = 1;
	    		}
	    		
			}
		}
		if (enemy[i]->x > game->girl.x && enemy[i]->y > game->girl.y)
		{
			for(j=0.5; j <= speedE; j++)
			{
				enemy[i]->x -= j;
				enemy[i]->y -= j;
				if(enemy[i]->x == game->girl.x || enemy[i]->y == game->girl.y)
				{
	    			speedE = 1;
	    		}
	    		
			}
		}
		if(enemy[i]->y < game->girl.y && enemy[i]->x > game->girl.x)
		{
			for(j=0.5; j <= speedE; j++)
			{
				enemy[i]->y += j;
	 			enemy[i]->x -= j;
	 			if(enemy[i]->x == game->girl.x || enemy[i]->y == game->girl.y)
	 			{
	    			speedE = 1;
	    		}
	    		
	 		}
	 	}
	 	if(enemy[i]->y > game->girl.y && enemy[i]->x < game->girl.x)
		{
			for(j=0.5; j <= speedE; j++)
			{
				enemy[i]->y -= j;
				enemy[i]->x += j;
				if(enemy[i]->x == game->girl.x || enemy[i]->y == game->girl.y)
	 			{
	    			speedE = 1;
	    		}	
			}
		}


		//colisao girl vs enemy
	    if (enemy[i]->x+30 <= game->girl.x+60 && enemy[i]->x+30 >= game->girl.x)
	    { 
	    	if(enemy[i]->y+10 <= game->girl.y+90 && enemy[i]->y+10 >= game->girl.y)
	   	  		game->girl.alive=0;
	    }

	    if(!game->girl.alive)
	    	gameRunning = 0;
	}

	return gameRunning;
}


int processEvent(SDL_Window *window, GameState *game)
{
	SDL_Event event;
	GameState gameState;
	int gameRunning = 1; 
	int speedP = 5, speedE = 1;
	
	while(SDL_PollEvent(&event))
	{
		switch(event.type)
		{
			case SDL_QUIT:
				gameRunning = 0;
			break;
		}
	}

	//Movimento do player
	const Uint8 *state = SDL_GetKeyboardState(NULL);
	    if(state[SDL_SCANCODE_UP])
	    {
	    	game->girl.facingright = 0;
	    	game->girl.facingleft = 0;
	    	game->girl.facingup = 1;

	        game->girl.y -= speedP;

	        game->srcRect.x = 4*game->girl.w;
		   	game->srcRect.y = 0;

	    	if(game->srcRect.x != 4*game->girl.w && game->srcRect.y != 0)
	    	{
	    		game->srcRect.x = 4*game->srcRect.x;
	    		game->srcRect.y = 0;
	    	}

	   	}

	    if(state[SDL_SCANCODE_DOWN])
	    {
	        game->girl.facingright = 0;
	    	game->girl.facingleft = 0;
	    	game->girl.facingup = 0;

	       	game->girl.y += speedP;
	
	       	game->srcRect.x = 0;
	    	game->srcRect.y = 0;

	    	if(game->srcRect.x != 0 && game->srcRect.y != 0)
	    		game->srcRect.x = 0;
	    }

	    if(state[SDL_SCANCODE_LEFT])
	    {
	    	game->girl.facingright = 0;
	    	game->girl.facingleft = 1;
	    	game->girl.facingup = 0;

	    	game->girl.x -= speedP;

	    	if(game->srcRect.y == 0)
	    		game->srcRect.y += game->girl.h;

	    	if(game->srcRect.y != game->girl.h)
	    		game->srcRect.y = game->girl.h;

            if (game->srcRect.x == 7*game->girl.w)
            {
	            game->srcRect.x = 0;
            }
            else 
            {
            	game->srcRect.x += game->girl.w;
            }
	    }

	    if(state[SDL_SCANCODE_RIGHT])
	    {
	    	game->girl.facingleft = 0;
	    	game->girl.facingright = 1;
	    	game->girl.facingup = 0;

	       	game->girl.x += speedP;

	       	if(game->srcRect.y == 0)
	    		game->srcRect.y += 2*game->girl.h;

	    	if(game->srcRect.y != 2*game->girl.h)
	    		game->srcRect.y = 2*game->girl.h;

            if (game->srcRect.x == 7*game->girl.w)
            {
	            game->srcRect.x = 0;
            }

            else 
            {
            	game->srcRect.x += game->girl.w;
            }
	    }

	    if(state[SDL_SCANCODE_SPACE])
	   	{
	   		time++;

	   		if (game->girl.facingright == 1)
	   		{
		   		if (game->srcRect.x != 420 && game->srcRect.y != 270)
		   		{
		   			game->srcRect.x = 420;
		   			game->srcRect.y = 270;
		   		}

		   		if (time % 6 == 0)
		   		{
		   			addBullet(game->girl.x+60, game->girl.y+45, 6);
		   		}

	   		}

	   		if (game->girl.facingleft == 1)
	   		{
	   			if (game->srcRect.x != 180 && game->srcRect.y != 270)
		   		{
		   			game->srcRect.x = 180;
		   			game->srcRect.y = 270;
		   		}

		   		if (time % 6 == 0)
		   		{
		   			addBullet(game->girl.x-5, game->girl.y+45, -6);
		   		}

	   		}

	   		// if (game->girl.facingup == 1)
	   		// {
	   		// 	addBullet(game->girl.x, game->girl.y+5, 6);
	   		// }

	   		game->girl.shooting = 1;

	   	}

	   	else
	   	{
	   		game->girl.shooting = 0;
	   	}

	 //    //Movimento do inimigo
	   	// for(i=0; i<MAX_ENEMIES; i++)
	   	// {
	   		addEnemy(enemies0, 938, 300, 57, 20);
	   		addEnemy(enemies1, 50, 62, 57, 20);
	   		addEnemy(enemies2, 62, 300, 57, 20);
	   		addEnemy(enemies3, 500, 538, 57, 20);

	   		enemyMoviment(enemies0, &gameState, speedE);
	   		enemyMoviment(enemies1, &gameState, speedE);
	   		enemyMoviment(enemies2, &gameState, speedE);
	   		enemyMoviment(enemies3, &gameState, speedE);
		// }

	return gameRunning;
}


void doRender(GameState *game)
{
	int i;

	if(game->girl.alive == 1)
	{
		//Desenhando o fundo
		SDL_RenderCopy(game->renderer, game->background, NULL, NULL);

		//Desenhando o "player"
		SDL_Rect rect = {game->girl.x, game->girl.y, game->girl.w, game->girl.h};
		SDL_RenderCopy(game->renderer, game->girlImg, &game->srcRect, &rect);

		//Desenhando os inimigos

		for(i = 0; i < MAX_ENEMIES; i++) if(enemies0[i])
		{
			SDL_Rect enemyRect0 = {enemies0[i]->x, enemies0[i]->y, enemies0[i]->w, enemies0[i]->h};
			SDL_RenderCopy(game->renderer, game->enemyImg, NULL, &enemyRect0);
			SDL_Delay(5000);
		}
		
		for(i = 0; i < MAX_ENEMIES; i++) if(enemies1[i])
		{
			SDL_Rect enemyRect1 = {enemies1[i]->x, enemies1[i]->y, enemies1[i]->w, enemies1[i]->h};
			SDL_RenderCopy(game->renderer, game->enemyImg, NULL, &enemyRect1);
			SDL_Delay(5000);
		}

		for(i = 0; i < MAX_ENEMIES; i++) if(enemies2[i])
		{
			SDL_Rect enemyRect2 = {enemies2[i]->x, enemies2[i]->y, enemies2[i]->w, enemies2[i]->h};
			SDL_RenderCopy(game->renderer, game->enemyImg, NULL, &enemyRect2);
			SDL_Delay(5000);
		}

		for(i = 0; i < MAX_ENEMIES; i++) if(enemies3[i])
		{
			SDL_Rect enemyRect3 = {enemies3[i]->x, enemies3[i]->y, enemies3[i]->w, enemies3[i]->h};
			SDL_RenderCopy(game->renderer, game->enemyImg, NULL, &enemyRect3);
			SDL_Delay(5000);
		}

		//Desenhando o bullet
		for(i = 0; i < MAX_BULLETS; i++) if(bullets[i])
		{
			SDL_Rect rect = {bullets[i]->x, bullets[i]->y, 16, 16};
			SDL_RenderCopy(game->renderer, game->bulletTexture, NULL, &rect);
			SDL_Delay(5000);
		}
		
		//Mostrando a obra de arte para o usuário :)
		SDL_RenderPresent(game->renderer);
	}
	
	if(game->girl.alive == 0)
	{
		//Desenhando o fundo game over
		SDL_RenderCopy(game->renderer, game->background_gameover, NULL, NULL);
		SDL_RenderPresent(game->renderer);
	}

	// if(qtdeEnemies == 0)
	// {
	// 	//Desenhando o fundo you win
	// 	SDL_RenderCopy(game->renderer, game->background_youwin, NULL, NULL);
	// 	SDL_RenderPresent(game->renderer);
	// }


}

void collisionPlayer(SDL_Window *window, GameState *game)
{
    int wallWidth = 62;

    if (game->girl.x < wallWidth ) 
    {
        game->girl.x = wallWidth;
    }
    else if (game->girl.x > (SCREEN_WIDTH-wallWidth)-game->girl.w) 
    {
        game->girl.x = (SCREEN_WIDTH-wallWidth)-game->girl.w;
    }
    if (game->girl.y < wallWidth) 
    {
        game->girl.y = wallWidth;
    }
    else if (game->girl.y > (SCREEN_HEIGHT-wallWidth)-game->girl.h) 
    {
        game->girl.y = (SCREEN_HEIGHT-wallWidth)-game->girl.h;
    }
}

void collisionEnemy(SDL_Window *window, Enemy *enemy[MAX_ENEMIES])
{
    int wallWidth = 62;
    int i;

    for(i=0; i<MAX_ENEMIES; i++)
    {
	    if (enemy[i]->x < wallWidth ) 
	    {
	        enemy[i]->x = wallWidth;
	    }
	    else if (enemy[i]->x > (SCREEN_WIDTH-wallWidth)-enemy[i]->w) 
	    {
	        enemy[i]->x = (SCREEN_WIDTH-wallWidth)-enemy[i]->w;
	    }
	    if (enemy[i]->y < wallWidth) 
	    {
	        enemy[i]->y = wallWidth;
	    }
	    else if (enemy[i]->y > (SCREEN_HEIGHT-wallWidth)-enemy[i]->h) 
	    {
	        enemy[i]->y = (SCREEN_HEIGHT-wallWidth)-enemy[i]->h;
	    }
	}
}

int bulletLogic (GameState *game) 
{
	int i, j, checando=0;

	//explode = 0;

	for(i = 0; i < MAX_BULLETS; i++) if(bullets[i])
  	{
	   	bullets[i]->x += bullets[i]->vel;
	   	
	   	for (j = 0; j < MAX_ENEMIES; j++) if(enemies0[j])
		{
			//checando = 0;
	   		//colisão bala vs inimigo
			if(bullets[i]->x >= enemies0[j]->x && bullets[i]->x <= enemies0[j]->x+57 
				&& bullets[i]->y >= enemies0[j]->y && bullets[i]->y <= enemies0[j]->y+20)
		   	{
		   		//checando=1;
		   		//if(checando)
   				//{
		   			enemies0[j]->alive=0;
		   			removeEnemy(enemies0, j);
		   			//qtdeEnemies--;
   				//}
		   	}
	  	}

	  	for (j = 0; j < MAX_ENEMIES; j++) if(enemies1[j])
		{
			//checando = 0;
	   		//colisão bala vs inimigo
			if(bullets[i]->x >= enemies1[j]->x && bullets[i]->x <= enemies1[j]->x+57 
				&& bullets[i]->y >= enemies1[j]->y && bullets[i]->y <= enemies1[j]->y+20)
		   	{
		   		//checando=1;
		   		//if(checando)
   				//{
		   			enemies1[j]->alive=0;
		   			removeEnemy(enemies1, j);
		   			//qtdeEnemies--;
   				//}
		   	}
	  	}

	  	for (j = 0; j < MAX_ENEMIES; j++) if(enemies2[j])
		{
			//checando = 0;
	   		//colisão bala vs inimigo
			if(bullets[i]->x >= enemies2[j]->x && bullets[i]->x <= enemies2[j]->x+57 
				&& bullets[i]->y >= enemies2[j]->y && bullets[i]->y <= enemies2[j]->y+20)
		   	{
		   		//checando=1;
		   		//if(checando)
   				//{
		   			enemies2[j]->alive=0;
		   			removeEnemy(enemies2, j);
		   			//qtdeEnemies--;
   				//}
		   	}
	  	}

	  	for (j = 0; j < MAX_ENEMIES; j++) if(enemies3[j])
		{
			checando = 0;
	   		//colisão bala vs inimigo
			if(bullets[i]->x >= enemies3[j]->x && bullets[i]->x <= enemies3[j]->x+57 
				&& bullets[i]->y >= enemies3[j]->y && bullets[i]->y <= enemies3[j]->y+20)
		   	{
		   		//checando=1;
		   		//if(checando)
   				//{
		   			enemies3[j]->alive=0;
		   			removeEnemy(enemies3, j);
		   			//qtdeEnemies--;
   				//}
		   	}
	  	}

   		if(bullets[i]->x <= 60 || bullets[i]->x >= 838)
	      		removeBullet(i);
	}

}

void quitGame (SDL_Window *window, GameState *game)
{
	SDL_DestroyWindow(window);
	SDL_DestroyTexture(game->girlImg);
	SDL_DestroyRenderer(game->renderer);
	SDL_DestroyTexture(game->enemyImg);
	SDL_DestroyTexture(game->background);
	SDL_DestroyTexture(game->background_gameover);
	SDL_DestroyTexture(game->menu);
	SDL_DestroyTexture(game->logo);
	SDL_DestroyTexture(game->instrucoes);
	SDL_DestroyTexture(game->creditos);
	SDL_DestroyTexture(game->bulletTexture);
	SDL_DestroyTexture(game->background_youwin);
	Mix_FreeChunk(game->mus_menu);

	for(int i = 0; i < MAX_BULLETS; i++)
	{
		removeBullet(i);
	}

	//Limpando a bagunça
	SDL_Quit();
}
	

int main(int argc, char *args[])
{

	GameState gameState;

	SDL_Window *window = NULL;
	
	SDL_Event event;

	Mix_OpenAudio( 22050, MIX_DEFAULT_FORMAT, 2, 1024 );

	gameState.mus_menu = Mix_LoadWAV("menu.wav");

	int gameRunning=1;

	gameState.girl.alive = 1;

	gameState.girl.x = 500;
	gameState.girl.y = 300;
	gameState.girl.w = 60;
	gameState.girl.h = 90;

	// gameState.enemies[0].x = 938;
	// gameState.enemies[0].y = 300;
	// gameState.enemies[0].w = 57;
	// gameState.enemies[0].h = 20;

	// gameState.enemies[1].x = 500;
	// gameState.enemies[1].y = 62;
	// gameState.enemies[1].w = 57;
	// gameState.enemies[1].h = 20;

	// gameState.enemies[2].x = 62;
	// gameState.enemies[2].y = 300;
	// gameState.enemies[2].w = 57;
	// gameState.enemies[2].h = 20;

	// gameState.enemies[3].x = 500;
	// gameState.enemies[3].y = 538;
	// gameState.enemies[3].w = 57;
	// gameState.enemies[3].h = 20;

	// int i;

	// for (i = 0; i < 4; i++)
	// {
	// 	gameState.enemies[i].alive = 1;
	// }
 
	if(SDL_Init(SDL_INIT_EVERYTHING) < 0)
		SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Can't init SDL: %s", SDL_GetError());

	window = SDL_CreateWindow("Bat Hunter!", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, 0);
	gameState.renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);

	LoadGame(&gameState);

	//img logo
	while(troca == 0)
	{
		SDL_RenderCopy(gameState.renderer, gameState.logo, NULL, NULL);
		SDL_RenderPresent(gameState.renderer);
		SDL_Delay(2000);
		troca = 1;
	}

	troca = 0;

	while(troca == 0)
	{
		SDL_RenderCopy(gameState.renderer, gameState.logo, NULL, NULL);
		SDL_RenderPresent(gameState.renderer);
		SDL_Delay(2000);
		troca = 1;
	}


	while(opcao != 1)
	{
		Mix_PlayChannel( 0, gameState.mus_menu, 0 );
		
	 	while(opcao == 0)
	 	{
			SDL_RenderCopy(gameState.renderer, gameState.menu, NULL, NULL);
			SDL_RenderPresent(gameState.renderer);

			while(SDL_PollEvent(&event))
			{
				if(event.type == SDL_KEYDOWN)
				{
					switch(event.key.keysym.sym)
					{
						case SDLK_1: 
							opcao=1;
						break;
						case SDLK_2: 
							opcao=2;
						break;
						case SDLK_3:
							opcao=3;
						break;
						case SDLK_4: 
							{gameRunning = 0;
							quitGame(window, &gameState);
							return 0;}						
						break;
						case SDLK_5:
							opcao=5;
						break;
						case SDLK_ESCAPE:
							{gameRunning = 0;
							quitGame(window, &gameState);
							return 0;}
						break;
					}
				}
				if(event.type == SDL_QUIT)
				{
					{gameRunning = 0;
					quitGame(window, &gameState);
					return 0;}	
				}

			}
	 	}

		while(opcao == 2)
		{
			SDL_RenderCopy(gameState.renderer, gameState.instrucoes, NULL, NULL);
			SDL_RenderPresent(gameState.renderer);

			while(SDL_PollEvent(&event))
			{
				if(event.type == SDL_KEYDOWN)
				{
					switch(event.key.keysym.sym)
					{
						case SDLK_0: 
							opcao=0;
						break;
					}
				}
			}
		}

		while(opcao == 5)
		{
			SDL_RenderCopy(gameState.renderer, gameState.creditos, NULL, NULL);
			SDL_RenderPresent(gameState.renderer);

			while(SDL_PollEvent(&event))
			{
				if(event.type == SDL_KEYDOWN)
				{
					switch(event.key.keysym.sym)
					{
						case SDLK_0: 
							opcao=0;
						break;
					}
				}
			}
		}
	}

	if (opcao == 1)
	{
		SDL_Delay(1000);

		while(gameRunning)
		{

			gameRunning = processEvent(window, &gameState);

			doRender(&gameState);

			bulletLogic(&gameState);

			collisionPlayer(window, &gameState);

			collisionEnemy(window, enemies0);

			collisionEnemy(window, enemies1);

			collisionEnemy(window, enemies2);

			collisionEnemy(window, enemies3);

			//SDL_Delay(10);
		}

	}

	SDL_Delay(5000);

	quitGame(window, &gameState);	

	return 0;
}