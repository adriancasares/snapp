#include "native/root/rootIO.h"
#include "value/dataType.h"
#include "semantics/astRunner.h"
#include "value/import.h"
#include <cctype>

namespace Snapp::Native {

    FunctionValue createLengthFunction() {
        FunctionValue lengthFunction;
        lengthFunction.addOverload({
            DataType::Int,
            {DataType::Str},
            [](const std::vector<DataValue>& args, Scope* scope) -> std::optional<DataValue> {
                std::string str = *coerceStr(args.at(0));
                int length = str.length();
                return length;
            }
        });
        return lengthFunction;
    }

    FunctionValue createSubstring() {
        FunctionValue substring;
        substring.addOverload({
            DataType::Str,
            {DataType::Str,DataType::Int,DataType::Int},
            [](const std::vector<DataValue>& args, Scope* scope) -> std::optional<DataValue>
            {
                std::string str = *coerceStr(args.at(0));
                int i = *coerceInt(args.at(1));
                int j = *coerceInt(args.at(2));
                return str.substr(i,j-i);
            }
        });
        return substring;
    }

    FunctionValue createReplaceFirst() {
        FunctionValue replaceFirst;
        replaceFirst.addOverload({
            DataType::Str,
            {DataType::Str,DataType::Str,DataType::Str},
            [](const std::vector<DataValue>& args, Scope* scope) -> std::optional<DataValue>
            {
                std::string str = *coerceStr(args.at(0));
                std::string find = *coerceStr(args.at(1));
                std::string replace = *coerceStr(args.at(2));
                int i = str.find(find);
                if(i != std::string::npos){
                    return str.replace(i,find.length(),replace);
                }
                return str;
            }
        });
        return replaceFirst;
    }

    FunctionValue createReplaceLast() {
        FunctionValue replaceLast;
        replaceLast.addOverload({
            DataType::Str,
            {DataType::Str,DataType::Str,DataType::Str},
            [](const std::vector<DataValue>& args, Scope* scope) -> std::optional<DataValue>
            {
              std::string str = *coerceStr(args.at(0));
              std::string find = *coerceStr(args.at(1));
              std::string replace = *coerceStr(args.at(2));
              int i = str.rfind(find);
              if(i != std::string::npos){
                return str.replace(i,find.length(),replace);
              }
              return str;
            }
        });
        return replaceLast;
    }

    FunctionValue createReplaceAll() {
        FunctionValue replaceAll;
        replaceAll.addOverload({
            DataType::Str,
            {DataType::Str,DataType::Str,DataType::Str},
            [](const std::vector<DataValue>& args, Scope* scope) -> std::optional<DataValue>
            {
              std::string str = *coerceStr(args.at(0));
              std::string find = *coerceStr(args.at(1));
              std::string replace = *coerceStr(args.at(2));
              int i = 0;
              while(str.find(find) !=std::string::npos){
                i = str.find(find);
                str.replace(i,find.length(),replace);
              }
              return str;
            }
        });
        return replaceAll;
    }

    FunctionValue createUppercase() {
        FunctionValue uppercase;
        uppercase.addOverload({
            DataType::Str,
            {DataType::Str},
            [](const std::vector<DataValue>& args, Scope* scope) -> std::optional<DataValue>
            {
              std::string str = *coerceStr(args.at(0));
              for(int i = 0; i < str.length(); i++){
                str[i] = toupper(str[i]);
              }
              return str;
            }
        });
        return uppercase;
    }

    FunctionValue createLowercase() {
        FunctionValue lowercase;
        lowercase.addOverload({
            DataType::Str,
            {DataType::Str},
            [](const std::vector<DataValue>& args, Scope* scope) -> std::optional<DataValue>
            {
              std::string str = *coerceStr(args.at(0));
              for(int i = 0; i < str.length(); i++){
                str[i] = tolower(str[i]);
              }
              return str;
            }
        });
        return lowercase;
    }

    FunctionValue createIsAlphabetical() {
      FunctionValue isAlphabetical;
      isAlphabetical.addOverload({
            DataType::Bool,
            {DataType::Str},
            [](const std::vector<DataValue>& args, Scope* scope) -> std::optional<DataValue>
            {
              std::string str = *coerceStr(args.at(0));
              for(int i = 0; i < str.length(); i++){
                if(!isalpha(str[i])){
                  return false;
                }
              }
              return true;
            }
        });
        return isAlphabetical;
    }

    FunctionValue createIsAlphanumeric() {
        FunctionValue isAlphanumeric;
        isAlphanumeric.addOverload({
            DataType::Bool,
            {DataType::Str},
            [](const std::vector<DataValue>& args, Scope* scope) -> std::optional<DataValue>
            {
              std::string str = *coerceStr(args.at(0));
              for(int i = 0; i < str.length(); i++){
                if(!isalnum(str[i])){
                  return false;
                }
              }
              return true;
            }
        });
        return isAlphanumeric;
    }

    FunctionValue createToInt() {
        FunctionValue toInt;
        toInt.addOverload({
            DataType::Int,
            {DataType::Str},
            [](const std::vector<DataValue>& args, Scope* scope) -> std::optional<DataValue>
            {
              std::string str = *coerceStr(args.at(0));
              return std::stoi(str);
            }
        });
        return toInt;
    }

    FunctionValue createToFloat() {
        FunctionValue toFloat;
        toFloat.addOverload({
            DataType::Float,
            {DataType::Str},
            [](const std::vector<DataValue>& args, Scope* scope) -> std::optional<DataValue>
            {
              std::string str = *coerceStr(args.at(0));
              return std::stod(str);
            }
        });
        return toFloat;
    }

    FunctionValue createCompare() {
        FunctionValue compare;
        compare.addOverload({
            DataType::Int,
            {DataType::Str,DataType::Str},
            [](const std::vector<DataValue>& args, Scope* scope) -> std::optional<DataValue>
            {
              std::string str1 = *coerceStr(args.at(0));
              std::string str2 = *coerceStr(args.at(1));
              return str1.compare(str2);
            }
        });
        return compare;
    }

    ImportValue* createRootString() {
        auto* scope = new Scope(nullptr, true);
        auto* rootString = new ImportValue(scope);

        scope->add("length", createLengthFunction());
        scope->add("substring",createSubstring());
        scope->add("replaceFirst", createReplaceFirst());
        scope->add("replaceLast",createReplaceLast());
        scope->add("replaceAll",createReplaceAll());
        scope->add("uppercase", createUppercase());
        scope->add("lowercase",createLowercase());
        scope->add("isAlphabetical",createIsAlphabetical());
        scope->add("isAlphanumeric",createIsAlphanumeric());
        scope->add("toInt",createToInt());
        scope->add("toFloat",createToFloat());
        scope->add("compare",createCompare());

        return rootString;
    }

}