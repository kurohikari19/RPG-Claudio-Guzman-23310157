//
// Created by Victor Navarro on 15/02/24.
//
#pragma once
#ifndef RPG_PLAYER_H
#define RPG_PLAYER_H
#include "../Character/Character.h"
#include "../Enemy/Enemy.h"
#include "../Combat/Action.h"
#include <vector>

class Enemy;

class Player: public Character {
    //TODO: Implement Classes (Mage, Warrior, Rogue, etc..)
    //TODO: Implement Inventory
private:
    int level;
    int experience;

public:
    Player(string _name, int _health, int _attack, int _defense, int _speed);
    void doAttack(Character *target) override;
    void takeDamage(int damage) override;
    Character* selectTarget(vector<Enemy*> possibleTargets);
    Action takeAction(vector<Enemy*> enemies, vector<Player*>& partyMembers, int totalEnemyExperience);

    void doDefend();

    void gainExperience(int exp); // Método para sumar la experiencia ganada
    int getExperience() const; // Método para obtener la experiencia total del jugador
    void checkExperience();

    void checkLevelUp(); // Método para verificar si el jugador debe subir de nivel

    void levelUp(); // Actualizar para mostrar el menú y asignar puntos a las estadísticas

    void levelUp(vector<Enemy*>& enemies); // Sobrecarga de la función levelUp() para aumentar estadísticas de los enemigos

    void checkEnemyStats(const vector<Enemy*>& enemies);
    std::vector<Enemy*> enemies;

};


#endif //RPG_PLAYER_H
