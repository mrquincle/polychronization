/***************************************************************************************************
 * @brief
 * @file Neuron.h
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

#ifndef NEURON_H_
#define NEURON_H_

enum NeuronType {
	NT_TONIC_SPIKING,
	NT_PHASIC_SPIKING,
	NT_TONIC_BURSTING,
	NT_PHASIC_BURSTING,
	NT_MIXED_MODE,
	NT_SPIKE_FREQ_ADAPT,
	NT_CLASS1_EXC,
	NT_CLASS2_EXC,
	NT_SPIKE_LATENCY,
	NT_SUBTHRESHOLD_OSC,
	NT_RESONATOR,
	NT_INTEGRATOR,
	NT_REBOUND_SPIKE,
	NT_REBOUND_BURST,
	NT_THRESH_VARIABILITY,
	NT_BISTABILITY,
	NT_DAP,
	NT_ACCOMODATION,
	NT_INHIB_IND_SPIKING,
	NT_INHIB_IND_BURSTING,
	NT_POLYCHRONOUS_EXCITATORY,
	NT_POLYCHRONOUS_INHIBITORY,
	NT_COUNT						// total number of neuron types
};

enum NeuronSign {
	NS_EXCITATORY,
	NS_INHIBITORY,
	NS_COUNT
};

enum NeuronLocation {
	NL_INPUT,
	NL_HIDDEN,
	NL_OUTPUT,
	NL_COUNT						// total number of different types of neuron locations
};

typedef float NN_VALUE;

/**
 * An Izhikevich neuron.
 */
class Neuron {
public:
	//! Set the type of the neuron and update its internal values correspondingly
	Neuron(NeuronType type, NeuronSign sign, NeuronLocation loc);

	//! The update and the fire functions are separated. This is on purpose. It is recommended
	//! to first update all neurons so all new events have been propagated through the network
	//! and only then check which ones did fire.
	void update(NN_VALUE input);

	//! The neuron did fire in the last update event
	bool fired();

	NeuronLocation getLoc() const {
		return loc;
	}

	void setLoc(NeuronLocation loc) {
		this->loc = loc;
	}

	NeuronSign getSign() const {
		return sign;
	}

	void setSign(NeuronSign sign) {
		this->sign = sign;
	}

	NeuronType getType() const {
		return type;
	}

	void setType(NeuronType type) {
		this->type = type;
	}


private:
	NN_VALUE v; 	//< membrane_potential
	NN_VALUE u; 	//< membrane_recovery
	NN_VALUE a;		//< membrane_recovery_timescale
	NN_VALUE b;		//< membrane_recovery_sensitivity, set to +20.0 mV by default
	NN_VALUE c;		//< membrane_potential_reset, set to -65.0 mV by default
	NN_VALUE d;		//< membrane_recovery_reset

	NeuronType type;

	NeuronSign sign;

	NeuronLocation loc;

	bool spike;
};


#endif /* NEURON_H_ */
