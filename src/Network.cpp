/***************************************************************************************************
 * @brief
 * @file Network.c
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

#include <Network.h>

#include <assert.h>
#include <stddef.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include <iostream>

using namespace std;

/**
 * The STDP rule described in Izhikevich article "Polychronization: Computation with Spikes"
 * is implemented by using an array with the exponential values for all t precalculated (the
 * interspike time t can not be larger than the maximum delay, so it's a small array). The tau
 * parameters and A parameters as in the article are used:
 * - LTP = 0.10 * exp(-t/20) with t in ms
 * - LTD = 0.12 * exp(+t/20) with t in ms
 *
 * The first item is when there is an interval of 0. So, that value is conflicting in both
 * sequences. When spikes are really at the same time, no weight change occurs.
 * @remark A float (6 significant decimals) is used.
 */
//const float LTD[16] = {-0.120000, -0.114148, -0.108580, -0.103285, -0.098248, -0.093456, -0.088898, -0.084563,
//		-0.080438, -0.076515, -0.072784, -0.069234, -0.065857, -0.062645, -0.059590, -0.056684};
/**
 * @see LTP
 */
//const float LTP[16] = {0.100000, 0.095123, 0.090484, 0.086071, 0.081873, 0.077880, 0.074082, 0.070469,
//		0.067032, 0.063763, 0.060653, 0.057695, 0.054881, 0.052205, 0.049659, 0.047237};

Network::Network() {
	srand48(time(NULL));
	t = 0;
}

/**
 * Add a new neuron to the network
 */
void Network::addNeuron(NeuronType type, NeuronSign sign, NeuronLocation loc) {
	ConnNeuron *cn = new ConnNeuron(neurons.size());
	cn->neuron = new Neuron(type, sign, loc);
	cn->outgoing = NULL;
	neurons.push_back(cn);
}

/**
 * Add excitatory and inhibitory synapses. The delays and weights are initialized as described in the
 * matlab file from Izhikevich: http://www.izhikevich.org/publications/spnet.m
 */
void Network::addSynapse(ConnNeuron *src, ConnNeuron *target) {
	if (src->outgoing == NULL) {
		src->outgoing = new SYNAPSES(0);
	}
	Synapse *synapse = new Synapse(src, target);
	src->outgoing->push_back(synapse);
	if (src->neuron->getSign() == NS_EXCITATORY) {
		synapse->weight = 6.0;
		synapse->delay = (int)(drand48()*HISTORY_SIZE);
	}
	else if (src->neuron->getSign() == NS_INHIBITORY) {
		synapse->weight = -5.0;
		synapse->delay = 1;
	}
	synapses.push_back(synapse);
}

/**
 * Add outgoing synapses
 */
void Network::addSynapses(ConnNeuron *src, float fraction) {
	NEURONS *nn;
	if (fraction == 1.0) {
		nn = &neurons;
	} else {
		nn = new NEURONS(0);
		getNeurons(*nn, fraction);
	}
	NEURONS::iterator it;
	for (it = nn->begin(); it != nn->end(); ++it) {
		if ((*it)->id != src->id) {
			addSynapse(src, *it);
		}
	}
	if (fraction != 1.0)
		delete nn;
}

void Network::getNeurons(std::vector<ConnNeuron*> &subset, float fraction) {
	NEURONS::iterator it;
	subset.clear();
	for (it = neurons.begin(); it != neurons.end(); ++it) {
		if (drand48() < fraction) {
			subset.push_back(*it);
		}
	}
}

void Network::addSynapses(float fraction) {
	NEURONS::iterator it;
	for (it = neurons.begin(); it != neurons.end(); ++it) {
		addSynapses(*it, fraction);
	}
}

void Network::tick() {
	++t;
	updateSpikes();
	updateSynapses();
	updateNeurons();
}

int Network::getSpikes(std::vector<bool> & activity) {
	NEURONS::iterator it;
	activity.clear();
	int r = 0;
	for (it = neurons.begin(); it != neurons.end(); ++it) {
		bool raised = (*it)->raised();
		activity.push_back(raised);
		if (raised) ++r;
	}
	assert (activity.size() == neurons.size());
	return r;
}

void Network::updateSpikes() {
	NEURONS::iterator it;
	for (it = neurons.begin(); it != neurons.end(); ++it) {
		(*it)->advance();
		if ((*it)->neuron->fired()) (*it)->raise();
	}
}

/**
 * Updated function after Freek's suggestions. Needs to be tested.
 */
void Network::updateSynapses() {
	SYNAPSES::iterator it;
	for (it = synapses.begin(); it != synapses.end(); ++it) {
		// adjust only excitatory connections
		if ((*it)->pre->neuron->getSign() == NS_INHIBITORY) continue;

		// if a pre synaptic spike reaches the post-synaptic neuron
		if ((*it)->pre->raised((*it)->delay)) {
			// apply LTD with the most recent post-synaptic spike
			int first_spike = (*it)->post->first();
			if (first_spike >= 0) {
//				if (t > 1990)
//					cout << "Increment weight from " << (*it)->weight;
				(*it)->weight += 0.12 * exp(+first_spike/(NN_VALUE)HISTORY_SIZE);
//				if (t > 1990)
//					cout << " to " << (*it)->weight << endl;
				// increase the post-synaptic neuron's input
				(*it)->post->input += (*it)->weight / NN_VALUE(3);
			}
		}
		// if a post-synaptic spike occurs
		if ((*it)->post->raised()) {
			// apply LTP with the most recent pre-synaptic spike that has arrived at the post-synaptic neuron,
			// so occurred at least "delay" ms ago
			int first_spike = (*it)->pre->first((*it)->delay);
			if (first_spike >= 0) {
//				if (t > 1990)
//					cout << "Decrement weight from " << (*it)->weight;
				(*it)->weight -= 0.10 * exp(-first_spike/(NN_VALUE)HISTORY_SIZE);
//				if (t > 1990)
//					cout << " to " << (*it)->weight << endl;
			}
		}

		if ((*it)->weight > 10.0)
			(*it)->weight = 10.0;
		if ((*it)->weight < -10.0)
			(*it)->weight = -10.0;
	}
}

/**
 * For every non-input neuron update is called once with the accumulated current figure
 * previously calculated in propagateSpikes. In the case of a neuron with 8 simultaneously
 * spiking input neurons, this figure might become the summation of all weights, say
 * 8*6 = 48 mA.
 */
void Network::updateNeurons() {
	NEURONS::iterator it;
	for (it = neurons.begin(); it != neurons.end(); ++it) {
		assert ((*it)->neuron != NULL);
		if ((*it)->neuron->getLoc() != NL_INPUT) {
//			cout << (*it)->input << endl;
			(*it)->neuron->update((*it)->input);
			if (drand48() < 0.5)
				(*it)->input = 0;
			else
				(*it)->input = 20;
		}
	}
}


