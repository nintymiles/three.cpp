#ifndef EVENT_DISPATCHER_H
#define EVENT_DISPATCHER_H
#include <vector>
#include <map>
#include <string>
#include <algorithm>

class EventDispatcher;
/**
 * https://github.com/mrdoob/eventdispatcher.js/
 */
struct Event{
	std::string type;
	std::shared_ptr<EventDispatcher> target;

	bool operator==(const Event &rhs){
		return type == rhs.type;
	}
};


class EventDispatcher {
    std::map<std::string,std::vector<Event>> _listeners = {};
    //std::vector<Event> _listeners = {};
	public:
		EventDispatcher(){};

		void addEventListener( std::string type,Event listener ) {
			std::vector<Event>& events = _listeners[ type ];
			auto findResult = std::find(begin(events),end(events),listener);

			if(findResult!=std::end(events)){
				events.push_back( listener );
			}
		}

		bool hasEventListener( std::string type,Event listener ) {
			std::vector<Event>& events = _listeners[ type ];
			auto findResult = std::find(begin(events),end(events),listener);

			return findResult!=std::end(events) ;

		}

		void removeEventListener( std::string type, Event listener ) {
			std::vector<Event>& events = _listeners[ type ];
			if(events.size() == 0) return;
			
			auto findResult = std::find(begin(events),end(events),listener);

			if(findResult!=std::end(events)){
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
