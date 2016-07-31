
#include "TabuSearch.h"
#include <algorithm>
#include <iostream>
#include <ctime>
#include <limits>
#include <numeric>

#define cityCounts 201
#define capcity 600

TabuSearch::TabuSearch(VehicleRouteProblem& t) :
vrp(t)
{
}


TabuSearch::~TabuSearch()
{
}

void TabuSearch::startAlgorithm(int tabu_list_size, int neighbours_count, int steps_without_change, int steps_with_random_neighbourhood)
{
	//std::cout << "parameters:" << tabu_list_size << "," << neighbours_count << "," << steps_without_change << "," << steps_with_random_neighbourhood << "\n";
	if (vrp.getCitiesCount() == 0) return;
	srand(static_cast<unsigned int>(time(NULL)));
	
	double current_cost;
	std::vector <int> current_solution(generateRandomSolution());//给出大小200的解序列, 初始化给current_solution

	current_cost = countBestCost(current_solution);

	the_best_solution = current_solution;
	min_cost = current_cost;

	std::pair<int, int> swaped_cities;
	std::pair<int, int> saved_swap;
	
	std::vector <int> the_best_neighbour;
	the_best_neighbour = the_best_solution;
	std::vector <int> neighbour;
	double neighbour_cost;
	double the_best_neighbour_cost;

	int steps = 0;
	int change_neighbourhood_count = 0;

	do {
		std::cout << "随机次数:" << change_neighbourhood_count << std::endl;
		the_best_neighbour_cost = std::numeric_limits<double>::max();
		
		//多次交换,从每次交换中寻找局部最优解
		for (int i = 0; i < neighbours_count; i++)
		{
			neighbour = current_solution;
			swaped_cities = swapTwoRandomCities(neighbour);//neighbor已经更新
			neighbour_cost = countBestCost(neighbour);
			if (neighbour_cost < the_best_neighbour_cost && checkTabu(swaped_cities, neighbour, neighbour_cost))
			{
				the_best_neighbour_cost = neighbour_cost;
				//std::cout << the_best_neighbour_cost << std::endl;
				the_best_neighbour = neighbour;
				saved_swap = swaped_cities;
			}
		}

		//更新当前局部最优解
		current_solution = the_best_neighbour;
		current_cost = the_best_neighbour_cost;
		
		//如果找到更优解，更新全局最优解，步数清零
		if (current_cost < min_cost)
		{
			the_best_solution = current_solution;
			min_cost = current_cost;
			std::cout <<"find a btter global solution:"<< min_cost << std::endl;
			steps = 0;
		}

		//更新禁忌表
		if (tabu_list.size() > tabu_list_size)
		{
			tabu_list.pop_front();
		}
		tabu_list.push_back(saved_swap);
		steps++;
		std::cout << "连续没有比最优解"<<min_cost<<"小的次数：" <<steps<< std::endl;

		//如果在一定步骤之内没有找到更优秀的解，即陷入局部最优解，再次随机，再次循环以上步骤
		if (steps > steps_without_change)
		{
			current_solution = generateRandomSolution();
			current_cost = countBestCost(current_solution);
			change_neighbourhood_count++;
			steps = 0;
		}
		
	
	} while (!kbhit());
	//while (change_neighbourhood_count < steps_with_random_neighbourhood);
}

std::vector <int> TabuSearch::generateRandomSolution()
{
	std::vector <int> solution;
	for (int i = 1; i < cityCounts; i++)
	{
		solution.push_back(i);
	}
	std::random_shuffle(solution.begin(), solution.end());
	return solution;
}

double TabuSearch::countCost(std::vector<int> path)
{
	std::vector<double> cost(1,0) ;
	std::vector<double> carryLoad(1, 0);
	std::vector<int> tem;
	int j = 0;
	double sumCost = 0;

	//清空解结构
	pathStruc.clear();
	std::vector<std::vector<int>>().swap(pathStruc);
	
	carryLoad.at(0) += vrp.cityDemand.at(path[0]);

	//按容量贪婪原则分配每辆车的路径
	for (int i = 0; i < cityCounts-2; i++)
	{
	
		if (carryLoad.at(j)<=capcity)
		{
			cost.at(j) += vrp.getEdgeCost(path[i], path[i + 1]);
			carryLoad.at(j) += vrp.cityDemand.at(path[i+1]);
			tem.push_back(path[i]);			
		}
		else
		{
			cost.at(j) = cost.at(j) - vrp.getEdgeCost(path[i], path[i + 1]);
			carryLoad.at(j) -= vrp.cityDemand.at(path[i + 1]);
			tem.pop_back();
			pathStruc.push_back(tem);

			//清空tem,准备装载下一车辆路径
			tem.clear();
			std::vector<int>().swap(tem);
		
			i = i - 1;
			j++;
			cost.push_back(0);
			carryLoad.push_back(0);
			carryLoad.at(j) = vrp.cityDemand.at(path[i]);

		}
	}

	pathStruc.push_back(tem);

	//计算所有车路径距离之和
	for (int i = 0; pathStruc.begin()+i!=pathStruc.end(); i++)
	{
		sumCost += vrp.getEdgeCost(0, pathStruc.at(i).front())+ vrp.getEdgeCost(0, pathStruc.at(i).back()) + cost.at(i);
	}

	return sumCost;
}

double TabuSearch::countBestCost(std::vector<int> path)
{
	double bestCost = 15000;// std::numeric_limits<double>::max();
	double currentCost;
	std::vector<std::vector<int>>bestPathStruc;
	for (int i = 0; i <abs(0.00005*bestCost*bestCost-1.7786*bestCost+14278); i++)
	{
		std::vector<std::vector<int>>().swap(pathStruc);
		currentCost = countCostOneRandom(path);

		if (currentCost < bestCost){
			bestCost = currentCost;
			
			bestPathStruc = pathStruc;
		}


	}
	//std::cout << "random best cost:" << bestCost << std::endl;
	pathStruc = bestPathStruc;
	return bestCost;
}

//每辆车在容量附近但随机的减少，使得车辆容量不饱和
double TabuSearch::countCostOneRandom(std::vector<int> path)
{
	std::vector<double> cost(1, 0);
	std::vector<double> carryLoad(1, 0);
	std::vector<int> tem;
	int j = 0;
	double sumCost = 0;

	//清空解结构
	pathStruc.clear();
	std::vector<std::vector<int>>().swap(pathStruc);

	carryLoad.at(0) += vrp.cityDemand.at(path[0]);

	//按容量贪婪原则分配每辆车的路径
	for (int i = 0; i < cityCounts - 2; i++)
	{

		if (carryLoad.at(j) <= capcity)
		{
			cost.at(j) += vrp.getEdgeCost(path[i], path[i + 1]);
			carryLoad.at(j) += vrp.cityDemand.at(path[i + 1]);
			tem.push_back(path[i]);
		}
		else
		{
			int randomMinus = 0;
			randomMinus = 1+rand() % (int)(0.2*tem.size());
			for (int k = 0; k < randomMinus; k++)
			{
				cost.at(j) = cost.at(j) - vrp.getEdgeCost(path[i], path[i + 1]);
				carryLoad.at(j) -= vrp.cityDemand.at(path[i + 1]);
				tem.pop_back();
				i--;
			}
			

			pathStruc.push_back(tem);

			//清空tem,准备装载下一车辆路径
			tem.clear();
			std::vector<int>().swap(tem);

			j++;
			cost.push_back(0);
			carryLoad.push_back(0);
			carryLoad.at(j) = vrp.cityDemand.at(path[i]);

		}
	}

	pathStruc.push_back(tem);

	//计算所有车路径距离之和
	for (int i = 0; pathStruc.begin() + i != pathStruc.end(); i++)
	{
		sumCost += vrp.getEdgeCost(0, pathStruc.at(i).front()) + vrp.getEdgeCost(0, pathStruc.at(i).back()) + cost.at(i);
	}



	return sumCost;
}


//加入随机元素，在最优解和躲避梯度陡增值之间权衡， 给出在当前序列下一个最优解
double TabuSearch::countCostWithRandom(std::vector<int> path)
{
	std::vector<double> cost(1, 0);
	double sumCost = 0;
	double bestSumCost = std::numeric_limits<double>::max();
	double totalCostWithoutZero=0;
	double costThreshold = 5.0;
	std::vector<int> conditateBackCity;
	std::vector<int> temPath;
	std::vector<int> carPath;
	std::vector<std::vector<int>> randompathStruc;

	std::vector<std::vector<int>> temPathStruc;
	

	//清空解结构
	pathStruc.clear();
	std::vector<std::vector<int>>().swap(pathStruc);
	countCost(path);

	//加入候选点
	for (int i = 0; i < cityCounts-2; i++)
	{
		totalCostWithoutZero += vrp.getEdgeCost(path.at(i), path.at(i + 1));
	}
	
	for (int i = 0; i < cityCounts-2; i++)
	{
		//加入距离下一城市路程长于平均值加系数的节点  和  该点至下一点距离大于返回的节点
		if (vrp.getEdgeCost(path[i], path[i + 1]) > costThreshold*totalCostWithoutZero / (cityCounts - 1) || vrp.getEdgeCost(path[i], path[i + 1]) > 10*vrp.getEdgeCost(path[i], 0))

		{
			conditateBackCity.push_back(i);
		}
		
	}

	int cityID = -1;
	for (int i = 0; i < pathStruc.size(); i++)
	{
		cityID += pathStruc.at(i).size();
		conditateBackCity.push_back(cityID);
	}


	//重新分割解
	//std::cout <<"候选换车点数量"<< conditateBackCity.size() << std::endl;
	//system("pause");

	for (int j = pathStruc.size(); j < pathStruc.size()+2; j++)//尝试不同的车辆数
	{
		//std::cout << "车辆数：" << j << std::endl;
		for (int i = 0; i < 10*conditateBackCity.size(); i++)//尝试挑选不同的候选返程点
		{
			//std::cout << "不同返程点：" << sumCost << std::endl;
			std::vector<int>().swap(temPath);
			temPath=path;
			sumCost = 0;
			std::vector<double>().swap(cost);
			cost.push_back(0);

			//记录该解的解结构，评估该解的cost
			std::vector<std::vector<int>>().swap(randompathStruc);			
			std::random_shuffle(conditateBackCity.begin(), conditateBackCity.end());

			//插入换车点
			for (int k = 0; k < j-1; k++)
			{
				temPath.insert(temPath.begin() + conditateBackCity.at(k)+1,-1);
			}

			//解析出各车辆的解
			int carID = 0;
			for (int k = 0; k< temPath.size(); k++)
			{
				
				if (temPath.at(k)!=(-1))
				{
					
					carPath.push_back(temPath.at(k));
				}

				else
				{
					randompathStruc.push_back(carPath);
					std::vector<int>().swap(carPath);
				}

				if (k<cityCounts+j-3)
				{
					if (temPath.at(k + 1) != (-1))
					{
						cost.at(carID) += vrp.getEdgeCost(temPath[k], temPath[k + 1]);
					}
					else
					{
						carID++;
						cost.push_back(0);
					}
				}
				
			}
			randompathStruc.push_back(carPath);
			std::vector<int>().swap(carPath);

			//计算所有车路径距离之和
			for (int i = 0; i<randompathStruc.size(); i++)
			{
				
				if (randompathStruc.at(i).size()!=0)
				{
					if (vrp.getEdgeCost(0, randompathStruc.at(i).front()) + vrp.getEdgeCost(0, randompathStruc.at(i).back()) + cost.at(i)>capcity)
					{
						sumCost = std::numeric_limits<double>::max();
						break;
					}
					sumCost += vrp.getEdgeCost(0, randompathStruc.at(i).front()) + vrp.getEdgeCost(0, randompathStruc.at(i).back()) + cost.at(i);
				}
				
			}

			if (sumCost<bestSumCost)
			{
				pathStruc.swap(randompathStruc);
				bestSumCost = sumCost;
				std::cout << "随机车辆数最优解：" << bestSumCost << std::endl;
			}


		}
	}

	return bestSumCost;
}


std::pair<int, int> TabuSearch::swapTwoRandomCities(std::vector<int>& path)
{
	int firstPosition = rand() % path.size();

	int secondPosition;
	do {
		secondPosition = rand() % path.size();
	} while (firstPosition == secondPosition);

	std::pair<int, int> swaped_cities;

	if (firstPosition > secondPosition)
	{
		swaped_cities.first = path.at(secondPosition);
		swaped_cities.second = path.at(firstPosition);
	}
	else
	{
		swaped_cities.first = path.at(firstPosition);
		swaped_cities.second = path.at(secondPosition);
	}

	int buffor = path.at(firstPosition);
	path.at(firstPosition) = path.at(secondPosition);
	path.at(secondPosition) = buffor;
	
	return swaped_cities;
}

bool TabuSearch::checkTabu(std::pair<int, int>& swaped_cities, std::vector<int>& path, double neighbour_cost)
{
	std::list<std::pair<int, int> >::iterator list_iterator = tabu_list.begin();
	while (list_iterator != tabu_list.end())
	{
		if (swaped_cities.first == list_iterator->second || swaped_cities.second == list_iterator->first)//判断是否有某一城市又被向原来的方向交换
		{
			int first_element_index = std::find(path.begin(), path.end(), list_iterator->first) - path.begin();
			int second_element_index = std::find(path.begin(), path.end(), list_iterator->second) - path.begin();
			if (first_element_index < second_element_index && neighbour_cost > min_cost)
				return false;
		}
		list_iterator++;
	}
	return true;
}
