#pragma once
#include <string>
#include <vector>
#include "Facility.h"
#include <iostream>
using namespace std;
using std::string;
using std::vector;




// Hila: facility

Facility:: Facility(const string &name, const string &settlementName, const FacilityCategory category, const int price, const int lifeQuality_score, const int economy_score, const int environment_score)
: FacilityType(name, category, price, lifeQuality_score, economy_score, environment_score), settlementName(settlementName), status(FacilityStatus::UNDER_CONSTRUCTIONS), timeLeft(10){}

Facility:: Facility(const FacilityType &type, const string &settlementName) 
: FacilityType(type), settlementName(settlementName), status(FacilityStatus::UNDER_CONSTRUCTIONS), timeLeft(type.getCost()) {}
        
        
const string& Facility::getSettlementName() const {
    return settlementName;
}

const int Facility::getTimeLeft() const {
    return timeLeft;
}

// to work on this
FacilityStatus Facility:: step() {
    timeLeft--;
}

void Facility::setStatus(FacilityStatus status) {
    this->status = status; 
}
const FacilityStatus& Facility:: getStatus() const {
    return status;
}

const string Facility:: toString() const {
    string Category;
    if (getCategory() == FacilityCategory::LIFE_QUALITY)
        Category ="Life Quality";
    if (getCategory() == FacilityCategory::ECONOMY)
        Category ="Economy";
    if (getCategory() == FacilityCategory::ENVIRONMENT)
        Category = "Environment";
    string toString =  "Facility Name: " + getName() + ", Settlement Name: " + settlementName 
    + ", Category: " + Category;
    
};