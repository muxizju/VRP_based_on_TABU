// TSP_TS.cpp : Defines the entry point for the console application.
//
#include "TabuSearch.h"
#include "VehicleRouteProblem.h"
#include <fstream>

int main()
{
	VehicleRouteProblem vrp;
	vrp.readCitiesDataFromFileToCitiesMatrix("´ó×÷Òµ1.txt");

	TabuSearch* tabu=new TabuSearch(vrp);
	int tabu_list_size = 40;
	int neighbours_count = 1000;
	int steps_without_change = 50;
	int steps_with_random_neighbourhood = 10;
	tabu->startAlgorithm(tabu_list_size, neighbours_count, steps_without_change, steps_with_random_neighbourhood);

	std::ofstream record;
	record.open("results.txt");
	for (int i = 0; i < tabu->pathStruc.size(); i++)
	{
		record << "car " << i << " path: ";
		for (int j = 0; j < tabu->pathStruc.at(i).size(); j++)
		{
			record << tabu->pathStruc.at(i).at(j) << "  ";
		}

		record << std::endl;
	}

	record << tabu->min_cost;

	record.close();
	delete tabu;
	system("pause");
	return 0;
}

