#include "Action.h"
#include <iostream>
#include <string>
using namespace std
 
 //shira
 
BaseAction::BaseAction();//ToDo

BaseAction::ActionStatus getStatus() const
{
    return status;
}

void BaseAction::complete()
{
    status=ActionStatus::COMPLETED;
}

void BaseAction::error(string errorMsg)
{
    status=ActionStatus::ERROR;
    this->errorMsg=errorMsg;
}

const string& BaseAction::getErrorMsg() const
{
    return errorMsg;
}

    private:
        string errorMsg;
        ActionStatus status;

//simulateStep
SimulateStep::SimulateStep(const int numOfSteps): numOfSteps(numOfSteps){}

void SimulateStep::act(Simulation &simulation)
{
    for(int i=0; i< numOfSteps;i++)
    {
        simulation.step();
    }
}
const string SimulateStep::toString()
{
    return "step";
}
SimulateStep* SimulateStep::clone() const
{
    {return new SimulateStep(*this);}
}


//add plan
AddPlan::AddPlan(const string &settlementName, const string &selectionPolicy):settlementName(settlementName),
selectionPolicy(selectionPolicy){}

void AddPlan::act(Simulation &simulation)
{
    SelectionPolicy Policy;
            if (selectionPolicy == "nve")
                policy = NaiveSelection();
            if (selectionPolicy == "bal")
                policy = BalancedSelection(0,0,0);
            if (selectionPolicy == "eco")
                policy = EconomySelection();
            if (selectionPolicy == "env")
                policy = SustainabilitySelection();
    simulation.addPlan(simulation.getSettlement(settlementName),policy);
}

const string AddPlan::toString() const
{
    return "Add Plan";
}
AddPlan* AddPlan::clone() const
{
     return new AddPlan(*this);
}

//add settlement
AddSettlement::AddSettlement(const string &settlementName,SettlementType settlementType):
settlementName(settlementName), SettlementType(settlementType) {}

void AddSettlement::act(Simulation &simulation)
{
    Settlement* sett= new Settlement(settlementName,SettlementType);
    simulation.addSettlement(sett);
}

AddSettlement* AddSettlement::clone() const
{
    return new AddSettlement(*this);
}

const string AddSettlement::toString() const 
{
    return "Add settlement";
}

//add Facilty
AddFacility::AddFacility(const string &facilityName, const FacilityCategory facilityCategory, const int price, const int lifeQualityScore, const int economyScore, const int environmentScore):
facilityName(facilityName),facilityCategory(facilityCategory),price(price),lifeQualityScore(lifeQualityScore),
economyScore(economyScore),environmentScore(environmentScore){}

void AddFacility::act(Simulation &simulation)
{
FacilityType ft = FacilityType(facilityName,facilityCategory,price,lifeQualityScore,economyScore,environmentScore);
simulation.addFacility(ft);
}
AddFacility* AddFacility::clone() const 
{
    return new AddFacility(*this);
}
const string AddFacilitytoString() const
{
    return "Add facility";
}

//Print Plan Status
PrintPlanStatus::PrintPlanStatus(int planId):planId(planId){}

void PrintPlanStatus::act(Simulation &simulation)
{
   
     std::cout << simulation.getPlan(planId).toString(); << std::endl;
    
}
PrintPlanStatus* PrintPlanStatus::clone() const
{
    return new PrintPlanStatus(*this);
}
const PrintPlanStatus::string toString() const
{
    return "Print Plan Status";
}

//change plan policy
class ChangePlanPolicy : public BaseAction {
ChangePlanPolicy::ChangePlanPolicy(const int planId, const string &newPolicy):
planId(planId),newPolicy(newPolicy){}

void ChangePlanPolicy::act(Simulation &simulation)
{

    Plan p = simulation.getPlan(planId);
    SelectionPolicy* policy;
    if (newPolicy == "nve")
        policy* = new NaiveSelection();
    if (newPolicy == "bal")      
      int env=p.getEnvironmentScore()+p.getUnderConstructionsEnvironmentScore();
      int eco=p.getEconomyScore()+p.getUnderConstructionsEconomyScore();
      int lifequ=p.getlifeQualityScore()+p.getUnderConstructionslifeQualityScore();
      policy* = new BalancedSelection(lifequ,eco,env);
    if (newPolicy == "eco")
      policy = EconomySelection();
    if (newPolicy == "env")
      policy* = new SustainabilitySelection();
    
    p.setSelectionPolicy(policy);
}

ChangePlanPolicy* ChangePlanPolicy::clone() const
{
    return new ChangePlanPolicy(*this);
}
const string ChangePlanPolicy::toString() const 
{
    return "Change Plan Policy";
}

PrintActionsLog::PrintActionsLog();//need to do
void act(Simulation &simulation) override;
{

}
PrintActionsLog* PrintActionsLog::clone() const
{
    return new PrintActionsLog(*this);
}
const string PrintActionsLog::toString() const
{
    return "Print Action Log";
}






