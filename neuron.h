#ifndef NEURON_H
#define NEURON_H
#include <iostream>
#include <vector>
#include <array>
#include <unordered_map>
/*
///CONSTANTS
constexpr double Vthr = 20.0; //mV	potential to exceed for a spike
constexpr double Vreset = 0.0; //mV	initial potential
constexpr double TAU = 20.0; //ms
constexpr double C = 1.0; 
constexpr double R = TAU/C;
constexpr double Refractory_Time = 1.0; //ms	time after a spike during which the neuron will not respond
constexpr double J = 0.1; //mV	potential transmited for every spike signal
constexpr double D = 0.1; //ms	delay to transmit a spike signal*/

class Neuron {
	public:
	///CONSTRUCTOR
	Neuron(double dt);
	
	///GETTERS
	double get_membrane_potential() const;
	int get_nb_of_spikes() const;
	int get_signals(unsigned int nb_of_time_steps) const; /// Return the number of signals received for the time T+nb_of_time_steps
	
	///UPDATE
	void update(unsigned long T, double Iext = 0);
	void add_signals(); /// Take the number of signals received for the current time into account to calculate the new membrane potential
	
	///METHODS
	void add_synapse(Neuron* const& N); /// Add a neuron to which this one can transmit informations
	void transmit_signal(unsigned long T); /// Transmit a spike-signal to all neurons of synapses_ 
	void receive_signal(unsigned long T); /// Add a signal to the number of signals received
	void printSpikes() const; /// Print the number of spikes and at what time they happened
	void delete_synapse(Neuron* const& N); /// Delete the synapse to the neuron N
	
	///DESTRUCTOR
	~Neuron();
	
	private:
		///Time
	unsigned long time_;
	const double dt_;
		///Basic attributs
	double membrane_potential_;
	unsigned int nb_of_spikes_;
	std::vector<unsigned long> spike_times_;
	bool refractory_; //if the neuron just had a spike and its potential is 0 during a certain time
		///Synapses
	std::unordered_map<unsigned int, Neuron*> synapses_; //pointers to neurons which can receive a signal from this one
		///Counting of signals received
	std::array<unsigned int,20> signals_; //number of signals received from other neurons
	unsigned int current_index_; //indicate the current place in signals_
	unsigned int size_of_vector_; //size of the vector signals_
};

#endif
