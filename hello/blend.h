#ifndef __BLEND_H__
#define __BLEND_H__

/****
From http://stackoverflow.com/a/5925219/402692 answer to "How does photoshop blend two images together?"
****/

#define ChannelBlend_Normal(A,B)     ((uint8)(A))
#define ChannelBlend_Lighten(A,B)    ((uint8)((B > A) ? B:A))
#define ChannelBlend_Darken(A,B)     ((uint8)((B > A) ? A:B))
#define ChannelBlend_Multiply(A,B)   ((uint8)((A * B) / 255))
#define ChannelBlend_Average(A,B)    ((uint8)((A + B) / 2))
#define ChannelBlend_Add(A,B)        ((uint8)(min(255, (A + B))))
#define ChannelBlend_Subtract(A,B)   ((uint8)((A + B < 255) ? 0:(A + B - 255)))
#define ChannelBlend_Difference(A,B) ((uint8)(abs(A - B)))
#define ChannelBlend_Negation(A,B)   ((uint8)(255 - abs(255 - A - B)))
#define ChannelBlend_Screen(A,B)     ((uint8)(255 - (((255 - A) * (255 - B)) >> 8)))
#define ChannelBlend_Exclusion(A,B)  ((uint8)(A + B - 2 * A * B / 255))
#define ChannelBlend_Overlay(A,B)    ((uint8)((B < 128) ? (2 * A * B / 255):(255 - 2 * (255 - A) * (255 - B) / 255)))
#define ChannelBlend_SoftLight(A,B)  ((uint8)((B < 128)?(2*((A>>1)+64))*((float)B/255):(255-(2*(255-((A>>1)+64))*(float)(255-B)/255))))
#define ChannelBlend_HardLight(A,B)  (ChannelBlend_Overlay(B,A))
#define ChannelBlend_ColorDodge(A,B) ((uint8)((B == 255) ? B:min(255, ((A << 8 ) / (255 - B)))))
#define ChannelBlend_ColorBurn(A,B)  ((uint8)((B == 0) ? B:max(0, (255 - ((255 - A) << 8 ) / B))))
#define ChannelBlend_LinearDodge(A,B)(ChannelBlend_Add(A,B))
#define ChannelBlend_LinearBurn(A,B) (ChannelBlend_Subtract(A,B))
#define ChannelBlend_LinearLight(A,B)((uint8)(B < 128)?ChannelBlend_LinearBurn(A,(2 * B)):ChannelBlend_LinearDodge(A,(2 * (B - 128))))
#define ChannelBlend_VividLight(A,B) ((uint8)(B < 128)?ChannelBlend_ColorBurn(A,(2 * B)):ChannelBlend_ColorDodge(A,(2 * (B - 128))))
#define ChannelBlend_PinLight(A,B)   ((uint8)(B < 128)?ChannelBlend_Darken(A,(2 * B)):ChannelBlend_Lighten(A,(2 * (B - 128))))
#define ChannelBlend_HardMix(A,B)    ((uint8)((ChannelBlend_VividLight(A,B) < 128) ? 0:255))
#define ChannelBlend_Reflect(A,B)    ((uint8)((B == 255) ? B:min(255, (A * A / (255 - B)))))
#define ChannelBlend_Glow(A,B)       (ChannelBlend_Reflect(B,A))
#define ChannelBlend_Phoenix(A,B)    ((uint8)(min(A,B) - max(A,B) + 255))
#define ChannelBlend_Alpha(A,B,O)    ((uint8)(O * A + (1 - O) * B))
#define ChannelBlend_AlphaF(A,B,F,O) (ChannelBlend_Alpha(F(A,B),A,O))

#endif