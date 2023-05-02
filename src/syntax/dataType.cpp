#include "syntax/dataType.h"

namespace Snapp {

    BaseDataType findBaseDataType(const std::string& name) {
        auto it = nameToBaseDataType.find(name);
        return it == nameToBaseDataType.end() ? BaseDataType::Object : it->second;
    }

    DataType DataType::getDataType(const Snapp::DataValue &value) {
        if (std::holds_alternative<IntValue>(value)) {
            return DataType(BaseDataType::Int, "int", false);
        } else if (std::holds_alternative<FloatValue>(value)) {
            return DataType(BaseDataType::Float, "float", false);
        } else if (std::holds_alternative<BoolValue>(value)) {
            return DataType(BaseDataType::Bool, "bool", false);
        } else if (std::holds_alternative<StrValue>(value)) {
            return DataType(BaseDataType::Str, "str", false);
        } else if (std::holds_alternative<FunctionValue>(value)) {
            return DataType(BaseDataType::Function, "Function", false);
        } else if (std::holds_alternative<NativeFunctionValue>(value)) {
            return DataType(BaseDataType::Function, "Function", false);
        } else if (std::holds_alternative<FunctionGroup>(value)) {
            return DataType(BaseDataType::Function, "Function", false);
        } else {
            return DataType(BaseDataType::Unknown, "Unknown", false);
        }
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