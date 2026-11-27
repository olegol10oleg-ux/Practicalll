#include "../include/services/TourService.h"
#include <cstdlib>
#include <iostream>
#include <string>
#include <vector>

static std::string readLine(const std::string &prompt)
{
    std::cout << prompt;
    std::string line;
    if (!std::getline(std::cin, line))
    {
        std::cout << std::endl << "Input closed. Bye!" << std::endl;
        std::exit(0);
    }
    return line;
}

static std::string readNonEmpty(const std::string &prompt)
{
    while (true)
    {
        std::string line = readLine(prompt);
        if (!line.empty())
        {
            return line;
        }
        std::cout << "Value cannot be empty." << std::endl;
    }
}

static int32_t readInt(const std::string &prompt, int32_t min, int32_t max)
{
    while (true)
    {
        std::string line = readLine(prompt);
        try
        {
            size_t pos = 0;
            long value = std::stol(line, &pos);
            if (pos == line.size() && value >= min && value <= max)
            {
                return static_cast<int32_t>(value);
            }
        }
        catch (...)
        {
        }
        std::cout << "Enter a number from " << min << " to " << max << "." << std::endl;
    }
}

static bool isLeapYear(int32_t y)
{
    return (y % 4 == 0 && y % 100 != 0) || y % 400 == 0;
}

static int32_t daysInMonth(int32_t m, int32_t y)
{
    if (m == 2)
    {
        if (isLeapYear(y))
        {
            return 29;
        }
        return 28;
    }
    if (m == 4 || m == 6 || m == 9 || m == 11)
    {
        return 30;
    }
    return 31;
}

static Date readDate()
{
    while (true)
    {
        int32_t d = readInt("  Day: ", 1, 31);
        int32_t m = readInt("  Month: ", 1, 12);
        int32_t y = readInt("  Year: ", 2026, 2100);
        if (d <= daysInMonth(m, y))
        {
            return Date(d, m, y);
        }
        std::cout << "This month has fewer days. Enter the date again." << std::endl;
    }
}

static std::vector<std::string> splitList(const std::string &text)
{
    std::vector<std::string> result;
    std::string current;
    for (size_t i = 0; i <= text.size(); ++i)
    {
        if (i == text.size() || text[i] == ',')
        {
            size_t b = current.find_first_not_of(' ');
            size_t e = current.find_last_not_of(' ');
            if (b != std::string::npos)
            {
                result.push_back(current.substr(b, e - b + 1));
            }
            current.clear();
        }
        else
        {
            current += text[i];
        }
    }
    return result;
}

static void report(bool ok, const TourService &service)
{
    if (ok)
    {
        std::cout << "[OK] " << service.getLastMessage() << std::endl;
    }
    else
    {
        std::cout << "[FAILED] " << service.getLastMessage() << std::endl;
    }
}

static void printClients(const TourService &s)
{
    std::cout << "--- Clients ---" << std::endl;
    if (s.getClients().empty())
    {
        std::cout << "(none)" << std::endl;
    }
    for (const auto &c : s.getClients())
    {
        std::cout << c->getInfo() << std::endl;
    }
}

static void printGuides(const TourService &s)
{
    std::cout << "--- Guides ---" << std::endl;
    if (s.getGuides().empty())
    {
        std::cout << "(none)" << std::endl;
    }
    for (const auto &g : s.getGuides())
    {
        std::cout << g->getInfo();
        if (!g->getBusyDates().empty())
        {
            std::cout << ", busy: ";
            for (const auto &d : g->getBusyDates())
            {
                std::cout << d.toString() << " ";
            }
        }
        std::cout << std::endl;
    }
}

static void printVehicles(const TourService &s)
{
    std::cout << "--- Vehicles ---" << std::endl;
    if (s.getVehicles().empty())
    {
        std::cout << "(none)" << std::endl;
    }
    for (const auto &v : s.getVehicles())
    {
        std::cout << v->getInfo();
        if (!v->getBusyDates().empty())
        {
            std::cout << ", busy: ";
            for (const auto &d : v->getBusyDates())
            {
                std::cout << d.toString() << " ";
            }
        }
        std::cout << std::endl;
    }
}

static void printExcursions(const TourService &s)
{
    std::cout << "--- Excursions ---" << std::endl;
    if (s.getExcursions().empty())
    {
        std::cout << "(none)" << std::endl;
    }
    for (const auto &e : s.getExcursions())
    {
        std::cout << e->getInfo() << std::endl;
    }
}

static void printBookings(const TourService &s)
{
    std::cout << "--- Bookings ---" << std::endl;
    if (s.getBookings().empty())
    {
        std::cout << "(none)" << std::endl;
    }
    for (const auto &b : s.getBookings())
    {
        std::cout << b->getInfo() << std::endl;
    }
}

static void printGroups(const TourService &s)
{
    std::cout << "--- Groups ---" << std::endl;
    if (s.getGroups().empty())
    {
        std::cout << "(none)" << std::endl;
    }
    for (const auto &g : s.getGroups())
    {
        std::cout << g->getInfo() << std::endl;
    }
}

static void printWaitList(const TourService &s)
{
    std::cout << "--- Waiting list (first in line first) ---" << std::endl;
    std::vector<WaitEntry> entries = s.getWaitList();
    if (entries.empty())
    {
        std::cout << "(empty)" << std::endl;
    }
    for (size_t i = 0; i < entries.size(); ++i)
    {
        std::cout << i + 1 << ". " << entries[i].client->getName() << " wants "
                  << entries[i].seats << " seat(s) on \""
                  << entries[i].excursion->getTittle() << "\"" << std::endl;
    }
}

static void printLog(const TourService &s)
{
    std::cout << "--- Event log (newest first, last 15) ---" << std::endl;
    std::vector<std::string> entries = s.getRecentLog(15);
    if (entries.empty())
    {
        std::cout << "(empty)" << std::endl;
    }
    for (const auto &line : entries)
    {
        std::cout << "* " << line << std::endl;
    }
}

static void addClientMenu(TourService &s)
{
    std::string name = readNonEmpty("Name: ");
    std::string phone = readLine("Phone: ");
    std::string email = readLine("Email: ");
    report(s.addClient(name, phone, email) != nullptr, s);
}

static void addGuideMenu(TourService &s)
{
    std::string name = readNonEmpty("Name: ");
    std::string phone = readLine("Phone: ");
    std::vector<std::string> languages = splitList(readNonEmpty("Languages (comma separated): "));
    int32_t rating = readInt("Rating in hundredths, 0-500 (480 = 4.80): ", 0, 500);
    report(s.addGuide(name, phone, languages, rating) != nullptr, s);
}

static void addVehicleMenu(TourService &s)
{
    std::string model = readNonEmpty("Model: ");
    int32_t capacity = readInt("Capacity (seats): ", 1, 500);
    report(s.addVehicle(model, capacity) != nullptr, s);
}

static void addExcursionMenu(TourService &s)
{
    std::string title = readNonEmpty("Title: ");
    std::cout << "Date:" << std::endl;
    Date date = readDate();
    int32_t price = readInt("Price per seat (whole currency units): ", 0, 1000000);
    int32_t maxSeats = readInt("Max seats: ", 1, 500);
    int32_t minSeats = readInt("Min seats to hold the excursion: ", 0, maxSeats);
    report(s.addExcursion(title, date, price * 100, maxSeats, minSeats) != nullptr, s);
}

static void bookMenu(TourService &s)
{
    printClients(s);
    printExcursions(s);
    if (s.getClients().empty() || s.getExcursions().empty())
    {
        std::cout << "Add a client and an excursion first." << std::endl;
        return;
    }
    Client *client = s.findClient(readInt("Client id: ", 1, 1000000));
    Excursion *excursion = s.findExcursion(readInt("Excursion id: ", 1, 1000000));
    if (client == nullptr || excursion == nullptr)
    {
        std::cout << "[FAILED] Client or excursion not found" << std::endl;
        return;
    }
    int32_t seats = readInt("Seats: ", 1, 500);
    Booking *booking = s.bookExcursion(client, excursion, seats);
    bool inQueue = booking == nullptr && s.getLastMessage().find("waiting list") != std::string::npos;
    report(booking != nullptr || inQueue, s);
}

static void cancelBookingMenu(TourService &s)
{
    printBookings(s);
    if (s.getBookings().empty())
    {
        return;
    }
    int32_t id = readInt("Booking id to cancel (0 - back): ", 0, 1000000);
    if (id == 0)
    {
        return;
    }
    report(s.cancelBooking(id), s);
}

static void groupActions(TourService &s, Group *group)
{
    while (true)
    {
        std::cout << std::endl << group->getInfo() << std::endl;
        for (const Booking *b : group->getBookings())
        {
            std::cout << "   " << b->getInfo() << std::endl;
        }
        std::cout << std::endl << "Group process:" << std::endl
                     << " 1. Assign guide (best rating)" << std::endl
                     << " 2. Assign guide (choose manually)" << std::endl
                     << " 3. Assign vehicle (smallest that fits)" << std::endl
                     << " 4. Assign vehicle (choose manually)" << std::endl
                     << " 5. Mark group ready (confirm bookings)" << std::endl
                     << " 6. Start excursion" << std::endl
                     << " 7. Complete excursion" << std::endl
                     << " 8. Guide refuses" << std::endl
                     << " 9. Cancel if not enough seats" << std::endl
                     << "10. Cancel group (own reason)" << std::endl
                     << " 0. Back" << std::endl;
        int32_t choice = readInt("> ", 0, 10);
        switch (choice)
        {
        case 1:
        {
            std::string language = readLine("Required language (empty = any): ");
            report(s.autoAssignGuide(group, language), s);
            break;
        }
        case 2:
        {
            printGuides(s);
            std::string language = readLine("Required language (empty = any): ");
            Guide *guide = s.findGuide(readInt("Guide id: ", 1, 1000000));
            report(s.assignGuide(group, guide, language), s);
            break;
        }
        case 3:
            report(s.autoAssignVehicle(group), s);
            break;
        case 4:
        {
            printVehicles(s);
            Vehicle *vehicle = s.findVehicle(readInt("Vehicle id: ", 1, 1000000));
            report(s.assignVehicle(group, vehicle), s);
            break;
        }
        case 5:
            report(s.markReady(group), s);
            break;
        case 6:
            report(s.startGroup(group), s);
            break;
        case 7:
            report(s.completeGroup(group), s);
            break;
        case 8:
            report(s.guideRefuse(group), s);
            break;
        case 9:
            report(s.cancelGroupIfNotEnoughSeats(group), s);
            break;
        case 10:
        {
            std::string reason = readNonEmpty("Reason: ");
            report(s.cancelGroup(group, reason), s);
            break;
        }
        default:
            return;
        }
    }
}

static void groupMenu(TourService &s)
{
    while (true)
    {
        printGroups(s);
        if (s.getGroups().empty())
        {
            std::cout << "No groups yet. A group is created with the first booking." << std::endl;
            return;
        }
        int32_t id = readInt("Group id (0 - back): ", 0, 1000000);
        if (id == 0)
        {
            return;
        }
        Group *group = s.findGroup(id);
        if (group == nullptr)
        {
            std::cout << "[FAILED] Group not found" << std::endl;
            continue;
        }
        groupActions(s, group);
    }
}

int main()
{
    TourService service;
    std::cout << "=== Excursion booking system ===" << std::endl;

    while (true)
    {
        std::cout << std::endl << "Main menu:" << std::endl
                     << " 1. Add client" << std::endl
                     << " 2. Add guide" << std::endl
                     << " 3. Add vehicle" << std::endl
                     << " 4. Add excursion" << std::endl
                     << " 5. Book excursion" << std::endl
                     << " 6. Cancel booking" << std::endl
                     << " 7. Group process (guide, vehicle, start, finish)" << std::endl
                     << " 8. Show clients / guides / vehicles" << std::endl
                     << " 9. Show excursions / bookings / groups" << std::endl
                     << "10. Show waiting list" << std::endl
                     << "11. Show event log" << std::endl
                     << " 0. Exit" << std::endl;
        int32_t choice = readInt("> ", 0, 11);
        switch (choice)
        {
        case 1:
            addClientMenu(service);
            break;
        case 2:
            addGuideMenu(service);
            break;
        case 3:
            addVehicleMenu(service);
            break;
        case 4:
            addExcursionMenu(service);
            break;
        case 5:
            bookMenu(service);
            break;
        case 6:
            cancelBookingMenu(service);
            break;
        case 7:
            groupMenu(service);
            break;
        case 8:
            printClients(service);
            printGuides(service);
            printVehicles(service);
            break;
        case 9:
            printExcursions(service);
            printBookings(service);
            printGroups(service);
            break;
        case 10:
            printWaitList(service);
            break;
        case 11:
            printLog(service);
            break;
        default:
            std::cout << "Bye!" << std::endl;
            return 0;
        }
    }
}