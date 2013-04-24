/** @file AllocationFailure.h
 * Thrown when failed to allocate the dynamic space to a variable.
 * e.g., a = new int; and a == NULL
 */
 
#include <string>

#ifndef __ALLOCATIONFAILURE_H__
#define __ALLOCATIONFAILURE_H__

class AllocationFailure {
public: AllocationFailure() {}
    AllocationFailure(std::string msg) : msg(msg) {}
    std::string getMessage() const { return msg; }
private:
    std::string msg;
};
#endif
