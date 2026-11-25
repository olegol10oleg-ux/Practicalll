#ifndef PRACTICA_COLLEGE_GUIDE_H
#define PRACTICA_COLLEGE_GUIDE_H
#include "Person.h"
#include "Date.h"
#include <string>
#include <cstdint>
#include <vector>

class Guide :public Person
{
public:


private:
    std::vector<std::string> languages;
    std::vector<Date> busyDates;
    
    
};


#endif
