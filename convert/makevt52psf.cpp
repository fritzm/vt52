#include <fstream>
#include <iomanip>
#include <iostream>

#include "vt52rom.h"

using namespace std;

#define PSF2_MAGIC0     0x72
#define PSF2_MAGIC1     0xb5
#define PSF2_MAGIC2     0x4a
#define PSF2_MAGIC3     0x86

#define PSF2_MAXVERSION 0

struct psf2_header {
    uint8_t magic[4];
    int32_t version;
    int32_t headersize;    // offset of bitmaps in file
    int32_t flags;
    int32_t length;        // number of glyphs
    int32_t charsize;      // number of bytes for each character
    int32_t height, width; // max dimensions of glyphs
};

int main()
{
    ofstream g("vt52.psf");

    const int w = 18;
    const int h = 30;
    const int n = 256;

    psf2_header header{
        { PSF2_MAGIC0, PSF2_MAGIC1, PSF2_MAGIC2, PSF2_MAGIC3 },
        PSF2_MAXVERSION,
        sizeof(header),
        0,
        n,
        h * ((w+7)/8),
        h, w
    };

    g.write((const char *)&header, sizeof(header));

    for(int i=0; i<n; ++i) {
        for(int r=0; r<8; ++r) {
            int d = (i < 128) ? vt52rom[i*8 + r] : 0;

            uint8_t d0 = 0;
            d0 |= ((d & 0x01) ? 0x3 : 0x0) << 0;
            d0 |= ((d & 0x02) ? 0x3 : 0x0) << 2;
            d0 |= ((d & 0x04) ? 0x3 : 0x0) << 4;
            d0 |= ((d & 0x08) ? 0x3 : 0x0) << 6;

            uint8_t d1 = 0;
            d1 |= ((d & 0x10) ? 0x3 : 0x0) << 0;
            d1 |= ((d & 0x20) ? 0x3 : 0x0) << 2;
            d1 |= ((d & 0x40) ? 0x3 : 0x0) << 4;
            d1 |= ((d & 0x80) ? 0x3 : 0x0) << 6;

            g << static_cast<char>(d1) << static_cast<char>(d0) << static_cast<char>(0);
            g << static_cast<char>(d1) << static_cast<char>(d0) << static_cast<char>(0);
            g << static_cast<char>(d1) << static_cast<char>(d0) << static_cast<char>(0);
        }

        g << static_cast<char>(0) << static_cast<char>(0) << static_cast<char>(0);
        g << static_cast<char>(0) << static_cast<char>(0) << static_cast<char>(0);
        g << static_cast<char>(0) << static_cast<char>(0) << static_cast<char>(0);

        g << static_cast<char>(0) << static_cast<char>(0) << static_cast<char>(0);
        g << static_cast<char>(0) << static_cast<char>(0) << static_cast<char>(0);
        g << static_cast<char>(0) << static_cast<char>(0) << static_cast<char>(0);

    }

    return 0;
}
