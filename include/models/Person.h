#ifndef PRACTICA_COLLEGE_PERSON_H
#define PRACTICA_COLLEGE_PERSON_H
#include <cstdint>
#include <string>

class Person
{
public:
    Person(int32_t id, const std::string& name, const std::string& phone);
       virtual ~Person() = default;

    int32_t getId() const;
    const std::string& getName() const;
    const std::string& getPhone() const;

    void setName(const std::string &newName);
    void setPhone(const std::string& newPhone);

    virtual std::string getInfo() const = 0;

protected:
    int32_t id;
    std::string name;
    std::string phone;
};


#endif
