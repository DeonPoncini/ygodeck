#include <ygo/deck/DB.h>

namespace ygo
{
namespace deck
{

DB& DB::get()
{
    static DB instance;
    return instance;
}

}
}
