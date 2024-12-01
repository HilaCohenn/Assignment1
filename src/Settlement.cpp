#include "Settlement.h"
using namespace std;

Settlement::Settlement(const string &name, SettlementType type): name(name), type(type){}

const string & Settlement::getName() const{
    return name;}

SettlementType  Settlement::getType() const{
    return type;}
    
const string  Settlement::toString() const{
    if (type==SettlementType::CITY)
        return "the settlement "+name+" is a City." ;
    if (type==SettlementType::VILLAGE)
        return "the settlement "+name+" is a Village" ;
    return "the settlement "+name+" is a Metropolis" ;}