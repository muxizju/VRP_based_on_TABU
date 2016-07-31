#pragma once

#include <string>
#include <fstream>
#include <vector>
#include <list>
class VehicleRouteProblem
{
	std::vector< std::vector<double>* >* cities_matrix;

public:
	VehicleRouteProblem();
	~VehicleRouteProblem();
	
	//���ļ��ж�ȡ�������󣬶�ȡ����λ�ã�������о������
	bool readCitiesDataFromFileToCitiesMatrix(std::string filePath);
	void VehicleRouteProblem::readDemandFromFile(std::ifstream& file_with_cities);
	void readEdgeCostsFromFile(std::ifstream& file_with_cities);
	void readPositionFromFile(int dimension, std::ifstream& file_with_cities, int row);	

	int string2int(const std::string &string);

	double getEdgeCost(int row, int column);
	int getCitiesCount();

	std::list<std::pair<int, int> > cityCordinate;
	std::vector< int > cityDemand;
};

