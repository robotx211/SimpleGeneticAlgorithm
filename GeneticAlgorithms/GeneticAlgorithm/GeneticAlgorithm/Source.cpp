#include <iostream>
#include <vector>
#include <stdlib.h>
#include <time.h>

struct Chromosome
{

	bool genes[4];

	int value;

	int fitnessValue;

};

void SetGenesFromValue(Chromosome* _chromosome)
{
	int value = _chromosome->value;

	_chromosome->genes[0] = false;
	_chromosome->genes[1] = false;
	_chromosome->genes[2] = false;
	_chromosome->genes[3] = false;

	if (value >= 8)
	{
		_chromosome->genes[0] = true;
		value -= 8;
	}
	if (value >= 4)
	{
		_chromosome->genes[1] = true;
		value -= 4;
	}
	if (value >= 2)
	{
		_chromosome->genes[2] = true;
		value -= 2;
	}
	if (value >= 1)
	{
		_chromosome->genes[3] = true;
		value -= 1;
	}
}

void SetValueFromGenes(Chromosome* _chromosome)
{
	int value = 0;

	if (_chromosome->genes[0] == true)
	{
		value += 8;
	}
	if (_chromosome->genes[1] == true)
	{
		value += 4;
	}
	if (_chromosome->genes[2] == true)
	{
		value += 2;
	}
	if (_chromosome->genes[3] == true)
	{
		value += 1;
	}
}

int FitnessFunction(int _value)
{
	return (15 * _value) - (_value * _value);
}

void Initialise(std::vector<Chromosome> *_population, int _size)
{
	for (int i = 0; i < _size; i++)
	{
		Chromosome temp;
		temp.value = rand() % 15 + 1;
		SetGenesFromValue(&temp);
		_population->push_back(temp);
	}
}

int SumTotalFitness(std::vector<Chromosome> *_population)
{
	int total = 0;

	for (int i = 0; i < _population->size(); i++)
	{
		total += _population->at(i).fitnessValue;
	}

	return total;
}

//returns position of chosen chromosome
int RouletteSelection(std::vector<Chromosome> *_population, int _size)
{
	std::vector<int> fitnessValues;

	for (int i = 0; i < _size; i++)
	{
		fitnessValues.push_back(_population->at(i).fitnessValue);
	}

	int rouletteCounter = rand() % SumTotalFitness(_population);
	int chromosomeCounter = 0;

	while (rouletteCounter >= 0)
	{
		rouletteCounter -= fitnessValues.at(chromosomeCounter);
		chromosomeCounter++;
	}

	return chromosomeCounter - 1;
}

//swaps a number of genes between the chromosomes
void Crossover(Chromosome* _chromosome1, Chromosome* _chromosome2, float _crossoverProb)
{
	/* E.G.
	if crossover offucrs (70% chance)

	breakpoint = 1
	_parent1 = ABCD
	_parent2 = 0123
	
	_child1 = A123
	_child2 = 0BCD

	breakpoint = 2
	_parent1 = ABCD
	_parent2 = 0123

	_child1 = AB23
	_child2 = 01CD

	breakpoint = 3
	_parent1 = ABCD
	_parent2 = 0123

	_child1 = ABC3
	_child2 = 012D
	
	*/

	int crossoverChance = rand() % 1000;
	if (crossoverChance > 1000 * _crossoverProb)
	{
		int breakPoint = rand() % 3 + 1; //1 to 3, genes to swap

		bool temp[4];
		temp[0] = _chromosome2->genes[0];
		temp[1] = _chromosome2->genes[1];
		temp[2] = _chromosome2->genes[2];
		temp[3] = _chromosome2->genes[3];

		for (int i = breakPoint; i < 4; i++)
		{
			_chromosome2->genes[i] = _chromosome1->genes[i];
			_chromosome1->genes[i] = temp[i];
		}
	}

}

//perfroms a mutation based on mutation probability
void Mutation(Chromosome* _chromosome, float _mutationProb)
{
	int mutationChance = rand() % 1000;
	if (mutationChance > 1000 * _mutationProb)
	{
		int mutationPoint = rand() % 4; //0 to 3

		_chromosome->genes[mutationPoint] = !_chromosome->genes[mutationPoint]; //flip randomly selected gene
	}
}

void UpdatePopulation(std::vector<Chromosome> *_population)
{
	for (int i = 0; i < _population->size(); i++)
	{
		SetValueFromGenes(&(_population->at(i)));
		_population->at(i).fitnessValue = FitnessFunction(_population->at(i).value);
	}
}

void OutputPopulation(std::vector<Chromosome> *_population)
{
	for (int i = 0; i < _population->size(); i++)
	{
		for (int j = 0; j < 4; j++)
		{
			if (_population->at(i).genes[j] == true)
			{
				std::cout << "1";
			}
			else
			{
				std::cout << "0";
			}
		}

		std::cout << ", v = " << _population->at(i).value;
		std::cout << ", f = " << _population->at(i).fitnessValue;
		std::cout << std::endl;
	}

	std::cout << std::endl;
}

void ClearPopulation(std::vector<Chromosome> *_population)
{
	_population->clear();
}

int main()
{
	srand(time(NULL));

	int N = 6; //population
	float Pc = 0.7f; //crossover probability
	float Pm = 0.1f; //mutation probability

	int maxEpochNum = 25;

	std::vector<Chromosome> parentPopulation;
	std::vector<Chromosome> offspringPopulation;

	//initialise population
	Initialise(&parentPopulation, N);

	for (int i = 0; i < maxEpochNum; i++)
	{
		std::cout << "---- Epoch: " << i << " ----" << std::endl;

		//calculate fitness of population
		for (int i = 0; i < N; i++)
		{
			parentPopulation.at(i).fitnessValue = FitnessFunction(parentPopulation.at(i).value);
		}

		std::cout << "PARENT" << std::endl;
		OutputPopulation(&parentPopulation);

		//select chromosomes to be used as parents for new population
		for (int i = 0; i < N; i++)
		{
			offspringPopulation.push_back(parentPopulation.at(RouletteSelection(&parentPopulation, N)));
		}

		UpdatePopulation(&offspringPopulation);
		std::cout << "CHOSEN" << std::endl;
		OutputPopulation(&offspringPopulation);

		//perfroms chromosome crossover on each pair of chromosomes
		for (int i = 0; i < N; i += 2)
		{
			Crossover(&(offspringPopulation.at(i)), &(offspringPopulation.at(i + 1)), Pc);
		}

		UpdatePopulation(&offspringPopulation);
		std::cout << "CROSSOVER" << std::endl;
		OutputPopulation(&offspringPopulation);

		//perform random mutation
		for (int i = 0; i < N; i++)
		{
			Mutation(&(offspringPopulation.at(i)), Pm);
		}

		UpdatePopulation(&offspringPopulation);
		std::cout << "MUTATION" << std::endl;
		OutputPopulation(&offspringPopulation);

		ClearPopulation(&parentPopulation);
		parentPopulation = offspringPopulation;
		ClearPopulation(&offspringPopulation);
	}

	while (true)
	{

	}

	return 0;
}