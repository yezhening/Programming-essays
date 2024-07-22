#include<iostream>
#include <string>
using namespace std;

class Animal
{
private:
    string type;
    string color;
public:
    friend class Fish;
    friend class Bird;
    Animal(string t,string c)
    {
        type = t;
        color = c; 
    }
    virtual void Print() = 0;
};
class Fish :public Animal
{
private:
    int Osteichthyes;
public:
    Fish(string t, string c,int o):Animal(t, c)
    {
        Osteichthyes = o;
    }
    virtual void Print()
    {
        cout << "type: " << Animal::type << ", color: " << Animal::color << ", Osteichthyes: " << Osteichthyes << endl;
    }
};
class Bird :public Animal
{
private:
    int daytime;
public:
    Bird(string t, string c, int d) :Animal(t, c)
    {
        daytime = d;
    }
    virtual void Print()
    {
        cout << "type: " << Animal::type << ", color: " << Animal::color << ", daytime: " << daytime << endl;
    }
};

int main()
{
    Animal* animal;
    string type, color;
    bool Osteichthyes, daytime;
    cin >> type >> color >> Osteichthyes;
    Fish fish(type, color, Osteichthyes);
    fish.Print();
    animal = &fish;
    animal->Print();
    cin >> type >> color >> daytime;
    Bird bird(type, color, daytime);
    bird.Print();
    animal = &bird;
    animal->Print();
    return 0;
}


