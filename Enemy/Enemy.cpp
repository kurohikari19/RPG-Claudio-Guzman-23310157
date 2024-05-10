//
// Created by Victor Navarro on 15/02/24.
//

#include "Enemy.h"
#include "../Utils.h"
#include <iostream>
#include "../Character/Character.h"
#include "../Combat/Combat.h"


using namespace combat_utils;

Enemy::Enemy(string _name, int _health, int _attack, int _defense, int _speed, int _experience) : Character(_name, _health, _attack, _defense, _speed, false) {
    experience = _experience;
}

void Enemy::doAttack(Character *target) {
    target->takeDamage(getRolledAttack(attack));
}

void Enemy::takeDamage(int damage) {
    int trueDamage = damage - defense;
    health-= trueDamage;

    cout << name << " took " << trueDamage << " damage!" << endl;
    if(health <= 0) {
        cout << name << " has been defeated!" << endl;
        int experienceGained = getExperience();
        cout << "You gained " << experienceGained << " experience!" << endl;
        cout << "--------------------------------" << endl;
        cout << "If you gained more than 100 point of experience, "
                "please check your stats (option 3) for increase them." << endl
                << "Your enemy also increasing it stats along with you" << endl;
        Combat::addEnemyExperience(experienceGained);
    }
}

Character* Enemy::selectTarget(vector<Player*> possibleTargets) {
    //target with less health
    int lessHealth = 9999999;
    Character* target = nullptr;
    for(auto character : possibleTargets) {
        if(character->getHealth() < lessHealth) {
            lessHealth = character->getHealth();
            target = character;
        }
    }
    return target;
}

Action Enemy::takeAction(vector<Player*> partyMembers) {
    Action currentAction;
    currentAction.speed = getSpeed();

    Character* target = selectTarget(partyMembers);
    currentAction.target = target;
    currentAction.action = [this, target](){
        doAttack(target);
    };

    return currentAction;
}

void Enemy::defendIfNeeded() {
    // Verificar si la vida del enemigo está al 15%
    if ((double)health / this->getMaxHealth() <= 0.15) {
        // Generar un número aleatorio entre 0 y 99 para representar un porcentaje
        int randomChance = rand() % 100;
        // Verificar si el número aleatorio está dentro del 40%
        if (randomChance < 40) {
            // Realizar la acción de defensa
            defend();
        }
    }
}

int Enemy::getExperience() const {
    return experience;
}

