#include "Format.h"

#include "Common.h"

#include <db/SQLite3.h>
#include <ygo/data/Serialize.h>

#include <stdexcept>

namespace ygo
{
namespace deck
{

Format::Format(data::Format format, std::string formatDate) :
    mFormat(format),
    mFormatDate(std::move(formatDate))
{
    auto formatExists = false;
    db::SQLite3 db(DBPATH);
    db.select("format_id", "formats",
            db::DBPair("name",mFormatDate),
            [&](db::SQLite3::DataMap data) {
                formatExists = true;
            });
    // check the format is legitimate
    if (!formatExists) {
        throw std::runtime_error(std::string("Invalid format ") + mFormatDate);
    }
}

int Format::cardCount(const std::string& name) const
{
    // look up this card in the formats database
    auto count = 0;
    auto callback = false;
    db::SQLite3 db(DBPATH);
    db.select("card_status","formats",
            db::DBAnd({
                db::DBPair("card_name",name),
                db::DBOr({
                    db::DBPair("name",mFormatDate),
                    db::DBPair("name",
                        data::fromLimitation(data::Limitation::ILLEGAL))})}),
            [&](db::SQLite3::DataMap data) {
                callback = true;
                count = CardLimitation(data::toLimitation(data["card_status"]),
                        mFormat);
            });
    if (!callback) {
        count = CardLimitation(data::Limitation::UNLIMITED, mFormat);
    }
    return count;
}

std::vector<std::string> Format::formats()
{
    static std::vector<std::string> ft = {
        fromFormat(data::Format::TRADITIONAL),
        fromFormat(data::Format::ADVANCED)
    };
    return ft;
}

std::vector<std::string> Format::formatDates()
{
    std::vector<std::string> ft;
    // get all the format dates
    db::SQLite3 db(DBPATH);
    db.select(db::DBUnique("name"),"formats",db::DBTrue(),
            [&](db::SQLite3::DataMap data) {
                auto format = data["name"];
                if (format == fromLimitation(data::Limitation::ILLEGAL)) {
                    return;
                }
                ft.push_back(format);
            });
    return ft;
}

}
}
