#ifndef YGO_DECK_FORMAT_H
#define YGO_DECK_FORMAT_H

#include <data/FormatData.h>

#include <string>
#include <vector>

namespace ygo
{
namespace deck
{

class Format
{
public:
    Format(data::Format format, std::string formatDate);

    inline data::Format format() const { return mFormat; }
    inline std::string formatDate() const { return mFormatDate; }

    int cardCount(const std::string& card) const;

    static std::vector<std::string> formatDates();
    static std::vector<std::string> formats();

private:
    data::Format mFormat;
    std::string mFormatDate;
};

}
}

#endif
