#include "native/root/rootIO.h"
#include "value/dataType.h"
#include "semantics/astRunner.h"
#include "native/root/rootDate.h"
#include "value/import.h"

namespace Snapp::Native {
int getCurrentUnix() {
  std::time_t now = std::time(nullptr);
  std::chrono::seconds seconds_since_epoch = std::chrono::seconds(now);
  int seconds = seconds_since_epoch.count();

  return seconds;
}

int getCurrentMs() {
  std::time_t now = std::time(nullptr);
  auto ms = std::chrono::duration_cast<std::chrono::milliseconds>(
                std::chrono::system_clock::now().time_since_epoch())
                .count() %
            1000;

  return static_cast<int>(ms);
}

NativeFunction createConstructor() {
  return {DataType::Void,
          {},
          [](const std::vector<DataValue> &args,
             Scope *scope) -> std::optional<DataValue> {
            std::time_t now = std::time(nullptr);
            std::tm *local_time = std::localtime(&now);

            scope->assign("unix_", getCurrentUnix());
            scope->assign("millis_", getCurrentMs());
            scope->assign("year_", local_time->tm_year + 1900);
            scope->assign("month_", local_time->tm_mon + 1);
            scope->assign("week_", (local_time->tm_yday + 1) / 7 + 1);
            scope->assign("weekday_", local_time->tm_wday);
            scope->assign("day_", local_time->tm_mday);
            scope->assign("hour_", local_time->tm_hour);
            scope->assign("minute_", local_time->tm_min);
            scope->assign("second_", local_time->tm_sec);

            return std::nullopt;
          }};
}

FunctionValue createGetTotalSeconds() {
  FunctionValue getTotalSeconds;
  getTotalSeconds.addOverload({DataType::Int,
                               {},
                               [](const std::vector<DataValue> &args,
                                  Scope *scope) -> std::optional<DataValue> {
                                 return std::get<int>(scope->get("unix_"));
                               }});
  return getTotalSeconds;
}

FunctionValue createGetMillis() {
  FunctionValue getMillis;
  getMillis.addOverload({DataType::Int,
                         {},
                         [](const std::vector<DataValue> &args,
                            Scope *scope) -> std::optional<DataValue> {
                           return std::get<int>(scope->get("millis_"));
                         }});
  return getMillis;
}

FunctionValue createGetYear() {
  FunctionValue getYear;
  getYear.addOverload({DataType::Int,
                       {},
                       [](const std::vector<DataValue> &args,
Scope *scope) -> std::optional<DataValue> {
return std::get<int>(scope->get("year_"));
}});
  return getYear;
}

FunctionValue createGetTotalYears() {
  FunctionValue getTotalYears;
  getTotalYears.addOverload({DataType::Int,
                             {},
                             [](const std::vector<DataValue> &args,
                                Scope *scope) -> std::optional<DataValue> {
                               int year = std::get<int>(scope->get("year_"));
                               return year - 1970;
                             }});
  return getTotalYears;
}

FunctionValue createGetMonth() {
  FunctionValue getMonth;
  getMonth.addOverload({DataType::Int,
                        {},
                        [](const std::vector<DataValue> &args,
                           Scope *scope) -> std::optional<DataValue> {
                          return std::get<int>(scope->get("month_"));
                        }});
  return getMonth;
}

FunctionValue createGetTotalMonths() {
  FunctionValue getTotalMonths;
  getTotalMonths.addOverload({DataType::Int,
                              {},
                              [](const std::vector<DataValue> &args,
                                 Scope *scope) -> std::optional<DataValue> {
                                int year = std::get<int>(scope->get("year_"));
                                int month = std::get<int>(scope->get("month_"));
                                return (year - 1970) * 12 + (month - 1);
                              }});
  return getTotalMonths;
}
FunctionValue createGetWeekday() {
  FunctionValue getWeekday;
  getWeekday.addOverload({DataType::Int,
                          {},
                          [](const std::vector<DataValue> &args,
                             Scope *scope) -> std::optional<DataValue> {
                            return std::get<int>(scope->get("weekday_"));
                          }});
  return getWeekday;
}

FunctionValue createGetWeek() {
    FunctionValue getWeek;
    getWeek.addOverload({DataType::Int,
                         {},
                         [](const std::vector<DataValue> &args,
                            Scope *scope) -> std::optional<DataValue> {
                             return std::get<int>(scope->get("week_"));
                         }});
    return getWeek;
}

FunctionValue createGetDay() {
    FunctionValue getDay;
    getDay.addOverload({DataType::Int,
                        {},
                        [](const std::vector<DataValue> &args,
                           Scope *scope) -> std::optional<DataValue> {
                            return std::get<int>(scope->get("day_"));
                        }});
    return getDay;
}

FunctionValue createGetHour() {
    FunctionValue getHour;
    getHour.addOverload({DataType::Int,
                         {},
                         [](const std::vector<DataValue> &args,
                            Scope *scope) -> std::optional<DataValue> {
                             return std::get<int>(scope->get("hour_"));
                         }});
    return getHour;
}

FunctionValue createGetMinute() {
    FunctionValue getMinute;
    getMinute.addOverload({DataType::Int,
                           {},
                           [](const std::vector<DataValue> &args,
                              Scope *scope) -> std::optional<DataValue> {
                               return std::get<int>(scope->get("minute_"));
                           }});
    return getMinute;
}

FunctionValue createGetSecond() {
    FunctionValue getSecond;
    getSecond.addOverload({DataType::Int,
                           {},
                           [](const std::vector<DataValue> &args,
                              Scope *scope) -> std::optional<DataValue> {
                               return std::get<int>(scope->get("second_"));
                           }});
    return getSecond;
}

ClassValue* createRootDate() {
    ClassValue* dateClass = new ClassValue("Date");

    ClassAttribute unixAttribute = {
        DataType::Int,
        new SyntaxNodeLiteral(0)
    };

    ClassAttribute millisecondAttribute = {
        DataType::Int,
        new SyntaxNodeLiteral(0)
    };

    ClassAttribute yearAttribute = {
        DataType::Int,
        new SyntaxNodeLiteral(0)
    };

    ClassAttribute totalYearsAttribute = {
        DataType::Int,
        new SyntaxNodeLiteral(0)
    };

    ClassAttribute monthAttribute = {
        DataType::Int,
        new SyntaxNodeLiteral(0)
    };

    ClassAttribute totalMonthsAttribute = {
        DataType::Int,
        new SyntaxNodeLiteral(0)
    };

    ClassAttribute weekAttribute = {
        DataType::Int,
        new SyntaxNodeLiteral(0)
    };

    ClassAttribute weekdayAttribute = {
        DataType::Int,
        new SyntaxNodeLiteral(0)
    };

    ClassAttribute dayAttribute = {
        DataType::Int,
        new SyntaxNodeLiteral(0)
    };

    ClassAttribute hourAttribute = {
        DataType::Int,
        new SyntaxNodeLiteral(0)
    };

    ClassAttribute minuteAttribute = {
        DataType::Int,
        new SyntaxNodeLiteral(0)
    };

    ClassAttribute secondAttribute = {
        DataType::Int,
        new SyntaxNodeLiteral(0)
    };

    dateClass->add("unix_", unixAttribute);
    dateClass->add("millis_", millisecondAttribute);
    dateClass->add("year_", yearAttribute);
    dateClass->add("totalYears_", totalYearsAttribute);
    dateClass->add("month_", monthAttribute);
    dateClass->add("totalMonths_", totalMonthsAttribute);
    dateClass->add("week_", weekAttribute);
    dateClass->add("weekday_", weekdayAttribute);
    dateClass->add("day_", dayAttribute);
    dateClass->add("hour_", hourAttribute);
    dateClass->add("minute_", minuteAttribute);
    dateClass->add("second_", secondAttribute);

    dateClass->constructor().addOverload(createConstructor());

    dateClass->add("getTotalSeconds", createGetTotalSeconds());
    dateClass->add("getMillis", createGetMillis());
    dateClass->add("getYear", createGetYear());
    dateClass->add("getTotalYears", createGetTotalYears());
    dateClass->add("getMonth", createGetMonth());
    dateClass->add("getTotalMonths", createGetTotalMonths());
    dateClass->add("getWeek", createGetWeek());
    dateClass->add("getWeekday", createGetWeekday());
    dateClass->add("getDay", createGetDay());
    dateClass->add("getHour", createGetHour());
    dateClass->add("getMinute", createGetMinute());
    dateClass->add("getSecond", createGetSecond());

    return dateClass;
}

}
