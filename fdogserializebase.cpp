#include "fdogserializebase.h"

using namespace fsj;

FdogSerializeBase * FdogSerializeBase::fdogserializebase = nullptr;
mutex * FdogSerializeBase::mutex_base = new(mutex);

FdogSerializeStruct * FdogSerializeStruct::fdogserializestruct = nullptr;
mutex * FdogSerializeStruct::mutex_struct = new(mutex);

FdogSerializeBase * FdogSerializeBase::Instance(){
    mutex_base->lock();
    if(fdogserializebase == nullptr){
        fdogserializebase = new FdogSerializeBase();
    }
    mutex_base->unlock();
    return fdogserializebase;
}

FdogSerializeStruct * FdogSerializeStruct::Instance(){
    mutex_struct->lock();
    if(fdogserializestruct == nullptr){
        fdogserializestruct = new FdogSerializeStruct();
    }
    mutex_struct->unlock();
    return fdogserializestruct;
}

void * func(){
}