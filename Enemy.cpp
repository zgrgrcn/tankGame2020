#include "Enemy.h"
#include "Constants.h"
#include "resource.h"
#include <iostream>
using namespace std;

void Enemy::Fire(GameEngine* _pGame, Bitmap* _bulletBitmap, RECT rcBounds, int direction)
{
	int posX, posY;
	Bullet* bullet = new Bullet(_bulletBitmap, rcBounds, BA_WRAP);
	bullet->SetTag(TAG_BULLET_ENEMY);
	bullet->groupTag = TAG_BULLET;

	
	if (_movementType == 1) {
		//Up
		if (direction == 1)
		{
			posX = GetPosition().left + GetWidth() / 2 - 2;
			posY = GetPosition().top;

			bullet->SetPosition(posX, posY - 1);
			bullet->SetVelocity(0, -BULLET_SPEED);

			_pGame->AddSprite(bullet);
		}


		// Right
		if (direction == 2) {
			posX = GetPosition().right;
			posY = GetPosition().top + GetHeight() / 4;

			bullet->SetPosition(posX + 1, posY);
			bullet->SetVelocity(BULLET_SPEED, 0);

			_pGame->AddSprite(bullet);
		}


		if (direction == 3) {
			posX = GetPosition().left + GetWidth() / 2 - 2;
			posY = GetPosition().bottom;

			bullet->SetPosition(posX, posY + 1);
			bullet->SetVelocity(0, BULLET_SPEED);

			_pGame->AddSprite(bullet);
		}



		if (direction == 4) {
			posX = GetPosition().left;
			posY = GetPosition().top + GetHeight() / 4;

			bullet->SetPosition(posX - 1, posY);
			bullet->SetVelocity(-BULLET_SPEED, 0);

			_pGame->AddSprite(bullet);
		}
	}
	else if (_movementType == 0) {

		Bullet* bullet2 = new Bullet(_bulletBitmap, rcBounds, BA_WRAP);
		Bullet* bullet3 = new Bullet(_bulletBitmap, rcBounds, BA_WRAP);
		Bullet* bullet4 = new Bullet(_bulletBitmap, rcBounds, BA_WRAP);
		bullet2->SetTag(TAG_BULLET_ENEMY);
		bullet2->groupTag = TAG_BULLET;
		bullet3->SetTag(TAG_BULLET_ENEMY);
		bullet3->groupTag = TAG_BULLET;
		bullet4->SetTag(TAG_BULLET_ENEMY);
		bullet4->groupTag = TAG_BULLET;

			posX = GetPosition().left + GetWidth() / 2 - 2;
			posY = GetPosition().top;

			bullet->SetPosition(posX, posY - 1);
			bullet->SetVelocity(0, -BULLET_SPEED);
			_pGame->AddSprite(bullet);

		// Right
		
			posX = GetPosition().right;
			posY = GetPosition().top + GetHeight() / 4;

			bullet2->SetPosition(posX + 1, posY);
			bullet2->SetVelocity(BULLET_SPEED, 0);
			_pGame->AddSprite(bullet2);
		


		
			posX = GetPosition().left + GetWidth() / 2 - 2;
			posY = GetPosition().bottom;

			bullet3->SetPosition(posX, posY + 1);
			bullet3->SetVelocity(0, BULLET_SPEED);
			_pGame->AddSprite(bullet3);
		

			posX = GetPosition().left;
			posY = GetPosition().top + GetHeight() / 4;

			bullet4->SetPosition(posX - 1, posY);
			bullet4->SetVelocity(-BULLET_SPEED, 0);
			_pGame->AddSprite(bullet4);		
	}
	
}

void Enemy::Move(int targetX, int targetY, char _gameMatrix[][MATRIX_WIDTH])
{
	//0 random, 1 focused -> _movementType
	
		this->_movementCapacity -= PLAYER_SPEED / 5;
		srand(GetTickCount());
		if (_movementCapacity <= 0) {
			if (this->_movementType == 1) {
				int Ex = this->_enemyMatrixPosX;
				int Ey = this->_enemyMatrixPosY;
				this->SetVelocity(0, 0);
				int leftDist = 999, rightDist = 999, topDist = 999, downDist = 999;
				if (Ex > 0 && _gameMatrix[Ex - 1][Ey] == 'n') {//left
					leftDist = abs((Ex - 1) - targetX) + abs(Ey - targetY);
				}
				if (Ex < (MATRIX_WIDTH - 1) && _gameMatrix[Ex + 1][Ey] == 'n') {//Right
					rightDist = abs((Ex + 1) - targetX) + abs(Ey - targetY);
				}
				if (Ey > 0 && _gameMatrix[Ex][Ey - 1] == 'n') {//Up
					topDist = abs((Ey - 1) - targetY) + abs(Ex - targetX);
				}
				if (Ey < (MATRIX_HEIGHT - 1) && _gameMatrix[Ex][Ey + 1] == 'n') {//Down
					downDist = abs((Ey + 1) - targetY) + abs(Ex - targetX);
				}
				int minimumDistance = min(min(leftDist, rightDist), min(topDist, downDist));

				//birden fazla deger minimum ise aralarindan birini buyut/digerini tercih et
				if (minimumDistance == leftDist && minimumDistance == rightDist) {// l ve r esit
					if (rand() % 2 == 0)
						leftDist = 999;
					else
						rightDist = 999;
				}
				if (minimumDistance == leftDist && minimumDistance == downDist) {// l ve d esit
					if (rand() % 2 == 0)
						leftDist = 999;
					else
						downDist = 999;
				}
				if (minimumDistance == leftDist && minimumDistance == topDist) {// l ve t esit
					if (rand() % 2 == 0)
						leftDist = 999;
					else
						topDist = 999;
				}
				if (minimumDistance == topDist && minimumDistance == rightDist) {// t ve r esit
					if (rand() % 2 == 0)
						topDist = 999;
					else
						rightDist = 999;
				}
				if (minimumDistance == topDist && minimumDistance == downDist) {// t ve d esit
					if (rand() % 2 == 0)
						topDist = 999;
					else
						downDist = 999;
				}
				if (minimumDistance == downDist && minimumDistance == rightDist) {// d ve r esit
					if (rand() % 2 == 0)
						downDist = 999;
					else
						rightDist = 999;
				}



				if (minimumDistance == 0 || minimumDistance == 999)
					return;
				if (minimumDistance == leftDist)
				{
					this->_enemyMatrixPosX--;
					this->SetVelocity(-PLAYER_SPEED / 5, 0);
					_movementCapacity = SPRITE_SIZE;
					this->direction = 4;
					this->UpdateSprite(_enemyBitmapLeft);
				}
				else if (minimumDistance == rightDist)
				{
					this->_enemyMatrixPosX++;
					this->SetVelocity(PLAYER_SPEED / 5, 0);
					_movementCapacity = SPRITE_SIZE;
					this->direction = 2;
					this->UpdateSprite(_enemyBitmapRight);
				}
				else if (minimumDistance == topDist)
				{
					this->_enemyMatrixPosY--;
					this->SetVelocity(0, -PLAYER_SPEED / 5);
					_movementCapacity = SPRITE_SIZE;
					this->direction = 1;
					this->UpdateSprite(_enemyBitmapUp);
				}
				else if (minimumDistance == downDist)
				{
					this->_enemyMatrixPosY++;
					this->SetVelocity(0, PLAYER_SPEED / 5);
					_movementCapacity = SPRITE_SIZE;
					this->direction = 3;
					this->UpdateSprite(_enemyBitmapDown);
				}
			}
			else if (this->_movementType == 0) {
				int Ex = this->_enemyMatrixPosX;
				int Ey = this->_enemyMatrixPosY;
				this->SetVelocity(0, 0);

				//set random direction
				int randomDirection = rand() % 4 + 1;
				this->direction = randomDirection;
				switch (randomDirection)
				{
				case 1: // Up
					if (Ey > 0 && _gameMatrix[Ex][Ey - 1] == 'n') {
						this->_enemyMatrixPosY--;
						this->SetVelocity(0, -PLAYER_SPEED / 5);
						this->UpdateSprite(_enemyBitmapUp);
						_movementCapacity = SPRITE_SIZE;
					}
					break;
				case 2: // Right
					if (Ex < (MATRIX_WIDTH - 1) && _gameMatrix[Ex + 1][Ey] == 'n') {
						this->_enemyMatrixPosX++;
						this->SetVelocity(PLAYER_SPEED / 5, 0);
						this->UpdateSprite(_enemyBitmapRight);
						_movementCapacity = SPRITE_SIZE;
					}
					break;
				case 3: // Down
					if (Ey < (MATRIX_HEIGHT - 1) && _gameMatrix[Ex][Ey + 1] == 'n') {
						this->_enemyMatrixPosY++;
						this->SetVelocity(0, PLAYER_SPEED / 5);
						this->UpdateSprite(_enemyBitmapDown);
						_movementCapacity = SPRITE_SIZE;
					}
					break;
				case 4: // Left
					if (Ex > 0 && _gameMatrix[Ex - 1][Ey] == 'n') {
						this->_enemyMatrixPosX--;
						this->SetVelocity(-PLAYER_SPEED / 5, 0);
						this->UpdateSprite(_enemyBitmapLeft);
						_movementCapacity = SPRITE_SIZE;
					}
					break;
				default:
					break;
				}
				
			}
		}

	
	
}

