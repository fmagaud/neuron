#ifndef NEURON_H
#define NEURON_H
#include <iostream>
#include <vector>

class Neuron {
	public:
	///CONSTRUCTOR
	Neuron(double dt);
	
	void add_synapse(Neuron* const& N);
	
	///GETTERS
	double get_membrane_potential() const;
	int get_nb_of_spikes() const;
	void printSpikes() const;
	int get_signals() const;
	
	///UPDATE
	void update(double T, double Iext = 0/*, double j = 0*/);
	
	void transmit_signal() const;
	
	void receive_signal();
	
	void add_signals();
	
	///DESTRUCTOR
	~Neuron();
	
	private:
	double membrane_potential_;
	int nb_of_spikes_;
	std::vector<double> spike_times_;
	int time_;
	bool refractory_;
	std::vector<Neuron*> synapses_;
	int signals_;
	double dt_;
};

#endif
