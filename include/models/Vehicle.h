#ifndef PRACTICA_COLLEGE_VEHICLE_H
#define PRACTICA_COLLEGE_VEHICLE_H
#include "Date.h"
#include <vector>
class Vehicle
{
public:
    Vehicle(int32_t id, const std::string& model, int32_t capacity);
     int32_t getId() const;
    const std::string& getModel() const;
    int32_t getCapacity() const;

    bool canFit(int32_t groupSize) const;
    bool isAvailable(const Date& date) const;
    bool reserve(const Date& date);

    void release(const Date& date);

    const std::vector<Date>& getBusyDates() const;
    std::string getInfo() const;
private:
    int32_t id = 0;
    std::string model;
    int32_t capacity = 0;
    std::vector<Date> busyDates;
};


#endif
