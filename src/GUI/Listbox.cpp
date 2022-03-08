#include "GUI/GUI.hpp"

/*********************Constructors / destructors**********************************/
GUI::Listbox::Listbox(float xPerc, float yPerc, unsigned int NOElementsShown, sf::Font& font,
	sf::Texture& texSheet, sf::Vector2f sizePerc, sf::Vector2f res) :
	TexturedGUI(),
	upBtn(xPerc, yPerc, sf::Vector2f(sizePerc.x * (2.f / 3.f), sizePerc.x * (1.f / 3.f)), font, "Up", sf::Color(74, 74, 67, 128), sf::Color(110, 110, 98, 160), sf::Color(145, 145, 129, 200), res),
	downBtn(xPerc, yPerc, sf::Vector2f(sizePerc.x * (2.f / 3.f), sizePerc.x * (1.f / 3.f)), font, "Down", sf::Color(74, 74, 67, 128), sf::Color(110, 110, 98, 160), sf::Color(145, 145, 129, 200), res),

	font(font),
	texSheet(texSheet)
{
	shown = false;
	areControlBtnsShown = false;
	textured = true;
	textureType = btnTypes::bgr;
	this->currentRes = res;

	this->keyTimeMax = 10.f;
	this->keyTime = 0.f;

	this->NOElementsShown = NOElementsShown;
	this->NOELements = 0;
	this->sizePerc = sizePerc;

	shape.setSize(sf::Vector2f(PeToPiX(sizePerc.x, res.x), PeToPiY(sizePerc.y, res.y) * NOElementsShown));
	shape.setPosition(upBtn.GetGlobalBounds().left + (upBtn.GetGlobalBounds().width / 2.f) - (shape.getSize().x / 2.f),
		upBtn.GetGlobalBounds().top + upBtn.GetGlobalBounds().height);
	shape.setTexture(&btnTextures.at(textureType));
	sf::IntRect tempRect;
	tempRect.left = 0;
	tempRect.top = 0;
	tempRect.height = btnTextures.at(textureType).getSize().y;
	tempRect.width = btnTextures.at(textureType).getSize().x;
	shape.setTextureRect(tempRect);

	downBtn.SetPosition(PeToPiX(xPerc, res.x), shape.getGlobalBounds().top + shape.getGlobalBounds().height);
}

GUI::Listbox::~Listbox()
{
}
/*********************Accessors**********************************/
std::vector<std::unique_ptr<GUI::Displaybox>>& GUI::Listbox::GetList()
{
	return list;
}

GUI::Displaybox& GUI::Listbox::GetElementAt(int index)
{
	return *list.at(index);
}

bool GUI::Listbox::IsShown()
{
	return shown;
}

bool GUI::Listbox::AreBtnsShown()
{
	return areControlBtnsShown;
}

bool GUI::Listbox::GetKeyTimer()
{
	if (keyTime >= keyTimeMax)
	{
		keyTime = 0.f;
		return true;
	}
	return false;
}

ItemData& GUI::Listbox::GetInput()
{
	for (auto& a : list)
	{
		if (a->GetState() == btnStates::BTN_PRESSED)
		{
			return a->GetItemData();
		}
	}

	return list.at(0)->GetItemData();
}

sf::FloatRect GUI::Listbox::GetBounds()
{
	return shape.getGlobalBounds();
}

unsigned int GUI::Listbox::GetInputIndex()
{

	for (unsigned int i = 0; i < list.size(); i++)
	{
		if (list.at(i)->GetState() == btnStates::BTN_PRESSED)
		{
			return i;
		}
	}
	return 0;
}

/********************Modifiers**********************************/
void GUI::Listbox::Show()
{
	shown = !shown;
}

void GUI::Listbox::ShowControlBtns(bool toShow)
{
	areControlBtnsShown = toShow;
}

void GUI::Listbox::PushBackElement(ItemData& data)
{
	list.push_back(std::unique_ptr<GUI::Displaybox>(new GUI::Displaybox(
		shape.getPosition().x, shape.getPosition().y + (PeToPiY(sizePerc.y, currentRes.y) * NOELements), sizePerc, &texSheet, data, font, currentRes)));

	NOELements++;

	shownElements.push_back(shownElements.size());

	MoveListDown(); //temporal solution for elements not showing inside listbox before moving the list
}

void GUI::Listbox::RemoveElementAt(unsigned int IDelement)
{
	list.erase(list.begin() + IDelement); //idk if this works or not.. XD
}

void GUI::Listbox::Clear()
{
	NOELements = 0;
	shownElements.clear();
	list.clear();
}
/*********************Public functions**********************************/
bool GUI::Listbox::UpdateInput()
{
	if (shown == true)
	{
		for (auto& a : list)
		{
			if (a->GetState() == btnStates::BTN_PRESSED && GetKeyTimer() == true)
			{
				return true;
			}
		}
	}
	return false;
}

void GUI::Listbox::Update(sf::Vector2i mousePosWindow, float dt)
{
	if (shown)
	{
		UpdateKeyTimer(dt);
		UpdateBtnInput();
		if (areControlBtnsShown)
		{
			upBtn.Update(mousePosWindow);
			downBtn.Update(mousePosWindow);
		}
		UpdateList(mousePosWindow);
	}
}

void GUI::Listbox::Display(sf::RenderTarget& trg)
{
	if (shown)
	{
		if (areControlBtnsShown)
		{
			upBtn.Display(trg);
			downBtn.Display(trg);
		}
		trg.draw(shape);
		DisplayList(trg);
	}
}
/*****************************Private functions**************************/

void GUI::Listbox::UpdateKeyTimer(float dt)
{
	if (keyTime <= keyTimeMax)
	{
		keyTime += 100.f * dt;
	}
}

void GUI::Listbox::AfterInit(sf::Vector2f res)
{
	currentRes = res;

	upBtn.AferInit(res);
	downBtn.AferInit(res);
	shape.setSize(sf::Vector2f(PeToPiX(sizePerc.x, res.x), PeToPiY(sizePerc.y, res.y) * NOElementsShown));
	shape.setPosition(upBtn.GetGlobalBounds().left + (upBtn.GetGlobalBounds().width / 2.f) - (shape.getSize().x / 2.f),
		upBtn.GetGlobalBounds().top + upBtn.GetGlobalBounds().height);

	downBtn.SetPosition(downBtn.GetPos().x, shape.getGlobalBounds().top + shape.getGlobalBounds().height);

	for (auto& a : list)
	{
		a->AfterInit(res);
	}

	MoveListDown(); //temporal solution for elements not showing inside listbox before moving the list
	MoveListUp();
}

void GUI::Listbox::MoveListUp()
{
	if (shownElements.size() > 1) //move list if there are atleast two elements
	{
		int lastVal;		  //saves temporarily last val of vector
		unsigned int counter; //counter that is one ahead of iterator

		lastVal = shownElements.at(shownElements.size() - 1);			  //saves the last value
		shownElements.at(shownElements.size() - 1) = shownElements.at(0); // last val -> first val -> ring like

		counter = 1;

		for (unsigned int i = 0; i < shownElements.size(); ++i, ++counter)
		{

			if (counter >= shownElements.size())
			{
				break;
			}
			shownElements.at(i) = shownElements.at(counter); // moves element back by one
		}
		shownElements.at(shownElements.size() - 2) = lastVal; // sets penultimate value to the last value
	}
	if (shownElements.size() > 0)
	{
		for (unsigned int i = 0; i < NOElementsShown; ++i)
		{
			if (i > shownElements.size() - 1) // end loop if there are no elements left
			{
				break;
			}

			if (static_cast<unsigned>(shownElements.at(i)) >= list.size())
			{
				continue;
			}

			if (list.at(shownElements.at(i)) != NULL)
			{ //sets positions of shown elements
				list.at(shownElements.at(i))->SetPosition(shape.getPosition().x, shape.getPosition().y + (PeToPiY(sizePerc.y, currentRes.y) * i));
			}
		}
	}
}

void GUI::Listbox::MoveListDown()
{
	if (shownElements.size() > 1) //move list if there are atleast two elements
	{
		int counter;											  //counter that is one behind of iterator
		int lastVal = shownElements.at(shownElements.size() - 1); //saves temporarily last val of vector
		counter = shownElements.size() - 2;						  //counter is set to penultimate size of vector

		for (int i = shownElements.size() - 1; i >= 0; --i, --counter)
		{ //iterates from behind to front

			if (counter < 0)
			{ // cant go past 0
				break;
			}
			shownElements.at(i) = shownElements.at(counter); //sets element to one that its behind from it
		}
		shownElements.at(0) = lastVal; //sets first val to last val -> ring like
	}
	if (shownElements.size() > 0)
	{
		for (unsigned int i = 0; i < NOElementsShown; ++i)
		{
			if (i > shownElements.size() - 1) // end loop if there are no elements left
			{
				break;
			}

			if (static_cast<unsigned>(shownElements.at(i)) >= list.size())
			{
				continue;
			}

			if (list.at(shownElements.at(i)) != NULL)
			{ //sets positions to shown elements
				list.at(shownElements.at(i))->SetPosition(shape.getPosition().x, shape.getPosition().y + (PeToPiY(sizePerc.y, currentRes.y) * i));
			}
		}
	}
}

void GUI::Listbox::UpdateBtnInput()
{
	if (upBtn.GetState() == btnStates::BTN_PRESSED && GetKeyTimer() == true)
	{
		MoveListUp();
	}
	if (downBtn.GetState() == btnStates::BTN_PRESSED && GetKeyTimer() == true)
	{
		MoveListDown();
	}
}

void GUI::Listbox::UpdateList(sf::Vector2i mousePosWindow)
{
	if (shownElements.size() > 0) //update only if list contains atleast one element
	{
		for (unsigned int i = 0; i < NOElementsShown; ++i) //updates shown buttons
		{
			if (i > shownElements.size() - 1) // end loop if there are no elements left
			{
				break;
			}
			if (static_cast<unsigned>(shownElements.at(i)) >= list.size())
			{
				continue;
			}

			if (list.at(shownElements.at(i)) != NULL)
			{
				list.at(shownElements.at(i))->Update(mousePosWindow);
			}
		}
	}
}

void GUI::Listbox::DisplayList(sf::RenderTarget& trg)
{
	if (shownElements.size() > 0) //display if atleast one element
	{
		for (unsigned int i = 0; i < NOElementsShown; ++i) //displays shown buttons
		{
			if (i > shownElements.size() - 1) // end loop if there are no elements left
			{
				break;
			}

			if (static_cast<unsigned>(shownElements.at(i)) >= list.size())
			{
				continue;
			}

			if (list.at(shownElements.at(i)) != NULL)
			{
				list.at(shownElements.at(i))->Display(trg);
			}
		}
	}
}
