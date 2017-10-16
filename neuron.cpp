#include "neuron.h"
#include <cmath>

///-----------------------------CONSTANTS----------------------------///
constexpr double Vthr = 20.0; //mV	potential to exceed for a spike
constexpr double Vreset = 0.0; //mV	initial potential
constexpr double TAU = 20.0; //ms
constexpr double C = 1.0; 
constexpr double R = TAU/C;
constexpr double Refractory_Time = 1.0; //ms	time after a spike during which the neuron will not respond
constexpr double J = 0.1; //mV	potential transmited for every spike signal
constexpr double D = 0.4; //ms	delay to transmit a spike signal


///----------------------------CONSTRUCTOR---------------------------///
Neuron::Neuron(double dt)
: time_(0), dt_(dt), membrane_potential_(0.0), nb_of_spikes_(0), refractory_(false), current_index_(0)
{
	for (auto& s : signals_) {
		s = 0;
	}
	
	size_of_vector_ = signals_.size();
}

///------------------------------GETTERS-----------------------------///
double Neuron::get_membrane_potential() const
{
	return membrane_potential_;
}

int Neuron::get_nb_of_spikes() const
{
	return nb_of_spikes_;
}

int Neuron::get_signals(unsigned int nb_of_time_steps) const
{
	unsigned int i(current_index_+nb_of_time_steps);
	if (i>=size_of_vector_) {
		return signals_[i-size_of_vector_];
	}
	return signals_[i];
}

///-----------------------------UPDATE-------------------------------///
void Neuron::update(unsigned long T, double Iext)
{
	unsigned int t(T-time_);
	
	//update of the signals
	
		//we're still in the array
	if (current_index_+t<size_of_vector_) {
		unsigned int new_index(current_index_+t);
		for (unsigned int i(current_index_); i<new_index ; ++i) {
			signals_[new_index] += signals_[i];
			signals_[i] = 0;
		}
		current_index_ = new_index;
		
		//we're out of the array
	} else {
		unsigned int new_index(current_index_+t-size_of_vector_);
		for (unsigned int i(current_index_); i<size_of_vector_ ; ++i) {	//end of the array
			signals_[new_index] += signals_[i];
			signals_[i] = 0;
		}
		for (unsigned int i(0); i<new_index ; ++i) {	//we go back to the begining of the array
			signals_[new_index] += signals_[i];
			signals_[i] = 0;
		}
		current_index_ = new_index;
	}
	
	//calculation of the new membrane potential
	double h(t*dt_);
	if (not refractory_) {
		membrane_potential_ = exp(-h/TAU) * membrane_potential_ + Iext*R*(1-exp(-h/TAU)) + J*signals_[current_index_];
	} else {
		unsigned int refractory_time_steps(Refractory_Time/dt_);
		if (T-spike_times_.back() > refractory_time_steps) {
			refractory_ = false;
			membrane_potential_ = exp(-h/TAU) * membrane_potential_ + Iext*R*(1-exp(-h/TAU)) + J*signals_[current_index_];
		}
	}
	signals_[current_index_] = 0;
	
	//a spike appears
	if (membrane_potential_ > Vthr) {
		nb_of_spikes_ += 1;
		spike_times_.push_back(T);
		refractory_ = true;
		membrane_potential_ = Vreset;
		
		transmit_signal(T);
	}
	
	//update of time
	time_ = T;
}

void Neuron::add_signals()
{
	if (not refractory_) {
		//modifying the membrane potential, counting the signals received
		membrane_potential_ += signals_[current_index_]*J;
		
		//checking new spikes
		if (membrane_potential_ > Vthr) {
			nb_of_spikes_ += 1;
			spike_times_.push_back(time_);
			refractory_ = true;
			membrane_potential_ = Vreset;
		
			transmit_signal(time_);	
		}
	}
	
	signals_[current_index_] = 0;
}

///------------------------------METHODS-----------------------------///
void Neuron::add_synapse(Neuron* const& N)
{
	synapses_.insert({synapses_.size(), N});
}

void Neuron::transmit_signal(unsigned long T)
{
	size_t nb_synapses(synapses_.size());
	
	if (nb_synapses > 0) {
		for (unsigned int i(0); i<nb_synapses ; ++i) {
			synapses_[i]->receive_signal(T);
		}
	}
}

void Neuron::receive_signal(unsigned long T)
{
	unsigned int delay(T-time_ + D/dt_);
	
		//we're still in the array
	if (current_index_+delay<size_of_vector_) {
		signals_[current_index_+delay] += 1;
		
		//we're out of the array
	} else {
		signals_[current_index_+delay-size_of_vector_] += 1;
	}
}

void Neuron::printSpikes() const
{
	std::cout << nb_of_spikes_ << " : ";
	for (size_t i=0; i < spike_times_.size() ; ++i) {
		std::cout << spike_times_[i] * dt_ << " ";
	}
	std::cout << std::endl;
}

void Neuron::delete_synapse(Neuron* const& N)
{
	size_t nb_synapses(synapses_.size());
	int index(-1);
	
	//if the synapse with N exists, the pointer is set at nullptr and index is the index of this pointer
	for (unsigned int i(0); i<nb_synapses; ++i) {
		if (synapses_[i] == N) {
			synapses_[i] = nullptr;
			index = i;
		}
	}
	
	//if the synapse with N exists: shifting synapses to have a linear indexation
	if (index > -1) {
		for ( unsigned int i(index); i<nb_synapses-1 ; ++i) {
			synapses_[i] = synapses_[i+1];
		}
		synapses_.erase(nb_synapses-1);
	}
}

///-----------------------------DESTRUCTOR---------------------------///
Neuron::~Neuron()
{
	while (synapses_.size() > 0) {
		delete_synapse(synapses_[0]);
	}
}
