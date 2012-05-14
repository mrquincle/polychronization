/***************************************************************************************************
 * @brief
 * @file Neuron.cpp
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

#include <Neuron.h>

/**
 * The configuration for each neuron type is given by only 5 parameters in
 * Izhikevich models. The parameters are ordered: a b c d I
 */
float NeuronConfig[]= {
		0.02,  0.2,  -65,   6,    14,  // tonic spiking
		0.02,  0.25, -65,   6,     0.5,// phasic spiking
		0.02,  0.2,  -50,   2,    15,  // tonic bursting
		0.02,  0.25, -55,   0.05,  0.6,// phasic bursting
		0.02,  0.2,  -55,   4,    10,  // mixed mode
		0.01,  0.2,  -65,   8,    30,  // spike frequency adaptation
		0.02, -0.1,  -55,   6,     0,  // Class 1
		0.2,   0.26, -65,   0,     0,  // Class 2
		0.02,  0.2,  -65,   6,     7,  // spike latency
		0.05,  0.26, -60,   0,     0,  // subthreshold oscillations
		0.1,   0.26, -60,  -1,     0,  // resonator
		0.02, -0.1,  -55,   6,     0,  // integrator
		0.03,  0.25, -60,   4,     0,  // rebound spike
		0.03,  0.25, -52,   0,     0,  // rebound burst
		0.03,  0.25, -60,   4,     0,  // threshold variability
		1,     1.5,  -60,   0,   -65,  // bistability
		1,     0.2,  -60, -21,     0,  // DAP
		0.02,   1,   -55,   4,     0,  // accomodation
		-0.02, -1,   -60,   8,    80,  // inhibition-induced spiking
		-0.026,-1,   -45,   0,    80}; // inhibition-induced bursting

/**
 * Check the parameters at http://vesicle.nsi.edu/users/izhikevich/publications/figure1.m
 * To see the graphs, use testNeuron, however, adapt the time scale and the input each time.
 */
Neuron::Neuron(NeuronType type, NeuronSign sign, NeuronLocation loc) {
	this->type = type;
	this->sign = sign;
	this->loc = loc;
	switch (type) {
	case NT_TONIC_SPIKING:
		a = +0.02; b = +0.20; c = -65.0; d = +6.00;
		v = -70.0; u = v * b;
		break;
	case NT_PHASIC_SPIKING:
		a = +0.02; b = +0.25; c = -65.0; d = +6.00;
		v = -64.0; u = v * b;
		break;
	case NT_INTEGRATOR:
		a = +0.02; b = -0.10; c = -55.0; d = +6.00;
		v = -60.0; u = v * b;
		break;
	case NT_POLYCHRONOUS_EXCITATORY:
		a = +0.02; b = +0.20; c = -65.0; d = +8.00;
		v = -65.0; u = v * b;
		break;
	case NT_POLYCHRONOUS_INHIBITORY:
		a = +0.1; b = +0.20; c = -65.0; d = +2.00;
		v = -65.0; u = v * b;
		break;
	default:
		v = -64.0;
		u = v * 0.2;
		b = +0.25;
		c = -65.0;
		if (sign == NS_EXCITATORY) {
			a = +0.02; //should be randomized
			d = +6.00;
		} else {
			a = +0.10;
			d = +2.00;
		}
		break;
	}
}

/**
 * The potentials and other (membrane) parameters are updated, subsequently those values are
 * checked against a certain threshold and it is decided if the neuron fires or not. When a
 * neuron fires it does not iterate through its connections, because they are not known to
 * it. It just returns a "fired" state.
 * First it has to be known which neurons spiked, if simultaneous arrival of spikes from
 * neurons earlier and later in the iteration is important, first all spikes have to be
 * retrieved. If delays are also important, those spikes have to be stored and also the time
 * they will be travelling to the next neuron. However, that is the responsibility of the
 * network, not of this file.
 */
bool Neuron::fired() {
	return spike;
}

/**
 * Besides checking for the firing condition, the internal variables have to be updated. The
 * neuron does not know anything about the others, so the sum over all its inputs is provided
 * as one argument: input.
 */
void Neuron::update(NN_VALUE input) {
	//printf("Parameters: %f, %f becomes with input I=%f: ", n->v, n->u, I);
	NN_VALUE euler_step; int euler;
	switch (type) {
	case NT_INTEGRATOR:
		euler_step = 0.25;
//		for (int e = 0; e < (NN_VALUE)1/euler_step; ++e)
			v += euler_step * ((0.04 * v + 4.1) * v + 108.0 - u + input);
		break;
	default:
		euler_step = 0.5;
//		for (int e = 0; e < (NN_VALUE)1/euler_step; ++e)
			v += euler_step * ((0.04 * v + 5.0) * v + 140.0 - u + input);
		break;
	}
	u += a * (b * v - u);

	spike = false;
	if (v >= 30.0) {
		v = c;
		u += d;
		spike = true;
	}
}
