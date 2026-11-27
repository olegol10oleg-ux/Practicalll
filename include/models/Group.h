#ifndef PRACTICA_COLLEGE_GROUP_H
#define PRACTICA_COLLEGE_GROUP_H
#include "Excursion.h"
#include "Vehicle.h"
#include "Booking.h"
#include "Guide.h"
#include <string>
#include <cstdint>
#include <vector>

enum class GroupStatus
{
    FORMING,
    GUIDE_ASSIGNED,
    TRANSPORT_ASSIGNED,
    READY,
    IN_PROGRESS,
    COMPLETED,
    CANCELLED,
};

class Group
{
public:
    Group(int32_t id, Excursion *excursion);

    int32_t getId() const;
    Excursion *getExcursion() const;
    Guide *getGuide() const;
    Vehicle *getVehicle() const;
    GroupStatus getStatus() const;
    const std::vector<Booking *> &getBookings() const;

    int32_t getTotalSeats() const;
    const std::string &getLastError() const;
    const std::string &getCancelReason() const;

    bool addBooking(Booking *booking);
    bool removeBooking(int32_t bookingId);
    bool assignGuide(Guide *guide, const std::string &language = "");
    bool guideRefuse();
    bool assignVehicle(Vehicle *vehicle);
    bool markReady();
    bool start();
    bool complete();
    bool cancel(const std::string &reason);
    bool cancelIfNotEnoughSeats();

    std::string getStatusString() const;
    std::string getInfo() const;

private:
    bool fail(const std::string &message);
    void releaseResources();

    int32_t id = 0;
    Excursion *excursion = nullptr;
    Guide *guide = nullptr;
    Vehicle *vehicle = nullptr;
    std::vector<Booking *> bookings;
    GroupStatus status = GroupStatus::FORMING;
    std::string lastError;
    std::string cancelReason;
};


#endif
