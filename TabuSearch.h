#pragma once
#include "VehicleRouteProblem.h"
#include <string>
#include <vector>
#include <list>
#include <conio.h>

class TabuSearch
{
	std::list<std::pair<int, int> > tabu_list;

	
	double countCost(std::vector<int> path);
	double TabuSearch::countCostWithRandom(std::vector<int> path);
	VehicleRouteProblem& vrp;

public:
	TabuSearch(VehicleRouteProblem& vrp);
	~TabuSearch();

	std::vector<std::vector<int>> pathStruc;
	double TabuSearch::countCostOneRandom(std::vector<int> path);
	double TabuSearch::countBestCost(std::vector<int> path);
	std::pair<int, int> swapTwoRandomCities(std::vector<int>& path);
	std::vector <int> generateRandomSolution();
	void startAlgorithm(int tabu_list_size = 10, int neighbours_count = 400, int steps_without_change = 100, int steps_with_random_neighbourhood = 20);

	bool checkTabu(std::pair<int, int>&, std::vector<int>& path, double neighbour_cost);

	std::vector< int > the_best_solution;
	double min_cost;
};