#include "syntax/dataType.h"

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

    std::ostream& operator<<(std::ostream& out, const DataType& dataType) {
        return out << "DataType{" << (dataType.live() ? "@" : "") << dataType.name() << "}";
    }

}