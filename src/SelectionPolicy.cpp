#pragma once
#include <string>
#include <vector>
#include "SelectionPolicy.h"
#include <iostream>
#include <algorithm>
using namespace std;
using std::string;
using std::vector;

// hila

//naive
NaiveSelection::NaiveSelection() : lastSelectedIndex(-1) {}
const FacilityType& NaiveSelection::selectFacility(const vector<FacilityType>& facilitiesOptions){
    lastSelectedIndex++;
    return facilitiesOptions[lastSelectedIndex];
}
const string NaiveSelection::toString() const {
    return "NaiveSelection";
}

NaiveSelection* NaiveSelection::clone() const {
    return new NaiveSelection(*this);
}

// Balanced
BalancedSelection::BalancedSelection(int LifeQualityScore, int EconomyScore, int EnvironmentScore)
    : LifeQualityScore(LifeQualityScore), EconomyScore(EconomyScore), EnvironmentScore(EnvironmentScore) {}

const FacilityType& BalancedSelection::selectFacility(const vector<FacilityType>& facilitiesOptions){
    const FacilityType* bestFacility = nullptr;
    int bestBalance = std::numeric_limits<int>::max(); 

    for (const FacilityType& facility : facilitiesOptions) {
        int newLifeQualityScore = LifeQualityScore + facility.getLifeQualityScore();
        int newEconomyScore = EconomyScore + facility.getEconomyScore();
        int newEnvironmentScore = EnvironmentScore + facility.getEnvironmentScore();

        int maxScore = std::max({newLifeQualityScore, newEconomyScore, newEnvironmentScore});
        int minScore = std::min({newLifeQualityScore, newEconomyScore, newEnvironmentScore});
        int balance = maxScore - minScore;

        if (balance < bestBalance) {
            bestBalance = balance;
            bestFacility = &facility;
        }
    }
    return *bestFacility;
}

const string BalancedSelection::toString() const {
    return "BalancedSelection";
}

BalancedSelection* BalancedSelection::clone() const {
    return new BalancedSelection(*this);
};