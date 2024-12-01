#include "SelectionPolicy.h"
using namespace std;
#include <stdexcept>
#include <algorithm>
#include <initializer_list>

//Naive Selection
NaiveSelection::NaiveSelection():SelectionPolicy(),lastSelectedIndex(-1){}

NaiveSelection::NaiveSelection(const NaiveSelection& other): SelectionPolicy(), lastSelectedIndex(other.lastSelectedIndex){}

const FacilityType& NaiveSelection::selectFacility(const vector<FacilityType>& facilitiesOptions){
        if (facilitiesOptions.empty()) {
        throw std::out_of_range("facilitiesOptions is empty");
    }
    lastSelectedIndex+=1;
    lastSelectedIndex=(lastSelectedIndex%facilitiesOptions.size());
    return facilitiesOptions[lastSelectedIndex];
}

const string NaiveSelection::toString() const {
    return "SelectionPolicy: nve";}

NaiveSelection* NaiveSelection::clone() const 
{return new NaiveSelection(*this);}

//BalancedSelection
BalancedSelection::BalancedSelection(int LifeQualityScore, int EconomyScore, int EnvironmentScore):SelectionPolicy(),LifeQualityScore(LifeQualityScore),
EconomyScore(EconomyScore),EnvironmentScore(EnvironmentScore){};

BalancedSelection::BalancedSelection(const BalancedSelection& other):SelectionPolicy(),LifeQualityScore(other.LifeQualityScore),
EconomyScore(other.EconomyScore),EnvironmentScore(other.EnvironmentScore){};

  int BalancedSelection::findDiffrence(int LifeQualityScore,int EconomyScore,int EnvironmentScore){
    int max=LifeQualityScore;
    int min=LifeQualityScore;
    if(EconomyScore>EnvironmentScore)
    {
        if(EconomyScore>max)
        {
            max=EconomyScore;
        }
        if(EnvironmentScore<min)
        {
            min=EnvironmentScore;
        }
    }
    else
    {
          if(EnvironmentScore>max)
        {
            max=EnvironmentScore;
        }
           if(EconomyScore<min)
        {
            min=EconomyScore;
        }
    }
    return max-min;
  }

const FacilityType& BalancedSelection::selectFacility(const vector<FacilityType>& facilitiesOptions) 
{
    int bal_index=0;
    int minDiffrence=0;
    int diffrence=0;
    int totalLifeQuality;
    int totalEconomy;
    int totalEnviorment;
    for(size_t i=0;i<facilitiesOptions.size();i++)
    {totalLifeQuality=LifeQualityScore+facilitiesOptions[i].getLifeQualityScore();
     totalEconomy=EconomyScore+facilitiesOptions[i].getEconomyScore();
     totalEnviorment=EnvironmentScore+facilitiesOptions[i].getEnvironmentScore();
     diffrence=this->findDiffrence(totalLifeQuality,totalEconomy,totalEnviorment);
     if(diffrence<minDiffrence || i==0)
     {minDiffrence=diffrence;
     bal_index=i;}
    }
    return facilitiesOptions[bal_index];
}

const string BalancedSelection::toString() const {
    return "SelectionPolicy: bal";
}

BalancedSelection* BalancedSelection::clone() const 
{
    return new BalancedSelection(*this);
}

//EconomySelection
EconomySelection::EconomySelection():SelectionPolicy(),lastSelectedIndex(-1){}
const FacilityType& EconomySelection::selectFacility(const vector<FacilityType>& facilitiesOptions)
{
    lastSelectedIndex+=1;
    lastSelectedIndex=(lastSelectedIndex%facilitiesOptions.size());
   while(facilitiesOptions[lastSelectedIndex].getCategory()!=FacilityCategory::ECONOMY)
   {
    lastSelectedIndex+=1;
    lastSelectedIndex=(lastSelectedIndex%facilitiesOptions.size());
   }
   return facilitiesOptions[lastSelectedIndex];
}
const string EconomySelection::toString() const 
{
    return "SelectionPolicy: eco";
}

EconomySelection* EconomySelection::clone() const 
{
    return new EconomySelection(*this);
}

//Sustainability Selection
SustainabilitySelection::SustainabilitySelection():SelectionPolicy(),lastSelectedIndex(-1){}

const FacilityType& SustainabilitySelection::selectFacility(const vector<FacilityType>& facilitiesOptions)
{
    lastSelectedIndex+=1;
    lastSelectedIndex=(lastSelectedIndex%facilitiesOptions.size());
   while(facilitiesOptions[lastSelectedIndex].getCategory()!=FacilityCategory::ENVIRONMENT)
   {
    lastSelectedIndex+=1;
    lastSelectedIndex=(lastSelectedIndex%facilitiesOptions.size());
   }
   return facilitiesOptions[lastSelectedIndex];
}

const string SustainabilitySelection::toString() const 
{
    return "SelectionPolicy: env";
}

SustainabilitySelection* SustainabilitySelection::clone() const 
{
    return new SustainabilitySelection(*this);
}