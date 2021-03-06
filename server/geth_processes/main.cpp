#include "geths.h"

#include <iostream>
#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <stdint.h>
#include <stdexcept>
#include <string>
#include <sstream>
#include <fstream>
#include <limits>
#include <algorithm>
#include <sys/types.h>
#include <map>

#include <vector>

using namespace std;

struct simulationState{
	string baseDir; /* Basedir of the controller folder */
	string mappingPath;
	string regenerateFile;
	string blockchainPath;
	string datadirBase;
	string baseDirRaw;
	int basePort;
	bool useClassicalApproach;
} simulationParams;


map<int, string> enodes;
map<int, string> coinbaseAddresses;
string interface; // Smart contract interface


//processes that need to happen:

//adding and dropping peers

//interface to smart contract (from js!)
// smartContractInterfaceStringBg(robotId, interface, contractAddress, "vote", args, 4, opinionInt, nodeInt, simulationParams.blockchainPath);


//deploy the contracts

//pass the robots their enode /addresses /ids?

//the interface is the .abi!

//use the background geth calls!!


int main(){
    int nodeInt = 0;
    int numRobots = 4;


	//set up the global simulation parameters	
	simulationParams.basePort = 33200;
	simulationParams.baseDirRaw = "";


	for (int robotId = 0; robotId < numRobots; robotId++){
    	//get dataDir for robot
    	ostringstream blockPathStream;
		blockPathStream << "eth_data/data" << robotId;
		cout << "blockchainPath is " << blockPathStream.str() << endl;
    	simulationParams.blockchainPath = "eth_data/data";


	    /* Find out on which cluster node this robot's geth process should be executed */
	    ostringstream genesisPathStream;
	    genesisPathStream << simulationParams.baseDirRaw << "genesis/genesis" << simulationParams.basePort  << ".json";
	    string genesisPath = genesisPathStream.str();
	    
	    geth_init(robotId, nodeInt, simulationParams.basePort, simulationParams.blockchainPath, genesisPath);
	    start_geth(robotId, nodeInt, simulationParams.basePort, simulationParams.blockchainPath);
	    createAccount(robotId, nodeInt, simulationParams.basePort, simulationParams.blockchainPath);

	    //deploy contracts
	 	string res = deploy_contract(robotId, "interface.txt", "data.txt", "contractTemplate.txt", nodeInt,  simulationParams.blockchainPath);
	 	cout << "result is " << res << endl;

    	enodes[robotId] = get_enode(robotId, nodeInt, simulationParams.basePort, simulationParams.blockchainPath);
    	string enode = enodes[robotId];
    	unlockAccount(robotId, "test", nodeInt, simulationParams.basePort, simulationParams.blockchainPath);


	    coinbaseAddresses[robotId] = getCoinbase(robotId, nodeInt, simulationParams.basePort, simulationParams.blockchainPath);	    
	    prepare_for_new_genesis(robotId, nodeInt, simulationParams.basePort, simulationParams.blockchainPath);
	}
	for (int robotId = 0; robotId < numRobots; robotId++){
		cout << "address is   " << coinbaseAddresses[robotId] << endl;
		cout << "enode is   " << enodes[robotId] << endl;
	}
}