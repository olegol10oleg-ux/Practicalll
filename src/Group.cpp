#include "../include/models/Group.h"
#include <algorithm>
#include <string>
#include <vector>
#include <cstdint>

Group::Group(int32_t id, Excursion *excursion)
    : id(id), excursion(excursion)
{
}

int32_t Group::getId() const
{
    return id;
}

Excursion *Group::getExcursion() const
{
    return excursion;
}

Guide *Group::getGuide() const
{
    return guide;
}

Vehicle *Group::getVehicle() const
{
    return vehicle;
}

GroupStatus Group::getStatus() const
{
    return status;
}

const std::vector<Booking *> &Group::getBookings() const
{
    return bookings;
}

const std::string &Group::getLastError() const
{
    return lastError;
}

const std::string &Group::getCancelReason() const
{
    return cancelReason;
}

int32_t Group::getTotalSeats() const
{
    int32_t total = 0;
    for (const auto *x : bookings)
    {
        if (x->isActive())
        {
            total += x->getSeatsCount();
        }
    }
    return total;
}

bool Group::fail(const std::string &message)
{
    lastError = message;
    return false;
}

void Group::releaseResources()
{
    if (excursion != nullptr)
    {
        if (guide != nullptr)
        {
            guide->release(excursion->getDate());
        }
        if (vehicle != nullptr)
        {
            vehicle->release(excursion->getDate());
        }
    }
    guide = nullptr;
    vehicle = nullptr;
}
bool Group::addBooking(Booking *booking)
{
    if (booking == nullptr)
    {
        return fail("Booking is null");
    }
    if (status == GroupStatus::READY || status == GroupStatus::IN_PROGRESS ||
        status == GroupStatus::COMPLETED || status == GroupStatus::CANCELLED)
    {
        return fail("Group is closed for new bookings");
    }
    if (booking->getExcursion() != excursion)
    {
        return fail("Booking is for another excursion");
    }
    if (!booking->isActive())
    {
        return fail("Booking is cancelled");
    }
    if (std::find(bookings.begin(), bookings.end(), booking) != bookings.end())
    {
        return fail("Booking is already in the group");
    }
    if (vehicle != nullptr && getTotalSeats() + booking->getSeatsCount() > vehicle->getCapacity())
    {
        return fail("Vehicle capacity is exceeded");
    }
    bookings.push_back(booking);
    return true;
}

bool Group::removeBooking(int32_t bookingId)
{
    if (status == GroupStatus::READY || status == GroupStatus::IN_PROGRESS ||
        status == GroupStatus::COMPLETED || status == GroupStatus::CANCELLED)
    {
        return fail("Group is closed for changes");
    }
    auto it = std::find_if(bookings.begin(), bookings.end(),
                           [bookingId](const Booking *b) { return b->getId() == bookingId; });
    if (it == bookings.end())
    {
        return fail("Booking not found in the group");
    }
    bookings.erase(it);
    return true;
}
bool Group::assignGuide(Guide *newGuide, const std::string &language)
{
    if (status != GroupStatus::FORMING)
    {
        return fail("Guide can be assigned only while the group is forming");
    }
    if (newGuide == nullptr)
    {
        return fail("Guide is null");
    }
    if (bookings.empty())
    {
        return fail("Group has no bookings");
    }
    if (!language.empty() && !newGuide->speaks(language))
    {
        return fail("Guide does not speak " + language);
    }
    if (!newGuide->reserve(excursion->getDate()))
    {
        return fail("Guide is busy on " + excursion->getDate().toString());
    }
    guide = newGuide;
    status = GroupStatus::GUIDE_ASSIGNED;
    return true;
}

bool Group::guideRefuse()
{
    if (guide == nullptr)
    {
        return fail("No guide is assigned");
    }
    if (status == GroupStatus::IN_PROGRESS)
    {
        return fail("Excursion is already in progress");
    }
    return cancel("Guide refused");
}
bool Group::assignVehicle(Vehicle *newVehicle)
{
    if (status != GroupStatus::GUIDE_ASSIGNED)
    {
        return fail("Assign a guide first");
    }
    if (newVehicle == nullptr)
    {
        return fail("Vehicle is null");
    }
    if (!newVehicle->canFit(getTotalSeats()))
    {
        return fail("Vehicle capacity is too small for " + std::to_string(getTotalSeats()) + " seats");
    }
    if (!newVehicle->reserve(excursion->getDate()))
    {
        return fail("Vehicle is busy on " + excursion->getDate().toString());
    }
    vehicle = newVehicle;
    status = GroupStatus::TRANSPORT_ASSIGNED;
    return true;
}
bool Group::markReady()
{
    if (status != GroupStatus::TRANSPORT_ASSIGNED)
    {
        return fail("Guide and vehicle must be assigned first");
    }
    if (getTotalSeats() < excursion->getMinSeats())
    {
        return fail("Minimum number of seats is not reached");
    }
    for (auto *b : bookings)
    {
        if (b->getStatus() == PENDING)
        {
            b->confirm();
        }
    }
    status = GroupStatus::READY;
    return true;
}
bool Group::start()
{
    if (status != GroupStatus::READY)
    {
        return fail("Group is not ready");
    }
    status = GroupStatus::IN_PROGRESS;
    return true;
}

bool Group::complete()
{
    if (status != GroupStatus::IN_PROGRESS)
    {
        return fail("Excursion is not in progress");
    }
    status = GroupStatus::COMPLETED;
    return true;
}
bool Group::cancel(const std::string &reason)
{
    if (status == GroupStatus::IN_PROGRESS)
    {
        return fail("Cannot cancel: excursion is in progress");
    }
    if (status == GroupStatus::COMPLETED)
    {
        return fail("Cannot cancel: excursion is completed");
    }
    if (status == GroupStatus::CANCELLED)
    {
        return fail("Group is already cancelled");
    }
    for (auto *x : bookings)
    {
        x->cancel();
    }
    releaseResources();
    cancelReason = reason;
    status = GroupStatus::CANCELLED;
    return true;
}

bool Group::cancelIfNotEnoughSeats()
{
    if (excursion == nullptr || getTotalSeats() >= excursion->getMinSeats())
    {
        return fail("Minimum number of seats is reached, no need to cancel");
    }
    return cancel("Not enough seats");
}



std::string Group::getStatusString() const
{
        std::string result;
        switch (status)
        {
            case GroupStatus::FORMING:
                result = "FORMING";
                break;
            case GroupStatus::GUIDE_ASSIGNED:
                result = "GUIDE_ASSIGNED";
                break;
            case GroupStatus::TRANSPORT_ASSIGNED:
                result = "TRANSPORT_ASSIGNED";
                break;
            case GroupStatus::READY:
                result = "READY";
                break;
            case GroupStatus::IN_PROGRESS:
                result = "IN_PROGRESS";
                break;
            case GroupStatus::COMPLETED:
                result = "COMPLETED";
                break;
            case GroupStatus::CANCELLED:
                result = "CANCELLED";
                break;
            default:
                result = "UNKNOWN";
                break;
        }
    return result;
}


std::string Group::getInfo() const
{
    std::string guideName = "-";
    if (guide != nullptr)
    {
        guideName = guide->getName();
    }
    std::string vehicleModel = "-";
    if (vehicle != nullptr)
    {
        vehicleModel = vehicle->getModel();
    }
    std::string excursionTitle = "-";
    if (excursion != nullptr)
    {
        excursionTitle = excursion->getTittle();
    }

    std::string result = "Group #" + std::to_string(id) +
                         ", excursion: " + excursionTitle +
                         ", status: " + getStatusString() +
                         ", guide: " + guideName +
                         ", vehicle: " + vehicleModel +
                         ", seats: " + std::to_string(getTotalSeats()) +
                         ", bookings: " + std::to_string(bookings.size());
    if (status == GroupStatus::CANCELLED)
    {
        result += ", reason: " + cancelReason;
    }
    return result;
}