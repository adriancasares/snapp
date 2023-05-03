#include "value/dataType.h"

namespace Snapp {

    BaseDataType findBaseDataType(const std::string& name) {
        auto it = nameToBaseDataType.find(name);
        return it == nameToBaseDataType.end() ? BaseDataType::Object : it->second;
    }

    DataType::DataType(BaseDataType base, std::string name, bool live) {
        base_ = base;
        name_ = name;
        live_ = live;
    }

    BaseDataType DataType::base() const {
        return base_;
    }

    const std::string& DataType::name() const {
        return name_;
    }

    bool DataType::live() const {
        return live_;
    }

    DataType DataType::Void(BaseDataType::Void, "void");
    DataType DataType::Int(BaseDataType::Int, "int");
    DataType DataType::Float(BaseDataType::Float, "float");
    DataType DataType::Bool(BaseDataType::Bool, "bool");
    DataType DataType::Str(BaseDataType::Str, "str");

    std::ostream& operator<<(std::ostream& out, const DataType& dataType) {
        return out << "DataType{" << (dataType.live() ? "@" : "") << dataType.name() << "}";
    }

}