#include "CardSelector.h"

#include "DBCommon.h"

#include <db/SQLite3.h>
#include <data/Serialize.h>

namespace ygo
{

CardSelector::CardSelector()
{
}

StaticCardData CardSelector::query(const std::string& name)
{
    StaticCardData s;
    db::SQLite3 db(DBPATH);
    db.select(db::DBAll(),"card",db::DBPair("name",name),
            [&](db::SQLite3::DataMap data)
            {
                s.name = name;
                s.cardType = toCardType(data["cardType"]);
                s.attribute = toAttribute(data["attribute"]);
                s.monsterType = toMonsterType(data["monsterType"]);
                s.type = toType(data["type"]);
                s.monsterAbility = toMonsterType(data["monsterAbility"]);
                s.level = std::atoi(data["level"].c_str());
                s.attack = std::atoi(data["attack"].c_str());
                s.defense = std::atoi(data["defense"].c_str());
                s.lpendulum = std::atoi(data["lpendulum"].c_str());
                s.rpendulum = std::atoi(data["rpendulum"].c_str());
                s.spellType = toSpellType(data["spellType"]);
                s.trapType = toTrapType(data["trapType"]);
            });
    return s;
}

CardSelector::CardList CardSelector::execute()
{
    CardList list;

    db::SQLite3 db(DBPATH);
    db.select("name","card",db::DBAnd(mQuery)
            ,[&](db::SQLite3::DataMap data)
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
    mQuery.emplace_back(db::DBFuzzyPair("name",like));
    return *this;
}

CardSelector& CardSelector::cardType(CardType ct)
{
    mQuery.emplace_back(db::DBPair("cardType",fromCardType(ct)));
    return *this;
}

CardSelector& CardSelector::attribute(Attribute a)
{
    mQuery.emplace_back(db::DBPair("attribute",fromAttribute(a)));
    return *this;
}

CardSelector& CardSelector::monsterType(MonsterType mt)
{
    mQuery.emplace_back(db::DBPair("monsterType",fromMonsterType(mt)));
    return *this;
}

CardSelector& CardSelector::type(Type t)
{
    mQuery.emplace_back(db::DBPair("type",fromType(t)));
    return *this;
}

CardSelector& CardSelector::level(int l, db::Operator op)
{
    mQuery.emplace_back(db::DBPair("level",operator_to_string(op) +
                std::to_string(l)));
    return *this;
}

CardSelector& CardSelector::attack(int a, db::Operator op)
{
    mQuery.emplace_back(db::DBPair("attack",operator_to_string(op) +
                std::to_string(a)));
    return *this;
}

CardSelector& CardSelector::defense(int d, db::Operator op)
{
    mQuery.emplace_back(db::DBPair("defense",operator_to_string(op) +
                std::to_string(d)));
    return *this;
}

CardSelector& CardSelector::lpendulum(int d, db::Operator op)
{
    mQuery.emplace_back(db::DBPair("lpendulum",operator_to_string(op) +
                std::to_string(d)));
    return *this;
}

CardSelector& CardSelector::rpendulum(int d, db::Operator op)
{
    mQuery.emplace_back(db::DBPair("rpendulum",operator_to_string(op) +
                std::to_string(d)));
    return *this;
}

CardSelector& CardSelector::spellType(SpellType st)
{
    mQuery.emplace_back(db::DBPair("spellType",fromSpellType(st)));
    return *this;
}

CardSelector& CardSelector::trapType(TrapType tt)
{
    mQuery.emplace_back(db::DBPair("trapType",fromTrapType(tt)));
    return *this;
}

}
