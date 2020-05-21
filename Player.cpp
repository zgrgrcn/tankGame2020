#include "Player.h"
#include "Bullet.h"

void Player::Fire(GameEngine* _pGame, Bitmap* _bulletBitmap, RECT rcBounds, int direction)
{

	if (_ammo>0)
	{
		_ammo--;

		Bullet* bullet = new Bullet(_bulletBitmap, rcBounds, BA_WRAP);
		bullet->SetTag(TAG_BULLET_PLAYER);
		bullet->groupTag = TAG_BULLET;

		int posX, posY;

		//Top
		if (direction == 1) {
			posX = GetPosition().left + GetWidth() / 2 - 2;
			posY = GetPosition().top;

			bullet->SetPosition(posX, posY);
			bullet->SetVelocity(0, -BULLET_SPEED);
		}
		// Right
		else if (direction == 2) {
			posX = GetPosition().right;
			posY = GetPosition().top + GetHeight() / 4;

			bullet->SetPosition(posX, posY);
			bullet->SetVelocity(BULLET_SPEED, 0);
		}

		// Down
		else if (direction == 3) {
			posX = GetPosition().left + GetWidth() / 2 - 2;
			posY = GetPosition().bottom;

			bullet->SetPosition(posX, posY);
			bullet->SetVelocity(0, BULLET_SPEED);
		}

		//Left
		else if (direction == 4) {
			posX = GetPosition().left;
			posY = GetPosition().top + GetHeight() / 4;

			bullet->SetPosition(posX, posY);
			bullet->SetVelocity(-BULLET_SPEED, 0);
		}
		_pGame->AddSprite(bullet);
	}
}
