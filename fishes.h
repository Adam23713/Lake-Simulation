#ifndef FISHES_H
#define FISHES_H

#include "waterobject.h"

enum class SPECIES {Carnivorous, Herbivorous, Omnivorous}; //Carnivorous = Húsevő | Herbivorous = Növényevő | Omnivorous = Mindenevő

class Fish : public WaterObject
{

private:
    Fish* _target;
    unsigned char _size;
    SPECIES _species;

public:
    //Constructors
    Fish() = delete;
    Fish(Point2D position, unsigned char size, SPECIES spec);
    Fish(int x, int y, unsigned char size, SPECIES spec);

    //Functions
    void SetTarget(Fish* fish);
    Fish* ShowTarget() const ;
    SPECIES GetSpecies() const;
    unsigned char GetSize() const;
    void Move();
    void Eat(WaterObject&);

};

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
