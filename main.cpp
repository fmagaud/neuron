#include "neuron.cpp"
#include "brain.cpp"
#include <iostream>
#include <fstream>

int main()
{
	double t_stop;
	std::cout << "How long is the simulation (ms)? ";
	std::cin >> t_stop;
	
	double dt;
	std::cout << "What is the simulation step (ms)? ";
	std::cin >> dt;
	
	unsigned long Tstop(t_stop/dt);
	
	double Iext;
	double ta;
	double tb;
	std::cout << "What is the external current (mA) and for which interval of time? ";
	std::cin >> Iext;
	std::cin >> ta;
	while (ta < 0) {
		std::cout << "It is too small, try again : ";
		std::cin >> ta;
	}
	std::cin >> tb;
	while (tb < ta or tb > t_stop) {
		std::cout << "This is not possible, try again : ";
		std::cin >> tb;
	}
	
	unsigned long Ta(ta/dt);
	unsigned long Tb(tb/dt);
	
	std::cout << "GO!!!" << std::endl;
	
	///-------------------------ONE-NEURON---------------------------///
	
	/**double time(Tstart);
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
		N.update(time, I);
	}
	
	std::cout << "Number of spikes : ";
	N.printSpikes();
	
	file.close();
	
	std::cout << "Done" << std::endl;**/
	
	//-------------------------TWO-NEURONS----------------------------//
	
	/*int time(0);
	Neuron N1(dt);
	Neuron N2(dt);
	N1.add_synapse(&N2);
	double I(0.0);
	std::ofstream file;
	file.open("two_neuron.txt");
	
	
	while (time <= Tstop) {
		
		if (time >= Ta and time <= Tb) {
			I = Iext;
		} else {
			I = 0;
		}
		
		time += 1;
		
		file << "T = " << time*dt << "ms : N1 " << N1.get_membrane_potential() << " N2 " << N2.get_membrane_potential() << std::endl;
		std::cout << "T = " << time*dt << "ms : N1 " << N1.get_membrane_potential() << " N2 " << N2.get_membrane_potential() << std::endl;
		std::cout << "N1 " << N1.get_nb_of_spikes() << std::endl;
		std::cout << "N2 " << N2.get_nb_of_spikes() << std::endl;
		
		int signals(0);
		do {
			N1.update(time, I);
			N2.update(time);
			N1.add_signals();
			N2.add_signals();
			N1.update(time, I);
			N2.update(time);
			
			signals = N1.get_signals() + N2.get_signals();
			
		} while (signals != 0);
	}
	
	file.close();
	
	std::cout << "Done" << std::endl;*/
	
	///--------------------TWO-NEURONS-WITH-BRAIN--------------------///
	
	unsigned long time(0);
	Brain B(2, dt); //creation of a brain with 2 neurons
	B.add_connection(0, 1); //creation of a connection from neuron1 to neuron2
	
	double I(0.0);
	std::ofstream file;
	file.open("two_neuron.txt");
	
	
	while (time <= Tstop) {
		//adapting I from user input
		if (time >= Ta and time <= Tb) {
			I = Iext;
		} else {
			I = 0;
		}
		
		//saving and printing on the terminal the data
		//B.save_into_file(file);
		B.print();
		
		//updating time and brain
		time += 1;
		B.update(time,I);
	}
	
	file.close();
	
	std::cout << "Done" << std::endl;
	
	return 0;
}
