#include <ygo/deck/Format.h>

#include <ygo/deck/DB.h>

#include <kizhi/Log.h>
#include <mindbw/SQLite3.h>
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
    KIZHI_TRACE_F << "Creating format " << data::fromFormat(format) << " "
        << formatDate;
    auto formatExists = false;
    mindbw::SQLite3 db(DB::get().path());
    db.select("format_id", "formats",
            mindbw::Equal("name",mFormatDate),
            [&](mindbw::DataMap data) {
                formatExists = true;
            });
    // check the format is legitimate
    if (!formatExists) {
        KIZHI_FATAL_F << "Invalid format: " << formatDate;
        throw std::runtime_error(std::string("Invalid format ") + mFormatDate);
    }
}

int Format::cardCount(const std::string& name) const
{
    // look up this card in the formats database
    auto count = 0;
    auto callback = false;
    mindbw::SQLite3 db(DB::get().path());
    db.select("card_status","formats",
            mindbw::And({
                mindbw::Equal("card_name",name),
                mindbw::Or({
                    mindbw::Equal("name",mFormatDate),
                    mindbw::Equal("name",
                        data::fromLimitation(data::Limitation::ILLEGAL))})}),
            [&](mindbw::DataMap data) {
                callback = true;
                count = CardLimitation(data::toLimitation(data["card_status"]),
                        mFormat);
            });
    if (!callback) {
        KIZHI_TRACE_F << name << " has no limitations in " << mFormatDate
            << " " << data::fromFormat(mFormat);
        count = CardLimitation(data::Limitation::UNLIMITED, mFormat);
    }
    KIZHI_TRACE_F << name << " can appear " << count << " times in a deck";
    return count;
}

std::vector<std::string> Format::formats()
{
    static std::vector<std::string> ft = {
        fromFormat(data::Format::TRADITIONAL),
        fromFormat(data::Format::ADVANCED)
    };
    KIZHI_TRACE_F << "There are " << ft.size() << " formats";
    return ft;
}

std::vector<std::string> Format::formatDates()
{
    std::vector<std::string> ft;
    // get all the format dates
    mindbw::SQLite3 db(DB::get().path());
    db.select(mindbw::Unique("name"),"formats",mindbw::True(),
            [&](mindbw::DataMap data) {
                auto format = data["name"];
                if (format == fromLimitation(data::Limitation::ILLEGAL)) {
                    return;
                }
                ft.push_back(format);
            });
    KIZHI_TRACE_F << "There are " << ft.size() << " format dates";
    return ft;
}

}
}
