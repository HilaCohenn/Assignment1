#include "Settlement.h";
using namespace std;
#pragma once
#include <string>
#include <vector>

class Facility;

enum class SettlementType {
    VILLAGE,
    CITY,
    METROPOLIS,
   
};

class Settlement {
    public:
        Settlement(const string &name, SettlementType type): name(name), type(type){}
        const string &getName() const{
            return name;}
        SettlementType getType() const{
            return type;}
        const string toString() const{
            if (type==SettlementType::CITY)
                return "the settlement "+name+" is a City." ;
            if (type==SettlementType::VILLAGE)
                return "the settlement "+name+" is a Village" ;
            return "the settlement "+name+" is a Metropolis" ;
        }
        

        private:
            const string name;
            SettlementType type;
};