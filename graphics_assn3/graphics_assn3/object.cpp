#include "object.h"
#include "Wall.h"
#include "character.h"

//Return whether this object and object 'a' collides.
bool object::collisionCheck(object* a) {
	//wall - player/thief collision check
	if (type == 0) {
		Wall* wall = (Wall*)this;
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

		return (player->getX()) + 15 > (thief->getX());
	}
}