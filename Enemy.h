//=============================================================
//Include Files
//=============================================================
#include <windows.h>
#include "resource.h"
#include "GameEngine.h"
#include "Bitmap.h"
#include "Sprite.h"
#include "Constants.h"
#include "Bullet.h"
//=============================================================
//Global Variables
//=============================================================
class Enemy : public Sprite {
public:
	// Constructor and Destructor
	Enemy(Bitmap* enemyBitmapUp, Bitmap* enemyBitmapRight ,
		Bitmap* enemyBitmapDown,Bitmap* enemyBitmapLeft, RECT& rcBounds, BOUNDSACTION baBoundsAction = BA_STOP) : Sprite(enemyBitmapUp, rcBounds, baBoundsAction = BA_STOP)
	{
		_enemyBitmapUp = enemyBitmapUp;
		_enemyBitmapRight =enemyBitmapRight;
		_enemyBitmapDown = enemyBitmapDown;
		_enemyBitmapLeft = enemyBitmapLeft;
		_movementCapacity=0;
	}

	Enemy(Bitmap* pBitmap, RECT& rcBounds, BOUNDSACTION baBoundsAction = BA_STOP) :
		Sprite(pBitmap, rcBounds, baBoundsAction = BA_STOP) {}
	Enemy(Bitmap* pBitmap, POINT ptPosition, POINT ptVelocity, int iZOrder, RECT& rcBounds, BOUNDSACTION m_baBoundsAction = BA_STOP) :
		Sprite(pBitmap, ptPosition, ptVelocity, iZOrder, rcBounds, m_baBoundsAction = BA_STOP) {}
	~Enemy() {}

	//void	EnemyMove(GameEngine* _pGame, char gameMatrix[20][20], HINSTANCE hInstance);
	//double		Enemy::findDistance(int a1, int b1, int a2,int b2);
	int		_type;
	int		_enemyID;
	int		_enemyLives;
	int		_enemyMatrixPosX;
	int		_enemyMatrixPosY;
	int		_movementCapacity;
	int		_movementType; //0 random, 1 focused

	Bitmap*		_enemyBitmapUp;
	Bitmap*		_enemyBitmapRight;
	Bitmap*		_enemyBitmapDown;
	Bitmap*		_enemyBitmapLeft;

	void	Fire(GameEngine* _pGame, Bitmap* _bulletBitmap, RECT rcBounds, int direction);
	void	Move(int targetX, int targetY,char _gameMatrix[][MATRIX_WIDTH]);
};

//=============================================================
//Funcation Declarations
//=============================================================
