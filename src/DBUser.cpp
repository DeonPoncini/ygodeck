#include "DBUser.h"

namespace ygo
{

DBUser::DBUser(std::string name, std::string id) :
    mName(std::move(name)),
    mID(std::move(id))
{
}

}
