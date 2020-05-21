#ifndef BULLET_HEADER
#define BULLET_HEADER

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
class Bullet : public Sprite {
public:
	// Constructors and Destructor
	Bullet(Bitmap* pBitmap) : Sprite(pBitmap) {}
	Bullet(Bitmap* pBitmap, RECT& rcBounds, BOUNDSACTION baBoundsAction = BA_STOP) :
		Sprite(pBitmap, rcBounds, baBoundsAction = BA_STOP) {}
	Bullet(Bitmap* pBitmap, POINT ptPosition, POINT ptVelocity, int iZOrder, RECT& rcBounds, BOUNDSACTION m_baBoundsAction = BA_STOP) :
		Sprite(pBitmap, ptPosition, ptVelocity, iZOrder, rcBounds, m_baBoundsAction = BA_STOP) {}
	~Bullet() {}
};

//=============================================================
//Funcation Declarations
//=============================================================
void	Move(int iXDistance, int iYDistance);
#endif