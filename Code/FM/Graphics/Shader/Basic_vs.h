#if 0
//
// Generated by Microsoft (R) HLSL Shader Compiler 6.3.9600.16384
//
//
// Buffer Definitions: 
//
// cbuffer cbPerCamera
// {
//
//   float4x4 gViewProj;                // Offset:    0 Size:    64
//
// }
//
// cbuffer cbPerObject
// {
//
//   float4x4 gWorld;                   // Offset:    0 Size:    64
//   float4x4 gWorldInvTranspose;       // Offset:   64 Size:    64
//
// }
//
//
// Resource Bindings:
//
// Name                                 Type  Format         Dim Slot Elements
// ------------------------------ ---------- ------- ----------- ---- --------
// cbPerCamera                       cbuffer      NA          NA    0        1
// cbPerObject                       cbuffer      NA          NA    1        1
//
//
//
// Input signature:
//
// Name                 Index   Mask Register SysValue  Format   Used
// -------------------- ----- ------ -------- -------- ------- ------
// POSITION                 0   xyz         0     NONE   float   xyz 
// TEXCOORD                 0   xy          1     NONE   float   xy  
// NORMAL                   0   xyzw        2     NONE   float   xyzw
// BONEINDICES              0   xyzw        3     NONE    uint       
// WEIGHTS                  0   xyzw        4     NONE   float       
//
//
// Output signature:
//
// Name                 Index   Mask Register SysValue  Format   Used
// -------------------- ----- ------ -------- -------- ------- ------
// NORMAL                   0   xyz         0     NONE   float   xyz 
// TANGENT                  0   xyz         1     NONE   float   xyz 
// TEXCOORD                 0   xy          2     NONE   float   xy  
// SV_POSITION              0   xyzw        3      POS   float   xyzw
//
vs_5_0
dcl_globalFlags refactoringAllowed
dcl_constantbuffer cb0[4], immediateIndexed
dcl_constantbuffer cb1[7], immediateIndexed
dcl_input v0.xyz
dcl_input v1.xy
dcl_input v2.xyzw
dcl_output o0.xyz
dcl_output o1.xyz
dcl_output o2.xy
dcl_output_siv o3.xyzw, position
dcl_temps 2
dp2 r0.x, v2.xyxx, v2.xyxx
add r0.x, -r0.x, l(1.000000)
sqrt r0.x, r0.x
mul r0.yz, v2.xxzx, l(0.000000, 32767.000000, 32767.000000, 0.000000)
ftoi r0.yz, r0.yyzy
and r0.yz, r0.yyzy, l(0, 1, 1, 0)
movc r0.yz, r0.yyzy, l(0,1.000000,1.000000,0), l(0,-1.000000,-1.000000,0)
mul r1.z, r0.y, r0.x
mov r1.xy, v2.xyxx
dp3 o0.x, r1.xyzx, cb1[4].xyzx
dp3 o0.y, r1.xyzx, cb1[5].xyzx
dp3 o0.z, r1.xyzx, cb1[6].xyzx
dp2 r0.x, v2.zwzz, v2.zwzz
add r0.x, -r0.x, l(1.000000)
sqrt r0.x, r0.x
mul r0.z, r0.z, r0.x
mov r0.xy, v2.zwzz
dp3 o1.x, r0.xyzx, cb1[0].xyzx
dp3 o1.y, r0.xyzx, cb1[1].xyzx
dp3 o1.z, r0.xyzx, cb1[2].xyzx
mov o2.xy, v1.xyxx
mov r0.xyz, v0.xyzx
mov r0.w, l(1.000000)
dp4 r1.x, r0.xyzw, cb1[0].xyzw
dp4 r1.y, r0.xyzw, cb1[1].xyzw
dp4 r1.z, r0.xyzw, cb1[2].xyzw
dp4 r1.w, r0.xyzw, cb1[3].xyzw
dp4 o3.x, r1.xyzw, cb0[0].xyzw
dp4 o3.y, r1.xyzw, cb0[1].xyzw
dp4 o3.z, r1.xyzw, cb0[2].xyzw
dp4 o3.w, r1.xyzw, cb0[3].xyzw
ret 
// Approximately 32 instruction slots used
#endif

const BYTE Basic_vs_bytecodes[] =
{
     68,  88,  66,  67,  63,   2, 
    187, 111, 151,  24, 183,  76, 
     40, 253,  13, 209, 112,  42, 
    238,  57,   1,   0,   0,   0, 
     24,   8,   0,   0,   5,   0, 
      0,   0,  52,   0,   0,   0, 
    252,   1,   0,   0, 180,   2, 
      0,   0,  72,   3,   0,   0, 
    124,   7,   0,   0,  82,  68, 
     69,  70, 192,   1,   0,   0, 
      2,   0,   0,   0, 148,   0, 
      0,   0,   2,   0,   0,   0, 
     60,   0,   0,   0,   0,   5, 
    254, 255,   0,   1,   0,   0, 
    142,   1,   0,   0,  82,  68, 
     49,  49,  60,   0,   0,   0, 
     24,   0,   0,   0,  32,   0, 
      0,   0,  40,   0,   0,   0, 
     36,   0,   0,   0,  12,   0, 
      0,   0,   0,   0,   0,   0, 
    124,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      1,   0,   0,   0,   1,   0, 
      0,   0, 136,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   1,   0, 
      0,   0,   1,   0,   0,   0, 
      1,   0,   0,   0,  99,  98, 
     80, 101, 114,  67,  97, 109, 
    101, 114,  97,   0,  99,  98, 
     80, 101, 114,  79,  98, 106, 
    101,  99, 116,   0, 124,   0, 
      0,   0,   1,   0,   0,   0, 
    196,   0,   0,   0,  64,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   0,   0, 136,   0, 
      0,   0,   2,   0,   0,   0, 
     36,   1,   0,   0, 128,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   0,   0, 236,   0, 
      0,   0,   0,   0,   0,   0, 
     64,   0,   0,   0,   2,   0, 
      0,   0,   0,   1,   0,   0, 
      0,   0,   0,   0, 255, 255, 
    255, 255,   0,   0,   0,   0, 
    255, 255, 255, 255,   0,   0, 
      0,   0, 103,  86, 105, 101, 
    119,  80, 114, 111, 106,   0, 
    102, 108, 111,  97, 116,  52, 
    120,  52,   0, 171,   3,   0, 
      3,   0,   4,   0,   4,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
    246,   0,   0,   0, 116,   1, 
      0,   0,   0,   0,   0,   0, 
     64,   0,   0,   0,   2,   0, 
      0,   0,   0,   1,   0,   0, 
      0,   0,   0,   0, 255, 255, 
    255, 255,   0,   0,   0,   0, 
    255, 255, 255, 255,   0,   0, 
      0,   0, 123,   1,   0,   0, 
     64,   0,   0,   0,  64,   0, 
      0,   0,   2,   0,   0,   0, 
      0,   1,   0,   0,   0,   0, 
      0,   0, 255, 255, 255, 255, 
      0,   0,   0,   0, 255, 255, 
    255, 255,   0,   0,   0,   0, 
    103,  87, 111, 114, 108, 100, 
      0, 103,  87, 111, 114, 108, 
    100,  73, 110, 118,  84, 114, 
     97, 110, 115, 112, 111, 115, 
    101,   0,  77, 105,  99, 114, 
    111, 115, 111, 102, 116,  32, 
     40,  82,  41,  32,  72,  76, 
     83,  76,  32,  83, 104,  97, 
    100, 101, 114,  32,  67, 111, 
    109, 112, 105, 108, 101, 114, 
     32,  54,  46,  51,  46,  57, 
     54,  48,  48,  46,  49,  54, 
     51,  56,  52,   0,  73,  83, 
     71,  78, 176,   0,   0,   0, 
      5,   0,   0,   0,   8,   0, 
      0,   0, 128,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   3,   0,   0,   0, 
      0,   0,   0,   0,   7,   7, 
      0,   0, 137,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   3,   0,   0,   0, 
      1,   0,   0,   0,   3,   3, 
      0,   0, 146,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   3,   0,   0,   0, 
      2,   0,   0,   0,  15,  15, 
      0,   0, 153,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   1,   0,   0,   0, 
      3,   0,   0,   0,  15,   0, 
      0,   0, 165,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   3,   0,   0,   0, 
      4,   0,   0,   0,  15,   0, 
      0,   0,  80,  79,  83,  73, 
     84,  73,  79,  78,   0,  84, 
     69,  88,  67,  79,  79,  82, 
     68,   0,  78,  79,  82,  77, 
     65,  76,   0,  66,  79,  78, 
     69,  73,  78,  68,  73,  67, 
     69,  83,   0,  87,  69,  73, 
     71,  72,  84,  83,   0, 171, 
    171, 171,  79,  83,  71,  78, 
    140,   0,   0,   0,   4,   0, 
      0,   0,   8,   0,   0,   0, 
    104,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      3,   0,   0,   0,   0,   0, 
      0,   0,   7,   8,   0,   0, 
    111,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      3,   0,   0,   0,   1,   0, 
      0,   0,   7,   8,   0,   0, 
    119,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      3,   0,   0,   0,   2,   0, 
      0,   0,   3,  12,   0,   0, 
    128,   0,   0,   0,   0,   0, 
      0,   0,   1,   0,   0,   0, 
      3,   0,   0,   0,   3,   0, 
      0,   0,  15,   0,   0,   0, 
     78,  79,  82,  77,  65,  76, 
      0,  84,  65,  78,  71,  69, 
     78,  84,   0,  84,  69,  88, 
     67,  79,  79,  82,  68,   0, 
     83,  86,  95,  80,  79,  83, 
     73,  84,  73,  79,  78,   0, 
     83,  72,  69,  88,  44,   4, 
      0,   0,  80,   0,   1,   0, 
     11,   1,   0,   0, 106,   8, 
      0,   1,  89,   0,   0,   4, 
     70, 142,  32,   0,   0,   0, 
      0,   0,   4,   0,   0,   0, 
     89,   0,   0,   4,  70, 142, 
     32,   0,   1,   0,   0,   0, 
      7,   0,   0,   0,  95,   0, 
      0,   3, 114,  16,  16,   0, 
      0,   0,   0,   0,  95,   0, 
      0,   3,  50,  16,  16,   0, 
      1,   0,   0,   0,  95,   0, 
      0,   3, 242,  16,  16,   0, 
      2,   0,   0,   0, 101,   0, 
      0,   3, 114,  32,  16,   0, 
      0,   0,   0,   0, 101,   0, 
      0,   3, 114,  32,  16,   0, 
      1,   0,   0,   0, 101,   0, 
      0,   3,  50,  32,  16,   0, 
      2,   0,   0,   0, 103,   0, 
      0,   4, 242,  32,  16,   0, 
      3,   0,   0,   0,   1,   0, 
      0,   0, 104,   0,   0,   2, 
      2,   0,   0,   0,  15,   0, 
      0,   7,  18,   0,  16,   0, 
      0,   0,   0,   0,  70,  16, 
     16,   0,   2,   0,   0,   0, 
     70,  16,  16,   0,   2,   0, 
      0,   0,   0,   0,   0,   8, 
     18,   0,  16,   0,   0,   0, 
      0,   0,  10,   0,  16, 128, 
     65,   0,   0,   0,   0,   0, 
      0,   0,   1,  64,   0,   0, 
      0,   0, 128,  63,  75,   0, 
      0,   5,  18,   0,  16,   0, 
      0,   0,   0,   0,  10,   0, 
     16,   0,   0,   0,   0,   0, 
     56,   0,   0,  10,  98,   0, 
     16,   0,   0,   0,   0,   0, 
      6,  18,  16,   0,   2,   0, 
      0,   0,   2,  64,   0,   0, 
      0,   0,   0,   0,   0, 254, 
    255,  70,   0, 254, 255,  70, 
      0,   0,   0,   0,  27,   0, 
      0,   5,  98,   0,  16,   0, 
      0,   0,   0,   0,  86,   6, 
     16,   0,   0,   0,   0,   0, 
      1,   0,   0,  10,  98,   0, 
     16,   0,   0,   0,   0,   0, 
     86,   6,  16,   0,   0,   0, 
      0,   0,   2,  64,   0,   0, 
      0,   0,   0,   0,   1,   0, 
      0,   0,   1,   0,   0,   0, 
      0,   0,   0,   0,  55,   0, 
      0,  15,  98,   0,  16,   0, 
      0,   0,   0,   0,  86,   6, 
     16,   0,   0,   0,   0,   0, 
      2,  64,   0,   0,   0,   0, 
      0,   0,   0,   0, 128,  63, 
      0,   0, 128,  63,   0,   0, 
      0,   0,   2,  64,   0,   0, 
      0,   0,   0,   0,   0,   0, 
    128, 191,   0,   0, 128, 191, 
      0,   0,   0,   0,  56,   0, 
      0,   7,  66,   0,  16,   0, 
      1,   0,   0,   0,  26,   0, 
     16,   0,   0,   0,   0,   0, 
     10,   0,  16,   0,   0,   0, 
      0,   0,  54,   0,   0,   5, 
     50,   0,  16,   0,   1,   0, 
      0,   0,  70,  16,  16,   0, 
      2,   0,   0,   0,  16,   0, 
      0,   8,  18,  32,  16,   0, 
      0,   0,   0,   0,  70,   2, 
     16,   0,   1,   0,   0,   0, 
     70, 130,  32,   0,   1,   0, 
      0,   0,   4,   0,   0,   0, 
     16,   0,   0,   8,  34,  32, 
     16,   0,   0,   0,   0,   0, 
     70,   2,  16,   0,   1,   0, 
      0,   0,  70, 130,  32,   0, 
      1,   0,   0,   0,   5,   0, 
      0,   0,  16,   0,   0,   8, 
     66,  32,  16,   0,   0,   0, 
      0,   0,  70,   2,  16,   0, 
      1,   0,   0,   0,  70, 130, 
     32,   0,   1,   0,   0,   0, 
      6,   0,   0,   0,  15,   0, 
      0,   7,  18,   0,  16,   0, 
      0,   0,   0,   0, 230,  26, 
     16,   0,   2,   0,   0,   0, 
    230,  26,  16,   0,   2,   0, 
      0,   0,   0,   0,   0,   8, 
     18,   0,  16,   0,   0,   0, 
      0,   0,  10,   0,  16, 128, 
     65,   0,   0,   0,   0,   0, 
      0,   0,   1,  64,   0,   0, 
      0,   0, 128,  63,  75,   0, 
      0,   5,  18,   0,  16,   0, 
      0,   0,   0,   0,  10,   0, 
     16,   0,   0,   0,   0,   0, 
     56,   0,   0,   7,  66,   0, 
     16,   0,   0,   0,   0,   0, 
     42,   0,  16,   0,   0,   0, 
      0,   0,  10,   0,  16,   0, 
      0,   0,   0,   0,  54,   0, 
      0,   5,  50,   0,  16,   0, 
      0,   0,   0,   0, 230,  26, 
     16,   0,   2,   0,   0,   0, 
     16,   0,   0,   8,  18,  32, 
     16,   0,   1,   0,   0,   0, 
     70,   2,  16,   0,   0,   0, 
      0,   0,  70, 130,  32,   0, 
      1,   0,   0,   0,   0,   0, 
      0,   0,  16,   0,   0,   8, 
     34,  32,  16,   0,   1,   0, 
      0,   0,  70,   2,  16,   0, 
      0,   0,   0,   0,  70, 130, 
     32,   0,   1,   0,   0,   0, 
      1,   0,   0,   0,  16,   0, 
      0,   8,  66,  32,  16,   0, 
      1,   0,   0,   0,  70,   2, 
     16,   0,   0,   0,   0,   0, 
     70, 130,  32,   0,   1,   0, 
      0,   0,   2,   0,   0,   0, 
     54,   0,   0,   5,  50,  32, 
     16,   0,   2,   0,   0,   0, 
     70,  16,  16,   0,   1,   0, 
      0,   0,  54,   0,   0,   5, 
    114,   0,  16,   0,   0,   0, 
      0,   0,  70,  18,  16,   0, 
      0,   0,   0,   0,  54,   0, 
      0,   5, 130,   0,  16,   0, 
      0,   0,   0,   0,   1,  64, 
      0,   0,   0,   0, 128,  63, 
     17,   0,   0,   8,  18,   0, 
     16,   0,   1,   0,   0,   0, 
     70,  14,  16,   0,   0,   0, 
      0,   0,  70, 142,  32,   0, 
      1,   0,   0,   0,   0,   0, 
      0,   0,  17,   0,   0,   8, 
     34,   0,  16,   0,   1,   0, 
      0,   0,  70,  14,  16,   0, 
      0,   0,   0,   0,  70, 142, 
     32,   0,   1,   0,   0,   0, 
      1,   0,   0,   0,  17,   0, 
      0,   8,  66,   0,  16,   0, 
      1,   0,   0,   0,  70,  14, 
     16,   0,   0,   0,   0,   0, 
     70, 142,  32,   0,   1,   0, 
      0,   0,   2,   0,   0,   0, 
     17,   0,   0,   8, 130,   0, 
     16,   0,   1,   0,   0,   0, 
     70,  14,  16,   0,   0,   0, 
      0,   0,  70, 142,  32,   0, 
      1,   0,   0,   0,   3,   0, 
      0,   0,  17,   0,   0,   8, 
     18,  32,  16,   0,   3,   0, 
      0,   0,  70,  14,  16,   0, 
      1,   0,   0,   0,  70, 142, 
     32,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,  17,   0, 
      0,   8,  34,  32,  16,   0, 
      3,   0,   0,   0,  70,  14, 
     16,   0,   1,   0,   0,   0, 
     70, 142,  32,   0,   0,   0, 
      0,   0,   1,   0,   0,   0, 
     17,   0,   0,   8,  66,  32, 
     16,   0,   3,   0,   0,   0, 
     70,  14,  16,   0,   1,   0, 
      0,   0,  70, 142,  32,   0, 
      0,   0,   0,   0,   2,   0, 
      0,   0,  17,   0,   0,   8, 
    130,  32,  16,   0,   3,   0, 
      0,   0,  70,  14,  16,   0, 
      1,   0,   0,   0,  70, 142, 
     32,   0,   0,   0,   0,   0, 
      3,   0,   0,   0,  62,   0, 
      0,   1,  83,  84,  65,  84, 
    148,   0,   0,   0,  32,   0, 
      0,   0,   2,   0,   0,   0, 
      0,   0,   0,   0,   7,   0, 
      0,   0,  23,   0,   0,   0, 
      0,   0,   0,   0,   1,   0, 
      0,   0,   1,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   5,   0,   0,   0, 
      1,   0,   0,   0,   1,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0
};
