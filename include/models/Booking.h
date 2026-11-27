#ifndef PRACTICA_COLLEGE_BOOKING_H
#define PRACTICA_COLLEGE_BOOKING_H
#include "Client.h"
#include "Excursion.h"
#include <string>
#include <cstdint>

enum BookingStatus
{
    PENDING,
    CONFIRMED,
    CANCELLED,
};

class Booking
{
public:
    Booking(int32_t id, Client *client, Excursion *excursion, int32_t seatsCount);

    int32_t getId() const;
    Client *getClient() const;
    Excursion *getExcursion() const;
    int32_t getSeatsCount() const;
    BookingStatus getStatus() const;

    bool isActive() const;
    int32_t getTotalPrice() const;

    bool confirm();
    bool cancel();

    std::string getInfo() const;
    std::string getStatusString() const;
private:
    int32_t id = 0;
    Client *client = nullptr;
    Excursion *excursion = nullptr;
    int32_t seatsCount = 0;
    BookingStatus status = PENDING;
};

#endif
