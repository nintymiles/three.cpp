#ifndef EVENT_DISPATCHER_H
#define EVENT_DISPATCHER_H
#include <vector>
#include <map>
#include <string>
#include <algorithm>
#include <memory>

#include "event_listener.h";
/**
 * https://github.com/mrdoob/eventdispatcher.js/
 */
class EventDispatcher;

struct Event{
	std::string type;
	std::shared_ptr<EventDispatcher> target;

	bool operator==(const Event &rhs){
		return type == rhs.type;
	}
};


class EventDispatcher {
    std::map<std::string,std::vector<EventListener*>> _listeners = {};
    //std::vector<Event> _listeners = {};
	public:
		EventDispatcher(){};

		void addEventListener( std::string type,EventListener* listener ) {
			std::vector<EventListener*>& listeners = _listeners[ type ];
			auto findResult = std::find(begin(listeners),end(listeners),listener);

			if(findResult!=std::end(listeners)){
				events.push_back( listener );
			}
		}

		bool hasEventListener( std::string type,EventListener* listener ) {
			std::vector<EventListener*>& listeners = _listeners[ type ];
			auto findResult = std::find(begin(listeners),end(listeners),listener);

			return findResult!=std::end(listeners) ;

		}

		void removeEventListener( std::string type, EventListener* listener ) {
			std::vector<EventListener*>& listeners = _listeners[ type ];
			if(events.size() == 0) return;
			
			auto findResult = std::find(begin(listeners),end(listeners),listener);

			if(findResult!=std::end(listeners)){
				events.erase(findResult);
			}

		}

		void dispatchEvent( Event event ) {

			// if ( this._listeners === undefined ) return;

			// const listeners = this._listeners;
			// const listenerArray = listeners[ event.type ];

			// if ( listenerArray !== undefined ) {

			// 	event.target = this;

			// 	// Make a copy, in case listeners are removed while iterating.
			// 	const array = listenerArray.slice( 0 );

			// 	for ( let i = 0, l = array.length; i < l; i ++ ) {

			// 		array[ i ].call( this, event );

			// 	}

			// 	event.target = null;

			// }

		}

};


#endif /* EVENT_DISPATCHER_H */
