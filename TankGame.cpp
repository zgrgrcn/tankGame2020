//=============================================================
//Include Files
//=============================================================
#include "TankGame.h"
#include "Constants.h"
#include "resource.h"
#include <ctime>
#include <conio.h>
//=============================================================
//Game Engine Functions
//=============================================================

BOOL GameInitialize(HINSTANCE hInstance)
{
	//Create the game engine
	_pGame = new GameEngine(hInstance, TEXT("Tank 2020"),
		TEXT("Tank 2020"), IDB_PLAYERUP, IDB_PLAYERUP, SCREEN_WIDTH, SCREEN_HEIGHT);

	if (_pGame == NULL)
		return FALSE;


	//Set the frame rate
	_pGame->SetFrameRate(60);

	//Store the instance handle
	_hInstance = hInstance;
	timeStart = std::time(nullptr);

	return TRUE;
}

void GameStart(HWND hWindow)
{
	InitGameVariables();
	//Cleanup/close MIDI Player
	_pGame->CloseMIDIPlayer();

	//Create the offscreen device context and bitmap
	_hOffscreenDC = CreateCompatibleDC(GetDC(hWindow));
	_hOffscreenBitmap = CreateCompatibleBitmap(GetDC(hWindow),
		_pGame->GetWidth(), _pGame->GetHeight());
	SelectObject(_hOffscreenDC, _hOffscreenBitmap);

	HDC hDC = GetDC(hWindow);


	GenerateLevel();
	InitBitmaps(hDC);
	PaintLevel();
	StartMusic();
}

void GameEnd()
{
	//Cleanup and close MIDI player

	//_pGame->CloseMIDIPlayer();
	//Cleanup the offscreen device context and bitmap
	DeleteObject(_hOffscreenBitmap);
	DeleteDC(_hOffscreenDC);

	//Cleanup the bitmaps

	delete _pGame;
}

void GameActivate(HWND hWindow)
{
	//_pGame->PlayMIDISong(TEXT(""), FALSE);
}

void GameDeactivate(HWND hWindow)
{
	//_pGame->PauseMIDISong();
}

void GamePaint(HDC hDC)
{
	//Draw the bakcground highway
	_pBackgroundBitmap->Draw(hDC, 0, 0);

	//Draw the sprites
	_pGame->DrawSprites(hDC);

	// Draw the score
	TCHAR szText[64];
	RECT rect = { 50, 50,  SCREEN_WIDTH - MARGIN_SCORE_RIGHT / 2, 0 + MARGIN_SCORE_TOP };
	wsprintf(szText, "Score: %d", _player->_playerTotal);
	SetBkMode(hDC, TRANSPARENT);
	SetTextColor(hDC, RGB(255, 255, 255));
	DrawText(hDC, szText, -1, &rect, DT_SINGLELINE | DT_RIGHT | DT_VCENTER);

	// Draw the ammo text
	rect = { 50, 50, MARGIN_SCORE_RIGHT, 0 + MARGIN_SCORE_TOP };
	wsprintf(szText, "Ammo: %d", _player->_ammo);
	SetBkMode(hDC, TRANSPARENT);
	SetTextColor(hDC, RGB(255, 255, 255));
	DrawText(hDC, szText, -1, &rect, DT_SINGLELINE | DT_RIGHT | DT_VCENTER);

	// Draw the kill count text
	rect = { 50, 50, MARGIN_SCORE_RIGHT + 150, 0 + MARGIN_SCORE_TOP };
	wsprintf(szText, "Kill: %d", _player->_killCount);
	SetBkMode(hDC, TRANSPARENT);
	SetTextColor(hDC, RGB(255, 255, 255));
	DrawText(hDC, szText, -1, &rect, DT_SINGLELINE | DT_RIGHT | DT_VCENTER);

	// Draw the flag count text
	rect = { 50, 50, MARGIN_SCORE_RIGHT + 225, 0 + MARGIN_SCORE_TOP };
	wsprintf(szText, "Flag: %d", _flagCounter);
	SetBkMode(hDC, TRANSPARENT);
	SetTextColor(hDC, RGB(255, 255, 255));
	DrawText(hDC, szText, -1, &rect, DT_SINGLELINE | DT_RIGHT | DT_VCENTER);

	// Draw the Timer text
	rect = { 50, 50, MARGIN_SCORE_RIGHT + 80, 0 + MARGIN_SCORE_TOP };
	std::time_t currentTime = std::time(nullptr);
	wsprintf(szText, "Round: %d", _gameRound);
	SetBkMode(hDC, TRANSPARENT);
	SetTextColor(hDC, RGB(255, 255, 255));
	DrawText(hDC, szText, -1, &rect, DT_SINGLELINE | DT_RIGHT | DT_VCENTER);
	if (GAME_TIME - (currentTime - timeStart) <= 0) {
		_bGameOver = true;
	}


	_playerBitmapUp->Draw(hDC, 380 + (_playerBitmapUp->GetWidth()), MARGIN_SCORE_TOP, TRUE);
	rect = { 50, 50, MARGIN_SCORE_RIGHT + 355, 0 + MARGIN_SCORE_TOP };
	wsprintf(szText, " X %d", _player->_playerLives);
	SetBkMode(hDC, TRANSPARENT);
	SetTextColor(hDC, RGB(255, 255, 255));
	DrawText(hDC, szText, -1, &rect, DT_SINGLELINE | DT_RIGHT | DT_VCENTER);

	/*// Draw the number of remaining lives (cars)
	for (int i = 0; i < _player->_playerLives; i++)
		*/

		/*
		//Draw the number of remaining chicken lives
		for (int i = 0; i < _iNumLives; i++)
			_pChickenHeadBitmap->Draw(hDC,
			406 + (_pChickenHeadBitmap->GetWidth() * i), 382, TRUE);
		*/
}

void GameCycle()
{
	//Obtain a device context for repainting the game
	HWND	hWindow = _pGame->GetWindow();
	HDC		hDC = GetDC(hWindow);
	if (!_bGameOver)
	{
		//Update the sprites
		_pGame->UpdateSprites();
		ChangeAllEnemyDirections();
		if (_timer >= 10) {
			EnemyFireRandomly();
			_timer = 0;
		}
		else
			_timer++;

		//hdc ile hdw buradaydi

		//Paint the game to the offscreen device context
		GamePaint(_hOffscreenDC);

		//Blit the offscreen bitmap to the game screen
		BitBlt(hDC, 0, 0, _pGame->GetWidth(), _pGame->GetHeight(),
			_hOffscreenDC, 0, 0, SRCCOPY);

		//Cleanup
		ReleaseDC(hWindow, hDC);
	}
	else
	{
		/*
		TCHAR szText[64];
		RECT rect = { 0,0,450,750};
		wsprintf(szText, "Game Over: %d points", _player->_playerScore);
		SetBkMode(hDC, TRANSPARENT);
		SetTextColor(hDC, RGB(255, 255, 255));
		DrawText(hDC, szText, -1, &rect, DT_SINGLELINE | DT_RIGHT | DT_VCENTER);
		*/

		_gameOverBitmap->Draw(hDC, 175, 200, TRUE);
		GamePaint(_hOffscreenDC);

		_pGame->CloseMIDIPlayer();
		_pGame->PlayMIDISong("gameOver.mid");


		/*while (true) {

			Sleep(100);
			if (GetAsyncKeyState(VK_ESCAPE) < 0 || GetAsyncKeyState(VK_RETURN) < 0)
			{
				break;
			}
		}

		GameEnd();*/
	}

}

void EnemyFireRandomly()
{
	srand(GetTickCount());
	int previusEnemy = -1;
	int random = rand() % _enemies.size();
	while (random == previusEnemy) {
		srand(GetTickCount());
		random = rand() % _enemies.size();
	}

	if (_enemies.at(random)->_movementType == 0) {
		int rm = rand() % 2;
		if (rm == 0) {
			_enemies.at(random)->Fire(_pGame, _bulletBitmap, rcBounds, _enemies.at(random)->direction);
		}
	}
	else {
		_enemies.at(random)->Fire(_pGame, _bulletBitmap, rcBounds, _enemies.at(random)->direction);	
	}

	previusEnemy = random;
}

//delete
void UpdateEnemySprite(Enemy* enemy)
{
	switch (enemy->direction)
	{
	case 1: // Up
		enemy->UpdateSprite(_enemyBitmapUp);
		break;

	case 2: // Right
		enemy->UpdateSprite(_enemyBitmapRight);
		break;

	case 3: // Down
		enemy->UpdateSprite(_enemyBitmapDown);
		break;

	case 4: // Left
		enemy->UpdateSprite(_enemyBitmapLeft);
		break;

	default:
		break;
	}
}

//rename
void ChangeAllEnemyDirections()
{

	for (int i = 0; i < _enemies.size(); i++)
	{
		_enemies.at(i)->Move(_player->_playerMatrixPosX, _player->_playerMatrixPosY, _gameMatrix);

		//SetRandomDirection(_enemies.at(i));
		//UpdateEnemySprite(_enemies.at(i));
	}
}

void ChangeEnemyOppositeDirection(Enemy* enemy)
{
	int posX = enemy->GetPosition().left;
	int posY = enemy->GetPosition().top;

	enemy->_movementCapacity = 400;
	if (enemy->direction == 1) // UP
	{
		// Set enemy direction to DOWN
		enemy->UpdateSprite(_enemyBitmapDown);
		enemy->SetVelocity(0, 1);
		enemy->SetPosition(posX, posY + 1);
		enemy->direction = 3;
	}
	else if (enemy->direction == 3) // DOWN
	{
		// Set enemy velocity and direction to UP
		enemy->UpdateSprite(_enemyBitmapUp);
		enemy->SetVelocity(0, -1);
		enemy->SetPosition(posX, posY - 1);
		enemy->direction = 1;
	}
	else if (enemy->direction == 2) // RIGHT
	{
		// Set enemy velocity and direction to LEFT
		enemy->UpdateSprite(_enemyBitmapLeft);
		enemy->SetVelocity(-1, 0);
		enemy->SetPosition(posX - 1, posY);
		enemy->direction = 4;
	}
	else if (enemy->direction == 4) // LEFT
	{
		// Set enemy velocity and direction to RIGHT
		enemy->UpdateSprite(_enemyBitmapRight);
		enemy->SetVelocity(1, 0);
		enemy->SetPosition(posX + 1, posY);
		enemy->direction = 2;
	}
}

bool isAvailable(int newPosX, int newPosY)
{
	//SCREEN_WIDTH=750 , GAMEAREA_MARGIN_X=150
	// base = b, p = player, e = enemy, w = wood, s = steel, x=border, n = none
	int posX = (int)((newPosX - GAMEAREA_MARGIN_X / 2) / _cellWidth);
	int posY = (int)((newPosY - GAMEAREA_MARGIN_Y / 2) / _cellWidth);
	char nextStep = _gameMatrix[posX][posY];

	if (nextStep == 'n' || nextStep == 'p')
		return true;
	return false;
}

void HandleKeys()
{
	
	int cx = _player->GetPosition().left;
	int cy = _player->GetPosition().top;
	int cmx = cx / 40;
	int cmy = cy / 40;

	if (cx % 40 < 5)
	{
		cx = cmx * 40;
	}

	if (cx % 40 > 35)
	{
		cx = (cmx + 1) * 40;
	}

	if (cy % 40 < 5)
	{
		cy = cmy * 40;
	}

	if (cy % 40 > 35)
	{
		cy = (cmy + 1) * 40;
	}

	_player->SetPosition(cx, cy);

	if (!_bGameOver)
	{
		_bfrm -= PLAYER_SPEED;
		if (_bfrm <= 0)
		{

			if (GetAsyncKeyState(VK_LEFT) < 0 && _player->direction != 4)
			{
				_player->direction = 4;
				_player->UpdateSprite(_playerBitmapLeft);
			}

			else if (GetAsyncKeyState(VK_RIGHT) < 0 && _player->direction != 2)
			{
				_player->direction = 2;
				_player->UpdateSprite(_playerBitmapRight);
			}

			else if (GetAsyncKeyState(VK_UP) < 0 && _player->direction != 1)
			{
				_player->direction = 1;
				_player->UpdateSprite(_playerBitmapUp);
			}

			else if (GetAsyncKeyState(VK_DOWN) < 0 && _player->direction != 3)
			{
				_player->direction = 3;
				_player->UpdateSprite(_playerBitmapDown);
			}

			_player->SetVelocity(0, 0);
			int bx = _player->_playerMatrixPosX;
			int by = _player->_playerMatrixPosY;
			_gameMatrix[bx][by] = 'n';
			//1Top-2right-3Bottom-4Left
			if (bx > 0 && (_gameMatrix[bx - 1][by] == 'f' || _gameMatrix[bx - 1][by] == 'n') && GetAsyncKeyState(VK_LEFT) < 0)
			{
				_player->_playerMatrixPosX--;
				_player->SetVelocity(-PLAYER_SPEED, 0);
				_bfrm = SPRITE_SIZE;
				_player->direction = 4;
			}
			else if (bx < (MATRIX_WIDTH - 1) && (_gameMatrix[bx + 1][by] == 'f' || _gameMatrix[bx + 1][by] == 'n') && GetAsyncKeyState(VK_RIGHT) < 0)
			{
				_player->_playerMatrixPosX++;
				_player->SetVelocity(PLAYER_SPEED, 0);
				_bfrm = SPRITE_SIZE;
				_player->direction = 2;
			}
			else if (by > 0 && (_gameMatrix[bx][by - 1] == 'f' || _gameMatrix[bx][by - 1] == 'n') && GetAsyncKeyState(VK_UP) < 0)
			{
				_player->_playerMatrixPosY--;
				_player->SetVelocity(0, -PLAYER_SPEED);
				_bfrm = SPRITE_SIZE;
				_player->direction = 1;
			}
			else if (by < (MATRIX_HEIGHT - 1) && (_gameMatrix[bx][by + 1] == 'n' || _gameMatrix[bx][by + 1] == 'f') && GetAsyncKeyState(VK_DOWN) < 0)
			{
				_player->_playerMatrixPosY++;
				_player->SetVelocity(0, PLAYER_SPEED);
				_bfrm = SPRITE_SIZE;
				_player->direction = 3;
			}
			_gameMatrix[_player->_playerMatrixPosX][_player->_playerMatrixPosY] = 'p';
		}


		if (GetAsyncKeyState(VK_SPACE) < 0 && _FireDelay > 6)
		{
			PlaySound((LPCSTR)IDW_TANKFIRE, _hInstance, SND_ASYNC |
				SND_RESOURCE);
			_player->Fire(_pGame, _bulletBitmap, rcBounds, _player->direction);
			_FireDelay = 0;

			if (_player->_ammo > 0)
			{
				// Animation
				int direction = _player->direction;
				double posX = 0;
				double posY = 0;
				// Up
				if (direction == 1) {
					posX = _player->GetPosition().left + _player->GetWidth() / 2 - 7;
					posY = _player->GetPosition().top - 15;
				}
				// Right
				else if (direction == 2)
				{
					posX = _player->GetPosition().right;
					posY = _player->GetPosition().top + _player->GetHeight() / 4;
				}

				// Down
				else if (direction == 3) {
					posX = _player->GetPosition().left + _player->GetWidth() / 2 - 7;
					posY = _player->GetPosition().bottom;
				}

				//Left
				else if (direction == 4) {
					posX = _player->GetPosition().left - 15;
					posY = _player->GetPosition().top + _player->GetHeight() / 4;
				}

				RECT playerPos = _player->GetPosition();
				playerPos.left = posX;
				playerPos.top = posY;
				SmallExplosion(playerPos);
			}
		}


		//Reset the input delay
		_iInputDelay = 0;
		_FireDelay++;
	}
}

void clearMatrix() {
	for (int i = 0; i < 20; i++)
	{
		for (int j = 0; j < 20; j++)
		{
			if (_gameMatrix[i][j] == 'p') {
				_gameMatrix[i][j] = 'n';
			}

		}
	}
}

void HandleJoystick(JOYSTATE jsJoystickState)
{
}

void UpdateMatrix(RECT position)
{
	int matrixPosX = (int)((position.left - GAMEAREA_MARGIN_X / 2) / _cellWidth);
	int matrixPosY = (int)((position.top - GAMEAREA_MARGIN_Y / 2) / _cellWidth);
	_gameMatrix[matrixPosX][matrixPosY] = 'n';
}

BOOL SpriteCollision(Sprite* pSpriteHitter, Sprite* pSpriteHittee)
{

	// When player collide with environment.
	if ((pSpriteHitter->GetTag() == TAG_PLAYER && pSpriteHittee->groupTag == TAG_ENV) ||
		(pSpriteHitter->groupTag == TAG_ENV && pSpriteHittee->GetTag() == TAG_PLAYER))
	{
		Player* player = ((Player*)pSpriteHitter);
		if (pSpriteHittee->GetTag() == TAG_PLAYER)
		{
			player = ((Player*)pSpriteHittee);
		}

		int posX = player->GetPosition().left;
		int posY = player->GetPosition().top;

		if (player->direction == 1) // UP
		{
			player->SetPosition(posX, posY + 1);
		}
		else if (player->direction == 3) // DOWN
		{
			player->SetPosition(posX, posY - 1);
		}
		else if (player->direction == 2) // RIGHT
		{
			player->SetPosition(posX - 1, posY);
		}
		else if (player->direction == 4) // RIGHT
		{
			player->SetPosition(posX + 1, posY);
		}
		player->SetVelocity(0, 0);
	}
	// When enemy collide with environment.
	else if ((pSpriteHitter->GetTag() == TAG_ENEMY && pSpriteHittee->groupTag == TAG_ENV) ||
		(pSpriteHitter->groupTag == TAG_ENV && pSpriteHittee->GetTag() == TAG_ENEMY))
	{
		Enemy* enemy = ((Enemy*)pSpriteHitter);
		if (pSpriteHittee->GetTag() == TAG_ENEMY)
		{
			enemy = ((Enemy*)pSpriteHittee);
		}
		//ChangeEnemyOppositeDirection(enemy);
	}
	// When enemy collide with another enemy.
	else if ((pSpriteHitter->GetTag() == TAG_ENEMY && pSpriteHittee->GetTag() == TAG_ENEMY))
	{
		//ChangeEnemyOppositeDirection(((Enemy*)pSpriteHitter));
		//ChangeEnemyOppositeDirection(((Enemy*)pSpriteHittee));	
	}
	// When enemy collide with player.
	else if ((pSpriteHitter->GetTag() == TAG_PLAYER && pSpriteHittee->GetTag() == TAG_ENEMY))
	{
		Enemy* enemy = ((Enemy*)pSpriteHittee);
		Player* player = ((Player*)pSpriteHitter);

		player->_playerLives--;
		PlaySound((LPCSTR)IDW_PLAYERDEAD, _hInstance, SND_ASYNC | SND_RESOURCE);
		_player->_playerScore += 25 + 20 * (_gameRound * 0.2);
		_player->_playerTotal += 25 + 20 * (_gameRound * 0.2);
		_player->_killCount += 1;

		if (_player->_playerScore >= 500)
		{
			_player->_playerScore -= 500;
			_player->_playerLives++;
		}

		//Seed the random number generator
		srand(GetTickCount());

		UpdateMatrix(enemy->GetPosition());

		int randX = 0;
		int randY = 0;

		do {
			randX = (rand() % MATRIX_WIDTH - 2) + 1;
			randY = (rand() % MATRIX_HEIGHT - 2) + 1;
		} while ((_gameMatrix[randX][randY] != 'n'));

		enemy->_enemyMatrixPosX = randX;
		enemy->_enemyMatrixPosY = randY;
		enemy->SetVelocity(0, 0);
		enemy->SetPosition(_cellWidth * randX + GAMEAREA_MARGIN_X / 2, _cellHeight * randY + GAMEAREA_MARGIN_Y / 2);
		enemy->Move(_player->_playerMatrixPosX, _player->_playerMatrixPosY, _gameMatrix);

		//Seed the random number generator
		srand(GetTickCount());

		if (rand() % 2 == 0)
		{
			PlaySound((LPCSTR)IDW_ENEMYEXPLODE1, _hInstance, SND_ASYNC | SND_RESOURCE);
		}
		else
		{
			PlaySound((LPCSTR)IDW_ENEMYEXPLODE2, _hInstance, SND_ASYNC | SND_RESOURCE);
		}

		if (player->_playerLives <= 0) {
			//endgame
			_bGameOver = true;
		}


		//ChangeEnemyOppositeDirection(enemy);
	}
	// When bullet collide with environment.
	else if ((pSpriteHitter->groupTag == TAG_BULLET && pSpriteHittee->groupTag == TAG_ENV))
	{
		Environment* env = ((Environment*)pSpriteHitter);
		Bullet* bullet = ((Bullet*)pSpriteHittee);
		if (pSpriteHittee->groupTag == TAG_ENV)
		{
			env = ((Environment*)pSpriteHittee);
			bullet = ((Bullet*)pSpriteHitter);
		}

		if (env->GetTag() == TAG_WOOD)
		{
			//PlaySound((LPCSTR)IDW_CRATEBREAK, _hInstance, SND_ASYNC | SND_RESOURCE);
			if (bullet->GetTag() == TAG_BULLET_PLAYER) {
				env->_envHealth -=2;
				
			}
			else if ((bullet->GetTag() == TAG_BULLET_ENEMY)) {
				env->_envHealth--;
			}
			
			if (env->_envHealth <= 0) {
				UpdateMatrix(env->GetPosition());
				//SmallExplosion(bullet->GetPosition());
				env->Kill();
			}

		}
		else if (env->GetTag() == TAG_STEEL)
		{
			if (bullet->GetTag() == TAG_BULLET_PLAYER) {
				env->_envHealth -= 2;

			}
			else if ((bullet->GetTag() == TAG_BULLET_ENEMY)) {
				env->_envHealth--;
			}

			
			if (env->_envHealth <= 0) {
				UpdateMatrix(env->GetPosition());
				env->Kill();
			}
			//SmallExplosion(_player->GetPosition());
		}
		bullet->Kill();
	}
	// When bullet collide with enemy.
	else if ((pSpriteHitter->GetTag() == TAG_BULLET_PLAYER && pSpriteHittee->GetTag() == TAG_ENEMY))
	{
		// Determine enemy sprite
		Enemy* enemy = ((Enemy*)pSpriteHitter);
		Sprite* bullet = pSpriteHittee;

		if (pSpriteHittee->GetTag() == TAG_ENEMY)
		{
			enemy = ((Enemy*)pSpriteHittee);
			bullet = pSpriteHitter;
		}

		// Kill the enemy
		if (enemy->_enemyLives < 1) {

			_player->_playerScore += 25 + 20 * (_gameRound * 0.2);
			_player->_playerTotal += 25 + 20 * (_gameRound * 0.2);
			_player->_killCount += 1;

			if (_player->_playerScore >= 500)
			{
				_player->_playerScore -= 500;
				_player->_playerLives++;
			}

			//Seed the random number generator
			srand(GetTickCount());

			UpdateMatrix(enemy->GetPosition());

			int randX = 0;
			int randY = 0;

			do {
				randX = (rand() % MATRIX_WIDTH - 2) + 1;
				randY = (rand() % MATRIX_HEIGHT - 2) + 1;
			} while ((_gameMatrix[randX][randY] != 'n'));

			enemy->_enemyMatrixPosX = randX;
			enemy->_enemyMatrixPosY = randY;
			enemy->SetVelocity(0, 0);
			enemy->SetPosition(_cellWidth * randX + GAMEAREA_MARGIN_X / 2, _cellHeight * randY + GAMEAREA_MARGIN_Y / 2);
			enemy->Move(_player->_playerMatrixPosX, _player->_playerMatrixPosY, _gameMatrix);

			//Seed the random number generator
			srand(GetTickCount());

			if (rand() % 2 == 0)
			{
				PlaySound((LPCSTR)IDW_ENEMYEXPLODE1, _hInstance, SND_ASYNC | SND_RESOURCE);
			}
			else
			{
				PlaySound((LPCSTR)IDW_ENEMYEXPLODE2, _hInstance, SND_ASYNC | SND_RESOURCE);
			}

			//SmallExplosion(enemy->GetPosition());
			//LargeExplosion(enemy->GetPosition());
		}
		else enemy->_enemyLives--;

		// Kill the bullet
		bullet->Kill();
	}

	// When enemy bullet collide with the player.
	else if ((pSpriteHitter->GetTag() == TAG_BULLET_ENEMY && pSpriteHittee->GetTag() == TAG_PLAYER))
	{
		Bullet* bullet = ((Bullet*)pSpriteHitter);
		Player* player = ((Player*)pSpriteHittee);
		if (pSpriteHittee->groupTag == TAG_BULLET)
		{
			bullet = ((Bullet*)pSpriteHittee);
			player = ((Player*)pSpriteHitter);
		}
		bullet->Kill();
		player->_playerLives--;
		PlaySound((LPCSTR)IDW_PLAYERDEAD, _hInstance, SND_ASYNC | SND_RESOURCE);
		if (player->_playerLives <= 0) {
			//endgame
			_bGameOver = true;
		}

	}
	// When bullet collide with another bullet.
	else if (pSpriteHitter->groupTag == TAG_BULLET && pSpriteHittee->groupTag == TAG_BULLET)
	{
		SmallExplosion(pSpriteHittee->GetPosition());
		pSpriteHitter->Kill();
		pSpriteHittee->Kill();
	}
	// When the player collide with flag
	else if ((pSpriteHitter->GetTag() == TAG_PLAYER && pSpriteHittee->GetTag() == TAG_FLAG) ||
		(pSpriteHitter->GetTag() == TAG_FLAG && pSpriteHittee->GetTag() == TAG_PLAYER))
	{
		if (pSpriteHitter->GetTag() == TAG_FLAG) {

			std::time_t currentTime = std::time(nullptr);
			//timeStart = currentTime;
			_flagCounter--;
			if (_flagCounter >= 0)
				spawnNewFlag(pSpriteHitter);
		}
		else {
			std::time_t currentTime = std::time(nullptr);
			//timeStart = currentTime;
			_flagCounter--;
			if (_flagCounter >= 0)
				spawnNewFlag(pSpriteHittee);
		}


		PlaySound((LPCSTR)IDW_GOTTOFLAG, _hInstance, SND_ASYNC | SND_RESOURCE);
		if (_flagCounter <= 0)
		{
			NextLevel();
		}
	}
	return FALSE;
}

void SpriteDying(Sprite* pSprite)
{
	// See if an alien missile sprite is dying
	/*
	if (pSprite->GetTag() == TAG_WOOD)
	{
		// Play the small explosion sound
		PlaySound((LPCSTR)IDW_ENEMYEXPLODE1, _hInstance, SND_ASYNC |
			SND_RESOURCE | SND_NOSTOP);


		// Create a small explosion sprite at the missile's position
		RECT rcPos = { 0,0,0,0 };
		Sprite* pSprite = new Sprite(_pSmExplosionBitmap, rcBounds);
		pSprite->SetNumFrames(8, TRUE);
		pSprite->SetPosition(rcPos.left, rcPos.top);
		_pGame->AddSprite(pSprite);

	}
	*/
}

void GenerateLevel()
{
	//Seed the random number generator
	srand(GetTickCount());
	// base, player, enemy, wood, steel, none
	int probablityOfMapItems[6] = { 0, 0, 0, 15, 10, 75 };
	
	// Fill rest of the map
	int itemIndex = 0;
	for (size_t i = 0; i < MATRIX_WIDTH; i++)
	{

		for (size_t j = 0; j < MATRIX_HEIGHT; j++)
		{
			int percentage = rand() % 100;
			int itemIndex = 0;

			if (percentage > 95)
			{
				itemIndex = 3;
			}
			else if (percentage > 60 && percentage <= 95)
			{
				itemIndex = 4;
			}
			else
			{
				itemIndex = 5;
			}

			_gameMatrix[j][i] = _symbols[itemIndex];
		}
	}

	int randEnemyPosX = 0;
	int randEnemyPosY = 0;
	// Spawn Enemies
	for (size_t i = 0; i < _currentEnemyNumber; i++)
	{
		// Choose available cell
		do {
			randEnemyPosX = (rand() % (MATRIX_WIDTH - 2)) + 1;
			randEnemyPosY = (rand() % (MATRIX_HEIGHT - 2)) + 1;
		} while (_gameMatrix[randEnemyPosX][randEnemyPosY] != 'n');
		// Place enemy to the game matrix
		_gameMatrix[randEnemyPosX][randEnemyPosY] = 'e';
	}
	
	// Spawn Flag
	int randFlagPosX = 0;
	int randFlagPosY = 0;
	do {
		randFlagPosX = (rand() % (MATRIX_WIDTH - 2)) + 1;
		randFlagPosY = (rand() % (MATRIX_HEIGHT - 2)) + 1;
	} while (_gameMatrix[randFlagPosX][randFlagPosY] != 'n');

	_gameMatrix[randFlagPosX][randFlagPosY] = 'f';

	// Borders
	// Top Border
	for (size_t i = 0; i < MATRIX_WIDTH; i++)
	{
		_gameMatrix[0][i] = 'x';
	}

	// Right Border
	for (size_t i = 0; i < MATRIX_WIDTH; i++)
	{
		_gameMatrix[MATRIX_HEIGHT - 1][i] = 'x';
	}

	// Left Border
	for (size_t i = 0; i < MATRIX_HEIGHT; i++)
	{
		_gameMatrix[i][0] = 'x';
	}

	// Bottom Border
	for (size_t i = 0; i < MATRIX_HEIGHT; i++)
	{
		_gameMatrix[i][MATRIX_HEIGHT - 1] = 'x';
	}

	// Locate the player
	_gameMatrix[7][7] = 'p';
}

//delete
void SetRandomDirection(Sprite* sprite)
{
	int randomDirection = rand() % 4 + 1;
	sprite->direction = randomDirection;
	switch (randomDirection)
	{
	case 1: // Up
		sprite->SetVelocity(0, -1);
		break;
	case 2: // Right
		sprite->SetVelocity(1, 0);
		break;
	case 3: // Down
		sprite->SetVelocity(0, 1);
		break;
	case 4: // Left
		sprite->SetVelocity(-1, 0);
		break;
	default:
		break;
	}
}

void PaintLevel()
{
	bool enemyType = false;
	Sprite* pSprite;

	for (size_t posX = 0; posX < MATRIX_WIDTH; posX++)
	{
		for (size_t posY = 0; posY < MATRIX_HEIGHT; posY++)
		{
			//Seed the random number generator
			srand(GetTickCount());
			if (_gameMatrix[posX][posY] != 'n' && _gameMatrix[posX][posY] != 'p')
			{
				switch (_gameMatrix[posX][posY])
				{
				case 'e':
					if (enemyType) {
						pSprite = new Enemy(_enemyBitmapUp, _enemyBitmapRight, _enemyBitmapDown, _enemyBitmapLeft, rcBounds, BA_WRAP);
						((Enemy*)pSprite)->_movementType = 1; //0 random, 1 focused
					}
					else {
						pSprite = new Enemy(_enemy2BitmapUp, _enemy2BitmapRight, _enemy2BitmapDown, _enemy2BitmapLeft, rcBounds, BA_WRAP);
						((Enemy*)pSprite)->_movementType = 0; //0 random, 1 focused
					}
					enemyType = !enemyType;
					pSprite->SetTag(TAG_ENEMY);
					pSprite->SetZOrder(2);


					//SetRandomDirection(pSprite);
					UpdateEnemySprite(((Enemy*)pSprite));
					_gameMatrix[posX][posY] = 'n';
					((Enemy*)(pSprite))->_enemyLives = 1;
					((Enemy*)(pSprite))->_enemyID = _enemyIDs;
					((Enemy*)(pSprite))->_enemyMatrixPosX = posX;
					((Enemy*)(pSprite))->_enemyMatrixPosY = posY;
					_enemies.push_back((Enemy*)(pSprite));
					_enemyIDs++;
					break;
				case 'w':
					pSprite = new Environment(_woodBitmap, rcBounds, BA_WRAP);
					pSprite->SetTag(TAG_WOOD);
					pSprite->groupTag = TAG_ENV;
					pSprite->SetZOrder(2);
					((Environment*)(pSprite))->_envHealth = 2;
					break;
				case 's':
					pSprite = new Environment(_steelBitmap, rcBounds, BA_WRAP);
					pSprite->SetTag(TAG_STEEL);
					pSprite->groupTag = TAG_ENV;
					pSprite->SetZOrder(2);
					((Environment*)(pSprite))->_envHealth = 10;
					break;
				case 'b':
					pSprite = new Sprite(_baseBitmap, rcBounds, BA_WRAP);
					pSprite->SetTag(TAG_BASE);
					pSprite->groupTag = TAG_ENV;
					pSprite->SetZOrder(2);
					break;
				case 'x':
					pSprite = new Sprite(_borderBitmap, rcBounds, BA_WRAP);
					pSprite->SetTag(TAG_BORDER);
					pSprite->groupTag = TAG_ENV;
					pSprite->SetZOrder(2);
					break;
				case 'f':
					pSprite = new Sprite(_flagBitmap, rcBounds, BA_WRAP);
					pSprite->SetTag(TAG_FLAG);
					pSprite->groupTag = TAG_FLAG;
					pSprite->SetZOrder(2);
				}

				pSprite->SetPosition(_cellWidth * posX + GAMEAREA_MARGIN_X / 2, _cellHeight * posY + GAMEAREA_MARGIN_Y / 2);
				_pGame->AddSprite(pSprite);
			}
		}
	}

	// Add player to the scene
	_player = new Player(_playerBitmapUp, rcBounds, BA_STOP);

	//2,4 20/20 -> 650,650
	_player->_playerScore = 0;
	_player->_playerTotal = 0;
	_player->_killCount = 0;
	_player->_flagCount = 0;
	_player->_playerLives = 15;
	_player->_playerMatrixPosX = 7;
	_player->_playerMatrixPosY = 7;

	_player->direction = 1;
	_player->SetPosition(GAMEAREA_MARGIN_X / 2 + SPRITE_SIZE * _player->_playerMatrixPosX, GAMEAREA_MARGIN_Y / 2 + SPRITE_SIZE * _player->_playerMatrixPosY);
	_player->SetVelocity(0, 0);
	_player->SetZOrder(2);
	_player->SetTag(TAG_PLAYER);
	_pGame->AddSprite(_player);

	_gameMatrix[7][7] = 'n';
	// Add borders to the scene

}

void MouseButtonDown(int x, int y, BOOL bLeft)
{
	//Start a new game, if necessary
	if (_bGameOver)
	{
		//Restart the background music
		_pGame->PlayMIDISong();

		//Initialize the game variables
		//_iNumLives = 3;
		//_iScore = 0;
		_bGameOver = FALSE;
	}
}

void MouseButtonUp(int x, int y, BOOL bLeft)
{
}

void MouseMove(int x, int y)
{
}

void SmallExplosion(RECT rcPosition)
{
	// Create a small explosion sprite at the missile's position
	RECT rcPos = rcPosition;
	Sprite* explosionSprite = new Sprite(_pSmExplosionBitmap, rcBounds);
	explosionSprite->SetNumFrames(8, TRUE);
	explosionSprite->SetPosition(rcPos.left, rcPos.top);
	_pGame->AddSprite(explosionSprite);
}

void spawnNewFlag(Sprite* eSprite) {

	// Delete last flag
	for (int i = 0; i < MATRIX_WIDTH; i++)
	{
		for (int j = 0; j < MATRIX_WIDTH; j++)
		{
			if (_gameMatrix[i][j] == 'p') {
				_lastSpawnedFlagX = i;
				_lastSpawnedFlagY = j;

			}
		}
	}


	//give point to player
	_player->_playerScore += 50 + 50 * (_gameRound * 0.2);
	_player->_playerTotal += 50 + 50 * (_gameRound * 0.2);
	_player->_flagCount += 1;
	_player->_ammo += 15;
	if (_player->_playerScore >= 500)
	{
		_player->_playerScore -= 500;
		_player->_playerLives++;
	}

	// Spawn new Steel
	int randomSteel = (rand() % 4) + 3;
	for (int i = 0; i < randomSteel; i++) {
		srand(GetTickCount());
		int randFlagPosX = 0;
		int randFlagPosY = 0;

		Sprite* pSprite = new Environment(_steelBitmap, rcBounds, BA_WRAP);
		pSprite->SetTag(TAG_STEEL);
		pSprite->groupTag = TAG_ENV;
		pSprite->SetZOrder(2);
		((Environment*)(pSprite))->_envHealth = 10;

		do {
			randFlagPosX = (rand() % (MATRIX_WIDTH - 2)) + 1;
			randFlagPosY = (rand() % (MATRIX_HEIGHT - 2)) + 1;
		} while (_gameMatrix[randFlagPosX][randFlagPosY] != 'n');

		_gameMatrix[randFlagPosX][randFlagPosY] = 's';
		pSprite->SetPosition(_cellWidth * randFlagPosX + GAMEAREA_MARGIN_X / 2, _cellHeight * randFlagPosY + GAMEAREA_MARGIN_Y / 2);
		_pGame->AddSprite(pSprite);
	}


	// Spawn new Flag
	int randFlagPosX = 0;
	int randFlagPosY = 0;
	do {
		randFlagPosX = (rand() % (MATRIX_WIDTH - 2)) + 1;
		randFlagPosY = (rand() % (MATRIX_HEIGHT - 2)) + 1;
	} while ((_gameMatrix[randFlagPosX][randFlagPosY] != 'n') || ((randFlagPosX == _lastSpawnedFlagX && randFlagPosY == _lastSpawnedFlagY)
		|| (randFlagPosX == _lastSpawnedFlagX + 1 && randFlagPosY == _lastSpawnedFlagY) || (randFlagPosX == _lastSpawnedFlagX - 1 && randFlagPosY == _lastSpawnedFlagY)
		|| (randFlagPosX == _lastSpawnedFlagX && randFlagPosY == _lastSpawnedFlagY - 1) || (randFlagPosX == _lastSpawnedFlagX && randFlagPosY == _lastSpawnedFlagY + 1)));

	eSprite->SetPosition(_cellWidth * randFlagPosX + GAMEAREA_MARGIN_X / 2, _cellHeight * randFlagPosY + GAMEAREA_MARGIN_Y / 2);
	_gameMatrix[randFlagPosX][randFlagPosY] = 'f';
	//_pGame->AddSprite(eSprite);

}

void LargeExplosion(RECT rcPosition)
{
	Sprite* pSprite = new Sprite(_pLgExplosionBitmap, rcBounds);
	pSprite->SetNumFrames(8, TRUE);
	pSprite->SetPosition(rcPosition.left, rcPosition.top);
	_pGame->AddSprite(pSprite);
}

void StartMusic()
{
	//Start playing game music
	_soundtrackNumber = rand() % 4;

	if (_soundtrackNumber == 0) {
		_pGame->PlayMIDISong(TEXT("Music.mid"));
	}
	else if (_soundtrackNumber == 1) {
		_pGame->PlayMIDISong(TEXT("Music2.mid"));
	}
	else if (_soundtrackNumber == 2) {
		_pGame->PlayMIDISong(TEXT("Music1.mid"));
	}
	else if (_soundtrackNumber == 3) {
		_pGame->PlayMIDISong(TEXT("Music3.mid"));
	}
}

void InitGameVariables()
{
	_cellWidth = (SCREEN_WIDTH - GAMEAREA_MARGIN_X) / MATRIX_WIDTH;
	_cellHeight = (SCREEN_HEIGHT - GAMEAREA_MARGIN_Y) / MATRIX_HEIGHT;
	//_playerSpeed = 5;

	_flagCounter = FLAG_START;
	_flagIncrease = 0;

	_symbols[0] = 'b';
	_symbols[1] = 'p';
	_symbols[2] = 'e';
	_symbols[3] = 'w';
	_symbols[4] = 's';
	_symbols[5] = 'n';
	//_enemyperLevel = 10;
	_enemyIDs = 0;
	_currentEnemyNumber = ENEMYNUMBER;
	_timer = 0;
	_bfrm = 0;

	_iInputDelay = 0;
	_FireDelay = 7;
	_bGameOver = FALSE;
}

void NextLevel()
{
	GenerateNextLevel();
	//_flagIncrease++;
	_flagCounter = FLAG_START;
}

void GenerateNextLevel()
{
	_gameRound++;

	int flagX = _lastSpawnedFlagX;
	int	flagY = _lastSpawnedFlagY;
	_player->_playerScore += 50;
	_player->_playerTotal += 250;
	_player->_flagCount++;

	if (_lastSpawnedFlagX > -5) {
		_lastSpawnedFlagY = _lastSpawnedFlagY;
	}

	// Empty the game matrix
	for (int i = 1; i < MATRIX_WIDTH - 1; i++)
	{
		for (int j = 1; j < MATRIX_HEIGHT - 1; j++)
		{
			if (_gameMatrix[i][j] != 'p' && _gameMatrix[i][j] != 'x' && _gameMatrix[i][j] != 'e')
			{
				_gameMatrix[i][j] = 'n';
			}
		}
	}

	/*

	int rX;
	int rY;

	for (int i = 0; i < _enemies.size(); i++)
	{
		do {
			rX = (rand() % (MATRIX_WIDTH - 2)) + 1;
			rY = (rand() % (MATRIX_HEIGHT - 2)) + 1;
		} while (_gameMatrix[rX][rY] != 'n');

		_enemies.at(i)->SetPosition(_cellWidth * rX + GAMEAREA_MARGIN_X / 2, _cellHeight * rY + GAMEAREA_MARGIN_Y / 2);
	}

	*/

	// Change position of the all items
	vector<Sprite*>::iterator siSprite;

	for (siSprite = _pGame->m_vSprites.begin(); siSprite != _pGame->m_vSprites.end(); /* siSprite++ */)
	{
		Sprite* sprite = (*siSprite);
		int tag = (*siSprite)->GetTag();
		int groupTag = (*siSprite)->groupTag;
		int randPosX = 0;
		int randPosY = 0;

		if (tag == TAG_FLAG)
		{
			/*do {
				randPosX = (rand() % (MATRIX_WIDTH - 2)) + 1;
				randPosY = (rand() % (MATRIX_HEIGHT - 2)) + 1;
			} while (_gameMatrix[randPosX][randPosY] != 'n');
			_gameMatrix[randPosX][randPosY] = 'f';
			flagX = randPosX;
			flagY = randPosY;
			(*siSprite)->SetPosition(_cellWidth * randPosX + GAMEAREA_MARGIN_X / 2, _cellHeight * randPosY + GAMEAREA_MARGIN_Y / 2);*/
		}

		else if (tag == TAG_WOOD || tag == TAG_STEEL) {

			do {
				randPosX = (rand() % (MATRIX_WIDTH - 2)) + 1;
				randPosY = (rand() % (MATRIX_HEIGHT - 2)) + 1;
			} while ((_gameMatrix[randPosX][randPosY] != 'n') || ((randPosX == _lastSpawnedFlagX && randPosY == _lastSpawnedFlagY)
				|| (randPosX == _lastSpawnedFlagX + 1 && randPosY == _lastSpawnedFlagY) || (randPosX == _lastSpawnedFlagX - 1 && randPosY == _lastSpawnedFlagY)
				|| (randPosX == _lastSpawnedFlagX && randPosY == _lastSpawnedFlagY - 1) || (randPosX == _lastSpawnedFlagX && randPosY == _lastSpawnedFlagY + 1)));

			if (tag == TAG_WOOD)
			{
				_gameMatrix[randPosX][randPosY] = 'w';
				((Wood*)&siSprite)->_matrixPosX = randPosX;
				((Wood*)&siSprite)->_matrixPosY = randPosY;
			}
			else if (tag == TAG_STEEL)
			{
				_gameMatrix[randPosX][randPosY] = 's';
				((Steel*)&siSprite)->_matrixPosX = randPosX;
				((Steel*)&siSprite)->_matrixPosY = randPosY;
				((Steel*)&siSprite)->_envHealth = 6;
			}

			(*siSprite)->SetPosition(_cellWidth * randPosX + GAMEAREA_MARGIN_X / 2, _cellHeight * randPosY + GAMEAREA_MARGIN_Y / 2);
		}

		siSprite++;
	}

	_lastSpawnedFlagX = flagX;
	_lastSpawnedFlagY = flagY;

	if (_gameRound % 3 == 0) {
		spawnEnemy(0);
	}

	if (_gameRound % 2 == 0) {
		spawnEnemy(1);
	}
	
}

void InitBitmaps(HDC hDC)
{
	//Create and load the bitmaps

	RECT rcbounds1 = { 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT };
	rcBounds = rcbounds1;

	// Initialize Bitmaps
	_pBackgroundBitmap = new Bitmap(hDC, IDB_BACKGROUND, _hInstance);
	_playerBitmapUp = new Bitmap(hDC, IDB_PLAYERUP, _hInstance);
	_playerBitmapRight = new Bitmap(hDC, IDB_PLAYERRIGHT, _hInstance);
	_playerBitmapDown = new Bitmap(hDC, IDB_PLAYERDOWN, _hInstance);
	_playerBitmapLeft = new Bitmap(hDC, IDB_PLAYERLEFT, _hInstance);
	_woodBitmap = new Bitmap(hDC, IDB_BLOCKWOOD, _hInstance);
	_steelBitmap = new Bitmap(hDC, IDB_BLOCKSTEEL, _hInstance);
	_baseBitmap = new Bitmap(hDC, IDB_TANK_BLUE_UP, _hInstance);
	_emptyBitmap = new Bitmap(hDC, IDB_TILEBLACK, _hInstance);
	_enemyBitmapUp = new Bitmap(hDC, IDB_TANK_RED_UP, _hInstance);
	_enemyBitmapRight = new Bitmap(hDC, IDB_TANK_RED_RIGHT, _hInstance);
	_enemyBitmapDown = new Bitmap(hDC, IDB_TANK_RED_DOWN, _hInstance);
	_enemyBitmapLeft = new Bitmap(hDC, IDB_TANK_RED_LEFT, _hInstance);
	_enemy2BitmapUp = new Bitmap(hDC, IDB_TANK_GREY_UP, _hInstance);
	_enemy2BitmapRight = new Bitmap(hDC, IDB_TANK_GREY_RIGHT, _hInstance);
	_enemy2BitmapDown = new Bitmap(hDC, IDB_TANK_GREY_DOWN, _hInstance);
	_enemy2BitmapLeft = new Bitmap(hDC, IDB_TANK_GREY_LEFT, _hInstance);
	_bulletBitmap = new Bitmap(hDC, IDB_BULLET, _hInstance);
	_borderBitmap = new Bitmap(hDC, IDB_TILEBORDER, _hInstance);
	_pLgExplosionBitmap = new Bitmap(hDC, IDB_LGEXPLOSION, _hInstance);
	_pSmExplosionBitmap = new Bitmap(hDC, IDB_SMEXPLOSION, _hInstance);
	_gameOverBitmap = new Bitmap(hDC, IDB_GAMEOVER, _hInstance);
	_flagBitmap = new Bitmap(hDC, IDB_FLAG, _hInstance);
}

void spawnEnemy(int enemyType) {

	Sprite* pSprite;


	//Seed the random number generator
	int posX = 0;
	int posY = 0;

	do {
		srand(GetTickCount());
		posX = rand() % MATRIX_WIDTH;
		posY = rand() % MATRIX_HEIGHT;
	} while (_gameMatrix[posX][posY] != 'n');


	if (enemyType) {
		pSprite = new Enemy(_enemyBitmapUp, _enemyBitmapRight, _enemyBitmapDown, _enemyBitmapLeft, rcBounds, BA_WRAP);
		((Enemy*)pSprite)->_movementType = 1; //0 random, 1 focused
	}
	else {
		pSprite = new Enemy(_enemy2BitmapUp, _enemy2BitmapRight, _enemy2BitmapDown, _enemy2BitmapLeft, rcBounds, BA_WRAP);
		((Enemy*)pSprite)->_movementType = 0; //0 random, 1 focused
	}
	enemyType = !enemyType;
	pSprite->SetTag(TAG_ENEMY);
	pSprite->SetZOrder(2);

	//SetRandomDirection(pSprite);
	UpdateEnemySprite(((Enemy*)pSprite));
	_gameMatrix[posX][posY] = 'n';
	((Enemy*)(pSprite))->_enemyLives = 1;
	((Enemy*)(pSprite))->_enemyID = _enemyIDs;
	((Enemy*)(pSprite))->_enemyMatrixPosX = posX;
	((Enemy*)(pSprite))->_enemyMatrixPosY = posY;
	_enemies.push_back((Enemy*)(pSprite));
	_enemyIDs++;


	pSprite->SetPosition(_cellWidth * posX + GAMEAREA_MARGIN_X / 2, _cellHeight * posY + GAMEAREA_MARGIN_Y / 2);
	_pGame->AddSprite(pSprite);
}