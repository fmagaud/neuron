#ifndef BRAIN_H
#define BRAIN_H
#include <iostream>
#include <vector>
#include <unordered_map>
#include "neuron.h"

class Brain {
	public:
	///CONSTRUCTOR
	Brain(unsigned int nb_neurons, double dt);
	
	///UPDATE
	void update(unsigned long T, double Iext);
	
	///METHODS
	void add_connection(unsigned int transmitter_neuron_index, unsigned int receiver_neuron_index); /// Add a synapse connection between the transmitter neuron and the receiver one
	void print(); /// Print the time and the membrane potential plus the number of spikes of each neuron
	//void save_into_file(std::ofstream file) const; /// Save into the file the time and the membrane potential of each neuron
	void delete_neuron(unsigned int index); /// Delete a neuron from the brain
	
	///DESTRUCTOR
	~Brain();
	
	private:
	std::unordered_map<unsigned int, Neuron*> network_; //contains pointers to all neurons of the brain
	unsigned int nb_neurons_;
	unsigned long time_; //clock
	const double dt_; //time step
};

#endif
