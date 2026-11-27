#ifndef PRACTICA_COLLEGE_TOURSERVICE_H
#define PRACTICA_COLLEGE_TOURSERVICE_H
#include "../models/Booking.h"
#include "../models/Client.h"
#include "../models/Excursion.h"
#include "../models/Group.h"
#include "../models/Guide.h"
#include "../models/Vehicle.h"
#include <cstdint>
#include <memory>
#include <queue>
#include <stack>
#include <string>
#include <vector>

class WaitEntry
{
public:
    Client *client;
    Excursion *excursion;
    int32_t seats;
};

class TourService
{
public:
    Client *addClient(const std::string &name, const std::string &phone, const std::string &email);
    Guide *addGuide(const std::string &name, const std::string &phone,
        const std::vector<std::string> &languages, int32_t rating);
    Vehicle *addVehicle(const std::string &model, int32_t capacity);
    Excursion *addExcursion(const std::string &title, const Date &date,
        int32_t price, int32_t maxSeats, int32_t minSeats);

    Client *findClient(int32_t id) const;
    Guide *findGuide(int32_t id) const;
    Vehicle *findVehicle(int32_t id) const;
    Excursion *findExcursion(int32_t id) const;
    Booking *findBooking(int32_t id) const;
    Group *findGroup(int32_t id) const;
    Group *findGroupFor(const Excursion *excursion) const;

    const std::vector<std::unique_ptr<Client>> &getClients() const;
    const std::vector<std::unique_ptr<Guide>> &getGuides() const;
    const std::vector<std::unique_ptr<Vehicle>> &getVehicles() const;
    const std::vector<std::unique_ptr<Excursion>> &getExcursions() const;
    const std::vector<std::unique_ptr<Booking>> &getBookings() const;
    const std::vector<std::unique_ptr<Group>> &getGroups() const;

    Booking *bookExcursion(Client *client, Excursion *excursion, int32_t seats);
    bool cancelBooking(int32_t bookingId);

    std::vector<Guide *> findAvailableGuides(const Date &date, const std::string &language) const;
    std::vector<Vehicle *> findSuitableVehicles(const Date &date, int32_t seats) const;

    bool assignGuide(Group *group, Guide *guide, const std::string &language);
    bool autoAssignGuide(Group *group, const std::string &language);
    bool assignVehicle(Group *group, Vehicle *vehicle);
    bool autoAssignVehicle(Group *group);
    bool markReady(Group *group);
    bool startGroup(Group *group);
    bool completeGroup(Group *group);
    bool guideRefuse(Group *group);
    bool cancelGroup(Group *group, const std::string &reason);
    bool cancelGroupIfNotEnoughSeats(Group *group);

    std::vector<WaitEntry> getWaitList() const;
    std::vector<std::string> getRecentLog(size_t count) const;
    const std::string &getLastMessage() const;

private:
    Booking *createBooking(Client *client, Excursion *excursion, int32_t seats);
    void processWaitList(Excursion *excursion);
    void clearWaitList(Excursion *excursion);
    bool groupStep(bool ok, Group *group, const std::string &successText);
    bool fail(const std::string &message);
    void log(const std::string &text);

    std::vector<std::unique_ptr<Client>> clients;
    std::vector<std::unique_ptr<Guide>> guides;
    std::vector<std::unique_ptr<Vehicle>> vehicles;
    std::vector<std::unique_ptr<Excursion>> excursions;
    std::vector<std::unique_ptr<Booking>> bookings;
    std::vector<std::unique_ptr<Group>> groups;

    std::queue<WaitEntry> waitList;
    std::stack<std::string> history;

    int32_t nextClientId = 1;
    int32_t nextGuideId = 1;
    int32_t nextVehicleId = 1;
    int32_t nextExcursionId = 1;
    int32_t nextBookingId = 1;
    int32_t nextGroupId = 1;

    std::string lastMessage;
};

#endif