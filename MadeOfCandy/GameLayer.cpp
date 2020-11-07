
#include "GameLayer.h"

GameLayer::GameLayer(Game* game) : Layer(game) {
	init();
}

void GameLayer::init() {
	angel = new Angel(100, 100, getGame());
	girl = new TheGirl(50, 50, 60,1, getGame());
	background = new Background("res/world/City.png", WIDTH * 0.5, HEIGHT * 0.5, getGame());

	textLifes = new Text("", WIDTH * 0.92, HEIGHT * 0.04, getGame());
	textLifes->content = to_string(girl->getLife());
	lifes = new Actor("res/icons/heart.png",
		WIDTH * 0.85, HEIGHT * 0.05, 24, 24, getGame());

	controlShoot = false;
	controlMoveElement = false;
	controlMoveY = 0;
	controlMoveX = 0;

	enemies.clear(); 
	enemies.push_back(new Blob(300, 50, getGame()));
	enemies.push_back(new Blob(300, 100, getGame()));
	enemies.push_back(new Blob(300, 150, getGame()));
	enemies.push_back(new Blob(300, 200, getGame()));
	enemies.push_back(new Blob(300, 250, getGame()));
	enemies.push_back(new Blob(300, 300, getGame()));

}

void GameLayer::processControls() {
	SDL_Event event;
	while (SDL_PollEvent(&event)) {
		keysToControls(event);
	}
	angel->shoot(controlShoot);
//	angel->moveElement(controlMoveElement);
	if (controlMoveX > 0 || controlMoveX < 0) {
		angel->moveX(controlMoveX);
		controlMoveX = 0;
	}

	if (controlMoveY > 0 || controlMoveY < 0) {
		angel->moveY(controlMoveY);
		controlMoveY = 0;
	}


}

void GameLayer::update() {
	angel->update();
	girl->update();

	for (auto const& enemy : enemies) {
		enemy->update();
	}

	enemyColisions();

	cout << "update GameLayer" << endl;
}

void GameLayer::draw() {
	background->draw();

	lifes->draw();
	textLifes->draw();

	angel->drawAngel();
	girl->draw();

	for (auto const& enemy : enemies) {
		enemy->draw();
	}

	SDL_RenderPresent(getGame()->getRenderer()); 
}

void GameLayer::keysToControls(SDL_Event event) {
	if (event.type == SDL_QUIT) {
		getGame()->stopGame();
	}

	if (event.type == SDL_KEYDOWN) {
		int code = event.key.keysym.sym;

		switch (code) {
		case SDLK_ESCAPE:
			getGame()->stopGame();
			break;
		case SDLK_d: 
			controlMoveX = 1;
			break;
		case SDLK_a: 
			controlMoveX = -1;
			break;
		case SDLK_w: 
			controlMoveY = -1;
			break;
		case SDLK_s: 
			controlMoveY = 1;
			break;
		case SDLK_SPACE: 
			controlShoot = true;
			break;
		case SDLK_LSHIFT:
			controlMoveElement = true;
			break;
		}
	}
}

void GameLayer::enemyColisions() {
	list<Enemy*> deleteEnemies;

	//girl and enemy
	for (auto const& enemy : enemies) {
		if (girl->isOverlap(enemy)) {

			girl->loseLife(enemy->getDamage());
			textLifes->content = to_string(girl->getLife());
			if (girl->isDead()) {
				init();
			}

			bool eInList = std::find(deleteEnemies.begin(),
				deleteEnemies.end(),
				enemy) != deleteEnemies.end();

			if (!eInList) {
				deleteEnemies.push_back(enemy);
			}
		}
	}

	//enemy and ray
	if (angel->getRay() != nullptr) {
		for (auto const& enemy : enemies) {
			if (enemy->isOverlap(angel->getRay())) {
				bool eInList = std::find(deleteEnemies.begin(),
					deleteEnemies.end(),
					enemy) != deleteEnemies.end();

				if (!eInList) {
					deleteEnemies.push_back(enemy);
				}
			}
		}//for
	}//if


	for (auto const& delEnemy : deleteEnemies) {
		enemies.remove(delEnemy);
	}
	deleteEnemies.clear();

}



