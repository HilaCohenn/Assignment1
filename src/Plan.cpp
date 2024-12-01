#include "Plan.h"
using namespace std;
#include <iostream>
#include <vector>
#include <sstream>

Plan::Plan(const int planId, const Settlement &settlement, SelectionPolicy *selectionPolicy,const vector<FacilityType> &facilityOptions):
plan_id(planId),settlement(settlement),selectionPolicy(selectionPolicy),status(PlanStatus::AVALIABLE),facilities(),
underConstruction(),facilityOptions(facilityOptions),life_quality_score(0),economy_score(0),environment_score(0){}

Plan::Plan(const Plan& other):plan_id(other.plan_id),settlement(other.settlement),selectionPolicy(other.selectionPolicy->clone()),status(other.status),
facilities(),underConstruction(),facilityOptions(other.facilityOptions),life_quality_score(other.life_quality_score),
economy_score(other.economy_score),environment_score(other.environment_score){
for(size_t i=0;i<other.getFacilities().size();i++)
{
    Facility* facility= new Facility(*((other.getFacilities())[i]));
    facilities.push_back(facility);
}
for(size_t i=0;i<other.underConstruction.size();i++)
{
    Facility* facility= new Facility(*(other.underConstruction[i]));
    underConstruction.push_back(facility);
}
}

Plan::Plan(Plan&& other) : Plan(other.plan_id,other.settlement,other.selectionPolicy,other.facilityOptions)
{
    other.selectionPolicy=nullptr;
}

Plan::~Plan()
{
  if (selectionPolicy) {
        delete selectionPolicy;
        selectionPolicy = nullptr;
    }
     for (auto facility : facilities) {
        if (facility) {
            delete facility;
        }
    }
    facilities.clear();
    for (auto facility : underConstruction) {
        if (facility) {
            delete facility;
        }
    }
    underConstruction.clear();
}

const int Plan::getPlanID() const
{
    return plan_id;
}

const int Plan::getlifeQualityScore() const
{
    return life_quality_score;
}

const int Plan::getEconomyScore() const
{
    return economy_score;
}

const int Plan::getEnvironmentScore() const
{
    return environment_score;
}

const int Plan::getUnderConstructionslifeQualityScore() const
{
    int updateLifeQuality=0;
    for(size_t i =0;i<underConstruction.size();i++)
        {
            updateLifeQuality+=underConstruction[i]->getLifeQualityScore();
        }
    return updateLifeQuality;
}

const int Plan::getUnderConstructionsEconomyScore() const
{
    int updateEconomy=0;
    for(size_t i =0;i<underConstruction.size();i++)
        {
            updateEconomy+=underConstruction[i]->getEconomyScore();
        }
    return updateEconomy;
}

const int Plan::getUnderConstructionsEnvironmentScore() const
{
    int updateEnviorment=0;
    for(size_t i =0;i<underConstruction.size();i++)
        {
            updateEnviorment+=underConstruction[i]->getEnvironmentScore();
        }
    return updateEnviorment;
}

void Plan::setSelectionPolicy(SelectionPolicy *selectionPolicy){
    if(selectionPolicy != this->selectionPolicy)
    {
    if (this->selectionPolicy){
    delete this->selectionPolicy;}
    this->selectionPolicy=selectionPolicy;
    }
}

void Plan::printStatus(){
    if(status==PlanStatus::AVALIABLE)
    cout << "PlanStatus: AVALIABLE"  << endl;
    else
    {cout << "PlanStatus: BUSY"  << endl;}
}

const vector<Facility*>& Plan::getFacilities() const
{
    return facilities;
}

void Plan::addFacility(Facility* facility)
{
        if (!facility) {
        return; 
    }
    if(facility->getStatus() == FacilityStatus::UNDER_CONSTRUCTIONS)
    {underConstruction.push_back(facility);}
    else{
        facilities.push_back(facility);
        life_quality_score+=facility->getLifeQualityScore();
        economy_score+=facility->getEconomyScore();
        environment_score+=facility->getEnvironmentScore();
    }

}

void Plan::step()
{
    if (status==PlanStatus::AVALIABLE)
    {
        size_t limit=3;
        if(settlement.getType()==SettlementType::VILLAGE)
        {limit=1;}
        else if(settlement.getType()==SettlementType::CITY)
        {limit=2;}
        while(underConstruction.size()!=limit)
        {
            Facility* faciltyToAdd= new Facility((*selectionPolicy).selectFacility(facilityOptions),settlement.getName());
            addFacility(faciltyToAdd);
        }
        for(size_t i=0;i<underConstruction.size();i++)
        {
          if (underConstruction[i]->step() == FacilityStatus::OPERATIONAL)
           {
            addFacility(underConstruction[i]);
            underConstruction.erase(underConstruction.begin() + i);
            i--;
           }
        }
        if(underConstruction.size()<limit)
        {status=PlanStatus::AVALIABLE;}
        else
        {status=PlanStatus::BUSY;}

    }
}

const string Plan::toString() const
{
      std::ostringstream output;

    output << "PlanID: " << plan_id << '\n';
    output << "SettlementName: " << settlement.getName() << '\n';
    output << "PlanStatus: " << (status == PlanStatus::AVALIABLE ? "AVALIABLE" : "BUSY") << '\n';
    output << "SelectionPolicy: " << selectionPolicy->toString() << '\n';
    output << "LifeQualityScore: " << life_quality_score << '\n';
    output << "EconomyScore: " << economy_score << '\n';
    output << "EnvironmentScore: " << environment_score << '\n';

    output << "Facilities Under Construction:\n";
    for (const Facility* facility : underConstruction) {
        output << "FacilityName: " << facility->getName() << '\n';
        output << "FacilityStatus: " << (facility->getStatus() == FacilityStatus::UNDER_CONSTRUCTIONS ? "UNDER_CONSTRUCTIONS" : "OPERATIONAL") << '\n';
    }

    output << "Completed Facilities:\n";
    for (const Facility* facility : facilities) {
        output << "FacilityName: " << facility->getName() << '\n';
        output << "FacilityStatus: " << (facility->getStatus() == FacilityStatus::UNDER_CONSTRUCTIONS ? "UNDER_CONSTRUCTIONS" : "OPERATIONAL") << '\n';
    }

    return output.str();  
}