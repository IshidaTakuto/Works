xof 0302txt 0064
template Header {
 <3D82AB43-62DA-11cf-AB39-0020AF71E433>
 WORD major;
 WORD minor;
 DWORD flags;
}

template Vector {
 <3D82AB5E-62DA-11cf-AB39-0020AF71E433>
 FLOAT x;
 FLOAT y;
 FLOAT z;
}

template Coords2d {
 <F6F23F44-7686-11cf-8F52-0040333594A3>
 FLOAT u;
 FLOAT v;
}

template Matrix4x4 {
 <F6F23F45-7686-11cf-8F52-0040333594A3>
 array FLOAT matrix[16];
}

template ColorRGBA {
 <35FF44E0-6C7C-11cf-8F52-0040333594A3>
 FLOAT red;
 FLOAT green;
 FLOAT blue;
 FLOAT alpha;
}

template ColorRGB {
 <D3E16E81-7835-11cf-8F52-0040333594A3>
 FLOAT red;
 FLOAT green;
 FLOAT blue;
}

template IndexedColor {
 <1630B820-7842-11cf-8F52-0040333594A3>
 DWORD index;
 ColorRGBA indexColor;
}

template Boolean {
 <4885AE61-78E8-11cf-8F52-0040333594A3>
 WORD truefalse;
}

template Boolean2d {
 <4885AE63-78E8-11cf-8F52-0040333594A3>
 Boolean u;
 Boolean v;
}

template MaterialWrap {
 <4885AE60-78E8-11cf-8F52-0040333594A3>
 Boolean u;
 Boolean v;
}

template TextureFilename {
 <A42790E1-7810-11cf-8F52-0040333594A3>
 STRING filename;
}

template Material {
 <3D82AB4D-62DA-11cf-AB39-0020AF71E433>
 ColorRGBA faceColor;
 FLOAT power;
 ColorRGB specularColor;
 ColorRGB emissiveColor;
 [...]
}

template MeshFace {
 <3D82AB5F-62DA-11cf-AB39-0020AF71E433>
 DWORD nFaceVertexIndices;
 array DWORD faceVertexIndices[nFaceVertexIndices];
}

template MeshFaceWraps {
 <4885AE62-78E8-11cf-8F52-0040333594A3>
 DWORD nFaceWrapValues;
 Boolean2d faceWrapValues;
}

template MeshTextureCoords {
 <F6F23F40-7686-11cf-8F52-0040333594A3>
 DWORD nTextureCoords;
 array Coords2d textureCoords[nTextureCoords];
}

template MeshMaterialList {
 <F6F23F42-7686-11cf-8F52-0040333594A3>
 DWORD nMaterials;
 DWORD nFaceIndexes;
 array DWORD faceIndexes[nFaceIndexes];
 [Material]
}

template MeshNormals {
 <F6F23F43-7686-11cf-8F52-0040333594A3>
 DWORD nNormals;
 array Vector normals[nNormals];
 DWORD nFaceNormals;
 array MeshFace faceNormals[nFaceNormals];
}

template MeshVertexColors {
 <1630B821-7842-11cf-8F52-0040333594A3>
 DWORD nVertexColors;
 array IndexedColor vertexColors[nVertexColors];
}

template Mesh {
 <3D82AB44-62DA-11cf-AB39-0020AF71E433>
 DWORD nVertices;
 array Vector vertices[nVertices];
 DWORD nFaces;
 array MeshFace faces[nFaces];
 [...]
}

Header{
1;
0;
1;
}

Mesh {
 41;
 -1.96477;-0.71131;2.43350;,
 -2.35521;-1.66994;-0.45570;,
 -2.69516;-5.76350;2.95439;,
 -1.88312;-4.04503;5.34365;,
 2.47397;-0.42540;2.28835;,
 3.46951;-3.75609;5.17639;,
 2.71374;-5.15972;3.40909;,
 2.45180;-1.59077;0.23184;,
 -0.15129;-6.36160;1.02360;,
 0.31576;-2.93305;-1.22269;,
 0.44184;0.12871;9.86518;,
 0.33323;0.68589;4.59910;,
 -0.01036;-9.19902;4.15658;,
 2.55957;-8.07587;5.33884;,
 2.49758;-15.03586;9.50550;,
 1.67454;-15.05284;9.44372;,
 -0.02521;-14.95824;9.24208;,
 -2.38749;-8.74431;4.28521;,
 2.89622;-6.74243;6.85802;,
 2.29499;-14.32179;11.35901;,
 -0.87111;-14.53591;10.89409;,
 -2.30798;-7.09111;6.53894;,
 0.93470;-13.92999;12.41596;,
 0.93009;-5.43121;10.47358;,
 1.56224;-0.22316;3.02961;,
 2.15518;-2.06966;7.21182;,
 2.04271;-6.17322;8.42755;,
 1.70448;-14.15170;11.81784;,
 -0.00484;-0.02520;0.06460;,
 -0.00484;-0.02520;0.06460;,
 -0.00484;-0.02520;0.06460;,
 -0.00484;-0.02520;0.06460;,
 -0.00484;-0.02520;0.06460;,
 -0.00484;-0.02520;0.06460;,
 -0.00484;-0.02520;0.06460;,
 1.03352;-15.50587;11.65199;,
 1.03352;-15.50587;11.65199;,
 1.03352;-15.50587;11.65199;,
 1.03352;-15.50587;11.65199;,
 1.03352;-15.50587;11.65199;,
 1.03352;-15.50587;11.65199;;
 
 35;
 4;0,1,2,3;,
 4;4,5,6,7;,
 4;7,6,8,9;,
 4;9,8,2,1;,
 4;0,3,10,11;,
 4;12,8,6,13;,
 4;12,13,14,15;,
 4;12,15,16,17;,
 4;12,17,2,8;,
 4;13,6,5,18;,
 4;13,18,19,14;,
 4;17,16,20,21;,
 4;17,21,3,2;,
 4;21,20,22,23;,
 4;21,23,10,3;,
 4;24,11,10,25;,
 4;24,25,5,4;,
 4;25,10,23,26;,
 4;25,26,18,5;,
 4;26,23,22,27;,
 4;26,27,19,18;,
 3;28,7,9;,
 3;29,4,7;,
 3;30,24,4;,
 3;31,11,24;,
 3;32,9,1;,
 3;33,1,0;,
 3;34,0,11;,
 3;35,27,22;,
 3;36,19,27;,
 3;37,14,19;,
 3;38,15,14;,
 3;39,16,15;,
 3;40,20,16;,
 3;39,22,20;;
 
 MeshMaterialList {
  1;
  35;
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0;;
  Material {
   1.000000;1.000000;1.000000;1.000000;;
   0.000000;
   0.000000;0.000000;0.000000;;
   0.000000;0.000000;0.000000;;
  }
 }
 MeshNormals {
  30;
  0.857275;-0.314800;-0.407408;,
  -0.900208;-0.254718;-0.353191;,
  -0.442101;-0.841726;-0.309911;,
  0.983068;0.136199;0.122582;,
  -0.937685;0.209237;0.277428;,
  0.103310;-0.611564;-0.784422;,
  -0.020017;0.695799;0.717957;,
  0.752327;0.211773;-0.623824;,
  -0.740313;0.319044;-0.591733;,
  -0.769799;0.637180;0.037565;,
  0.715214;0.691717;-0.099982;,
  0.130201;-0.040077;-0.990677;,
  -0.027354;0.998487;0.047696;,
  0.120286;-0.615977;-0.778526;,
  0.819034;-0.363616;-0.443810;,
  -0.764061;-0.467053;-0.445053;,
  0.970782;0.022177;0.238936;,
  -0.945365;0.024045;0.325125;,
  0.034229;0.276065;0.960529;,
  0.846183;-0.231036;0.480206;,
  0.016779;-0.134697;0.990745;,
  -0.805274;-0.432667;0.405380;,
  0.663918;-0.715009;-0.219034;,
  0.070420;-0.846679;-0.527424;,
  0.567166;0.821072;0.064523;,
  0.855567;0.375568;0.356307;,
  0.835560;0.161964;0.524984;,
  0.682863;-0.071631;0.727026;,
  0.069452;0.882293;-0.465548;,
  0.145080;-0.888352;0.435641;;
  35;
  4;9,8,1,4;,
  4;10,3,0,7;,
  4;7,0,5,11;,
  4;11,5,1,8;,
  4;9,4,6,12;,
  4;13,5,0,14;,
  4;13,14,22,23;,
  4;13,23,2,15;,
  4;13,15,1,5;,
  4;14,0,3,16;,
  4;14,16,19,22;,
  4;15,2,21,17;,
  4;15,17,4,1;,
  4;17,21,20,18;,
  4;17,18,6,4;,
  4;24,12,6,25;,
  4;24,25,3,10;,
  4;25,6,18,26;,
  4;25,26,16,3;,
  4;26,18,20,27;,
  4;26,27,19,16;,
  3;28,7,11;,
  3;28,10,7;,
  3;28,24,10;,
  3;28,12,24;,
  3;28,11,8;,
  3;28,8,9;,
  3;28,9,12;,
  3;29,27,20;,
  3;29,19,27;,
  3;29,22,19;,
  3;29,23,22;,
  3;29,2,23;,
  3;29,21,2;,
  3;29,20,21;;
 }
 MeshTextureCoords {
  41;
  0.565150;0.751790;,
  0.565630;0.750990;,
  0.585160;0.775880;,
  0.585160;0.775880;,
  0.603360;0.716880;,
  0.615830;0.736070;,
  0.615830;0.736070;,
  0.601520;0.718010;,
  0.606090;0.772370;,
  0.581020;0.734460;,
  0.606090;0.772370;,
  0.583080;0.737030;,
  0.622510;0.793880;,
  0.633240;0.765420;,
  0.680100;0.844380;,
  0.666680;0.851770;,
  0.654460;0.858490;,
  0.603940;0.798260;,
  0.633240;0.765420;,
  0.680100;0.844380;,
  0.654460;0.858490;,
  0.603940;0.798260;,
  0.666680;0.851770;,
  0.622510;0.793880;,
  0.594560;0.725630;,
  0.611600;0.751830;,
  0.628580;0.777770;,
  0.674270;0.847590;,
  0.591270;0.726240;,
  0.602440;0.717450;,
  0.598960;0.721260;,
  0.588820;0.731330;,
  0.573330;0.742730;,
  0.565390;0.751390;,
  0.574120;0.744410;,
  0.670480;0.849680;,
  0.677180;0.845990;,
  0.680100;0.844380;,
  0.673390;0.848080;,
  0.660570;0.855130;,
  0.654460;0.858490;;
 }
}
