#include "Simulation.h"
#include "Settlement.h"
#include "Action.h"
#include <fstream>
#include <sstream>
#include <stdexcept>
#include "Simulation.h"
#include "SelectionPolicy.h"
#include <iostream>
#include "Auxiliary.h"
#include "Plan.h"
using namespace std;

Simulation::Simulation(const string &configFilePath) 
    : isRunning(false), planCounter(0) {
    std::ifstream configFile(configFilePath);

// do a functions that tells me what is the type from the string 
// and then i can put it in the constructors

    string line;
    while (std::getline(configFile, line)) {
        std::vector<std::string> commands = Auxiliary::parseArguments(line);
        if (commands[0] == "settlement") {
            addSettlement(new Settlement (commands[1], commands[2]));
        } 
        else if (commands[0] == "facility") {
            string facilityName;
            int price, life, eco, env;
            FacilityCategory category;
            lineStream >> facilityName >> category >> price >> life >> eco >> env;
            addFacility(new FacilityType (facilityName, category, price, life, eco, env));
        } 
        else if (commands[0] == "plan") {
            string settlementName, policyType;
            lineStream >> settlementName >> policyType;
            // Find settlement and add Plan with the appropriate SelectionPolicy

        }
    }
}



void Simulation::start() {
    isRunning = true;
    cout << "The simulation has started" << endl;
    std::istringstream lineStream(line);
    string command;
    lineStream >> command;
    while (isRunning){
        // check each line and do things
        string command;
        getline(cin, command);
        if (command == "settlement"){
            string settlementName;
            SettlementType settlementType;
            lineStream >> settlementName >> settlementType;
            if (!isSettlementExists(settlementName)){
                AddSettlement *action = new AddSettlement(settlementName, settlementType);
                addAction(action);
                action->act(*this);
            }
        }
        if (command == "facility"){
            string facilityName;
            int price, life, eco, env;
            FacilityCategory category;
            lineStream >> facilityName >> category >> price >> life >> eco >> env;
            AddFacility *action = new AddFacility(facilityName, category, price, life, eco, env);
            addAction(action);
            action->act(*this);
        }
        if (command == "plan"){

        }
        

        }
}

void Simulation::addPlan(const Settlement &settlement, SelectionPolicy *selectionPolicy) {
    // Add a new plan
}

void Simulation::addAction(BaseAction *action) {
    actionsLog.push_back(action);
}

bool Simulation::addSettlement(Settlement *settlement) {
    settlements.push_back(settlement);
    return true; //default return
}

bool Simulation::addFacility(FacilityType facility) {
    facilitiesOptions.push_back(facility);
    return true; // Default return
}

bool Simulation::isSettlementExists(const string &settlementName) {
    for (Settlement* settlement: settlements){
        if (settlement->getName() == settlementName)
            return true;
    }
    return false;
}

Settlement &Simulation::getSettlement(const string &settlementName) {
    for (Settlement* settlement: settlements){
        if (settlement->getName() == settlementName){
            
            return *settlement;
        }
    }
    return;
}

Plan &Simulation::getPlan(const int planID) {
    for (Plan plan : plans){
        if (plan.getPlanID()== planID)
            return plan;
    }
}

void Simulation::step() {
    for (Plan plan : plans){
        plan.step();
    }
}

void Simulation::close() {
    isRunning = false;
    //add more as we go 
}

void Simulation::open() {
    isRunning = true;
}
