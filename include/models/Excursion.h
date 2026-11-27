#ifndef PRACTICA_COLLEGE_EXCURSION_H
#define PRACTICA_COLLEGE_EXCURSION_H
#include "Date.h"
#include <string>
#include <cstdint>


class Excursion
{
public:
    Excursion(int32_t id, const std::string& tittle, const Date& date, int32_t price, int32_t maxSeats, int32_t minSeats);
    int32_t getId() const;
    const std::string& getTittle() const;
    const Date& getDate() const;
    int32_t getPrice() const;
    int32_t getMaxSeats() const;
    int32_t getMinSeats() const;
    int32_t getBookedSeats() const;

    int32_t freeSeats() const;

    bool hasEnoughSeats(int32_t n) const;
    bool isMinReached() const;
    bool bookSeats(int32_t n);
    bool releaseSeats(int32_t n);

    std::string getPriceString() const;
    std::string getInfo() const;


private:
   int32_t id = 0;
    std::string tittle;
    Date date;
    int32_t price = 0;
    int32_t maxSeats = 0;
    int32_t minSeats = 0;
    int32_t bookedSeats = 0;



};


#endif
