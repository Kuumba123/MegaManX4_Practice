/*
 * 2nd file due to a lack of space...
 */
#include <common.h>
#include <gpu.h>

#if BUILD == 561
#define textBufferAddress 0x80175f50 //before Arc Buffer
#else
#define textBufferAddress 0x80175ff8
#endif

#define MAX_CHAR_COUNT 255

int debugTextCount = 0;
char hexDigits[] = "0123456789ABCDEF";
char lowerhexDigits[] = "0123456789abcdef";
DR_TPAGE static debugfontTPage = {0, 0xE1000014};

void DebugTextFlush()
{
    DrawSync(0);

    if (debugTextCount != 0)
    {
        (((SPRT_8*)textBufferAddress) + (debugTextCount - 1))->tag = 0x03FFFFFF;
        debugTextCount = 0;
        catPrim(&debugfontTPage, textBufferAddress);
        setlen(&debugfontTPage, 1);
        DrawOTag(&debugfontTPage);
        DrawSync(0);
    }
}

void DrawChar(char letter, int destX, int destY, uint8_t clut)
{
    if (debugTextCount < MAX_CHAR_COUNT && letter != ' ')
    {
        letter -= 0x20;
        SPRT_8 *sp = ((SPRT_8*)textBufferAddress) + debugTextCount;
        setRGB0(sp, 128, 128, 128);
        setXY0(sp, destX, destY); // 32 Chars per row
        setUV0(sp, (letter & 0x1F) * 8, 0xE0 + (letter / 32) * 8);
        setClut(sp, 0x100 + clut * 16, 0x1F8);
        setSprt8(sp);
        catPrim(sp, sp + 1);
        debugTextCount += 1;
    }
}

#undef MAX_CHAR_COUNT
#undef textBufferAddress