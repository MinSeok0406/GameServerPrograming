#include "BulletObject.h"

BulletObject::BulletObject(int x, int y, OBJECT_TYPE objType)
	: _x(x), _y(y), IBaseObject(objType)
{
}

BulletObject::~BulletObject()
{
}

bool BulletObject::Update()
{
	if (this->GetObjectType() == OBJECT_TYPE::PLAYER_BULLET)
	{
		//Playerbullet.push_back({ p->x, p->y - 1 });

	  /*// 몬스터 피격 판정
		int y = 0;
		int x = 0;
		for (int i = 0; i < (int)Playerbullet.size(); ++i)
		{
			for (int j = 0; j < (int)enemies.size(); ++j)
			{
				if (!enemies[j].live)
				{
					continue;
				}

				y = Playerbullet[i].y - 1;
				x = Playerbullet[i].x;
				if (enemies[j].x == x && enemies[j].y == y)
				{
					Playerbullet[i].y = -1;
					Playerbullet[i].x = -1;
					enemies[j].live = false;
				}
			}
		}*/
	}
	else if (this->GetObjectType() == OBJECT_TYPE::ENEMY_BULLET)
	{
		// 플레이어 피격 판정
		/*int y = 0;
		int x = 0;
		for (int i = 0; i < (int)Enemybullet.size(); ++i)
		{
			y = Enemybullet[i].y + 1;
			x = Enemybullet[i].x;
			if (p->x == x && p->y == y)
			{
				Enemybullet[i].x = -1;
				Enemybullet[i].y = -1;
				p->hp -= 20;
				if (p->hp <= 0)
				{
					p->live = false;
				}
			}
		}*/
	}

	return false;
}

void BulletObject::Render()
{
	if (this->GetObjectType() == OBJECT_TYPE::PLAYER_BULLET)
	{
		// 총알 이동
		/*for (int i = 0; i < (int)Playerbullet.size(); ++i)
		{
			if (Playerbullet[i].x != -1 && Playerbullet[i].y != -1)
			{
				g_screenBuffer->Sprite_Draw(Playerbullet[i].x, Playerbullet[i].y, '^');
				Playerbullet[i].y -= 1;

				if (Playerbullet[i].y < 0)
				{
					Playerbullet[i].y = -1;
					Playerbullet[i].x = -1;
				}
			}
		}*/
	}
	else if (this->GetObjectType() == OBJECT_TYPE::ENEMY_BULLET)
	{
		// 적 총알 이동
		/*for (int i = 0; i < (int)Enemybullet.size(); ++i)
		{
			if (Enemybullet[i].x != -1 && Enemybullet[i].y != -1)
			{
				g_screenBuffer->Sprite_Draw(Enemybullet[i].x, Enemybullet[i].y, 'v');
				Enemybullet[i].y += 1;

				if (Enemybullet[i].y >= dfSCREEN_HEIGHT)
				{
					Enemybullet[i].x = -1;
					Enemybullet[i].y = -1;
				}
			}
		}*/
	}
}

bool BulletObject::RemoveObject()
{
	return false;
}

bool BulletObject::OnCollision(IBaseObject* obj)
{
	return false;
}
