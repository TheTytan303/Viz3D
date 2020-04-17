#include "DataMiner.h"
#include <sstream>
#include <cstdlib>
#include <ctime>
DataMiner::DataMiner(const char* fileName)
{
	this->fileName = fileName;
	this->file.open(fileName);

	std::string cline;
	getline(this->file, cline);

	std::istringstream iss(cline);
	iss >> meshSize[0] >> meshSize[1] >> meshSize[2];
	std::string tmp;
	while (iss >> tmp) {
		this->valuesNames.push_back(tmp);
	}
}
DataMiner::DataMiner(const std::string fileName)
	:
	DataMiner(fileName.c_str())
{
}

DataMiner::~DataMiner() noexcept
{

}

void DataMiner::closeFile() noexcept {
	this->file.close();
}
CubeCell DataMiner::GetNextCell()
{
	if (file.eof()) {
		throw "end of file";
	}
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
	std::vector<float> color = getColorOf(values[0]);
	CubeCell returnVale(meshSize, x, y, z, color, values);
	return returnVale;
}
CubeCell DataMiner::GetCellAt(int index){
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
	std::vector<float> color = getColorOf(values[0]);
	CubeCell returnVale(meshSize, x, y, z, color, values);
	return returnVale;
};
unsigned short* DataMiner::GetMeshSize() noexcept
{
	return nullptr;
}

std::vector<std::string> DataMiner::GetValuesNames() noexcept
{
	return std::vector<std::string>();
}

std::vector<float> DataMiner::getColorOf(int id) noexcept {
	try {
		return colors.at(id);
	}
	catch (...)
	{
		std::vector<float> returnVale;
		for (int i = 0; i < 3; i++) {
			returnVale.push_back(static_cast <float> (rand()) / static_cast <float> (RAND_MAX));
		}
		colors[id] = returnVale;
		return returnVale;
	}
}
