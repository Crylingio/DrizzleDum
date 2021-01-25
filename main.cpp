
//11/1/17 crow

//#include "stdafx.h"
#include "windows.h"
#include "time.h"
#include "filesystem.h"
#include <array>
#include <iostream>
#include <string>
#include <vector>
#include <fstream>
using namespace std;
using Filedata = baelothelib::Filedata;
using FileWriter = baelothelib::FileWriter;
using FileReader = baelothelib::FileReader;
HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);


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

int goblinheight[3] = { 2, 3, 4 };
int bossdef = 0;
string goblincolor[6] = { "Yellow-Green", "Green", "Brown-Green", "Blue", "Red", "Jet Black" };
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
int difficulty = 2;
string difficultyn = "Normal";
int special;
bool battlemenu = false;


struct Inventory { //8
    int nhpp = 3;
    int ghpp = 1;
    int bhpp = 0;
    int fullre = 0;
    int lunch = 0;
    int gold = 100;
    int bottlerocket = 0;
    int mbottlerocket = 0;
} inventory;

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
    int height = 3;
    int maxHP;
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
    int seedfix = 1;
    string color = "Green";
    string name;
} goblin;

    Filedata fd({difficultyn, goblin.name},
    {seed, difficulty, bossdef, inventory.nhpp, inventory.ghpp, inventory.bhpp, inventory.fullre, inventory.lunch, inventory.gold, inventory.bottlerocket, inventory. mbottlerocket, goblin.height, goblin.maxHP, goblin.maxMP, goblin.mp, goblin.hp, goblin.str, goblin.spd, goblin.def, goblin.lvl, goblin.crt, goblin.exp, goblin.age, goblin.weight},
    {});

string goblinname[3] = { "First", " Second", " Last" };

int main()
{
    //SetConsoleTitle("DrizzleDum");
    SetConsoleTextAttribute(hConsole, k);
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
            case 6:
                cout << "Secret Boss!" << endl;
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
            inven();
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
        inventory.bottlerocket = inventory.bottlerocket + 1;
        inventory.nhpp = inventory.nhpp + 2;
        inventory.ghpp = inventory.ghpp + 2;
        goblin.exp = goblin.exp + 250;
        bossdef = 2;
        wait_enter();
        city();
    }
    if (bossdef == 3) {
        cout << "The big boy troll falls to the ground... You find some items.\n3 Bottle Rockets\n3 Greater Health Potions\n3 Lunches\n+500 EXP" << endl;
        inventory.bottlerocket = inventory.bottlerocket + 1;
        inventory.nhpp = inventory.nhpp + 2;
        inventory.ghpp = inventory.ghpp + 2;
        goblin.exp = goblin.exp + 500;
        bossdef = 4;
        wait_enter();
        city();
    }
    else if (bossdef == 5) {
        cout << "The bad boy troll falls to the ground... You find some items.\n3 Bottle Rockets\n1 Multi-Bottle Rocket\n5 Greater Health Potions\n5 Lunches\n1 Full Restore\n+1000 EXP" << endl;
        inventory.bottlerocket = inventory.bottlerocket + 3;
        inventory.mbottlerocket = inventory.mbottlerocket + 1;
        inventory.ghpp = inventory.ghpp + 5;
        inventory.lunch = inventory.lunch + 3;
        inventory.fullre = inventory.fullre + 3;
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
        inventory.gold = inventory.gold + damage;
        damage = rand() % (enemy.estr + enemy.maxeHP + enemy.elvl);
        cout << "You gain " << damage << " EXP!" << endl;
        goblin.exp = goblin.exp + damage;
        goblin.seedfix = goblin.seedfix + 1;
        damage = rand() % 2 + 1;
        cout << "You find " << damage << " normal health potions!" << endl;
        inventory.nhpp = inventory.nhpp + damage;
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
    if (enemy.ehp < enemy.maxeHP / 3) {
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
    system("CLS");
    sout("-*- The Goblin Adventure of Doom and Misery (+ Pain) -*- ", 100, true);
    cout << "(1) Create your Goblin!\n(2) Load a File\n\n\nWARNING: Save and loads DO work! \nSpeedrun Version! v1.0!\n\n\nWelcome to the all new era of Drizzle Dum..\nAny% Rules\n - No save editing\n - Any seed\n - Any difficulty\n - No Macros / TAS" << endl;
    cin >> a;
    switch (a) {
    case 1:
        goblinnaming();
        break;
    case 2:
        load();
        break;
    }

}

void load() {
    string saveload;
    cout << "What file would you like to load?\n(Input File name)" << endl;
    cin >> saveload;
    saveload = saveload + ".txt";
    Filedata rd;
    FileReader fr;
    fr.open(saveload).read(rd);
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
    goblin.seedfix = rd.m_nums.at(24);
    srand(seed + goblin.seedfix);
city();
}

void save() {
    if(savedone == false) {
    cout << "What do you want your save named? \n(No spaces!)" << endl;
    cin >> savename;
    savename = savename + ".txt";
    savedone = true;
    }
    cout << "Saving..." << endl;
    wait_enter();
    Filedata fd({goblin.name, difficultyn},
                {seed, difficulty, bossdef, inventory.nhpp, inventory.ghpp, inventory.bhpp, inventory.fullre, inventory.lunch, inventory.gold, inventory.bottlerocket, inventory.mbottlerocket, goblin.height, goblin.maxHP, goblin.maxMP, goblin.mp, goblin.hp, goblin.str, goblin.spd, goblin.def, goblin.lvl, goblin.crt, goblin.exp, goblin.age, goblin.weight, goblin.seedfix},
                {});
    FileWriter fw;
    if(fw.open(fd).write(savename)) {
    }
}


void city() {
    while (true) {
        statcheck();
        system("CLS");
        cout << "-*- The Goblin City -*-" << endl;
        cout << "\n(1) The Shop\n(2) Explore\n(3) Inventory\n(4) Save\n\nHP: " << goblin.hp << "\nMP: " << goblin.mp << "\n\nGold: " << inventory.gold << endl;
        cin >> a;
        switch (a) {
        case 4000:
            load();
        case 1:
            system("CLS");
            cout << "'Welcome to the shop! What do you want?' The shop-person at the counter says." << endl;
            cout << "(1) Budget Health Potion - 1 Gold\n(2) Normal Health Potion - 50 Gold\n(3) Greater Health Potion - 200 Gold\n(4) Lunch - 100 Gold\n(5) Full Restore - 750 Gold\n\n(6) Skip Sandwich (1000 Gold | +3 Spd)\n(7) Trout Yogurt (1000 Gold | +3 Str)\n(8) Brain Food (1000 Gold | +3 Crit Chance)\n(9) Cheeseburger (1000 Gold | +3 Def)\n(10) Life Noodles (2000 Gold | +5 MaxHP +3 MaxMP)\n\n(11) Bottle Rocket (250 Gold Each)\n(12) Multi-Bottle Rocket - (1000 Gold) \n\n(0) Exit" << endl;
            cin >> a;
            switch (a) {
            case 1:
                if (inventory.gold <= 0) {
                    cout << "You don't have enough gold!" << endl;
                    wait_enter();
                }
                else if (inventory.gold >= 1) {
                    cout << "How many Budget Potions would you like to buy? (1 Gold Each)" << endl;
                    cin >> special;
                    if (special < 0) {
                        cout << "Stop." << endl;
                        wait_enter();
                        continue;
                    }
                    if (inventory.gold < special) {
                        cout << "You don't have enough gold!" << endl;
                        wait_enter();
                    }
                    else if (inventory.gold >= special) {
                        cout << "You bought " << special << " budget potions for " << special << " gold!" << endl;
                        inventory.bhpp = inventory.bhpp + special;
                        inventory.gold = inventory.gold - special * 1;
                        wait_enter();

                    }
                }
                break;
            case 2:
                if (inventory.gold <= 49) {
                    cout << "You don't have enough gold!" << endl;
                    wait_enter();
                }
                else if (inventory.gold >= 50) {
                    cout << "How many Normal Health Potions would you like to buy? (50 Gold Each)" << endl;
                    cin >> special;
                    if (special < 0) {
                        cout << "Stop." << endl;
                        wait_enter();
                        continue;
                    }
                    if (inventory.gold < special * 50) {
                        cout << "You don't have enough gold!" << endl;
                        wait_enter();
                    }
                    else if (inventory.gold >= special) {
                        cout << "You bought " << special << " Normal Health Potions for " << special * 50 << " gold!" << endl;
                        inventory.nhpp = inventory.nhpp + special;
                        inventory.gold = inventory.gold - special * 50;
                        wait_enter();
                    }
                }
                break;
            case 3:
                if (inventory.gold <= 199) {
                    cout << "You don't have enough gold!" << endl;
                    wait_enter();
                }
                else if (inventory.gold >= 200) {
                    cout << "How many Greater Health Potions would you like to buy? (200 Gold Each)" << endl;
                    cin >> special;
                    if (special < 0) {
                        cout << "Stop." << endl;
                        wait_enter();
                        continue;
                    }
                    if (inventory.gold < special * 200) {
                        cout << "You don't have enough gold!" << endl;
                        wait_enter();
                    }
                    else if (inventory.gold >= special) {
                        cout << "You bought " << special << " Greater Health Potions for " << special * 200 << " gold!" << endl;
                        inventory.ghpp = inventory.ghpp + special;
                        inventory.gold = inventory.gold - special * 200;
                        wait_enter();
                    }
                }
                break;
            case 4:
                if (inventory.gold <= 99) {
                    cout << "You don't have enough gold!" << endl;
                    wait_enter();
                }
                else if (inventory.gold >= 100) {
                    cout << "How many Lunches would you like to buy? (100 Gold Each)" << endl;
                    cin >> special;
                    if (special < 0) {
                        cout << "Stop." << endl;
                        wait_enter();
                        continue;
                    }
                    if (inventory.gold < special * 100) {
                        cout << "You don't have enough gold!" << endl;
                        wait_enter();
                    }
                    else if (inventory.gold >= special) {
                        cout << "You bought " << special << " Lunches for " << special * 100 << " gold!" << endl;
                        inventory.lunch = inventory.lunch + special;
                        inventory.gold = inventory.gold - special * 100;
                        wait_enter();
                    }
                }
                break;
            case 5:
                if (inventory.gold < 750) {
                    cout << "You don't have enough gold!" << endl;
                    wait_enter();
                }
                else if (inventory.gold >= 750) {
                    cout << "How many Full Restores would you like to buy? (750 Gold Each)" << endl;
                    cin >> special;
                    if (special < 0) {
                        cout << "Stop." << endl;
                        wait_enter();
                        continue;
                    }
                    if (inventory.gold < special * 750) {
                        cout << "You don't have enough gold!" << endl;
                        wait_enter();
                    }
                    else if (inventory.gold >= special) {
                        cout << "You bought " << special << " Full Restores for " << special * 750 << " gold!" << endl;
                        inventory.fullre = inventory.fullre + special;
                        inventory.gold = inventory.gold - special * 750;
                        wait_enter();
                    }
                }
                break;
            case 6:
                if (inventory.gold < 999) {
                    cout << "You don't have enough gold!" << endl;
                    wait_enter();
                }
                else if (inventory.gold >= 1000) {
                    cout << "You bought a Skip Sandwich for 100 gold!" << endl;
                    goblin.spd = goblin.spd + 3;
                    inventory.gold = inventory.gold - 1000;
                    wait_enter();
                }
                break;
            case 7:
                if (inventory.gold < 999) {
                    cout << "You don't have enough gold!" << endl;
                    wait_enter();
                }
                else if (inventory.gold >= 1000) {
                    cout << "You bought a Trout Yogurt for 1000 gold!" << endl;
                    goblin.str = goblin.str + 3;
                    inventory.gold = inventory.gold - 1000;
                    wait_enter();
                }
                break;
            case 8:
                if (inventory.gold < 999) {
                    cout << "You don't have enough gold!" << endl;
                    wait_enter();
                }
                else if (inventory.gold >= 1000) {
                    cout << "You bought Brain Food for 1000 gold!" << endl;
                    goblin.crt = goblin.crt + 3;
                    inventory.gold = inventory.gold - 1000;
                    wait_enter();
                }
                break;
            case 9:
                if (inventory.gold < 999) {
                    cout << "You don't have enough gold!" << endl;
                    wait_enter();
                }
                else if (inventory.gold >= 1000) {
                    cout << "You bought a Cheeseburger for 1000 gold!" << endl;
                    goblin.def = goblin.def + 3;
                    inventory.gold = inventory.gold - 1000;
                    wait_enter();
                }
                break;
            case 10:
                if (inventory.gold < 1999) {
                    cout << "You don't have enough gold!" << endl;
                    wait_enter();
                }
                else if (inventory.gold >= 2000) {
                    cout << "You bought some Life Noodles for 2000 gold!" << endl;
                    maxHPMPadd = maxHPMPadd + 5;
                    inventory.gold = inventory.gold - 2000;
                    wait_enter();
                }
                break;
            case 11:
                if (inventory.gold <= 249) {
                    cout << "You don't have enough gold!" << endl;
                    wait_enter();
                }
                else if (inventory.gold >= 250) {
                    cout << "How many Bottle Rockets would you like to buy? (250 Gold Each)" << endl;
                    cin >> special;
                    if (special < 0) {
                        cout << "Stop." << endl;
                        wait_enter();
                        continue;
                    }
                    if (inventory.gold < special * 250) {
                        cout << "You don't have enough gold!" << endl;
                        wait_enter();
                    }
                    else if (inventory.gold >= special * 250) {
                        cout << "You bought " << special << " Bottle Rockets for " << special * 250 << " gold!" << endl;
                        inventory.bottlerocket = inventory.bottlerocket + special;
                        inventory.gold = inventory.gold - special * 250;
                        wait_enter();

                    }
                }
                break;
            case 12:
                if (inventory.gold <= 999) {
                    cout << "You don't have enough gold!" << endl;
                    wait_enter();
                }
                else if (inventory.gold >= 1000) {
                    cout << "How many Multi-Bottle Rockets would you like to buy? (1000 Gold Each)" << endl;
                    cin >> special;
                    if (special < 0) {
                        cout << "Stop." << endl;
                        wait_enter();
                        continue;
                    }
                    if (inventory.gold < special * 1000) {
                        cout << "You don't have enough gold!" << endl;
                        wait_enter();
                    }
                    else if (inventory.gold >= special * 1000) {
                        cout << "You bought " << special << " Multi-Bottle Rockets for " << special * 1000 << " gold!" << endl;
                        inventory.mbottlerocket = inventory.mbottlerocket + special;
                        inventory.gold = inventory.gold - special * 1000;
                        wait_enter();

                    }
                }
                break;
            }
            break;
        case 2:
            encounter();
            break;
        case 3:
            inven();
            break;
        case 4:
            save();
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
    goblin.maxMP = (goblin.str) - (goblin.def) + (goblin.age + 20) + goblin.lvl * 2;
    goblin.maxHP = (goblin.str * 2) + (goblin.def * 3) - (goblin.age + 20) + goblin.lvl;
    if (goblin.maxHP < 9) {
        goblin.maxHP = 10;
    }
    if (goblin.maxMP < 9) {
        goblin.maxMP = 10;
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

void inven() {
    if(goblin.exp <= 0) {
       goblin.exp = 1;
       }
    int explevel = (goblin.exp -(goblin.lvl * 50 * difficulty));
    if(explevel < 0) {
    explevel = 1;
    }
    system("CLS");
    cout << "-*- Inventory -*-" << endl;
    cout << "Current Health: " << goblin.hp << "\nCurrent Level: " << goblin.lvl << "\n\nGold: " << inventory.gold << "\n\n(1) Budget Health Potions: " << inventory.bhpp << "\n(2) Normal Health Potions: " << inventory.nhpp << "\n(3) Greater Health Potions: " << inventory.ghpp << "\n(4) Lunches: " << inventory.lunch << "\n(5) Full Restores : " << inventory.fullre << "\n(6) Bottle Rockets: " << inventory.bottlerocket << "\n(7) Multi-Bottle Rockets: " << inventory.mbottlerocket << "\n\n(0) End" << endl;
    cout << "\n\nName: " << goblin.name << "\nAge: " << goblin.age << " years old\nWeight: " << goblin.weight << " pounds\nHeight: " << goblin.height << " feet\n\nStrength: " << goblin.str << "\nDefense: " << goblin.def << "\nSpeed: " << goblin.spd << "\nCritical Chance: " << goblin.crt << "\nMax Health: " << goblin.maxHP << "\nMax Mana: " << goblin.maxMP << "\n\nDifficulty: " << difficultyn << "\nExp Till Level Up: " <<  explevel << endl;
    cin >> a;
    switch (a) {
    case 1:
        if (goblin.hp == goblin.maxHP) {
            cout << "You are already at max HP!" << endl;
            wait_enter();
        }
        else if (goblin.hp < goblin.maxHP) {
            if (inventory.bhpp <= 0) {
                cout << "You do not have enough potions!" << endl;
                wait_enter();
            }
            else if (inventory.bhpp >= 1) {
                goblin.hp = goblin.hp + 1;
                cout << "You use a Budget Health Potion to restore 1 HP!\nHealth: " << goblin.hp << endl;
                inventory.bhpp = inventory.bhpp - 1;
                wait_enter();
            }
        }
        break;
    case 2:
        if (goblin.hp == goblin.maxHP) {
            cout << "You are already at max HP!" << endl;
            wait_enter();
        }
        else if (goblin.hp < goblin.maxHP) {
            if (inventory.nhpp <= 0) {
                cout << "You do not have enough potions!" << endl;
                wait_enter();
            }
            else if (inventory.nhpp >= 1) {
                goblin.hp = goblin.hp + 25;
                cout << "You use a Normal Health Potion to restore 25 HP!\nHealth: " << goblin.hp << endl;
                inventory.nhpp = inventory.nhpp - 1;
                wait_enter();
            }
        }
        break;
    case 3:
        if (goblin.hp == goblin.maxHP) {
            cout << "You are already at max HP!" << endl;
            wait_enter();
        }
        else if (goblin.hp < goblin.maxHP) {
            if (inventory.ghpp <= 0) {
                cout << "You do not have enough potions!" << endl;
                wait_enter();
            }
            else if (inventory.ghpp >= 1) {
                goblin.hp = goblin.hp + 100;
                cout << "You use a Greater Health Potion to restore 100 HP!\nHealth: " << goblin.hp << endl;
                inventory.ghpp = inventory.ghpp - 1;
                wait_enter();
            }
        }
        break;
    case 4:
        if (goblin.hp == goblin.maxHP) {
            cout << "You are already at max HP!" << endl;
            wait_enter();
        }
        else if (goblin.hp < goblin.maxHP) {
            if (battlemenu == false) {
                if (inventory.lunch <= 0) {
                    cout << "You do not have enough lunches!" << endl;
                    wait_enter();
                }
                else if (inventory.lunch >= 1) {
                    goblin.hp = goblin.maxHP;
                    cout << "You eat a lunch! Restoring all your HP!\nHealth: " << goblin.hp << endl;
                    inventory.lunch = inventory.lunch - 1;
                    wait_enter();
                }
                else if (battlemenu == true) {
                    cout << "You cannot eat a lunch while in battle!" << endl;
                    wait_enter();
                }
            }
        }
        break;
    case 5:
        if (goblin.hp == goblin.maxHP && goblin.mp == goblin.maxMP) {
            cout << "You are already at max HP and MP!" << endl;
            wait_enter();
        }
        else if (goblin.hp < goblin.maxHP || goblin.mp < goblin.maxMP) {
            if (inventory.fullre <= 0) {
                cout << "You do not have enough potions!" << endl;
                wait_enter();
            }
            else if (inventory.fullre >= 1) {
                goblin.hp = goblin.maxHP;
                goblin.mp = goblin.maxMP;
                cout << "You use a Full Restore to restore All your HP and MP Back!\nHealth: " << goblin.hp << endl;
                inventory.fullre = inventory.fullre - 1;
                wait_enter();
            }
        }
        break;
    case 6:
        if (battlemenu == true) {
            if (inventory.bottlerocket <= 0) {
                cout << "You do not have enough Bottle Rockets!" << endl;
                wait_enter();
            }
            else if (inventory.bottlerocket >= 1) {
                damage = rand() & 250 + 1;
                cout << "You use a Bottle Rocket! Dealing " << damage << " to the enemy!" << endl;
                enemy.ehp = enemy.ehp - damage;
                inventory.bottlerocket = inventory.bottlerocket - 1;
                wait_enter();
                enemyturn();
            }
        }
        else if (battlemenu == false) {
            cout << "You cannot use a Bottle Rocket outside of battle!" << endl;
            wait_enter();
        }
        break;
    case 7:
        if (battlemenu == true) {
            if (inventory.mbottlerocket <= 0) {
                cout << "You do not have enough Multi-Bottle Rockets!" << endl;
                wait_enter();
            }
            else if (inventory.mbottlerocket >= 1) {
                damage = rand() & 1000 + 1;
                cout << "You use a Multi-Bottle Rocket! Dealing " << damage << " to the enemy!" << endl;
                inventory.mbottlerocket = inventory.mbottlerocket - 1;
                enemy.ehp = enemy.ehp - damage;
                wait_enter();
                enemyturn();
            }
        }
        else if (battlemenu == false) {
            cout << "You cannot use a Multi-Bottle Rocket outside of battle!" << endl;
            wait_enter();
        }
        break;
        break;
    }
}


void goblingen() {
    while (true) {
        system("CLS");
        cout << "Welcome, " << goblin.name << ", to the world of Uearth! \n\nBefore your goblin starts adventuring, you must describe your goblin's physique." << endl;
        cout << "\n(1) Height\n(2) Weight\n(3) Age\n(4) Color\n\n(5) Finish" << endl;
        cout << "\nCurrent Build:\nHeight: " << goblin.height << " feet\nWeight: " << goblin.weight << " pounds\nAge: " << goblin.age << " years old\nColor: " << goblin.color << endl;
        cin >> a;
        switch (a) {
        case 1:
            for (size_t i = 0; i < 3; ++i) {
                cout << "(" << i << ") " << goblinheight[i] << " feet" << endl;
            }
            cin >> special;
            if (special > 2 || special < 0) {
                cout << "No." << endl;
                wait_enter();
            }
            else {
                goblin.height = goblinheight[special];
            }
            break;
        case 2:
            cout << "What do you want your weight to be? Min: 15 || Max: 65" << endl;
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
            cout << "What do you want your Age to be? Min: 15 || Max: 100" << endl;
            cin >> special;
            if (special < 15 || special > 100) {
                cout << "You cannot do that." << endl;
                wait_enter();
            }
            else if (special >= 15 && special <= 100) {
                cout << "Your age is: " << special << " years old." << endl;
                goblin.age = special;
                wait_enter();
            }
            break;
        case 4:
            for (size_t i = 0; i < 6; ++i) {
                cout << "(" << i << ") " << goblincolor[i] << endl;
            }
            cin >> special;
            goblin.color = goblincolor[special];
            break;
        case 5:
            system("CLS");
            goblin.str = ((goblin.height * 2) + (goblin.weight / 2) - (goblin.age / 2));
            goblin.def = ((goblin.height * 3) + (goblin.weight) - goblin.age);
            goblin.spd = (((goblin.height * 15) - (goblin.weight - 15)));
            goblin.crt = ((goblin.height / 3) - (goblin.weight / 2) + goblin.age);
            statcheck();
            goblin.hp = goblin.maxHP;
            goblin.mp = goblin.maxMP;
            cout << "Generating Stats for your Goblin. Are you ok with these stats?" << endl;
            cout << "Strength: " << goblin.str << "\nDefense: " << goblin.def << "\nSpeed: " << goblin.spd << "\nCritical Chance: " << goblin.crt << "\nMax Health: " << goblin.maxHP << "\nMax Mana: " << goblin.maxMP << "\n(1) Yes! The perfect Goblin!\n(2) No, take that away!" << endl;
            cin >> a;
            switch (a) {
            case 1:
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
    sout("\n\nWhat is your favorite thing?", 100, true);
    cin >> dad;
    for (size_t i = 0; i < dad.size(); i++) {
        seed += int(dad[i]);
    }
    srand(seed);
    cout << "\n\nWhat difficulty?" << endl;
    cout << "\n(1) Easy\n(2) Normal\n(3) Hard" << endl;
    cin >> a;
    switch (a) {
    case 1:
        difficulty = 1;
        difficultyn = "Easy";
        break;
    case 2:
        difficulty = 2;
        difficultyn = "Normal";
        break;
    case 3:
        difficulty = 3;
        difficultyn = "Hard";
        break;
    }

    while (true) {
        system("CLS");
        sout("-*- What do you name your goblin? -*-", 100, true);
        cout << "\n(1) Choose First Name!" << endl;
        cout << "(2) Middle Name!" << endl;
        cout << "(3) Rad Last Name!" << endl;
        cout << "(4) Random!" << endl;
        cout << "\n(5) Continue" << endl;
        cout << "\nCurrent Name: " << goblinname[0] << goblinname[1] << goblinname[2] << endl;
        cin >> a;
        switch (a) {
        case 1:
            for (size_t i = 0; i < 50; ++i) {
                cout << "(" << i << ") " << goblinfname[i] << endl;
            }
            cin >> a;
            goblinname[0] = goblinfname[a];
            break;
        case 2:
            for (size_t i = 0; i < 20; ++i) {
                cout << "(" << i << ") " << goblinmname[i] << endl;
            }
            cin >> a;
            goblinname[1] = goblinmname[a];
            break;
        case 3:
            for (size_t i = 0; i < 50; ++i) {
                cout << "(" << i << ") " << goblinlname[i] << endl;
            }
            cin >> a;
            goblinname[2] = goblinlname[a];
            break;
        case 5:
            cout << "Do you want to continue?\n(1) Yes!\n(2) Let Me Reconsider..." << endl;
            cin >> a;
            switch (a) {
            case 1:
                goblin.name = goblinname[0] + goblinname[1] + goblinname[2];
                goblingen();
                break;
            case 2:
                continue;
            }
        case 4:
            string goblinrname1 = goblinfname[rand() % 50] + goblinmname[rand() % 20] + goblinlname[rand() % 50];
            string goblinrname2 = goblinfname[rand() % 50] + goblinmname[rand() % 20] + goblinlname[rand() % 50];
            string goblinrname3 = goblinfname[rand() % 50] + goblinmname[rand() % 20] + goblinlname[rand() % 50];
            cout << "-*-Choose One-*-" << endl;
            cout << "(1) " << goblinrname1 << "\n(2) " << goblinrname2 << "\n(3) " << goblinrname3 << "\n\n(4) Nevermind" << endl;
            cin >> special;
            switch (special) {
            case 1:
                goblin.name = goblinrname1;
                goblingen();
                break;
            case 2:
                goblin.name = goblinrname2;
                goblingen();
                break;
            case 3:
                goblin.name = goblinrname3;
                goblingen();
                break;
            case 4:
                continue;
            }
            break;

        }
    }
}

void wait_enter(void) //Method of wait_enter, call it to create a Press Enter to continue screen.
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








