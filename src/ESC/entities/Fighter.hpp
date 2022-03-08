#ifndef FIGHTER_HPP
#define FIGHTER_HPP

#include "ESC/entities/Entity.hpp"
#include "containers/EnemyData.hpp"

class Fighter
{

private:
	AttributeComponent attrComp;
	BodyComponent body;
	AnimationComponent animationComp;

public:
	Fighter(attributes& attributes, std::vector<AnimationData>& animData, sf::Texture* tex, sf::Font& font, sf::Vector2f res);
	Fighter(AttributeComponent& attrComp, std::vector<AnimationData>& animData, sf::Texture* tex, sf::Vector2f res);
	~Fighter();
	//accessors
public:
	//returns true if certain anim cycle ended
	bool IsCycleDone(const std::string& animName);

	//returns reference to attributes
	attributes& GetAttributes();

	//returns reference to attrComp
	AttributeComponent& GetAttributeComponent();
	//modifiers
public:
	void SetPosition(sf::Vector2f pos);

	//public functions
public:
	void AfterInit(sf::Vector2f res);

	//updates
	void Update(float dt);

	//plays specific animation
	void PlayAnim(const std::string& name, float dt, bool reset = true);

	void StopAnimating();

	//displays
	void Display(sf::RenderTarget& trg);

	//private funtions
private:
	void CreateAnimations(std::vector<AnimationData>& animData);
};

#endif