#include "Action.h"
#include "SelectionPolicy.h"
#include <iostream>
#include <string>
using namespace std;


BaseAction::BaseAction():errorMsg(""),status(ActionStatus::COMPLETED){}

ActionStatus BaseAction:: getStatus() const
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
    std::cout <<"Error :"<< (this->getErrorMsg())<< std::endl;
}

const string& BaseAction::getErrorMsg() const
{
    return errorMsg;
}

//simulateStep
SimulateStep::SimulateStep(const int numOfSteps): numOfSteps(numOfSteps){}

void SimulateStep::act(Simulation &simulation)
{
    for(int i=0; i< numOfSteps;i++)
    {
        simulation.step();
    }
    this->complete();
}

const string SimulateStep::toString() const
{
    return "step " + std::to_string(numOfSteps);
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
    if(!simulation.isPlanExists(settlementName,selectionPolicy))
    {
        this->error("cannot create this plan");
    }
    else
    {
        SelectionPolicy *policy;
            if (selectionPolicy == "nve")
                policy = new NaiveSelection();
            else if (selectionPolicy == "bal")
                policy = new BalancedSelection(0,0,0);
            else if (selectionPolicy == "eco")
                policy = new EconomySelection();
            else if (selectionPolicy == "env")
                policy = new SustainabilitySelection();   

        simulation.addPlan(simulation.getSettlement(settlementName),policy);
        this->complete();
    }
}

const string AddPlan::toString() const
{
    return "plan " + settlementName+" " +selectionPolicy;
}
AddPlan* AddPlan::clone() const
{
     return new AddPlan(*this);
}

//add settlement
AddSettlement::AddSettlement(const string &settlementName,SettlementType settlementType):
settlementName(settlementName), settlementType(settlementType) {}

void AddSettlement::act(Simulation &simulation)
{
    Settlement* sett= new Settlement(settlementName,settlementType);
    if(simulation.addSettlement(sett))
    {
        this->complete();
    }
    else
    {
        this->error("Settlement already exist");
    }
}

AddSettlement* AddSettlement::clone() const
{
    return new AddSettlement(*this);
}

const string AddSettlement::toString() const 
{
    if(settlementType==SettlementType::VILLAGE)
        return "settlement " + settlementName+ "VILLAGE";
    else if(settlementType==SettlementType::METROPOLIS)
        return "settlement " + settlementName+ "METROPOLIS";
    else 
        return "settlement " + settlementName+ "CITY";
}

//add Facilty
AddFacility::AddFacility(const string &facilityName, const FacilityCategory facilityCategory, const int price, const int lifeQualityScore, const int economyScore, const int environmentScore):
facilityName(facilityName),facilityCategory(facilityCategory),price(price),lifeQualityScore(lifeQualityScore),
economyScore(economyScore),environmentScore(environmentScore){}

void AddFacility::act(Simulation &simulation)
{
FacilityType ft = FacilityType(facilityName,facilityCategory,price,lifeQualityScore,economyScore,environmentScore);
if(simulation.addFacility(ft))
{
        this->complete();
    }
    else
    {
        this->error("Facility already exist");
    }
}


AddFacility* AddFacility::clone() const 
{
    return new AddFacility(*this);
}
const string AddFacility::toString() const
{
    if(facilityCategory==FacilityCategory::ECONOMY)
       { return "facility " +facilityName+ " 1 "+std::to_string(price)+" "+std::to_string(lifeQualityScore)+" "+std::to_string(economyScore)+" "+std::to_string(environmentScore);}
    else if(facilityCategory==FacilityCategory::ENVIRONMENT)
        { return "facility " +facilityName+ " 2 "+std::to_string(price)+" "+std::to_string(lifeQualityScore)+" "+std::to_string(economyScore)+" "+std::to_string(environmentScore);}
    return "facility " +facilityName+ " 0 "+std::to_string(price)+" "+std::to_string(lifeQualityScore)+" "+std::to_string(economyScore)+" "+std::to_string(environmentScore);
}

//Print Plan Status
PrintPlanStatus::PrintPlanStatus(int planId):planId(planId){}

void PrintPlanStatus::act(Simulation &simulation)
{
   if(simulation.isPlanExists(planId))
     {
        Plan plan = simulation.getPlan(planId);
        std::cout << plan.toString() << std::endl;
     this->complete(); }
    else
    {
        this->error("Plan doesn't exist.");
    }
}
PrintPlanStatus* PrintPlanStatus::clone() const
{
    return new PrintPlanStatus(*this);
}
const string PrintPlanStatus:: toString() const
{
    return "planStatus "+std::to_string(planId);
}

//change plan policy
ChangePlanPolicy::ChangePlanPolicy(const int planId, const string &newPolicy): 
planId(planId), newPolicy(newPolicy){}

void ChangePlanPolicy::act(Simulation &simulation)
{
    if(simulation.isPlanExists(planId))
    {
        Plan p = simulation.getPlan(planId);
        SelectionPolicy* policy;
        if (newPolicy == "nve")
            policy = new NaiveSelection();
        else if (newPolicy == "bal") {     
            int env=p.getEnvironmentScore()+p.getUnderConstructionsEnvironmentScore();
            int eco=p.getEconomyScore()+p.getUnderConstructionsEconomyScore();
            int lifequ=p.getlifeQualityScore()+p.getUnderConstructionslifeQualityScore();
            policy = new BalancedSelection(lifequ,eco,env);
        }
        else if (newPolicy == "eco")
           policy = new EconomySelection();
        else if (newPolicy == "env")
          policy = new SustainabilitySelection();
    
        if(p.getSelectionPolicy().toString()!=policy->toString())
        {
        p.setSelectionPolicy(policy);
        this->complete();
        }
        else{
        this->error("cannot change selection policy");
        }
    }
    else
    {
        this->error("Plan doesn't exists.");
    }
}

ChangePlanPolicy* ChangePlanPolicy::clone() const
{
    return new ChangePlanPolicy(*this);
}
const string ChangePlanPolicy::toString() const 
{
    return "ChangePolicy "+std::to_string(planId)+" "+ newPolicy;
}


//print action log
PrintActionsLog::PrintActionsLog(){}
void PrintActionsLog::act(Simulation &simulation) 
{
    vector<BaseAction*> BA=simulation.getactionsLog();
    for(size_t i=0;i<BA.size()-1;i++)
    {
     if(BA[i]->getStatus()==ActionStatus::COMPLETED)
      std::cout <<BA[i]->toString()+" COMPLETED"<< std::endl;
      else
      std::cout <<BA[i]->toString()+" ERROR"<< std::endl;
    }
}
PrintActionsLog* PrintActionsLog::clone() const
{
    return new PrintActionsLog(*this);
}
const string PrintActionsLog::toString() const
{
    return "log";
}

//close
Close::Close(){}
void Close::act(Simulation &simulation)
{
simulation.close();
this->complete();
}

Close* Close::clone() const 
{ return new Close(*this);}

const string Close::toString() const
{
return "close";
}

//backup
BackupSimulation::BackupSimulation(){}
void BackupSimulation::act(Simulation &simulation)
{
backup=new Simulation(simulation);
this->complete();
}

BackupSimulation* BackupSimulation::clone() const {
return new BackupSimulation(*this);}
const string BackupSimulation::toString() const {
return "backup";}

//restore
RestoreSimulation::RestoreSimulation(){}
void RestoreSimulation::act(Simulation &simulation)
{
    if(backup)
    {simulation=*backup;
    this->complete();} //simulation is changing because its a ref.
    else
    {this->error("No backup available");}
} 
RestoreSimulation* RestoreSimulation::clone() const
{return new RestoreSimulation(*this);}
const string RestoreSimulation::toString() const 
{return "restore";}
