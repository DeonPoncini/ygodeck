#ifndef YGO_DECK_DECKERROR_H
#define YGO_DECK_DECKERROR_H

namespace ygo
{
namespace deck
{
enum class DeckError
{
    OK,
    FORBIDDEN,
    LIMIT_REACHED,
    DECK_FULL
};
}
}

#endif
