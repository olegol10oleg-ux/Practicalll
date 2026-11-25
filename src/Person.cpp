#include "../include/models/Person.h"
#include <string>
#include <cstdint>
Person::Person(int32_t id, const std::string& name, const std::string& phone)
: id(id), name(name), phone(phone)
{}

int32_t Person::getId() const
{
    return id;
}
const std::string& Person::getName() const
{
    return name;
}
const std::string& Person::getPhone() const
{
    return phone;
}

void Person::setName(const std::string& newName)
{
    name = newName;
}
void Person::setPhone(const std::string& newPhone)
{
    phone = newPhone;
}
