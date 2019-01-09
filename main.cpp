
//11/1/17 crow

//#include "stdafx.h"
#include "windows.h"
#include "time.h"
#include "lib/filesystem.h"
#include <conio.h>
#include <array>
#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <algorithm>
#include <map>
#include <memory>
#include <utility>
#include <stdexcept>
using namespace std;
using Filedata = baelothelib::Filedata;
using FileWriter = baelothelib::FileWriter;
using FileReader = baelothelib::FileReader;
HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);

void inventory();
void city();
void load();
void battle();
void save();
void estatgen();
void randcheck();
void enemyturn();
void clvl();
void statcheck();
void inven();
void wait_enter();
void start();
void goblinnaming();
void sout(string message, int delay, bool endline); //Declares the sout method.

string savename;
bool savedone = false;

enum {
	COLOR_LIGHTBLUE = 9,
	COLOR_LIGHTGREEN,
	COLOR_LIGHTCYAN,
	COLOR_LIGHTRED,
	COLOR_LIGHTMAGENTA,
	COLOR_YELLOW,
	COLOR_WHITE,
};

/* Item system 0.5 by baelothe      *
* Changelog:                       *
* 0.5: Initial version             *
* Planned versions:                *
* 0.6: Better autocasting,         *
* inventory categories             *
* 0.7: Inventory filters and       *
* sorting                          *
* 0.8: General rewrite             *
* 0.9: Improve the main parts of   *
* the system                       *
* 1.0: Item scripting              */
namespace ItemSystem
{
	/* The Items namespace holds the different Item subtypes  *
	* and the item categories. Define any new subtypes in    *
	* this namespace.                                        */
	namespace Items
	{
		/* Item categories are used to downcast to derived      *
		* pointers and sorting the inventory (WIP).            */
		enum class Category : unsigned int {
			CONSUMABLE, WEAPON, KEY, MISC
		};
		// Returns a integer value of a category (starts at 0, goes up).
		const unsigned int categoryToValue(const Category& c) {
			return static_cast<unsigned int>(c);
		}
		/* The Item class is the base type of all item objects. *
		* Derive this to create subtypes. Making objects of    *
		* this class won't work -- using a derived class is    *
		* required.                                            */
		class Item {
		private:
			// The display name of the item.
			const std::string name;
			// The description of the item.
			const std::string desc;
			// The ID of the item.
			const unsigned short id;
			// The buy price of the item.
			const unsigned short buy;
			// The sell price of the item.
			const unsigned short sell;
			// The category of the item (used for sorting and casting).
			const Category category;
		protected:
			// ctor(s)
			Item(const std::string& _name, const std::string& _desc,
				const unsigned short& _id, const unsigned short& _buy,
				const unsigned short& _sell, const Category& _category)
				: name(_name), desc(_desc), id(_id), buy(_buy),
				sell(_sell), category(_category) { /* empty ctor */
			}
		public:
			// copy ctor(s)
			Item(const Item& other) : name(other.name), desc(other.desc), id(other.id),
				buy(other.buy), sell(other.sell), category(other.category) { /* empty ctor */
			}
			// dtor(s)
			virtual ~Item() {
				// Override if needed
			}
			// Use function (WIP)
			virtual void use() = 0;
			// Getter functions
			const std::string& getName() const {
				return name;
			}
			const std::string& getDesc() const {
				return desc;
			}
			const unsigned short& getID() const {
				return id;
			}
			const unsigned short& getBuyPrice() const {
				return buy;
			}
			const unsigned short& getSellPrice() const {
				return sell;
			}
			const Category& getCategory() const {
				return category;
			}
		};
		/* The Weapon class derives from Item. Weapons are      *
		* unstackable (WIP), have damage values, and can be    *
		* equipped on the goblincter.                           */
		class Weapon : public Item {
		private:
			// The damage value of the weapon.
			const unsigned short damage;
			// The critical chance of the weapon.
			const unsigned short crit;
			// The Spell damage of the weapon.
			const unsigned short spelldam;
			// The accuracy value of the weapon.
			const unsigned short accuracy;
		public:
			// ctor(s)
			Weapon(const std::string& _name, const std::string& _desc,
				const unsigned short& _id, const unsigned short& _damage, const unsigned short& _crit, const unsigned short& _spelldam, const unsigned short& _accuracy,
				const unsigned short& _buy, const unsigned short& _sell)
				: Item(_name, _desc, _id, _buy, _sell, Category::WEAPON), damage(_damage), crit(_crit), spelldam(_spelldam), accuracy(_accuracy) { }
			// dtor(s)
			virtual ~Weapon() {
				// Override if needed
			}
			// Use function (WIP)
			virtual void use() override {
				// Unused
			}
			// Getter functions
			virtual const unsigned short& getDamage() const {
				return damage;
			}
			virtual const unsigned short& getAccuracy() const {
				return accuracy;
			}
			virtual const unsigned short& getSpellDamage() const {
				return spelldam;
			}
			virtual const unsigned short& getCrit() const {
				return crit;
			}
		};
		/* The Consumable class derives from Item. Consumables  *
		* are stackable, and affect the stats of the           *
		* goblinter or enemies.                                */
		class Consumable : public Item {
		private:
			// The health regain of the consumable.
			const unsigned short health;
			// The mana regain of the consumable.
			const unsigned short mana;
		public:
			// ctor(s)
			Consumable(const std::string& _name, const std::string& _desc,
				const unsigned short& _id, const unsigned short& _health,
				const unsigned short& _mana, const unsigned short& _buy,
				const unsigned short& _sell) : Item(_name, _desc, _id,
					_buy, _sell, Category::CONSUMABLE), health(_health), mana(_mana) { }
			// dtor(s)
			virtual ~Consumable() {
				// Override if needed
			}
			// Use function (WIP)
			virtual void use() override {
				// WIP
			}
			// Getter functions
			virtual const unsigned short& getHealth() const {
				return health;
			}
			virtual const unsigned short& getMana() const {
				return mana;
			}
		};
	}
	/* The Tables namespace has several item lookup tables,   *
	* making the generation and storage of item subtypes     *
	* easier.                                                */
	namespace Tables
	{
		// Better readability
		using namespace ItemSystem::Items;
		// Makes it easy to autogenerate item IDs
		unsigned short next_id = 0;
		class WeaponTable {
		private:
			// The table of weapons
			const std::map<const std::string, const Weapon> table{
				/*{
				"Weapon name goes here",
				Weapon(name, desc, next_id++, damage, crit, spell damage, buy, sell, accuracy,)
				}*/
				//{
				//"Test Weapon",
				//Weapon("Test Weapon", "For testing, idiot.", next_id++, 20, 10, 5)
				//}
				{
					"Stick",
					Weapon("Stick", "Useless, cannot gain any proficiency bonuses.\nSell it as soon as you can.", next_id++, 1, 0, 1, 100,  0, 0)
				},
				{
					"Modal Soul",
					Weapon("Modal Soul", "Wait!", next_id++, 9999, 100, 9999, 100, 9999, 9999)
				},
				{
					"Wooden Bow",
					Weapon("Wooden Bow", "A starter weapon for most hunters in training.", next_id++, 3, 5, 0, 50, 10, 7)
				},
				{
					"Reinforced Bow",
					Weapon("Reinforced Bow", "A more unique and composed wooden bow.", next_id++, 7, 10, 0, 60, 30, 20)
				},
				{
					"Iron Bow",
					Weapon("Iron Bow", "Forged in some unholy Audi'je's home, this bow is made from \nregurgitated iron.", next_id++, 15, 15, 0, 70, 60, 50)
				},
				{
					"Tactical Compound Bow",
					Weapon("Tactical Compound Bow", "A bow with a 16x optical scope, strap, and a better string.", next_id++, 35, 20, 0, 100, 100, 80)
				},
				{
					"Meteor Bow",
					Weapon("Meteor Bow", "Forged from a passing meteor, nicknamed MB-132. Incredibly Rare", next_id++, 50, 25, 0, 75, 250, 200)
				},
				{
					"Star Bow",
					Weapon("Star Bow", "This bow has an unkown origin, it's only main feature is the\ndistinct glow the arrows make when fired. The bow seems to possess a special \npower... The power of light.", next_id++, 100, 50, 0, 75, 1000, 750)
				},
				{
					"Copper Shortsword",
					Weapon("Copper Shortsword", "A flimsy blade, made from the cheapest material on the planet.", next_id++, 5, 1, 0, 90, 15, 10)
				},
				{
					"Iron Blade",
					Weapon("Iron Blade", "A less flimsy blade, made from less-cheap materials.", next_id++, 9, 2, 0, 90, 30, 20)
				},
				{
					"Steel Blade",
					Weapon("Steel Blade", "A blade made out of a semi-usable material. Not the best, or \nworst.", next_id++ , 20, 3, 0, 95, 75, 50)
				},
				{
					"Obsidian Longsword",
					Weapon("Obsidian Longsword", "A dangerous weapon, a major step-up from the previous blades.", next_id++, 50, 4, 0, 80, 150, 100)
				},
				{
					"Core Lightblade",
					Weapon("Core Lightblade", "The sword forged inside the planet's core. Has a obsidian blade \nwith magma flowing around it.", next_id++, 100, 4, 0, 95, 500, 400)
				},
				{
					"The Singularity Blade",
					Weapon("The Singularity Blade", "Cuts with astronomical force, created from a god of the past.\nThe blade is complicated and overdone in features, filling a part of the\nblade with a moral of rebirth.", next_id++, 200, 3, 0, 95, 1500, 1250)
				},
				{
					"Wooden Staff",
					Weapon("Wooden Staff", "A wooden stick with some message in an unknown language cut in on \nthe side.", next_id++, 1, 0, 15, 100, 5, 3)
				},
				{
					"Infused Staff",
					Weapon("Infused Staff", "Covered in markings and scratches, it seems to emanate power.", next_id++, 2, 1, 25, 100, 10, 5)
				},
				{
					"Cut Wand",
					Weapon("Cut Wand", "A small wand, it can be aimed to cast certain, more powerful \nspells.", next_id++, 2, 1, 55, 100, 50, 25)
				},
				{
					"Nuja Wand",
					Weapon("Nuja Wand", "A wand passed down through the ages. Holds a rhythmic power.", next_id++, 4, 1, 120, 100, 100, 75)
				},
				{
					"F.I.L.O.",
					Weapon("F.I.L.O.", "F.I.L.O is a staff made as a tribute to a human who died in an \naccident. Just you holding it brings back depressed memories.", next_id++, 5, 1, 200, 100, 150, 100)
				},
				{
					"Staff of Mythos",
					Weapon("Staff of Mythos", "This staff holds the power from the great eldritch monsters. \nIt was found by a human when he destroyed the eldritch beings on his planet. \nHolds the souls of all the eldritch monsters.", next_id++, 10, 1, 350, 100, 1000, 750)
				},
				{
					"Leather Gloves",
					Weapon("Leather Gloves", "Torn up, burnt, and dipped in acid. These leather gloves are \nwell-worn.", next_id++, 7, 0, 0, 99, 5, 3)
				},
				{
					"Red Rubber Gloves",
					Weapon("Red Rubber Gloves", "Somebody used these one time.", next_id++, 15, 0, 0, 99, 30, 20)
				},
				{
					"Brass Knuckles",
					Weapon("Brass Knuckles", "Packs a punch, and a pierce. The brass knuckles are light and \npowerful.", next_id++, 40, 0, 0, 99, 60, 50)
				},
				{
					"Power Glove",
					Weapon("Power Glove", "Confused with the techinical masterpiece often. The powerglove \nexplodes on impact.", next_id++, 75, 0, 0, 99, 150, 125)
				},
				{
					"Torched Wristband",
					Weapon("Torched Wristband", "Although not connected to the fists, the Torched Wristband \ninfuses the hand with strong power, also producing an explosion \nin the process.", next_id++, 150, 0, 0, 99, 750, 500)
				},
				{
					"Hell-Forged Wristband",
					Weapon("Hell-Forged Wristband", "Infuses the user with a large amount of strength so powerful, it \ncould destroy a building in one hit. The Hell-Forged wristband was \nused by a head executioner in hell to keep control of his troops.", next_id++, 300, 0, 0, 99, 1750, 1400)
				}
			};
		public:
			// Generates a reference to an item in the table.
			const Weapon& generate(const std::string& nameID) const {
				// Check if the entry exists
				if (table.count(nameID) == 0)
					throw std::invalid_argument("Attempted to generate a Weapon that does not exist");
				// Return the reference
				return table.at(nameID);
			}
		} const WeaponTable;
		class ConsumableTable {
		private:
			// The table of consumables
			const std::map<const std::string, const Consumable> table{
				/*{
				"Consumable name goes here",
				Consumable(name, desc, next_id++, health, mana, buy, sell)
				}*/
				{
					"Test Consumable",
					Consumable("Test Consumable", "For testing, idiot.", next_id++, 20, 8, 10, 5)
				},
				{
					"Normal Health Potion",
					Consumable("Normal Health Potion", "Heals 25 HP ", next_id++, 25, 0, 15, 3)
				},
				{
					"Greater Health Potion",
					Consumable("Greater Health Potion", "Heals 100 HP", next_id++, 100, 0, 45, 20)
				},
				{
					"Super Health Potion",
					Consumable("Super Health Potion", "Heals 500 HP", next_id++, 500, 0, 100, 50)
				},
				{
					"Full Health Potion",
					Consumable("Full Health Potion", "Heals 9999 HP", next_id++, 9999, 0, 300, 150)
				},
				{
					"Normal Mana Potion",
					Consumable("Normal Mana Potion", "Restores 15 MP", next_id++, 0, 15, 10, 5)
				},
				{
					"Greater Mana Potion",
					Consumable("Greater Mana Potion", "Restores 45 MP", next_id++, 0, 45, 45, 20)
				},
				{
					"Super Mana Potion",
					Consumable("Super Mana Potion", "Restores 100 MP", next_id++, 0, 100, 100, 50)
				},
				{
					"Full Mana Potion",
					Consumable("Full Mana Potion", "Restores 9999 MP", next_id++, 0, 9999, 300, 150)
				},
				{
					"Full Restore",
					Consumable("Full Restore", "Restores 9999 MP and 9999 HP", next_id++, 9999, 9999, 500, 250)

				}
			};
		public:
			// Generates a reference to an item in the table.
			const Consumable& generate(const std::string& nameID) const {
				// Check if the entry exists
				if (table.count(nameID) == 0)
					throw std::invalid_argument("Attempted to generate a Weapon that does not exist");
				// Return the reference
				return table.at(nameID);
			}
		} const ConsumableTable;
	}
	/* The Container namespace has the item slot class and    *
	* the inventory system.                                  */
	namespace Container
	{
		// Better readability
		using namespace ItemSystem::Items;
		/* The ItemSlot class encapsulates a item subtype and   *
		* allows it to be stored in the inventory, along with  *
		* keeping track of how many items are stacked.         */
		class ItemSlot {
		private:
			// Pointer to (dynamically allocated) item-based instance
			std::shared_ptr<const Item> item;
			// Stack amount
			unsigned short stack;
		public:
			// ctor(s)
			ItemSlot(const Item* _item = nullptr, const unsigned short& _stack = 1)
				: item(_item), stack(_stack) { }
			// Updates the item in the slot (destroying the old instance).
			void update(const Item* newitem) {
				item.reset(newitem);
			}
			// Getter and setter functions
			const std::shared_ptr<const Item>& getItem() const {
				return item;
			}
			const unsigned short& getStackAmount() const {
				return stack;
			}
			// Increments (adds 1 to) the stack amount.
			void incrementStackAmount() {
				stack++;
			}
			// Decrements (removes 1 from) the stack amount.
			void decrementStackAmount() {
				stack--;
			}
			// Operator overloading
			bool operator==(const ItemSlot& other) {
				if (item->getID() == other.getItem()->getID())
					return true;
				else return false;
			}
		};
		/* The Inventory holds a vector of item slots, and      *
		* handles the creation and deletion of those slots.    *
		* The inventory can be added to, deleted from, taken   *
		* from, read entry by entry, and (WIP) sorted and      *
		* filtered.                                            */
		class Inventory {
		private:
			// The data storage of the inventory.
			std::vector<ItemSlot> storage;
		public:
			// ctor(s)
			Inventory() {
				// Reserve space
				storage.reserve(20);
			}
			// Adds an item to the inventory.
			template<class ItemType>
			void addItem(const ItemType& item) {
				// Initialize an item slot
				ItemSlot slot(new ItemType(item));
				// Check through inventory to see if an entry already exists
				for (ItemSlot& entry : storage) {
					// If there's an entry match
					if (entry == slot) {
						// Add to the existing slot's stack
						entry.incrementStackAmount();
						// Temporary item slot is destroyed here; no memory leaks (hopefully)
						return;
					}
				}
				// No matches, add new entry
				storage.push_back(std::move(slot));
			}
			// Adds multiple items to the inventory.
			template<class ItemType>
			void addItems(const std::initializer_list<const ItemType>& items) {
				// Call addItem() for each item
				for (auto it = items.begin(); it != items.end(); ++it)
					addItem(*it);
			}
			// Inspects an item in the inventory.
			const Item* inspectItem(const unsigned int& index) {
				// Check OOB
				if (index >= storage.size()) return nullptr;
				// Return the item pointer
				return storage.at(index).getItem().get();
			}
			// Inspects a item slot from the inventory.
			const ItemSlot* inspectSlot(const unsigned int& index) {
				// Check OOB
				if (index >= storage.size()) return nullptr;
				// Return the slot reference
				return &(storage.at(index));
			}
			// Removes a single item from the inventory.
			void deleteItem(const unsigned int& index) {
				// Check OOB
				if (index >= storage.size()) return;
				// Remove from stack
				storage.at(index).decrementStackAmount();
				// Check if the slot is empty (and should be deleted)
				if (storage.at(index).getStackAmount() == 0)
					// Delete the slot
					storage.erase(storage.begin() + index);
			}
			// Removes an item slot from the inventory.
			void deleteSlot(const unsigned int& index) {
				// Check OOB
				if (index >= storage.size()) return;
				// Remove the item slot at the specified index
				storage.erase(storage.begin() + index);
			}
			// Takes an item from the inventory (deleting the item slot if stack is out).
			const std::shared_ptr<const Item> takeItem(const unsigned int& index) {
				// Check OOB
				if (index >= storage.size()) return nullptr;
				// Get item
				const std::shared_ptr<const Item> item = storage.at(index).getItem();
				// Remove from stack or delete the item slot
				if (storage.at(index).getStackAmount() > 1) {
					// Subtract from stack
					storage.at(index).decrementStackAmount();
				}
				// No stack remaining, remove the entry
				else {
					// Delete the entry
					deleteItem(index);
				}
				// Return the item
				return item;
			}
			// Gets the entire inventory vector (read-only).
			const std::vector<ItemSlot>& getAll() {
				return storage;
			}
		};
	}
}

using namespace ItemSystem::Items;
using namespace ItemSystem::Tables;
using namespace ItemSystem::Container;

int goblinheight[4] = {0, 2, 3, 4 };
int bossdef = 0;
string goblincolor[7] = {"Color of the Sun", "Yellow-Green", "Green", "Brown-Green", "Blue", "Red", "Jet Black" };
string goblinlname[50] = { ""," Pipes", " Cumoso", " Big Boy", " Star", " Ross", " Benes", " Richards", " Costanza", " Seinfeld", " Kilmister", " Drizzle", " Drozzle", " Dropling", " Griphook", " Gribble", " Glip", " Smork", " Goblin", ", The Dangerous", ", The Deadly", ", The Destroyer" , " The I" , " The II", " The III", " Jimbo", " Ultralord", " Edge", " Neutron", " Bob", " Cool", " Wizard", " Small", " Force", ", The Fast" , ", The Stealthy", " Operator", " Warlock", " Warrior", " Rogue", " Man", " Wick", " Reeves", " Machine", " Face", " Lord", " Worlds", " Smith", " The Glutton", " Stupid" };
string goblinfname[50] = { "", "Jerry", "Lemmy", "Dum", "Kramer", "George", "Morty", "Elaine", "Newman", "Michael", "Susan", "Keem", "Durbortanizaltibmel", "Snorty", "Rivers", "Cam", "Geoffrey", "King Gizzard and The", "Cthulhu", "Arachmigo", "Me,", "Orlock" , "Zorka" , "Orslon" , "Scloot" , "Bob" , "Bobblin" , "Gremlin", "Scalbino" , "Jose", "Hungry", "Sheen", "Jimmy", "Ghoul", "nock", "Mr." , "Ms.", "Mrs.", "Ness", "Rocket", "Scarface", "Jon", "Keanu", "Burgie", "Pirate", "Lord", "Isaac", "Goblin" , "Destroyer", "Meme" };
string goblinmname[20] = { "", " The", " Me,", " A", " Driddle", " Goblin", " and", " for", " with" , " but", " his", " her", " their", " nor", " xor", " Lizard", ", The Goblin", " or", " Mc", " of" };
string dad;
int seed = 0;
int maxHPMPadd = 0;
int DEF_FAST_DELAY = 0;
int k = 10;
int a;
int damage;
int difficulty = 1;
string difficultyn = "Normal";
int special;
bool battlemenu = false;

/*
struct Inventory { //8
    int nhpp = 3;
    int ghpp = 1;
    int bhpp = 0;
    int fullre = 0;
    int lunch = 0;
    int gold;
    int bottlerocket = 0;
    int mbottlerocket = 0;
} inventore;
*/

struct Enemy {
    int maxeHP;
    int maxeMP;
    int ehp;
    int emp;
    int estr;
    int espd;
    int edef;
    int elvl;
    int ecrt;
    int eheight = 3;
    int eage = 30;
    int eweight = 35;
    string ename;
} enemy;

struct Goblin { //13
    int co = 15;
    int ca = 10;
    int height = 3;
    int maxHP;
    int gold;
    int maxMP;
    int mp;
    int hp;
    int str;
    int spd;
    int def;
    int lvl = 1;
    int crt;
    int exp = 0;
    int age = 30;
    int weight = 35;
    string antiquity = "Wayward Vagabond";
    double expmultiplier = 1;
    string name = "Drizzle Dum";
    double potency = 1.00;
    // Inventory
	Inventory inventory;
	Weapon* equipped;
} goblin;

   // Filedata fd({difficultyn, goblin.name},
//    {seed, difficulty, bossdef, inventory.nhpp, inventory.ghpp, inventory.bhpp, inventory.fullre, inventory.lunch, inventory.gold, inventory.bottlerocket, inventory. mbottlerocket, goblin.height, goblin.maxHP, goblin.maxMP, goblin.mp, goblin.hp, goblin.str, goblin.spd, goblin.def, goblin.lvl, goblin.crt, goblin.exp, goblin.age, goblin.weight},
  //  {});

string goblinname[3] = { "First", " Second", " Last" };

void set_console_color(int fg)
{
	HANDLE handle = GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleTextAttribute(handle, fg);
}

void wait_enter(void) //Method of wait_enter, call it to create a Press Enter to continue screen.
{
    cout << "Press ENTER to continue...";
    cin.ignore();
}

void wait_enter_2(void) //Method of wait_enter, call it to create a Press Enter to continue screen.
{
    cin.ignore();
    cout << "Press ENTER to continue...";
    cin.ignore();
}

void sout(string message, int delay, bool endline) // The REAL Sout method
{
    int start_delay = delay;
    for (int i = 0; i <= message.size(); ++i)
    {
        if (GetAsyncKeyState(VK_CONTROL) && 0x8000)
            delay = DEF_FAST_DELAY;
        else
            delay = start_delay;
        cout << message[i];
        Sleep(delay);
    }
    if (endline) cout << endl;
    else return;
}

void swapColor()
{
	static bool accent = true;
	if (accent)
		set_console_color(goblin.co);
	else
		 set_console_color(goblin.ca);
	accent = !accent;
}


int waitForKey()
{
	return _getch() - 48;
}

void weaponinv() {
    static int input;
	system("cls");
	cout << "-*- Inventory -*-\nCurrent Weapon: " << goblin.equipped->getName() << "\nDescription: " << goblin.equipped->getDesc() << endl;
	cout << "\n-*- Weapon Stats -*-\nDamage: " << goblin.equipped->getDamage() << "\nCrit Bonus: " << goblin.equipped->getCrit() << "\nSpell Damage: " << goblin.equipped->getSpellDamage() << "\nAccuracy: " << goblin.equipped->getAccuracy() << "\nWeapon Cost: " << goblin.equipped->getBuyPrice() << "\n" << endl;
	int index = 1;
	cout << "0) Exit" << endl;
	std::vector<Weapon*> weapons;
	for (const auto& item : goblin.inventory.getAll()) {
		if (item.getItem()->getCategory() == Category::WEAPON) {
			auto toCast = item.getItem().get();
			auto weapon = const_cast<Weapon*>(dynamic_cast<const Weapon*>(toCast));
			weapons.push_back(weapon);
		}
	}
	for (const auto& wep : weapons)
	{
		cout << index << ") " << wep->getName() << std::endl;
		index++;
	}
	cin >> input;
	if (input == 0) {

	}
	else if (input != 0) {
		goblin.equipped = const_cast<Weapon*>(dynamic_cast<const Weapon*>(goblin.inventory.inspectItem(input - 1)));
		weaponinv();
	}
}

void consuminv() {
	unsigned int index = 0;
	cout << "0) Exit" << endl;
	// Filter inventory into vector.
	std::vector<std::pair<Consumable*, unsigned int>> filtered;
	for (const auto& item : goblin.inventory.getAll()) {
		if (item.getItem()->getCategory() == Category::CONSUMABLE) {
			auto consumable = const_cast<Consumable*>(dynamic_cast<const Consumable*>(item.getItem().get()));
			filtered.push_back(std::make_pair(consumable, index));
			std::cout << filtered.size() << ") " << consumable->getName() << " x" << item.getStackAmount() << std::endl;
		}
		index++;
	}
	// Get input;
	unsigned int userInput;
	std::cin >> userInput;
	if (userInput == 0) return;
	userInput--;
	// Take item and use it.
	unsigned int takenInput = filtered.at(userInput).second;
	auto takenItem = filtered.at(userInput).first;
	goblin.hp += takenItem->getHealth();
	goblin.mp += takenItem->getMana();
	// Print info.
	std::cout << "You restore " << takenItem->getHealth() << " HP and " << takenItem->getMana() << " MP!" << endl;
	goblin.inventory.deleteItem(takenInput);
	statcheck();
	wait_enter();
}

void options() {
	system("cls");
	int colorc;
	int colorce;
	cout << "-*- Options -*-" << endl;
	swapColor();
	cout << "\n1";
	swapColor();
	cout << ") Change Console Color\n" << endl;
	swapColor();
	cout << "2";
	swapColor();
	cout << ") Change Accent Color" << endl;
	colorc = waitForKey();
		switch (colorc) {
		case 1:
			system("cls");
			cout << "-*- Console Color -*- \n\nWhat color do you want your console?\n\n";
			swapColor();
			cout << "1";
			swapColor();
			cout << ") White\n\n";
			swapColor();
			cout << "2";
			swapColor();
			cout << ") Yellow\n\n";
			swapColor();
			cout << "3";
			swapColor();
			cout << ") Green\n\n";
			swapColor();
			cout << "4";
			swapColor();
			cout << ") Red\n\n";
			swapColor();
			cout << "5";
			swapColor();
			cout << ") Magenta\n\n";
			swapColor();
			cout << "6";
			swapColor();
			cout << ") Cyan" << endl;
			colorc = waitForKey();
			switch (colorc) {
			case 1:
				goblin.co = 15;
				break;
			case 2:
				goblin.co = 14;
				break;
			case 3:
				goblin.co = 10;
				break;
			case 4:
				goblin.co = 12;
				break;
            case 5:
                goblin.co = 13;
                break;
            case 6:
                goblin.co = 11;
                break;
			}
			swapColor();
			swapColor();
        break;
		case 2:
			SetConsoleTextAttribute(hConsole, goblin.co);
			system("cls");
			cout << "-*- Accent Color -*-\n\nWhat is your choice/marker color?\n\n";
            swapColor();
			cout << "1";
			swapColor();
			cout << ") White\n\n";
			swapColor();
			cout << "2";
			swapColor();
			cout << ") Yellow\n\n";
			swapColor();
			cout << "3";
			swapColor();
			cout << ") Green\n\n";
			swapColor();
			cout << "4";
			swapColor();
			cout << ") Red\n\n";
			swapColor();
			cout << "5";
			swapColor();
			cout << ") Magenta\n\n";
			swapColor();
			cout << "6";
			swapColor();
			cout << ") Cyan" << endl;
			colorc = waitForKey();
			switch (colorc) {
			case 1:
				goblin.ca = 15;
				break;
			case 2:
				goblin.ca = 14;
				break;
			case 3:
				goblin.ca = 10;
				break;
			case 4:
				goblin.ca = 12;
				break;
            case 5:
                goblin.ca = 13;
                break;
            case 6:
                goblin.ca = 11;
                break;
			}
			break;
		}

}

void inventory() {
    static int input;
	system("cls");
	cout << "-*- Stats -*-\n\nName: " << goblin.name << "\nAntiquity: " << goblin.antiquity << "\n\nMax Health: " << goblin.maxHP << "\nMax Mana: " << goblin.maxMP << "\n\nCurrent HP: " << goblin.hp << "\nCurrent MP: " << goblin.mp << "\n\nStrength: " << goblin.str << "\nDefense: " << goblin.def << "\nDexterity: " << goblin.spd << "\nCritical Chance: " << goblin.crt << "\n\nDifficulty: " << difficultyn << "\nDifficulty Multiplier: " << difficulty << "x" << endl;
	cout << "\nEXP To Next Level: " << ((goblin.lvl * 50 * difficulty) * goblin.expmultiplier) - goblin.exp << endl;
	cout << "\n1) Exit\n2) Weapons\n3) Consumables" << endl;
	input = waitForKey();
	switch (input) {
	case 1:
		break;
	case 2:
		weaponinv();
		break;
	case 3:
		consuminv();
		break;

	}
}

int main()
{
    srand(time(0));
    SetConsoleTitle("DrizzleDum");
    set_console_color(COLOR_WHITE);
    start();

}



void encounter() {
    estatgen();
    cout << "You take a nice stroll in the forest. \nSuddenly, " << enemy.ename << " jumps out from the bushes!\n(1) Fight\n(2) Not Like This\n(3) Walk Past this Fool (Fight a Boss)" << endl;
    cin >> a;
    switch (a) {
    case 1:
        battle();
        break;
    case 2:
        city();

        break;
    case 3:
        system("CLS");
        cout << "-*- Boss Menu -*-\n(1) Hungry Troll  (LVL 5)\n(2) Big Boy Troll (LVL 15)\n(3) Bad Boy Troll (LVL 25)\n(4) D'wahmoV     (LVL 40)\n(5) Col - Orc God (LVL 50)" << endl;
        cin >> special;
        switch (special) {
        case 1:
            if (goblin.lvl < 5) {
                cout << "You do not have the required level!" << endl;
                wait_enter();
            }
            else if (goblin.lvl >= 5) {
                if (bossdef >= 2) {
                    cout << "You cannot fight a boss you already fought!" << endl;
                    wait_enter();
                } else if(bossdef <= 1) {
                system("CLS");
                cout << "You look around for some food, suddenly, a troll jumps out.\n\n'Oh... Do you have food?' He asks.\n\nYou shake your head.\n\n'WELL THEN I'LL JUST EAT YOU!' He screams.\nHe charges at you with a club." << endl;
                bossdef = 1;
                enemy.estr = 30 * difficulty;
                enemy.edef = 1;
                enemy.ecrt = 10 * difficulty;
                enemy.espd = 1;
                enemy.eage = 25;
                enemy.eheight = 3;
                enemy.eweight = 35;
                enemy.elvl = 5;
                enemy.maxeHP = 350;
                enemy.maxeMP = 5;
                enemy.emp = enemy.maxeMP;
                enemy.ehp = enemy.maxeHP;
                enemy.ename = "Hungry Troll";
                wait_enter();
                battle();
            }
            }
            break;
        case 2:
            if (goblin.lvl < 15) {
                cout << "You do not have the required level!" << endl;
                wait_enter();
            }
            else if (goblin.lvl >= 15) {
                if (bossdef >= 4) {
                    cout << "You cannot fight a boss you already fought!" << endl;
                    wait_enter();
                }
                if (bossdef <= 3) {
                system("CLS");
                cout << "While eating a small lunch outside a town, A BIG BOY runs up to you.\n\n'Give Lunch or I tell mom.' He says to you\n\nYou shake your head.\n\n'MEANIE! GIVE IT HERE!' He SCREAMS INTO YOUR EAR\nHis hands are priming with readiness to destroy you." << endl;
                bossdef = 3;
                enemy.estr = 50 * difficulty;
                enemy.edef = 10 * difficulty;
                enemy.ecrt = 10 * difficulty;
                enemy.espd = 10;
                enemy.eage = 15;
                enemy.eheight = 4;
                enemy.eweight = 50;
                enemy.elvl = 15;
                enemy.maxeHP = 650;
                enemy.maxeMP = 30;
                enemy.emp = enemy.maxeMP;
                enemy.ehp = enemy.maxeHP;
                enemy.ename = "Big Boy Troll";
                wait_enter();
                battle();
            }
            }
            break;
        case 3:
            if (goblin.lvl < 25) {
                cout << "You do not have the required level!" << endl;
                wait_enter();
            }
            else if (goblin.lvl >= 25) {
                if (bossdef >= 6) {
                    cout << "You cannot fight a boss you already fought!" << endl;
                    wait_enter();
                } else if(bossdef <= 5) {
                system("CLS");
                cout << "A very quite incredibly not nice boy (troll) slowly and rapidly encroaches upon your current position\n\n He screams, 'I want to eat lunch now please.'\n\nDespite your attempts to discourage him, he begins to maul you\nas you shake your head." << endl;
                bossdef = 5;
                enemy.estr = 75 * difficulty;
                enemy.edef = 50 * difficulty;
                enemy.ecrt = 0;
                enemy.espd = 25 * difficulty;
                enemy.eage = 45;
                enemy.eheight = 6;
                enemy.eweight = 360;
                enemy.elvl = 25;
                enemy.maxeHP = 1000;
                enemy.maxeMP = 100;
                enemy.emp = enemy.maxeMP;
                enemy.ehp = enemy.maxeHP;
                enemy.ename = "Bad Boy Troll";
                wait_enter();
                battle();
            }
            }
            break;
        case 4:
            if (goblin.lvl < 40) {
                cout << "You do not have the required level!" << endl;
                wait_enter();
            }
            else if (goblin.lvl >= 40) {
                if (bossdef >= 8) {
                    cout << "You cannot fight a boss you already fought!" << endl;
                    wait_enter();
                } else if(bossdef <= 7) {
                system("CLS");
                cout << "You walk near a volcano, and see an Orc King standing on top of it.\n\n'You're the one my men have told me about.' He says\nHis long dark hair flowing through the ash.\nYou shake your head.\n\nHe turns around, holding the largest broadsword ever forged.\n\nHe nods, signaling for you to fight." << endl;
                bossdef = 7;
                enemy.estr = 150 * difficulty;
                enemy.edef = 150 * difficulty;
                enemy.ecrt = 10 * difficulty;
                enemy.espd = 35 * difficulty;
                enemy.eage = 50;
                enemy.eheight = 6;
                enemy.eweight = 300;
                enemy.elvl = 40 * difficulty;
                enemy.maxeHP = 2500 * difficulty;
                enemy.maxeMP = 1000 * difficulty;
                enemy.emp = enemy.maxeMP;
                enemy.ehp = enemy.maxeHP;
                enemy.ename = "D'wahmoV, The Orc King. Destroyer of Trolls and Goblins";
                wait_enter();
                battle();
            }
            }
            break;
        case 5:
            if (goblin.lvl < 50) {
                cout << "You do not have the required level!" << endl;
                wait_enter();
            }
            else if (goblin.lvl >= 50) {
                if (bossdef >= 10) {
                    cout << "You cannot fight a boss you already fought!" << endl;
                    wait_enter();
                } else if(bossdef <= 9) {
                system("CLS");
                cout << "You look at the map the Orc King gave you.\nIt leads to the largest mountain on Uearth, you slowly make your way towards it.\n\nSuddenly, you take a step and a white light blinds your vision.\nYour vision clears and you look ahead, seeing a man floating.\n\n'You Killed D'wahmoV?'\n\nYou want to shake your head but cant.\n\n'Well... He has sacrificed most of this planet's goblins and trolls to me, and \nyou wont stop that.'\n\nYou stare at him, preparing to fight." << endl;
                bossdef = 9;
                enemy.estr = 999 * difficulty;
                enemy.edef = 999 * difficulty;
                enemy.ecrt = 2 * difficulty;
                enemy.espd = 50;
                enemy.eage = 99;
                enemy.eheight = 999;
                enemy.eweight = 999;
                enemy.elvl = 50 * difficulty;
                enemy.maxeHP = 2500 * difficulty;
                enemy.maxeMP = 9999;
                enemy.emp = enemy.maxeMP;
                enemy.ehp = enemy.maxeHP;
                enemy.ename = "Col, The Orc God. Destroyer of Eternity";
                wait_enter();
                battle();
            }
            }
            break;
        }
    }
}

void battle() {
    battlemenu = true;
    while (battlemenu == true) {
        if (goblin.hp <= 0) {
            cout << goblin.name << " is defeated... Your journey comes to an end." << endl;
            wait_enter();
            exit(EXIT_SUCCESS);
            main();
        }
        system("CLS");
        statcheck();
        cout << "-*- Battle Menu -*-" << "\n\n" << goblin.name << "\nHP: " << goblin.hp << "    MP: " << goblin.mp << "\n\n(1) Attack\n(2) Inventory\n(3) Cast\n(4) Examine\n\nEnemy name: " << enemy.ename << "\nEnemy HP: " << enemy.ehp << "\nEnemy MP: " << enemy.emp << endl;
        cin >> a;
        switch (a) {
        case 4:
            system("CLS");
            cout << "You look at the enemy, you see who they really are." << endl;
            cout << "Name: " << enemy.ename << "\nAge: " << enemy.eage << " years old\nWeight: " << enemy.eweight << " pounds\nHeight: " << enemy.eheight << " feet\n\nStrength: " << enemy.estr << "\nDefense: " << enemy.edef << "\nSpeed: " << enemy.espd << "\nCritical Chance: " << enemy.ecrt << "\nMax Health: " << enemy.maxeHP << "\nMax Mana: " << enemy.maxeMP << endl;
            wait_enter();
            break;
        case 1:
            system("CLS");
            cout << "-*- Combo Menu -*-\n\nKnown Combos:\n(1) Kick\n(2) Punch\n(3) " << goblin.name << "'s Signature Slam" << endl;
            cin >> special;
            switch (special) {
            case 1:
                damage = rand() % (goblin.spd - (goblin.age / 5) - enemy.edef / 3);
                randcheck();
                cout << "The enemy takes " << damage << " damage!" << endl;
                enemy.ehp = enemy.ehp - damage;
                damage = rand() % 100 - goblin.crt;
                if (damage <= 1) {
                    damage = rand() % ((goblin.spd - (goblin.age / 5) - enemy.edef / 3) * 2);
                    randcheck();
                    cout << "The enemy takes " << damage << " damage from a critical hit!" << endl;
                    enemy.ehp = enemy.ehp - damage;
                }
                enemyturn();
                break;
            case 2:
                damage = rand() % (goblin.str - (goblin.age / 5) - enemy.edef / 3);
                randcheck();
                cout << "The enemy takes " << damage << " damage!" << endl;
                enemy.ehp = enemy.ehp - damage;
                damage = rand() % 100 - goblin.crt;
                if (damage <= 1) {
                    damage = rand() % ((goblin.str - (goblin.age / 5) - enemy.edef / 3) * 2);
                    randcheck();
                    cout << "The enemy takes " << damage << " damage from a critical hit!" << endl;
                    enemy.ehp = enemy.ehp - damage;
                }
                enemyturn();
                break;
            case 3:
                damage = rand() % (goblin.age - (goblin.age / 2) - 25) - enemy.edef / 3;
                randcheck();
                cout << "The enemy takes " << damage << " damage!" << endl;
                enemy.ehp = enemy.ehp - damage;
                damage = rand() % 100 - goblin.crt;
                if (damage <= 1) {
                    damage = rand() % ((goblin.age - (goblin.age / 2) - 25) - enemy.edef / 3 * 2);
                    randcheck();
                    cout << "The enemy takes " << damage << " damage from a critical hit!" << endl;
                    enemy.ehp = enemy.ehp - damage;
                }
                enemyturn();
                break;
            }
            break;
        case 2:
            inventory();
            break;
        case 3:
            system("CLS");
            cout << "-*- Cast Menu -*-" << endl;
            cout << "(1) Cure (10 MP)\n(2) Fireball (10 MP)\n(3) Earthquake (25 MP)" << endl;
            cin >> a;
            switch (a) {
            case 1:
                if (goblin.mp < 10) {
                    cout << "You do not have enough mana!" << endl;
                    wait_enter();
                }
                else if (goblin.mp >= 10) {
                    damage = rand() % (10 + goblin.age) / difficulty;
                    randcheck();
                    cout << "You heal " << damage << " back!" << endl;
                    goblin.hp = goblin.hp + damage;
                    goblin.mp = goblin.mp - 10;
                    enemyturn();
                }
                break;
            case 2:
                if (goblin.mp < 10) {
                    cout << "You do not have enough mana!" << endl;
                    wait_enter();
                }
                else if (goblin.mp >= 10) {
                    damage = rand() % (goblin.age + 25) / difficulty;
                    randcheck();
                    cout << "You deal " << damage << " damage to the enemy!" << endl;
                    enemy.ehp = enemy.ehp - damage;
                    goblin.mp = goblin.mp - 10;
                    enemyturn();
                }
                break;
            case 3:
                if (goblin.mp < 25) {
                    cout << "You do not have enough mana!" << endl;
                    wait_enter();
                }
                else if (goblin.mp >= 25) {
                    damage = rand() % (goblin.age * 2 * goblin.lvl) / difficulty;
                    randcheck();
                    cout << "You deal " << damage << " damage to the enemy!" << endl;
                    enemy.ehp = enemy.ehp - damage;
                    goblin.mp = goblin.mp - 25;
                    enemyturn();
                }
                break;
            }
        }

    }
}

void clvl() {
    battlemenu = false;
    if (bossdef == 1) {
        cout << "The hungry troll falls to the ground... You find some items.\n1 Bottle Rocket\n2 Normal Health Potions\n2 Greater Health Potions\n+250 EXP" << endl;
//        inventory.bottlerocket = inventory.bottlerocket + 1;
//        inventory.nhpp = inventory.nhpp + 2;
     //   inventory.ghpp = inventory.ghpp + 2;
        goblin.exp = goblin.exp + 250;
        bossdef = 2;
        wait_enter();
        city();
    }
    if (bossdef == 3) {
        cout << "The big boy troll falls to the ground... You find some items.\n3 Bottle Rockets\n3 Greater Health Potions\n3 Lunches\n+500 EXP" << endl;
     //   inventory.bottlerocket = inventory.bottlerocket + 1;
      //  inventory.nhpp = inventory.nhpp + 2;
     //   inventory.ghpp = inventory.ghpp + 2;
        goblin.exp = goblin.exp + 500;
        bossdef = 4;
        wait_enter();
        city();
    }
    else if (bossdef == 5) {
        cout << "The bad boy troll falls to the ground... You find some items.\n3 Bottle Rockets\n1 Multi-Bottle Rocket\n5 Greater Health Potions\n5 Lunches\n1 Full Restore\n+1000 EXP" << endl;
     //   inventory.bottlerocket = inventory.bottlerocket + 3;
   //     inventory.mbottlerocket = inventory.mbottlerocket + 1;
   //    inventory.ghpp = inventory.ghpp + 5;
   //     inventory.lunch = inventory.lunch + 3;
     //   inventory.fullre = inventory.fullre + 3;
        goblin.exp = goblin.exp + 1000;
        bossdef = 6;
        wait_enter();
        city();
    }
    else if (bossdef == 7) {
        cout << "The orc king collapses, looking at you with bruises and scars.\n'You... you...' He reaches his hand out. He is holding a map.\n'You... are worthy for him...' He puts it in your hand and dies.\n\nHe was carrying nothing, only his sword and armor." << endl;
        goblin.exp = goblin.exp + 5000;
        bossdef = 8;
        wait_enter();
        city();
    }
    else if (bossdef == 9) {
        cout << "You defeat Col, the green residue in the sky seems to go away.\nUearth is back to normal, you see goblins, trolls, orcs, and other creatrues\nexit from caves and enter the sunlight.\n\n\n\n\nThe game is over, but if you want to continue you can! Congrats!" << endl;
        goblin.exp = goblin.exp + 50000;
        bossdef = 10;
        wait_enter();
        city();

    }
    if (bossdef == 0 || bossdef == 2 || bossdef == 4 || bossdef == 6 || bossdef == 8 || bossdef == 10) {
        damage = rand() % (enemy.eheight * 2 + enemy.estr + enemy.edef + enemy.elvl + enemy.maxeHP) * difficulty;
        cout << "You earned " << damage << " gold!" << endl;
//        inventory.gold = inventory.gold + damage;
        damage = rand() % (enemy.estr + enemy.maxeHP + enemy.elvl);
        cout << "You gain " << damage << " EXP!" << endl;
        goblin.exp = goblin.exp + damage;
        damage = rand() % 2 + 1;
        cout << "You find " << damage << " normal health potions!" << endl;
      //  inventory.nhpp = inventory.nhpp + damage;
    }
    while (goblin.exp > (int)(goblin.lvl * 50 * difficulty)) {
        goblin.lvl++;
        cout << "You leveled! Current level: " << goblin.lvl << endl;
        damage = rand() % 2 + 1;
        goblin.crt = goblin.crt + damage;
        damage = rand() % 2 + 1;
        goblin.str = goblin.str + damage;
        damage = rand() % 2 + 1;
        goblin.def = goblin.def + damage;
        damage = rand() % 2 + 1;
        goblin.spd = goblin.spd + damage;
    }
    wait_enter();
    city();
}


void enemyturn() {
    damage = rand() % 150 - goblin.spd;
    if (damage <= 1) {
        damage = rand() % goblin.spd - (goblin.age / 5);
        randcheck();
        cout << "You deal " << damage << " extra damage because of your speed!" << endl;
        enemy.ehp = enemy.ehp - damage;
        damage = rand() % 175 - goblin.spd;
        if (damage <= 1) {
            damage = rand() % goblin.spd * 2 - (goblin.age / 5);
            randcheck();
            cout << "You deal " << damage << " extra-extra damage because of your speed!" << endl;
            enemy.ehp = enemy.ehp - damage;
            damage = rand() % 200 - goblin.spd;
            if (damage <= 1) {
                damage = rand() % goblin.spd * 3 - (goblin.age / 5);
                randcheck();
                cout << "You deal " << damage << " extra-extra-EXTRA damage because of your speed!" << endl;
                enemy.ehp = enemy.ehp - damage;
            }
        }
    }
    cout << "\n" << endl;
    if (enemy.ehp <= 0) {
        cout << enemy.ename << " falls to the ground, defeated." << endl;
        clvl();
    }
    if (enemy.ehp < enemy.ehp / 3) {
        damage = rand() % 3 + 1;
        if (damage == 1) {
            damage = enemy.ehp / 10;
            cout << "The enemy heals " << damage << " health back from a potion!" << endl;
            enemy.ehp = enemy.ehp + enemy.ehp / 10;
        }
        else if (damage >= 2) {
            damage = rand() % (enemy.estr - (enemy.eage / 5) - goblin.def / 3);
            randcheck();
            cout << "The enemy attacks! Dealing " << damage << " damage!" << endl;
            goblin.hp = goblin.hp - damage;
            damage = rand() % 100 - enemy.ecrt;

            if (damage <= 1) {
                damage = rand() % (((enemy.eage - (enemy.eage / 2) - 25) - goblin.def / 3) * 2);
                randcheck();
                cout << "You take " << damage << " damage from a critical hit!" << endl;
                goblin.hp = goblin.hp - damage;

            }
            wait_enter();
        }
    }
    else if (enemy.ehp >= enemy.ehp / 3) {
        if (enemy.emp < 10) {
            damage = rand() % (enemy.estr - (enemy.eage / 5) - goblin.def / 3);
            randcheck();
            cout << "The enemy attacks! Dealing " << damage << " damage!" << endl;
            goblin.hp = goblin.hp - damage;
            damage = rand() % 100 - enemy.ecrt;
            if (damage <= 1) {
                damage = rand() % (((enemy.eage - (enemy.eage / 2) - 25) - goblin.def / 3) * 2);
                randcheck();
                cout << "You take " << damage << " damage from a critical hit!" << endl;
                goblin.hp = goblin.hp - damage;
            }
        }
        else if (enemy.emp >= 10) {
            damage = rand() % 3 + 1;
            if (damage == 1) {
                damage = rand() % (goblin.age + 25) / difficulty;
                randcheck();
                cout << "The enemy casts fireball... Dealing " << damage << " damage to you!" << endl;
                goblin.hp = goblin.hp - damage;
                enemy.emp = enemy.emp - 10;
            }
            else if (damage >= 2) {
                damage = rand() % (enemy.estr - (enemy.eage / 5) - goblin.def / 3);
                randcheck();
                cout << "The enemy attacks! Dealing " << damage << " damage!" << endl;
                goblin.hp = goblin.hp - damage;
                damage = rand() % 100 - enemy.ecrt;
                if (damage <= 1) {
                    damage = rand() % (((enemy.eage - (enemy.eage / 2) - 25) - goblin.def / 3) * 2);
                    randcheck();
                    cout << "You take " << damage << " damage from a critical hit!" << endl;
                    goblin.hp = goblin.hp - damage;
                }
            }
        }
    }
    damage = rand() % 150 - enemy.espd;
    if (damage <= 1) {
        damage = rand() % enemy.espd - (enemy.eage / 5);
        randcheck();
        cout << "The enemy deals " << damage << " extra damage because of their speed!" << endl;
        goblin.hp = goblin.hp - damage;
        damage = rand() % 175 - enemy.espd;
        if (damage <= 1) {
            damage = rand() % goblin.spd - (goblin.age / 5);
            randcheck();
            cout << "The enemy deals " << damage << " extra-extra damage because of their speed!" << endl;
            goblin.hp = goblin.hp - damage;
            damage = rand() % 200 - enemy.espd;
            if (damage <= 1) {
                damage = rand() % goblin.spd - (goblin.age / 5);
                randcheck();
                cout << "The enemy deals " << damage << " extra-extra-EXTRA damage because of their speed!" << endl;
                goblin.hp = goblin.hp - damage;
            }
        }
    }
    wait_enter();
    enemy.emp = enemy.emp + 1;
    goblin.mp = goblin.mp + 1;
}

void randcheck() {
    if (damage <= 0) {
        damage = 0;
    }
}

void start() {
    static bool startfin = false;
    swapColor();
    while(startfin == false) {
    system("CLS");
    sout("-*- Drizzle Dum! -*- ", 25, true);
    swapColor();
    cout << "\n1";
    swapColor();
    cout << ") Create your Goblin!\n\n";
    swapColor();
    cout << "2";
    swapColor();
    cout << ") Load a File\n\n";
    swapColor();
    cout << "3";
    swapColor();
    cout << ") Settings\n\n\nBuild Vers. 1.1 - Balancing Update\n\nMade by";
    swapColor();
    cout << " crowyo!" << endl;
    swapColor();
    a = waitForKey();
    switch (a) {
    case 1:
        startfin = true;
        goblinnaming();
        break;
    case 2:
        startfin = true;
//        load();
        break;
    case 3:
        options();
    }
    }
}

/*
void load() {
    system("CLS");
    string saveload;
    cout << "-*- Loading -*-\n\nWhat file would you like to load?\n\n(Input File name, no .txt!)\n" << endl;
    cin >> saveload;
    saveload = saveload + ".txt";
    Filedata rd;
    FileReader fr;
//    fr.open(saveload).read(rd);
    goblin.name = rd.m_strs.at(0);
    difficultyn = rd.m_strs.at(1);

    seed = rd.m_nums.at(0);
    difficulty = rd.m_nums.at(1);
    bossdef = rd.m_nums.at(2);

    inventory.nhpp = rd.m_nums.at(3);
    inventory.ghpp = rd.m_nums.at(4);
    inventory.bhpp = rd.m_nums.at(5);
    inventory.fullre = rd.m_nums.at(6);
    inventory.lunch = rd.m_nums.at(7);
    inventory.gold = rd.m_nums.at(8);
    inventory.bottlerocket = rd.m_nums.at(9);
    inventory.mbottlerocket = rd.m_nums.at(10);
    goblin.height = rd.m_nums.at(11);
    goblin.maxHP = rd.m_nums.at(12);
    goblin.maxMP = rd.m_nums.at(13);
    goblin.mp = rd.m_nums.at(14);
    goblin.hp = rd.m_nums.at(15);
    goblin.str = rd.m_nums.at(16);
    goblin.spd = rd.m_nums.at(17);
    goblin.def = rd.m_nums.at(18);
    goblin.lvl = rd.m_nums.at(19);
    goblin.crt = rd.m_nums.at(20);
    goblin.exp = rd.m_nums.at(21);
    goblin.age = rd.m_nums.at(22);
    goblin.weight = rd.m_nums.at(23);
city();
}

void save() {
    if(savedone == false) {
    cout << "-*- Saving! -*-\n\nWhat do you want your save named? \n(No spaces!)" << endl;
    cin >> savename;
    savename = savename + ".txt";
    savedone = true;
    save();
    }
    else if(savedone == true) {
    system("cls");
    cout << "Saving..." << endl;
    wait_enter();
    Filedata fd({goblin.name, difficultyn},
                {seed, difficulty, bossdef, inventory.nhpp, inventory.ghpp, inventory.bhpp, inventory.fullre, inventory.lunch, inventory.gold, inventory.bottlerocket, inventory.mbottlerocket, goblin.height, goblin.maxHP, goblin.maxMP, goblin.mp, goblin.hp, goblin.str, goblin.spd, goblin.def, goblin.lvl, goblin.crt, goblin.exp, goblin.age, goblin.weight},
                {});
    FileWriter fw;
//    if(fw.open(fd).write(savename)) {
  //  }
    }
}

*/
void blackmarket() {
    static int input;
	static int input2;
	bool shopc = true;
	while(shopc == true) {
	system("cls");
		cout << "You look at the shops available." << endl;
		cout << "\n1) Warrior's Supply \n2) Hunter's Edge\n3) The Magic's Gathering\n4) Rocket Wrestling\n5) Mike's Friendly Store\n6) Exit" << endl;
		cout << "\nGold Available: " << goblin.gold << endl;
		cin >> input;
		switch (input) {
        case 6:
            shopc = false;
            break;
		case 1:
			system("cls");
			cout << "-*- Warrior's Supply -*-" << endl;
			cout << "\nYou walk in and are greeted by two Bren'kibs. \nThey are polishing their spears and weapons for selling.\nThey show you around the store." << endl;
			cout << "\n1) Copper Shortsword - " << WeaponTable.generate("Copper Shortsword").getBuyPrice() << " Gold\n2) Iron Blade - " << WeaponTable.generate("Iron Blade").getBuyPrice() << " Gold" << endl;
			cout << "3) Steel Blade - " << WeaponTable.generate("Steel Blade").getBuyPrice() << " Gold\n4) Obsidian Longsword - " << WeaponTable.generate("Obsidian Longsword").getBuyPrice() << " Gold" << endl;
			cout << "5) Core Lightblade - " << WeaponTable.generate("Core Lightblade").getBuyPrice() << " Gold\n6) The Singularity Blade - " << WeaponTable.generate("The Singularity Blade").getBuyPrice() << " Gold" << endl;
			cout << "\n0) Exit" << endl;
			cin >> input2;
			switch (input2) {
			case 1:
				if (goblin.gold < WeaponTable.generate("Copper Shortsword").getBuyPrice()) {
					cout << "You do not have enough gold." << endl;
					wait_enter_2();
					break;
				}
				else if (goblin.gold >= WeaponTable.generate("Copper Shortsword").getBuyPrice()) {
					cout << "You buy the Copper Shortsword for " << WeaponTable.generate("Copper Shortsword").getBuyPrice() << " gold." << endl;
					goblin.gold = goblin.gold - WeaponTable.generate("Copper Shortsword").getBuyPrice();
					goblin.inventory.addItem(WeaponTable.generate("Copper Shortsword"));
					wait_enter_2();
					break;
				}
				break;
			case 2:
				if (goblin.gold < WeaponTable.generate("Iron Blade").getBuyPrice()) {
					cout << "You do not have enough gold." << endl;
					wait_enter_2();
					break;
				}
				else if (goblin.gold >= WeaponTable.generate("Iron Blade").getBuyPrice()) {
					cout << "You buy the Iron Blade for " << WeaponTable.generate("Iron Blade").getBuyPrice() << " gold." << endl;
					goblin.gold = goblin.gold - WeaponTable.generate("Iron Blade").getBuyPrice();
					goblin.inventory.addItem(WeaponTable.generate("Iron Blade"));
					wait_enter_2();
					break;
				}
				break;
			case 3:
				if (goblin.gold < WeaponTable.generate("Steel Blade").getBuyPrice()) {
					cout << "You do not have enough gold." << endl;
					wait_enter_2();
					break;
				}
				else if (goblin.gold >= WeaponTable.generate("Steel Blade").getBuyPrice()) {
					cout << "You buy the Steel Blade for " << WeaponTable.generate("Steel Blade").getBuyPrice() << " gold." << endl;
					goblin.gold = goblin.gold - WeaponTable.generate("Steel Blade").getBuyPrice();
					goblin.inventory.addItem(WeaponTable.generate("Steel Blade"));
					wait_enter_2();
					break;
				}
				break;
			case 4:
				if (goblin.gold < WeaponTable.generate("Obsidian Longsword").getBuyPrice()) {
					cout << "You do not have enough gold." << endl;
					wait_enter_2();
					break;
				}
				else if (goblin.gold >= WeaponTable.generate("Obsidian Longsword").getBuyPrice()) {
					cout << "You buy the Obsidian Longsword for " << WeaponTable.generate("Obsidian Longsword").getBuyPrice() << " gold." << endl;
					goblin.gold = goblin.gold - WeaponTable.generate("Obsidian Longsword").getBuyPrice();
					goblin.inventory.addItem(WeaponTable.generate("Obsidian Longsword"));
					wait_enter_2();
					break;
				}
				break;
			case 5:
				if (goblin.gold < WeaponTable.generate("Core Lightblade").getBuyPrice()) {
					cout << "You do not have enough gold." << endl;
					wait_enter_2();
					break;
				}
				else if (goblin.gold >= WeaponTable.generate("Core Lightblade").getBuyPrice()) {
					cout << "You buy the Core Lightblade for " << WeaponTable.generate("Core Lightblade").getBuyPrice() << " gold." << endl;
					goblin.gold = goblin.gold - WeaponTable.generate("Core Lightblade").getBuyPrice();
					goblin.inventory.addItem(WeaponTable.generate("Core Lightblade"));
					wait_enter_2();
					break;
				}
				break;
			case 6:
				if (goblin.gold < WeaponTable.generate("The Singularity Blade").getBuyPrice()) {
					cout << "You do not have enough gold." << endl;
					wait_enter_2();
					break;
				}
				else if (goblin.gold >= WeaponTable.generate("The Singularity Blade").getBuyPrice()) {
					cout << "You buy the The Singularity Blade for " << WeaponTable.generate("The Singularity Blade").getBuyPrice() << " gold." << endl;
					goblin.gold = goblin.gold - WeaponTable.generate("The Singularity Blade").getBuyPrice();
					goblin.inventory.addItem(WeaponTable.generate("The Singularity Blade"));
					wait_enter_2();
					break;
				}

				break;
			}
			break;
		case 2:
			system("cls");
			cout << "-*- Hunter's Edge -*-" << endl;
			cout << "\nAs you enter the shop you see a Kenku sitting behind the counter.\nHe throws a dagger at you, barely missing you.\nHe laughs and lets you look at his wares.\n\n1) Wooden Bow - " << WeaponTable.generate("Wooden Bow").getBuyPrice() << " Gold\n2) Reinforced Bow - " << WeaponTable.generate("Reinforced Bow").getBuyPrice() << " Gold\n3) Iron Bow - " << WeaponTable.generate("Iron Bow").getBuyPrice() << " Gold\n4) Tactical Compound Bow - " << WeaponTable.generate("Tactical Compound Bow").getBuyPrice() << " Gold" << endl;
			cout << "5) Meteor Bow - " << WeaponTable.generate("Meteor Bow").getBuyPrice() << " Gold\n6) Star Bow - " << WeaponTable.generate("Star Bow").getBuyPrice() << " Gold" << endl;
			cout << "\n0) Exit" << endl;
			cin >> input2;
			switch (input2) {
			case 1:
				if (goblin.gold < WeaponTable.generate("Wooden Bow").getBuyPrice()) {
					cout << "You do not have enough gold." << endl;
					wait_enter_2();
					break;
				}
				else if (goblin.gold >= WeaponTable.generate("Wooden Bow").getBuyPrice()) {
					cout << "You buy the wooden bow for " << WeaponTable.generate("Wooden Bow").getBuyPrice() << " gold." << endl;
					goblin.gold = goblin.gold - WeaponTable.generate("Wooden Bow").getBuyPrice();
					goblin.inventory.addItem(WeaponTable.generate("Wooden Bow"));
					wait_enter_2();
					break;
				}
				break;
			case 2:
				if (goblin.gold < WeaponTable.generate("Reinforced Bow").getBuyPrice()) {
					cout << "You do not have enough gold." << endl;
					wait_enter_2();
					break;
				}
				else if (goblin.gold >= WeaponTable.generate("Reinforced Bow").getBuyPrice()) {
					cout << "You buy the Reinforced Bow for " << WeaponTable.generate("Reinforced Bow").getBuyPrice() << " gold." << endl;
					goblin.gold = goblin.gold - WeaponTable.generate("Reinforced Bow").getBuyPrice();
					goblin.inventory.addItem(WeaponTable.generate("Reinforced Bow"));
					wait_enter_2();
					break;
				}
				break;
			case 3:
				if (goblin.gold < WeaponTable.generate("Iron Bow").getBuyPrice()) {
					cout << "You do not have enough gold." << endl;
					wait_enter_2();
					break;
				}
				else if (goblin.gold >= WeaponTable.generate("Iron Bow").getBuyPrice()) {
					cout << "You buy the Iron Bow for " << WeaponTable.generate("Iron Bow").getBuyPrice() << " gold." << endl;
					goblin.gold = goblin.gold - WeaponTable.generate("Iron Bow").getBuyPrice();
					goblin.inventory.addItem(WeaponTable.generate("Iron Bow"));
					wait_enter_2();
					break;
				}
				break;
			case 4:
				if (goblin.gold < WeaponTable.generate("Tactical Compound Bow").getBuyPrice()) {
					cout << "You do not have enough gold." << endl;
					wait_enter_2();
					break;
				}
				else if (goblin.gold >= WeaponTable.generate("Tactical Compound Bow").getBuyPrice()) {
					cout << "You buy the Tactical Compound Bow for " << WeaponTable.generate("Tactical Compound Bow").getBuyPrice() << " gold." << endl;
					goblin.gold = goblin.gold - WeaponTable.generate("Tactical Compound Bow").getBuyPrice();
					goblin.inventory.addItem(WeaponTable.generate("Tactical Compound Bow"));
					wait_enter_2();
					break;
				}
				break;
			case 5:
				if (goblin.gold < WeaponTable.generate("Meteor Bow").getBuyPrice()) {
					cout << "You do not have enough gold." << endl;
					wait_enter_2();
					break;
				}
				else if (goblin.gold >= WeaponTable.generate("Meteor Bow").getBuyPrice()) {
					cout << "You buy the Meteor Bow for " << WeaponTable.generate("Meteor Bow").getBuyPrice() << " gold." << endl;
					goblin.gold = goblin.gold - WeaponTable.generate("Meteor Bow").getBuyPrice();
					goblin.inventory.addItem(WeaponTable.generate("Meteor Bow"));
					wait_enter_2();
					break;
				}
				break;
			case 6:
				if (goblin.gold < WeaponTable.generate("Star Bow").getBuyPrice()) {
					cout << "You do not have enough gold." << endl;
					wait_enter_2();
					break;
				}
				else if (goblin.gold >= WeaponTable.generate("Star Bow").getBuyPrice()) {
					cout << "You buy the Star Bow for " << WeaponTable.generate("Star Bow").getBuyPrice() << " gold." << endl;
					goblin.gold = goblin.gold - WeaponTable.generate("Star Bow").getBuyPrice();
					goblin.inventory.addItem(WeaponTable.generate("Star Bow"));
					wait_enter_2();
					break;
				}
				break;
			}
			break;
		case 3:
			system("cls");
			cout << "-*- The Magic's Gathering -*-" << endl;
			cout << "\nYou walk into the Magic's Gathering. A very happy human greets you\n'W-we-we have D&D Sessions on thursdays...' He says\nHe hands you a flyer\n'OH, and I-I also sell things...' He says." << endl;
			cout << "\n1) Wooden Staff - " << WeaponTable.generate("Wooden Staff").getBuyPrice() << " Gold\n2) Infused Staff - " << WeaponTable.generate("Infused Staff").getBuyPrice() << " Gold\n3) Cut Wand - " << WeaponTable.generate("Cut Wand").getBuyPrice() << " Gold\n4) Nuja Wand - " << WeaponTable.generate("Nuja Wand").getBuyPrice() << " Gold" << endl;
			cout << "5) F.I.L.O. - " << WeaponTable.generate("F.I.L.O.").getBuyPrice() << " Gold\n6) Staff of Mythos - " << WeaponTable.generate("Staff of Mythos").getBuyPrice() << " Gold" << endl;
			cout << "\n0) Exit" << endl;
			cin >> input2;
			switch (input2) {
			case 1:
				if (goblin.gold < WeaponTable.generate("Wooden Staff").getBuyPrice()) {
					cout << "You do not have enough gold." << endl;
					wait_enter_2();
					break;
				}
				else if (goblin.gold >= WeaponTable.generate("Wooden Staff").getBuyPrice()) {
					cout << "You buy the Wooden Staff for " << WeaponTable.generate("Wooden Staff").getBuyPrice() << " gold." << endl;
					goblin.gold = goblin.gold - WeaponTable.generate("Wooden Staff").getBuyPrice();
					goblin.inventory.addItem(WeaponTable.generate("Wooden Staff"));
					wait_enter_2();
					break;
				}
				break;
			case 2:
				if (goblin.gold < WeaponTable.generate("Infused Staff").getBuyPrice()) {
					cout << "You do not have enough gold." << endl;
					wait_enter_2();
					break;
				}
				else if (goblin.gold >= WeaponTable.generate("Infused Staff").getBuyPrice()) {
					cout << "You buy the Infused Staff for " << WeaponTable.generate("Infused Staff").getBuyPrice() << " gold." << endl;
					goblin.gold = goblin.gold - WeaponTable.generate("Infused Staff").getBuyPrice();
					goblin.inventory.addItem(WeaponTable.generate("Infused Staff"));
					wait_enter_2();
					break;
				}
				break;
			case 3:
				if (goblin.gold < WeaponTable.generate("Cut Wand").getBuyPrice()) {
					cout << "You do not have enough gold." << endl;
					wait_enter_2();
					break;
				}
				else if (goblin.gold >= WeaponTable.generate("Cut Wand").getBuyPrice()) {
					cout << "You buy the Cut Wand for " << WeaponTable.generate("Cut Wand").getBuyPrice() << " gold." << endl;
					goblin.gold = goblin.gold - WeaponTable.generate("Cut Wand").getBuyPrice();
					goblin.inventory.addItem(WeaponTable.generate("Cut Wand"));
					wait_enter_2();
					break;
				}
				break;
			case 4:
				if (goblin.gold < WeaponTable.generate("Nuja Wand").getBuyPrice()) {
					cout << "You do not have enough gold." << endl;
					wait_enter_2();
					break;
				}
				else if (goblin.gold >= WeaponTable.generate("Nuja Wand").getBuyPrice()) {
					cout << "You buy the Nuja Wand for " << WeaponTable.generate("Nuja Wand").getBuyPrice() << " gold." << endl;
					goblin.gold = goblin.gold - WeaponTable.generate("Nuja Wand").getBuyPrice();
					goblin.inventory.addItem(WeaponTable.generate("Nuja Wand"));
					wait_enter_2();
					break;
				}
				break;
			case 5:
				if (goblin.gold < WeaponTable.generate("F.I.L.O.").getBuyPrice()) {
					cout << "You do not have enough gold." << endl;
					wait_enter_2();
					break;
				}
				else if (goblin.gold >= WeaponTable.generate("F.I.L.O.").getBuyPrice()) {
					cout << "You buy F.I.L.O. for " << WeaponTable.generate("F.I.L.O.").getBuyPrice() << " gold." << endl;
					goblin.gold = goblin.gold - WeaponTable.generate("F.I.L.O.").getBuyPrice();
					goblin.inventory.addItem(WeaponTable.generate("F.I.L.O."));
					wait_enter_2();
					break;
				}
				break;
			case 6:
				if (goblin.gold < WeaponTable.generate("Staff of Mythos").getBuyPrice()) {
					cout << "You do not have enough gold." << endl;
					wait_enter_2();
					break;
				}
				else if (goblin.gold >= WeaponTable.generate("Staff of Mythos").getBuyPrice()) {
					cout << "You buy the Staff of Mythos for " << WeaponTable.generate("Staff of Mythos").getBuyPrice() << " gold." << endl;
					goblin.gold = goblin.gold - WeaponTable.generate("Staff of Mythos").getBuyPrice();
					goblin.inventory.addItem(WeaponTable.generate("Staff of Mythos"));
					wait_enter_2();
					break;
				}
				break;
			}
			break;
		case 4:
			system("cls");
			cout << "-*- Rocket Wrestling -*-" << endl;
			cout << "\nAs you walk in, a massive explosion rings out and two skeleton samurais\njump down from nowhere. They greet you with power gloves primed.\nThey lift up their weapons and smile. They show you around." << endl;
			cout << "\n1) Leather Gloves - " << WeaponTable.generate("Leather Gloves").getBuyPrice() << " Gold\n2) Red Rubber Gloves - " << WeaponTable.generate("Red Rubber Gloves").getBuyPrice() << " Gold\n3) Brass Knuckles - " << WeaponTable.generate("Brass Knuckles").getBuyPrice() << " Gold\n4) Power Glove - " << WeaponTable.generate("Power Glove").getBuyPrice() << " Gold" << endl;
			cout << "5) Torched Wristband - " << WeaponTable.generate("Torched Wristband").getBuyPrice() << " Gold\n6) Hell-Forged Wristband - " << WeaponTable.generate("Hell-Forged Wristband").getBuyPrice() << " Gold" << endl;
			cout << "\n0) Exit" << endl;
			cin >> input2;
			switch (input2) {
			case 1:
				if (goblin.gold < WeaponTable.generate("Leather Gloves").getBuyPrice()) {
					cout << "You do not have enough gold." << endl;
					wait_enter_2();
					break;
				}
				else if (goblin.gold >= WeaponTable.generate("Leather Gloves").getBuyPrice()) {
					cout << "You buy the Leather Gloves for " << WeaponTable.generate("Leather Gloves").getBuyPrice() << " gold." << endl;
					goblin.gold = goblin.gold - WeaponTable.generate("Leather Gloves").getBuyPrice();
					goblin.inventory.addItem(WeaponTable.generate("Leather Gloves"));
					wait_enter_2();
					break;
				}
				break;
			case 2:
				if (goblin.gold < WeaponTable.generate("Red Rubber Gloves").getBuyPrice()) {
					cout << "You do not have enough gold." << endl;
					wait_enter_2();
					break;
				}
				else if (goblin.gold >= WeaponTable.generate("Red Rubber Gloves").getBuyPrice()) {
					cout << "You buy the Red Rubber Gloves for " << WeaponTable.generate("Red Rubber Gloves").getBuyPrice() << " gold." << endl;
					goblin.gold = goblin.gold - WeaponTable.generate("Red Rubber Gloves").getBuyPrice();
					goblin.inventory.addItem(WeaponTable.generate("Red Rubber Gloves"));
					wait_enter_2();
					break;
				}
				break;
			case 3:
				if (goblin.gold < WeaponTable.generate("Brass Knuckles").getBuyPrice()) {
					cout << "You do not have enough gold." << endl;
					wait_enter_2();
					break;
				}
				else if (goblin.gold >= WeaponTable.generate("Brass Knuckles").getBuyPrice()) {
					cout << "You buy the Brass Knuckles for " << WeaponTable.generate("Brass Knuckles").getBuyPrice() << " gold." << endl;
					goblin.gold = goblin.gold - WeaponTable.generate("Brass Knuckles").getBuyPrice();
					goblin.inventory.addItem(WeaponTable.generate("Brass Knuckles"));
					wait_enter_2();
					break;
				}
				break;
			case 4:
				if (goblin.gold < WeaponTable.generate("Power Glove").getBuyPrice()) {
					cout << "You do not have enough gold." << endl;
					wait_enter_2();
					break;
				}
				else if (goblin.gold >= WeaponTable.generate("Power Glove").getBuyPrice()) {
					cout << "You buy the Power Glove for " << WeaponTable.generate("Power Glove").getBuyPrice() << " gold." << endl;
					goblin.gold = goblin.gold - WeaponTable.generate("Power Glove").getBuyPrice();
					goblin.inventory.addItem(WeaponTable.generate("Power Glove"));
					wait_enter_2();
					break;
				}
				break;
			case 5:
				if (goblin.gold < WeaponTable.generate("Torched Wristband").getBuyPrice()) {
					cout << "You do not have enough gold." << endl;
					wait_enter_2();
					break;
				}
				else if (goblin.gold >= WeaponTable.generate("Torched Wristband").getBuyPrice()) {
					cout << "You buy the Torched Wristband for " << WeaponTable.generate("Torched Wristband").getBuyPrice() << " gold." << endl;
					goblin.gold = goblin.gold - WeaponTable.generate("Torched Wristband").getBuyPrice();
					goblin.inventory.addItem(WeaponTable.generate("Torched Wristband"));
					wait_enter_2();
					break;
				}
				break;
			case 6:
				if (goblin.gold < WeaponTable.generate("Hell-Forged Wristband").getBuyPrice()) {
					cout << "You do not have enough gold." << endl;
					wait_enter_2();
					break;
				}
				else if (goblin.gold >= WeaponTable.generate("Hell-Forged Wristband").getBuyPrice()) {
					cout << "You buy the Hell-Forged Wristband for " << WeaponTable.generate("Hell-Forged Wristband").getBuyPrice() << " gold." << endl;
					goblin.gold = goblin.gold - WeaponTable.generate("Hell-Forged Wristband").getBuyPrice();
					goblin.inventory.addItem(WeaponTable.generate("Hell-Forged Wristband"));
					wait_enter_2();
					break;
				}
				break;
			}
			break;
		case 5:
			system("cls");
			cout << "-*- Mike's Friendly Store -*-" << endl;
			cout << "A very disgusting looking teen at the counter greets you as you walk in\n'Welcome to the store. SIR.' He says, with a very punchable face." << endl;
			cout << "\n1) Normal Health Potion - " << ConsumableTable.generate("Normal Health Potion").getBuyPrice() << " Gold\n2) Greater Health Potion - " << ConsumableTable.generate("Greater Health Potion").getBuyPrice() << " Gold\n3) Super Health Potion - " << ConsumableTable.generate("Super Health Potion").getBuyPrice() << " Gold" << endl;
			cout << "4) Full Health Potion - " << ConsumableTable.generate("Full Health Potion").getBuyPrice() << " Gold\n\n5) Normal Mana Potion - " << ConsumableTable.generate("Normal Mana Potion").getBuyPrice() << " Gold\n6) Greater Mana Potion - " << ConsumableTable.generate("Greater Mana Potion").getBuyPrice() << " Gold" << endl;
			cout << "7) Super Mana Potion - " << ConsumableTable.generate("Super Mana Potion").getBuyPrice() << " Gold\n8) Full Mana Potion - " << ConsumableTable.generate("Full Mana Potion").getBuyPrice() << " Gold" << endl;
			cin >> input2;
			switch (input2) {
			case 1:
				cout << "How many would you like to buy? (" << ConsumableTable.generate("Normal Health Potion").getBuyPrice() << " Each)" << endl;
				cin >> input2;
				if (input2 * ConsumableTable.generate("Normal Health Potion").getBuyPrice() > goblin.gold) {
					cout << "You do not have enough gold!" << endl;
					wait_enter_2();
					city();
				}
				else if (input2 * ConsumableTable.generate("Normal Health Potion").getBuyPrice() <= goblin.gold) {
					cout << "You bought " << input2 << " Normal Health Potions for " << input2 * ConsumableTable.generate("Normal Health Potion").getBuyPrice() << " gold" << endl;
					goblin.gold -= input2 * ConsumableTable.generate("Normal Health Potion").getBuyPrice();
					for (std::size_t i = 0; i < input2; ++i)
						goblin.inventory.addItem(ConsumableTable.generate("Normal Health Potion"));
					wait_enter_2();
				}
				break;
			case 2:
				cout << "How many would you like to buy? (" << ConsumableTable.generate("Greater Health Potion").getBuyPrice() << " Each)" << endl;
				cin >> input2;
				if (input2 * ConsumableTable.generate("Greater Health Potion").getBuyPrice() > goblin.gold) {
					cout << "You do not have enough gold!" << endl;
					wait_enter_2();
					city();
				}
				else if (input2 * ConsumableTable.generate("Greater Health Potion").getBuyPrice() <= goblin.gold) {
					cout << "You bought " << input2 << " Greater Health Potions for " << input2 * ConsumableTable.generate("Greater Health Potion").getBuyPrice() << " gold" << endl;
					goblin.gold -= input2 * ConsumableTable.generate("Greater Health Potion").getBuyPrice();
					for (std::size_t i = 0; i < input2; ++i)
						goblin.inventory.addItem(ConsumableTable.generate("Greater Health Potion"));
					wait_enter_2();
				}
				break;
			case 3:
				cout << "How many would you like to buy? (" << ConsumableTable.generate("Super Health Potion").getBuyPrice() << " Each)" << endl;
				cin >> input2;
				if (input2 * ConsumableTable.generate("Super Health Potion").getBuyPrice() > goblin.gold) {
					cout << "You do not have enough gold!" << endl;
					wait_enter_2();
					city();
				}
				else if (input2 * ConsumableTable.generate("Super Health Potion").getBuyPrice() <= goblin.gold) {
					cout << "You bought " << input2 << " Super Health Potions for " << input2 * ConsumableTable.generate("Super Health Potion").getBuyPrice() << " gold" << endl;
					goblin.gold -= input2 * ConsumableTable.generate("Super Health Potion").getBuyPrice();
					for (std::size_t i = 0; i < input2; ++i)
						goblin.inventory.addItem(ConsumableTable.generate("Super Health Potion"));
					wait_enter_2();
				}
				break;
			case 4:
				cout << "How many would you like to buy? (" << ConsumableTable.generate("Full Health Potion").getBuyPrice() << " Each)" << endl;
				cin >> input2;
				if (input2 * ConsumableTable.generate("Full Health Potion").getBuyPrice() > goblin.gold) {
					cout << "You do not have enough gold!" << endl;
					wait_enter_2();
					city();
				}
				else if (input2 * ConsumableTable.generate("Full Health Potion").getBuyPrice() <= goblin.gold) {
					cout << "You bought " << input2 << " Full Health Potions for " << input2 * ConsumableTable.generate("Full Health Potion").getBuyPrice() << " gold" << endl;
					goblin.gold -= input2 * ConsumableTable.generate("Full Health Potion").getBuyPrice();
					for (std::size_t i = 0; i < input2; ++i)
						goblin.inventory.addItem(ConsumableTable.generate("Full Health Potion"));
					wait_enter_2();
				}
				break;
			case 5:
				cout << "How many would you like to buy? (" << ConsumableTable.generate("Normal Mana Potion").getBuyPrice() << " Each)" << endl;
				cin >> input2;
				if (input2 * ConsumableTable.generate("Normal Mana Potion").getBuyPrice() > goblin.gold) {
					cout << "You do not have enough gold!" << endl;
					wait_enter_2();
					city();
				}
				else if (input2 * ConsumableTable.generate("Normal Mana Potion").getBuyPrice() <= goblin.gold) {
					cout << "You bought " << input2 << " Normal Mana Potions for " << input2 * ConsumableTable.generate("Normal Mana Potion").getBuyPrice() << " gold" << endl;
					goblin.gold -= input2 * ConsumableTable.generate("Normal Mana Potion").getBuyPrice();
					for (std::size_t i = 0; i < input2; ++i)
						goblin.inventory.addItem(ConsumableTable.generate("Normal Mana Potion"));
					wait_enter_2();
				}
				break;
			case 6:
				cout << "How many would you like to buy? (" << ConsumableTable.generate("Greater Mana Potion").getBuyPrice() << " Each)" << endl;
				cin >> input2;
				if (input2 * ConsumableTable.generate("Greater Mana Potion").getBuyPrice() > goblin.gold) {
					cout << "You do not have enough gold!" << endl;
					wait_enter_2();
					city();
				}
				else if (input2 * ConsumableTable.generate("Greater Mana Potion").getBuyPrice() <= goblin.gold) {
					cout << "You bought " << input2 << " Greater Mana Potions for " << input2 * ConsumableTable.generate("Greater Mana Potion").getBuyPrice() << " gold" << endl;
					goblin.gold -= input2 * ConsumableTable.generate("Greater Mana Potion").getBuyPrice();
					for (std::size_t i = 0; i < input2; ++i)
						goblin.inventory.addItem(ConsumableTable.generate("Greater Mana Potion"));
					wait_enter_2();
				}
				break;
			case 7:
				cout << "How many would you like to buy? (" << ConsumableTable.generate("Super Mana Potion").getBuyPrice() << " Each)" << endl;
				cin >> input2;
				if (input2 * ConsumableTable.generate("Super Mana Potion").getBuyPrice() > goblin.gold) {
					cout << "You do not have enough gold!" << endl;
					wait_enter_2();
					city();
				}
				else if (input2 * ConsumableTable.generate("Super Mana Potion").getBuyPrice() <= goblin.gold) {
					cout << "You bought " << input2 << " Super Mana Potions for " << input2 * ConsumableTable.generate("Super Mana Potion").getBuyPrice() << " gold" << endl;
					goblin.gold -= input2 * ConsumableTable.generate("Super Mana Potion").getBuyPrice();
					for (std::size_t i = 0; i < input2; ++i)
						goblin.inventory.addItem(ConsumableTable.generate("Super Mana Potion"));
					wait_enter_2();
				}
				break;
			case 8:
				cout << "How many would you like to buy? (" << ConsumableTable.generate("Full Mana Potion").getBuyPrice() << " Each)" << endl;
				cin >> input2;
				if (input2 * ConsumableTable.generate("Full Mana Potion").getBuyPrice() > goblin.gold) {
					cout << "You do not have enough gold!" << endl;
					wait_enter_2();
					city();
				}
				else if (input2 * ConsumableTable.generate("Full Mana Potion").getBuyPrice() <= goblin.gold) {
					cout << "You bought " << input2 << " Full Mana Potions for " << input2 * ConsumableTable.generate("Full Mana Potion").getBuyPrice() << " gold" << endl;
					goblin.gold -= input2 * ConsumableTable.generate("Full Mana Potion").getBuyPrice();
					for (std::size_t i = 0; i < input2; ++i)
						goblin.inventory.addItem(ConsumableTable.generate("Full Mana Potion"));
					wait_enter_2();
				}
				break;
			}
			break;
		}
	}
}

void city() {
    while (true) {
        statcheck();
        system("CLS");
        cout << "-*- The Goblin City -*-" << endl;
        cout << "\n(1) The Shop\n(2) Explore\n(3) Inventory\n(4) Save\n\nHP: " << goblin.hp << "\nMP: " << goblin.mp << "\n\nGold: " << goblin.gold << endl;
        cin >> a;
        switch (a) {
        case 1:
            blackmarket();
            break;
        case 2:
            encounter();
            break;
        case 3:
            inventory();
            break;
        case 4:
           // save();
            break;
        }
    }
}
void estatgen() {
    enemy.eheight = rand() % 4 + 1;
    enemy.eage = rand() % 85 + 15;
    enemy.eweight = rand() % 35 + 15;
    enemy.elvl = rand() % goblin.lvl + 4 * difficulty;
    enemy.ename = goblinfname[rand() % 50] + goblinmname[rand() % 20] + goblinlname[rand() % 50];
    enemy.estr = ((enemy.eheight * 2) + (enemy.eweight / 2) - (enemy.eage / 2)) + goblin.lvl * 2 * difficulty;
    enemy.edef = ((enemy.eheight * 3) + (enemy.eweight) - enemy.eage) + goblin.lvl * 2 * difficulty;
    enemy.espd = (((enemy.eheight * 15) - (enemy.eweight - 15))) + goblin.lvl * 2 * difficulty;
    enemy.ecrt = ((enemy.eheight / 3) - (enemy.eweight / 2) + enemy.eage) + goblin.lvl * 2 * difficulty;
    enemy.maxeHP = (enemy.estr * 2) + (enemy.edef * 3) + (enemy.eage + 20) + enemy.elvl * 2 * difficulty;
    enemy.maxeMP = (enemy.estr) - (enemy.edef) + (enemy.eage + 20) + enemy.elvl;
    if (enemy.maxeMP < 9) {
        enemy.maxeMP = 10;
    }
    if (enemy.ecrt > 100) {
        enemy.ecrt = 100;
    }
    if (enemy.maxeHP < 9) {
        enemy.maxeHP = 10;
    }
    if (enemy.estr <= 0) {
        enemy.estr = 1;
    }
    if (enemy.espd <= 0) {
        enemy.espd = 1;
    }
    if (enemy.espd >= 50) {
        enemy.espd = 50;
    }
    if (enemy.edef <= 0) {
        enemy.edef = 1;
    }
    if (enemy.ecrt <= 0) {
        enemy.ecrt = 1;
    }
    enemy.emp = enemy.maxeMP;
    enemy.ehp = enemy.maxeHP;
}

void statcheck() {
    if(goblin.potency < 0) {
        goblin.potency = .01;
    }
    if (goblin.maxHP < 5) {
        goblin.maxHP = 5;
    }
    if (goblin.maxMP < 5) {
        goblin.maxMP = 5;
    }
    if (goblin.hp > goblin.maxHP) {
        goblin.hp = goblin.maxHP;
    }
    if (goblin.mp > goblin.maxMP) {
        goblin.mp = goblin.maxMP;
    }
    if (enemy.emp > enemy.maxeMP) {
        enemy.emp = enemy.maxeMP;
    }
    if (goblin.crt >= 100) {
        goblin.crt = 100;
    }
    if (goblin.str <= 0) {
        goblin.str = 1;
    }
    if (goblin.spd <= 0) {
        goblin.spd = 1;
    }
    if (goblin.spd >= 50) {
        goblin.spd = 50;
    }
    if (goblin.def <= 0) {
        goblin.def = 1;
    }
    if (goblin.crt <= 0) {
        goblin.crt = 1;
    }
}




void goblingen() {
    while (true) {
        system("CLS");
        cout << "-*- Goblin Creation! -*-\n\nWelcome to the world of Uearth! \n\nDescribe your goblin..." << endl;
        swapColor();
        cout << "\n1";
        swapColor();
        cout << ") Height\n\n";
        swapColor();
        cout << "2";
        swapColor();
        cout << ") Weight\n\n";
        swapColor();
        cout << "3";
        swapColor();
        cout << ") Age\n\n";
        swapColor();
        cout << "4";
        swapColor();
        cout << ") Antiquity\n\n\n";
        swapColor();
        cout << "5";
        swapColor();
        cout << ") Finish" << endl;
        cout << "\nCurrent Build: \nName: ";
        swapColor();
        cout << goblin.name;
        swapColor();
        cout << "\nHeight: ";
        swapColor();
        cout << goblin.height;
        swapColor();
        cout << " feet\nWeight: ";
        swapColor();
        cout << goblin.weight;
        swapColor();
        cout << " pounds\nAge: ";
        swapColor();
        cout << goblin.age;
        swapColor();
        cout << " years old\nAntiquity: ";
        swapColor();
        cout << goblin.antiquity << endl;
        swapColor();
        a = waitForKey();
        switch (a) {
        case 1:
            system("cls");
            cout << "-*- Height -*-\n\nHeight:\nHeight focuses on your strength and defense, but decreases your spells' potency." << endl;
            for (size_t i = 1; i < 4; ++i) {
                swapColor();
                cout << i;
                swapColor();
                cout << ") " << goblinheight[i] << " feet" << endl;
            }
            special = waitForKey();
            if (special > 3 || special < 1) {
                cout << "No." << endl;
                wait_enter();
            }
            else {
                goblin.height = goblinheight[special];
            }
            break;
        case 2:
            system("cls");
            cout << "-*- Weight -*-\n\nWhat do you want your weight to be? Min: 15 || Max: 65\n\nWeight:\nThe heavier you are, the harder you hit. Your defense is also increased greatly\nBut, your spells are less potent and your dexterity is greatly reduced.\n" << endl;
            cin >> special;
            if (special < 15 || special > 65) {
                cout << "You cannot do that." << endl;
                wait_enter();
            }
            else if (special >= 15 && special <= 65) {
                cout << "Your weight is: " << special << " pounds." << endl;
                goblin.weight = special;
                wait_enter();
            }
            break;
        case 3:
            system("cls");
            cout << "-*- Age -*-\n\nWhat do you want your Age to be? Min: 15 || Max: 50\n\nAge: \nThe older you are, the more potent your spells- however... you are frail. \nYour melee attacks are less powerful unless you use your Signature Slam.\n" << endl;
            cin >> special;
            if (special < 15 || special > 50) {
                cout << "You cannot do that." << endl;
                wait_enter();
            }
            else if (special >= 15 && special <= 50) {
                cout << "Your age is: " << special << " years old." << endl;
                goblin.age = special;
                wait_enter();
            }
            break;
        case 4:
            system("cls");
            cout << "-*- Antiquity -*-\nYour past experiences can prove useful in your adventures...\n\n" << endl;
            swapColor();
            cout << "1";
            swapColor();
            cout << ") Wayward Vagabond -\n     You previously lived in many places, drifting across the lands and \n     experiencing everything they have to offer.\n     (All Combat Stats +3 || Potency +.3x || 500 Starting Gold)" << endl;
            swapColor();
            cout << "\n2";
            swapColor();
            cout << ") Unassailable Scrapper -\n     You have fought constantly and know everything about battle.\n     You have never taken in interest in magic before.\n     (All Combat Stats +10 || Potency -.5x || 100 Starting Gold)" << endl;
            swapColor();
            cout << "\n3";
            swapColor();
            cout << ") Supernatural Thaumaturgist -\n     You have performed miracles to countless goblins and trolls.\n     Your healing spells know no bounds.\n     (Cure heals 2x as much || Potency +.3x || 250 Starting Gold)" << endl;
            swapColor();
            cout << "\n4";
            swapColor();
            cout << ") Vehement Occultist -\n     You have mastered destructive spells and used them in combat.\n     Your spells' power is unmatched.\n     (Destructive Spells deal 1.5x Damage || Potency +.3x || 250 Starting Gold)" << endl;
            special = waitForKey();
            switch(special) {
            case 1:
            goblin.antiquity = "Wayward Vagabond";
            break;
            case 2:
            goblin.antiquity = "Unassailable Scrapper";
            break;
            case 3:
            goblin.antiquity = "Supernatural Thaumaturgist";
            break;
            case 4:
            goblin.antiquity = "Vehement Occultist";
            break;
            }
            break;
        case 5:
            system("CLS");
            goblin.str = (goblin.height * 3) + (goblin.weight / 5) - (goblin.age / 10);
            goblin.def = (goblin.height - 1) + (goblin.weight / 3) - (goblin.age / 6);
            goblin.spd = (goblin.height * -2) - (goblin.weight / 2) + (50 - (goblin.age / 2));
            goblin.crt = goblin.age / 5;
            goblin.maxHP = goblin.height + (goblin.weight + 10) - (10 + (goblin.age / 2));
            goblin.maxMP = (goblin.age / 2) - (goblin.weight / 10) - goblin.height;
            goblin.potency = 1 + ((double)goblin.age / 100) - ((double)goblin.height / 5);
            goblin.hp = goblin.maxHP;
            goblin.mp = goblin.maxMP;
            if(goblin.antiquity == "Wayward Vagabond") {
            goblin.str += 3;
            goblin.def += 3;
            goblin.spd += 3;
            goblin.crt += 3;
            goblin.potency += .3;
            goblin.gold = 500;
            } else if (goblin.antiquity == "Unassailable Scrapper") {
            goblin.str += 10;
            goblin.def += 10;
            goblin.spd += 10;
            goblin.crt += 5;
            goblin.potency -= .5;
            goblin.gold = 100;
            } else if (goblin.antiquity == "Supernatural Thaumaturgist") {
            goblin.potency += .3;
            goblin.gold = 250;
            } else if(goblin.antiquity == "Vehement Occultist") {
            goblin.potency += .3;
            goblin.gold = 250;
            }
            statcheck();
            cout << "-*- Generation of Stats -*-" << endl;
            cout << "Strength: ";
            swapColor();
            cout << goblin.str << endl;
            swapColor();
            cout << "Defense: ";
            swapColor();
            cout << goblin.def << endl;
            swapColor();
            cout << "Dexterity: ";
            swapColor();
            cout << goblin.spd << endl;
            swapColor();
            cout << "Critical Chance: ";
            swapColor();
            cout << goblin.crt;
            swapColor();
            cout << "%\nPotency: ";
            swapColor();
            cout << goblin.potency;
            swapColor();
            cout << "x\n\nMax Health: ";
            swapColor();
            cout << goblin.maxHP << endl;
            swapColor();
            cout << "Max Mana: ";
            swapColor();
            cout << goblin.maxMP << endl;
            cout << "\n1";
            swapColor();
            cout << ") Yes! The perfect Goblin!\n" << endl;
            swapColor();
            cout << "2";
            swapColor();
            cout << ") No, take that away!\n\n-*- Stat Explanation -*-\nStrength: Melee damage\nDefense: % of melee damage reduction\nDexterity: Determines multi-attack\nCritical Chance: Chance of dealing 2x damage\nPotency: Spells power is multiplied by the value\nHP: Hitpoints\nMP: Manapoints" << endl;
            cin >> a;
            switch (a) {
            case 1:
                goblin.inventory.addItem(WeaponTable.generate("Stick"));
                goblin.equipped = const_cast<Weapon*>(dynamic_cast<const Weapon*>(goblin.inventory.inspectItem(0)));
                if(goblin.name == "Crowyo") {
                    goblin.inventory.addItem(WeaponTable.generate("Modal Soul"));
                    goblin.equipped = const_cast<Weapon*>(dynamic_cast<const Weapon*>(goblin.inventory.inspectItem(0)));
                }
                city();
                break;
            case 2:
                goblingen();
                break;
            }
        }
    }
}

void goblinnaming() {
    system("CLS");
    cout << "-*- Goblin Generation -*- " << endl;
    cout << "\n\nWhat difficulty would you like to play on?\n" << endl;
    swapColor();
    cout << "1";
    swapColor();
    cout << ") Easy (.75x Multiplier, .75x Loot)\n\n";
    swapColor();
    cout << "2";
    swapColor();
    cout << ") Normal (1x Multiplier, 1x Loot)\n\n";
    swapColor();
    cout <<"3";
    swapColor();
    cout << ") Hard (2x Mulitplier, 2x Loot)" << endl;
    a = waitForKey();
    switch (a) {
    case 1:
        difficulty = .75;
        difficultyn = "Easy";
        break;
    case 2:
        difficulty = 1;
        difficultyn = "Normal";
        break;
    case 3:
        difficulty = 2;
        difficultyn = "Hard";
        break;
    }
    string oldname;
    bool randcheck = false;
    while (randcheck == false) {
		oldname = goblin.name;
		system("cls");
		cout << "-*- Name -*-\n\nWhat's your name?\nType anything for a custom name!\n\n";
		swapColor();
		cout << "1";
		swapColor();
		cout << ") Finish\n\n";
		swapColor();
		cout << "2";
		swapColor();
		cout << ") Random Goblin Name\n\nCurrent Name: " << goblin.name << "\n" << endl;
		getline(cin, goblin.name);
		if (goblin.name == "1") {
			randcheck = true;
		}
		if (goblin.name == "2") {
			system("cls");
			string goblinrname1 = goblinfname[rand() % 50] + goblinmname[rand() % 20] + goblinlname[rand() % 50];
            goblin.name = goblinrname1;
		}
	}
 goblin.name = oldname;
  goblingen();
}










