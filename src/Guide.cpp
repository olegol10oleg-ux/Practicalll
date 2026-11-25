#include "../include/models/Guide.h"
#include <string>
#include <vector>
#include <cstdint>

Guide::Guide(int32_t id, const std::string& name, const std::string& phone,
             const std::vector<std::string>& languages, int32_t rating)
    : Person(id, name, phone)
{
    this->languages = languages;
    if (rating >= 0 && rating <= 500)
    {
        this->rating = rating;
    }
    else
    {
        this->rating = 0.0;
    }
}

const std::vector<std::string>& Guide::getLanguages() const
{
    return languages;
}

void Guide::setLanguages(const std::string& language)
{
    if (!speaks(language))
    {
        languages.emplace_back(language);
    }
}

bool Guide::speaks(const std::string& language) const
{
    for (const auto& i : languages)
    {
        if (i == language)
        {
            return true;
        }
    }
    return false;
}
int32_t Guide::getRating() const
{
    return rating;
}

bool Guide::setRating(double newRating)
{
    if (newRating < 0.0 || newRating > 5.0)
    {
        return false;
    }
    rating = newRating;
    return true;
}

bool Guide::isAvailable(const Date& date) const
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
bool Guide::reserve(const Date& date)
{
    if (!isAvailable(date))
    {
        return false;
    }
    busyDates.emplace_back(date);
    return true;
}
void Guide::release(const Date& date)
{
    for (size_t i = 0; i < busyDates.size(); ++i)
    {
        if (busyDates[i] == date)
        {
            for (size_t j = i; j + 1 < busyDates.size(); ++j)
            {
                busyDates[j] = busyDates[j + 1];
            }
            busyDates.pop_back();
            return;
        }
    }
}
const std::vector<Date>& Guide::getBusyDates() const
{
    return busyDates;
}

std::string Guide::getInfo() const
{
    std::string cents = std::to_string(rating % 100);
    if (rating % 100 < 10)
    {
        cents = "0" + cents;
    }

    std::string result = "Guide" + std::to_string(id) + ":" + name + "phone:"
    + phone + "rating:" + std::to_string(rating / 100) + "." + cents + "languages:";
    for (size_t i = 0; i < languages.size(); ++i)
    {
        result += languages[i];
        if (i + 1 < languages.size())
        {
            result += ", ";
        }
    }
    return result;
}