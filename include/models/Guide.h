#ifndef PRACTICA_COLLEGE_GUIDE_H
#define PRACTICA_COLLEGE_GUIDE_H
#include "Person.h"
#include "Date.h"
#include <string>
#include <cstdint>
#include <vector>

class Guide : public Person
{
public:
Guide(int32_t id, const std::string& name, const std::string& phone,
        const std::vector<std::string>& languages, int32_t rating);
const std::vector<std::string>& getLanguages() const;

void setLanguages(const std::string& language);

bool speaks(const std::string& language) const;

int32_t getRating() const;

bool setRating(double newRating);
bool isAvailable(const Date& date) const;
bool reserve(const Date& date);

void release(const Date& date);

const std::vector<Date>& getBusyDates() const;

std::string getInfo() const override;




private:
    std::vector<std::string> languages;
    std::vector<Date> busyDates;

    int32_t rating = 0;
    
};


#endif
