/***************************************************************************************************
 * @brief
 * @file Network.h
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


#ifndef NETWORK_H_
#define NETWORK_H_

#include <list>
#include <vector>
#include <queue>
#include <Neuron.h>
#include <iostream>

struct Synapse;

typedef std::vector<Synapse*> SYNAPSES;

typedef std::deque<bool> HISTORY;

#define HISTORY_SIZE 20 //16

class ConnNeuron {
public:

	ConnNeuron(int id) {
		for (int i = 0; i < HISTORY_SIZE; ++i)
			history.push_back(false);
		input = NN_VALUE(0);
		this->id = id;
	}
	Neuron *neuron;
	SYNAPSES *outgoing;
	HISTORY history;

	NN_VALUE input;

	//! An identifier makes things just so easy
	int id;

	inline bool raised(int delay=0) { return history[delay]; }

	inline void raise() {
		history[0] = true;
	}

	inline int first(int delay=0) {
		for (int i = delay; i < history.size(); ++i) {
			if (history[i]) return i;
		}
		return -1;
	}

	void advance() {
//		print();
		history.push_front(false);
		history.pop_back();
//		print();
	}

	void print() {
//		std::cout << "Spike history [" << id << "]: ";
		for (int i = 0; i < history.size(); ++i) {
			std::cout << history[i];
		}
//		std::cout << " and first is " << first(0);
//		std::cout << std::endl;
	}
};

class Synapse {
public:
	Synapse(ConnNeuron *src, ConnNeuron *dest) {
		delay = 0;
		weight = NN_VALUE(0);
		pre = src;
		post = dest;
	}
	~Synapse() {};
	ConnNeuron *pre;
	ConnNeuron *post;
	int delay;
	NN_VALUE weight;
};

typedef std::vector<ConnNeuron*> NEURONS;

class Network {
public:
	Network();

	~Network() {};

	//! Add a neuron
	void addNeuron(NeuronType type, NeuronSign sign, NeuronLocation loc);

	//! Add a synapse between two neurons
	void addSynapse(ConnNeuron *src, ConnNeuron *target);

	//! Add synapses to all other neurons
	void addSynapses(ConnNeuron *src, float fraction = 1.0);

	//! Create network
	void addSynapses(float fraction);

	//! Update the entire network
	void tick();

	//! Get a fraction of the neurons
	void getNeurons(std::vector<ConnNeuron*> &subset, float fraction);

	//! Get the spiking neurons in the network (an activity vector)
	int getSpikes(std::vector<bool> & activity);

	//! Update all neurons given new calculated input
	void updateNeurons();

	//! Check which neurons did fire and update the spike history
	void updateSpikes();

	//! Propagate the spikes over the synapses and adapt the weights
	void updateSynapses();

	//! Create a copy of a neuron
//	Neuron *copy(Neuron *src);

	//! Move outgoing synapses from one neuron to another
//	void moveOutgoingSynapses(Neuron *src, Neuron *target);

	//! Add a synapse between two neurons
//	struct Synapse *addSynapse(Neuron *src, Neuron *target);

private:
	NEURONS neurons;

	SYNAPSES synapses;

	//! For debugging purposes
	int t;
};


#endif /* NETWORK_H_ */
