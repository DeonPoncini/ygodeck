#ifndef YGO_DECK_DB_H
#define YGO_DECK_DB_H

#include <string>

namespace ygo
{
namespace deck
{

class DB
{
public:
    static DB& get();

    void path(std::string db);
    inline std::string path() const { return mPath; }

private:
    DB() = default;
    std::string mPath;
};

}
}

#endif
