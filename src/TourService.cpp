#include "../include/services/TourService.h"

static bool isClosedForBookings(const Group *group)
{
    GroupStatus s = group->getStatus();
    return s == GroupStatus::READY || s == GroupStatus::IN_PROGRESS || s == GroupStatus::COMPLETED;
}


static void sortGuidesByRating(std::vector<Guide *> &items)
{
    for (size_t i = 1; i < items.size(); ++i)
    {
        Guide *current = items[i];
        size_t j = i;
        while (j > 0 && items[j - 1]->getRating() < current->getRating())
        {
            items[j] = items[j - 1];
            --j;
        }
        items[j] = current;
    }
}


static void sortVehiclesByCapacity(std::vector<Vehicle *> &items)
{
    for (size_t i = 1; i < items.size(); ++i)
    {
        Vehicle *current = items[i];
        size_t j = i;
        while (j > 0 && items[j - 1]->getCapacity() > current->getCapacity())
        {
            items[j] = items[j - 1];
            --j;
        }
        items[j] = current;
    }
}
bool TourService::fail(const std::string &message)
{
    lastMessage = message;
    return false;
}

void TourService::log(const std::string &text)
{
    history.push(text);
}

const std::string &TourService::getLastMessage() const
{
    return lastMessage;
}

std::vector<std::string> TourService::getRecentLog(size_t count) const
{
    std::stack<std::string> copy = history;
    std::vector<std::string> result;
    while (!copy.empty() && result.size() < count)
    {
        result.push_back(copy.top());
        copy.pop();
    }
    return result;
}
std::vector<WaitEntry> TourService::getWaitList() const
{
    std::queue<WaitEntry> copy = waitList;
    std::vector<WaitEntry> result;
    while (!copy.empty())
    {
        result.push_back(copy.front());
        copy.pop();
    }
    return result;
}
Client *TourService::addClient(const std::string &name, const std::string &phone, const std::string &email)
{
    if (name.empty())
    {
        fail("Client name cannot be empty");
        return nullptr;
    }
    clients.push_back(std::make_unique<Client>(nextClientId++, name, phone, email));
    Client *client = clients.back().get();
    lastMessage = "Client added: " + client->getInfo();
    log(lastMessage);
    return client;
}

Guide *TourService::addGuide(const std::string &name, const std::string &phone,
                             const std::vector<std::string> &languages, int32_t rating)
{
    if (name.empty())
    {
        fail("Guide name cannot be empty");
        return nullptr;
    }
    if (languages.empty())
    {
        fail("Guide must speak at least one language");
        return nullptr;
    }
    if (rating < 0 || rating > 500)
    {
        fail("Rating must be between 0 and 500 (480 = 4.80)");
        return nullptr;
    }
    guides.push_back(std::make_unique<Guide>(nextGuideId++, name, phone, languages, rating));
    Guide *guide = guides.back().get();
    lastMessage = "Guide added: " + guide->getInfo();
    log(lastMessage);
    return guide;
}

Vehicle *TourService::addVehicle(const std::string &model, int32_t capacity)
{
    if (model.empty())
    {
        fail("Vehicle model cannot be empty");
        return nullptr;
    }
    if (capacity <= 0)
    {
        fail("Capacity must be positive");
        return nullptr;
    }
    vehicles.push_back(std::make_unique<Vehicle>(nextVehicleId++, model, capacity));
    Vehicle *vehicle = vehicles.back().get();
    lastMessage = "Vehicle added: " + vehicle->getInfo();
    log(lastMessage);
    return vehicle;
}

Excursion *TourService::addExcursion(const std::string &title, const Date &date,
                                     int32_t price, int32_t maxSeats, int32_t minSeats)
{
    if (title.empty())
    {
        fail("Excursion title cannot be empty");
        return nullptr;
    }
    if (price < 0)
    {
        fail("Price cannot be negative");
        return nullptr;
    }
    if (maxSeats <= 0)
    {
        fail("Max seats must be positive");
        return nullptr;
    }
    if (minSeats < 0 || minSeats > maxSeats)
    {
        fail("Min seats must be between 0 and max seats");
        return nullptr;
    }
    excursions.push_back(std::make_unique<Excursion>(nextExcursionId++, title, date, price, maxSeats, minSeats));
    Excursion *excursion = excursions.back().get();
    lastMessage = "Excursion added: " + excursion->getInfo();
    log(lastMessage);
    return excursion;
}
Client *TourService::findClient(int32_t id) const
{
    for (const auto &item : clients)
    {
        if (item->getId() == id)
        {
            return item.get();
        }
    }
    return nullptr;
}
Guide *TourService::findGuide(int32_t id) const
{
    for (const auto &item : guides)
    {
        if (item->getId() == id)
        {
            return item.get();
        }
    }
    return nullptr;
}

Vehicle *TourService::findVehicle(int32_t id) const
{
    for (const auto &item : vehicles)
    {
        if (item->getId() == id)
        {
            return item.get();
        }
    }
    return nullptr;
}

Excursion *TourService::findExcursion(int32_t id) const
{
    for (const auto &item : excursions)
    {
        if (item->getId() == id)
        {
            return item.get();
        }
    }
    return nullptr;
}
Booking *TourService::findBooking(int32_t id) const
{
    for (const auto &item : bookings)
    {
        if (item->getId() == id)
        {
            return item.get();
        }
    }
    return nullptr;
}

Group *TourService::findGroup(int32_t id) const
{
    for (const auto &item : groups)
    {
        if (item->getId() == id)
        {
            return item.get();
        }
    }
    return nullptr;
}
Group *TourService::findGroupFor(const Excursion *excursion) const
{
    for (auto it = groups.rbegin(); it != groups.rend(); ++it)
    {
        if ((*it)->getExcursion() == excursion && (*it)->getStatus() != GroupStatus::CANCELLED)
        {
            return it->get();
        }
    }
    return nullptr;
}
const std::vector<std::unique_ptr<Client>> &TourService::getClients() const { return clients; }
const std::vector<std::unique_ptr<Guide>> &TourService::getGuides() const { return guides; }
const std::vector<std::unique_ptr<Vehicle>> &TourService::getVehicles() const { return vehicles; }
const std::vector<std::unique_ptr<Excursion>> &TourService::getExcursions() const { return excursions; }
const std::vector<std::unique_ptr<Booking>> &TourService::getBookings() const { return bookings; }
const std::vector<std::unique_ptr<Group>> &TourService::getGroups() const { return groups; }

Booking *TourService::createBooking(Client *client, Excursion *excursion, int32_t seats)
{
    Group *group = findGroupFor(excursion);
    if (group != nullptr && isClosedForBookings(group))
    {
        fail("Group #" + std::to_string(group->getId()) + " is already closed for new bookings");
        return nullptr;
    }

    bookings.push_back(std::make_unique<Booking>(nextBookingId++, client, excursion, seats));
    Booking *booking = bookings.back().get();
    if (!booking->isActive())
    {
        bookings.pop_back();
        fail("Not enough free seats");
        return nullptr;
    }

    bool newGroup = false;
    if (group == nullptr)
    {
        groups.push_back(std::make_unique<Group>(nextGroupId++, excursion));
        group = groups.back().get();
        newGroup = true;
    }
    if (!group->addBooking(booking))
    {
        std::string reason = group->getLastError();
        booking->cancel();
        bookings.pop_back();
        if (newGroup)
        {
            groups.pop_back();
        }
        fail(reason);
        return nullptr;
    }
    return booking;
}

Booking *TourService::bookExcursion(Client *client, Excursion *excursion, int32_t seats)
{
    if (client == nullptr || excursion == nullptr)
    {
        fail("Client or excursion not found");
        return nullptr;
    }
    if (seats <= 0)
    {
        fail("Seats count must be positive");
        return nullptr;
    }
    if (!excursion->hasEnoughSeats(seats))
    {
        Group *group = findGroupFor(excursion);
        if (group != nullptr && isClosedForBookings(group))
        {
            fail("Group #" + std::to_string(group->getId()) + " is already closed for new bookings");
            return nullptr;
        }
        waitList.push({client, excursion, seats});
        lastMessage = "Not enough seats (free: " + std::to_string(excursion->freeSeats()) +
                      "). " + client->getName() + " added to the waiting list";
        log(lastMessage);
        return nullptr;
    }

    Booking *booking = createBooking(client, excursion, seats);
    if (booking != nullptr)
    {
        lastMessage = "Booking created: " + booking->getInfo();
        log(lastMessage);
    }
    return booking;
}

bool TourService::cancelBooking(int32_t bookingId)
{
    Booking *booking = findBooking(bookingId);
    if (booking == nullptr)
    {
        return fail("Booking not found");
    }
    if (!booking->isActive())
    {
        return fail("Booking is already cancelled");
    }

    Excursion *excursion = booking->getExcursion();
    Group *group = findGroupFor(excursion);
    if (group != nullptr)
    {
        GroupStatus s = group->getStatus();
        if (s == GroupStatus::IN_PROGRESS || s == GroupStatus::COMPLETED)
        {
            return fail("Cannot cancel: the excursion has already started");
        }
        group->removeBooking(bookingId); // в закрытой (READY) группе бронь остается, но неактивна
    }

    booking->cancel();
    lastMessage = "Booking #" + std::to_string(bookingId) + " cancelled, seats released";
    log(lastMessage);
    processWaitList(excursion);
    return true;
}

void TourService::processWaitList(Excursion *excursion)
{
    std::string summary = lastMessage;
    size_t count = waitList.size();
    for (size_t i = 0; i < count; ++i)
    {
        WaitEntry entry = waitList.front();
        waitList.pop();
        if (entry.excursion == excursion && excursion->hasEnoughSeats(entry.seats))
        {
            Booking *booking = createBooking(entry.client, entry.excursion, entry.seats);
            if (booking != nullptr)
            {
                std::string text = "Waiting list: " + booking->getInfo();
                log(text);
                summary += " | " + text;
                continue;
            }
        }
        waitList.push(entry);
    }
    lastMessage = summary;
}

void TourService::clearWaitList(Excursion *excursion)
{
    size_t count = waitList.size();
    for (size_t i = 0; i < count; ++i)
    {
        WaitEntry entry = waitList.front();
        waitList.pop();
        if (entry.excursion != excursion)
        {
            waitList.push(entry);
        }
    }
}

std::vector<Guide *> TourService::findAvailableGuides(const Date &date, const std::string &language) const
{
    std::vector<Guide *> result;
    for (const auto &g : guides)
    {
        if (g->isAvailable(date) && (language.empty() || g->speaks(language)))
        {
            result.push_back(g.get());
        }
    }
    sortGuidesByRating(result);
    return result;
}

std::vector<Vehicle *> TourService::findSuitableVehicles(const Date &date, int32_t seats) const
{
    std::vector<Vehicle *> result;
    for (const auto &v : vehicles)
    {
        if (v->canFit(seats) && v->isAvailable(date))
        {
            result.push_back(v.get());
        }
    }
    sortVehiclesByCapacity(result);
    return result;
}
bool TourService::groupStep(bool ok, Group *group, const std::string &successText)
{
    if (!ok)
    {
        return fail(group->getLastError());
    }
    lastMessage = "Group #" + std::to_string(group->getId()) + ": " + successText;
    log(lastMessage);
    return true;
}

bool TourService::assignGuide(Group *group, Guide *guide, const std::string &language)
{
    if (group == nullptr || guide == nullptr)
    {
        return fail("Group or guide not found");
    }
    bool ok = group->assignGuide(guide, language);
    return groupStep(ok, group, "guide " + guide->getName() + " assigned");
}

bool TourService::autoAssignGuide(Group *group, const std::string &language)
{
    if (group == nullptr)
    {
        return fail("Group not found");
    }
    std::vector<Guide *> candidates = findAvailableGuides(group->getExcursion()->getDate(), language);
    if (candidates.empty())
    {
        return fail("No available guide for this date" +
                    (language.empty() ? std::string() : " and language " + language));
    }
    return assignGuide(group, candidates.front(), language); // лучший по рейтингу
}

bool TourService::assignVehicle(Group *group, Vehicle *vehicle)
{
    if (group == nullptr || vehicle == nullptr)
    {
        return fail("Group or vehicle not found");
    }
    bool ok = group->assignVehicle(vehicle);
    return groupStep(ok, group, "vehicle " + vehicle->getModel() + " assigned");
}

bool TourService::autoAssignVehicle(Group *group)
{
    if (group == nullptr)
    {
        return fail("Group not found");
    }
    std::vector<Vehicle *> candidates =
        findSuitableVehicles(group->getExcursion()->getDate(), group->getTotalSeats());
    if (candidates.empty())
    {
        return fail("No free vehicle that fits " + std::to_string(group->getTotalSeats()) + " seats");
    }
    return assignVehicle(group, candidates.front()); // самый маленький подходящий
}

bool TourService::markReady(Group *group)
{
    if (group == nullptr)
    {
        return fail("Group not found");
    }
    return groupStep(group->markReady(), group, "ready, all bookings confirmed");
}

bool TourService::startGroup(Group *group)
{
    if (group == nullptr)
    {
        return fail("Group not found");
    }
    return groupStep(group->start(), group, "excursion started");
}

bool TourService::completeGroup(Group *group)
{
    if (group == nullptr)
    {
        return fail("Group not found");
    }
    return groupStep(group->complete(), group, "excursion completed");
}

bool TourService::guideRefuse(Group *group)
{
    if (group == nullptr)
    {
        return fail("Group not found");
    }
    Excursion *excursion = group->getExcursion();
    bool ok = groupStep(group->guideRefuse(), group, "cancelled: guide refused");
    if (ok)
    {
        clearWaitList(excursion);
    }
    return ok;
}

bool TourService::cancelGroup(Group *group, const std::string &reason)
{
    if (group == nullptr)
    {
        return fail("Group not found");
    }
    Excursion *excursion = group->getExcursion();
    bool ok = groupStep(group->cancel(reason), group, "cancelled: " + reason);
    if (ok)
    {
        clearWaitList(excursion);
    }
    return ok;
}

bool TourService::cancelGroupIfNotEnoughSeats(Group *group)
{
    if (group == nullptr)
    {
        return fail("Group not found");
    }
    Excursion *excursion = group->getExcursion();
    bool ok = groupStep(group->cancelIfNotEnoughSeats(), group, "cancelled: not enough seats");
    if (ok)
    {
        clearWaitList(excursion);
    }
    return ok;
}