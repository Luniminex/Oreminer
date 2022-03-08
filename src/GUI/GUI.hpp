#ifndef GUI_HPP
#define GUI_HPP

#include "containers/ItemData.hpp"
#include "managers/ResourceManager.hpp"
#include "observers/AudioPlayer.hpp"
#include <algorithm>
#include <vector>

class ItemData;

enum btnStates
{
	BTN_IDLE = 0,
	BTN_HOVER,
	BTN_PRESSED
};

namespace GUI
{

enum btnTypes
{
	basic,
	big,
	bgr,
	wide
};
//converts % value to pixel value relative to resolution - on X axis
float PeToPiX(const float perc, float res);

//converts % value to pixel value relative to resolution - on Y axis
float PeToPiY(const float perc, float res);

//converts pixel value to % value relative to resolution on x axis
float PiToPeX(const float pixelValue, float resWidth);

//converts pixel value to % value relative to resolution on xyaxis
float PiToPeY(const float pixelValue, float resHeight);

//calculates char size relative to screen resolution
unsigned int CaclCharSize(sf::Vector2f res);

//calculates char size relative to screen resolution
unsigned int CaclCharSizeSmall(sf::Vector2f res);

//calculates char size relative to screen resolution
unsigned int CaclCharSizeSmallest(sf::Vector2f res);

//calculates char size relative to screen resolution
unsigned int CaclCharSize(float resWidth, float resHeight);

//calculates char size relative to screen resolution
unsigned int CaclCharSizeSmall(float resWidth, float resHeight);

//calculates char size relative to screen resolution
unsigned int CaclCharSizeSmallest(float resWidth, float resHeight);

//inserts lines into text so the text can fit inside bounds
std::string FitTextInsideBox(sf::Text& txt, float bound);

class TexturedGUI
{
public:
	bool textured = false;
	int textureType = -1;
	static std::map<int, sf::Texture> btnTextures;

	TexturedGUI();
	~TexturedGUI();
	static void LoadTextures();
};

class Button : public TexturedGUI
{
public:
	unsigned int state;

private:
	sf::RectangleShape body;
	sf::Font& font;
	sf::Text text;

	sf::Color idleClr;	  //barva výplně, když není kurzor v oblasti tlačítka
	sf::Color hoverClr;	  //barva výplně, když je kurzor v oblasti tlačítka
	sf::Color pressedClr; //barva výplně, když uživatel zmáčknul tlačítko

	sf::Color idleOutlClr;	  //barva obrysu, když není kurzor v oblasti tlačítka
	sf::Color hoverOutlClr;	  //barva obrysu, když je kurzor v oblasti tlačítka
	sf::Color pressedOutlClr; // barva obrysu, když uživatel zmáčknul tlačítko

	sf::Vector2f posPerc;  // pozice v procentech
	sf::Vector2f sizePerc; //velikost v procentech

	bool shown; //jestli Je tlačítko ukázáno

public:
	//use this function at the start of the program to load all btn textures
	static void LoadBtnTextures();

	//returns position of button
	sf::Vector2f GetPos();

	//returns global bounds of btn
	sf::FloatRect GetGlobalBounds();

	//returns size of button
	sf::Vector2f GetSize();

	//returns current state of btn
	unsigned int GetState();

	//returns display text
	const std::string GetText();

	//returns true if btn is shown
	bool IsShown();

	//modifiers
public:
	//sets position of button
	void SetPosition(sf::Vector2f pos);

	//sets positions
	void SetPosition(float x, float y);

	//sets size
	void SetSize(float x, float y);

	//sets char size of text
	void SetCharSize(unsigned int size);

	//sets text of the button
	void SetText(const std::string& str);

	//sets the outline properties
	void setOutline(float outlineThickness, sf::Color idleOutlClr,
		sf::Color hoverOutlClr, sf::Color pressedOutlClr);

	//sets shown
	void SetShown(bool shown);

	//constructors
public:
	Button(float posXPerc, float posYPerc, sf::Vector2f sizeInPerc,
		sf::Font& font, const std::string& displayText, sf::Color idleClr, sf::Color hoverClr,
		sf::Color pressedClr, sf::Vector2f res);
	Button(float posXPerc, float posYPerc, sf::Vector2f sizeInPerc, sf::Font& font, const std::string& displayText, int textureBtnType, sf::Vector2f res);

	//copy constructor
	Button(const Button& btn);

	~Button();

	//functions
public:
	//reinitializes button after resolution change
	void AferInit(sf::Vector2f res);

	//updates states
	void Update(const sf::Vector2i& mousePosWindow);

	//renders to target
	void Display(sf::RenderTarget& target);

private:
	void InitBtn(float posXPerc, float posYPerc, sf::Vector2f sizePerc,
		sf::Font& font, const std::string& displayText, sf::Vector2f res);
};

class Checkbox
{

	//public variables
public:
	unsigned int state;
	bool checked;

	//variables
private:
	sf::RectangleShape body;
	sf::Font& font;
	sf::Text text;
	float offsetPerc;

	sf::Vector2f posPerc; //stores value in perc values
	sf::Vector2f sizePerc;

	sf::Color idleCheckedColor;
	sf::Color idleUncheckedColor;

	sf::Color hoverCheckedColor;
	sf::Color hoverUncheckedColor;

	sf::Color PressedCheckedColor;
	sf::Color PressedUncheckedColor;
	/**********************************/
	sf::Color idleCheckedColorOut;
	sf::Color idleUncheckedColorOut;

	sf::Color hoverCheckedColorOut;
	sf::Color hoverUncheckedColorOut;

	sf::Color PressedCheckedColorOut;
	sf::Color PressedUncheckedColorOut;

	float keyTime; //key timer
	float keyTimeMax;

	//constructors
public:
	Checkbox(float posXPerc, float posYPerc, sf::Vector2f sizePerc, float offsetPerc, bool checked,
		sf::Font& font, const std::string& displayText, float outThickness, sf::Color idleCheckedColor,
		sf::Color idleUncheckedColor, sf::Color hoverCheckedColor, sf::Color hoverUncheckedColor,
		sf::Color PressedCheckedColor, sf::Color PressedUncheckedColor, sf::Color idleCheckedColorOut,
		sf::Color idleUncheckedColorOut, sf::Color hoverCheckedColorOut, sf::Color hoverUncheckedColorOut,
		sf::Color PressedCheckedColorOut, sf::Color PressedUncheckedColorOut, sf::Vector2f res);

	~Checkbox();

	//accessors
public:
	//returns state
	unsigned int GetState();

	//returns true if checked
	bool Checked();

	//modifiers
public:
	//sets positions
	void SetPosition(float x, float y, float offset);

	//sets size
	void SetSize(float x, float y);

	//sets char size of text
	void SetCharSize(unsigned int size);

	//sets text of the button
	void SetText(const std::string& str);

	//functions
public:
	//reinitializes button after resolution change
	void AferInit(sf::Vector2f res); ////////////////////////////////////////UDELĚEJ VIZ SETTINGS STATE

	//updates states
	void Update(const sf::Vector2i& mousePosWindow, float dt);

	//renders to target
	void Display(sf::RenderTarget& target);
	//private functions
private:
	//updates key timer
	void UpdateKeyTimer(float dt);

	//returns true if is able to click
	bool GetKeyTime();
};

class DropdownList
{
public:
	sf::Vector2f size;

	sf::Vector2f posPerc; //size in perc values
	sf::Vector2f sizePerc;

	bool isShown; //keepts track if drplist is unpacked

	std::unique_ptr<GUI::Button> activeEl;
	//std::unique_ptr<GUI::Button> switchBtn;			//the activated element from drplist
	std::vector<std::unique_ptr<GUI::Button>> list; //stores all the options for drplist

	float keyTime; //key timer
	float keyTimeMax;

	sf::Font& font;

	//constructors / destructors
public:
	DropdownList(float posXPerc, float posYPerc, sf::Vector2f sizePerc,
		sf::Font& font, std::vector<std::string> texts, sf::Vector2f res);
	~DropdownList();

	//accessors
public:
	//returns true if is able to do input
	bool GetKeyTimer();

	//returns text of drop down active element
	const std::string GetActiveText();

	//modifiers
public:
	//sets position
	void SetPosition(sf::Vector2f pos);

	//sets size
	void SetSize(float x, float y);

	//sets char size
	void SetCharSize(unsigned int size);

	//public functions
public:
	//reinitializes button after resolution change
	void AferInit(sf::Vector2f res); ////////////////////////////////dodělej viz settings state

	//updates drplist
	void Update(float dt, const sf::Vector2i& mousePosWindow);

	//displays dropdownlist to target
	void Display(sf::RenderTarget& target);

	//private functions
private:
	//inits dropdown list
	void InitDropdownList(float posXPerc, float posYPerc, sf::Vector2f sizePerc,
		sf::Font& font, std::vector<std::string> texts, sf::Vector2f res);

	//updates key timer
	void UpdateKeyTimer(float dt);

	//updates buttons
	void UpdateButtons(const sf::Vector2i& mousePosWindoww);
};

class TextureSelector
{
public:
	sf::RectangleShape bounds; // bounding rectangle
	sf::Sprite texSheet;
	Button hideButton;

	bool hidden;  // show tex. selector only if is not hidden
	bool active;  // if mouse pos is inside tex. selector
	bool changed; //true if texture was changed

	float gridSize; // size per grid inside tex selector

	float keyTime; //key timer
	float keyTimeMax;

	sf::Vector2f posPerc; // stores values in percentage values
	sf::Vector2f sizePerc;

	sf::Vector2f tileSize;

	sf::Vector2i gridPosInside;
	sf::IntRect selectedRect;

	//constructors
public:
	TextureSelector(float xPerc, float yPerc, sf::Vector2f btnSizePerc, const std::string& btnText, float texWidth, float texHeight,
		float gridSize, sf::Vector2f tileSize, sf::Font& font, sf::Texture& texture, sf::Vector2f res);

	~TextureSelector();

	//accessors
public:
	//returns true if is able to do input
	bool GetKeyTimer();

	//returns if it was changed and sets it back
	bool WasChanged();

	//returns rect that is set to the selected part of texture
	sf::IntRect& GetSelectedTexture();

	//calculates ID of thing through rectXY
	int GetSelectedID();

	//returns size of btn
	sf::Vector2f GetBtnSize();

	//modifiers
public:
	//sets positions
	void SetPosition(float x, float y);

	//sets size
	void SetSize(float x, float y);

	//sets char size of text
	void SetCharSize(unsigned int size);

	//sets texture
	void setTex(sf::Texture& tex);

	//functions
public:
	//inits selector
	void InitTS();

	//reinitializes button after resolution change
	void AferInit(sf::Vector2f res);

	//updates key timer
	void UpdateKeyTimer(float dt);

	//updates stuff
	void Update(float dt, sf::Vector2i mousePosWindow);

	//renders to screen
	void Display(sf::RenderTarget& trg);

private:
};

class Displaybox : public TexturedGUI
{

private:
	unsigned int state; //keeps track in which state it currently is

	sf::Vector2f posPerc; // stores values in percentage values
	sf::Vector2f sizePerc;

	sf::Text name;			  //name of the thing to display
	sf::Text detailsLeft;	  // contains details that update
	sf::Text detailsRight;	  // contains details that update
	sf::RectangleShape shape; //body of the whole thing
	sf::RectangleShape img;	  //img of the thing diplayed

	ItemData item;

	sf::Vector2f currentRes; // holds current resolution
							 //constructors
public:
	Displaybox(float posXPerc, float posYPerc, sf::Vector2f sizePerc, sf::Texture* texToDisplay,
		ItemData& itemData, sf::Font& font, sf::Vector2f res);

	~Displaybox();
	//accessors
public:
	static void LoadBtnTextures();
	//returns in which state display box is
	unsigned int GetState();

	//returns size of display box
	sf::Vector2f GetSize();

	//returns size of display box
	sf::Vector2f GetPosition();

	//returns item data
	ItemData& GetItemData();

	//modifiers
public:
	void SetPosition(float x, float y);

	void SetPercPosition(float xPerc, float yPerc);

	void ChangeTitle(const std::string& txt);
	void ChangeLeftSpecial(const std::string& txt);
	void ChangeRightSpecial(const std::string& txt);

	//functions
public:
	//functions should be used after changing resolution
	/*void AfterInit(float x, float y, float sizeX, unsigned int charSizeTitle,
		unsigned int charSizeDetail, float padding);*/

	//reinits gui after res has been changed
	void AfterInit(sf::Vector2f res);

	//upadates display box
	void Update(sf::Vector2i mousePosWin);

	//renders button to target
	void Display(sf::RenderTarget& trg);
};

class Listbox : public TexturedGUI
{

private:
	bool shown;				  //keep tracks if it is shown or not
	bool areControlBtnsShown; //keeps track if control btns are shown or not

	Button upBtn;	//button for navigating up in the list
	Button downBtn; //buttom for navigating down in the list

	sf::RectangleShape shape;
	sf::Font& font; //font used

	std::vector<std::unique_ptr<Displaybox>> list; //map of all display boxes

	unsigned int NOElementsShown; // how many elements are able to be shown
	unsigned int NOELements;	  //number of elements in map

	sf::Texture& texSheet;	 //text sheet elements use
	sf::Vector2f sizePerc;	 //size of elements in perc
	sf::Vector2f currentRes; // holds current resolution

	float keyTime; //key timer
	float keyTimeMax;

	std::vector<int> shownElements; //IDs of elemenets that are shown

	//constructors
public:
	Listbox(float xPerc, float yPerc, unsigned int NOElementsShown, sf::Font& font,
		sf::Texture& texSheet, sf::Vector2f sizePerc, sf::Vector2f res);

	~Listbox();

	//accessors
public:
	std::vector<std::unique_ptr<Displaybox>>& GetList();

	GUI::Displaybox& GetElementAt(int index);
	//returns true if is able to do input
	bool GetKeyTimer();
	//returns true if shown
	bool IsShown();
	//returns true if btns are shown or not
	bool AreBtnsShown();
	//get element at
	ItemData& GetInput();
	//returs bounds of shape
	sf::FloatRect GetBounds();
	//returns index at which was list clicked
	unsigned int GetInputIndex();

	//modifiers
public:
	//hides/shows listbox
	void Show();
	//hides/shows control btns
	void ShowControlBtns(bool toShow);
	//pushes back element
	void PushBackElement(ItemData& data);
	//removes element at index
	void RemoveElementAt(unsigned int IDelement);
	//clears listbox
	void Clear();

	//functions
public:
	//function to call after resolution was changed
	void AfterInit(sf::Vector2f res);
	//checks for input and if some element is clicked, return it
	bool UpdateInput();
	//updates all elements
	void Update(sf::Vector2i mousePosWindow, float dt);
	//draws all elements
	void Display(sf::RenderTarget& trg);

private:
	//updates key timer
	void UpdateKeyTimer(float dt);

	//moves list up
	void MoveListUp();

	//moves list down
	void MoveListDown();

	//updates input of btns
	void UpdateBtnInput();

	//updates list
	void UpdateList(sf::Vector2i mousePosWindow);

	//displays list
	void DisplayList(sf::RenderTarget& trg);
};

//box that describes items with option to show some special buttons
class DescriptionBox : public TexturedGUI
{
private:
	bool shown; // indicates if is shown or not

	sf::RectangleShape shape; //overall shape
	sf::RectangleShape img;	  //shows img of the item
	sf::Font& font;			  //font used

	std::vector<std::unique_ptr<Button>> btns; //vector of all buttons, going from left to right
	const int maxNOBtns = 5;
	int NOBtns; //total NO btns

	sf::Text title;		   // name of the item
	sf::Text description;  // description of item
	sf::Text leftSpecial;  //something to display on left side of desc box
	sf::Text rightSpecial; //something to display on right side of desc box

	sf::Vector2f posPerc;  //hold position on screen in perc values
	sf::Vector2f sizePerc; //hold size in perc values

	ItemData& data;

	sf::Vector2f currentRes;

	float keyTime; //key timer
	float keyTimeMax;

	//constructors
public:
	DescriptionBox(sf::Vector2f posPerc, sf::Vector2f sizePerc, sf::Texture& tex, sf::IntRect texRect, sf::Font& font,
		ItemData& data, sf::Vector2f res);

	~DescriptionBox();

	//accessors
public:
	//returns true if is able to do input
	bool GetKeyTimer();

	//returns true if is shown
	bool IsShown();

	//returns btn at index, if not shown, returns nullptr instead
	GUI::Button& GetButton(unsigned int index);

	//returns number of active btns
	int GetNOBtns();

	ItemData& GetItemData();

	//returns title string
	std::string GetTitle();

	//modifiers
public:
	//shows description box
	void Show(bool show);

	//hides description box
	void Hide();

	//adds button
	bool AddButton(const std::string& text);

	//removes last button
	bool RemoveButton();

	void RemoveAllButtons();

	//resets state of btns to idle
	void ResetStateBtns();

	void ChangeTextureRect(sf::IntRect texRect);

	void ChangeTitle(const std::string& text);

	void ChangeDescription(const std::string& text);

	void ChangeLeftSpecial(const std::string& text);

	void ChangeRightSpecial(const std::string& text);

	void SetPosition(sf::Vector2f position);

	void SetSize(sf::Vector2f size);

	void LoadItem(ItemData& data);

	//public functions
public:
	//function to call after resolution was changed
	void AfterInit(sf::Vector2f res);
	//updates all elements
	void Update(sf::Vector2i mousePosWindow, float dt);
	//draws all elements
	void Display(sf::RenderTarget& trg);
	//private functions
private:
	//updates key timer
	void UpdateKeyTimer(float dt);
};

class AttributeInfo : public TexturedGUI
{
	//variables
public:
	bool shown; //indicates if is shown or not

	sf::RectangleShape shape;
	sf::Text text;
	sf::Font& font;

	sf::Vector2f posPerc;			//position in perc values
	sf::Vector2f sizePerc;			//size in perc values
	sf::Vector2f currentResolution; //current resolutions

	float keyTime; //key timer
	float keyTimeMax;

	//constructors / destructors
public:
	AttributeInfo(sf::Vector2f posPerc, sf::Vector2f sizePerc, sf::Font& font, sf::Vector2f res);
	~AttributeInfo();

	//accessors
public:
	//returns true if is able to do input
	bool GetKeyTimer();

	//returns true if is shown
	bool IsShown();

	//modifiers
public:
	//clears text string
	void ClearAttributes();

	//shows element
	void Show();
	//hides element
	void Hide();
	//adds attribute
	void AddAttribute(const std::string& str);

	//public functions
public:
	void AfterInit(sf::Vector2f res);

	void Update(float dt);

	void Display(sf::RenderTarget& trg);

private:
	//updates key timer
	void UpdateKeyTimer(float dt);
};

class DialogueBox : public TexturedGUI
{

public:
	ResourceManager* resourceman;

	sf::Text dialogue;
	sf::RectangleShape box;
	sf::RectangleShape next;
	sf::Texture tex;
	std::vector<std::string>* texts;

	bool shown;
	bool nextPage; //if true, next part of dialogue is shown
	int page;	   //page currently on
	bool finished; //true if all pages have been reed
				   //constructor / destructor
public:
	DialogueBox(sf::Font& font, sf::Vector2f res);
	~DialogueBox();
	//accessors
public:
	bool IsShown();

	bool IsFinished();

	//modifiers
public:
	void Show(bool shown);

	void NextDialogue();

	void SetDialogue(std::vector<std::string>* texts);

	//public functions:
public:
	void AfterInit(sf::Vector2f res);

	bool Update();

	void Display(sf::RenderTarget& trg);
};

class QuestInfo
{
private:
	sf::Text questName;
	sf::Text questContext;
	sf::Text goals;
	sf::Text rewards;

	sf::Vector2f posPerc;
	sf::Vector2f sizePerc;

	sf::Font& font;
	sf::Vector2f res;

public:
	QuestInfo(float posXPerc, float posYPerc, sf::Vector2f sizePerc, sf::Font& font, sf::Vector2f res);
	~QuestInfo();

public:
	void SetQuestName(const std::string& text);

	void SetQuestContext(const std::string& text);

	void SetGoals(std::vector<std::string>& texts);

	void CompleteGoals();

	void SetRewards(std::vector<std::string>& texts);

	void AfterInit(sf::Vector2f res);

	void Update();

	void Display(sf::RenderTarget& trg);
};

class CraftBox : public TexturedGUI
{
private:
	sf::RectangleShape box;

	sf::RectangleShape result;

	std::vector<sf::RectangleShape> ingredients;
	std::vector<sf::Text> NOIngredients;
	std::vector<int> ingredientsID;
	std::vector<int> amounts;
	int resultID;

	sf::Color pressedClr;
	sf::Color hoverClr;
	sf::Color idleClr;

	sf::Color pressedOutlClr;
	sf::Color hoverOutlClr;
	sf::Color idleOutlClr;

	int state;
	sf::Vector2f posPerc;
	sf::Vector2f sizePerc;
	sf::Vector2f res;
	bool shown;

public:
	// list of tex coords used, last one is the result
	CraftBox(sf::Vector2f posPerc, sf::Vector2f sizePerc, sf::Texture& tex, std::vector<sf::Vector2f> texCoords, std::vector<int> NOitemsNeeded, std::vector<int> ingredientsID, int resultID, sf::Font& font, sf::Vector2f res);
	~CraftBox();

	int GetState();

	int GetResultID();

	std::vector<int> GetIngredientsID();

	std::vector<int> GetAmounts();

	void FunkyStuff();

	bool IsShown();

	void SetShown(bool shown);

	void Update(sf::Vector2i mousePosWindow);

	void Display(sf::RenderTarget& trg);
};

class CraftingInferface
{

private:
	sf::Vector2f posPerc;
	sf::Vector2f sizePerc;

	sf::Texture& TexForBoxes;
	sf::Font& font;
	sf::Vector2f res;

	bool shown;

	sf::RectangleShape box;
	DescriptionBox descBox;

	sf::Text pageIndicator;
	Button nextPage;
	Button previousPage;

	std::vector<std::vector<std::unique_ptr<CraftBox>>> crafts;

	int NOPages;
	int currentPage;
	int NoCrafts;
	int activePage;

	int craftBoxSelected;
	int selectedResult;
	std::vector<int> selectedIngredients;
	std::vector<int> selectedAmounts;
	bool needsRefresh;

	float keyTime; //key timer
	float keyTimeMax;

public:
	CraftingInferface(sf::Vector2f posPerc, sf::Vector2f sizePerc, ItemData& itemdata, sf::Texture& itemTexture, sf::Font& font, sf::Vector2f res);
	~CraftingInferface();

public:
	bool GetKeyTimer();

	bool IsShown();

	//if true make sure to load item to description box
	bool NeedsToRefresh();

	int GetSelectedResult();

	bool TriedCraft();

	std::vector<int> GetSelectedIngredients();

	std::vector<int> GetSelectedAmounts();

	void setShown(bool shown);
	/*
	*	loads itemdata to description box
	*/

	void ClearAll();

	GUI::CraftBox& GetCraftBoxAt(int index);

	void LoadItem(ItemData& data);

	void AddCraftBox(std::vector<sf::Vector2f> texCoords, std::vector<int> NOitemsNeeded, std::vector<int> ingredientsID, int resultID);

	int CheckInput();

	void Update(sf::Vector2i mousePosWindow, float dt);

	void AfterInit(sf::Vector2f res);

	void Display(sf::RenderTarget& trg);

	void UpdateKeyTimer(float dt);
};

class LevelUpScreen
{

private:
	sf::RectangleShape box;
	sf::Text congrats;
	sf::Text attributesText;
	std::map<int, std::unique_ptr<GUI::Button>> levelUpBtns;
	std::map<int, int> increaseValues;

	bool shown;
	bool leveledUp;
	int levelID;
	sf::Vector2f posPerc;
	sf::Vector2f sizePerc;
	sf::Vector2f res;
	sf::Font& font;

public:
	LevelUpScreen(sf::Vector2f posPerc, sf::Vector2f sizePerc, sf::Font& font, sf::Vector2f res);
	~LevelUpScreen();

	bool DidLevelUp();

	void SetLeveledUP(bool leveled);

	std::pair<int, int> LeveledID();

	bool IsShown();

	void SetShown(bool shown);

	void AfterInit(sf::Vector2f res);

	void LevelUp(std::vector<int>& atrTypes);

	void Update(sf::Vector2i mousePosWin);

	void Display(sf::RenderTarget& trg);
};

}
#endif