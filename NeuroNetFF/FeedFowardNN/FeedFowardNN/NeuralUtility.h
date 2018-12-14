#pragma once
#include <vector>
#include "DataRecord.h"
#include <cstdlib>
#include <algorithm>
#include "stdafx.h"
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <sstream>

using namespace std;

class NeuralUtility
{
public:
	vector <double> Normalise(const vector <double> values, const vector<double> max, const vector<double> min);
	vector <double> DeNormalise(const vector <double> values, const vector<double> max, const vector<double> min);
	vector <double> DeStringify(char * row);
	void SuffleData(char *filename, char *tempFilename);
	void SuffleArrayData(vector<DataRecord> &data);
	vector<double> MaximumAndMinValue(char *fileName);
	void FindAndReplace(string & data, string toSearch, string replaceStr);
	vector<double> ConvertStringToVectorDouble(string& value, char delimiter)
	{
		
		stringstream myStream(value);
		vector<double> data;
		string cellData;
		while (getline(myStream, cellData, ','))
		{
			// cellData now has the value of a cell in your csv file.
			data.push_back(stod(cellData));
		}

		return data;
		
	}
	vector<unsigned> ConvertStringToVectorUnsigned(string& value, char delimiter)
	{

		stringstream myStream(value);
		vector<unsigned> data;
		string cellData;
		while (getline(myStream, cellData, ','))
		{
			// cellData now has the value of a cell in your csv file.
			cout << cellData;
			data.push_back(stoi(cellData));
		}

		return data;

	}

	/*int WriteLine(char *text);*/
	int GenerateRandoms(int lower, int upper);

private:
	
};

void NeuralUtility::SuffleArrayData(vector<DataRecord> &data)
{
	random_shuffle(data.begin(), data.end());
}
vector <double> NeuralUtility::Normalise(const vector <double> values, const vector<double> max, const vector<double> min)
{
	vector <double> output;
	for(unsigned i =0; i< values.size();++i)
	{
		output.push_back((values[i] - min[i]) / (max[i] - min[i]));
	}

	return output;
}

vector <double> NeuralUtility::DeNormalise(const vector <double> values, const  vector<double> max, const vector<double> min)
{
	vector <double> output;
	for (unsigned i = 0; i< values.size(); ++i)
	{
		output.push_back((values[i] * (max[i] - min[i])) + min[i]);
	}

	return output;
}

vector <double> NeuralUtility::DeStringify(char * row)
{
	vector <double> output;
	char *end = row;
	while (*end) {
		output.push_back(strtol(row, &end, 10));
		while (*end == ',') {
			end++;
		}
		row = end;
	}
	return output;
}

void  NeuralUtility::SuffleData(char *filename, char *tempFilename)
{
	
	//open the file
	//count number of records
	//create array with record index numbers
	//resuffle

	ifstream in(filename);
	string line;
	vector<int> recList;
	int i = 1;

	while (!in.eof() && getline(in, line))
	{
		if (!in.eof())
		{
			recList.push_back(i);
			i++;
		}
	}

	//resuffle recList
	random_shuffle(recList.begin(), recList.end());

	in.clear();
	in.seekg(0);

	cout << recList[0];

	ofstream out(tempFilename);

	for (unsigned r = 0; r < recList.size(); r++)
	{
		i = 0;
		while (!in.eof() && getline(in, line))
		{
			++i;
			if (i == recList[r])
			{
				break;
			}
		}
		out << line << endl;
		out.flush();
		in.clear();
		in.seekg(0);
	}

	in.close();
	out.close();

	//copy file

	std::ifstream  src(tempFilename, std::ios::binary);
	std::ofstream  dst(filename, std::ios::binary);

	dst << src.rdbuf();

}

vector<double> NeuralUtility::MaximumAndMinValue(char *fileName)
{
	ifstream _File(fileName);

	if (_File.eof())
	{
		printf("Failed to load data file!");
		exit;
	}
	vector <double> data;
	double max = 0, min=999999999;

	string line;
	while (!_File.eof() && getline(_File, line, '\n'))
	{
		stringstream myStream(line);
		string cellData;
		double val = 0;
		while (getline(myStream, cellData, ','))
		{
			// cellData now has the value of a cell in your csv file.
			val = stod(cellData);
			max = max > val ? max : val;
			min = min < val ? min : val;
		}
	}

	data.push_back(max);
	data.push_back(min);

	return data;
}

int NeuralUtility::GenerateRandoms(int lower, int upper)
{
		int num = (rand() %
			(upper - lower + 1)) + lower;

		return num;
}

void NeuralUtility::FindAndReplace(std::string & data, std::string toSearch, std::string replaceStr)
{
	// Get the first occurrence
	size_t pos = data.find(toSearch);

	// Repeat till end is reached
	while (pos != std::string::npos)
	{
		// Replace this occurrence of Sub String
		data.replace(pos, toSearch.size(), replaceStr);
		// Get the next occurrence from the current position
		pos = data.find(toSearch, pos + toSearch.size());
	}
}
