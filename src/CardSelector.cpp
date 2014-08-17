#include "CardSelector.h"

#include "Serialize.h"

namespace ygo
{

CardSelector::CardSelector() :
    mDB("card.db")
{
}

CardSelector::CardList CardSelector::execute()
{
    CardList list;

    mDB.select("name","card",DBAnd(mQuery)
            ,[&](DB::DataMap data)
            {
                for (auto&& kv : data)
                {
                    list.push_back(kv.second);
                }
            });
    mQuery.clear();
    return list;
}

CardSelector& CardSelector::name(const std::string& like)
{
    mQuery.emplace_back(DBFuzzyPair("name",like));
    return *this;
}

CardSelector& CardSelector::cardType(CardType ct)
{
    mQuery.emplace_back(DBPair("cardType",fromCardType(ct)));
    return *this;
}

CardSelector& CardSelector::attribute(Attribute a)
{
    mQuery.emplace_back(DBPair("attribute",fromAttribute(a)));
    return *this;
}

CardSelector& CardSelector::monsterType(MonsterType mt)
{
    mQuery.emplace_back(DBPair("monsterType",fromMonsterType(mt)));
    return *this;
}

CardSelector& CardSelector::type(Type t)
{
    mQuery.emplace_back(DBPair("type",fromType(t)));
    return *this;
}

CardSelector& CardSelector::monsterAbility(MonsterAbility ma)
{
    mQuery.emplace_back(DBPair("monsterAbility",fromMonsterAbility(ma)));
    return *this;
}

CardSelector& CardSelector::level(int l, Operator op)
{
    mQuery.emplace_back(DBPair("level",operator_to_string(op) +
                std::to_string(l)));
    return *this;
}

CardSelector& CardSelector::attack(int a, Operator op)
{
    mQuery.emplace_back(DBPair("attack",operator_to_string(op) +
                std::to_string(a)));
    return *this;
}

CardSelector& CardSelector::defense(int d, Operator op)
{
    mQuery.emplace_back(DBPair("defense",operator_to_string(op) +
                std::to_string(d)));
    return *this;
}

CardSelector& CardSelector::lpendulum(int d, Operator op)
{
    mQuery.emplace_back(DBPair("lpendulum",operator_to_string(op) +
                std::to_string(d)));
    return *this;
}

CardSelector& CardSelector::rpendulum(int d, Operator op)
{
    mQuery.emplace_back(DBPair("rpendulum",operator_to_string(op) +
                std::to_string(d)));
    return *this;
}

CardSelector& CardSelector::spellType(SpellType st)
{
    mQuery.emplace_back(DBPair("spellType",fromSpellType(st)));
    return *this;
}

CardSelector& CardSelector::trapType(TrapType tt)
{
    mQuery.emplace_back(DBPair("trapType",fromTrapType(tt)));
    return *this;
}

}
