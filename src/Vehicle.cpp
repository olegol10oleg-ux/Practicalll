#include "../include/models/Vehicle.h"

#include <vector>
#include <string>
#include <cstdint>

Vehicle::Vehicle(int32_t id, const std::string& model, int32_t capacity)
    : id(id), model(model), capacity(capacity)
{
    this->id = id;
    this->model = model;
    if (capacity > 0)
    {
        this->capacity = capacity;
    }
    else
    {
        this->capacity = 0;
    }
}
int32_t Vehicle::getId() const
{
    return id;
}

const std::string& Vehicle::getModel() const
{
    return model;
}

int32_t Vehicle::getCapacity() const
{
    return capacity;
}
bool Vehicle::canFit(int32_t groupSize) const
{
    return groupSize > 0 && groupSize <= capacity;
}

bool Vehicle::isAvailable(const Date& date) const
{
    for (const auto& x : busyDates)
    {
        if (x == date)
        {
            return false;
        }
    }
    return true;
}
bool Vehicle::reserve(const Date& date)
{
    if (isAvailable(date))
    {
        return false;
    }
    busyDates.emplace_back(date);
    return true;
}
void Vehicle::release(const Date& date)
{
    for (size_t i = 0; i < busyDates.size(); ++i)
    {
        if (busyDates[i] == date)
        {
            for (size_t j = i; j < busyDates.size(); ++j)
            {
                busyDates[j] = busyDates[j + 1];
            }
            busyDates.pop_back();
            return;
        }
    }
}
const std::vector<Date>& Vehicle::getBusyDates() const
{
    return busyDates;
}
std::string Vehicle::getInfo() const
{
    return "Vehicle:" + std::to_string(id) + ":" + model + "capacity:" + std::to_string(capacity);
}