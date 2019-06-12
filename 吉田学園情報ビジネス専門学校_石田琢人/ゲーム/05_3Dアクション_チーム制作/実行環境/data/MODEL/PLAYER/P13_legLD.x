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
 40;
 -0.02199;-7.47555;-3.36391;,
 0.30782;-11.77545;-3.99050;,
 -2.35018;-11.49775;-1.61446;,
 -2.54063;-7.79623;-1.44104;,
 0.65566;-16.81377;-4.56415;,
 -1.99282;-16.58684;-1.45501;,
 1.74798;-20.25695;-3.30198;,
 -2.07711;-20.35238;-0.76974;,
 3.13051;-6.62846;-1.10840;,
 3.46086;-10.76211;-1.06299;,
 3.72307;-15.49713;-1.48956;,
 4.28434;-20.56365;-0.70954;,
 4.28156;-6.42337;3.12596;,
 4.99511;-10.64961;3.92471;,
 5.82211;-15.04639;3.05996;,
 6.21917;-20.00502;3.76983;,
 0.52535;-6.00916;5.38680;,
 1.20095;-9.49586;6.08655;,
 1.63547;-16.37016;7.50415;,
 1.62015;-20.45915;7.16004;,
 -2.91221;-12.13198;2.94252;,
 -2.17010;-7.02441;2.73097;,
 -2.80310;-18.12199;4.64043;,
 -1.89951;-20.59364;5.71300;,
 2.88731;-15.34498;0.56499;,
 0.11581;-17.26820;-0.31823;,
 3.51466;-18.59041;3.08789;,
 0.48108;-18.27297;5.05426;,
 0.09313;-1.73245;-3.53651;,
 2.50221;0.09870;-1.41177;,
 -2.99975;-0.86218;-1.53766;,
 3.22456;-1.00572;1.90368;,
 -2.93239;-0.82914;1.74599;,
 0.62713;-0.14721;3.67459;,
 -0.41987;-0.01357;0.17654;,
 -0.41987;-0.01357;0.17654;,
 -0.41987;-0.01357;0.17654;,
 -0.41987;-0.01357;0.17654;,
 -0.41987;-0.01357;0.17654;,
 -0.41987;-0.01357;0.17654;;
 
 33;
 4;0,1,2,3;,
 4;1,4,5,2;,
 4;4,6,7,5;,
 4;8,9,1,0;,
 4;9,10,4,1;,
 4;10,11,6,4;,
 4;12,13,9,8;,
 4;13,14,10,9;,
 4;14,15,11,10;,
 4;16,17,13,12;,
 4;17,18,14,13;,
 4;18,19,15,14;,
 4;3,2,20,21;,
 4;2,5,22,20;,
 4;5,7,23,22;,
 4;21,20,17,16;,
 4;20,22,18,17;,
 4;22,23,19,18;,
 4;11,24,25,6;,
 4;15,26,24,11;,
 4;19,27,26,15;,
 4;28,29,8,0;,
 4;28,0,3,30;,
 4;29,31,12,8;,
 4;30,3,21,32;,
 4;31,33,16,12;,
 4;32,21,16,33;,
 3;34,29,28;,
 3;35,31,29;,
 3;36,33,31;,
 3;37,32,33;,
 3;38,30,32;,
 3;39,28,30;;
 
 MeshMaterialList {
  1;
  33;
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
   0.015360;0.037440;0.082560;1.000000;;
   8.000000;
   0.000000;0.000000;0.000000;;
   0.016000;0.039000;0.086000;;
  }
 }
 MeshNormals {
  35;
  0.832990;0.075875;-0.548061;,
  -0.899618;-0.009032;-0.436585;,
  -0.904227;0.047437;0.424410;,
  0.918068;0.212705;0.334525;,
  0.005236;0.030178;-0.999531;,
  -0.063922;0.221781;0.972999;,
  -0.897338;-0.002119;-0.441339;,
  -0.024833;0.074064;-0.996944;,
  -0.890105;-0.148562;-0.430863;,
  0.008814;-0.079978;-0.996758;,
  -0.873728;-0.250202;-0.417132;,
  -0.251741;-0.771773;-0.583946;,
  0.818802;0.120665;-0.561252;,
  0.822171;0.070113;-0.564906;,
  0.374910;-0.910614;-0.173853;,
  0.938727;0.187644;0.289106;,
  0.966935;0.103605;0.233031;,
  0.296346;-0.954626;-0.029466;,
  -0.925595;0.060172;0.373702;,
  -0.904905;-0.013181;0.425409;,
  -0.870784;-0.083830;0.484466;,
  -0.042865;0.187178;0.981390;,
  0.058842;0.107350;0.992479;,
  -0.143290;-0.526380;0.838088;,
  -0.357011;-0.541937;0.760820;,
  -0.630655;-0.605344;0.485626;,
  -0.639311;-0.739137;-0.212035;,
  -0.359575;-0.773330;-0.522174;,
  -0.081810;0.574035;-0.814734;,
  0.542163;0.718725;-0.435308;,
  -0.671005;0.614546;-0.414831;,
  0.626796;0.733890;0.261786;,
  -0.702731;0.640732;0.309244;,
  -0.004501;0.816815;0.576883;,
  -0.077685;0.992869;-0.090417;;
  33;
  4;4,7,6,1;,
  4;7,9,8,6;,
  4;9,11,10,8;,
  4;0,12,7,4;,
  4;12,13,9,7;,
  4;13,14,11,9;,
  4;3,15,12,0;,
  4;15,16,13,12;,
  4;16,17,14,13;,
  4;5,21,15,3;,
  4;21,22,16,15;,
  4;22,23,17,16;,
  4;1,6,18,2;,
  4;6,8,19,18;,
  4;8,10,20,19;,
  4;2,18,21,5;,
  4;18,19,22,21;,
  4;19,20,23,22;,
  4;14,25,24,11;,
  4;17,26,25,14;,
  4;23,27,26,17;,
  4;28,29,0,4;,
  4;28,4,1,30;,
  4;29,31,3,0;,
  4;30,1,2,32;,
  4;31,33,5,3;,
  4;32,2,5,33;,
  3;34,29,28;,
  3;34,31,29;,
  3;34,33,31;,
  3;34,32,33;,
  3;34,30,32;,
  3;34,28,30;;
 }
 MeshTextureCoords {
  40;
  0.581020;0.734460;,
  0.581020;0.734460;,
  0.565630;0.750990;,
  0.565630;0.750990;,
  0.581020;0.734460;,
  0.565630;0.750990;,
  0.581020;0.734460;,
  0.565630;0.750990;,
  0.601520;0.718010;,
  0.601520;0.718010;,
  0.601520;0.718010;,
  0.601520;0.718010;,
  0.603360;0.716880;,
  0.603360;0.716880;,
  0.603360;0.716880;,
  0.603360;0.716880;,
  0.583080;0.737030;,
  0.583080;0.737030;,
  0.583080;0.737030;,
  0.583080;0.737030;,
  0.565150;0.751790;,
  0.565150;0.751790;,
  0.565150;0.751790;,
  0.565150;0.751790;,
  0.601520;0.718010;,
  0.581020;0.734460;,
  0.603360;0.716880;,
  0.583080;0.737030;,
  0.571440;0.722170;,
  0.590280;0.709790;,
  0.551000;0.733540;,
  0.590670;0.709550;,
  0.550900;0.733710;,
  0.571880;0.722720;,
  0.580860;0.715980;,
  0.590480;0.709670;,
  0.581270;0.716140;,
  0.561390;0.728210;,
  0.550950;0.733620;,
  0.561220;0.727860;;
 }
}
