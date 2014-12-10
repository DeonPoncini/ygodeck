#include "CardSelector.h"

#include "Common.h"

#include <db/SQLite3.h>
#include <ygo/data/Serialize.h>

namespace ygo
{
namespace deck
{

CardSelector::CardSelector()
{
}

data::StaticCardData CardSelector::query(const std::string& name)
{
    data::StaticCardData s;
    db::SQLite3 db(DBPATH);
    db.select(db::All(),"card",db::Equal("name",name),
            [&](db::DataMap data) {
                s.name = name.c_str();
                s.cardType = data::toCardType(data["cardType"]);
                s.attribute = data::toAttribute(data["attribute"]);
                s.monsterType = data::toMonsterType(data["monsterType"]);
                s.type = data::toType(data["type"]);
                s.monsterAbility = data::toMonsterType(data["monsterAbility"]);
                s.level = std::atoi(data["level"].c_str());
                s.attack = std::atoi(data["attack"].c_str());
                s.defense = std::atoi(data["defense"].c_str());
                s.lpendulum = std::atoi(data["lpendulum"].c_str());
                s.rpendulum = std::atoi(data["rpendulum"].c_str());
                s.spellType = data::toSpellType(data["spellType"]);
                s.trapType = data::toTrapType(data["trapType"]);
            });
    return s;
}

CardSelector::CardList CardSelector::execute()
{
    CardList list;

    db::SQLite3 db(DBPATH);
    db.select("name","card",db::And(mQuery)
            ,[&](db::DataMap data) {
                for (auto&& kv : data) {
                    list.push_back(kv.second);
                }
            });
    mQuery.clear();
    return list;
}

CardSelector& CardSelector::name(const std::string& like)
{
    mQuery.emplace_back(db::Like("name",like));
    return *this;
}

CardSelector& CardSelector::cardType(data::CardType ct)
{
    mQuery.emplace_back(db::Equal("cardType",fromCardType(ct)));
    return *this;
}

CardSelector& CardSelector::attribute(data::Attribute a)
{
    mQuery.emplace_back(db::Equal("attribute",fromAttribute(a)));
    return *this;
}

CardSelector& CardSelector::monsterType(data::MonsterType mt)
{
    mQuery.emplace_back(db::Equal("monsterType",fromMonsterType(mt)));
    return *this;
}

CardSelector& CardSelector::type(data::Type t)
{
    mQuery.emplace_back(db::Equal("type",fromType(t)));
    return *this;
}

CardSelector& CardSelector::level(int l, db::Operator op)
{
    mQuery.emplace_back(db::Compare("level",std::to_string(l), op));
    return *this;
}

CardSelector& CardSelector::attack(int a, db::Operator op)
{
    mQuery.emplace_back(db::Compare("attack",std::to_string(a), op));
    return *this;
}

CardSelector& CardSelector::defense(int d, db::Operator op)
{
    mQuery.emplace_back(db::Compare("defense",std::to_string(d), op));
    return *this;
}

CardSelector& CardSelector::lpendulum(int d, db::Operator op)
{
    mQuery.emplace_back(db::Compare("lpendulum",std::to_string(d), op));
    return *this;
}

CardSelector& CardSelector::rpendulum(int d, db::Operator op)
{
    mQuery.emplace_back(db::Compare("rpendulum",std::to_string(d), op));
    return *this;
}

CardSelector& CardSelector::spellType(data::SpellType st)
{
    mQuery.emplace_back(db::Equal("spellType",fromSpellType(st)));
    return *this;
}

CardSelector& CardSelector::trapType(data::TrapType tt)
{
    mQuery.emplace_back(db::Equal("trapType",fromTrapType(tt)));
    return *this;
}

}
}
