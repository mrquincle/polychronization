/***************************************************************************************************
 * @brief
 * @file TestNetwork.cpp
 *
 * This file is created at Almende B.V. It is open-source software and part of the Common Hybrid 
 * Agent Platform (CHAP). A toolbox with a lot of open-source tools, ranging from thread pools and 
 * TCP/IP components to control architectures and learning algorithms. This software is published 
 * under the GNU Lesser General Public license (LGPL).
 *
 * It is not possible to add usage restrictions to an open-source license. Nevertheless, we 
 * personally strongly object against this software used by the military, in the bio-industry, for 
 * animal experimentation, or anything that violates the Universal Declaration of Human Rights.
 *
 * Copyright © 2012 Anne van Rossum <anne@almende.com>
 ***************************************************************************************************
 * @author 	Anne C. van Rossum
 * @date	May 14, 2012
 * @project	Replicator FP7
 * @company	Almende B.V. & Distributed Organisms B.V.
 * @case	Self-organised criticality
 **************************************************************************************************/

#include <stdlib.h>
#include <iostream>

#include <Network.h>
#include <Plot.h>

#define NETWORK_SIZE		1000
#define TIME_SPAN			1000*5

using namespace std;

/**
 * We want to show http://www.izhikevich.org/publications/spnet.htm
 */
int main() {
	cout << "Create network" << endl;
	Network *network = new Network();

	cout << "Add " << NETWORK_SIZE << " neurons" << endl;
	for (int i = 0; i < (float)NETWORK_SIZE * 0.8; ++i) {
		network->addNeuron(NT_POLYCHRONOUS_EXCITATORY, NS_EXCITATORY, NL_HIDDEN);
	}
	for (int i = 0; i < (float)NETWORK_SIZE * 0.2; ++i) {
		network->addNeuron(NT_POLYCHRONOUS_INHIBITORY, NS_INHIBITORY, NL_HIDDEN);
	}

	cout << "Make it sparsely connected" << endl;
	network->addSynapses(0.1);


	Plot plot;
	plot.Init(PL_GRID);
	plot.SetFileName("test", PL_GRID);
	DataContainer &data = plot.GetData();

	std::vector<bool> spikes;
	std::vector<DC_TYPE> activity;
	for (int t = 0; t < TIME_SPAN; ++t) {
		network->tick();
		int r = network->getSpikes(spikes);
		if (!(t % 100)) {
			cout << "[t=" << t << "] spike count: " << r << endl;
		}
		activity.clear();
//		cout << "t=" << t << ": ";
		for (int i = 0; i < spikes.size(); ++i) {
			activity.push_back(spikes[i]);
//			cout << (spikes[i] ? "! " : "X ");
		}
//		cout << endl;
		data.addItems(activity, t);
	}
	plot.Draw(PL_GRID);

	return EXIT_SUCCESS;
}




