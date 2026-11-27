#include "../include/models/Booking.h"
#include <cstdint>


Booking::Booking(int32_t id, Client* client, Excursion* excursion, int32_t seatsCount)
    : id(id), client(client), excursion(excursion)
{
    if (client != nullptr && excursion != nullptr && excursion->bookSeats(seatsCount))
    {
        this->seatsCount = seatsCount;
        status = PENDING;
    }
    else
    {
        this->seatsCount = 0;
        status = CANCELLED;
    }
}

int32_t Booking::getId() const
{
    return id;
}

Client* Booking::getClient() const
{
    return client;
}

Excursion* Booking::getExcursion() const
{
    return excursion;
}

int32_t Booking::getSeatsCount() const
{
    return seatsCount;
}

BookingStatus Booking::getStatus() const
{
    return status;
}

bool Booking::isActive() const
{
    return status == PENDING || status == CONFIRMED;
}

int32_t Booking::getTotalPrice() const
{
    if (excursion == nullptr)
    {
        return 0;
    }
    return excursion->getPrice() * seatsCount;
}

bool Booking::confirm()
{
    if (status != PENDING)
    {
        return false;
    }
    status = CONFIRMED;
    return true;
}

bool Booking::cancel()
{
    if (status == CANCELLED)
    {
        return false;
    }
    if (excursion != nullptr && seatsCount > 0)
    {
        excursion->releaseSeats(seatsCount);
    }
    status = CANCELLED;
    return true;
}

std::string Booking::getStatusString() const
{
    if (status == PENDING)
    {
        return "PENDING";
    }
    if (status == CONFIRMED)
    {
        return "CONFIRMED";
    }
    return "CANCELLED";
}

std::string Booking::getInfo() const
{
    std::string total = std::to_string(getTotalPrice() / 100) + ".";
    int32_t cents = getTotalPrice() % 100;
    if (cents < 10)
    {
        total += "0";
    }
    total += std::to_string(cents);

    std::string clientName = "-";
    if (client != nullptr)
    {
        clientName = client->getName();
    }

    std::string excursionTitle = "-";
    if (excursion != nullptr)
    {
        excursionTitle = excursion->getTittle();
    }

    return "Booking #" + std::to_string(id) +
           ", client: " + clientName +
           ", excursion: " + excursionTitle +
           ", seats: " + std::to_string(seatsCount) +
           ", total: " + total +
           ", status: " + getStatusString();
}