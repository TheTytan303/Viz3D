#include "DataMiner.h"
#include <sstream>
#include <cstdlib>
#include <ctime>
DataMiner::DataMiner(const char* fileName)
{
	this->fileName = fileName;
	this->file.open(fileName);
	if (file.fail())
	{
		throw "failed to open file";
	}
	std::string cline;
	getline(this->file, cline);

	std::istringstream iss(cline);
	iss >> meshSize[0] >> meshSize[1] >> meshSize[2];
	if (meshSize[0] <= 0 || meshSize[1] <= 0 || meshSize[2] <= 0)
	{
		meshSize[0] = 0;
		meshSize[1] = 0;
		meshSize[2] = 0;
		//return;
	}
	std::string tmp;
	std::vector<std::string> valuesNames;
	while (iss >> tmp) {
		valuesNames.push_back(tmp);
	}
	if (valuesNames.size() > 0)
	{
		valuesNames.erase(valuesNames.begin());
	}
	CubeCell::setNames(valuesNames);
}
DataMiner::DataMiner(const std::string fileName)
	:
	DataMiner(fileName.c_str())
{
}

DataMiner::DataMiner(const std::wstring fileName)
{
	//this->fileName = fileName.c_str();
	this->file.open(fileName);
	if (file.fail())
	{
		throw "failed to open file";
	}
	std::string cline;
	getline(this->file, cline);

	std::istringstream iss(cline);
	iss >> meshSize[0] >> meshSize[1] >> meshSize[2];
	if (meshSize[0] <= 0 || meshSize[1] <= 0 || meshSize[2] <= 0)
	{
		meshSize[0] = 0;
		meshSize[1] = 0;
		meshSize[2] = 0;
		return;
	}
	std::string tmp;
	std::vector<std::string> valuesNames;
	while (iss >> tmp) {
		valuesNames.push_back(tmp);
	}
	if (valuesNames.size() > 0)
	{
		valuesNames.erase(valuesNames.begin());
	}
	CubeCell::setNames(valuesNames);
}


DataMiner::~DataMiner() noexcept
{

}

void DataMiner::closeFile() noexcept {
	this->file.close();
}

std::unique_ptr<Cell> DataMiner::GetNextCell()
{
	unsigned short x, y, z;
	std::vector<float> values;
	std::string cline;

	getline(this->file, cline);
	std::istringstream iss(cline);
	iss >> x >> y >> z;
	float tmp;
	while (iss >> tmp) {
		values.push_back(tmp);
	}
	if (file.eof()) {
		throw "end of file";
	}
	int grain = (int)values[0];
	values.erase(values.begin());
	initGrain(grain);
	std::unique_ptr<Cell> returnVale = std::make_unique<Cell>(x, y, z, grain, values);
	return returnVale;
}

std::unique_ptr<Cell> DataMiner::GetCellAt(int index){
	file.seekg(file.beg);
	std::string cline;
	for (int i = 0; i < index; i++)
	{
		if (file.eof()) {
			throw "end of file";
		}
		std::getline(this->file, cline);
	}
	
	unsigned short x, y, z;
	std::vector<float> values;

	getline(this->file, cline);
	std::istringstream iss(cline);
	iss >> x >> y >> z;
	float tmp;
	while (iss >> tmp) {
		values.push_back(tmp);
	}
	int grain =(int) values[0];
	values.erase(values.begin());
	initGrain(grain);
	std::unique_ptr<Cell> returnVale = std::make_unique<Cell>(x, y, z, grain, values);
	return returnVale;
};

unsigned short* DataMiner::GetMeshSize() noexcept
{
	return meshSize;
}

std::vector<std::string> DataMiner::GetValuesNames() noexcept
{
	return std::vector<std::string>();
}

void DataMiner::initGrain(int id) noexcept {
	if (!std::count(grains.begin(), grains.end(), id))
	{
		CubeCell::addColor(id);
		grains.push_back(id);
	}
}