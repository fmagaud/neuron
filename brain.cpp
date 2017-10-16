#include "brain.h"
#include <cmath>

///CONSTRUCTOR
Brain::Brain(unsigned int nb_neurons, double dt)
: nb_neurons_(nb_neurons), time_(0), dt_(dt)
{
	for (unsigned int i(0) ; i < nb_neurons ; ++i) {
		network_.insert({i,new Neuron(dt)});
	}
}

///UPDATE
void Brain::update(unsigned long T, double Iext)
{
	if (nb_neurons_ > 0) {
		//update all neurons and the first one with an input current of Iext (0 for the others)
		network_[0]->update(T, Iext);
		for (unsigned int i(1); i<nb_neurons_; ++i) {
			network_[i]->update(T);
		}
		
		//only when there is no delay or the delay is smaller than the time step : otherwise no additional signals will appear (->useless)
		/**
		int signals(0);
		do {
			//recalculation of the membrane potentials, counting the signals received
			for (unsigned int i(0); i<nb_neurons_; ++i) {
				network_[i]->add_signals();
			}
		
			//checking if some signal haven't been added to the membrane potential
			for (unsigned int i(0); i<nb_neurons_; ++i) {
				signals += network_[i]->get_signals(0);
			}
			
		} while (signals != 0);
		**/
	}
	
	//update of time
	time_ = T;
}

///METHODS
void Brain::add_connection(unsigned int transmitter_neuron_index, unsigned int receiver_neuron_index)
{
	if (transmitter_neuron_index<nb_neurons_ and receiver_neuron_index<nb_neurons_) {
		network_[transmitter_neuron_index]->add_synapse(network_[receiver_neuron_index]);
	}
}

void Brain::print()
{
	std::cout << "T = " << time_*dt_ << "ms : ";
	
	for (unsigned int i(0); i<nb_neurons_ ; ++i) {
		std::cout << "N" << i+1 << " " << network_[i]->get_membrane_potential() << " , " << network_[i]->get_nb_of_spikes() << "  ";
	}
	std::cout << std::endl;
}

/*void Brain::save_into_file(std::ofstream file) const
{
	file << "T = " << time*dt << "ms : ";
	
	for (unsigned int i(0); i<nb_neurons_ ; ++i) {
		file << "N" << i+1 << " " << network_[i]->get_membrane_potential() << "  ";
	}
	file << std::endl;
}*/

void Brain::delete_neuron(unsigned int index) 
{	
	if (index <= nb_neurons_ - 1) {
		Neuron* const& neuron(network_[index]);
		
		//delete the synapses of the neuron with all neurons "before" it
		for (unsigned int i(0); i<index ; ++i) {
			network_[i]->delete_synapse(neuron);
		}
		//delete the synapses of the neuron with all neurons "after" it
		if (index != nb_neurons_ - 1) {
			for (unsigned int j(index+1); j<nb_neurons_ ; ++j) {
				network_[j]->delete_synapse(neuron);
			}
		}
		
		//delete the neuron inside the brain
		delete neuron;
		network_[index] = nullptr;
		
		//shifting neurons to have a linear indexation
		--nb_neurons_;
		for (unsigned int i(index); i<nb_neurons_ ; ++i) {
			network_[i]= network_[i+1];
		}
		network_.erase(nb_neurons_-1);
	}
}
	

///DESTRUCTOR
Brain::~Brain()
{
	while (nb_neurons_ > 0) {
		delete_neuron(0);
	}
}
