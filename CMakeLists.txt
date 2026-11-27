cmake_minimum_required(VERSION 4.4)
project(Practica_college)

set(CMAKE_CXX_STANDARD 20)

add_executable(Practica_college src/main.cpp
        src/Person.cpp
        include/models/Person.h
        src/Client.cpp
        include/models/Client.h
        src/Guide.cpp
        include/models/Guide.h
        src/Vehicle.cpp
        include/models/Vehicle.h
        src/Excursion.cpp
        include/models/Excursion.h
        src/Booking.cpp
        include/models/Booking.h
        src/Group.cpp
        include/models/Group.h
        include/models/Date.h
        src/TourService.cpp
        include/services/TourService.h)
