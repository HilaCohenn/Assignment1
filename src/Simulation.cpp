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
    : isRunning(false), planCounter(0), actionsLog(), plans(), settlements(), facilitiesOptions() {
    std::ifstream configFile(configFilePath);

    string line;
    while (std::getline(configFile, line)) {
        vector<string> commands = Auxiliary::parseArguments(line);
        if (commands[0] == "settlement") {
            addSettlement(new Settlement (commands[1], static_cast<SettlementType>(stoi(commands[2]))));
        } 
        else if (commands[0] == "facility") {
            string facilityName = commands[1];
            int price= stoi(commands[3]), life= stoi(commands[4]), eco= stoi(commands[5]), env= stoi(commands[6]);
            FacilityCategory category = static_cast<FacilityCategory>(stoi(commands[2]));
            addFacility(FacilityType (facilityName, category, price, life, eco, env));
        } 
        else if (commands[0] == "plan") {
            string settlementName= commands[1], policyType= commands[2];
            SelectionPolicy *policy;
            if (policyType == "nve")
                policy = new NaiveSelection();
            else if (policyType == "bal")
                policy = new BalancedSelection(0,0,0);
            else if (policyType == "eco")
                policy = new EconomySelection();
            else if (policyType == "env")
                policy = new SustainabilitySelection();
            addPlan (getSettlement(settlementName), policy);
        }
    }
}

// copy constructor
Simulation::Simulation(const Simulation& other): isRunning(other.isRunning), 
planCounter(other.planCounter), actionsLog(), plans(other.plans),settlements(), facilitiesOptions(other.facilitiesOptions){

    for(BaseAction* action: other.actionsLog){
        actionsLog.push_back(action->clone());
    }

    for(Settlement* settlement: settlements){
        Settlement* copiedSettlement = new Settlement(settlement->getName(), settlement->getType());
        settlements.push_back(copiedSettlement);
    }
}

// Move constructor
Simulation::Simulation(Simulation&& other): isRunning(other.isRunning), planCounter(other.planCounter),
    actionsLog(move(other.actionsLog)), plans(move(other.plans)), settlements(move(other.settlements)),
    facilitiesOptions(move(other.facilitiesOptions)) {
    other.isRunning = false;
    other.planCounter = 0;
    other.actionsLog.clear();
    other.plans.clear();
    other.settlements.clear();
    other.facilitiesOptions.clear();
}

// distructor
Simulation::~Simulation() {
    for (BaseAction* action : actionsLog) {
        delete action;
    }
    actionsLog.clear();

    for (Settlement* settlement : settlements) {
        delete settlement;
    }
    settlements.clear();
}

void Simulation::start() {
    open();
    cout << "The simulation has started" << endl;
    string line;
    while (isRunning){
        
        getline(cin, line);
        std::vector<std::string> commands = Auxiliary::parseArguments(line);
        BaseAction *action;
        string actionName = commands[0];
        // Settlement
        if (actionName == "settlement"){
            if (!isSettlementExists(commands[1])){
                action = new AddSettlement(commands[1], static_cast<SettlementType>(std::stoi(commands[2])));
            }
        }

        //Facility
        else if (actionName == "facility"){
            string facilityName = commands[1];
            int price= stoi(commands[3]), life= stoi(commands[4]), eco= stoi(commands[5]), env= stoi(commands[6]);
            FacilityCategory category = static_cast<FacilityCategory>(stoi(commands[2]));
            action = new AddFacility(facilityName, category, price, life, eco, env);
        }

        //Plan
        else if (actionName == "plan"){
            string settlementName= commands[1], policyType= commands[2];
            action = new AddPlan (settlementName, policyType);
        }

        // simulate step
        else if (actionName == "step"){
            int num = stoi(commands[1]);
            action = new SimulateStep (num);
        }

        // print plan status
        else if(actionName == "planStatus"){
            int id = stoi(commands[1]);
            action = new PrintPlanStatus(id);
        }

        // change plan policy
        else if(actionName == "changePolicy"){
            int id = stoi(commands[1]);
            string policy = commands[2];
            action = new ChangePlanPolicy (id, policy);
        } 

        // print actions log
        else if (actionName == "log"){
            action = new PrintActionsLog();
        }

        // close
        else if (actionName == "close"){
            action = new Close();
        }

        // backup simulation
        else if (actionName == "backup"){
            action = new BackupSimulation();
        }

        // restore simulation
        else if (actionName == "restore"){
            action = new RestoreSimulation();
        }

        addAction(action);
        action->act(*this);
    }
}

void Simulation::addPlan(const Settlement &settlement, SelectionPolicy *selectionPolicy) {
    plans.push_back(Plan (planCounter, settlement, selectionPolicy, facilitiesOptions));
    planCounter++;
}

void Simulation::addAction(BaseAction *action) {
    actionsLog.push_back(action);
}

bool Simulation::addSettlement(Settlement *settlement) {
    if (isSettlementExists(settlement->getName())){
        return false;
    }
    settlements.push_back(settlement);
    return true;
}

bool Simulation::addFacility(FacilityType facility) {
    if (isFacilityExists(facility.getName()))
        return false;
    facilitiesOptions.push_back(facility);
    return true; 
}

bool Simulation::isSettlementExists(const string &settlementName) {
    for (Settlement* settlement: settlements){
        if (settlement->getName() == settlementName)
            return true;
    }
    return false;
}

bool Simulation::isFacilityExists(const string &facilityName) {
    for (FacilityType facility: facilitiesOptions){
        if (facility.getName() == facilityName)
            return true;
    }
    return false;
}

bool Simulation::isPlanExists(const string &settlementName, const string policy) {
    if (isSettlementExists(settlementName) && (policy== "nve"||policy=="bal"||policy=="eco"||policy=="env"))
        return true;
    else 
        return false;
}

bool Simulation::isPlanExists(int planId) {
    for (const Plan& plan : plans){
        if (plan.getPlanID()== planId)
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
        cout << "we searched for non existing settlement" << endl;
        return *settlements[0]; // we should not get here 
}

vector<BaseAction*> &Simulation::getactionsLog (){
    return actionsLog;
}

Plan &Simulation::getPlan(const int planID) {
    Plan *getplan;
    for (Plan plan : plans){
        if (plan.getPlanID()== planID)
            getplan= &plan;
    }
    return *getplan;
}

void Simulation::step() {
    for (Plan plan : plans){
        plan.step();
    }
}

void Simulation::close() {
    isRunning = false;
    for(Plan plan: plans)
    {
      cout <<plan.toString()<< endl;
    }
}

void Simulation::open() {
    isRunning = true;
}

// move operator
Simulation& Simulation::operator=(Simulation&& other) {
    if (this != &other) {
        for (BaseAction* action : actionsLog) {
            delete action;
        }
        actionsLog.clear();
        for (Settlement* settlement : settlements) {
            delete settlement;
        }
        settlements.clear();

        isRunning = other.isRunning;
        planCounter = other.planCounter;

        actionsLog = move(other.actionsLog);
        plans = move(other.plans);
        settlements = move(other.settlements);
        facilitiesOptions = move(other.facilitiesOptions);

        other.isRunning = false;
        other.planCounter = 0;
        other.actionsLog.clear();
        other.settlements.clear();
        other.plans.clear();
        other.facilitiesOptions.clear();
    }
    return *this;
}

// operator =
Simulation& Simulation::operator=(const Simulation& other) {

    if (this == &other) {
        return *this;
    }

    for (auto* settlement : settlements) {
        delete settlement;
    }
    settlements.clear();

    for (auto* action : actionsLog) {
        delete action;
    }
    actionsLog.clear();

    isRunning = other.isRunning;
    planCounter = other.planCounter;
    facilitiesOptions =  vector<FacilityType>(other.facilitiesOptions);
    plans =  vector<Plan>(other.plans);

    for (Settlement* settlement : other.settlements) {
        settlements.push_back(new Settlement(*settlement));
    }

    for (BaseAction* action : other.actionsLog) {
        actionsLog.push_back(action->clone());
    }

    return *this;
}


