#include "neuron.h"
#include <cmath>

constexpr double Vthr = 20.0; //mV
constexpr double Vreset = 0.0; //mV
constexpr double TAU = 20.0; //ms
constexpr double C = 1.0; 
constexpr double R = TAU/C;
constexpr double Refractory_Time = 1.0; //ms
constexpr double J = 0.1; //mV


///CONSTRUCTOR
Neuron::Neuron(double dt)
: membrane_potential_(0.0), nb_of_spikes_(0), time_(0), refractory_(false), signals_(0), dt_(dt)
{}

void Neuron::add_synapse(Neuron* const& N)
{
	synapses_.push_back(N);
}

///GETTERS
double Neuron::get_membrane_potential() const
{
	return membrane_potential_;
}

int Neuron::get_nb_of_spikes() const
{
	return nb_of_spikes_;
}

void Neuron::printSpikes() const
{
	std::cout << nb_of_spikes_ << " : ";
	for (size_t i=0; i < spike_times_.size() ; ++i) {
		std::cout << spike_times_[i] * dt_ << " ";
	}
	std::cout << std::endl;
}

int Neuron::get_signals() const
{
	return signals_;
}

///UPDATE
void Neuron::update(double T, double Iext/*, double j*/)
{
	double t(T-time_);
	
	///--------------------------------------------------------------///
	
	if (T > time_) {
	
	if (not refractory_) {
		membrane_potential_ = exp(-t*dt_/TAU) * membrane_potential_ + Iext*R*(1-exp(-t*dt_/TAU)) /*+ j*/;
	} else {
		if (T-spike_times_.back() > Refractory_Time) {
			refractory_ = false;
			membrane_potential_ = exp(-t*dt_/TAU) * membrane_potential_ + Iext*R*(1-exp(-t*dt_/TAU)) /*+ j*/;
		}
	}
	
	if (membrane_potential_ > Vthr) {
		nb_of_spikes_ += 1;
		spike_times_.push_back(T);
		
		transmit_signal();
		
		refractory_ = true;
		membrane_potential_ = Vreset;
	}
	
	time_ = T;

	}
	
	///--------------------------------------------------------------///
	if (T == time_) {
	
	if (membrane_potential_ > Vthr) {
		nb_of_spikes_ += 1;
		spike_times_.push_back(T);
		
		transmit_signal();
		
		refractory_ = true;
		membrane_potential_ = Vreset;
	}
	
	}
}

void Neuron::add_signals()
{
	if (not refractory_) {
		membrane_potential_ += signals_*J;
	}
	
	signals_ = 0;
}

void Neuron::transmit_signal() const
{
	if (not synapses_.empty()) {
		for (auto neuron : synapses_) {
			neuron->receive_signal();
		}
	}
}

void Neuron::receive_signal()
{
	++ signals_;
}

///DESTRUCTOR
Neuron::~Neuron()
{
	/*if (not synapses_.empty()) {
		for (auto neuron : synapses_) {
			delete neuron;
		}
		synapses_.clear();
	}*/
	synapses_.clear();
}
