#include "Facility.h"

//FacilityType Class
FacilityType::FacilityType(const string &name, const FacilityCategory category, const int price, const int lifeQuality_score, const int economy_score, const int environment_score):
name(name), category(category), price(price), lifeQuality_score(lifeQuality_score),
economy_score(economy_score),environment_score(environment_score){}

const string& FacilityType::getName() const
{
    return name;
}

int FacilityType::getCost() const
{
    return price;
}

int FacilityType::getLifeQualityScore() const
{
    return lifeQuality_score;
}

int FacilityType::getEnvironmentScore() const
{
    return environment_score;
}

int FacilityType::getEconomyScore() const
{
    return economy_score;
}

FacilityCategory FacilityType::getCategory() const
{
    return category;
}

//Facility Class        
Facility::Facility(const string &name, const string &settlementName, const FacilityCategory category, const int price, const int lifeQuality_score, const int economy_score, const int environment_score):
FacilityType(name,category,price,lifeQuality_score,economy_score,environment_score),settlementName(settlementName),status(FacilityStatus:: UNDER_CONSTRUCTIONS),timeLeft(price){
    } 

Facility::Facility(const FacilityType &type, const string &settlementName):
    FacilityType(type),settlementName(settlementName),status(FacilityStatus:: UNDER_CONSTRUCTIONS),timeLeft(type.getCost()){
    }

const string & Facility::getSettlementName() const{
    return settlementName ;}

const int  Facility::getTimeLeft() const{
    return timeLeft;}

FacilityStatus Facility::step(){
    if (status == FacilityStatus::UNDER_CONSTRUCTIONS)
    {
        timeLeft--;
        if (timeLeft==0)
        {status= FacilityStatus::OPERATIONAL;}
    }
    return status;
}

void  Facility::setStatus(FacilityStatus status){
        this->status=status;
        }

const FacilityStatus&  Facility::getStatus() const{
    return status;}
       
const string  Facility::toString() const
    {
        if(status==FacilityStatus::OPERATIONAL)
        {return "FacilityName: "+ name + "\n FaciltySatus: OPERATIONAL";}
        return "FacilityName: "+ name + "\n FaciltySatus: UNDER_CONSTRUCTIONS";
        }


