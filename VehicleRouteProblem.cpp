
#include "VehicleRouteProblem.h"
#include <vector>
#include <algorithm>
#include <math.h>

#define cityCounts 201
#define capcity 600

VehicleRouteProblem::VehicleRouteProblem()
{
	cities_matrix = new std::vector< std::vector<double>* >;
}


VehicleRouteProblem::~VehicleRouteProblem()
{
	cities_matrix->clear();
	delete cities_matrix;
}




bool VehicleRouteProblem::readCitiesDataFromFileToCitiesMatrix(std::string file_path)
{
	std::ifstream file_with_cities(file_path);

	if (file_with_cities.good())
	{		
		readDemandFromFile(file_with_cities);

		file_with_cities.close();

		std::ifstream file_with_cities_twice(file_path);

		readEdgeCostsFromFile(file_with_cities_twice);

		
		file_with_cities_twice.close();

		if (cities_matrix->size() > 0)
		{
			return true;
		}
		
	}
	return false;

}

void VehicleRouteProblem::readEdgeCostsFromFile(std::ifstream& file_with_cities)
{
	
	cities_matrix->clear();

	for (int i = 0; i < cityCounts; ++i)
	{
		cities_matrix->push_back(new std::vector<double>);
		readPositionFromFile(cityCounts, file_with_cities, i);
	}
}

void VehicleRouteProblem::readDemandFromFile( std::ifstream& file_with_cities)
{
	std::string readChar;
	std::pair<int, int> cordinate;
	for (int i = 0; i < cityCounts; i++)
	{
	
		file_with_cities >> readChar;
		file_with_cities >> cordinate.first;
		file_with_cities >> cordinate.second;

		cityCordinate.push_back(cordinate);
		
		file_with_cities >> readChar;

		if (i == 0)
		{
			cityDemand.push_back(0);
		}
		else
		{

			cityDemand.push_back(string2int(readChar));
		}
	}

}

void VehicleRouteProblem::readPositionFromFile(int dimension, std::ifstream& file_with_cities, int row)
{
	double edgeCost;
	std::list<std::pair<int, int> >::iterator list_iterator = cityCordinate.begin();
	std::pair<int,int> startCity;
	std::pair<int,int> finalCity;
	for (int i = 0; i < row; i++)
	{
		list_iterator++;
	}

	startCity.first=list_iterator->first;
	startCity.second = list_iterator->second;


	for (list_iterator = cityCordinate.begin(); list_iterator!=cityCordinate.end(); list_iterator++)
	{

		finalCity.first = list_iterator->first;
		finalCity.second = list_iterator->second;
		
		edgeCost = sqrt( (finalCity.second - startCity.second)   * (finalCity.second - startCity.second)
			+ (finalCity.first - startCity.first) * (finalCity.first - startCity.first)  );

		cities_matrix->at(row)->push_back(edgeCost);
	}

}


int VehicleRouteProblem::string2int(const std::string &string)
{
	return atoi(string.c_str());
}



double VehicleRouteProblem::getEdgeCost(int row, int column)
{
	if (row<0||column<0)
	{
		return 0;
	}
	return cities_matrix->at(row)->at(column);
}

int VehicleRouteProblem::getCitiesCount()
{
	return cities_matrix->size();
}


