//
// Created by SeanR on 2023/12/19.
//

#ifndef THREE_CPP_SHADER_DATA_H
#define THREE_CPP_SHADER_DATA_H

#include "shader_material.h"
#include "uniform_values.h"

#include "vector2.h"

struct ShaderData{
    static ShaderMaterial::sptr getCopyShader(){
        ShaderMaterial::sptr copyShader = std::make_shared<ShaderMaterial>();

        UniformValues::sptr uniforms =std::make_shared<UniformValues>();
        uniforms->set<Texture::sptr>("tDiffuse", nullptr);
        uniforms->set("opacity",1.0f);

        copyShader->uniforms = uniforms;

        copyShader->vertexShader = R""(

            varying vec2 vUv;

            void main() {

                vUv = uv;
                gl_Position = projectionMatrix * modelViewMatrix * vec4( position, 1.0 );

            }

        )"";

        copyShader->fragmentShader = R""(

            uniform float opacity;

            uniform sampler2D tDiffuse;

            varying vec2 vUv;

            void main() {

                gl_FragColor = texture2D( tDiffuse, vUv );
                gl_FragColor.a *= opacity;


            }

        )"";

        return copyShader;
    }

    static ShaderMaterial::sptr getDotScreenShader(){
        ShaderMaterial::sptr shader = std::make_shared<ShaderMaterial>();

        UniformValues::sptr uniforms =std::make_shared<UniformValues>();
        uniforms->set<Texture::sptr>("tDiffuse", nullptr);
        uniforms->set("tSize",Vector2( 256, 256 ));
        uniforms->set("center",Vector2( 0.5, 0.5 ));
        uniforms->set("angle",1.57f);
        uniforms->set("scale",1.f);

        shader->uniforms = uniforms;

        shader->vertexShader = R""(

            varying vec2 vUv;

            void main() {

                vUv = uv;
                gl_Position = projectionMatrix * modelViewMatrix * vec4( position, 1.0 );

            }

        )"";

        shader->fragmentShader = R""(

            uniform vec2 center;
            uniform float angle;
            uniform float scale;
            uniform vec2 tSize;

            uniform sampler2D tDiffuse;

            varying vec2 vUv;

            float pattern() {

                float s = sin( angle ), c = cos( angle );

                vec2 tex = vUv * tSize - center;
                vec2 point = vec2( c * tex.x - s * tex.y, s * tex.x + c * tex.y ) * scale;

                return ( sin( point.x ) * sin( point.y ) ) * 4.0;

            }

            void main() {

                vec4 color = texture2D( tDiffuse, vUv );

                float average = ( color.r + color.g + color.b ) / 3.0;

                gl_FragColor = vec4( vec3( average * 10.0 - 5.0 + pattern() ), color.a );

            }

        )"";

        return shader;
    }

    static ShaderMaterial::sptr getRGBShiftShader(){
        ShaderMaterial::sptr shader = std::make_shared<ShaderMaterial>();

        UniformValues::sptr uniforms =std::make_shared<UniformValues>();
        uniforms->set<Texture::sptr>("tDiffuse", nullptr);
        uniforms->set("amount",.005f);
        uniforms->set("angle",.0f);

        shader->uniforms = uniforms;

        shader->vertexShader = R""(

            varying vec2 vUv;

            void main() {

                vUv = uv;
                gl_Position = projectionMatrix * modelViewMatrix * vec4( position, 1.0 );

            }

        )"";

        shader->fragmentShader = R""(

            uniform sampler2D tDiffuse;
            uniform float amount;
            uniform float angle;

            varying vec2 vUv;

            void main() {

                vec2 offset = amount * vec2( cos(angle), sin(angle));
                vec4 cr = texture2D(tDiffuse, vUv + offset);
                vec4 cga = texture2D(tDiffuse, vUv);
                vec4 cb = texture2D(tDiffuse, vUv - offset);
                gl_FragColor = vec4(cr.r, cga.g, cb.b, cga.a);

            }

        )"";

        return shader;
    }

    static ShaderMaterial::sptr getPixelShader(){
        ShaderMaterial::sptr shader = std::make_shared<ShaderMaterial>();

        UniformValues::sptr uniforms =std::make_shared<UniformValues>();
        uniforms->set<Texture::sptr>("tDiffuse", nullptr);
        uniforms->set("resolution", Vector2(0,0));
        uniforms->set("pixelSize",1.f);

        shader->uniforms = uniforms;

        shader->vertexShader = R""(

            varying highp vec2 vUv;

            void main() {

                vUv = uv;
                gl_Position = projectionMatrix * modelViewMatrix * vec4( position, 1.0 );

            }

        )"";

        shader->fragmentShader = R""(

            uniform sampler2D tDiffuse;
            uniform float pixelSize;
            uniform vec2 resolution;

            varying highp vec2 vUv;

            void main(){

                vec2 dxy = pixelSize / resolution;
                vec2 coord = dxy * floor( vUv / dxy );
                gl_FragColor = texture2D(tDiffuse, coord);

            }

        )"";

        return shader;
    }

    /**
    * RGB Halftone shader for three.js.
    *	NOTE:
    * 		Shape (1 = Dot, 2 = Ellipse, 3 = Line, 4 = Square)
    *		Blending Mode (1 = Linear, 2 = Multiply, 3 = Add, 4 = Lighter, 5 = Darker)
    */
    static ShaderMaterial::sptr getHalfToneShader(){
        ShaderMaterial::sptr shader = std::make_shared<ShaderMaterial>();

        UniformValues::sptr uniforms =std::make_shared<UniformValues>();
        uniforms->set<Texture::sptr>("tDiffuse", nullptr);
        uniforms->set("resolution", Vector2(0,0));
        uniforms->set("shape",1);
        uniforms->set("radius",4.f);
        uniforms->set("rotateR",(float)math_number::PI / 12 * 1);
        uniforms->set("rotateG",(float)math_number::PI / 12 * 2);
        uniforms->set("rotateB",(float)math_number::PI / 12 * 3);
        uniforms->set("scatter",0.f);
        uniforms->set("width",1.f);
        uniforms->set("height",1.f);
        uniforms->set("blending",1.f);
        uniforms->set("blendingMode",1);
        uniforms->set("greyscale",false);
        uniforms->set("disable",false);

        shader->uniforms = uniforms;

        shader->vertexShader = R""(

            varying vec2 vUV;

            void main() {

                vUV = uv;
                gl_Position = projectionMatrix * modelViewMatrix * vec4(position, 1.0);

            }

        )"";

        shader->fragmentShader = R""(

        #define SQRT2_MINUS_ONE 0.41421356
		#define SQRT2_HALF_MINUS_ONE 0.20710678
		#define PI2 6.28318531
		#define SHAPE_DOT 1
		#define SHAPE_ELLIPSE 2
		#define SHAPE_LINE 3
		#define SHAPE_SQUARE 4
		#define BLENDING_LINEAR 1
		#define BLENDING_MULTIPLY 2
		#define BLENDING_ADD 3
		#define BLENDING_LIGHTER 4
		#define BLENDING_DARKER 5
		uniform sampler2D tDiffuse;
		uniform float radius;
		uniform float rotateR;
		uniform float rotateG;
		uniform float rotateB;
		uniform float scatter;
		uniform float width;
		uniform float height;
		uniform int shape;
		uniform bool disable;
		uniform float blending;
		uniform int blendingMode;
		varying vec2 vUV;
		uniform bool greyscale;
		const int samples = 8;

		float blend( float a, float b, float t ) {

		// linear blend
			return a * ( 1.0 - t ) + b * t;

		}

		float hypot( float x, float y ) {

		// vector magnitude
			return sqrt( x * x + y * y );

		}

		float rand( vec2 seed ){

		// get pseudo-random number
			return fract( sin( dot( seed.xy, vec2( 12.9898, 78.233 ) ) ) * 43758.5453 );

		}

		float distanceToDotRadius( float channel, vec2 coord, vec2 normal, vec2 p, float angle, float rad_max ) {

		// apply shape-specific transforms
			float dist = hypot( coord.x - p.x, coord.y - p.y );
			float rad = channel;

			if ( shape == SHAPE_DOT ) {

				rad = pow( abs( rad ), 1.125 ) * rad_max;

			} else if ( shape == SHAPE_ELLIPSE ) {

				rad = pow( abs( rad ), 1.125 ) * rad_max;

				if ( dist != 0.0 ) {
					float dot_p = abs( ( p.x - coord.x ) / dist * normal.x + ( p.y - coord.y ) / dist * normal.y );
					dist = ( dist * ( 1.0 - SQRT2_HALF_MINUS_ONE ) ) + dot_p * dist * SQRT2_MINUS_ONE;
				}

			} else if ( shape == SHAPE_LINE ) {

				rad = pow( abs( rad ), 1.5) * rad_max;
				float dot_p = ( p.x - coord.x ) * normal.x + ( p.y - coord.y ) * normal.y;
				dist = hypot( normal.x * dot_p, normal.y * dot_p );

			} else if ( shape == SHAPE_SQUARE ) {

				float theta = atan( p.y - coord.y, p.x - coord.x ) - angle;
				float sin_t = abs( sin( theta ) );
				float cos_t = abs( cos( theta ) );
				rad = pow( abs( rad ), 1.4 );
				rad = rad_max * ( rad + ( ( sin_t > cos_t ) ? rad - sin_t * rad : rad - cos_t * rad ) );

			}

			return rad - dist;

		}

		struct Cell {

		// grid sample positions
			vec2 normal;
			vec2 p1;
			vec2 p2;
			vec2 p3;
			vec2 p4;
			float samp2;
			float samp1;
			float samp3;
			float samp4;

		};

		vec4 getSample( vec2 point ) {

		// multi-sampled point
			vec4 tex = texture2D( tDiffuse, vec2( point.x / width, point.y / height ) );
			float base = rand( vec2( floor( point.x ), floor( point.y ) ) ) * PI2;
			float step = PI2 / float( samples );
			float dist = radius * 0.66;

			for ( int i = 0; i < samples; ++i ) {

				float r = base + step * float( i );
				vec2 coord = point + vec2( cos( r ) * dist, sin( r ) * dist );
				tex += texture2D( tDiffuse, vec2( coord.x / width, coord.y / height ) );

			}

			tex /= float( samples ) + 1.0;
			return tex;

		}

		float getDotColour( Cell c, vec2 p, int channel, float angle, float aa ) {

		// get colour for given point
			float dist_c_1, dist_c_2, dist_c_3, dist_c_4, res;

			if ( channel == 0 ) {

				c.samp1 = getSample( c.p1 ).r;
				c.samp2 = getSample( c.p2 ).r;
				c.samp3 = getSample( c.p3 ).r;
				c.samp4 = getSample( c.p4 ).r;

			} else if (channel == 1) {

				c.samp1 = getSample( c.p1 ).g;
				c.samp2 = getSample( c.p2 ).g;
				c.samp3 = getSample( c.p3 ).g;
				c.samp4 = getSample( c.p4 ).g;

			} else {

				c.samp1 = getSample( c.p1 ).b;
				c.samp3 = getSample( c.p3 ).b;
				c.samp2 = getSample( c.p2 ).b;
				c.samp4 = getSample( c.p4 ).b;

			}

			dist_c_1 = distanceToDotRadius( c.samp1, c.p1, c.normal, p, angle, radius );
			dist_c_2 = distanceToDotRadius( c.samp2, c.p2, c.normal, p, angle, radius );
			dist_c_3 = distanceToDotRadius( c.samp3, c.p3, c.normal, p, angle, radius );
			dist_c_4 = distanceToDotRadius( c.samp4, c.p4, c.normal, p, angle, radius );
			res = ( dist_c_1 > 0.0 ) ? clamp( dist_c_1 / aa, 0.0, 1.0 ) : 0.0;
			res += ( dist_c_2 > 0.0 ) ? clamp( dist_c_2 / aa, 0.0, 1.0 ) : 0.0;
			res += ( dist_c_3 > 0.0 ) ? clamp( dist_c_3 / aa, 0.0, 1.0 ) : 0.0;
			res += ( dist_c_4 > 0.0 ) ? clamp( dist_c_4 / aa, 0.0, 1.0 ) : 0.0;
			res = clamp( res, 0.0, 1.0 );

			return res;

		}

		Cell getReferenceCell( vec2 p, vec2 origin, float grid_angle, float step ) {

		// get containing cell
			Cell c;

		// calc grid
			vec2 n = vec2( cos( grid_angle ), sin( grid_angle ) );
			float threshold = step * 0.5;
			float dot_normal = n.x * ( p.x - origin.x ) + n.y * ( p.y - origin.y );
			float dot_line = -n.y * ( p.x - origin.x ) + n.x * ( p.y - origin.y );
			vec2 offset = vec2( n.x * dot_normal, n.y * dot_normal );
			float offset_normal = mod( hypot( offset.x, offset.y ), step );
			float normal_dir = ( dot_normal < 0.0 ) ? 1.0 : -1.0;
			float normal_scale = ( ( offset_normal < threshold ) ? -offset_normal : step - offset_normal ) * normal_dir;
			float offset_line = mod( hypot( ( p.x - offset.x ) - origin.x, ( p.y - offset.y ) - origin.y ), step );
			float line_dir = ( dot_line < 0.0 ) ? 1.0 : -1.0;
			float line_scale = ( ( offset_line < threshold ) ? -offset_line : step - offset_line ) * line_dir;

		// get closest corner
			c.normal = n;
			c.p1.x = p.x - n.x * normal_scale + n.y * line_scale;
			c.p1.y = p.y - n.y * normal_scale - n.x * line_scale;

		// scatter
			if ( scatter != 0.0 ) {

				float off_mag = scatter * threshold * 0.5;
				float off_angle = rand( vec2( floor( c.p1.x ), floor( c.p1.y ) ) ) * PI2;
				c.p1.x += cos( off_angle ) * off_mag;
				c.p1.y += sin( off_angle ) * off_mag;

			}

		// find corners
			float normal_step = normal_dir * ( ( offset_normal < threshold ) ? step : -step );
			float line_step = line_dir * ( ( offset_line < threshold ) ? step : -step );
			c.p2.x = c.p1.x - n.x * normal_step;
			c.p2.y = c.p1.y - n.y * normal_step;
			c.p3.x = c.p1.x + n.y * line_step;
			c.p3.y = c.p1.y - n.x * line_step;
			c.p4.x = c.p1.x - n.x * normal_step + n.y * line_step;
			c.p4.y = c.p1.y - n.y * normal_step - n.x * line_step;

			return c;

		}

		float blendColour( float a, float b, float t ) {

		// blend colours
			if ( blendingMode == BLENDING_LINEAR ) {
				return blend( a, b, 1.0 - t );
			} else if ( blendingMode == BLENDING_ADD ) {
				return blend( a, min( 1.0, a + b ), t );
			} else if ( blendingMode == BLENDING_MULTIPLY ) {
				return blend( a, max( 0.0, a * b ), t );
			} else if ( blendingMode == BLENDING_LIGHTER ) {
				return blend( a, max( a, b ), t );
			} else if ( blendingMode == BLENDING_DARKER ) {
				return blend( a, min( a, b ), t );
			} else {
				return blend( a, b, 1.0 - t );
			}

		}

		void main() {

			if ( ! disable ) {

		// setup
				vec2 p = vec2( vUV.x * width, vUV.y * height );
				vec2 origin = vec2( 0, 0 );
				float aa = ( radius < 2.5 ) ? radius * 0.5 : 1.25;

		// get channel samples
				Cell cell_r = getReferenceCell( p, origin, rotateR, radius );
				Cell cell_g = getReferenceCell( p, origin, rotateG, radius );
				Cell cell_b = getReferenceCell( p, origin, rotateB, radius );
				float r = getDotColour( cell_r, p, 0, rotateR, aa );
				float g = getDotColour( cell_g, p, 1, rotateG, aa );
				float b = getDotColour( cell_b, p, 2, rotateB, aa );

		// blend with original
				vec4 colour = texture2D( tDiffuse, vUV );
				r = blendColour( r, colour.r, blending );
				g = blendColour( g, colour.g, blending );
				b = blendColour( b, colour.b, blending );

				if ( greyscale ) {
					r = g = b = (r + b + g) / 3.0;
				}

				gl_FragColor = vec4( r, g, b, 1.0 );

			} else {

				gl_FragColor = texture2D( tDiffuse, vUV );

			}

		}

        )"";

        return shader;
    }

    /**
     * Sobel Edge Detection (see https://youtu.be/uihBwtPIBxM)
     *
     * As mentioned in the video the Sobel operator expects a grayscale image as input.
     *
     */
    static ShaderMaterial::sptr getSobelShader(){
        ShaderMaterial::sptr shader = std::make_shared<ShaderMaterial>();

        UniformValues::sptr uniforms =std::make_shared<UniformValues>();
        uniforms->set<Texture::sptr>("tDiffuse", nullptr);
        uniforms->set("resolution", Vector2(0,0));

        shader->uniforms = uniforms;

        shader->vertexShader = R""(

            varying vec2 vUv;

            void main() {

                vUv = uv;
                gl_Position = projectionMatrix * modelViewMatrix * vec4( position, 1.0 );

            }

        )"";

        shader->fragmentShader = R""(

            uniform sampler2D tDiffuse;
            uniform vec2 resolution;
            varying vec2 vUv;

            void main() {

                vec2 texel = vec2( 1.0 / resolution.x, 1.0 / resolution.y );

            // kernel definition (in glsl matrices are filled in column-major order)

                const mat3 Gx = mat3( -1, -2, -1, 0, 0, 0, 1, 2, 1 ); // x direction kernel
                const mat3 Gy = mat3( -1, 0, 1, -2, 0, 2, -1, 0, 1 ); // y direction kernel

            // fetch the 3x3 neighbourhood of a fragment

            // first column

                float tx0y0 = texture2D( tDiffuse, vUv + texel * vec2( -1, -1 ) ).r;
                float tx0y1 = texture2D( tDiffuse, vUv + texel * vec2( -1,  0 ) ).r;
                float tx0y2 = texture2D( tDiffuse, vUv + texel * vec2( -1,  1 ) ).r;

            // second column

                float tx1y0 = texture2D( tDiffuse, vUv + texel * vec2(  0, -1 ) ).r;
                float tx1y1 = texture2D( tDiffuse, vUv + texel * vec2(  0,  0 ) ).r;
                float tx1y2 = texture2D( tDiffuse, vUv + texel * vec2(  0,  1 ) ).r;

            // third column

                float tx2y0 = texture2D( tDiffuse, vUv + texel * vec2(  1, -1 ) ).r;
                float tx2y1 = texture2D( tDiffuse, vUv + texel * vec2(  1,  0 ) ).r;
                float tx2y2 = texture2D( tDiffuse, vUv + texel * vec2(  1,  1 ) ).r;

            // gradient value in x direction

                float valueGx = Gx[0][0] * tx0y0 + Gx[1][0] * tx1y0 + Gx[2][0] * tx2y0 +
                    Gx[0][1] * tx0y1 + Gx[1][1] * tx1y1 + Gx[2][1] * tx2y1 +
                    Gx[0][2] * tx0y2 + Gx[1][2] * tx1y2 + Gx[2][2] * tx2y2;

            // gradient value in y direction

                float valueGy = Gy[0][0] * tx0y0 + Gy[1][0] * tx1y0 + Gy[2][0] * tx2y0 +
                    Gy[0][1] * tx0y1 + Gy[1][1] * tx1y1 + Gy[2][1] * tx2y1 +
                    Gy[0][2] * tx0y2 + Gy[1][2] * tx1y2 + Gy[2][2] * tx2y2;

            // magnitute of the total gradient

                float G = sqrt( ( valueGx * valueGx ) + ( valueGy * valueGy ) );

                gl_FragColor = vec4( vec3( G ), 1 );

            }

        )"";

        return shader;
    }

    static ShaderMaterial::sptr getLuminosityShader(){
        ShaderMaterial::sptr shader = std::make_shared<ShaderMaterial>();

        UniformValues::sptr uniforms =std::make_shared<UniformValues>();
        uniforms->set<Texture::sptr>("tDiffuse", nullptr);

        shader->uniforms = uniforms;

        shader->vertexShader = R""(

            varying vec2 vUv;

            void main() {

                vUv = uv;
                gl_Position = projectionMatrix * modelViewMatrix * vec4( position, 1.0 );

            }

        )"";

        shader->fragmentShader = R""(

            #include <common>

            uniform sampler2D tDiffuse;

            varying vec2 vUv;

            void main() {

                vec4 texel = texture2D( tDiffuse, vUv );

                float l = luminance( texel.rgb );

                gl_FragColor = vec4( l, l, l, texel.w );

            }

        )"";

        return shader;
    }

    /**
     * Afterimage shader
     * I created this effect inspired by a demo on codepen:
     * https://codepen.io/brunoimbrizi/pen/MoRJaN?page=1&
     */
    static ShaderMaterial::sptr getAfterImageShader(){
        ShaderMaterial::sptr shader = std::make_shared<ShaderMaterial>();

        UniformValues::sptr uniforms =std::make_shared<UniformValues>();
        uniforms->set("damp", .96f);
        uniforms->set<Texture::sptr>("tOld", nullptr);
        uniforms->set<Texture::sptr>("tNew", nullptr);

        shader->uniforms = uniforms;

        shader->vertexShader = R""(

            varying vec2 vUv;

            void main() {

                vUv = uv;
                gl_Position = projectionMatrix * modelViewMatrix * vec4( position, 1.0 );

            }

        )"";

        shader->fragmentShader = R""(

            uniform float damp;

            uniform sampler2D tOld;
            uniform sampler2D tNew;

            varying vec2 vUv;

            vec4 when_gt( vec4 x, float y ) {

                return max( sign( x - y ), 0.0 );

            }

            void main() {

                vec4 texelOld = texture2D( tOld, vUv );
                vec4 texelNew = texture2D( tNew, vUv );

                texelOld *= damp * when_gt( texelOld, 0.1 );

                gl_FragColor = max(texelNew, texelOld);

            }

        )"";

        return shader;
    }

    /**
    * RGB Shift Shader
    * Shifts red and blue channels from center in opposite directions
    * Ported from http://kriss.cx/tom/2009/05/rgb-shift/
    * by Tom Butterworth / http://kriss.cx/tom/
    *
    * amount: shift distance (1 is width of input)
    * angle: shift angle in radians
    */
    static ShaderMaterial::sptr getDigitalGlitchShader(){
        ShaderMaterial::sptr shader = std::make_shared<ShaderMaterial>();

        UniformValues::sptr uniforms =std::make_shared<UniformValues>();
        uniforms->set<Texture::sptr>("tDiffuse", nullptr); //diffuse texture
        uniforms->set<Texture::sptr>("tDisp", nullptr); //displacement texture for digital glitch squares
        uniforms->set("byp", 0);
        uniforms->set("amount", .08f);
        uniforms->set("angle", .02f);
        uniforms->set("seed", .02f);
        uniforms->set("seed_x", .02f);
        uniforms->set("seed_y", .02f);
        uniforms->set("distortion_x", .5f);
        uniforms->set("distortion_y", .6f);
        uniforms->set("col_s", .05f);

        shader->uniforms = uniforms;

        shader->vertexShader = R""(

                varying vec2 vUv;
                void main() {
                    vUv = uv;
                    gl_Position = projectionMatrix * modelViewMatrix * vec4( position, 1.0 );
                }
        )"";


        shader->fragmentShader = R""(

                uniform int byp; //should we apply the glitch ?

                uniform sampler2D tDiffuse;
                uniform sampler2D tDisp;

                uniform float amount;
                uniform float angle;
                uniform float seed;
                uniform float seed_x;
                uniform float seed_y;
                uniform float distortion_x;
                uniform float distortion_y;
                uniform float col_s;

                varying vec2 vUv;


                float rand(vec2 co){
                    return fract(sin(dot(co.xy ,vec2(12.9898,78.233))) * 43758.5453);
                }

                void main() {
                    if(byp<1) {
                        vec2 p = vUv;
                        float xs = floor(gl_FragCoord.x / 0.5);
                        float ys = floor(gl_FragCoord.y / 0.5);
                        //based on staffantans glitch shader for unity https://github.com/staffantan/unityglitch
                        float disp = texture2D(tDisp, p*seed*seed).r;
                        if(p.y<distortion_x+col_s && p.y>distortion_x-col_s*seed) {
                            if(seed_x>0.){
                                p.y = 1. - (p.y + distortion_y);
                            }
                            else {
                                p.y = distortion_y;
                            }
                        }
                        if(p.x<distortion_y+col_s && p.x>distortion_y-col_s*seed) {
                            if(seed_y>0.){
                                p.x=distortion_x;
                            }
                            else {
                                p.x = 1. - (p.x + distortion_x);
                            }
                        }
                        p.x+=disp*seed_x*(seed/5.);
                        p.y+=disp*seed_y*(seed/5.);
                        //base from RGB shift shader
                        vec2 offset = amount * vec2( cos(angle), sin(angle));
                        vec4 cr = texture2D(tDiffuse, p + offset);
                        vec4 cga = texture2D(tDiffuse, p);
                        vec4 cb = texture2D(tDiffuse, p - offset);
                        gl_FragColor = vec4(cr.r, cga.g, cb.b, cga.a);
                        //add noise
                        vec4 snow = 200.*amount*vec4(rand(vec2(xs * seed,ys * seed*50.))*0.2);
                        gl_FragColor = gl_FragColor+ snow;
                    }
                    else {
                        gl_FragColor=texture2D (tDiffuse, vUv);
                    }
                }
        )"";

        return shader;
    }

};




#endif //THREE_CPP_SHADER_DATA_H
