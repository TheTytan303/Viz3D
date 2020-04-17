#include "DataMiner.h"
#include <sstream>

DataMiner::DataMiner(const char* fileName)
{
	this->fileName = fileName;
	this->file.open(fileName);
	this->file >> meshSize[0] >> meshSize[1] >> meshSize[2];
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
	std::vector<float> color = { 0.5f,0.5f,0.5f };
	CubeCell returnVale(meshSize, x, y, z, color, values);
	return returnVale;
}

unsigned short* DataMiner::GetMeshSize() noexcept
{
	return nullptr;
}
