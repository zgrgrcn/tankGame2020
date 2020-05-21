//=============================================================
//Include Files
//=============================================================
#include <windows.h>
#include "resource.h"
#include "GameEngine.h"
#include "Bitmap.h"
#include "Sprite.h"

//=============================================================
//Global Variables
//=============================================================

class Environment : public Sprite {
public:
	// Constructors and Destructor
	// Second part is calling super constructor
	Environment(Bitmap* pBitmap) : Sprite(pBitmap) {}
	Environment(Bitmap* pBitmap, RECT& rcBounds, BOUNDSACTION baBoundsAction = BA_STOP) :
		Sprite(pBitmap, rcBounds, baBoundsAction = BA_STOP) {}
	Environment(Bitmap* pBitmap, POINT ptPosition, POINT ptVelocity, int iZOrder, RECT& rcBounds, BOUNDSACTION m_baBoundsAction = BA_STOP) :
		Sprite(pBitmap, ptPosition, ptVelocity, iZOrder, rcBounds, m_baBoundsAction = BA_STOP) {}
	virtual ~Environment() {}
	/*
	Bitmap* _environmentBitmap;
	Bitmap* _woodBitmap;
	Bitmap* _steelBitmap;
	Sprite* _environmentSprite
	*/
	int		_matrixPosX;
	int		_matrixPosY;
	int		_isBreakable;
	int		_isBulletProof;
	int		_isMoveable;
	int		_envHealth;
};

// Concrete class of the Environment
class Wood : public Environment
{
public:
	// Constructor and Destructor
	Wood(Bitmap* pBitmap) : Environment(pBitmap) {}
	Wood(Bitmap* pBitmap, RECT& rcBounds, BOUNDSACTION baBoundsAction = BA_STOP) :
		Environment(pBitmap, rcBounds, baBoundsAction = BA_STOP) {}
	Wood(Bitmap* pBitmap, POINT ptPosition, POINT ptVelocity, int iZOrder, RECT& rcBounds, BOUNDSACTION m_baBoundsAction = BA_STOP) :
		Environment(pBitmap, ptPosition, ptVelocity, iZOrder, rcBounds, m_baBoundsAction = BA_STOP) {}
	~Wood() {}
};

// Concrete class of the Environment
class Steel : public Environment
{
public:
	// Constructor and Destructor
	Steel(Bitmap* pBitmap) : Environment(pBitmap) {}
	Steel(Bitmap* pBitmap, RECT& rcBounds, BOUNDSACTION baBoundsAction = BA_STOP) :
		Environment(pBitmap, rcBounds, baBoundsAction = BA_STOP) {}
	Steel(Bitmap* pBitmap, POINT ptPosition, POINT ptVelocity, int iZOrder, RECT& rcBounds, BOUNDSACTION m_baBoundsAction = BA_STOP) :
		Environment(pBitmap, ptPosition, ptVelocity, iZOrder, rcBounds, m_baBoundsAction = BA_STOP) {}
	~Steel() {}
};