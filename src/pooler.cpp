//Copyright 2K Games, Engineering Graduate Program. All rights reserved. DO NOT redistribute.

#include "pooler.h"

#include <algorithm>
//included for the std::cerr output in SpawnEnemy
#include <iostream>

Pooler::Pooler(size_t minimumPoolSize) : minimumPoolSize(minimumPoolSize) {
}

size_t Pooler::PoolSize()
{
    return enemies.size();
}

Enemy* Pooler::CreateEnemy() {
    return new Enemy();
}

void Pooler::Clear() {
    for (Enemy* enemy : enemies) {
        delete enemy;
    }
    enemies.clear();
}

Enemy* Pooler::SpawnEnemy(const Vector2D& spawnPosition) {
    // Find an inactive enemy in the pool
    for (auto it = enemies.begin(); it != enemies.end(); ++it) {
        if (!(*it)->IsActive()) {
            // Activate the found enemy
            (*it)->Activate(spawnPosition);
            return *it;
        }
    }

    // No inactive enemy found, create a new one and add it to the pool
    Enemy* newEnemy = CreateEnemy();
    if (newEnemy) {
        newEnemy->Activate(spawnPosition);
        enemies.push_back(newEnemy);
        return newEnemy;
    } else {
        std::cerr << "Failed to create a new enemy." << std::endl;
        return nullptr;
    }

}

void Pooler::DespawnEnemy(Enemy* enemy) {
    if (enemy) {
        enemy->Deactivate();
    }
}

std::vector<Enemy*> Pooler::GetActiveEnemies(){
    std::vector<Enemy*> activeEnemies;

    //Iterate through the pool and count the active enemies
    for (auto it = enemies.begin(); it != enemies.end(); ++it) {
        if ((*it)->IsActive()) {
            activeEnemies.push_back(*it);
        }
    }
    return activeEnemies;
}
