#ifndef PRACTICA_COLLEGE_DATE_H
#define PRACTICA_COLLEGE_DATE_H
#include <string>
#include <cstdint>


class Date
{
public:
   int32_t day = 0;
   int32_t month = 0;
   int32_t year = 0;

   Date(int32_t day = 1, int32_t month = 1, int32_t year = 2026)
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
         m = "0" + m;
      }
      return d + "." + m + "." + std::to_string(year);
   }

};
#endif