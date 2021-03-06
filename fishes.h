#ifndef FISHES_H
#define FISHES_H

#include "waterobject.h"
#include "waterplants.h"
#include "databaseinterface.h"
#include <random>

//A halak táplálkozási szokásai
enum class SPECIES {Carnivorous, Herbivorous, Omnivorous}; //Carnivorous = Húsevő | Herbivorous = Növényevő | Omnivorous = Mindenevő

//8 irányba tudnak elmozdulni a halak
enum class DIRECTION {TopLeft, Top, TopRight, Right, BottomRight, Bottom, BottomLeft, Left};


//A fő Fish osztály. Ebből lehet további speciális osztályokat származtatni. Az abstract factory ilyen típussal tér vissza

class Fish : public WaterObject
{

#ifdef WITH_SQL
private:
    static DatabaseInterface* dataBase;
    void connectToTheDataBase();
#endif

private:
    bool _moved = false;
    int _xSize;
    int _ySize;
    std::mt19937 _randomEngine;
    std::vector<std::vector<WaterObject*>> *_grid;
    WaterObject* _target = nullptr;
    unsigned char _size;
    SPECIES _species;

    //Hogy beazonosítsuk a halat
    static int counter;
    int _id;


    //Private Move Function
    void randomMove();
    bool moveXY(WaterObject *space, Point2D point);
    Point2D generateNewPointCoordinate(DIRECTION direction);

public:
    static int currentTime;

    //Constructors
    Fish() = delete;
    virtual ~Fish();
    Fish(Point2D position, unsigned char size, SPECIES spec);
    Fish(int x, int y, unsigned char size, SPECIES spec);

    //Functions
    void SetMoved(bool value);
    bool Moved() const;
    void setTheMapGrid(std::vector<std::vector<WaterObject *> > *grid);
    void SetTarget(WaterObject* fish);
    WaterObject* ShowTarget() const ;
    SPECIES GetSpecies() const;
    unsigned char GetSize() const;
    void lookAround(WaterObject* vector[]);
    Point2D lookAroundForTheFood(WaterObject* vector[]);
    void Move();
    void Eat(WaterObject *);

};

//Minden halat amit egy txt fájlból olvas be SpecialFish típusú hal lesz
class SpecialFish final : public Fish
{
private:
    std::wstring _speciesName;

public:
    SpecialFish() = delete;
    SpecialFish(Point2D position, unsigned char size, SPECIES spec,  std::wstring speciesName);
    SpecialFish(int x, int y,  unsigned char size, SPECIES spec, std::wstring speciesName);

    //Public functions
    const std::wstring &GetSpeciesName() const;
};

//További hal osztályok

//Herbivorous
class CrucianCarp final : public Fish // Crucian carp = Kárász - Növényevő
{

public:
    CrucianCarp() = delete;
    CrucianCarp(Point2D position, unsigned char size);
    CrucianCarp(int x, int y, unsigned char size);
};

//Carnivorous
class NorthernPike final : public Fish // Northern pike = Csuka - Húsevő
{
public:
    NorthernPike() = delete;
    NorthernPike(Point2D position, unsigned char size);
    NorthernPike(int x, int y, unsigned char size);
};

//Herbivorous
class EuropeanChub final : public Fish // European Chub = Fejes domolykó - Növényevő
{
public:
    EuropeanChub() = delete;
    EuropeanChub(Point2D position, unsigned char size);
    EuropeanChub(int x, int y, unsigned char size);
};

#endif // FISHES_H
