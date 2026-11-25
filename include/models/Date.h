#ifndef PRACTICA_COLLEGE_DATE_H
#define PRACTICA_COLLEGE_DATE_H
#include <string>
#include <cstdint>
class Date
{
   uint32_t day = 0;
   uint32_t month = 0;
   uint32_t year = 0;

   Date(int day = 1, int month = 1, int year = 2026)
   {
      this->day = day;
      this->month = month;
      this->year = year;
   }
   bool operator==(const Date& other) const
   {
      return day == other.day && month == other.month && year == other.year;
   }

   [[nodiscard]] std::string toString() const
   {
      std::string d = std::to_string(day);
      std::string m = std::to_string(month);
      if (day < 10)
      {
         d = "0" + d;
      }
      if (month < 10)
      {
         d = "0" + m;
      }
      return d + "." + std::to_string(year);
   }

};

#endif
