#pragma once

//=============================================================
//Include Files
//=============================================================
#include "windows.h"
#include "resource.h"
#include "GameEngine.h"
#include "Bitmap.h"
#include "Sprite.h"
#include "Player.h"
#include "Enemy.h"
#include "Bullet.h"
#include <ctime>
#include "Environment.h"
//=============================================================
//Global Variables
//=============================================================
HINSTANCE	_hInstance;
GameEngine* _pGame;
HDC			_hOffscreenDC;
HBITMAP		_hOffscreenBitmap;
RECT		rcBounds;
Bitmap* _pBackgroundBitmap;
Bitmap* _baseBitmap;
Bitmap* _emptyBitmap;
Bitmap* _playerBitmapUp;
Bitmap* _playerBitmapDown;
Bitmap* _playerBitmapLeft;
Bitmap* _playerBitmapRight;
Bitmap* _woodBitmap;
Bitmap* _steelBitmap;
Bitmap* _enemyBitmapUp;
Bitmap* _enemyBitmapRight;
Bitmap* _enemyBitmapDown;
Bitmap* _enemyBitmapLeft;
Bitmap* _enemy2BitmapUp;
Bitmap* _enemy2BitmapRight;
Bitmap* _enemy2BitmapDown;
Bitmap* _enemy2BitmapLeft;
Bitmap* _bulletBitmap;
Bitmap* _borderBitmap;
Bitmap* _pSmExplosionBitmap;
Bitmap* _pLgExplosionBitmap;
Bitmap* _flagBitmap;
Bitmap* _gameOverBitmap;
Player* _player;
Wood* _wood;
Steel* _steel;
Enemy* _enemy;
int			_soundtrackNumber = 0;
int			_enemyIDs;
int			_iInputDelay;
int			_FireDelay;
int			_cellWidth;
int			_cellHeight;
int			_enemyperLevel;
int			_currentEnemyNumber;
int			_timer;
int			_flagCounter;
double		_bfrm;
int         _gameRound = 1;
int			_lastSpawnedFlagX;
int			_lastSpawnedFlagY;
int			_flagIncrease;
vector<Enemy*>		_enemies;
std::time_t	timeStart;

BOOL		_bGameOver;
// Add Offset for HUD and Exterior Walls

char		_gameMatrix[MATRIX_WIDTH][MATRIX_HEIGHT];
char		_symbols[6]; // base = b, p = player, e = enemy, w = wood, s = steel, x=border, n = none
//=============================================================
//Funcation Declarations
//=============================================================
void	GenerateLevel();
void	PaintLevel();
void	SpawnEnemy(); // int enemyperlevel
void	UpdateMatrix(RECT position);
bool	isAvailable(int newPosX, int newPosY);
void	spawnEnemy(int enemyType);
void	clearMatrix();
void	updateAllEnemyPos();
void	updateEnemyPos(Enemy* enemy);
void	SetRandomDirection(Sprite* sprite);
void	ChangeAllEnemyDirections();
void	ChangeEnemyOppositeDirection(Enemy* enemy);
void	EnemyFireRandomly();
void	SmallExplosion(RECT rcPosition);
void	spawnNewFlag(Sprite* eSprite);
void	NextLevel();
void	LargeExplosion(RECT rcPosition);
void	InitGameVariables();
void	StartMusic();
void	InitBitmaps(HDC hDC);
void	GenerateNextLevel();