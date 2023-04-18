#ifndef SYNTAX_DATATYPE_H_
#define SYNTAX_DATATYPE_H_

#include <iostream>
#include <string>
#include <map>
#include <variant>

namespace Snapp {

    using DataValue = std::variant<int, double, bool, std::string>;

    enum class BaseDataType {
        Unknown,
        Void,
        Int,
        Float,
        Bool,
        Str,
        Object,
    };

    const std::map<std::string, BaseDataType> nameToBaseDataType = {
        {"void", BaseDataType::Void},
        {"int", BaseDataType::Int},
        {"float", BaseDataType::Float},
        {"bool", BaseDataType::Bool},
        {"str", BaseDataType::Str},
        {"Object", BaseDataType::Object},
    };

    BaseDataType findBaseDataType(const std::string& name);

    class DataType {
    public:
        DataType(BaseDataType base, std::string name, bool live);

        BaseDataType base() const;
        const std::string& name() const;
        bool live() const;

    private:
        BaseDataType base_;
        std::string name_;
        bool live_;
    };

    std::ostream& operator<<(std::ostream& out, const DataType& dataType);

}

#endif /* SYNTAX_DATATYPE_H_ */