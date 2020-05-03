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
	if (meshSize[0] <= 0 || meshSize[1] <= 0 || meshSize[2] <= 0)
	{
		meshSize[0] = 0;
		meshSize[1] = 0;
		meshSize[2] = 0;
	}
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
/*
int DataMiner::GetFileHeight() noexcept
{
	return fileHeight;
}*/
std::unique_ptr<CubeCell> DataMiner::GetNextCell(Graphics& gfx)
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
	std::vector<float> color = getColorOf((int)values[0]);
	std::unique_ptr<CubeCell> returnVale = std::make_unique<CubeCell>(1, meshSize, x, y, z, color, values, gfx);
	return returnVale;
	//CubeCell returnVale(1, meshSize, x, y, z, color, values, gfx);//TODO 
	
}
std::unique_ptr<CubeCell> DataMiner::GetCellAt(int index, Graphics& gfx){
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
	std::vector<float> color = getColorOf((int)values[0]);
	std::unique_ptr<CubeCell> returnVale = std::make_unique<CubeCell>(index, meshSize, x, y, z, color, values, gfx);
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

std::vector<float> DataMiner::getColorOf(int id) noexcept {
	if (colors.count(id)) {
		return colors.at(id);
	}
	else
	{
		std::vector<float> returnVale;
		for (int i = 0; i < 3; i++) {
			returnVale.push_back(static_cast <float> (rand()) / static_cast <float> (RAND_MAX));
		}
		colors[id] = returnVale;
		return returnVale;
	}
	//try {
	//	return colors.at(id);
	//}
	//catch (...)
	//{
	//	std::vector<float> returnVale;
	//	for (int i = 0; i < 3; i++) {
	//		returnVale.push_back(static_cast <float> (rand()) / static_cast <float> (RAND_MAX));
	//	}
	//	colors[id] = returnVale;
	//	return returnVale;
	//}
}