/*
Завдання 2. В програмі - грі, розробленій на минулих заняттях додати можливість створення різних типів юнітів. 
До кожного з юнітів додати можливість модифікації в різні типи юнітів: 
- броньованого юніта
- прискореного юніта 
- влучного юніта
Використати патерн Декоратор.

*/



#include "stdafx.h"
#include <iostream> 
#include <string>
#include <vector>
#include <ctime>
#include <conio.h>
#include <iomanip>
#include <windows.h>

using namespace std;


const int teamPlayers = 3;
#define LINE cout << "----------------------------------------\n";

//абстрактний клас - основа патерну
//декорації підлягає метод modified()
class AbstractUnit
{
public:
	virtual ~AbstractUnit()
	{}

	virtual void modified(int skill) = 0;
};


//ще один абстрактний клас - основа майбутніх декорацій-обгорток
class ModifiedUnit : public AbstractUnit
{
private:
	AbstractUnit * wrappee;
public:
	//декоратор приховує всередині обгорнутий "справжній" об'єкт
	//і перенаправляє йому запити щось зробити
	ModifiedUnit(AbstractUnit* inner) : wrappee(inner) 
	{}

	~ModifiedUnit()
	{
		delete wrappee;
	}

	virtual void modified(int skill)
	{
		wrappee->modified(skill);
	}
};

// конкретні реалізації обгорток: спочатку працює вкладений об'єкт, потім - обгортка
class Armored : public ModifiedUnit //броньований
{
public:
	Armored(AbstractUnit* core) : ModifiedUnit(core)
	{}


	virtual void modified(int skill)
	{
		ModifiedUnit::modified(skill + 1);
	}
};

class Accelerated : public ModifiedUnit // прискорений
{
public:
	Accelerated(AbstractUnit* core) : ModifiedUnit(core)
	{}


	virtual void modified(int skill)
	{
		ModifiedUnit::modified(skill + 2);
	}
};

class Looking : public ModifiedUnit  // влучний
{
public:
	Looking(AbstractUnit* core) : ModifiedUnit(core) 
	{}


	virtual void modified(int skill)
	{
		ModifiedUnit::modified(skill + 3);
	}
};

class Unit : public AbstractUnit
{
protected:

	int hp;
	int dmg;
	int dodge;

public:

	// атака юнітом іншого юінта
	virtual void attack(Unit & unit)
	{
		if (unit.evading())
		{
			unit.setHp(unit.getHp() - this->getDmg());
			if (unit.getHp() < 0)
				unit.setHp(0);
			cout << " DAMAGED on " << this->getDmg() << ", his ";
			if (unit.die())
				cout << "hit-point = 0, he DIED x_x\n";
			else
				cout << "hit-point = " << unit.getHp() << endl;
		}
		else
		{
			cout << " EVADED. His hit-point = " << unit.getHp() << endl;
		}
		cout << endl;
	}

	//захист юніта при атаці
	virtual bool evading() const
	{
		int tmp = rand() % 100;
		if (tmp < this->getDodge())
			return true;
		else
			return false;

	}

	//перевірка чи живий юніт
	virtual bool die()
	{
		if (this->getHp() <= 0)
			return 1;
		else
			return 0;
	}

	void setDmg(int dmg)
	{
		this->dmg = dmg;
	}

	void setDodge(int dodge)
	{
		this->dodge = dodge;
	}

	void setHp(int hp)
	{
		this->hp = hp;
	}

	int getDmg() const
	{
		return dmg;
	}

	int getDodge() const
	{
		return dodge;
	}

	int getHp() const
	{
		return hp;
	}

	//вивід скіллів юніта
	void showUnit()
	{
		cout << "HP: " << hp << "; Dmgs: " << dmg << "; Dodge: " << dodge << "%\n";
	}

	void modified(int skill)
	{
		switch (skill)
		{
		case 1:
			setHp(getHp() * 2);
			break;
		case 2:
			setDmg(getDmg() * 1.5);
			break;
		case 3:
			setDodge(getDodge() * 1.5);
			break;
		default:
			break;
		}
	}
	

};

//юніт мечник
class Swordsman : public Unit
{
public:

	Swordsman()
	{
		hp = 15;
		dmg = 5;
		dodge = 60;
	}
};

//юніт лучник
class Archer : public Unit
{
public:

	Archer()
	{
		hp = 12;
		dmg = 4;
		dodge = 40;
	}

};

//юніт маг
class Mage : public Unit
{
public:

	Mage()
	{
		hp = 8;
		dmg = 10;
		dodge = 30;
	}
};

class Team
{
protected:

	string name;
	vector<Unit*> team{};

public:

	//наповнення команди випадковими типами юнітів
	Team(string name = "without name")
	{
		setName(name);

		for (size_t i = 0; i < teamPlayers; ++i)
		{
			int tmp = rand() % teamPlayers;
			switch (tmp)
			{
			case 0:
			{
				team.push_back(new Swordsman());
				break;
			}
			case 1:
			{
				team.push_back(new Archer());
				break;
			}
			case 2:
			{
				team.push_back(new Mage());
				break;
			}
			default:
			{}
			}
		}
	}

	string getName() const
	{
		return name;
	}

	void setName(string name)
	{
		this->name = name;
	}

	Unit* getUnit(int i)
	{
		return team[i];
	}

	//вивід сформованої команди
	void showTeam()
	{
		cout << "-=-=-=-=-=-=-=-\nTeam: " << name << endl << "-=-=-=-=-=-=-=-\n";
		for (size_t i = 0; i < team.size(); ++i)
		{
			if (typeid(Archer) == typeid(*team[i]))
				cout << "Archer";
			if (typeid(Swordsman) == typeid(*team[i]))
				cout << "Swordsman";
			if (typeid(Mage) == typeid(*team[i]))
				cout << "Mage";
			cout << i + 1 << "   \t";
			team[i]->showUnit();
		}
		cout << "- - - - - - - -\n";
	}

	//перевірка команди на наявність живих юнітів
	bool auditDie()
	{
		int count = 0;
		for (size_t i = 0; i < team.size(); ++i)
			if (!team[i]->die())
				++count;
		if (count == 0)
			return 1;
		else
			return 0;
	}

};

class Game
{

protected:
	Team first, second;

public:

	//перебіг бою
	void fight()
	{
		bool game = true;
		first.setName("Red");
		second.setName("Blue");
		first.showTeam();
		second.showTeam();
		cout << "Press 'Space' for continue\n\n";
		do
		{
			if (_kbhit())
			{
				switch (_getch())
				{
				case 32:
				{
					{
						for (size_t i = 0; i < teamPlayers; ++i)
						{
							//атака юніта першої команди
							if (!first.getUnit(i)->die())
							{
								bool type = 0;
								for (size_t j = 0; j < teamPlayers; ++j)
									if (getType(first, i) == getType(second, j) && !second.getUnit(j)->die())
									{
										showAttack(first, second, i, j);
										type = 1;
										break;
									}
								if (!type)
									for (size_t j = 0; j < teamPlayers; ++j)
										if (!second.getUnit(j)->die())
										{
											showAttack(first, second, i, j);
											break;
										}

							}
							//атака юніта другої команди
							if (!second.getUnit(i)->die())
							{
								bool type = 0;
								for (size_t j = 0; j < teamPlayers; ++j)
									if (getType(second, i) == getType(first, j) && !first.getUnit(j)->die())
									{
										showAttack(second, first, i, j);
										type = 1;
										break;
									}
								if (!type)
									for (size_t j = 0; j < teamPlayers; ++j)
										if (!first.getUnit(j)->die())
										{
											showAttack(second, first, i, j);
											break;
										}
							}

						}
						//перевірка чи є команда в якій повністю усі юніти загинули
						// якщо так - вивід результатів і оголошення переможця
						if (first.auditDie() || second.auditDie())
						{
							cout << "\n\n- = END of GAME = -\n\n";
							showResult();
							if (first.auditDie())
							{
								setColor(11);
								cout << "\nTeam \"" << second.getName() << "\" Win!\n";
								setColor();
							}
							else
							{
								setColor(12);
								cout << "\nTeam \"" << first.getName() << "\" Win!\n";
								setColor();
							}
							game = false;
						}
						else
							showResult();
					};
					break;
				}

				case 27:
				{
					game = false;
					break;
				}

				default:
				{
				}
				}
			}


		} while (game);
	}

	//вивід інфо по юніту
	string unitHP(Team team, int i, bool hp = 1, bool name = 1)
	{
		string tmpHp = "";
		string tmpName = "";

		if (hp)
			tmpHp = "(" + to_string(team.getUnit(i)->getHp()) + ")";

		if (name)
			tmpName = team.getName() + ":";

		return tmpName + (typeid(*team.getUnit(i)).name() + 6) + to_string(i + 1) + tmpHp;
	}

	//визначення типу юніта
	string getType(Team team, int i)
	{
		return (typeid(*team.getUnit(i)).name() + 6);
	}

	//текстове відображення бою, хто кого атакує і які наслідки
	void showAttack(Team first, Team second, int i, int j)
	{
		int red = 12;
		int blue = 11;

		if (first.getName() == "Red")
		{
			red = 12;
			blue = 11;
		}
		else
		{
			red = 11;
			blue = 12;
		}
		setColor(red);
		cout << unitHP(first, i);
		setColor();
		cout << " attack ";
		setColor(blue);
		cout << unitHP(second, j) << endl;
		cout << unitHP(second, j, 0);
		setColor();
		first.getUnit(i)->attack(*second.getUnit(j));
		(_getch());
	}

	//вивід результатів по завершенні раудну бою
	void showResult()
	{
		LINE
			cout << "Team \"" << first.getName() << "\"\t\tTeam \"" << second.getName() << "\"\n";
		cout << "----------\t\t---------\n";
		for (int i = 0; i < teamPlayers; ++i)
			cout << unitHP(first, i, 1, 0) << "\t\t" << unitHP(second, i, 1, 0) << endl;
		LINE
			cout << "Press 'Space' for continue or 'Esc' for exit\n\n";

	}

	//колір тексту
	void setColor(int color = 7)
	{
		HANDLE hStdOut = GetStdHandle(STD_OUTPUT_HANDLE);
		SetConsoleTextAttribute(hStdOut, color);
	}
};




int main()
{
	Mage mage;
	cout << "Usual Mage:\t";
	mage.showUnit(); // hp = 12 dmg = 10, dodge = 30%

	AbstractUnit* unitLkAcAr = new Armored(&mage);
	unitLkAcAr->modified(0); // hp = 24
	unitLkAcAr = new Looking(&mage);
	unitLkAcAr->modified(0);  // dmg = 15
	unitLkAcAr = new Accelerated(&mage);
	unitLkAcAr->modified(0); // doodge = 45%
	cout << "Modified Mage:\t"; 
	mage.showUnit();


	Archer archer;
	cout << "\nUsual Archer:\t"; // hp = 12
	archer.showUnit();
	AbstractUnit* unitArmored = new Armored(&archer);
	cout << "Armored Archer:\t"; // hp = 24
	unitArmored->modified(0); 
	archer.showUnit();


	//srand(unsigned(time(0)));
	//Game g;
	//g.fight();

	cout << endl;
	system("pause");
	return 0;

}






