#include "neuron.cpp"
#include <iostream>
#include <fstream>

int main()
{
	double Tstart(0);
	double Tstop;
	std::cout << "How long is the simulation (ms)? ";
	std::cin >> Tstop;
	
	double dt;
	std::cout << "What is the simulation step (ms)? ";
	std::cin >> dt;
	
	double Iext;
	double Ta;
	double Tb;
	std::cout << "What is the external current (mA) and for which interval of time? ";
	std::cin >> Iext;
	std::cin >> Ta;
	while (Ta < Tstart) {
		std::cout << "It is too small, try again";
		std::cin >> Ta;
	}
	std::cin >> Tb;
	while (Tb < Ta or Tb > Tstop) {
		std::cout << "This is not possible, try again";
		std::cin >> Tb;
	}
	
	std::cout << "GO!!!" << std::endl;
	
	double time(Tstart);
	Neuron N(Tstart);
	double I(0.0);
	std::ofstream file;
	file.open("neuron.txt");
	
	while (time <= Tstop) {
		if (time >= Ta and time <= Tb) {
			I = Iext;
		} else {
			I = 0;
		}
		
		file << "T = " << time << "ms : " << N.get_membrane_potential() << std::endl;
		std::cout << "T = " << time << "ms : " << N.get_membrane_potential() << std::endl;
		std::cout << I << std::endl;
		std::cout << N.get_nb_of_spikes() << std::endl;
		
		time += dt;
		N.update(I, time);
	}
	
	file.close();
	
	std::cout << "Done" << std::endl;
	
	return 0;
}
