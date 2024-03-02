#include "EntityManager.h"
#include <iostream>

EntityManager::EntityManager(sf::Image collisionMap) { cmap = collisionMap; }

EntityManager::~EntityManager() {}

Player *EntityManager::getPlayer() { return &player; }

vector<Enemy>* EntityManager::getEnemies() { return &enemies; }

void EntityManager::update(GameState &state) {
  player.update(state);

  for (auto &enemy : enemies) {
    enemy.update(state);

    sf::Vector2f pos = player.getShape().getPosition();
    sf::Vector2f epos = enemy.getShape().getPosition();

    if (enemy.checkPlayerCollision(pos.x, pos.y)) {
      if (pos.y - epos.y < -(CELL_SIZE / 2)) {
        enemy.die();
        player.jump();
      } else {
        player.die();
      }
    } else {
    }
  }
}

Player *EntityManager::spawnPlayer(int cx, int cy) {
  Player p = Player(cx, cy);
  player = p;
  return &player;
}

Enemy *EntityManager::spawnEnemy(int cx, int cy) {
  Enemy e = Enemy(cx, cy);
  enemies.push_back(e);
  cout << enemies.size() << endl;
  return &(enemies.back());
}

bool EntityManager::checkCollisionMap(int x, int y) {
  sf::Vector2u size = cmap.getSize();
  if (x > 0 && x < size.x && y > 0 && y < size.y)
    return cmap.getPixel(x, y) == sf::Color::Red;
  return false;
}

void EntityManager::resetEntities() {
  spawnPlayer(0, 0);
  for (auto &enemy : enemies) {
    enemy.reset();
  }
}
