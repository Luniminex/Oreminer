#include "Recipe.hpp"

Recipe::Recipe(std::vector<int> ingredients, std::vector<int> amounts, int result)
{
	this->ingredients = ingredients;
	this->amounts = amounts;
	this->result = result;
}