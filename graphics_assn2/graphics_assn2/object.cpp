#include "rect.h"
#include "character.h"

//두 오브젝트 a, b가 충돌하는지 여부를 반환한다.
bool collisionCheck(object* a, object* b) {
	//wall - player/thief collision check
	if (a->getType() == 0) {
		rect* wall = (rect*)a;
		character* ch = (character*)b;

		//Check if collision already occured
		if ((wall->getX() < ch->getX()) && !ch->getCollided()) {
			ch->setCollided();
			return true;
		}
		return false;
	}

	//player - thief collision check
	else {
		character* player = (character*)a;
		character* thief = (character*)b;

		return (player->getX() + player->getRad()) > (thief->getX() - thief->getRad());
	}
}