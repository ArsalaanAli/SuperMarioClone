#include "Player.h"
#include "Enemy.h"
#include "SFML/Graphics/Drawable.hpp"
#include <vector>

typedef sf::Drawable Entity;

class EntityManager {
public:
  EntityManager(sf::Image collisionMap);
  EntityManager(EntityManager &&) = default;
  EntityManager(const EntityManager &) = default;
  EntityManager &operator=(EntityManager &&) = default;
  EntityManager &operator=(const EntityManager &) = default;
  ~EntityManager();

  Player* getPlayer();
  vector<Enemy>* getEnemies();
  bool checkCollisionMap(int x, int y);

  void update(GameState &state);
  void resetEntities();

  Player* spawnPlayer(int cx, int cy);
  Enemy* spawnEnemy(int cx, int cy);
private:
  Player player;
  vector<Enemy> enemies;
  vector<Entity> entities;

  sf::Image cmap;
};

