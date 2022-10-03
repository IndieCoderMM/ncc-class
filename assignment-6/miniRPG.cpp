#include <iostream>
#include <conio.h>
#include <vector>
using namespace std;

#define MAX_HP 150
#define MAX_EXP 100
#define HP 100
#define ATK 10
#define DEF 10
#define STORAGE 3

class Player
{
private:
    string bag[STORAGE] = {"empty", "empty", "empty"};
    int experience = 0;
    int attack = ATK;
    int defend = DEF;
    int health = HP;

public:
    string name;
    string role;

    Player(string n, string t)
    {
        name = n;
        role = t;
        makeAttribute();
    }

    void makeAttribute();
    void showStatus();
    void showItems();
    void addItem(string item);
    void takeDamage(int damage);
    void restoreHp(int hp);
    void gainExp(int exp);
    void drawBar(int count, int max);
};

struct Points
{
    int regen;
    int damage;
    int exp;
};

class Level
{
public:
    vector<string> story;
    vector<string> options;
    vector<string> outcomes;
    struct Points points[2];
    string map;

    void reset();
    void setPoints(int fx[]);
    void makeStory(string storyText[], int length);
    void makeOptionOutcome(string choices[], string results[], int length);
    void printStory();
    void printOptions();
    void printOutcome(int choice);
    void printMap();
};

class Game
{
private:
    int stage = 0;

public:
    int pAction = 0;
    Level level;
    bool end = false;

    void createLevel();
    void runLevel(Player *p1);
    void getPlayerAction();
    void levelOutcome(Player *p1);
    void nextLevel();
};

int getIntegerInput(int min, int max, string prompt, string error);

int main()
{
    string name;
    string roles[] = {"KNIGHT", "WIZARD", "ROGUE"};
    int inputInt;
    char cmd;
    bool run = true;
    cout << "               _   _   _" << endl;
    cout << " |\\/| | |\\| | |_| |_| | _" << endl;
    cout << " |  | | | | | | \\ |   |_|" << endl;
    cout << "---------------------------" << endl;
    cout << "Enter your character name >> ";
    getline(cin, name);
    cout << "[1]. Knight | [2]. Wizard | [3]. Rogue" << endl;
    inputInt = getIntegerInput(1, 3, "Select your role >> ", "[e]::Invalid Role!");

    Player player(name, roles[inputInt - 1]);
    Game game;

    while (run)
    {
        player.showStatus();
        game.runLevel(&player);
        if (game.end)
            break;
        do
        {
            cout << "$ [C]ontinue story | [O]pen bag | [M]ap | [E]xit game >> ";
            cin >> cmd;

            if (cmd == 'o' || cmd == 'O')
                player.showItems();
            else if (cmd == 'm' || cmd == 'M')
                game.level.printMap();
            else if (cmd == 'e' || cmd == 'E')
                run = false;

        } while (cmd == 'o' || cmd == 'O' || cmd == 'm' || cmd == 'M');

        game.nextLevel();
    }
    cout << "Press any key to close the game...";
    getch();
    return 0;
}

//
// Game Methods
//
void Game::runLevel(Player *p1)
{
    createLevel();
    cout << "\t+--------------+" << endl;
    cout << "\t|  LEVEL [0" << stage + 1 << "]  |" << endl;
    cout << "\t+--------------+" << endl;
    if (stage == 0)
    {
        cout << "-> You are a " << p1->role << " named " << p1->name << "." << endl;
        cout << "-> You wake up in a dark castle.\n-> You are bleeding." << endl;
        p1->takeDamage(50);
    }
    // Final Level
    if (stage == 5)
    {
        cout << "-> Suddenly, the Book opens and becomes a portal.\n-> You get inside the portal and everything turns white." << endl;
        cout << "-> Another Adventure is Waiting for YOU..." << endl;
        end = true;
        return;
    }

    level.printStory();
    level.printOptions();
    getPlayerAction();
    cout << endl;
    level.printOutcome(pAction);
    levelOutcome(p1);

    // Collectibles
    if (stage == 0 && pAction == 1)
        p1->addItem("Magic Potion");
    else if (stage == 3 && pAction == 1)
        p1->addItem("Magic Book");
}

// Data for each level
void Game::createLevel()
{
    if (stage == 0)
    {
        string story[] = {"You don't know what happened.", "Many monsters are dead around you.", "You find a *Magic Potion* in a box."};
        string options[] = {"Drink *Magic Potion*", "Add to Your Bag"};
        string outcomes[] = {"You drink *Magic Potion*.\n-> Your damage are healed.", "You add *Magic Potion* into your bag."};
        int points[] = {30, 0, 20, 0, 0, 10};

        level.map = "#########################\n#  O                    #\n#     x x xxx    o      -->\n#   xxxxx  xxx          #\n#     xxxxx       xxx   #\n#                       #\n#    O    [P]           #\n#            xxx xx x   #\n#  o              x     #\n#########################\n(P:player x:dead monster)";
        level.makeStory(story, 3);
        level.makeOptionOutcome(options, outcomes, 2);
        level.setPoints(points);
    }
    else if (stage == 1)
    {
        string story[] = {"You walk around the castle and hear some noise from behind.", "Many skeletons are coming to attack you."};
        string options[] = {"Fight the Skeletons", "Run into a Room"};
        string outcomes[] = {"You fight back the skeletons army.\n-> Skeletons are too many. But you defeat them easily.\n-> You walk into a room to take some rest.", "You run into a room and close the door quickly.\n-> Skeletons are trying to break the door."};
        int points[] = {0, 50, 40, 0, 10, 10};

        level.map = "  #########################\n  #     #                 #\n  #     #                 #\n  #     ########          #\n  #  xxx    O  ######====##\n ##   xxxx                #\n    xxxxxx   [P]       o  #                          \n xxxxxx                   #\n ##    xxx         OO     #\n  #########################\n (P:player x:skeleton)";
        level.makeStory(story, 2);
        level.makeOptionOutcome(options, outcomes, 2);
        level.setPoints(points);
    }
    else if (stage == 2)
    {
        string story[] = {"You notice a strange *Symbol* on the wall."};
        string options[] = {"Touch the Symbol", "Ignore the Symbol"};
        string outcomes[] = {"You touch the symbol.\n-> The symbol turns red and a secret door open.\n-> You enter the secret room.", "You ignore the symbol and find the Exit.\n-> Suddenly a secret room open."};
        int points[] = {0, 0, 10, 0, 0, 0};

        level.map = "#########################\n#.......................#\n###########****##########\n#          [P]       +  #\n# OO              ___   #\n#         ##     |___|  #\n#    O    ##            #\n#                       #\n#  ++    ####      O    #\n##############=====######\n(P:player ****:secret door)";
        level.makeStory(story, 1);
        level.makeOptionOutcome(options, outcomes, 2);
        level.setPoints(points);
    }
    else if (stage == 3)
    {
        string story[] = {"You see a glowing Book on the table.", "The book is shining green light.", "On the cover, the title is *Book of Time*."};
        string options[] = {"Open the *Magic Book*", "Add to Bag"};
        string outcomes[] = {"You try to open the Book.\n-> But it's protected by magic Spell.", "You add the Book into your bag."};
        int points[] = {0, 0, 10, 0, 0, 10};

        level.map = "#########################\n#  O++ ##          $$   #\n#       o      ++     $ #\n#   +------+            #\n#   | {B}  |          O #\n#   +------+            #\n#         [P]       $$  #\n#  oo                   #\n#  ooo oo         ##    #\n#############****########\n(P:player B:book $:treasure)";
        level.makeStory(story, 3);
        level.makeOptionOutcome(options, outcomes, 2);
        level.setPoints(points);
    }
    else if (stage == 4)
    {
        string story[] = {"Suddenly, a Blood Demon fly into the room and start attacking you.", "The Demon is trying to get the Book.", "It has a very big sword."};
        string options[] = {"Defend the Book", "Attack the Demon"};
        string outcomes[] = {"You defend the Book very hard.", "You attack back the Demon.\n-> You kill the Demon after a long, hard battle."};
        int points[] = {0, 10, 10, 0, 40, 30};

        level.map = "#########################\n#  O++ ##          $$   #\n#       o      ++     $ #\n#   +------+            #\n#   |      |  [P]     O #\n#   +------+     [X]    #\n#                       #\n#  oo                   #\n#  ooo oo         ##    #\n#############****########\n(P:player X:demon)";
        level.makeStory(story, 3);
        level.makeOptionOutcome(options, outcomes, 2);
        level.setPoints(points);
    }
}

void Game::levelOutcome(Player *p1)
{
    p1->restoreHp(level.points[pAction].regen);
    p1->takeDamage(level.points[pAction].damage);
    p1->gainExp(level.points[pAction].exp);
}

void Game::getPlayerAction()
{
    pAction = getIntegerInput(1, 2, "Select your action >> ", "[e]::Invalid Action!");
    pAction -= 1;
}

void Game::nextLevel()
{
    stage++;
    level.reset();
}

//
// Player Methods
//
void Player::makeAttribute()
{
    if (role == "KNIGHT")
    {
        defend += 20;
        health += 50;
    }
    else if (role == "WIZARD")
    {
        attack += 20;
        health += 40;
    }
    else if (role == "THIEF")
    {
        attack += 30;
        defend += 20;
    }
}

void Player::showStatus()
{
    cout << "\n+-------------------------------------------------+" << endl;

    cout << "\tCHARACTER: " << name << " (" << role << ")" << endl;
    cout << "\tEXP: ";
    drawBar(experience / 10, MAX_EXP / 10);
    cout << endl;
    cout << "\tHP: ";
    drawBar(health / 10, MAX_HP / 10);
    cout << endl;
    cout << "\tATK: #" << attack << " | DEF: #" << defend << endl;
    cout << "+-------------------------------------------------+" << endl;
}

void Player::drawBar(int count, int max)
{
    cout << "[";
    for (int i = 0; i < count; i++)
        cout << "=";
    for (int j = count; j < max; j++)
        cout << ".";
    cout << "]";
}

void Player::showItems()
{
    int width = 15;
    int items = 0;
    cout << "// Opening your bag..." << endl;
    cout << "+---------------+";
    cout << "---------------";
    cout << "+---------------+\n|";
    for (int i = 0; i < STORAGE; i++)
    {

        if (bag[i] == "empty")
        {
            for (int s = 0; s < width; s++)
                cout << " ";
        }
        else
        {
            cout << bag[i];
            for (int s = bag[i].length(); s < width; s++)
                cout << " ";
            items++;
        }
        cout << "|";
    }
    cout << "\n+---------------+";
    cout << "---------------";
    cout << "+---------------+" << endl;
    if (items == 0)
        cout << "<i> Your bag is empty!" << endl;
    else
        cout << "<i> Your bag contains " << items << " items." << endl;
}

void Player::addItem(string item)
{
    bool added = false;
    for (int i = 0; i < STORAGE; i++)
    {
        if (bag[i] == "empty")
        {
            bag[i] = item;
            added = true;
            break;
        }
    }
    if (added)
    {
        cout << "// Added " << item << " into bag." << endl;
    }
    else
    {
        cout << "<i> Your bag is full!" << endl;
    }
}

void Player::takeDamage(int dmg)
{
    if (dmg <= 0)
        return;
    health -= dmg;
    if (health < 0)
        health = 0;
    cout << "// -- " << dmg << " HP <i> Damage taken!" << endl;
}

void Player::restoreHp(int hp)
{
    if (hp <= 0)
        return;
    health += hp;
    if (health > MAX_HP)
        health = MAX_HP;
    cout << "// ++ " << hp << " HP <i> Health restored!" << endl;
}

void Player::gainExp(int exp)
{
    if (exp <= 0)
        return;
    experience += exp;
    if (experience > MAX_EXP)
        experience = MAX_EXP;
    cout << "// ++ " << exp << " EXP <i> Experience increased!" << endl;
}

//
// Level Methods
//
void Level::printStory()
{
    for (int i = 0; i < story.size(); i++)
    {
        cout << "-> " << story[i] << endl;
    }
}

void Level::printOptions()
{
    for (int i = 0; i < options.size(); i++)
    {
        cout << "$---[" << i + 1 << "]. " << options[i] << endl;
    }
}

// #########################
// #  O++ ##          $$   #
// #       o      ++     $ #
// #   +------+            #
// #   |      |  [P]     O #
// #   +------+     [X]    #
// #                       #
// #  oo                   #
// #  ooo oo         ##    #
// #############****########
// (P:player X:demon)

void Level::setPoints(int fx[])
{
    points[0].regen = fx[0];
    points[0].damage = fx[1];
    points[0].exp = fx[2];
    points[1].regen = fx[3];
    points[1].damage = fx[4];
    points[1].exp = fx[5];
}

void Level::printOutcome(int choice)
{
    cout << "## " << outcomes[choice] << endl;
}

void Level::printMap()
{
    cout << endl;
    cout << map << endl;
    cout << endl;
}

void Level::makeStory(string storyText[], int length)
{
    for (int i = 0; i < length; i++)
    {
        story.push_back(storyText[i]);
    }
}

void Level::makeOptionOutcome(string choices[], string results[], int length)
{
    for (int i = 0; i < length; i++)
    {
        options.push_back(choices[i]);
        outcomes.push_back(results[i]);
    }
}

void Level::reset()
{
    story.clear();
    options.clear();
    outcomes.clear();
}

int getIntegerInput(int min, int max, string prompt, string error)
{
    int input = -1;
    do
    {
        cout << prompt;
        cin >> input;
        if (cin.fail() || input < min || input > max)
        {
            cout << error << endl;
            cin.clear();
            cin.ignore(1000, '\n');
        }
    } while (input < min || input > max);

    return input;
}
