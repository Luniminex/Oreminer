#include "ESC/components/BodyComponent.hpp"

//constructor / destructor

BodyComponent::BodyComponent(sf::Texture& tex, sf::Vector2f scale) : tex(tex)
{
	this->sprite.setTexture(tex);
	sprite.setScale(scale);
}

//functions

sf::Sprite& BodyComponent::GetSprite(){
	return this->sprite;
}

sf::Texture& BodyComponent::GetTexture()
{
	return tex;
}

sf::Vector2f BodyComponent::GetPos(){
	return sprite.getPosition();
}

void BodyComponent::SetPos(sf::Vector2f pos){
	sprite.setPosition(pos);
}

void BodyComponent::Display(sf::RenderTarget& target){
	target.draw(this->sprite);
}

