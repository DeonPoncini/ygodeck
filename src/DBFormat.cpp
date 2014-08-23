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

int DBFormat::cardCount(const std::string& name)
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

}
