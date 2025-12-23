// shared_defs.h
#include <boost/interprocess/sync/interprocess_mutex.hpp>
#include <boost/interprocess/sync/interprocess_condition.hpp>

struct SharedData {
    boost::interprocess::interprocess_mutex mutex;
    boost::interprocess::interprocess_condition cond;
    int counter;
};

