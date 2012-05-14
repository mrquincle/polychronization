Polychronization
================

# Introduction
Polychronization is an effect that occurs in recurrent neural networks with synaptic delays. It basically narrows down to the fact that neurons spike when pre-synaptic neuron spikes arrive at the same time. However, these pre-synaptic spikes have not been generated at the same time, but at a time that precisely corresponds with the delay after which they arrive at a post-synaptic neuron. Considering the fact that multiple spikes are necessary to have a subsequent neuron spike on its turn, it is possible to analyse a network and search for the cases in which such an event happens not just once, but multiple times with a group of neurons. Such a group of neurons is called a polychronous group.

# Implementation
See the Neuron.cpp file for a concise implementation of a spiking neuron (also following Izhikevich). 

The following picture visualizes 1000 neurons of which there are 800 excitatory. The outgoing synapses of these are adapted following an STDP rule (spike-time dependent plasticity). The outgoing synapses of the inhibitory neurons are not adapted.

![alt text](https://github.com/mrquincle/polychronization/raw/master/doc/spikes.jpeg "Spikes in a network of 1000 neurons")

The implementation tries to follow that of Izhikevich as close as possible, but uses C++ classes and std containers. It is slower, basically because if I don't care about speed I can program faster. :-) The neuron implementation is fine, the spike representation is moderately slow, but especially the network representation is meant for sparse networks (every neuron has a variable list of outgoing synapses).

# More information
For more information, look at http://www.izhikevich.org/publications/spnet.htm and the corresponding publications by Izhikevich. 


