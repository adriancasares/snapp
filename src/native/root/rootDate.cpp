#include "native/root/rootIO.h"
#include "value/dataType.h"
#include "semantics/astRunner.h"
#include "native/root/rootDate.h"
#include "value/import.h"

namespace Snapp::Native {

    int getCurrentUnix() {
        std::time_t now = std::time(nullptr);
        std::chrono::seconds seconds_since_epoch = std::chrono::seconds(now);
        int seconds = seconds_since_epoch.count();

        return seconds;
    }

    int getCurrentMs() {
        std::time_t now = std::time(nullptr);
        auto ms = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count() % 1000;

        return static_cast<int>(ms);
    }

    NativeFunction createConstructor() {
        return {
                                        DataType::Void,
                                        {},
                                        [](const std::vector<DataValue> &args,
                                            Scope *scope) -> std::optional<DataValue> {
                                                scope->assign("unix_", getCurrentUnix());
                                                scope->assign("millis_", getCurrentMs());

                                                return std::nullopt;
                                        }
                                    };
    }

    FunctionValue createGetTotalSeconds () {
        FunctionValue getTotalSeconds;
        getTotalSeconds.addOverload({
                                        DataType::Int,
                                        {},
                                        [](const std::vector<DataValue> &args,
                                            Scope *scope) -> std::optional<DataValue> {
                                                return std::get<int>(scope->get("unix_"));
                                        }
                                    });
        return getTotalSeconds;
    }

    FunctionValue createGetMillis() {
        FunctionValue getMillis;
        getMillis.addOverload({
                                        DataType::Int,
                                        {},
                                        [](const std::vector<DataValue> &args,
                                            Scope *scope) -> std::optional<DataValue> {
                                                return std::get<int>(scope->get("millis_"));
                                        }
                                    });
        return getMillis;
    }

    ClassValue* createRootDate() {
        ClassValue* dateClass = new ClassValue("Date");

        ClassAttribute unixAttribute = {
            DataType::Int,
            new SyntaxNodeLiteral(0)
        };

        ClassAttribute millisecondAttribute = {
            DataType::Int,
            new SyntaxNodeLiteral(0)
        };

        dateClass->add("unix_", unixAttribute);
        dateClass->add("millis_", millisecondAttribute);

        dateClass->constructor().addOverload(createConstructor());

        dateClass->add("getTotalSeconds", createGetTotalSeconds());
        dateClass->add("getMillis", createGetMillis());

        return dateClass;
    }

}