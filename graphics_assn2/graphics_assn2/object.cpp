#include "rect.h"
#include "character.h"

//두 오브젝트 a, b가 충돌하는지 여부를 반환한다.
bool object::collisionCheck(object* a) {
	//wall - player/thief collision check
	if (this->getType() == 0) {
		rect* wall = (rect*)this;
		character* ch = (character*)a;

		//Check if collision already occured
		if ((wall->getX() < ch->getX()) && !ch->getCollided()) {
			ch->setCollided();
			return true;
		}
		return false;
	}

	//player - thief collision check
	else {
		character* player = (character*)this;
		character* thief = (character*)a;

		return (player->getX() + player->getRad()) > (thief->getX() - thief->getRad());
	}
}