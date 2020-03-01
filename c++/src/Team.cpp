/**
 * 
 * FILE: Team.Cpp
 * DESCRIPTION: This file will be used to manage our
 * linked list of teams. Any functions related to searching
 * for teams and printing information related to them will be here.
 * AUTHOR: Jonathan Daniel
 * 
 * 
 * 
 **/

#include "Team.h"
#include <iostream>

using namespace std;

Team* headNode;
Team* tailNode;

int listSize = 0;

void insertNewTeam(Team* newTeam) {
    if (headNode == 0) {
        headNode = newTeam;
        tailNode = newTeam;

        headNode->setNextTeam(nullptr);
    } else {
        tailNode->setNextTeam(newTeam); //Old tail should print to new team
        tailNode = newTeam;
        tailNode->setNextTeam(nullptr);
    }

    listSize++;
}

void printAllTeams() {
    Team* currentTeam = headNode;
    while (currentTeam != nullptr) {
        currentTeam = currentTeam->getNextTeam();
    }
}

int getTeamPoints(unsigned int teamNum) {
    Team* currentTeam = headNode;
    while (currentTeam != nullptr) {
        if(teamNum == currentTeam->getTeamNum()){
            return currentTeam->getTotalDistrictPoints();
        }

        currentTeam = currentTeam->getNextTeam();
    }
    return -1;
}
