#include <ygo/deck/DB.h>

#include <kizhi/Log.h>

namespace ygo
{
namespace deck
{

DB& DB::get()
{
    static DB instance;
    return instance;
}

void DB::path(std::string db)
{
    mPath = std::move(db);
    KIZHI_TRACE_T("DB") << "Database path " << mPath;
}

}
}
