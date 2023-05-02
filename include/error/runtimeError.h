#ifndef SNAPP_ERROR_RUNTIMEERROR_H_
#define SNAPP_ERROR_RUNTIMEERROR_H_

#include <string>

namespace Snapp {

    class RuntimeError {
    public:
        RuntimeError() = default;
        RuntimeError(std::string message);
    
        const std::string& message() const;
    
        std::string output() const;
    
    private:
        std::string message_;
    };

}

#endif /* SNAPP_ERROR_RUNTIMEERROR_H_ */
