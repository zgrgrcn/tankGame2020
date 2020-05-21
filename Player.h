//=============================================================
//Include Files
//=============================================================
#include <windows.h>
#include "resource.h"
#include "GameEngine.h"
#include "Bitmap.h"
#include "Sprite.h"
#include "Constants.h"

//=============================================================
//Global Variables
//=============================================================
class Player : public Sprite
{
public:
	// Constructors and Destructor
	Player(Bitmap* pBitmap) : Sprite(pBitmap) {}
	Player(Bitmap* pBitmap, RECT& rcBounds, BOUNDSACTION baBoundsAction = BA_STOP) :
		Sprite(pBitmap, rcBounds, baBoundsAction = BA_STOP) {}
	Player(Bitmap* pBitmap, POINT ptPosition, POINT ptVelocity, int iZOrder, RECT& rcBounds, BOUNDSACTION m_baBoundsAction = BA_STOP) :
		Sprite(pBitmap, ptPosition, ptVelocity, iZOrder, rcBounds, m_baBoundsAction = BA_STOP) {}
	~Player() {}


	//=============================================================
	//Variables
	//=============================================================
	int		_playerSpeed;
	int		_playerMatrixPosX;
	int		_playerMatrixPosY;
	int		_playerLives;
	int		_playerLevel;
	int		_playerScore;
	int		_playerDirection; //1Top-2right-3Bottom-4Left
	int		_ammo = 25;
	int     _killCount;
	int     _flagCount;
	int		_playerTotal;


	//=============================================================
	//Funcation Declarations
	//=============================================================
	void	Move(int iXDistance, int iYDistance);
	void	Fire(GameEngine* _pGame, Bitmap* _bulletBitmap, RECT rcBounds, int direction);

	void	changeBitmap(Bitmap* pBitmap);

};

