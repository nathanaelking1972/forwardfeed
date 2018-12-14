#pragma once
#include <cmath>
#include <vector>
#include "Connection.h"
#include <cstdlib>

class Neuron
{
public:


	Neuron(unsigned numberOfOutputs, unsigned id, const NeuralNetworkConfig &config)
	{
		
		for (unsigned connection = 0; connection < numberOfOutputs; ++connection)
		{
			m_outputWeights.push_back(Connection());
			m_outputWeights.back().Weight = RandomWeight();
		}

		m_id = id;
		m_outputValue = 1;
		Eta = config.LearningRate;
		Alpha = config.Alpha;
		Lambda = config.Lambda;
	}

	Neuron(unsigned numberOfOutputs, unsigned id, const NeuralNetworkConfig &config, double NeuronValue, vector<double> &weights)
	{

		for (unsigned connection = 0; connection < numberOfOutputs; ++connection)
		{
			m_outputWeights.push_back(Connection());
			m_outputWeights.back().Weight = weights[connection];
		}

		m_id = id;
		m_outputValue = 1;
		Eta = config.LearningRate;
		Alpha = config.Alpha;
		Lambda = config.Lambda;
	}
	void FeedForward(const vector<Neuron> &previousLayer);
	void SetOutputValue(double value) { m_outputValue = value; }
	double GetOutputValue() const { return m_outputValue; }
	vector<double> GetWeights() const { 
		vector<double> weights;
		for (unsigned i = 0; i < m_outputWeights.size(); ++ i)
		{
			weights.push_back(m_outputWeights[i].Weight);
		}

		return weights;
	}

	vector<double> GetDeltaWeights() const {
		vector<double> weights;
		for (unsigned i = 0; i < m_outputWeights.size() - 1; ++ i)
		{
			weights.push_back(m_outputWeights[i].deltaWeight);
		}

		return weights;
	}

	void CalculateOutputGradients(double value)
	{
		m_gradient = Lambda * m_outputValue * (1 - m_outputValue) * (value - m_outputValue);
	}
	void CalculateHiddenGradients(const vector<Neuron>  &nextLayer)
	{
		double sumGW  = SumOfDeltaWeightGradient(nextLayer);
		m_gradient = sumGW * Lambda * m_outputValue * (1 - m_outputValue);
	}
	void UpdateInputWeights(vector<Neuron>  &previousLayer);
	
	double Eta;
	double Alpha;
	double Lambda;

private:
	unsigned m_id;
	double m_outputValue;
	vector<Connection> m_outputWeights;
	double m_gradient;
	
	double RandomWeight(void) { return rand() / double(RAND_MAX); }
	double CalculateActivationValue(double total)
	{
		return 1 / (1 + exp(-Lambda * total));

	}
	double CalculateDerivative(double value)
	{
		return  1.0 - value * value;
	}
	double SumOfDeltaWeightGradient(const  vector<Neuron>  &layer)
	{
		double sum=0;
		for (unsigned n = 0; n < layer.size() - 1; ++n)
		{
			sum += m_outputWeights[n].Weight * layer[n].m_gradient;
		}

		return sum;
	}
	
};


void Neuron::FeedForward(const  vector<Neuron>  &previousLayer)
{
	
	double total = 0;
	for (unsigned i = 0; i < previousLayer.size(); ++i)
	{
		total += previousLayer[i].GetOutputValue() * previousLayer[i].m_outputWeights[m_id].Weight;
	}

	m_outputValue = Neuron::CalculateActivationValue(total);
}

void Neuron::UpdateInputWeights(vector<Neuron>  &previousLayer)
{
	for (unsigned n = 0; n < previousLayer.size(); ++n)
	{
		Neuron &neuron = previousLayer[n];
		double oldDeltaWeight = neuron.m_outputWeights[m_id].deltaWeight;
		//individual input, magnified by the gradient and train rate
		double newDeltaWeight = Eta * neuron.GetOutputValue()* m_gradient + Alpha * oldDeltaWeight;
		neuron.m_outputWeights[m_id].deltaWeight = newDeltaWeight;
		neuron.m_outputWeights[m_id].Weight += newDeltaWeight;
	}
}

