//BAT HUNTER
//Trabalho de computação I - 2016.1
//Jogo de Mariana Mendes e Thalia Ferreira
//POWERFUL DEVELOPERS ©

#include <stdio.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_mixer.h>


#define MAX_BULLETS 1000

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
	int x, y, w, h, aparece;
} Premio;


typedef struct 
{
	GIRL girl;

	Enemy enemies[4];

	Premio premio;

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
	SDL_Rect esrcRect;	
	
	//renderer
	SDL_Renderer *renderer;

	//musicadomenu
	Mix_Chunk *mus_menu;

	//premio 1
	SDL_Texture *premioImg;

} GameState;


	Bullet *bullets[MAX_BULLETS] = {NULL};

	int troca=0, opcao=0; //explode=0;
	int qtdeEnemies=4;
	int time = 1;
	int pause = 0;


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


void LoadGame(GameState *game)
{
	SDL_Surface *surface = NULL;
	int i;

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

	game->esrcRect.x = 0;
	game->esrcRect.y = 0;
	game->esrcRect.w = game->enemies[0].w;
	game->esrcRect.h = game->enemies[0].h;

	// game->esrcRect.x = 0;
	// game->esrcRect.y = 0;
	// game->esrcRect.w = game->girl.w;
	// game->esrcRect.h = game->girl.h;

	// game->esrcRect.x = 0;
	// game->esrcRect.y = 0;
	// game->esrcRect.w = game->girl.w;
	// game->esrcRect.h = game->girl.h;

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
	surface = IMG_Load("enemy2.png");
	if(surface == NULL)
	{
		printf("Can't find enemy2.png!\n");
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

	//premio
	surface = IMG_Load("bau1.png");
	if(surface == NULL)
	{
		printf("can't find bau1.png\n");
		SDL_Quit();
		exit(1);
	}

	game->premioImg = SDL_CreateTextureFromSurface(game->renderer, surface);
	SDL_FreeSurface(surface);
}

void premioLogic (GameState *game)
{
	if (game->premio.aparece == 1)
	{

		if (game->premio.x+20 <= game->girl.x+60 && game->premio.x+20 >= game->girl.x)
	    { 
	    	if(game->premio.y+15 <= game->girl.y+90 && game->premio.y+15 >= game->girl.y)
	   	  	{
	   	  			printf("oq fazer? prox fase? you win? chefe?\n");
	   	  	}
	    }

	}
}

int processEvent(SDL_Window *window, GameState *game)
{
	SDL_Event event;
	int gameRunning = 1; 
	int speedP = 5, speedE = 1, i;
	float j;
	
	while(SDL_PollEvent(&event))
	{
		switch(event.type)
		{
			case SDL_QUIT:
				gameRunning = 0;
			break;
		}
	}

	if (pause == 1)
	{
		const Uint8 *state = SDL_GetKeyboardState(NULL);
		    if(state[SDL_SCANCODE_P])
		    {
		    	pause = 0;
		    }

	}

	if(pause == 0)
	{
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

		   		if (time % 20 == 0)
				{
					game->premio.aparece = 1;
				}

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

		   		else if (game->girl.facingleft == 1)
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

		   		game->girl.shooting = 1;

		   	}

		   	else
		   	{
		   		game->girl.shooting = 0;
		   	}

		   	if(state[SDL_SCANCODE_P])
		    {
		    	pause = 1;
		    }

		    //Movimento do inimigo
		   	for(i=0; i<4; i++)
		   	{

			    if(game->enemies[i].x < game->girl.x)
			    {
			    	for(j=0.5; j <= speedE; j++)
			    	{
			    		game->enemies[i].x += j;
			    		if(game->enemies[i].x == game->girl.x)
			    		{
			    			speedE = 1;
			    			//game->girl.alive=0;
			    		}
			    		
			    	}
			    }
			    if(game->enemies[i].x > game->girl.x)
			    {
			    	for(j=0.5; j <= speedE; j++)
			    	{
			    		game->enemies[i].x -= j;
			    		if(game->enemies[i].x == game->girl.x)
			    		{
			    			speedE = 1;
			    			//game->girl.alive=0;
			    		}
			    		
			    	}
			    }
			    if(game->enemies[i].y < game->girl.y)
			    {
			    	for(j=0.5; j <= speedE; j++)
			    	{
			    		game->enemies[i].y += j;
			    		if(game->enemies[i].y == game->girl.y)
			    		{
			    			speedE = 1;
			    			//game->girl.alive=0;
			    		}
			    		
			    	}
			    }
			    if(game->enemies[i].y > game->girl.y)
			    {
			    	for(j=0.5; j <= speedE; j++)
			    	{
			    		game->enemies[i].y -= j;
			    		if(game->enemies[i].y == game->girl.y)
			    		{
			    			speedE = 1;
			    			//game->girl.alive=0;
			    		}
			    		
			    	}
			    }
				if(game->enemies[i].x < game->girl.x && game->enemies[i].y < game->girl.y)
				{
					for(j=0.5; j <= speedE; j++)
					{
						game->enemies[i].x += j;
						game->enemies[i].y += j;
						if(game->enemies[i].x == game->girl.x || game->enemies[i].y == game->girl.y)
						{
			    			speedE = 1;
			    			//game->girl.alive=0;
			    		}
			    		
					}
				}
				if (game->enemies[i].x > game->girl.x && game->enemies[i].y > game->girl.y)
				{
					for(j=0.5; j <= speedE; j++)
					{
						game->enemies[i].x -= j;
						game->enemies[i].y -= j;
						if(game->enemies[i].x == game->girl.x || game->enemies[i].y == game->girl.y)
						{
			    			speedE = 1;
			    			//game->girl.alive=0;
			    		}
			    		
					}
				}
				if(game->enemies[i].y < game->girl.y && game->enemies[i].x > game->girl.x)
				{
					for(j=0.5; j <= speedE; j++)
					{
						game->enemies[i].y += j;
			 			game->enemies[i].x -= j;
			 			if(game->enemies[i].x == game->girl.x || game->enemies[i].y == game->girl.y)
			 			{
			    			speedE = 1;
			    			//game->girl.alive=0;
			    		}
			    		
			 		}
			 	}
			 	if(game->enemies[i].y > game->girl.y && game->enemies[i].x < game->girl.x)
				{
					for(j=0.5; j <= speedE; j++)
					{
						game->enemies[i].y -= j;
						game->enemies[i].x += j;
						if(game->enemies[i].x == game->girl.x || game->enemies[i].y == game->girl.y)
			 			{
			    			speedE = 1;
			    			//game->girl.alive=0;
			    		}
			    		
					}
				}

			    // colisao girl vs enemy
			    // if (game->enemies[i].x+30 <= game->girl.x+60 && game->enemies[i].x+30 >= game->girl.x)
			    // { 

			    // 	if(game->enemies[i].y+10 <= game->girl.y+90 && game->enemies[i].y+10 >= game->girl.y)
			   	//   	{
			   	//   			game->girl.alive = 0;
			   	//   	}
			    // }

			}

		}

	// //Movimento do player
	// const Uint8 *state = SDL_GetKeyboardState(NULL);
	//     if(state[SDL_SCANCODE_UP])
	//     {
	//     	game->girl.facingright = 0;
	//     	game->girl.facingleft = 0;
	//     	game->girl.facingup = 1;

	//         game->girl.y -= speedP;

	//         game->srcRect.x = 4*game->girl.w;
	// 	   	game->srcRect.y = 0;

	//     	if(game->srcRect.x != 4*game->girl.w && game->srcRect.y != 0)
	//     	{
	//     		game->srcRect.x = 4*game->srcRect.x;
	//     		game->srcRect.y = 0;
	//     	}

	//    	}

	//     if(state[SDL_SCANCODE_DOWN])
	//     {
	//         game->girl.facingright = 0;
	//     	game->girl.facingleft = 0;
	//     	game->girl.facingup = 0;

	//        	game->girl.y += speedP;
	
	//        	game->srcRect.x = 0;
	//     	game->srcRect.y = 0;

	//     	if(game->srcRect.x != 0 && game->srcRect.y != 0)
	//     		game->srcRect.x = 0;
	//     }

	//     if(state[SDL_SCANCODE_LEFT])
	//     {
	//     	game->girl.facingright = 0;
	//     	game->girl.facingleft = 1;
	//     	game->girl.facingup = 0;

	//     	game->girl.x -= speedP;

	//     	if(game->srcRect.y == 0)
	//     		game->srcRect.y += game->girl.h;

	//     	if(game->srcRect.y != game->girl.h)
	//     		game->srcRect.y = game->girl.h;

 //            if (game->srcRect.x == 7*game->girl.w)
 //            {
	//             game->srcRect.x = 0;
 //            }
 //            else 
 //            {
 //            	game->srcRect.x += game->girl.w;
 //            }
	//     }

	//     if(state[SDL_SCANCODE_RIGHT])
	//     {
	//     	game->girl.facingleft = 0;
	//     	game->girl.facingright = 1;
	//     	game->girl.facingup = 0;

	//        	game->girl.x += speedP;

	//        	if(game->srcRect.y == 0)
	//     		game->srcRect.y += 2*game->girl.h;

	//     	if(game->srcRect.y != 2*game->girl.h)
	//     		game->srcRect.y = 2*game->girl.h;

 //            if (game->srcRect.x == 7*game->girl.w)
 //            {
	//             game->srcRect.x = 0;
 //            }

 //            else 
 //            {
 //            	game->srcRect.x += game->girl.w;
 //            }
	//     }

	//     if(state[SDL_SCANCODE_SPACE])
	//    	{
	//    		time++;

	//    		if (time % 20 == 0)
	// 		{
	// 			game->premio.aparece = 1;
	// 		}

	//    		if (game->girl.facingright == 1)
	//    		{
	// 	   		if (game->srcRect.x != 420 && game->srcRect.y != 270)
	// 	   		{
	// 	   			game->srcRect.x = 420;
	// 	   			game->srcRect.y = 270;
	// 	   		}

	// 	   		if (time % 6 == 0)
	// 	   		{
	// 	   			addBullet(game->girl.x+60, game->girl.y+45, 6);
	// 	   		}

	//    		}

	//    		else if (game->girl.facingleft == 1)
	//    		{
	//    			if (game->srcRect.x != 180 && game->srcRect.y != 270)
	// 	   		{
	// 	   			game->srcRect.x = 180;
	// 	   			game->srcRect.y = 270;
	// 	   		}

	// 	   		if (time % 6 == 0)
	// 	   		{
	// 	   			addBullet(game->girl.x-5, game->girl.y+45, -6);
	// 	   		}

	//    		}

	//    		game->girl.shooting = 1;

	//    	}

	//    	else
	//    	{
	//    		game->girl.shooting = 0;
	//    	}

	//     //Movimento do inimigo
	//    	for(i=0; i<4; i++)
	//    	{

	// 	    if(game->enemies[i].x < game->girl.x)
	// 	    {
	// 	    	for(j=0.5; j <= speedE; j++)
	// 	    	{
	// 	    		game->enemies[i].x += j;
	// 	    		if(game->enemies[i].x == game->girl.x)
	// 	    		{
	// 	    			speedE = 1;
	// 	    			//game->girl.alive=0;
	// 	    		}
		    		
	// 	    	}
	// 	    }
	// 	    if(game->enemies[i].x > game->girl.x)
	// 	    {
	// 	    	for(j=0.5; j <= speedE; j++)
	// 	    	{
	// 	    		game->enemies[i].x -= j;
	// 	    		if(game->enemies[i].x == game->girl.x)
	// 	    		{
	// 	    			speedE = 1;
	// 	    			//game->girl.alive=0;
	// 	    		}
		    		
	// 	    	}
	// 	    }
	// 	    if(game->enemies[i].y < game->girl.y)
	// 	    {
	// 	    	for(j=0.5; j <= speedE; j++)
	// 	    	{
	// 	    		game->enemies[i].y += j;
	// 	    		if(game->enemies[i].y == game->girl.y)
	// 	    		{
	// 	    			speedE = 1;
	// 	    			//game->girl.alive=0;
	// 	    		}
		    		
	// 	    	}
	// 	    }
	// 	    if(game->enemies[i].y > game->girl.y)
	// 	    {
	// 	    	for(j=0.5; j <= speedE; j++)
	// 	    	{
	// 	    		game->enemies[i].y -= j;
	// 	    		if(game->enemies[i].y == game->girl.y)
	// 	    		{
	// 	    			speedE = 1;
	// 	    			//game->girl.alive=0;
	// 	    		}
		    		
	// 	    	}
	// 	    }
	// 		if(game->enemies[i].x < game->girl.x && game->enemies[i].y < game->girl.y)
	// 		{
	// 			for(j=0.5; j <= speedE; j++)
	// 			{
	// 				game->enemies[i].x += j;
	// 				game->enemies[i].y += j;
	// 				if(game->enemies[i].x == game->girl.x || game->enemies[i].y == game->girl.y)
	// 				{
	// 	    			speedE = 1;
	// 	    			//game->girl.alive=0;
	// 	    		}
		    		
	// 			}
	// 		}
	// 		if (game->enemies[i].x > game->girl.x && game->enemies[i].y > game->girl.y)
	// 		{
	// 			for(j=0.5; j <= speedE; j++)
	// 			{
	// 				game->enemies[i].x -= j;
	// 				game->enemies[i].y -= j;
	// 				if(game->enemies[i].x == game->girl.x || game->enemies[i].y == game->girl.y)
	// 				{
	// 	    			speedE = 1;
	// 	    			//game->girl.alive=0;
	// 	    		}
		    		
	// 			}
	// 		}
	// 		if(game->enemies[i].y < game->girl.y && game->enemies[i].x > game->girl.x)
	// 		{
	// 			for(j=0.5; j <= speedE; j++)
	// 			{
	// 				game->enemies[i].y += j;
	// 	 			game->enemies[i].x -= j;
	// 	 			if(game->enemies[i].x == game->girl.x || game->enemies[i].y == game->girl.y)
	// 	 			{
	// 	    			speedE = 1;
	// 	    			//game->girl.alive=0;
	// 	    		}
		    		
	// 	 		}
	// 	 	}
	// 	 	if(game->enemies[i].y > game->girl.y && game->enemies[i].x < game->girl.x)
	// 		{
	// 			for(j=0.5; j <= speedE; j++)
	// 			{
	// 				game->enemies[i].y -= j;
	// 				game->enemies[i].x += j;
	// 				if(game->enemies[i].x == game->girl.x || game->enemies[i].y == game->girl.y)
	// 	 			{
	// 	    			speedE = 1;
	// 	    			//game->girl.alive=0;
	// 	    		}
		    		
	// 			}
	// 		}

	// 	    // colisao girl vs enemy
	// 	    // if (game->enemies[i].x+30 <= game->girl.x+60 && game->enemies[i].x+30 >= game->girl.x)
	// 	    // { 

	// 	    // 	if(game->enemies[i].y+10 <= game->girl.y+90 && game->enemies[i].y+10 >= game->girl.y)
	// 	   	//   	{
	// 	   	//   			game->girl.alive = 0;
	// 	   	//   	}
	// 	    // }

	// 	}

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
		if (game->enemies[0].alive == 1)
		{
			SDL_Rect enemyRect0 = {game->enemies[0].x, game->enemies[0].y, game->enemies[0].w, game->enemies[0].h};
			SDL_RenderCopy(game->renderer, game->enemyImg, &game->esrcRect, &enemyRect0);
		}
		// else
		// {
		// 	SDL_Rect enemyRect0 = {0, 0, 0, 0};
		// 	SDL_RenderCopy(game->renderer, game->enemyImg, NULL, &enemyRect0);
		// }
	
		if (game->enemies[1].alive == 1)
		{
			SDL_Rect enemyRect1 = {game->enemies[1].x, game->enemies[1].y, game->enemies[1].w, game->enemies[1].h};
			SDL_RenderCopy(game->renderer, game->enemyImg, &game->esrcRect, &enemyRect1);
		}
		// else
		// {
		// 	SDL_Rect enemyRect1 = {0, 0, 0, 0};
		// 	SDL_RenderCopy(game->renderer, game->enemyImg, NULL, &enemyRect1);
		// }

		if (game->enemies[2].alive == 1)
		{
			SDL_Rect enemyRect2 = {game->enemies[2].x, game->enemies[2].y, game->enemies[2].w, game->enemies[2].h};
			SDL_RenderCopy(game->renderer, game->enemyImg, &game->esrcRect, &enemyRect2);
		}
		// else
		// {

		// 	SDL_Rect enemyRect2 = {0, 0, 0, 0};
		// 	SDL_RenderCopy(game->renderer, game->enemyImg, NULL, &enemyRect2);	
		//	}

		if (game->enemies[3].alive == 1)
		{
			SDL_Rect enemyRect3 = {game->enemies[3].x, game->enemies[3].y, game->enemies[3].w, game->enemies[3].h};
			SDL_RenderCopy(game->renderer, game->enemyImg, &game->esrcRect, &enemyRect3);
		}
		// else
		// {

		// 	SDL_Rect enemyRect3 = {0, 0, 0, 0};
		// 	SDL_RenderCopy(game->renderer, game->enemyImg, NULL, &enemyRect3);
		// }


		//Desenhando o bullet
		//if (explode == 0)
		//{
			for(i = 0; i < MAX_BULLETS; i++) if(bullets[i])
			{
				SDL_Rect rect = {bullets[i]->x, bullets[i]->y, 16, 16};
				SDL_RenderCopy(game->renderer, game->bulletTexture, NULL, &rect);
			}
		//}

			if (game->premio.aparece == 1)
			{
				SDL_Rect rect = {game->premio.x, game->premio.y, 36, 29};
				SDL_RenderCopy(game->renderer, game->premioImg, NULL, &rect);
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

void collisionEnemy(SDL_Window *window, GameState *game)
{
    int wallWidth = 62;
    int i;

    for(i=0; i<4; i++)
    {
	    if (game->enemies[i].x < wallWidth ) 
	    {
	        game->enemies[i].x = wallWidth;
	    }
	    else if (game->enemies[i].x > (SCREEN_WIDTH-wallWidth)-game->enemies[i].w) 
	    {
	        game->enemies[i].x = (SCREEN_WIDTH-wallWidth)-game->enemies[i].w;
	    }
	    if (game->enemies[i].y < wallWidth) 
	    {
	        game->enemies[i].y = wallWidth;
	    }
	    else if (game->enemies[i].y > (SCREEN_HEIGHT-wallWidth)-game->enemies[i].h) 
	    {
	        game->enemies[i].y = (SCREEN_HEIGHT-wallWidth)-game->enemies[i].h;
	    }
	}
}

void bulletLogic (GameState *game) 
{
	int i, j, wallWidth = 62;

	for(i = 0; i < MAX_BULLETS; i++) if(bullets[i])
  	{
		bullets[i]->x += bullets[i]->vel;
	   	
	   	for (j = 0; j < 4; j++)
		{

	   		//colisão bala vs inimigo
			if(bullets[i]->x >= game->enemies[j].x && bullets[i]->x <= game->enemies[j].x+57 
				&& bullets[i]->y >= game->enemies[j].y && bullets[i]->y <= game->enemies[j].y+20)
			{
				// printf("acertou morcego %d\n", j);

	   			game->enemies[j].alive=0;
	   			qtdeEnemies--;
		   	}

	  	}

   		if(bullets[i]->x <= -10 || bullets[i]->x >= 900)
	      		removeBullet(i);
	}

	//colisão bala vs parede
	// for(i = 0; i < MAX_BULLETS; i++) if(bullets[i])
	// {
	// 	if (bullets[i]->x < wallWidth ) 
	//     {
	//         bullets[i]->x = wallWidth;
	//         //explode = 1;
	//     }
	//     else if (bullets[i]->x > (SCREEN_WIDTH-wallWidth)-16) 
	//     {
	//         bullets[i]->x = (SCREEN_WIDTH-wallWidth)-16;
	//         //explode = 1;
	//     }
	//     if (bullets[i]->y < wallWidth) 
	//     {
	//         bullets[i]->y = wallWidth;
	//         //explode = 1;
	//     }
	//     else if (bullets[i]->y  > (SCREEN_HEIGHT-wallWidth)-16) 
	//     {
	//         bullets[i]->y  = (SCREEN_HEIGHT-wallWidth)-16;
	//         //explode = 1;
	//     }
	// }

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
	SDL_DestroyTexture(game->premioImg);
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

	gameState.premio.aparece = 0;

	// gameState.girl.life = 2;

	gameState.girl.x = 500;
	gameState.girl.y = 300;
	gameState.girl.w = 60;
	gameState.girl.h = 90;

	gameState.premio.x = 200;
	gameState.premio.y = 100;
	gameState.premio.w = 36;
	gameState.premio.h = 29;

	gameState.enemies[0].x = 938;
	gameState.enemies[0].y = 300;
	gameState.enemies[0].w = 53;
	gameState.enemies[0].h = 20;

	gameState.enemies[1].x = 500;
	gameState.enemies[1].y = 62;
	gameState.enemies[1].w = 53;
	gameState.enemies[1].h = 20;

	gameState.enemies[2].x = 62;
	gameState.enemies[2].y = 300;
	gameState.enemies[2].w = 53;
	gameState.enemies[2].h = 20;

	gameState.enemies[3].x = 500;
	gameState.enemies[3].y = 538;
	gameState.enemies[3].w = 53;
	gameState.enemies[3].h = 20;

	int i;

	for (i = 0; i < 4; i++)
	{
		gameState.enemies[i].alive = 1;
	}
 
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
		//Mix_PlayChannel( 0, gameState.mus_menu, 0 );
		
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

			premioLogic(&gameState);

			collisionPlayer(window, &gameState);

			collisionEnemy(window, &gameState);

			//SDL_Delay(10);
		}

		quitGame(window, &gameState);

	}	

	return 0;
}