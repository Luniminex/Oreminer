#include "Attributes.hpp"

attributes::attributes()
{
	//std::cout<<"Creating attributes"<<std::endl;
	atrTypes.resize(atrNameMap.size());
	//std::cout<<"Created attributes"<<std::endl;
}

attributes::attributes(attributes& atrs)
{
	//std::cout<<"Creating from copy constructor"<<std::endl;
	atrTypes.resize(atrNameMap.size());
	//std::cout << "Copy constructor" << std::endl;
	for (unsigned int i = 0; i < atrs.atrTypes.size(); i++)
	{
		//std::cout << " atrtypes at " << i << " :" << atrs.atrTypes.at(i) << std::endl;
		atrTypes.at(i) = atrs.atrTypes.at(i);
	}
	//std::cout<<"created attributes from copy constructor"<<std::endl;
}

void attributes::Init()
{
	atrTypes.resize(atrNameMap.size());
	ResetAll();
}

void attributes::IncreaseAllBy(attributes& atrs)
{
	int i = 0;
	for (auto& a : atrTypes)
	{
		a += atrs.atrTypes.at(i);
		i++;
	}
}

void attributes::DecreaseAllBy(attributes& atrs)
{
	int i = 0;
	for (auto& a : atrTypes)
	{
		a -= atrs.atrTypes.at(i);
		i++;
	}
}

void attributes::ResetAll()
{
	for (auto& a : atrTypes)
	{
		a = 0;
	}
}

void attributes::PrintAll()
{

	int i = 0;
	for (auto& a : atrNameMap)
	{
		//std::cout << a << ": " << atrTypes.at(i) << std::endl;
		i++;
		(void)a;
	}
}

attributes attributes::operator+(attributes attrs)
{
	attributes temp;
	temp.Init();
	for (unsigned int i = 0; i < attrs.atrTypes.size(); i++)
	{
		temp.atrTypes.at(i) = atrTypes.at(i) + attrs.atrTypes.at(i);
	}
	return temp;
}

attributes attributes::operator-(attributes attrs)
{
	attributes temp;
	temp.Init();
	for (unsigned int i = 0; i < attrs.atrTypes.size(); i++)
	{
		temp.atrTypes.at(i) = atrTypes.at(i) - attrs.atrTypes.at(i);
	}
	return temp;
}