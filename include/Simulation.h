#pragma once
#include <string>
#include <vector>
#include "Facility.h"
#include "Plan.h"
#include "Settlement.h"
using std::string;
using std::vector;

class BaseAction;
class SelectionPolicy;

class Simulation {
    public:
        Simulation(const string &configFilePath);
        Simulation(const Simulation& other); // copy constructor
        Simulation(Simulation&& other); // move constructor
        void start();
        void addPlan(const Settlement &settlement, SelectionPolicy *selectionPolicy);
        void addAction(BaseAction *action);
        bool addSettlement(Settlement *settlement);
        bool addFacility(FacilityType facility);
        bool isSettlementExists(const string &settlementName);
        bool isFacilityExists(const string &facilityName);
        bool isPlanExists(const string &settlementName, const string policy);
        bool isPlanExists(const int planID);
        Settlement &getSettlement(const string &settlementName);
        vector<BaseAction*> &getactionsLog ();
        Plan &getPlan(const int planID);
        void step();
        void close();
        void open();
        ~Simulation(); // distructor
        Simulation& operator=(const Simulation&); // copy assignment
        Simulation& operator=(Simulation&&); // move Assignment


    private:
        bool isRunning;
        int planCounter; //For assigning unique plan IDs
        vector<BaseAction*> actionsLog;
        vector<Plan> plans;
        vector<Settlement*> settlements;
        vector<FacilityType> facilitiesOptions;
}



;