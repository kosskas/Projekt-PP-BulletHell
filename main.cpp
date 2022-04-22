#define _CRT_SECURE_NO_WARNINGS
#define _USE_MATH_DEFINES
#include<math.h>
#include<stdio.h>
#include<time.h>
#include<string.h>
#include <cstdlib>
extern "C" {
#include"./SDL2-2.0.10/include/SDL.h"
#include"./SDL2-2.0.10/include/SDL_main.h"
}

#define SCREEN_WIDTH 660
#define SCREEN_HEIGHT 500
#define MAP_WIDTH 3000
#define MAP_HEIGHT 3000

#define SPEED 6000
#define B_SPEED 1000
#define MAX_N_BULLETS 40
#define MAX_N_EMENYS 4
#define ENEMIES_TO_DESTROY MAX_N_EMENYS - 1

#define PLAYER_SIZE 45
#define ENEMY_SIZE 90
#define BULLET_SIZE 30
#define PLAYER_BULLET_SIZE 30

#define DIRECTIONS 4
#define BULLET_FRAMES 4
#define ENEMY_FRAMES 5

#define LEVELS 3
#define NR_OF_INFOS 3

#define PLAYER_DMG 10
#define ENEMY_DMG 8
#define BULLET_DMG 6
#define HEART_BONUS -10

#define ENEMY_SCORE 100
#define BULLET_SCORE 5
#define HEART_SCORE -50
#define HIT_SCORE 10

#define PLAYER_HP 100
#define ENEMY_HP 150

///Player animations
const char* player_N_path = "./assets/player/planeN.bmp";
const char* player_S_path = "./assets/player/planeS.bmp";
const char* player_W_path = "./assets/player/planeW.bmp";
const char* player_E_path = "./assets/player/planeE.bmp";
const char* player_bullet_s_path = "./assets/player/bullet.bmp";

///Enemy animations
const char* enemy_0_path = "./assets/enemy/enemy0.bmp";
const char* enemy_1_path = "./assets/enemy/enemy1.bmp";
const char* enemy_2_path = "./assets/enemy/enemy2.bmp";
const char* enemy_3_path = "./assets/enemy/enemy3.bmp";
const char* enemy_4_path = "./assets/enemy/enemy4.bmp";
const char* enemy_dead_path = "./assets/enemy/enemy_dead.bmp";

///Bullet animations
const char* bullet_0_path = "./assets/enemy/fireball0.bmp";
const char* bullet_1_path = "./assets/enemy/fireball1.bmp";
const char* bullet_2_path = "./assets/enemy/fireball2.bmp";
const char* bullet_3_path = "./assets/enemy/fireball3.bmp";

///Meta animations
const char* background_path = "./assets/tlo2.bmp";
const char* charset_path = "./assets/cs8x8.bmp";
const char* menu_path = "./assets/menu.bmp";
const char* loss_screen_path = "./assets/loss.bmp";
const char* win_screen_path = "./assets/win.bmp";
const char* heart_s_path = "./assets/heart.bmp";

enum {
	MENU,
	WIN,
	LOSS
};
enum view {
	NORTH,
	EAST,
	SOUTH,
	WEST
};

struct point
{
	double x = 0;
	double y = 0;
};

struct timer {
	int poc;
	int kon, frames;
	double delta, fpsTimer, fps, worldTime;
};

struct pallete {
	int czarny;
	int zielony;
	int czerwony;
	int niebieski;
};

struct bullet
{
	SDL_Surface* sprite[BULLET_FRAMES];
	point pos;
	double speed_y = B_SPEED;
	double speed_x = B_SPEED;
	int v_x = 0;
	int v_y = 0;
	double sum = 0;
	bool active = false;
	int dmg = 6;
	int score = 10;
	int current_frame = 0;
	double frame_time = 0;
};

struct player
{
	SDL_Surface* sprite[DIRECTIONS];
	point pos = { SCREEN_WIDTH / 2 , SCREEN_HEIGHT / 2 };
	double speed_y = 0;
	double speed_x = 0;
	double invincibility_time;
	int destroyed_enemies = 0;
	int HP = PLAYER_HP;
	int dmg = PLAYER_DMG;
	int score = 0;
	int bonustime;
	int multipler = 1;
	view facing = NORTH;
	bullet blaster;
};

struct enemy
{
	SDL_Surface* sprite[ENEMY_FRAMES + 1];
	point pos;
	bullet e_bullet[MAX_N_BULLETS];
	int nr;
	double sum = 0;
	bool alive = true;
	int HP = ENEMY_HP;
	double invincibility_time;
	int dmg = ENEMY_DMG;
	bool allay = false;
	int score = 100;
	int current_frame = 0;
	double frame_time = 0;
};

struct level {
	point enemy_pos[MAX_N_EMENYS];
};

struct engine {
	SDL_Event event;
	SDL_Surface* screen, * charset;
	SDL_Texture* scrtex;
	SDL_Window* window;
	SDL_Renderer* renderer;
	SDL_Surface* background;
	SDL_Surface* menu[NR_OF_INFOS];
	level levels[LEVELS];
	pallete colors;
	timer timer;
	int rc;
	int lvl_nr = 0;
	bool quit = false;
	bool startLvl = false;
	int bg_posX = 0;
	int bg_posY = 0;
};

///Drawing
void DrawString(SDL_Surface* screen, int x, int y, const char* text, SDL_Surface* charset);
void DrawSurface(SDL_Surface* screen, SDL_Surface* sprite, int x, int y);
void DrawPixel(SDL_Surface* surface, int x, int y, Uint32 color);
void DrawLine(SDL_Surface* screen, int x, int y, int l, int dx, int dy, Uint32 color);
void DrawRectangle(SDL_Surface* screen, int x, int y, int l, int k, Uint32 outlineColor, Uint32 fillColor);

///Printing
void UpdateScreen(engine* E);
void PrintScreen(engine* E, player* P, enemy* N);
void PrintPlayer(engine* E, player* P);
void PrintEnemy(engine* E, enemy* N);
void PrintEnemyBullet(engine* E, enemy* N, bullet* B, int b_nr, int znak);
void PrintPlayerBullet(engine* E, player* P);
void PrintMenu(engine* E, player* P, enemy* N);
void PrintInfoBar(engine* E, player* P, enemy* N);

///Settings
void SetColorValue(engine* E, pallete* P);
void SetLevels(engine* E);
void SetLevel(engine* E, int nr);
void ApplyLevelSettings(engine* E, point enemys[MAX_N_EMENYS], int lvl_nr);
void SetPlayer(player* P);
void SetEnemy(enemy* N);
void SetEnemyBullet(enemy* N);

///Conditions
void CheckBorderColision(engine* E, player* P);
void CheckEnemyBulletBorderColision(enemy* N, bullet* B);
void CheckPlayerBulletBorderColision(player* P, bullet* B);
void CheckPlayerHitbox(engine* E, player* P, enemy* N);
void CheckEnemyHitbox(engine* E, player* P, enemy* N);
void CheckScoreMultipler(engine* E, player* P);
bool CheckLvlContition(engine* E, player* P);
bool IsEnd(engine* E, player* P);

///Loading
void PreLoad(engine* E, player* P, enemy* N);
SDL_Surface* LoadSprite(engine* E, const char* path);
void LoadEngineGameTextures(engine* E);
void LoadEnemyAnimation(engine* E, enemy* N);
void LoadPlayerAnimation(engine* E, player* P);
void LoadLevel(engine* E, player* P, enemy* N, int lvl);
void LoadAllTextures(engine* E, player* P, enemy* N);

///Events
void HandlePlayerMovement(engine* E, player* P);
void MenuKeyEvent(engine* E, player* P, enemy* N);
void HandleKeyEvent(engine* E, player* P, enemy* N);
void Shoot(engine* E, player* P);

///Game Engine
void RunGame(engine* E, player* P, enemy* N);
void RunLevel(engine* E, player* P, enemy* N);
void NewGame(engine*E, player* P, enemy* N);
void FreeSDL(engine* E);
timer StartTime(timer* T);
void UpdateTime(timer* T);
double Distance(point A, point B);

// main
#ifdef __cplusplus
extern "C"
#endif
int main(int argc, char **argv) {
	engine E;
	player ship;
	enemy aliens[MAX_N_EMENYS];
	PreLoad(&E, &ship, aliens);
	RunGame(&E, &ship, aliens);
	return 0;
};

///Uruchamia gre zlozona z 3 lvl
void RunGame(engine* E, player* P, enemy* N) {
	while (!E->quit)
	{
		MenuKeyEvent(E, P, N);
		PrintMenu(E, P, N);
		RunLevel(E, P, N);
	}
	FreeSDL(E);
}

///Wczytuje lvl i obsluguje go
void RunLevel(engine* E, player* P, enemy* N) {
	while (CheckLvlContition(E, P))
	{
		LoadLevel(E, P, N, E->lvl_nr);
		while (!IsEnd(E, P)) {
			UpdateTime(&E->timer);
			CheckScoreMultipler(E, P);
			HandlePlayerMovement(E, P);
			PrintScreen(E, P, N);
			HandleKeyEvent(E, P, N);
		}
		E->lvl_nr++;
	}
}

void MenuKeyEvent(engine* E, player* P, enemy* N) {

	while (SDL_PollEvent(&E->event)) {
		switch (E->event.type) {
		case SDL_KEYDOWN:
			if (E->event.key.keysym.sym == SDLK_ESCAPE)
				E->quit = 1;
			else if (E->event.key.keysym.sym == SDLK_1)
			{
				NewGame(E, P, N);
				E->lvl_nr = 0;
				E->startLvl = 1;
			}
			else if (E->event.key.keysym.sym == SDLK_2)
			{
				NewGame(E, P, N);
				E->lvl_nr = 1;
				E->startLvl = 1;
			}
			else if (E->event.key.keysym.sym == SDLK_3)
			{
				NewGame(E, P, N);
				E->lvl_nr = 2;
				E->startLvl = 1;
			}
			else if (E->event.key.keysym.sym == SDLK_n) {
				NewGame(E, P, N);
				E->startLvl = 1;
			}
			break;
		case SDL_QUIT:
			E->quit = 1;
			break;
		};
	};
}

///Zmienia kierunek i speed gracza oraz bulleta w zal. od strzalki
void HandleKeyEvent(engine* E, player* P, enemy* N) {

	while (SDL_PollEvent(&E->event)) {
		switch (E->event.type) {
		case SDL_KEYDOWN:
			if (E->event.key.keysym.sym == SDLK_ESCAPE) {
				E->quit = 1;
				E->startLvl = 0;
			}
			else if (E->event.key.keysym.sym == SDLK_UP) {
				P->speed_y = -SPEED;
				P->facing = NORTH;
			}
			else if (E->event.key.keysym.sym == SDLK_DOWN) {
				P->speed_y = SPEED;
				P->facing = SOUTH;
			}
			else if (E->event.key.keysym.sym == SDLK_LEFT) {
				P->speed_x = -SPEED;
				P->facing = WEST;
			}
			else if (E->event.key.keysym.sym == SDLK_RIGHT) {
				P->speed_x = SPEED;
				P->facing = EAST;
			}
			else if (E->event.key.keysym.sym == SDLK_SPACE)
				Shoot(E, P);
			else if (E->event.key.keysym.sym == SDLK_n)
				E->startLvl = 0;
			break;
		case SDL_QUIT:
			E->quit = 1;
			E->startLvl = 0;
			break;
		}
	}
}

/// Wyswietla menu w zal. od stanu gry i gracza
void PrintMenu(engine* E, player* P, enemy* N)
{
	if (P->HP <= 0)
		DrawSurface(E->screen, E->menu[LOSS], SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2);
	else if (E->lvl_nr >= LEVELS)
	{
		char text[128];
		DrawRectangle(E->screen, 4, 4, SCREEN_WIDTH - 8, 36, E->colors.czerwony, E->colors.niebieski);
		sprintf(text, "Score: %i", P->score);
		DrawString(E->screen, E->screen->w / 2 - strlen(text) * 8 / 2, 10, text, E->charset);
		DrawSurface(E->screen, E->menu[WIN], SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2);

	}
	else
		DrawSurface(E->screen, E->menu[MENU], SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2);
	UpdateScreen(E);
}

// narysowanie napisu txt na powierzchni screen, zaczynajπc od punktu (x, y)
// charset to bitmapa 128x128 zawierajπca znaki
// draw a text txt on surface screen, starting from the point (x, y)
// charset is a 128x128 bitmap containing character images
void DrawString(SDL_Surface *screen, int x, int y, const char *text,
                SDL_Surface *charset) {
	int px, py, c;
	SDL_Rect s, d;
	s.w = 8;
	s.h = 8;
	d.w = 8;
	d.h = 8;
	while(*text) {
		c = *text & 255;
		px = (c % 16) * 8;
		py = (c / 16) * 8;
		s.x = px;
		s.y = py;
		d.x = x;
		d.y = y;
		SDL_BlitSurface(charset, &s, screen, &d);
		x += 8;
		text++;
		};
	};


// narysowanie na ekranie screen powierzchni sprite w punkcie (x, y)
// (x, y) to punkt úrodka obrazka sprite na ekranie
// draw a surface sprite on a surface screen in point (x, y)
// (x, y) is the center of sprite on screen
void DrawSurface(SDL_Surface *screen, SDL_Surface *sprite, int x, int y) {
	SDL_Rect dest;
	dest.x = x - sprite->w / 2;
	dest.y = y - sprite->h / 2;
	dest.w = sprite->w;
	dest.h = sprite->h;
	SDL_BlitSurface(sprite, NULL, screen, &dest);
};

// rysowanie pojedynczego pixela
// draw a single pixel
void DrawPixel(SDL_Surface *surface, int x, int y, Uint32 color) {
	int bpp = surface->format->BytesPerPixel;
	Uint8 *p = (Uint8 *)surface->pixels + y * surface->pitch + x * bpp;
	*(Uint32 *)p = color;
	};


// rysowanie linii o d≥ugoúci l w pionie (gdy dx = 0, dy = 1) 
// bπdü poziomie (gdy dx = 1, dy = 0)
// draw a vertical (when dx = 0, dy = 1) or horizontal (when dx = 1, dy = 0) line
void DrawLine(SDL_Surface *screen, int x, int y, int l, int dx, int dy, Uint32 color) {
	for(int i = 0; i < l; i++) {
		DrawPixel(screen, x, y, color);
		x += dx;
		y += dy;
		};
	};


// rysowanie prostokπta o d≥ugoúci bokÛw l i k
// draw a rectangle of size l by k
void DrawRectangle(SDL_Surface *screen, int x, int y, int l, int k,
                   Uint32 outlineColor, Uint32 fillColor) {
	int i;
	DrawLine(screen, x, y, k, 0, 1, outlineColor);
	DrawLine(screen, x + l - 1, y, k, 0, 1, outlineColor);
	DrawLine(screen, x, y, l, 1, 0, outlineColor);
	DrawLine(screen, x, y + k - 1, l, 1, 0, outlineColor);
	for(i = y + 1; i < y + k - 1; i++)
		DrawLine(screen, x + 1, i, l - 2, 1, 0, fillColor);
}

///Wczytuje .bmp 
SDL_Surface *LoadSprite(engine*E, const char* path) {
	SDL_Surface *sprite;
	sprite = SDL_LoadBMP(path);
	if (sprite == NULL) {
		printf("SDL_LoadBMP(%s) error: %s\n", path, SDL_GetError());
		FreeSDL(E);
	}
	else
		printf("Zaladowano %s \n", path);
	return sprite;
}

// zwolnienie powierzchni / freeing all surfaces
void FreeSDL(engine* E) {
	SDL_FreeSurface(E->charset);
	SDL_FreeSurface(E->screen);
	SDL_DestroyTexture(E->scrtex);
	SDL_DestroyRenderer(E->renderer);
	SDL_DestroyWindow(E->window);
	SDL_Quit();
}

///Inicjalizacja wszystkich rzeczy na potrzeby silnika gry
void PreLoad(engine* E, player*P, enemy*N) {
	if (SDL_Init(SDL_INIT_EVERYTHING) != 0) {
		printf("SDL_Init error: %s\n", SDL_GetError());
		exit(EXIT_FAILURE);
	}
	// tryb pe≥noekranowy / fullscreen mode
//	rc = SDL_CreateWindowAndRenderer(0, 0, SDL_WINDOW_FULLSCREEN_DESKTOP,
//	                                 &window, &renderer);
	E->rc = SDL_CreateWindowAndRenderer(SCREEN_WIDTH, SCREEN_HEIGHT, 0,
		&E->window, &E->renderer);
	if (E->rc != 0) {
		SDL_Quit();
		printf("SDL_CreateWindowAndRenderer error: %s\n", SDL_GetError());
		exit(EXIT_FAILURE);
	};

	SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "linear");
	SDL_RenderSetLogicalSize(E->renderer, SCREEN_WIDTH, SCREEN_HEIGHT);
	SDL_SetRenderDrawColor(E->renderer, 0, 0, 0, 255);
	SDL_SetWindowTitle(E->window, "Bullet Hell");

	E->screen = SDL_CreateRGBSurface(0, SCREEN_WIDTH, SCREEN_HEIGHT, 32,
		0x00FF0000, 0x0000FF00, 0x000000FF, 0xFF000000);

	E->scrtex = SDL_CreateTexture(E->renderer, SDL_PIXELFORMAT_ARGB8888,
		SDL_TEXTUREACCESS_STREAMING,
		SCREEN_WIDTH, SCREEN_HEIGHT);

	// wy≥πczenie widocznoúci kursora myszy
	SDL_ShowCursor(SDL_DISABLE);

	LoadAllTextures(E, P, N);
	SDL_SetColorKey(E->charset, true, 0x000000);

	SetColorValue(E, &E->colors);

	for (int i = 0; i < MAX_N_EMENYS; i++)
		N[i].nr = i;
	SetLevels(E);
}

void SetColorValue(engine *E, pallete* P) {
	P->czarny = SDL_MapRGB(E->screen->format, 0x00, 0x00, 0x00);
	P->zielony = SDL_MapRGB(E->screen->format, 0x00, 0xFF, 0x00);
	P->czerwony = SDL_MapRGB(E->screen->format, 0xFF, 0x00, 0x00);
	P->niebieski = SDL_MapRGB(E->screen->format, 0x11, 0x11, 0xCC);
}

// tekst informacyjny / info text
void PrintInfoBar(engine* E, player *P, enemy*N) {
	char text[128];
	DrawRectangle(E->screen, 4, 4, SCREEN_WIDTH - 8, 36, E->colors.czerwony, E->colors.niebieski);
	sprintf(text, "Esc - exit, n - new game, movement - \030\031\032\033, shoot - SPACE");
	DrawString(E->screen, E->screen->w / 2 - strlen(text) * 8 / 2, 10, text, E->charset);
	sprintf(text,"Time: %.0lfs, HP: %i%%, Score: %i x%i",E->timer.worldTime, P->HP, P->score, P->multipler);
	DrawString(E->screen, E->screen->w / 2 - strlen(text) * 8 / 2, 26, text, E->charset);
}

///Sprawdza warunek pojednyczego lvla
bool IsEnd(engine* E, player* P){
	if (!E->startLvl || P->HP < 0 || P->destroyed_enemies >= ENEMIES_TO_DESTROY)
		return  true;
	else
		return false;
}

/// Sprawdza warunek calej gry (3lvli)
bool CheckLvlContition(engine*E, player*P) {
	if (E->startLvl && P->HP > 0 && E->lvl_nr < LEVELS)
		return true;
	return false;
}

///Sprawdza zerowanie mnoznika
void CheckScoreMultipler(engine* E, player* P) {
	if (E->timer.worldTime - P->bonustime >= 1.5)
	{
		P->bonustime = E->timer.worldTime;
		if (!(--P->multipler))
			P->multipler = 1;

	}
}

///Trzyma gracza w granicach mapy
void CheckBorderColision(engine* E, player* P) {
	if (P->pos.x >= MAP_WIDTH / 2)
		P->pos.x = MAP_WIDTH / 2;
	if(P->pos.x <= -MAP_WIDTH / 2)
		P->pos.x = -MAP_WIDTH / 2;

	if (P->pos.y <= -MAP_HEIGHT / 2)
		P->pos.y = -MAP_HEIGHT / 2;
	if(P->pos.y >= MAP_HEIGHT / 2)
		P->pos.y = MAP_HEIGHT / 2;
}

void HandlePlayerMovement(engine* E, player* P) {
	
	P->pos.x -= P->speed_x * E->timer.delta;
	P->pos.y -= P->speed_y * E->timer.delta;
	CheckBorderColision(E, P);
	P->speed_y = 0;
	P->speed_x = 0;
}

///Wyswietla wszystko w zal. od pozycji gracza
void PrintScreen(engine* E, player* P, enemy* N) {
	E->bg_posX = SCREEN_WIDTH / 2 + P->pos.x;
	E->bg_posY = SCREEN_HEIGHT / 2 + P->pos.y;
	DrawSurface(E->screen, E->background, E->bg_posX, E->bg_posY);
	PrintEnemy(E, N);
	CheckEnemyHitbox(E, P, N);
	PrintPlayer(E, P);
	CheckPlayerHitbox(E, P, N);
	PrintInfoBar(E, P, N);
	UpdateScreen(E);
}

void UpdateScreen(engine* E) {
	SDL_UpdateTexture(E->scrtex, NULL, E->screen->pixels, E->screen->pitch);
	//		SDL_RenderClear(renderer);
	SDL_RenderCopy(E->renderer, E->scrtex, NULL, NULL);
	SDL_RenderPresent(E->renderer);
}

void PrintPlayer(engine* E, player* P) {
	DrawSurface(E->screen, P->sprite[P->facing], SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2);
	PrintPlayerBullet(E, P);
}

void PrintPlayerBullet(engine* E, player* P) {
	CheckPlayerBulletBorderColision(P, &P->blaster);
	if (P->blaster.active) {
		P->blaster.pos.x += (P->blaster.speed_x*  E->timer.delta * P->blaster.v_x);
		P->blaster.pos.y += (P->blaster.speed_y * E->timer.delta * P->blaster.v_y);
		DrawSurface(E->screen, P->blaster.sprite[P->blaster.current_frame], E->bg_posX - P->blaster.pos.x, E->bg_posY - P->blaster.pos.y);
	}
}

void Shoot(engine* E, player* P) {
	P->blaster.active = true;
	P->blaster.pos = P->pos;
	switch (P->facing)
	{
	case NORTH:
		P->blaster.v_x = 0;
		P->blaster.v_y = 1;
		break;
	case SOUTH:
		P->blaster.v_x = 0;
		P->blaster.v_y = -1;
		break;
	case WEST:
		P->blaster.v_x = 1;
		P->blaster.v_y = 0;
		break;
	case EAST:
		P->blaster.v_x = -1;
		P->blaster.v_y = 0;
		break;
	default:
		break;
	}
}

void CheckEnemyHitbox(engine* E, player* P, enemy* N) {
	if (E->timer.worldTime - N->invincibility_time >= 0.1)
	{
		for (int j = 0; j < MAX_N_EMENYS; j++)
		{
			if (N[j].alive && P->blaster.active)
			{
				if (Distance(N[j].pos, P->blaster.pos) < (PLAYER_BULLET_SIZE + BULLET_SIZE))
				{
					N[j].HP -= P->dmg;
					if (E->timer.worldTime - P->bonustime >= 0.3)
					{
						P->bonustime = E->timer.worldTime;
						P->multipler++;
					}
					P->score += P->multipler* HIT_SCORE;

					P->blaster.active = false;
					if (N[j].HP <= 0)
					{
						P->destroyed_enemies++;
						P->score += N[j].score;
						N[j].alive = false;
					}
					N->invincibility_time = E->timer.worldTime;
					return;
				}
			}
		}
	}
}

void CheckPlayerHitbox(engine* E, player* P, enemy* N) {
	if (E->timer.worldTime - P->invincibility_time >=1)
	{
		for (int i = 0; i < MAX_N_EMENYS; i++)
		{
			if (N[i].alive && !N[i].allay)
			{
				if (Distance(P->pos, N[i].pos) < 65)
				{
					P->HP -= N[i].dmg;
					P->score -= N[i].score;
					P->invincibility_time = E->timer.worldTime;
					P->multipler = 1;
					return;
				}
			}
			for (int j = 0; j < MAX_N_BULLETS; j++)
			{
				if (N[i].e_bullet[j].active && N[i].alive )
				{
					if (Distance(P->pos, N[i].e_bullet[j].pos) <  45)
					{
						P->score -= N[i].e_bullet[j].score;
						N[i].e_bullet[j].active = false;
						P->HP -= N[i].e_bullet[j].dmg;
						P->multipler = 1;

						P->invincibility_time = E->timer.worldTime;
						return;
					}
				}
			}
		}
	}
}

void PrintEnemy(engine* E, enemy* N) {
	
	for (int j = 0; j < MAX_N_EMENYS; j++) {
		if (N[j].alive)
		{
			if (j == 1)
			{
				N[j].sum += E->timer.delta;
				N[j].pos.x = E->levels[E->lvl_nr].enemy_pos[j].x + cos(N[j].sum) * E->levels[E->lvl_nr].enemy_pos[j].y / 3;
				N[j].pos.y = E->levels[E->lvl_nr].enemy_pos[j].y + sin(N[j].sum) * E->levels[E->lvl_nr].enemy_pos[j].y / 3;
			}
			if (!N[j].allay && (E->timer.worldTime - N[j].frame_time >= 0.1))
			{
				N[j].frame_time = E->timer.worldTime;
				N[j].current_frame = (N[j].current_frame + 1) % ENEMY_FRAMES;
			}
			DrawSurface(E->screen, N[j].sprite[N[j].current_frame], E->bg_posX - N[j].pos.x, E->bg_posY - N[j].pos.y);
			for (int i = 0; i < MAX_N_BULLETS; i++)
			{
				PrintEnemyBullet(E, &N[j], &N[j].e_bullet[i], N[j].nr, i + 1);
			}
		}
		else
			DrawSurface(E->screen, N[j].sprite[5], E->bg_posX - N[j].pos.x, E->bg_posY - N[j].pos.y);
	}	
}

void PrintEnemyBullet(engine* E, enemy* N, bullet* B, int b_nr, int znak){
	CheckEnemyBulletBorderColision(N, B);
	if (B->active) {
		B->sum += E->timer.delta;		
		switch (b_nr)
		{
		case 0:
			B->pos.x = N->pos.x  +(N->pos.x * B->sum) * cos(znak + B->sum) /znak;
			B->pos.y = N->pos.y + (N->pos.y * B->sum) * sin(znak + B->sum) / znak;
			break;
		case 1:
			B->pos.x = N->pos.x + cos(B->sum + znak) * N->pos.y  /2;
			B->pos.y = N->pos.y + sin(B->sum + znak) * N->pos.y / 2;
			break;
		case 2:
			B->pos.x = N->pos.x + (znak * B->sum) * cos(B->sum  + znak);
			B->pos.y = N->pos.y + (znak * B->sum) * sin(B->sum + znak);
			break;
		default:
			break;
		}
		if (!N->allay && (E->timer.worldTime - B->frame_time >= 0.1))
		{
			B->frame_time = E->timer.worldTime;
			B->current_frame = (B->current_frame + 1 ) % BULLET_FRAMES;
		}
		DrawSurface(E->screen, B->sprite[B->current_frame], E->bg_posX - B->pos.x, E->bg_posY - B->pos.y);
	}
}

void CheckEnemyBulletBorderColision(enemy*N, bullet *B) {
	if (B->pos.x >= (double)(MAP_WIDTH / 2) ||  B->pos.x <= -(double)(MAP_WIDTH / 2))
		B->sum = 0;	
	if( B->pos.y >= (double)(MAP_HEIGHT / 2) ||B->pos.y <= -(double)(MAP_HEIGHT / 2))
		B->sum = 0;
	if (Distance(B->pos, N->pos) > 729)
		B->sum = 0;
}

void CheckPlayerBulletBorderColision(player* P, bullet* B) {
	if (B->pos.x >= (double)(MAP_WIDTH / 2) || B->pos.x <= -(double)(MAP_WIDTH / 2))
	{
		B->active = false;
		B->pos = P->pos;
	}
	if (B->pos.y >= (double)(MAP_HEIGHT / 2) || B->pos.y <= -(double)(MAP_HEIGHT / 2))
	{
		B->active = false;
		B->pos = P->pos;
	}
}

timer StartTime(timer* T) {
	timer nowy;
	nowy.fps = 0;
	nowy.fpsTimer = 0;
	nowy.frames = 0;
	nowy.poc = SDL_GetTicks();
	nowy.worldTime = 0;
	return nowy;
}

double Distance(point A, point B) {
	double dx = A.x - B.x;
	double dy = A.y - B.y;
	return sqrt(dx * dx + dy * dy);
}

void UpdateTime(timer* T) {
	T->kon = SDL_GetTicks();
	T->delta = (T->kon - T->poc) * 0.001;
	T->poc = T->kon;
	T->worldTime += T->delta;
	T->fpsTimer += T->delta;
	if (T->fpsTimer > 0.5) {
		T->fps = T->frames * 2;
		T->frames = 0;
		T->fpsTimer -= 0.5;
	}
	T->frames++;
}

///Resetuje wszystkie pozycje i wartosci i wczytuje 1.lvl
void NewGame(engine* E, player* P, enemy* N) {
	E->lvl_nr = 0;
	srand(time(NULL));
	for (int j = 0; j < MAX_N_EMENYS ; j++) {
		N[j].pos = E->levels[E->lvl_nr].enemy_pos[j];
		SetEnemy(&N[j]);
		N[j].invincibility_time = 0;
		if (j == MAX_N_EMENYS - 1)
			N[j].allay = true;
		SetEnemyBullet(&N[j]);
	}
	SetPlayer(P);
	P->score = 0;
	E->timer = StartTime(&E->timer);
}

///Wczytuje pozycje wrogow w zal. od lvla
void LoadLevel(engine* E, player* P, enemy* N, int lvl) {
	srand(time(NULL));
	for (int j = 0; j < MAX_N_EMENYS; j++) {
		N[j].pos = E->levels[lvl].enemy_pos[j];
		SetEnemy(&N[j]);
		N[j].invincibility_time = 0;
		if (j == MAX_N_EMENYS - 1)
			N[j].allay = true;
		SetEnemyBullet(&N[j]);
	}
	SetPlayer(P);
}

void SetEnemyBullet(enemy* N) {
	for (int i = 0; i < MAX_N_BULLETS; i++)
	{
		if (N->allay)
		{
			N->e_bullet[i].pos.x = rand() % MAP_WIDTH - (MAP_WIDTH / 2);
			N->e_bullet[i].pos.y = rand() % MAP_HEIGHT - (MAP_HEIGHT / 2);
			N->e_bullet[i].dmg = HEART_BONUS;
			N->e_bullet[i].score = HEART_SCORE;
			N->e_bullet[i].active = true;
		}
		else {
			N->e_bullet[i].sum = 0;
			N->e_bullet[i].dmg = BULLET_DMG;
			N->e_bullet[i].score = BULLET_SCORE;
			N->e_bullet[i].pos = N->pos;
			N->e_bullet[i].active = true;
		}
	}
}

void SetEnemy(enemy* N) {
	N->sum = 0;
	N->HP = ENEMY_HP;
	N->dmg = ENEMY_DMG;
	N->score = ENEMY_SCORE;
	N->alive = true;
}

void SetPlayer(player* P) {
	P->pos.x = SCREEN_WIDTH / 2;
	P->pos.y = SCREEN_HEIGHT / 2;
	P->HP = PLAYER_HP;
	P->blaster.active = false;
	P->destroyed_enemies = 0;
	P->multipler = 1;
	P->bonustime = 0;
	P->blaster.pos = P->pos;
}

///Wczytuje animacje wroga i bulleta
void LoadEnemyAnimation(engine* E, enemy* N) {

	SDL_Surface* ememy0 = LoadSprite(E, enemy_0_path);
	SDL_Surface* ememy1 = LoadSprite(E, enemy_1_path);
	SDL_Surface* ememy2 = LoadSprite(E, enemy_2_path);
	SDL_Surface* ememy3 = LoadSprite(E, enemy_3_path);
	SDL_Surface* ememy4 = LoadSprite(E, enemy_4_path);
	SDL_Surface* dead = LoadSprite(E, enemy_dead_path);
	SDL_Surface* heart = LoadSprite(E, heart_s_path);

	SDL_Surface* bullet0 = LoadSprite(E, bullet_0_path);
	SDL_Surface* bullet1 = LoadSprite(E, bullet_1_path);
	SDL_Surface* bullet2 = LoadSprite(E, bullet_2_path);
	SDL_Surface* bullet3 = LoadSprite(E, bullet_3_path);

	for (int j = 0; j < MAX_N_EMENYS; j++) {
		N[j].sprite[0] = ememy0;
		N[j].sprite[1] = ememy1;
		N[j].sprite[2] = ememy2;
		N[j].sprite[3] = ememy3;
		N[j].sprite[4] = ememy4;
		N[j].sprite[5] = dead;
		for (int i = 0; i < MAX_N_BULLETS; i++)
		{

			N[j].e_bullet[i].sprite[0] = bullet0;
			N[j].e_bullet[i].sprite[1] = bullet1;
			N[j].e_bullet[i].sprite[2] = bullet2;
			N[j].e_bullet[i].sprite[3] = bullet3;
			if (j == MAX_N_EMENYS - 1)
				N[j].e_bullet[i].sprite[0] = heart;
		}
	}
}

///Wczytuje animalnie gracza
void LoadPlayerAnimation(engine* E, player* P) {
	P->sprite[NORTH] = LoadSprite(E, player_N_path);
	P->sprite[SOUTH] = LoadSprite(E, player_S_path);
	P->sprite[WEST] = LoadSprite(E, player_W_path);
	P->sprite[EAST] = LoadSprite(E, player_E_path);
	P->blaster.sprite[0] = LoadSprite(E, player_bullet_s_path);
}

///Wczytuje wszystkie struktury
void LoadAllTextures(engine* E, player* P, enemy* N) {
	LoadEngineGameTextures(E);
	LoadEnemyAnimation(E, N);
	LoadPlayerAnimation(E, P);
}

void LoadEngineGameTextures(engine* E) {
	E->charset = LoadSprite(E, charset_path);
	E->background = LoadSprite(E, background_path);
	E->menu[MENU] = LoadSprite(E, menu_path);
	E->menu[LOSS] = LoadSprite(E, loss_screen_path);
	E->menu[WIN] = LoadSprite(E, win_screen_path);
}

void SetLevels(engine* E) {
	for (int i = 0; i < LEVELS; i++)
		SetLevel(E, i);
}

///Wczytuje pozycje przeciwnikow w zal. od lvla
void SetLevel(engine* E, int nr) {
	
	switch (nr)
	{
	case 0:
	{
		point wrog[MAX_N_EMENYS] = {
		{250, 250},
		{-125, -615},
		{-625, 225},
		{-MAP_WIDTH / 2, -MAP_HEIGHT / 2 }
		};
		ApplyLevelSettings(E, wrog, 0);
		break;
	}
	case 1:
	{
		point wrog[MAX_N_EMENYS] = {
		{100, 100},
		{-500, 200},
		{-100, -100},
		{-MAP_WIDTH / 2, -MAP_HEIGHT / 2 }
		};
		ApplyLevelSettings(E, wrog, 1);
		break;
	}
	case 2:
	{
		point wrog[MAX_N_EMENYS] = {
		{500, 500},
		{600, -200},
		{-300, -300},
		{-MAP_WIDTH / 2, -MAP_HEIGHT / 2 }
		};
		ApplyLevelSettings(E, wrog, 2);
		break;
	}
	default:
		break;
	}
}

void ApplyLevelSettings(engine* E, point enemys[MAX_N_EMENYS], int lvl_nr) {
	for (int i = 0; i < MAX_N_EMENYS; i++)
		E->levels[lvl_nr].enemy_pos[i] = enemys[i];
}