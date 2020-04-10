#include "rect.h"
#include "character.h"

//�� ������Ʈ a, b�� �浹�ϴ��� ���θ� ��ȯ�Ѵ�.
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