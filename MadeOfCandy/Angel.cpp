#include "Angel.h"

Angel::Angel(float pathX, float pathY, Game* game)
	: Actor("res/characters/Angel/Robbie/Robbie.png", 30, 46,pathX, pathY, game) {


	canShoot = true;
	ray = nullptr;


	isMoving = false;
	movedElement = nullptr;

	audioRay = new Audio("res/music/effects/Ray.wav", false);

	aLeft= new Animation("res/characters/Angel/Robbie/Robbie_left.png", getWidth(), getHeight(),
		96, 50, 6, 3, true,game);

	aLeftUp= new Animation("res/characters/Angel/Robbie/Robbie_leftUp.png", getWidth(), getHeight(),
		96, 50, 6, 3, true, game);

	aRight = new Animation("res/characters/Angel/Robbie/Robbie_right.png", getWidth(), getHeight(),
		96, 50, 6, 3, true, game);
	aRightUp = new Animation("res/characters/Angel/Robbie/Robbie_rightUp.png", getWidth(), getHeight(),
		96, 50, 6, 3, true, game);
	animation = aLeft;
}

void Angel::update() {
	animation->update();

	if (ray != nullptr) {
		ray->update();
		deleteRay();
	}
}

void Angel::moveX(float axis) {
	if (axis > 0 && getPathX() == PATHS_X-1) {
		
	}
	else if (axis < 0 && getPathX() == 1) {
		
	}
	else {
		if (axis < 0) {
			animation = aLeft;
		}
		else {
			animation = aRight;
		}

		incrementX(axis);
		if (isMoving) {
			movedElement->move(axis, 0);
		}
	}
	
}

void Angel::moveY(float axis) {
	if (axis > 0 && getPathY() == PATHS_Y) {
	}
	else if (axis < 0 && getPathY() == 1) {
	}
	else {
		if (axis < 0) {
			if (animation == aRight || animation == aRightUp) {
				animation = aRightUp;
			}
			else {
				animation = aLeftUp;
			}
		}
		else {
			if (animation == aRightUp || animation == aRight) {
				animation = aRight;
			}
			else {
				animation = aLeft;
			}

		}
		incrementY(axis);

		if(isMoving) {
			movedElement->move(0, axis);
		}
	}
	
}

void Angel::shoot(bool shoot) {
	if (shoot && canShoot) {
		audioRay->play();
		cantShoot();
		if (animation == aLeft || animation == aLeftUp) {
			ray = new CelestialRay(getPathX(), getPathY(), 30,true, getGame());
		}
		else {
			ray = new CelestialRay(getPathX(), getPathY(), 30,false, getGame());
		}
	}
}

CelestialRay* Angel::getRay() {
	return ray;
}

void Angel::cantShoot() {
	canShoot = false;
}

void Angel::deleteRay() {
	if (ray->isInRender()==false) {
		delete ray;
		ray = nullptr;
	}
}

void Angel::moveElement(bool move, list<Movable*> movables) {
	if (move && !isMoving) {
			isMoving = true;
			findMovedElement(movables);
	}

	else if (move && isMoving) {
			isMoving = false;
			movedElement = nullptr;
	}
}

void Angel::findMovedElement(list<Movable*> movables) {
	int pathX = getPathX();
	int pathY = getPathY();
	int orientation = 0;
	if (animation == aRight || animation == aRightUp) {
		orientation = 1;
	}
	else {
		orientation = -1;
	}
	for (auto const& movable : movables) {
		if (movable->getPathY() == pathY) {
			if (movable->getPathX() == pathX) {
				cout << "mismo X";
				movedElement = movable;
				return;
			}
			else if (movable->getPathX() == pathX + orientation) {
				movedElement = movable;
			}
		}

	}//for

	if (movedElement == nullptr) {
		isMoving = false;
	}
}

void Angel::draw() {
	animation->draw(x, y);
	if (ray != nullptr) {
		ray->draw();
	}

}



