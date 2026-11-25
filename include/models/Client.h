#ifndef PRACTICA_COLLEGE_CLIENT_H
#define PRACTICA_COLLEGE_CLIENT_H
#include "Person.h"
#include <string>
#include <list>



class Client : public Person
{
public:
    Client(int32_t id, const std::string& name, const std::string& phone, const std::string& email);

    const std::string& getEmail() const;

    void setEmail(const std::string& email);

    std::string getInfo() const override;



private:
    std::string email;
    

};


#endif
