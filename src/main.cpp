using namespace std;
#include "Simulation.h"
#include <iostream>
Simulation* backup = nullptr;

int main(int argc, char** argv){
    if(argc!=2){
        cout << "usage: simulation <config_path>" << endl;
        return 0;
    }
    string configurationFile = argv[1];
    *Simulation simulation(configurationFile);
    Simulation.start();
    if(backup!=nullptr){
    	delete backup;
    	backup = nullptr;
    } 
    return 0;
}