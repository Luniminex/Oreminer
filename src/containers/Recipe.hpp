#ifndef RECIPE_HPP
#define RECIPE_HPP

struct Recipe
{
	std::vector<int> ingredients; //ids of items needed to craft result
	std::vector<int> amounts;	  //amounts needed
	int result;					  // id of item that will be crafted from ingredients

	Recipe(std::vector<int> ingredients, std::vector<int> amounts, int result);
};

#endif