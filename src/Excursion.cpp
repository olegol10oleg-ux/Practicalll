#include "../include/models/Excursion.h"
#include <string>
#include <cstdint>

Excursion::Excursion(int32_t id, const std::string& tittle, const Date& date, int32_t price, int32_t maxSeats, int32_t minSeats)
    : id(id), tittle(tittle), date(date)
{
    if (price >= 0)
    {
        this->price = price;
    }
    else
    {
        this->price = 0;
    }
    if (maxSeats > 0)
    {
        this->maxSeats = maxSeats;
    }
    else
    {
        this->maxSeats = 0;
    }
    if (minSeats >= 0 && minSeats <= this->maxSeats)
    {
        this->minSeats = minSeats;
    }
    else
    {
        this->minSeats = 0;
    }
}
int32_t Excursion::getId() const
{
    return id;
}
const std::string& Excursion::getTittle() const
{
    return tittle;
}
const Date& Excursion::getDate() const
{
    return date;
}
int32_t Excursion::getPrice() const
{
    return price;
}
int32_t Excursion::getMaxSeats() const
{
    return maxSeats;
}
int32_t Excursion::getMinSeats() const
{
    return minSeats;
}
int32_t Excursion::getBookedSeats() const
{
    return bookedSeats;
}
int32_t Excursion::freeSeats() const
{
    return maxSeats - bookedSeats;
}
bool Excursion::hasEnoughSeats(int32_t n) const
{
    return n > 0 && n <= freeSeats();
}

bool Excursion::isMinReached() const
{
    return bookedSeats >= minSeats;
}
bool Excursion::bookSeats(int32_t n)
{
    if (!hasEnoughSeats(n))
    {
        return false;
    }
    bookedSeats += n;
    return true;
}
bool Excursion::releaseSeats(int32_t n)
{
    if (n <= 0 || n > bookedSeats)
    {
        return false;
    }
    bookedSeats -= n;
    return true;
}
std::string Excursion::getPriceString() const
{
    std::string cents = std::to_string(price % 100);
    if (price % 100 < 10)
    {
        cents = "0" + cents;
    }
    return std::to_string(price / 100) + "." + cents;
}
std::string Excursion::getInfo() const
{
    return "Excursion #" + std::to_string(id) + ": " + tittle + ", date: " + date.toString() +
        ", price: " + getPriceString() + ", seats: " + std::to_string(bookedSeats) + "/" +
        std::to_string(maxSeats) + " (min " + std::to_string(minSeats) + ")";
}