#include "../include/models/Client.h"


Client::Client(int32_t id, const std::string& name, const std::string& phone, const std::string& email)
    : Person(id, name, phone), email(email) {}

const std::string& Client::getEmail() const
{
    return email;
}

void Client::setEmail(const std::string& newEmail)
{
    email = newEmail;
}

std::string Client::getInfo() const
{
    return "Client #" + std::to_string(id) + ": " + name + "phone: " + phone + "email: " + email;
}