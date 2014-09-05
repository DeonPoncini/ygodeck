#include "DBFormat.h"

#include "DB.h"
#include "DBCommon.h"
#include "Serialize.h"

#include <stdexcept>

namespace ygo
{

DBFormat::DBFormat(Format format, std::string formatDate) :
    mFormat(format),
    mFormatDate(std::move(formatDate))
{
    auto formatExists = false;
    DB db(DBPATH);
    db.select("format_id", "formats",
            DBPair("name",mFormatDate),
            [&](DB::DataMap data)
            {
                formatExists = true;
            });
    // check the format is legitimate
    if (!formatExists)
    {
        throw std::runtime_error(std::string("Invalid format ") + mFormatDate);
    }
}

int DBFormat::cardCount(const std::string& name) const
{
    // look up this card in the formats database
    auto count = 0;
    auto callback = false;
    DB db(DBPATH);
    db.select("card_status","formats",
            DBAnd({
                DBPair("card_name",name),
                DBOr({
                    DBPair("name",mFormatDate),
                    DBPair("name",fromLimitation(Limitation::ILLEGAL))})}),
            [&](DB::DataMap data)
            {
                callback = true;
                count = CardLimitation(toLimitation(data["card_status"]),
                        mFormat);
            });
    if (!callback)
    {
        count = CardLimitation(Limitation::UNLIMITED, mFormat);
    }
    return count;
}

std::vector<std::string> DBFormat::formats()
{
    static std::vector<std::string> ft = {
        fromFormat(Format::TRADITIONAL),
        fromFormat(Format::ADVANCED)
    };
    return ft;
}

std::vector<std::string> DBFormat::formatDates()
{
    std::vector<std::string> ft;
    // get all the format dates
    DB db(DBPATH);
    db.select(DBUnique("name"),"formats",DBTrue(),
            [&](DB::DataMap data)
            {
                auto format = data["name"];
                if (format == fromLimitation(Limitation::ILLEGAL))
                {
                    return;
                }
                ft.push_back(format);
            });
    return ft;
}

}
