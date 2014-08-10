#ifndef YGO_DB_H
#define YGO_DB_H

#include <sqlite3.h>

#include <functional>
#include <string>
#include <unordered_map>

namespace ygo
{

enum class Operator
{
    LT,
    LTE,
    GT,
    GTE,
    EQ,
    NEQ,
};

std::string operator_to_string(Operator op);

class DB
{
public:
    typedef std::unordered_map<const char*, const char*> DataMap;
    typedef std::function<void(DataMap)> DataMapFn;

    explicit DB(const char* name);
    DB(const DB&) = delete;
    DB(DB&&) = delete;
    ~DB();

    void select(const std::string& select, const std::string& from,
            const std::string& where, DataMapFn f);
    void insert(const std::string& insert, const std::string& values,
            DataMapFn f);
    void update(const std::string& update, const std::string& set,
            const std::string& where, DataMapFn f);
    void del(const std::string& del, const std::string& where, DataMapFn f);

    void exec(const std::string& statement, DataMapFn f);

private:
    friend int callback(void* l, int argc, char** argv, char** azColName);

    sqlite3* mDB;
    DataMapFn mCBf;

};
}

#endif
