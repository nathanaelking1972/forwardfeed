#pragma once
#include "vector"
#include "NeuralNetworkConfig.h"
#include "Neuron.h"
#include "FileUtility.h"
#include "NeuralUtility.h"
#include "DataRecord.h"
#include <iterator>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <assert.h>
#include <conio.h>

using namespace std;

typedef vector <Neuron> Layer;

class NeuralNetwork
{
public:
	NeuralNetwork(const NeuralNetworkConfig &config);
	void StartTraininig();
	
	
	vector<double> Run(const vector<double> &inputs);
	~NeuralNetwork();
private:
	NeuralNetworkConfig _config;
	vector<Layer> m_Layers;
	vector<DataRecord> m_Data;
	vector<double> m_Max_Input;
	vector<double> m_Min_Input;
	vector<double> m_Max_Output;
	vector<double> m_Min_Output;
	NeuralUtility neuralUtility;
	double m_error;
	vector<double> FeedForward(const vector<double> &input);
	void BackPropogation(const vector<double>  &eExpectedOutput);
	void OutputResults(int epoch);
	void RMSResults(int epoch, double trainingRSM, double validationRSM, double testRSM);
	double ValidationRun();
	double TestRun();
	void Initialise();
	
	
};

NeuralNetwork::NeuralNetwork(const NeuralNetworkConfig &config)
{
	//create number of layers of the network
	_config = config;

	Initialise();


	m_Max_Input.push_back(4598.22);
	m_Max_Input.push_back(4989.31);
	m_Min_Input.push_back(274.062);
	m_Min_Input.push_back(677.306);

	m_Max_Output.push_back(186.359);
	m_Max_Output.push_back(300);
	m_Min_Output.push_back(114.47);
	m_Min_Output.push_back(101.751);


}

double  NeuralNetwork::TestRun()
{

	double _error = 0;
	vector<DataRecord> m_valData;
	ifstream _File(_config.TestDataFileName);

	if (_File.eof())
	{
		printf("Failed to load data file!");
		exit;
	}

	string line;
	while (!_File.eof() && getline(_File, line, '\n'))
	{
		stringstream myStream(line);
		string cellData;
		vector <double> data;
		DataRecord rec;
		while (getline(myStream, cellData, ','))
		{
			// cellData now has the value of a cell in your csv file.
			data.push_back(stod(cellData));
		}

		rec.LeftInput = data[0];
		rec.LeftOutput = data[1];
		rec.RightInput = data[2];
		rec.RightOutput = data[3];

		m_valData.push_back(rec);

		//close the file
		_File.close();
	}

	for each(DataRecord dRec in m_valData)
	{
		vector<double> input;
		input.push_back(dRec.LeftInput);
		input.push_back(dRec.LeftOutput);

		vector<double> output;
		output.push_back(dRec.RightInput);
		output.push_back(dRec.RightOutput);

		vector<double> result = FeedForward(neuralUtility.Normalise(input, m_Max_Input, m_Min_Input));
		//calculate error

		Layer &lastLayer = m_Layers.back();

		vector<double> normalisedOutput = neuralUtility.Normalise(output, m_Max_Output, m_Min_Output);

		for (unsigned i = 0; i < lastLayer.size() - 1; ++i)
		{
			double deltaError = normalisedOutput[i] - lastLayer[i].GetOutputValue();
			_error = deltaError * deltaError;
		}

		_error = sqrt(_error / (lastLayer.size() - 1)); //RMS Value

	}

	return _error;
}

double  NeuralNetwork::ValidationRun()
{

	double _error = 0;
	vector<DataRecord> m_valData;
	ifstream _File(_config.ValidationDataFileName);

	if (_File.eof())
	{
		printf("Failed to load data file!");
		exit;
	}

	string line;
	while (!_File.eof() && getline(_File, line, '\n'))
	{
		stringstream myStream(line);
		string cellData;
		vector <double> data;
		DataRecord rec;
		while (getline(myStream, cellData, ','))
		{
			// cellData now has the value of a cell in your csv file.
			data.push_back(stod(cellData));
		}

		rec.LeftInput = data[0];
		rec.LeftOutput = data[1];
		rec.RightInput = data[2];
		rec.RightOutput = data[3];

		m_valData.push_back(rec);

		//close the file
		_File.close();
	}

	for each(DataRecord dRec in m_valData)
	{
		vector<double> input;
		input.push_back(dRec.LeftInput);
		input.push_back(dRec.LeftOutput);

		vector<double> output;
		output.push_back(dRec.RightInput);
		output.push_back(dRec.RightOutput);

		vector<double> result = FeedForward(neuralUtility.Normalise(input, m_Max_Input, m_Min_Input));
		//calculate error

		Layer &lastLayer = m_Layers.back();

		vector<double> normalisedOutput = neuralUtility.Normalise(output, m_Max_Output, m_Min_Output);

		for (unsigned i = 0; i < lastLayer.size() - 1; ++i)
		{
			double deltaError = normalisedOutput[i] - lastLayer[i].GetOutputValue();
			_error = deltaError * deltaError;
		}

		_error = sqrt(_error / (lastLayer.size() - 1)); //RMS Value
		
	}

	//random_shuffle(m_valData.begin(), m_valData.end());

	return _error;
}
void NeuralNetwork::StartTraininig()
{

	//load file
	ifstream _File(_config.TrainingDataFileName);

	if (_File.eof())
	{
		printf("Failed to load data file!");
		exit;
	}

	string line;
	while (!_File.eof() && getline(_File, line, '\n'))
	{
		stringstream myStream(line);
		string cellData;
		vector <double> data;
		DataRecord rec;
		while (getline(myStream, cellData, ','))
		{
			// cellData now has the value of a cell in your csv file.
			data.push_back(stod(cellData));
		}
			rec.LeftInput = data[0];
		rec.RightInput = data[1];
		rec.LeftOutput = data[2];
		rec.RightOutput = data[3];

		m_Data.push_back(rec);

		//close the file
		_File.close();

	}

	unsigned epoch = 0;
	double errorRMSValidation = 0;

	for(epoch =0;epoch <_config.TotalEpoch -1 ;epoch++)
	{ 
	
		for each(DataRecord rec in m_Data)
		{
			vector<double> input;
			input.push_back(rec.LeftInput);
			input.push_back(rec.RightInput);

			vector<double> output;
			output.push_back(rec.LeftOutput);
			output.push_back(rec.RightOutput);

			vector<double> result = FeedForward(neuralUtility.Normalise(input, m_Max_Input, m_Min_Input));
			//calculate error & backpropogate

			BackPropogation(neuralUtility.Normalise(output, m_Max_Output, m_Min_Output));
			//to record the error

		}

		//run the validation data
		double errorRMSValidation = ValidationRun();

		//OutputResults(epoch);

		RMSResults(epoch, m_error, errorRMSValidation, 0);

		neuralUtility.SuffleArrayData(m_Data);
		
	}

	//Last iteration
	errorRMSValidation = ValidationRun();
	double errorRMSTest = TestRun();
	OutputResults(epoch);
	RMSResults(epoch, m_error, errorRMSValidation, errorRMSTest);
	_cputs("press any key to continue");
	_getch();
}

vector<double> NeuralNetwork::FeedForward(const vector<double> &input)
{
		
	//latch the input into the input neurons
	for (unsigned j = 0; j < input.size(); ++j)
	{
		m_Layers[0][j].SetOutputValue(input[j]);
	}


	//deal with hidden layers

	for (unsigned i = 1; i < m_Layers.size(); ++i)
	{
		Layer &lastLayer = m_Layers[i - 1];
		for (unsigned j = 0; j < m_Layers[i].size() - 1; ++j)
		{
			m_Layers[i][j].FeedForward(lastLayer);
		}
	}

	// output of last layer
	vector<double> returnValues;
	
	Layer lastLayer = m_Layers.back();

	for (unsigned i = 0; i < lastLayer.size()-1; ++i)
	{
		returnValues.push_back(lastLayer[i].GetOutputValue());
	}


	return returnValues;
}

void NeuralNetwork::BackPropogation(const vector<double> &expectedOutput)
{
	
	//calculate the error
	double error = 0;

	Layer &lastLayer = m_Layers.back();

	for (unsigned i = 0; i < lastLayer.size() -1; ++i)
	{
		double deltaError = expectedOutput[i] - lastLayer[i].GetOutputValue();
		error = deltaError * deltaError;
	}
	
	error = sqrt(error / (lastLayer.size() - 1)); //RMS Value

	m_error = error;

	//calculate the gradient for output layer

	for (unsigned i = 0; i < lastLayer.size() - 1; ++i)
	{
		lastLayer[i].CalculateOutputGradients(expectedOutput[i]);
	}

	//calculate the gradient for hidden layer

	for (unsigned i = m_Layers.size() - 2; i > 0; --i)
	{
		Layer &hiddenLayer = m_Layers[i];
		Layer &nextLayer = m_Layers[i + 1];
		for (unsigned j = 0; j < hiddenLayer.size(); ++j)
		{
			hiddenLayer[j].CalculateHiddenGradients(nextLayer);
		}
	}

	//update weights for all layers

	for (unsigned i = m_Layers.size() - 1; i > 0; --i)
	{
		Layer &currentLayer = m_Layers[i];
		Layer &PreviousLayer = m_Layers[i - 1];

		for (unsigned j = 0; j < currentLayer.size() - 1; ++j)
		{
			currentLayer[j].UpdateInputWeights(PreviousLayer);
		}
	}
}

vector<double> NeuralNetwork::Run(const vector<double> &input)
{
	
	//initialize weights
	//normalise weights
	//run forward
	//denormalise data
	return neuralUtility.DeNormalise(FeedForward(neuralUtility.Normalise(input, m_Max_Input, m_Min_Input)), m_Max_Output, m_Min_Output);
}

void NeuralNetwork::OutputResults(int epoch)
{
	
	ofstream out(_config.ResultFileName,std::ofstream::app);

	//write epoh number, alpha, lambda, learningrate
	
	std::ostringstream topology;
	std::copy(_config.Topology.begin(), _config.Topology.end() - 1,
		std::ostream_iterator<unsigned>(topology, ","));
 	topology << _config.Topology.back();

	out << topology.str() <<  endl;
	out << _config.Lambda << endl;
	out<<_config.Alpha << endl;
	out<< _config.LearningRate << endl;
	//weights and deltaweights
	//input to output layer


	for (unsigned i = 0; i < m_Layers.size(); ++i)
	{
		Layer &layer = m_Layers[i];
		
		for (unsigned j = 0; j < layer.size(); ++j)
		{
			out << layer[j].GetOutputValue()<<endl;
	
			vector <double> weights = layer[j].GetWeights();

			std::ostringstream weightsString;
			if (weights.size() > 0)
			{ 
				std::copy(weights.begin(), weights.end() - 1,
					std::ostream_iterator<double>(weightsString, ","));
				weightsString << weights.back();
				out << weightsString.str() << endl;
			}
   		}
	}

	out.flush();
	out.close();
}

void NeuralNetwork::RMSResults(int epoch, double trainingRSM, double validationRSM , double testRSM )
{
	ofstream out(_config.RMSFileName, std::ofstream::app);

	out << epoch << "," << trainingRSM  << "," << validationRSM << "," << testRSM << endl;

	out.flush();
	out.close();

	cout.precision(17);
	cout << trainingRSM << " ,  " << validationRSM << "," << testRSM << endl;
}

void NeuralNetwork::Initialise()
{
	string row;
	NeuralUtility neuralUtility;
	FileUtility fileUtility;

	if (_config.InitialisationFile != "")
	{
		
		fileUtility.OpenFile(_config.InitialisationFile, 'r');

		row = fileUtility.GetNextRow();	 //toplogy info

		_config.Topology.clear();
		_config.Topology = neuralUtility.ConvertStringToVectorUnsigned(row,',');
		row = fileUtility.GetNextRow();

		 _config.Lambda = stod(row);

		 row = fileUtility.GetNextRow();
		_config.Alpha = stod(row);

		row = fileUtility.GetNextRow();
		_config.LearningRate = stod(row);
	}
	unsigned noOfLayers = _config.Topology.size();
	int size = _config.Topology.size();

	for (unsigned i = 0; i < noOfLayers; ++i)
	{
		m_Layers.push_back(Layer());
		unsigned noOfOutputs = (i == size - 1 ? 0 : _config.Topology[i + 1]);

		//create neuron per layer including bias
		for (unsigned j = 0; j <= _config.Topology[i]; ++j)
		{
			if (_config.InitialisationFile != "")
			{
				
				double output = stod(fileUtility.GetNextRow());
				if (_config.Topology.size() - 1 != i)
				{
					row = fileUtility.GetNextRow();
					vector<double> weights = neuralUtility.ConvertStringToVectorDouble(row, ',');
					
					m_Layers.back().push_back(Neuron(noOfOutputs, j, _config, output
						, weights)); 
				}
				else
				{
					m_Layers.back().push_back(Neuron(noOfOutputs, j, _config));
				}

			}
			else
			{
				m_Layers.back().push_back(Neuron(noOfOutputs, j, _config));
			}

		}

	}

	if (_config.InitialisationFile != "")
	{
		fileUtility.CloseFile();
	}
}
NeuralNetwork::~NeuralNetwork()
{
	//realase all variables

	
}
