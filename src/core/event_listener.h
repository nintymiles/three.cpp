#ifndef SRC_CORE_EVENT_LISTENER_H_
#define SRC_CORE_EVENT_LISTENER_H_

#include "event_dispatcher.h"

class EventListener{
public:
    virtual void onDispatch(EventDispatcher* dispatcher,Event& event) = 0;
};

#endif /* SRC_CORE_EVENT_LISTENER_H_ */
