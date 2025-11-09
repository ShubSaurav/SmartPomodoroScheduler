#pragma once
#include <string>
#include <vector>
#include <map>
#include <sstream>
#include <iomanip>

class JSON {
public:
    // Force string literal to std::string conversion for operator[]
    JSON& operator[](const char* key) {
        return operator[](std::string(key));
    }

    const JSON& operator[](const char* key) const {
        return operator[](std::string(key));
    }

    enum Type {
        NULL_TYPE,
        BOOL_TYPE,
        NUMBER_TYPE,
        STRING_TYPE,
        ARRAY_TYPE,
        OBJECT_TYPE
    };

private:
    Type type;
    bool bool_value;
    double number_value;
    std::string string_value;
    std::vector<JSON> array_value;
    std::map<std::string, JSON> object_value;

public:
    JSON() : type(NULL_TYPE) {}
    JSON(bool v) : type(BOOL_TYPE), bool_value(v) {}
    JSON(int v) : type(NUMBER_TYPE), number_value(v) {}
    JSON(double v) : type(NUMBER_TYPE), number_value(v) {}
    JSON(const char* v) : type(STRING_TYPE), string_value(v) {}
    JSON(const std::string& v) : type(STRING_TYPE), string_value(v) {}
    JSON(long v) : type(NUMBER_TYPE), number_value(static_cast<double>(v)) {}

    static JSON Array() {
        JSON arr;
        arr.type = ARRAY_TYPE;
        return arr;
    }

    static JSON Object() {
        JSON obj;
        obj.type = OBJECT_TYPE;
        return obj;
    }

    void push_back(const JSON& value) {
        if (type != ARRAY_TYPE) type = ARRAY_TYPE;
        array_value.push_back(value);
    }

    JSON& operator[](const std::string& key) {
        if (type != OBJECT_TYPE) type = OBJECT_TYPE;
        return object_value[key];
    }

    const JSON& operator[](const std::string& key) const {
        static JSON null_value;
        if (type != OBJECT_TYPE) return null_value;
        auto it = object_value.find(key);
        if (it == object_value.end()) return null_value;
        return it->second;
    }

    std::string dump() const {
        std::ostringstream ss;
        switch (type) {
            case NULL_TYPE:
                ss << "null";
                break;
            case BOOL_TYPE:
                ss << (bool_value ? "true" : "false");
                break;
            case NUMBER_TYPE:
                ss << number_value;
                break;
            case STRING_TYPE:
                ss << '"' << escape(string_value) << '"';
                break;
            case ARRAY_TYPE:
                ss << '[';
                for (size_t i = 0; i < array_value.size(); ++i) {
                    if (i > 0) ss << ',';
                    ss << array_value[i].dump();
                }
                ss << ']';
                break;
            case OBJECT_TYPE:
                ss << '{';
                bool first = true;
                for (const auto& pair : object_value) {
                    if (!first) ss << ',';
                    ss << '"' << escape(pair.first) << "\":" << pair.second.dump();
                    first = false;
                }
                ss << '}';
                break;
        }
        return ss.str();
    }

private:
    static std::string escape(const std::string& s) {
        std::ostringstream ss;
        for (char c : s) {
            switch (c) {
                case '"': ss << "\\\""; break;
                case '\\': ss << "\\\\"; break;
                case '\b': ss << "\\b"; break;
                case '\f': ss << "\\f"; break;
                case '\n': ss << "\\n"; break;
                case '\r': ss << "\\r"; break;
                case '\t': ss << "\\t"; break;
                default:
                    if (c >= 0 && c < 32) {
                        ss << "\\u" << std::hex << std::setw(4) << std::setfill('0') << (int)c;
                    } else {
                        ss << c;
                    }
            }
        }
        return ss.str();
    }

public:
    // Type conversion operators
    operator bool() const { return bool_value; }
    operator int() const { return static_cast<int>(number_value); }
    operator double() const { return number_value; }
    operator std::string() const { return string_value; }
    operator long() const { return static_cast<long>(number_value); }
};