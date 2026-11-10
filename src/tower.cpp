//Copyright 2K Games, Engineering Graduate Program. All rights reserved. DO NOT redistribute.

#include "tower.h"
#include "logger.h"

#include <limits>
#include <cmath>

Tower::Tower(int x, int y, int rateOfFire)
    : position({x, y}),
      rateOfFire(rateOfFire),
      lastFireTime(std::chrono::steady_clock::now()),
      bAutoFire(true)
    {}

void Tower::Update(std::vector<Enemy*> enemies,
                   std::vector<Bullet>& bullets) {
    if (bAutoFire) {
        FireAtEnemy(enemies, bullets);
    }
}

void Tower::FireAtEnemy(const std::vector<Enemy*>& enemies,
                        std::vector<Bullet>& bullets) {
    auto currentTime = std::chrono::steady_clock::now();
    auto timeElapsed =
        std::chrono::duration_cast<std::chrono::milliseconds>(currentTime - lastFireTime);

    if (timeElapsed >= std::chrono::milliseconds(rateOfFire)) {
        Enemy* closestEnemy = FindClosestEnemy(enemies);

        if (bAutoFire) {
            int distance = CalculateDistance(closestEnemy->GetPosition(), position);

            if (distance <= ENEMYRANGE) {
                FireBulletAtEnemy(*closestEnemy, bullets);
                lastFireTime = currentTime;
            }
        } else {
            FireBulletAtEnemy(*closestEnemy, bullets);
            lastFireTime = currentTime;
        }
    }
}

bool Tower::GetAutoFire() const { return bAutoFire; }

void Tower::SetAutoFire(bool autoFire) { bAutoFire = autoFire; }

void Tower::ManualFire(std::vector<Enemy*> enemies,
                       std::vector<Bullet>& bullets) {
    FireAtEnemy(enemies, bullets);
}

void Tower::FireBulletAtEnemy(Enemy& enemy, std::vector<Bullet>& bullets) {
    Vector2D direction = {0, 0};
    if (enemy.GetPosition().x > position.x) direction.x = 1;
    if (enemy.GetPosition().x < position.x) direction.x = -1;
    if (enemy.GetPosition().y > position.y) direction.y = 1;
    if (enemy.GetPosition().y < position.y) direction.y = -1;

    bullets.emplace_back(position, direction);
}

Enemy* Tower::FindClosestEnemy(const std::vector<Enemy*>& enemies) {
    Enemy* closestEnemy = nullptr;
    int closestDistance = INT_MAX;

    for (Enemy* enemy : enemies) {
        if (enemy->IsActive()) {
            int distance = CalculateDistance(enemy->GetPosition(), position);
            if (distance < closestDistance) {
                closestDistance = distance;
                closestEnemy = enemy;
            }
        }
    }

    return closestEnemy;
}

// Calculate the distance between two positions (not going to lie, this one I needed a refresher on)
int Tower::CalculateDistance(const Vector2D& pos1, const Vector2D& pos2) {
    int dx = pos1.x - pos2.x;
    int dy = pos1.y - pos2.y;
    return static_cast<int>(std::sqrt(dx * dx + dy * dy));
}