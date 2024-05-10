//
// Created by Victor Navarro on 15/02/24.
//

#include "Player.h"
#include <iostream>

using namespace std;

Player::Player(string _name, int _health, int _attack, int _defense, int _speed)
        : Character(_name, _health, _attack, _defense, _speed, true), experience(0) {
    level = 1;
}

int Player::getExperience() const {
    return experience; // Devuelve la experiencia total del jugador
}

void Player::doAttack(Character *target) {
    target->takeDamage(attack);
}

void Player::doDefend() {
    defend();
}

void Player::takeDamage(int damage) {
    int trueDamage = damage - defense;

    health-= trueDamage;

    cout << name << " took " << trueDamage << " damage!" << endl;

    if(health <= 0) {
        cout << name << " has been defeated!" << endl;
    }
}


void Player::gainExperience(int exp) {
    experience += exp; // Suma la experiencia ganada al total del jugador
    checkLevelUp(); // Verifica si el jugador debe subir de nivel
}

void Player::checkLevelUp() {
    if (experience >= 100) {
        levelUp(); // Sube de nivel
        experience -= 100; // Resta 100 puntos de experiencia
        cout << name << " leveled up! Now at level " << level << endl;
    }
}

void Player::levelUp() {
    level++; // Incrementa el nivel

    cout << "You've leveled up! Distribute 5 points to your stats:" << endl;
    cout << "1. Health" << endl;
    cout << "2. Attack" << endl;
    cout << "3. Defense" << endl;
    cout << "4. Speed" << endl;

    int points = 5;
    while (points > 0) {
        int choice;
        cout << "You have " << points << " points left. Enter the number of the stat you want to increase: ";
        cin >> choice;

        switch (choice) {
            case 1:
                health++;
                cout << "Health increased by 1." << endl;
                break;
            case 2:
                attack++;
                cout << "Attack increased by 1." << endl;
                break;
            case 3:
                defense++;
                cout << "Defense increased by 1." << endl;
                break;
            case 4:
                speed++;
                cout << "Speed increased by 1." << endl;
                break;
            default:
                cout << "Invalid choice. Please try again." << endl;
                break;
        }

        points--;
    }
    // Incrementar las estadísticas de los enemigos
    for (Enemy* enemy : enemies) {
        enemy->increaseStats();
    }
}



void Enemy::increaseStats() {
    // Incrementar las estadísticas de los enemigos
    health += 2;
    attack += 1;
    defense += 1;
    speed += 1;
}

void Player::checkExperience() {
    cout << "Current experience: " << experience << endl;
}

Character* Player::selectTarget(vector<Enemy*> possibleTargets) {
    int selectedTarget = 0;
    cout << "Select a target: " << endl;
    for (int i = 0; i < possibleTargets.size(); i++) {
        cout << i << ". " << possibleTargets[i]->getName() << endl;
    }

    //TODO: Add input validation
    cin >> selectedTarget;
    return possibleTargets[selectedTarget];
}

Action Player::takeAction(vector<Enemy*> enemies, vector<Player*>& partyMembers, int totalEnemyExperience) {
    int action = 0;
    Action currentAction;
    Character* target = nullptr;

    do {
        cout << "--------------------------------" << endl;
        cout << "Select an action: " << endl
             << "1. Attack" << endl << "2. Defense" << endl
             << "3. Check Exp" << endl << "4. Check Level" << endl
             << "5. Check stats" << endl << "6. Check enemy stats" << endl
             << "7. Save Game" << endl << "8. Load Game" << endl;

        // Leer la entrada del usuario
        cin >> action;

        // Validar la entrada del usuario
        if (action < 1 || action > 8) {
            cout << "--------------------------------" << endl;
            cout << "Invalid action. Please try again." << endl;
            continue; // Volver a solicitar la entrada del usuario
        }

        switch(action) {
            case 1:
                cout << "--------------------------------" << endl;
                target = selectTarget(enemies);
                currentAction.target = target;
                currentAction.action = [this, target](){
                    doAttack(target);
                };
                currentAction.speed = getSpeed();
                break;
            case 2:
                cout << "--------------------------------" << endl;
                target = this;
                currentAction.target = target;
                currentAction.action = [this]() {
                    defend();
                };
                currentAction.speed = getSpeed();
                break;
            case 3:
                cout << "--------------------------------" << endl;
                cout << "Checking experience..." << endl;
                for (Player* player : partyMembers) {
                    int previousExp = player->getExperience();
                    player->gainExperience(totalEnemyExperience); // Ganancia de experiencia por derrotar a los enemigos
                    int newExp = player->getExperience();
                    cout << "Player " << player->getName() << " gained " << (newExp - previousExp) << " experience." << endl;
                }
                for (Enemy* enemy : enemies) {
                    enemy->increaseStats();
                }
                break;
            case 4:
                cout << "--------------------------------" << endl;
                cout << "Checking level..." << endl;
                cout << "Player " << getName() << " is level " << level << endl;
                break;
            case 5:
                cout << "--------------------------------" << endl;
                cout << "Checking stats..." << endl;
                cout << "Name: " << getName() << endl;
                cout << "Health: " << getHealth() << endl;
                cout << "Attack: " << getAttack() << endl;
                cout << "Defense: " << getDefense() << endl;
                cout << "Speed: " << getSpeed() << endl;
                break;
            case 6:
                cout << "--------------------------------" << endl;
                cout << "Checking enemy stats..." << endl;
                for (Enemy* enemy : enemies) {
                    cout << "--------------------------------" << endl;
                    cout << "Name: " << enemy->getName() << endl;
                    cout << "Health: " << enemy->getHealth() << endl;
                    cout << "Attack: " << enemy->getAttack() << endl;
                    cout << "Defense: " << enemy->getDefense() << endl;
                    cout << "Speed: " << enemy->getSpeed() << endl;
                }
                break;
            case 7:
                cout << "--------------------------------" << endl;
                saveToFile("player_stats.txt");
                cout << "Player stats saved successfully." << endl;
                break;
            case 8:
                cout << "--------------------------------" << endl;
                if (!loadFromFile("player_stats.txt")) {
                    cout << "Failed to load player stats. Starting a new game." << endl;
                    // Inicializar el jugador con estadísticas predeterminadas
                } else {
                    cout << "Player stats loaded successfully." << endl;
                }
                break;
            default:
                cout << "Invalid action" << endl;
                break;
        }
    } while (action == 3 || action == 4 || action == 5 || action == 6
    || action == 7 || action == 8); // Repetir mientras la acción sea 3, 4, 5, 6, 7 o 8

    return currentAction;
}