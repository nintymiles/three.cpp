#ifndef SRC_CORE_LAYERS_H_
#define SRC_CORE_LAYERS_H_

struct Layers {
    unsigned mask;

	Layers():mask( 1|0 ){};

	Layers& set( unsigned channel ) {

		mask = ( 1 << channel | 0 ) >> 0;
		return *this;
	}

	Layers& enable( unsigned channel ) {
		mask |= 1 << channel | 0;
		return *this;
	}

	Layers& enableAll() {
		mask = 0xffffffff | 0;
		return *this;
	}

	Layers& toggle( unsigned channel ) {
		mask ^= 1 << channel | 0;
		return *this;
	}

	Layers& disable( unsigned channel ) {
		mask &= ~ ( 1 << channel | 0 );
		return *this;
	}

	Layers& disableAll() {
		mask = 0;
		return *this;
	}

	bool test( Layers& layers ) {
		return ( mask & layers.mask ) != 0;
	}

	bool isEnabled( unsigned channel ) {
		return ( mask & ( 1 << channel | 0 ) ) != 0;
	}

};

#endif /* SRC_CORE_LAYERS_H_ */
