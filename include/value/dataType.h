#ifndef SNAPP_VALUE_DATATYPE_H_
#define SNAPP_VALUE_DATATYPE_H_

#include <iostream>
#include <string>
#include <map>

namespace Snapp {

    enum class BaseDataType {
        Unknown,
        Void,
        Int,
        Float,
        Bool,
        Str,
        Object,
        Function,
        Class,
        Import,
    };

    const std::map<std::string, BaseDataType> nameToBaseDataType = {
        {"void", BaseDataType::Void},
        {"int", BaseDataType::Int},
        {"float", BaseDataType::Float},
        {"bool", BaseDataType::Bool},
        {"str", BaseDataType::Str},
        {"Object", BaseDataType::Object},
        {"Function", BaseDataType::Function},
        {"Class", BaseDataType::Class},
        {"Import", BaseDataType::Import},
    };

    BaseDataType findBaseDataType(const std::string& name);

    class DataType {
    public:
        DataType(BaseDataType base, std::string name, bool live = false);

        BaseDataType base() const;
        const std::string& name() const;
        bool live() const;

        static DataType Void;
        static DataType Int;
        static DataType Float;
        static DataType Bool;
        static DataType Str;

        bool operator==(const DataType& other) const {
            return base_ == other.base_ && name_ == other.name_ && live_ == other.live_;
        }

        bool operator!=(const DataType& other) const {
            return !(*this == other);
        }

    private:
        BaseDataType base_;
        std::string name_;
        bool live_;
    };

    std::ostream& operator<<(std::ostream& out, const DataType& dataType);

}

#endif /* SNAPP_VALUE_DATATYPE_H_ */