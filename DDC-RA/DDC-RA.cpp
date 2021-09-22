/*
 * DDC-RA.cpp
 *
 *  Created on: Aug 8, 2019
 *      Author: alireza
 */

#include <ctime>
#include "elements/DataCenter.h"

int main () {

	DC::DataCenter dc;
	dc.make();
	//dc.print();

	clock_t begin = clock();
	//int sol=dc.allocate();


	int sol=dc.allocate_ff();

	clock_t end = clock();

	double duration = double(end - begin) / CLOCKS_PER_SEC;

	dc.printCSV();

	std::cout << "Solving time: "<< duration <<" s" << std::endl;

	return 0;
}
