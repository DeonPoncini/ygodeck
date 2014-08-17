#include "DB.h"

#include <stdexcept>

namespace ygo
{

std::string operator_to_string(Operator op)
{
    switch (op)
    {
        case Operator::LT: return "<";
        case Operator::LTE: return "<=";
        case Operator::GT: return ">";
        case Operator::GTE: return ">=";
        case Operator::EQ: return "==";
        case Operator::NEQ: return "!=";
    }
}

std::string DBEsc(const std::string& s)
{
    return std::string("'") + s + "'";
}

std::string DBPair(const std::string& key, const std::string& value)
{
    return key + "= " + DBEsc(value);
}

std::string DBFuzzyPair(const std::string& key, const std::string& value)
{
    return key + " LIKE " + DBEsc(std::string("%") + value + "%");
}

std::string DBNull()
{
    return "NULL";
}

std::string DBTrue()
{
    return "1";
}

std::string DBFalse()
{
    return "0";
}

std::string DBAll()
{
    return "*";
}

std::string DBList(const std::vector<std::string>& list)
{
    std::string s;
    if (list.empty())
    {
        return s;
    }

    s += DBNull();
    for (auto&& l : list)
    {
        s += "," + DBEsc(l);
    }
    return s;
}

std::string DBKeyList(const std::vector<std::string>& list)
{
    std::string s;
    if (list.empty())
    {
        return s;
    }

    s += DBNull();
    for (auto&& l : list)
    {
        s += "," + l;
    }
    return s;
}

std::string DBUnique(const std::string& s)
{
    return std::string("DISTINCT ") + s;
}

std::string DBAnd(const std::vector<std::string>& list)
{
    if (list.empty())
    {
        return "1";
    }
    std::string where = "(";
    where += list[0];
    for (auto i = 1; i < list.size(); ++i)
    {
        where += " AND " + list[i];
    }
    where += ")";
    return where;
}

std::string DBOr(const std::vector<std::string>& list)
{
    if (list.empty())
    {
        return "1";
    }
    std::string where = "(";
    where += list[0];
    for (auto i = 1; i < list.size(); ++i)
    {
        where += " OR " + list[i];
    }
    where += ")";
    return where;
}

DB::DB(const char* name)
{
    if (sqlite3_open(name,&mDB))
    {
        throw std::runtime_error(std::string("Can't open database ") + name);
    }
}

DB::~DB()
{
    sqlite3_close(mDB);
}

int callback(void* d, int argc, char** argv, char** azColName)
{
    auto db = static_cast<DB*>(d);
    // pack the map with the return values
    DB::DataMap ret;
    for (auto i = 0; i < argc; ++i)
    {
        ret.insert({azColName[i],argv[i] ? argv[i] : "NULL"});
    }
    db->mCBf(ret);
    return 0;
}

void DB::select(const std::string& select, const std::string& from,
            const std::string& where, DataMapFn f)
{
    std::string sql("SELECT ");
    sql += select + " FROM " + from + " WHERE " + where;
    exec(sql,f);
}

void DB::insert(const std::string& insert, const std::string& values)
{
    std::string sql("INSERT INTO ");
    sql += insert + " VALUES(" + values + ")";
    exec(sql,[&](DB::DataMap){});
}

void DB::update(const std::string& update, const std::string& set,
        const std::string& where)
{
    std::string sql("UPDATE ");
    sql += update + " SET " + set + " WHERE " + where;
    exec(sql,[&](DB::DataMap){});
}

void DB::del(const std::string& del, const std::string& where)
{
    std::string sql("DELETE FROM ");
    sql += del + " WHERE " + where;
    exec(sql,[&](DB::DataMap){});
}

void DB::exec(const std::string& statement, DataMapFn f)
{
    mCBf = f;
    char* zErrMsg = nullptr;
    if (sqlite3_exec(mDB,statement.c_str(),callback,
                static_cast<void*>(this),
                &zErrMsg) != SQLITE_OK)
    {
        std::string err(zErrMsg);
        sqlite3_free(zErrMsg);
        throw std::runtime_error(err);
    }
}

}
