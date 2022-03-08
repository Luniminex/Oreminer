#include "ESC/entities/Fighter.hpp"

/*************constructors / destructors***************/
Fighter::Fighter(attributes& attributes, std::vector<AnimationData>& animData, sf::Texture* tex, sf::Font& font, sf::Vector2f res) :
	attrComp(attributes, font, res),
	body(*tex, sf::Vector2f(2.1f * res.x / 1000, 2.1f * res.x / 1000)),
	animationComp(body.GetSprite(), *tex)
{
	std::cout << "Creating fighter" << std::endl;
	CreateAnimations(animData);
	//attrComp.CombineAttributes();

	std::cout << "Created fighter " << std::endl;
}

Fighter::Fighter(AttributeComponent& attrComp, std::vector<AnimationData>& animData, sf::Texture* tex, sf::Vector2f res) :
	attrComp(attrComp),
	body(*tex, sf::Vector2f(2.1f * res.x / 1000, 2.1f * res.x / 1000)),
	animationComp(body.GetSprite(), *tex)
{
	std::cout << "Creating fighter from attribute component" << std::endl;
	CreateAnimations(animData);
	this->attrComp.CombineAttributes();

	std::cout << "Created fighter from attribute component" << std::endl;
}

Fighter::~Fighter()
{
	attrComp.UncombineAttributes();
}

void Fighter::SetPosition(sf::Vector2f pos)
{
	body.GetSprite().setPosition(pos);
}
/*******accessors*********/
bool Fighter::IsCycleDone(const std::string& animName)
{
	return animationComp.EndedCycle(animName);
}

attributes& Fighter::GetAttributes()
{
	return attrComp.getAttributes();
}

AttributeComponent& Fighter::GetAttributeComponent()
{
	return attrComp;
}
/**********public functions***********/

void Fighter::AfterInit(sf::Vector2f res)
{
	attrComp.AfterInit(res);
	body.GetSprite().setScale(sf::Vector2f(2.1 * res.x / 1000, 2.1 * res.x / 1000));
}

void Fighter::Update(float dt)
{
	(void)dt;
}

void Fighter::PlayAnim(const std::string& name, float dt, bool reset)
{
	animationComp.Play(name, dt, false, 100.f, reset);
}

void Fighter::StopAnimating()
{
	animationComp.StopAnimationAt(0, 0, 0, 0);
}

void Fighter::Display(sf::RenderTarget& trg)
{
	body.Display(trg);
}

/***********************private functions***********/

void Fighter::CreateAnimations(std::vector<AnimationData>& animData)
{
	for (auto& a : animData)
	{
		animationComp.CreateAnimation(a.name, body.GetSprite(), body.GetTexture(), 128, 128, a.startx, a.starty, a.frames, a.frametime);
	}
	body.GetSprite().setOrigin(64.f, 64);
}
