#ifndef NEURON_H
#define NEURON_H
#include <iostream>
#include <vector>

class Neuron {
	public:
	///CONSTRUCTOR
	Neuron(double time = 0);
	
	///GETTERS
	double get_membrane_potential() const;
	int get_nb_of_spikes() const;
	
	void update(double Iext, double T);
	
	///DESTRUCTOR
	~Neuron();
	
	private:
	double membrane_potential_;
	int nb_of_spikes_;
	std::vector<double> spike_times_;
	double time_;
	bool refractory_;
};

#endif
