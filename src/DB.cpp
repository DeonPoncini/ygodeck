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

void DB::insert(const std::string& insert, const std::string& values,
            DataMapFn f)
{
    std::string sql("INSERT INTO ");
    sql += insert + " VALUES(" + values + ")";
    exec(sql,f);
}

void DB::update(const std::string& update, const std::string& set,
            const std::string& where, DataMapFn f)
{
    std::string sql("UPDATE ");
    sql += update + " SET " + set + " WHERE " + where;
    exec(sql,f);
}

void DB::del(const std::string& del, const std::string& where, DataMapFn f)
{
    std::string sql("DELETE FROM ");
    sql += del + " WHERE " + where;
    exec(sql,f);
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
