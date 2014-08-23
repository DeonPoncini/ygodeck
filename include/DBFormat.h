#ifndef YGO_DBFORMAT_H
#define YGO_DBFORMAT_H

#include "FormatData.h"

namespace ygo
{

class DBFormat
{
public:
    DBFormat(Format format, std::string formatDate);

    inline Format format() const { return mFormat; }
    inline std::string formatDate() const { return mFormatDate; }

    int cardCount(const std::string& card);

private:
    Format mFormat;
    std::string mFormatDate;
};

}

#endif
