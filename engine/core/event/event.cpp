#include "hzpch.h"
#include "event.h"


namespace RIZZ {
    std::ostream& operator<<(std::ostream& os, const EventArgs& e)
    {
        return os << e.ToString();
    }
}


