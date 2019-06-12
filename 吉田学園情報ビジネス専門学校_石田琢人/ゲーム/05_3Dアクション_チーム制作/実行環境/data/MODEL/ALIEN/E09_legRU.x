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
 82;
 -2.63332;-1.09240;-2.77397;,
 -3.85655;-8.94108;-5.17928;,
 -4.20507;-8.13878;-2.58836;,
 -3.59408;-1.28090;-0.67407;,
 -1.12706;-9.88688;-7.07089;,
 0.14295;-0.05554;-4.34937;,
 -2.05661;-19.52738;-1.53209;,
 0.27182;-19.56588;-3.25747;,
 0.24944;-18.28052;-4.10994;,
 -1.62700;-17.45334;-1.57339;,
 -3.56083;-12.56535;-6.02652;,
 -1.34140;-12.54232;-7.77089;,
 2.33408;-9.54625;-6.29638;,
 1.81260;-12.76741;-5.39915;,
 -3.70296;-12.32573;-4.19003;,
 1.16455;-9.20375;-2.42270;,
 0.58707;-12.75810;-2.48574;,
 -1.32902;-8.85963;1.67853;,
 -1.28004;-12.48784;0.31823;,
 -3.78423;-19.76155;-3.70890;,
 -3.73714;-18.47305;-4.77931;,
 -3.73296;-20.23621;-5.84085;,
 -1.77027;-22.00504;-8.37137;,
 -1.96583;-22.35971;-7.07089;,
 0.53395;-20.53789;-6.12960;,
 -3.89320;-17.36706;-7.33293;,
 -3.90693;-16.63687;-5.49784;,
 -3.70296;-12.32573;-4.19003;,
 -3.56083;-12.56535;-6.02652;,
 0.20705;-16.20936;-4.47161;,
 0.81309;-18.51946;-7.10883;,
 1.81260;-12.76741;-5.39915;,
 0.58707;-12.75810;-2.48574;,
 -1.58900;-15.91640;-1.27782;,
 -1.28004;-12.48784;0.31823;,
 -1.61440;-17.09866;-8.90339;,
 -1.60228;-19.69963;-9.55363;,
 -1.34140;-12.54232;-7.77089;,
 0.65698;-22.39710;-5.18075;,
 0.53395;-20.53789;-6.12960;,
 0.27182;-19.56588;-3.25747;,
 0.26654;-21.43846;-2.29155;,
 -3.78423;-19.76155;-3.70890;,
 -3.73296;-20.23621;-5.84085;,
 -4.15003;-22.31792;-4.49321;,
 -4.17220;-21.15255;-2.43670;,
 -1.96583;-22.35971;-7.07089;,
 -2.01399;-23.66020;-5.94774;,
 -2.05661;-19.52738;-1.53209;,
 -2.03146;-20.04126;-0.12595;,
 -1.70420;-20.82278;-8.60783;,
 3.34159;-0.53567;-1.49750;,
 1.89247;-1.48935;3.57108;,
 -1.14005;-1.70854;4.49340;,
 -3.65373;-1.44168;2.02725;,
 -2.96271;-8.55856;-0.56273;,
 -1.32902;-8.85963;1.67853;,
 -1.14005;-1.70854;4.49340;,
 -2.64747;-12.35510;-1.62875;,
 -2.96271;-8.55856;-0.56273;,
 -2.89314;-16.21747;-3.11766;,
 -2.64747;-12.35510;-1.62875;,
 -2.80954;-17.84914;-3.11542;,
 -3.03426;-19.65989;-2.76394;,
 -3.26047;-20.95031;-1.69544;,
 -3.03426;-19.65989;-2.76394;,
 -3.65373;-1.44168;2.02725;,
 -3.59408;-1.28090;-0.67407;,
 -2.49852;-23.31415;-2.57155;,
 -2.49852;-23.31415;-2.57155;,
 -2.49852;-23.31415;-2.57155;,
 -2.49852;-23.31415;-2.57155;,
 -2.49852;-23.31415;-2.57155;,
 -2.49852;-23.31415;-2.57155;,
 -2.49852;-23.31415;-2.57155;,
 0.01736;-0.02407;0.01552;,
 0.01736;-0.02407;0.01552;,
 0.01736;-0.02407;0.01552;,
 0.01736;-0.02407;0.01552;,
 0.01736;-0.02407;0.01552;,
 0.01736;-0.02407;0.01552;,
 0.01736;-0.02407;0.01552;;
 
 60;
 4;0,1,2,3;,
 4;4,1,0,5;,
 4;6,7,8,9;,
 4;10,1,4,11;,
 4;4,12,13,11;,
 4;14,2,1,10;,
 4;12,15,16,13;,
 4;15,17,18,16;,
 3;19,20,21;,
 3;22,23,21;,
 3;8,7,24;,
 3;23,22,24;,
 4;25,21,20,26;,
 4;27,28,25,26;,
 4;29,8,24,30;,
 4;31,32,29,30;,
 4;33,9,8,29;,
 4;32,34,33,29;,
 4;35,36,21,25;,
 4;28,37,35,25;,
 4;30,24,36,35;,
 4;37,31,30,35;,
 4;38,39,40,41;,
 4;42,43,44,45;,
 4;43,46,47,44;,
 4;46,39,38,47;,
 4;40,48,49,41;,
 3;22,21,50;,
 3;21,36,50;,
 3;36,24,50;,
 3;24,22,50;,
 4;51,52,15,12;,
 4;4,5,51,12;,
 4;52,53,17,15;,
 4;54,55,56,57;,
 4;58,18,17,59;,
 4;2,14,58,59;,
 4;60,33,34,61;,
 4;27,26,60,61;,
 4;62,9,33,60;,
 4;26,20,62,60;,
 4;63,6,9,62;,
 4;20,19,63,62;,
 4;64,49,48,65;,
 4;42,45,64,65;,
 4;59,66,67,2;,
 3;41,49,68;,
 3;49,64,69;,
 3;64,45,70;,
 3;45,44,71;,
 3;44,47,72;,
 3;47,38,73;,
 3;38,41,74;,
 3;75,51,5;,
 3;76,52,51;,
 3;77,53,52;,
 3;78,54,57;,
 3;79,67,66;,
 3;80,0,3;,
 3;81,5,0;;
 
 MeshMaterialList {
  1;
  60;
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
  51;
  -0.624345;0.701083;-0.344495;,
  -0.785950;0.618056;-0.017019;,
  -0.093111;0.828029;-0.552899;,
  -0.150529;0.502391;0.851436;,
  -0.821919;0.174188;-0.542318;,
  0.920791;-0.057164;-0.385845;,
  -0.916591;-0.166767;-0.363386;,
  0.899391;-0.273561;-0.340969;,
  -0.995913;-0.046927;0.077168;,
  0.839129;-0.249771;0.483195;,
  -0.968759;0.066739;0.238855;,
  0.923811;0.048572;0.379756;,
  0.904667;0.223666;0.362700;,
  -0.929178;0.281954;0.239019;,
  -0.058483;0.207782;-0.976425;,
  -0.119034;-0.686295;-0.717516;,
  -0.066947;-0.402514;0.912962;,
  -0.145142;-0.774333;-0.615908;,
  -0.004667;0.103389;0.994630;,
  -0.058368;0.562882;0.824474;,
  -0.028691;0.154082;-0.987641;,
  -0.869471;0.123504;-0.478295;,
  0.945163;-0.113372;-0.306289;,
  -0.981879;-0.079195;0.172167;,
  0.860468;-0.231236;0.454010;,
  0.003207;-0.385180;0.922836;,
  -0.045206;-0.391335;-0.919137;,
  -0.910480;0.055166;-0.409856;,
  0.962932;-0.075303;-0.259019;,
  -0.978998;-0.009763;0.203638;,
  0.892547;-0.163146;0.420409;,
  0.014754;-0.257671;0.966120;,
  0.012994;0.075789;-0.997039;,
  -0.870535;-0.481568;-0.101299;,
  0.750776;-0.658902;-0.046736;,
  0.803414;-0.242423;0.543836;,
  -0.881861;-0.084986;0.463788;,
  -0.119668;-0.906999;-0.403773;,
  -0.139941;0.047054;0.989041;,
  -0.076800;-0.437437;-0.895964;,
  0.670415;0.730383;-0.130710;,
  0.667034;0.443324;0.598773;,
  -0.860858;-0.200525;0.467668;,
  -0.854302;-0.179041;0.487968;,
  -0.861298;-0.076959;0.502237;,
  -0.841041;0.108430;0.529993;,
  -0.760644;0.417393;0.497197;,
  -0.719874;0.084589;0.688931;,
  -0.753903;0.514919;0.408030;,
  -0.197970;-0.822684;0.532916;,
  -0.112960;0.984914;0.131088;;
  60;
  4;0,4,8,1;,
  4;14,4,0,2;,
  4;19,12,11,18;,
  4;21,4,14,20;,
  4;14,5,22,20;,
  4;23,8,4,21;,
  4;5,9,24,22;,
  4;9,16,25,24;,
  3;13,10,6;,
  3;15,17,6;,
  3;11,12,7;,
  3;17,15,7;,
  4;27,6,10,29;,
  4;23,21,27,29;,
  4;30,11,7,28;,
  4;22,24,30,28;,
  4;31,18,11,30;,
  4;24,25,31,30;,
  4;32,26,6,27;,
  4;21,20,32,27;,
  4;28,7,26,32;,
  4;20,22,28,32;,
  4;34,7,12,35;,
  4;13,6,33,36;,
  4;6,17,37,33;,
  4;17,7,34,37;,
  4;12,19,38,35;,
  3;15,6,39;,
  3;6,26,39;,
  3;26,7,39;,
  3;7,15,39;,
  4;40,41,9,5;,
  4;14,2,40,5;,
  4;41,3,16,9;,
  4;48,42,16,3;,
  4;43,25,16,42;,
  4;8,23,43,42;,
  4;44,31,25,43;,
  4;23,29,44,43;,
  4;45,18,31,44;,
  4;29,10,45,44;,
  4;46,19,18,45;,
  4;10,13,46,45;,
  4;47,38,19,46;,
  4;13,36,47,46;,
  4;42,48,1,8;,
  3;35,38,49;,
  3;38,47,49;,
  3;47,36,49;,
  3;36,33,49;,
  3;33,37,49;,
  3;37,34,49;,
  3;34,35,49;,
  3;50,40,2;,
  3;50,41,40;,
  3;50,3,41;,
  3;50,48,3;,
  3;50,1,48;,
  3;50,0,1;,
  3;50,2,0;;
 }
 MeshTextureCoords {
  82;
  0.122620;0.563390;,
  0.145700;0.632620;,
  0.145700;0.632620;,
  0.122620;0.563390;,
  0.108120;0.643360;,
  0.095100;0.570770;,
  0.556580;0.709110;,
  0.534000;0.720190;,
  0.534000;0.720190;,
  0.556580;0.709110;,
  0.150300;0.688380;,
  0.118450;0.699760;,
  0.073900;0.653140;,
  0.089440;0.710120;,
  0.150300;0.688380;,
  0.073900;0.653140;,
  0.089440;0.710120;,
  0.108120;0.643360;,
  0.118450;0.699760;,
  0.581380;0.696950;,
  0.581380;0.696950;,
  0.581380;0.696950;,
  0.556580;0.709110;,
  0.556580;0.709110;,
  0.534000;0.720190;,
  0.579340;0.672200;,
  0.579960;0.679780;,
  0.575730;0.628340;,
  0.575730;0.628340;,
  0.529580;0.703990;,
  0.527560;0.696600;,
  0.514870;0.650080;,
  0.514870;0.650080;,
  0.554150;0.695860;,
  0.543880;0.639720;,
  0.553280;0.691080;,
  0.554420;0.697290;,
  0.543880;0.639720;,
  0.565630;0.750990;,
  0.547080;0.728860;,
  0.547080;0.728860;,
  0.565150;0.751790;,
  0.587270;0.707590;,
  0.587270;0.707590;,
  0.601520;0.718010;,
  0.603360;0.716880;,
  0.568870;0.718880;,
  0.581020;0.734460;,
  0.568870;0.718880;,
  0.583080;0.737030;,
  0.555370;0.702510;,
  0.070760;0.591480;,
  0.070700;0.590320;,
  0.095100;0.570770;,
  0.000000;0.000000;,
  0.000000;0.000000;,
  0.000000;0.000000;,
  0.000000;0.000000;,
  0.136470;0.693320;,
  0.129390;0.637280;,
  0.568760;0.686760;,
  0.561900;0.633280;,
  0.570610;0.702230;,
  0.570610;0.702230;,
  0.594560;0.725630;,
  0.579280;0.712490;,
  0.129390;0.637280;,
  0.145700;0.632620;,
  0.574120;0.744410;,
  0.588820;0.731330;,
  0.598960;0.721260;,
  0.602440;0.717450;,
  0.591270;0.726240;,
  0.573330;0.742730;,
  0.565390;0.751390;,
  0.082930;0.581130;,
  0.070730;0.590900;,
  0.082900;0.580550;,
  0.000000;0.000000;,
  0.137540;0.634950;,
  0.122620;0.563390;,
  0.108860;0.567080;;
 }
}
