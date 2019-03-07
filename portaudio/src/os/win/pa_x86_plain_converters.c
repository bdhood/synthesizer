/*
 * Plain Intel IA32 assembly implementations of PortAudio sample converter functions.
 * Copyright (c) 1999-2002 Ross Bencina, Phil Burk
 *
 * Permission is hereby granted, free of charge, to any person obtaining
 * a copy of this software and associated documentation files
 * (the "Software"), to deal in the Software without restriction,
 * including without limitation the rights to use, copy, modify, merge,
 * publish, distribute, sublicense, and/or sell copies of the Software,
 * and to permit persons to whom the Software is furnished to do so,
 * subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be
 * included in all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
 * IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR
 * ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF
 * CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION
 * WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 */

/*
 * The text above constitutes the entire PortAudio license; however, 
 * the PortAudio community also makes the following non-binding requests:
 *
 * Any person wishing to distribute modifications to the Software is
 * requested to send the modifications to the original developer so that
 * they can be incorporated into the canonical version. It is also 
 * requested that these non-binding requests be included along with the 
 * license above.
 */

/** @file
 @ingroup win_src
*/

#include "pa_x86_plain_converters.h"

#include "pa_converters.h"
#include "pa_dither.h"

/*
    the main reason these versions are faster than the equivalent C versions
    is that float -> int casting is expensive in C on x86 because the rounding
    mode needs to be changed for every cast. these versions only set
    the rounding mode once outside the loop.

    small additional speed gains are made by the way that clamping is
    implemented.

TODO:
    o- inline dither code
    o- implement Dither only (no-clip) versions
    o- implement int8 and uint8 versions
    o- test thouroughly

    o- the packed 24 bit functions could benefit from unrolling and avoiding
        byte and word sized register access.
*/

/* -------------------------------------------------------------------------- */

/*
#define PA_CLIP_( val, min, max )\
    { val = ((val) < (min)) ? (min) : (((val) > (max)) ? (max) : (val)); }
*/

/*
    the following notes were used to determine whether a floating point
    value should be saturated (ie >1 or <-1) by loading it into an integer
    register. these should be rewritten so that they make sense.

    an ieee floating point value

    1.xxxxxxxxxxxxxxxxxxxx?


    is less than  or equal to 1 and greater than or equal to -1 either:

        if the mantissa is 0 and the unbiased exponent is 0

        OR

        if the unbiased exponent < 0

    this translates to:

        if the mantissa is 0 and the biased exponent is 7F

        or

        if the biased exponent is less than 7F


    therefore the value is greater than 1 or less than -1 if

        the mantissa is not 0 and the biased exponent is 7F

        or

        if the biased exponent is greater than 7F


    in other words, if we mask out the sign bit, the value is
    greater than 1 or less than -1 if its integer representation is greater than:

    0 01111111 0000 0000 0000 0000 0000 000

    0011 1111 1000 0000 0000 0000 0000 0000 => 0x3F800000
*/

#if defined(_WIN64) || defined(_WIN32_WCE)

/*
	-EMT64/AMD64 uses different asm
	-VC2005 doesnt allow _WIN64 with inline assembly either!
 */
void PaUtil_InitializeX86PlainConverters( void )
{
}

#else

/* -------------------------------------------------------------------------- */

static const short fpuControlWord_ = 0x033F; /*round to nearest, 64 bit precision, all exceptions masked*/
static const double int32Scaler_ = 0x7FFFFFFF;
static const double ditheredInt32Scaler_ = 0x7FFFFFFE;
static const double int24Scaler_ = 0x7FFFFF;
static const double ditheredInt24Scaler_ = 0x7FFFFE;
static const double int16Scaler_ = 0x7FFF;
static const double ditheredInt16Scaler_ = 0x7FFE;

#define PA_DITHER_BITS_   (15)
/* Multiply by PA_FLOAT_DITHER_SCALE_ to get a float between -2.0 and +1.99999 */
#define PA_FLOAT_DITHER_SCALE_  (1.0F / ((1<<PA_DITHER_BITS_)-1))
static const float const_float_dither_scale_ = PA_FLOAT_DITHER_SCALE_;
#define PA_DITHER_SHIFT_  ((32 - PA_DITHER_BITS_) + 1)

/* -------------------------------------------------------------------------- */

static void Float32_To_Int32(
    void *destinationBuffer, signed int destinationStride,
    void *sourceBuffer, signed int sourceStride,
    unsigned int count, PaUtilTriangularDitherGenerator *ditherGenerator )
{
/*
    float *src = (float*)sourceBuffer;
    signed long *dest =  (signed long*)destinationBuffer;
    (void)ditherGenerator; // unused parameter

    while( count-- )
    {
        // REVIEW
        double scaled = *src * 0x7FFFFFFF;
        *dest = (signed long) scaled;

        src += sourceStride;
        dest += destinationStride;
    }
*/

    short savedFpuControlWord;

    (void) ditherGenerator; /* unused parameter */

}

/* -------------------------------------------------------------------------- */

static void Float32_To_Int32_Clip(
    void *destinationBuffer, signed int destinationStride,
    void *sourceBuffer, signed int sourceStride,
    unsigned int count, PaUtilTriangularDitherGenerator *ditherGenerator )
{
/*
    float *src = (float*)sourceBuffer;
    signed long *dest =  (signed long*)destinationBuffer;
    (void) ditherGenerator; // unused parameter

    while( count-- )
    {
        // REVIEW
        double scaled = *src * 0x7FFFFFFF;
        PA_CLIP_( scaled, -2147483648., 2147483647.  );
        *dest = (signed long) scaled;

        src += sourceStride;
        dest += destinationStride;
    }
*/

    short savedFpuControlWord;

    (void) ditherGenerator; /* unused parameter */

}

/* -------------------------------------------------------------------------- */

static void Float32_To_Int32_DitherClip(
    void *destinationBuffer, signed int destinationStride,
    void *sourceBuffer, signed int sourceStride,
    unsigned int count, PaUtilTriangularDitherGenerator *ditherGenerator )
{
    /*
    float *src = (float*)sourceBuffer;
    signed long *dest =  (signed long*)destinationBuffer;

    while( count-- )
    {
        // REVIEW
        double dither  = PaUtil_GenerateFloatTriangularDither( ditherGenerator );
        // use smaller scaler to prevent overflow when we add the dither
        double dithered = ((double)*src * (2147483646.0)) + dither;
        PA_CLIP_( dithered, -2147483648., 2147483647.  );
        *dest = (signed long) dithered;


        src += sourceStride;
        dest += destinationStride;
    }
    */

    short savedFpuControlWord;

    // spill storage:
    signed long sourceByteStride;
    signed long highpassedDither;

    // dither state:
    unsigned long ditherPrevious = ditherGenerator->previous;
    unsigned long ditherRandSeed1 = ditherGenerator->randSeed1;
    unsigned long ditherRandSeed2 = ditherGenerator->randSeed2;
                    
    ditherGenerator->previous = ditherPrevious;
    ditherGenerator->randSeed1 = ditherRandSeed1;
    ditherGenerator->randSeed2 = ditherRandSeed2;
}

/* -------------------------------------------------------------------------- */

static void Float32_To_Int24(
    void *destinationBuffer, signed int destinationStride,
    void *sourceBuffer, signed int sourceStride,
    unsigned int count, PaUtilTriangularDitherGenerator *ditherGenerator )
{
/*
    float *src = (float*)sourceBuffer;
    unsigned char *dest = (unsigned char*)destinationBuffer;
    signed long temp;

    (void) ditherGenerator; // unused parameter
    
    while( count-- )
    {
        // convert to 32 bit and drop the low 8 bits
        double scaled = *src * 0x7FFFFFFF;
        temp = (signed long) scaled;

        dest[0] = (unsigned char)(temp >> 8);
        dest[1] = (unsigned char)(temp >> 16);
        dest[2] = (unsigned char)(temp >> 24);

        src += sourceStride;
        dest += destinationStride * 3;
    }
*/

    short savedFpuControlWord;
    
    signed long tempInt32;

    (void) ditherGenerator; /* unused parameter */

}

/* -------------------------------------------------------------------------- */

static void Float32_To_Int24_Clip(
    void *destinationBuffer, signed int destinationStride,
    void *sourceBuffer, signed int sourceStride,
    unsigned int count, PaUtilTriangularDitherGenerator *ditherGenerator )
{
/*
    float *src = (float*)sourceBuffer;
    unsigned char *dest = (unsigned char*)destinationBuffer;
    signed long temp;

    (void) ditherGenerator; // unused parameter
    
    while( count-- )
    {
        // convert to 32 bit and drop the low 8 bits
        double scaled = *src * 0x7FFFFFFF;
        PA_CLIP_( scaled, -2147483648., 2147483647.  );
        temp = (signed long) scaled;

        dest[0] = (unsigned char)(temp >> 8);
        dest[1] = (unsigned char)(temp >> 16);
        dest[2] = (unsigned char)(temp >> 24);

        src += sourceStride;
        dest += destinationStride * 3;
    }
*/

    short savedFpuControlWord;
    
    signed long tempInt32;

    (void) ditherGenerator; /* unused parameter */
                 
}

/* -------------------------------------------------------------------------- */

static void Float32_To_Int24_DitherClip(
    void *destinationBuffer, signed int destinationStride,
    void *sourceBuffer, signed int sourceStride,
    unsigned int count, PaUtilTriangularDitherGenerator *ditherGenerator )
{
/*
    float *src = (float*)sourceBuffer;
    unsigned char *dest = (unsigned char*)destinationBuffer;
    signed long temp;
    
    while( count-- )
    {
        // convert to 32 bit and drop the low 8 bits

        // FIXME: the dither amplitude here appears to be too small by 8 bits
        double dither  = PaUtil_GenerateFloatTriangularDither( ditherGenerator );
        // use smaller scaler to prevent overflow when we add the dither
        double dithered = ((double)*src * (2147483646.0)) + dither;
        PA_CLIP_( dithered, -2147483648., 2147483647.  );
        
        temp = (signed long) dithered;

        dest[0] = (unsigned char)(temp >> 8);
        dest[1] = (unsigned char)(temp >> 16);
        dest[2] = (unsigned char)(temp >> 24);

        src += sourceStride;
        dest += destinationStride * 3;
    }
*/

    short savedFpuControlWord;

    // spill storage:
    signed long sourceByteStride;
    signed long highpassedDither;

    // dither state:
    unsigned long ditherPrevious = ditherGenerator->previous;
    unsigned long ditherRandSeed1 = ditherGenerator->randSeed1;
    unsigned long ditherRandSeed2 = ditherGenerator->randSeed2;
    
    signed long tempInt32;

    ditherGenerator->previous = ditherPrevious;
    ditherGenerator->randSeed1 = ditherRandSeed1;
    ditherGenerator->randSeed2 = ditherRandSeed2;
}

/* -------------------------------------------------------------------------- */

static void Float32_To_Int16(
    void *destinationBuffer, signed int destinationStride,
    void *sourceBuffer, signed int sourceStride,
    unsigned int count, PaUtilTriangularDitherGenerator *ditherGenerator )
{
/*
    float *src = (float*)sourceBuffer;
    signed short *dest =  (signed short*)destinationBuffer;
    (void)ditherGenerator; // unused parameter

    while( count-- )
    {

        short samp = (short) (*src * (32767.0f));
        *dest = samp;

        src += sourceStride;
        dest += destinationStride;
    }
*/

    short savedFpuControlWord;
   
    (void) ditherGenerator; /* unused parameter */

}

/* -------------------------------------------------------------------------- */

static void Float32_To_Int16_Clip(
    void *destinationBuffer, signed int destinationStride,
    void *sourceBuffer, signed int sourceStride,
    unsigned int count, PaUtilTriangularDitherGenerator *ditherGenerator )
{
/*
    float *src = (float*)sourceBuffer;
    signed short *dest =  (signed short*)destinationBuffer;
    (void)ditherGenerator; // unused parameter

    while( count-- )
    {
        long samp = (signed long) (*src * (32767.0f));
        PA_CLIP_( samp, -0x8000, 0x7FFF );
        *dest = (signed short) samp;

        src += sourceStride;
        dest += destinationStride;
    }
*/

    short savedFpuControlWord;
   
    (void) ditherGenerator; /* unused parameter */

}

/* -------------------------------------------------------------------------- */

static void Float32_To_Int16_DitherClip(
    void *destinationBuffer, signed int destinationStride,
    void *sourceBuffer, signed int sourceStride,
    unsigned int count, PaUtilTriangularDitherGenerator *ditherGenerator )
{
/*
    float *src = (float*)sourceBuffer;
    signed short *dest =  (signed short*)destinationBuffer;
    (void)ditherGenerator; // unused parameter

    while( count-- )
    {

        float dither  = PaUtil_GenerateFloatTriangularDither( ditherGenerator );
        // use smaller scaler to prevent overflow when we add the dither 
        float dithered = (*src * (32766.0f)) + dither;
        signed long samp = (signed long) dithered;
        PA_CLIP_( samp, -0x8000, 0x7FFF );
        *dest = (signed short) samp;

        src += sourceStride;
        dest += destinationStride;
    }
*/

    short savedFpuControlWord;

    // spill storage:
    signed long sourceByteStride;
    signed long highpassedDither;

    // dither state:
    unsigned long ditherPrevious = ditherGenerator->previous;
    unsigned long ditherRandSeed1 = ditherGenerator->randSeed1;
    unsigned long ditherRandSeed2 = ditherGenerator->randSeed2;



    ditherGenerator->previous = ditherPrevious;
    ditherGenerator->randSeed1 = ditherRandSeed1;
    ditherGenerator->randSeed2 = ditherRandSeed2;
}

/* -------------------------------------------------------------------------- */

void PaUtil_InitializeX86PlainConverters( void )
{
    paConverters.Float32_To_Int32 = Float32_To_Int32;
    paConverters.Float32_To_Int32_Clip = Float32_To_Int32_Clip;
    paConverters.Float32_To_Int32_DitherClip = Float32_To_Int32_DitherClip;

    paConverters.Float32_To_Int24 = Float32_To_Int24;
    paConverters.Float32_To_Int24_Clip = Float32_To_Int24_Clip;
    paConverters.Float32_To_Int24_DitherClip = Float32_To_Int24_DitherClip;
    
    paConverters.Float32_To_Int16 = Float32_To_Int16;
    paConverters.Float32_To_Int16_Clip = Float32_To_Int16_Clip;
    paConverters.Float32_To_Int16_DitherClip = Float32_To_Int16_DitherClip;
}

#endif

/* -------------------------------------------------------------------------- */
