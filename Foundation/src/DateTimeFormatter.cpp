//
// DateTimeFormatter.cpp
//
// $Id: //poco/svn/Foundation/src/DateTimeFormatter.cpp#2 $
//
// Library: Foundation
// Package: DateTime
// Module:  DateTimeFormatter
//
// Copyright (c) 2004-2006, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// Permission is hereby granted, free of charge, to any person or organization
// obtaining a copy of the software and accompanying documentation covered by
// this license (the "Software") to use, reproduce, display, distribute,
// execute, and transmit the Software, and to prepare derivative works of the
// Software, and to permit third-parties to whom the Software is furnished to
// do so, all subject to the following:
// 
// The copyright notices in the Software and this entire statement, including
// the above license grant, this restriction and the following disclaimer,
// must be included in all copies of the Software, in whole or in part, and
// all derivative works of the Software, unless such copies or derivative
// works are solely in the form of machine-executable object code generated by
// a source language processor.
// 
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE, TITLE AND NON-INFRINGEMENT. IN NO EVENT
// SHALL THE COPYRIGHT HOLDERS OR ANYONE DISTRIBUTING THE SOFTWARE BE LIABLE
// FOR ANY DAMAGES OR OTHER LIABILITY, WHETHER IN CONTRACT, TORT OR OTHERWISE,
// ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
// DEALINGS IN THE SOFTWARE.
//


#include "Poco/DateTimeFormatter.h"
#include "Poco/DateTimeFormat.h"
#include "Poco/DateTime.h"
#include "Poco/LocalDateTime.h"
#include "Poco/Timestamp.h"
#include "Poco/NumberFormatter.h"


namespace Poco {


std::string DateTimeFormatter::format(const Timestamp& timestamp, const std::string& fmt, int timeZoneDifferential)
{
	DateTime dateTime(timestamp);
	return format(dateTime, fmt, timeZoneDifferential);
}


std::string DateTimeFormatter::format(const DateTime& dateTime, const std::string& fmt, int timeZoneDifferential)
{
	std::string result;
	result.reserve(64);
	std::string::const_iterator it  = fmt.begin();
	std::string::const_iterator end = fmt.end();
	while (it != end)
	{
		if (*it == '%')
		{
			if (++it != end)
			{
				switch (*it)
				{
				case 'w': result.append(DateTimeFormat::WEEKDAY_NAMES[dateTime.dayOfWeek()], 0, 3); break;
				case 'W': result.append(DateTimeFormat::WEEKDAY_NAMES[dateTime.dayOfWeek()]); break;
				case 'b': result.append(DateTimeFormat::MONTH_NAMES[dateTime.month() - 1], 0, 3); break;
				case 'B': result.append(DateTimeFormat::MONTH_NAMES[dateTime.month() - 1]); break;
				case 'd': result.append(NumberFormatter::format0(dateTime.day(), 2)); break;
				case 'e': result.append(NumberFormatter::format(dateTime.day())); break;
				case 'f': result.append(NumberFormatter::format(dateTime.day(), 2)); break;
				case 'm': result.append(NumberFormatter::format0(dateTime.month(), 2)); break;
				case 'n': result.append(NumberFormatter::format(dateTime.month())); break;
				case 'o': result.append(NumberFormatter::format(dateTime.month(), 2)); break;
				case 'y': result.append(NumberFormatter::format0(dateTime.year() % 100, 2)); break;
				case 'Y': result.append(NumberFormatter::format0(dateTime.year(), 4)); break;
				case 'H': result.append(NumberFormatter::format0(dateTime.hour(), 2)); break;
				case 'h': result.append(NumberFormatter::format0(dateTime.hourAMPM(), 2)); break;
				case 'a': result.append(dateTime.isAM() ? "am" : "pm"); break;
				case 'A': result.append(dateTime.isAM() ? "AM" : "PM"); break;
				case 'M': result.append(NumberFormatter::format0(dateTime.minute(), 2)); break;
				case 'S': result.append(NumberFormatter::format0(dateTime.second(), 2)); break;
				case 'i': result.append(NumberFormatter::format0(dateTime.millisecond(), 3)); break;
				case 'c': result.append(NumberFormatter::format(dateTime.millisecond()/100)); break;
				case 'z': result.append(tzdISO(timeZoneDifferential)); break;
				case 'Z': result.append(tzdRFC(timeZoneDifferential)); break;
				default:  result += *it;
				}
				++it;
			}
		}
		else result += *it++;
	}
	return result;
}


std::string DateTimeFormatter::format(const LocalDateTime& dateTime, const std::string& fmt)
{
	return format(dateTime._dateTime, fmt, dateTime._tzd);
}


std::string DateTimeFormatter::format(const Timespan& timespan, const std::string& fmt)
{
	std::string result;
	result.reserve(32);
	std::string::const_iterator it  = fmt.begin();
	std::string::const_iterator end = fmt.end();
	while (it != end)
	{
		if (*it == '%')
		{
			if (++it != end)
			{
				switch (*it)
				{
				case 'd': result.append(NumberFormatter::format(timespan.days())); break;
				case 'H': result.append(NumberFormatter::format0(timespan.hours(), 2)); break;
				case 'h': result.append(NumberFormatter::format(timespan.totalHours())); break;
				case 'M': result.append(NumberFormatter::format0(timespan.minutes(), 2)); break;
				case 'm': result.append(NumberFormatter::format(timespan.totalMinutes())); break;
				case 'S': result.append(NumberFormatter::format0(timespan.seconds(), 2)); break;
				case 's': result.append(NumberFormatter::format(timespan.totalSeconds())); break;
				case 'i': result.append(NumberFormatter::format0(timespan.milliseconds(), 3)); break;
				case 'c': result.append(NumberFormatter::format(timespan.milliseconds()/100)); break;
				default:  result += *it;
				}
				++it;
			}
		}
		else result += *it++;
	}
	return result;
}


std::string DateTimeFormatter::tzdISO(int timeZoneDifferential)
{
	std::string tzd;
	tzd.reserve(8);
	if (timeZoneDifferential != UTC)
	{
		if (timeZoneDifferential >= 0)
		{
			tzd += '+';
			tzd += NumberFormatter::format0(timeZoneDifferential/3600, 2);
			tzd += ':';
			tzd += NumberFormatter::format0((timeZoneDifferential%3600)/60, 2);
		}
		else
		{
			tzd += '-';
			tzd += NumberFormatter::format0(-timeZoneDifferential/3600, 2);
			tzd += ':';
			tzd += NumberFormatter::format0((-timeZoneDifferential%3600)/60, 2);
		}
	}
	else tzd = "Z";
	return tzd;
}


std::string DateTimeFormatter::tzdRFC(int timeZoneDifferential)
{
	std::string tzd;
	tzd.reserve(8);
	if (timeZoneDifferential != UTC)
	{
		if (timeZoneDifferential >= 0)
		{
			tzd += '+';
			tzd += NumberFormatter::format0(timeZoneDifferential/3600, 2);
			tzd += NumberFormatter::format0((timeZoneDifferential%3600)/60, 2);
		}
		else
		{
			tzd += '-';
			tzd += NumberFormatter::format0(-timeZoneDifferential/3600, 2);
			tzd += NumberFormatter::format0((-timeZoneDifferential%3600)/60, 2);
		}		
	}
	else tzd = "GMT";
	return tzd;
}


} // namespace Poco
