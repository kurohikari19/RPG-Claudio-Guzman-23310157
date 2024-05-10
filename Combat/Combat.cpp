//
// Created by Victor Navarro on 19/02/24.
//

#include "Combat.h"
#include <string>
#include <iostream>
#include <utility>

using namespace std;

int Combat::totalEnemyExperience = 0; // Inicialización de la variable

void Combat::addEnemyExperience(int experience) {
    totalEnemyExperience += experience;
}

int Combat::getTotalEnemyExperience() {
    return totalEnemyExperience;
}

bool compareSpeed(Character *a, Character *b) {
    return a->getSpeed() > b->getSpeed();
}

Combat::Combat(vector<Character *> _participants) {
    participants = std::move(_participants);
    for(auto participant : participants) {
        if (participant->getIsPlayer()) {
            partyMembers.push_back((Player *) participant);
        } else {
            enemies.push_back((Enemy *) participant);
        }
    }
}

Combat::Combat(vector<Player*> _partyMembers, vector<Enemy*> _enemies) {
    partyMembers = std::move(_partyMembers);
    enemies = std::move(_enemies);
    participants = vector<Character*>();
    participants.insert(participants.end(), partyMembers.begin(), partyMembers.end());
    participants.insert(participants.end(), enemies.begin(), enemies.end());
}

Combat::Combat() {
    participants = vector<Character*>();
}

void Combat::addParticipant(Character *participant) {
    participants.push_back(participant);
    if(participant->getIsPlayer()){
        partyMembers.push_back((Player*) participant);
    } else {
        enemies.push_back((Enemy*) participant);
    }
}

void Combat::combatPrep() {
    // Sort participants by speed
    sort(participants.begin(), participants.end(), compareSpeed);
}

string Combat::toString() {
    string result = "";
    vector<Character*>::iterator it;
    for(it = participants.begin(); it != participants.end(); it++){
        result += (*it)->toString() + "\n";
    }
    cout<<"===================="<<endl;
    return result;
}

Character* Combat::getTarget(Character* attacker) {
    vector<Character*>::iterator it;
    for(it = participants.begin(); it != participants.end(); it++){
        if((*it)->getIsPlayer() != attacker->getIsPlayer()){
            return *it;
        }
    }
    //TODO: Handle this exception
    return nullptr;
}

void Combat::doCombat() {
    cout << "Inicio del combate" << endl;
    combatPrep();
    int round = 1;
    while (true) {
        // Imprimir mensaje de ronda
        cout << "Round " << round << endl;

        // Realizar acciones y continuar con la siguiente ronda
        vector<Character*>::iterator it = participants.begin();
        registerActions(it);
        executeActions(it);

        // Chequear si algún jugador ha sido derrotado
        partyMembers.erase(remove_if(partyMembers.begin(), partyMembers.end(), [&](Player* player) {
            if (player->getHealth() <= 0) {
                cout << "Player " << player->getName() << " has been defeated!" << endl;
                delete player;
                return true;
            }
            return false;
        }), partyMembers.end());

        // Verificar si el combate ha terminado
        if (enemies.empty() || partyMembers.empty()) {
            break;
        }

        round++;
    }

    if (enemies.empty()) {
        /*for (Player* player : partyMembers) {
            int previousExp = player->getExperience();
            player->gainExperience(totalEnemyExperience); // Ganancia de experiencia por derrotar a los enemigos
            int newExp = player->getExperience();
            cout << "Player " << player->getName() << " gained " << (newExp - previousExp) << " experience." << endl;
            player->checkLevelUp(); // Verificar si el jugador subió de nivel
        }*/
        cout << "You win!" << endl;
    } else {
        cout << "You lose!" << endl;
    }
}



void Combat::executeActions(vector<Character*>::iterator participant) {
    while(!actionQueue.empty()) {
        Action currentAction = actionQueue.top();
        currentAction.action();
        actionQueue.pop();

        //Check if there are any dead characters
        checkParticipantStatus(*participant);
        checkParticipantStatus(currentAction.target);
    }
}

void Combat::checkParticipantStatus(Character *participant) {
    if(participant->getHealth() <= 0) {
        if(participant->getIsPlayer()) {
            partyMembers.erase(remove(partyMembers.begin(), partyMembers.end(), participant), partyMembers.end());
        } else {
            enemies.erase(remove(enemies.begin(), enemies.end(), participant), enemies.end());
        }
        participants.erase(remove(participants.begin(), participants.end(), participant), participants.end());
    }
}

void Combat::takeAction(Character* character, ActionType actionType) {
    if (actionType == ActionType::Attack) {
        if (character->getIsPlayer()) {
            Action playerAction = ((Player*) character)->takeAction(enemies, partyMembers, totalEnemyExperience);
            actionQueue.push(playerAction);
        } else {
            Action enemyAction = ((Enemy*) character)->takeAction(partyMembers);
            actionQueue.push(enemyAction);
        }
    } else if (actionType == ActionType::Defend) {
        character->defend();
    }
}

void Combat::registerActions(vector<Character*>::iterator participantIterator) {
    while(participantIterator != participants.end()) {
        if((*participantIterator)->getIsPlayer()) {
            Action playerAction = ((Player*) *participantIterator)->takeAction(enemies, partyMembers, totalEnemyExperience);
            actionQueue.push(playerAction);
        } else {
            ((Enemy*) *participantIterator)->defendIfNeeded();
            Action enemyAction = ((Enemy*) *participantIterator)->takeAction(partyMembers);
            actionQueue.push(enemyAction);
        }

        participantIterator++;
    }
}