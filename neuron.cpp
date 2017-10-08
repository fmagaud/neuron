#include "neuron.h"
#include <cmath>

constexpr double Vthr = 20.0; //mV
constexpr double Vreset = 0.0; //mV
constexpr double TAU = 20.0; //ms
constexpr double C = 1.0; 
constexpr double R = TAU/C;
constexpr double Refractory_Time = 1.0; //ms


///CONSTRUCTOR
Neuron::Neuron(double time)
: membrane_potential_(0.0), nb_of_spikes_(0), time_(time), refractory_(false)
{}

///GETTERS
double Neuron::get_membrane_potential() const
{
	return membrane_potential_;
}

int Neuron::get_nb_of_spikes() const
{
	return nb_of_spikes_;
}

void Neuron::update(double Iext, double T)
{
	double t(T-time_);
	
	if (not refractory_) {
		membrane_potential_ = exp(-t/TAU) * membrane_potential_ + Iext*R*(1-exp(-t/TAU));
	} else {
		if (T-spike_times_.back() > Refractory_Time) {
			refractory_ = false;
			membrane_potential_ = exp(-t/TAU) * membrane_potential_ + Iext*R*(1-exp(-t/TAU));
		}
	}
	
	if (membrane_potential_ > Vthr) {
		nb_of_spikes_ += 1;
		spike_times_.push_back(T);
		refractory_ = true;
		membrane_potential_ = Vreset;
	}
	
	time_ = T;	
}

///DESTRUCTOR
Neuron::~Neuron()
{}
