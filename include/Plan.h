#pragma once
#include <vector>
#include "Facility.h"
#include "Settlement.h"
#include "SelectionPolicy.h"
using std::vector;

enum class PlanStatus {
    AVALIABLE,
    BUSY,
};

class Plan {
    public:
        Plan(const int planId, const Settlement &settlement, SelectionPolicy *selectionPolicy, const vector<FacilityType> &facilityOptions);
        Plan(const Plan& other);
        Plan(Plan&&);
        const int getPlanID() const;
        const SelectionPolicy& getSelectionPolicy() const;
        const int getlifeQualityScore() const;
        const int getEconomyScore() const;
        const int getEnvironmentScore() const;
        const int getUnderConstructionslifeQualityScore() const;
        const int getUnderConstructionsEconomyScore() const;
        const int getUnderConstructionsEnvironmentScore() const;
        void setSelectionPolicy(SelectionPolicy *selectionPolicy);
        void step();
        void printStatus();
        const vector<Facility*> &getFacilities() const;
        void addFacility(Facility* facility);
        const string toString() const;
        ~Plan();
        Plan& operator=(const Plan&) = delete;
        Plan& operator=(Plan&&) = delete;
        const string& getSettlementName();

    private:
        int plan_id;
        const Settlement &settlement;
        SelectionPolicy *selectionPolicy; 
        PlanStatus status;
        vector<Facility*> facilities;
        vector<Facility*> underConstruction;
        const vector<FacilityType> &facilityOptions;
        int life_quality_score, economy_score, environment_score;
};