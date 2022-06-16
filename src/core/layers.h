
struct Layers {
    int mask;

	Layers():mask( 1|0 ){};

	Layers& set( int channel ) {

		mask = ( 1 << channel | 0 ) >> 0;

	}

	Layers& enable( int channel ) {

		mask |= 1 << channel | 0;

	}

	Layers& enableAll() {

		mask = 0xffffffff | 0;

	}

	Layers& toggle( int channel ) {

		mask ^= 1 << channel | 0;

	}

	Layers& disable( int channel ) {

		mask &= ~ ( 1 << channel | 0 );

	}

	Layers& disableAll() {

		mask = 0;

	}

	bool test( Layers& layers ) {

		return ( mask & layers.mask ) != 0;

	}

	bool isEnabled( int channel ) {

		return ( mask & ( 1 << channel | 0 ) ) != 0;

	}

};
