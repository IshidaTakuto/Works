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
 166;
 11.12462;86.70706;3.61462;,
 32.17965;102.20793;10.45580;,
 27.37366;102.20793;19.88814;,
 9.46316;86.70706;6.87540;,
 19.88814;102.20793;27.37366;,
 6.87540;86.70706;9.46316;,
 10.45580;102.20793;32.17965;,
 3.61462;86.70706;11.12462;,
 0.00000;102.20793;33.83567;,
 0.00000;86.70706;11.69711;,
 -10.45580;102.20793;32.17965;,
 -3.61462;86.70706;11.12462;,
 -19.88810;102.20793;27.37366;,
 -6.87540;86.70706;9.46316;,
 -27.37366;102.20793;19.88810;,
 -9.46316;86.70706;6.87540;,
 -32.17965;102.20793;10.45580;,
 -11.12462;86.70706;3.61462;,
 -33.83567;102.20793;0.00000;,
 -11.69711;86.70706;0.00000;,
 -32.17965;102.20793;-10.45580;,
 -11.12462;86.70706;-3.61462;,
 -27.37366;102.20793;-19.88810;,
 -9.46316;86.70706;-6.87540;,
 -19.88810;102.20793;-27.37366;,
 -6.87540;86.70706;-9.46316;,
 -10.45580;102.20793;-32.17965;,
 -3.61462;86.70706;-11.12462;,
 0.00000;102.20793;-33.83567;,
 0.00000;86.70706;-11.69711;,
 10.45580;102.20793;-32.17965;,
 3.61459;86.70706;-11.12462;,
 19.88810;102.20793;-27.37366;,
 6.87540;86.70706;-9.46316;,
 27.37366;102.20793;-19.88810;,
 9.46316;86.70706;-6.87540;,
 32.17965;102.20793;-10.45580;,
 11.12462;86.70706;-3.61462;,
 33.83567;102.20793;0.00000;,
 11.69711;86.70706;0.00000;,
 32.17965;102.20793;10.45580;,
 11.12462;86.70706;3.61462;,
 39.77624;138.82110;12.92408;,
 33.83572;138.82110;24.58308;,
 24.58308;138.82110;33.83567;,
 12.92408;138.82110;39.77624;,
 0.00000;138.82110;41.82320;,
 -12.92408;138.82110;39.77624;,
 -24.58305;138.82110;33.83567;,
 -33.83567;138.82110;24.58305;,
 -39.77624;138.82110;12.92408;,
 -41.82320;138.82110;0.00000;,
 -39.77624;138.82110;-12.92408;,
 -33.83567;138.82110;-24.58305;,
 -24.58305;138.82110;-33.83567;,
 -12.92408;138.82110;-39.77624;,
 0.00000;138.82110;-41.82320;,
 12.92408;138.82110;-39.77624;,
 24.58305;138.82110;-33.83567;,
 33.83567;138.82110;-24.58305;,
 39.77624;138.82110;-12.92408;,
 41.82320;138.82110;0.00000;,
 39.77624;138.82110;12.92408;,
 31.22029;165.70060;10.14408;,
 26.55755;165.70060;19.29515;,
 19.29515;165.70060;26.55755;,
 10.14408;165.70060;31.22029;,
 0.00000;165.70060;32.82689;,
 -10.14408;165.70060;31.22029;,
 -19.29515;165.70060;26.55755;,
 -26.55755;165.70060;19.29515;,
 -31.22029;165.70060;10.14408;,
 -32.82689;165.70060;0.00000;,
 -31.22029;165.70060;-10.14408;,
 -26.55755;165.70060;-19.29515;,
 -19.29515;165.70060;-26.55755;,
 -10.14408;165.70060;-31.22029;,
 0.00000;165.70060;-32.82689;,
 10.14408;165.70060;-31.22029;,
 19.29515;165.70060;-26.55751;,
 26.55751;165.70060;-19.29515;,
 31.22022;165.70060;-10.14408;,
 32.82689;165.70060;0.00000;,
 31.22029;165.70060;10.14408;,
 0.00000;84.85442;0.00000;,
 0.00000;84.85442;0.00000;,
 0.00000;84.85442;0.00000;,
 0.00000;84.85442;0.00000;,
 0.00000;84.85442;0.00000;,
 0.00000;84.85442;0.00000;,
 0.00000;84.85442;0.00000;,
 0.00000;84.85442;0.00000;,
 0.00000;84.85442;0.00000;,
 0.00000;84.85442;0.00000;,
 0.00000;84.85442;0.00000;,
 0.00000;84.85442;0.00000;,
 0.00000;84.85442;0.00000;,
 0.00000;84.85442;0.00000;,
 0.00000;84.85442;0.00000;,
 0.00000;84.85442;0.00000;,
 0.00000;84.85442;0.00000;,
 0.00000;84.85442;0.00000;,
 0.00000;84.85442;0.00000;,
 0.00000;84.85442;0.00000;,
 -20.69802;-0.82692;0.00000;,
 -19.68500;-0.82692;-6.39603;,
 -19.68500;-0.82692;6.39603;,
 -16.74505;-0.82692;12.16598;,
 -16.74505;-0.82692;-12.16598;,
 -12.16598;-0.82692;16.74505;,
 -12.16598;-0.82692;-16.74505;,
 -6.39603;-0.82692;19.68500;,
 -6.39603;-0.82692;-19.68500;,
 0.00000;-0.82692;20.69802;,
 0.00000;-0.82692;-20.69802;,
 6.39603;-0.82692;19.68500;,
 6.39603;-0.82692;-19.68500;,
 12.16598;-0.82692;16.74505;,
 12.16598;-0.82692;-16.74505;,
 16.74505;-0.82692;12.16598;,
 16.74505;-0.82692;-12.16598;,
 19.68500;-0.82692;6.39603;,
 19.68500;-0.82692;-6.39603;,
 20.69802;-0.82692;-0.00000;,
 19.68500;-0.82692;6.39603;,
 0.00000;165.84497;0.00000;,
 16.74505;-0.82692;12.16598;,
 12.16598;-0.82692;16.74505;,
 6.39603;-0.82692;19.68500;,
 0.00000;-0.82692;20.69802;,
 -6.39603;-0.82692;19.68500;,
 -12.16598;-0.82692;16.74505;,
 -16.74505;-0.82692;12.16598;,
 -19.68500;-0.82692;6.39603;,
 -20.69802;-0.82692;0.00000;,
 -19.68500;-0.82692;-6.39603;,
 -16.74505;-0.82692;-12.16598;,
 -12.16598;-0.82692;-16.74505;,
 -6.39603;-0.82692;-19.68500;,
 0.00000;-0.82692;-20.69802;,
 6.39603;-0.82692;-19.68500;,
 12.16598;-0.82692;-16.74505;,
 16.74505;-0.82692;-12.16598;,
 19.68500;-0.82692;-6.39603;,
 20.69802;-0.82692;-0.00000;,
 19.68500;-0.82692;6.39603;,
 0.00000;188.00223;-0.00000;,
 0.00000;188.00223;-0.00000;,
 0.00000;188.00223;-0.00000;,
 0.00000;188.00223;-0.00000;,
 0.00000;188.00223;-0.00000;,
 0.00000;188.00223;-0.00000;,
 0.00000;188.00223;-0.00000;,
 0.00000;188.00223;-0.00000;,
 0.00000;188.00223;-0.00000;,
 0.00000;188.00223;-0.00000;,
 0.00000;188.00223;-0.00000;,
 0.00000;188.00223;-0.00000;,
 0.00000;188.00223;-0.00000;,
 0.00000;188.00223;-0.00000;,
 0.00000;188.00223;-0.00000;,
 0.00000;188.00223;-0.00000;,
 0.00000;188.00223;-0.00000;,
 0.00000;188.00223;-0.00000;,
 0.00000;188.00223;-0.00000;,
 0.00000;188.00223;-0.00000;;
 
 138;
 4;0,1,2,3;,
 4;3,2,4,5;,
 4;5,4,6,7;,
 4;7,6,8,9;,
 4;9,8,10,11;,
 4;11,10,12,13;,
 4;13,12,14,15;,
 4;15,14,16,17;,
 4;17,16,18,19;,
 4;19,18,20,21;,
 4;21,20,22,23;,
 4;23,22,24,25;,
 4;25,24,26,27;,
 4;27,26,28,29;,
 4;29,28,30,31;,
 4;31,30,32,33;,
 4;33,32,34,35;,
 4;35,34,36,37;,
 4;37,36,38,39;,
 4;39,38,40,41;,
 4;1,42,43,2;,
 4;2,43,44,4;,
 4;4,44,45,6;,
 4;6,45,46,8;,
 4;8,46,47,10;,
 4;10,47,48,12;,
 4;12,48,49,14;,
 4;14,49,50,16;,
 4;16,50,51,18;,
 4;18,51,52,20;,
 4;20,52,53,22;,
 4;22,53,54,24;,
 4;24,54,55,26;,
 4;26,55,56,28;,
 4;28,56,57,30;,
 4;30,57,58,32;,
 4;32,58,59,34;,
 4;34,59,60,36;,
 4;36,60,61,38;,
 4;38,61,62,40;,
 4;42,63,64,43;,
 4;43,64,65,44;,
 4;44,65,66,45;,
 4;45,66,67,46;,
 4;46,67,68,47;,
 4;47,68,69,48;,
 4;48,69,70,49;,
 4;49,70,71,50;,
 4;50,71,72,51;,
 4;51,72,73,52;,
 4;52,73,74,53;,
 4;53,74,75,54;,
 4;54,75,76,55;,
 4;55,76,77,56;,
 4;56,77,78,57;,
 4;57,78,79,58;,
 4;58,79,80,59;,
 4;59,80,81,60;,
 4;60,81,82,61;,
 4;61,82,83,62;,
 3;3,84,0;,
 3;5,85,3;,
 3;7,86,5;,
 3;9,87,7;,
 3;11,88,9;,
 3;13,89,11;,
 3;15,90,13;,
 3;17,91,15;,
 3;19,92,17;,
 3;21,93,19;,
 3;23,94,21;,
 3;25,95,23;,
 3;27,96,25;,
 3;29,97,27;,
 3;31,98,29;,
 3;33,99,31;,
 3;35,100,33;,
 3;37,101,35;,
 3;39,102,37;,
 3;41,103,39;,
 3;104,105,106;,
 3;106,105,107;,
 3;105,108,107;,
 3;107,108,109;,
 3;108,110,109;,
 3;109,110,111;,
 3;110,112,111;,
 3;111,112,113;,
 3;112,114,113;,
 3;113,114,115;,
 3;114,116,115;,
 3;115,116,117;,
 3;116,118,117;,
 3;117,118,119;,
 3;118,120,119;,
 3;119,120,121;,
 3;120,122,121;,
 3;121,122,123;,
 3;124,125,126;,
 3;126,125,127;,
 3;127,125,128;,
 3;128,125,129;,
 3;129,125,130;,
 3;130,125,131;,
 3;131,125,132;,
 3;132,125,133;,
 3;133,125,134;,
 3;134,125,135;,
 3;135,125,136;,
 3;136,125,137;,
 3;137,125,138;,
 3;138,125,139;,
 3;139,125,140;,
 3;140,125,141;,
 3;141,125,142;,
 3;142,125,143;,
 3;143,125,144;,
 3;144,125,145;,
 3;146,64,63;,
 3;147,65,64;,
 3;148,66,65;,
 3;149,67,66;,
 3;150,68,67;,
 3;151,69,68;,
 3;152,70,69;,
 3;153,71,70;,
 3;154,72,71;,
 3;155,73,72;,
 3;156,74,73;,
 3;157,75,74;,
 3;158,76,75;,
 3;159,77,76;,
 3;160,78,77;,
 3;161,79,78;,
 3;162,80,79;,
 3;163,81,80;,
 3;164,82,81;,
 3;165,83,82;;
 
 MeshMaterialList {
  1;
  138;
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
   0.800000;0.800000;0.800000;1.000000;;
   5.000000;
   0.000000;0.000000;0.000000;;
   0.000000;0.000000;0.000000;;
   TextureFilename {
    "data\\TEXTURE\\snowtree.jpg";
   }
  }
 }
 MeshNormals {
  106;
  0.355875;-0.927352;0.115631;,
  0.302725;-0.927352;0.219943;,
  0.219943;-0.927352;0.302725;,
  0.115631;-0.927352;0.355875;,
  0.000000;-0.927352;0.374190;,
  -0.115631;-0.927352;0.355875;,
  -0.219943;-0.927352;0.302725;,
  -0.302725;-0.927352;0.219943;,
  -0.355875;-0.927352;0.115631;,
  -0.374190;-0.927352;0.000000;,
  -0.355875;-0.927352;-0.115631;,
  -0.302725;-0.927352;-0.219943;,
  -0.219943;-0.927352;-0.302725;,
  -0.115631;-0.927352;-0.355875;,
  0.000000;-0.927352;-0.374190;,
  0.115630;-0.927352;-0.355875;,
  0.219943;-0.927352;-0.302726;,
  0.302725;-0.927352;-0.219943;,
  0.355875;-0.927352;-0.115631;,
  0.374190;-0.927352;-0.000000;,
  0.790862;-0.555433;0.256965;,
  0.672747;-0.555433;0.488779;,
  0.488779;-0.555432;0.672748;,
  0.256966;-0.555432;0.790862;,
  0.000000;-0.555432;0.831562;,
  -0.256966;-0.555432;0.790862;,
  -0.488780;-0.555432;0.672747;,
  -0.672747;-0.555432;0.488780;,
  -0.790862;-0.555432;0.256966;,
  -0.831562;-0.555432;0.000000;,
  -0.790862;-0.555432;-0.256966;,
  -0.672747;-0.555432;-0.488780;,
  -0.488780;-0.555432;-0.672747;,
  -0.256966;-0.555432;-0.790862;,
  0.000000;-0.555432;-0.831562;,
  0.256966;-0.555432;-0.790862;,
  0.488780;-0.555432;-0.672748;,
  0.672747;-0.555432;-0.488780;,
  0.790862;-0.555432;-0.256966;,
  0.831562;-0.555432;-0.000000;,
  0.949661;0.054155;0.308562;,
  0.807830;0.054155;0.586923;,
  0.586922;0.054155;0.807830;,
  0.308563;0.054155;0.949661;,
  0.000000;0.054155;0.998533;,
  -0.308563;0.054155;0.949661;,
  -0.586923;0.054155;0.807829;,
  -0.807829;0.054155;0.586923;,
  -0.949661;0.054155;0.308563;,
  -0.998533;0.054155;0.000000;,
  -0.949661;0.054155;-0.308563;,
  -0.807829;0.054155;-0.586923;,
  -0.586923;0.054155;-0.807829;,
  -0.308563;0.054155;-0.949661;,
  0.000000;0.054155;-0.998533;,
  0.308563;0.054155;-0.949661;,
  0.586924;0.054155;-0.807829;,
  0.807830;0.054156;-0.586923;,
  0.949661;0.054156;-0.308564;,
  0.998533;0.054155;-0.000001;,
  0.757278;0.604969;0.246053;,
  0.644178;0.604969;0.468024;,
  0.468024;0.604969;0.644179;,
  0.246053;0.604969;0.757278;,
  0.000000;0.604969;0.796249;,
  -0.246054;0.604969;0.757278;,
  -0.468024;0.604969;0.644178;,
  -0.644178;0.604969;0.468024;,
  -0.757278;0.604969;0.246054;,
  -0.796249;0.604969;0.000000;,
  -0.757278;0.604969;-0.246054;,
  -0.644178;0.604969;-0.468024;,
  -0.468024;0.604969;-0.644179;,
  -0.246054;0.604969;-0.757278;,
  0.000000;0.604969;-0.796249;,
  0.246054;0.604969;-0.757277;,
  0.468025;0.604969;-0.644178;,
  0.644179;0.604969;-0.468023;,
  0.757277;0.604969;-0.246055;,
  0.796249;0.604969;-0.000002;,
  0.000000;1.000000;-0.000000;,
  -0.000000;-1.000000;-0.000000;,
  0.000000;-1.000000;-0.000000;,
  0.305935;0.140883;0.941571;,
  0.943807;0.123238;0.306661;,
  0.802850;0.123238;0.583305;,
  0.583305;0.123238;0.802850;,
  0.306661;0.123238;0.943807;,
  0.000000;0.123238;0.992377;,
  -0.306661;0.123238;0.943807;,
  -0.583305;0.123238;0.802850;,
  -0.990026;0.140883;-0.000000;,
  -0.802850;0.123238;0.583305;,
  -0.943807;0.123238;0.306661;,
  -0.992377;0.123238;0.000000;,
  -0.943807;0.123238;-0.306661;,
  -0.802850;0.123238;-0.583305;,
  -0.583305;0.123238;-0.802850;,
  0.305935;0.140883;-0.941571;,
  -0.306661;0.123238;-0.943807;,
  0.000000;0.123238;-0.992377;,
  0.306661;0.123238;-0.943807;,
  0.583305;0.123238;-0.802850;,
  0.802850;0.123238;-0.583305;,
  0.943807;0.123238;-0.306661;,
  0.992377;0.123238;0.000000;;
  138;
  4;0,20,21,1;,
  4;1,21,22,2;,
  4;2,22,23,3;,
  4;3,23,24,4;,
  4;4,24,25,5;,
  4;5,25,26,6;,
  4;6,26,27,7;,
  4;7,27,28,8;,
  4;8,28,29,9;,
  4;9,29,30,10;,
  4;10,30,31,11;,
  4;11,31,32,12;,
  4;12,32,33,13;,
  4;13,33,34,14;,
  4;14,34,35,15;,
  4;15,35,36,16;,
  4;16,36,37,17;,
  4;17,37,38,18;,
  4;18,38,39,19;,
  4;19,39,20,0;,
  4;20,40,41,21;,
  4;21,41,42,22;,
  4;22,42,43,23;,
  4;23,43,44,24;,
  4;24,44,45,25;,
  4;25,45,46,26;,
  4;26,46,47,27;,
  4;27,47,48,28;,
  4;28,48,49,29;,
  4;29,49,50,30;,
  4;30,50,51,31;,
  4;31,51,52,32;,
  4;32,52,53,33;,
  4;33,53,54,34;,
  4;34,54,55,35;,
  4;35,55,56,36;,
  4;36,56,57,37;,
  4;37,57,58,38;,
  4;38,58,59,39;,
  4;39,59,40,20;,
  4;40,60,61,41;,
  4;41,61,62,42;,
  4;42,62,63,43;,
  4;43,63,64,44;,
  4;44,64,65,45;,
  4;45,65,66,46;,
  4;46,66,67,47;,
  4;47,67,68,48;,
  4;48,68,69,49;,
  4;49,69,70,50;,
  4;50,70,71,51;,
  4;51,71,72,52;,
  4;52,72,73,53;,
  4;53,73,74,54;,
  4;54,74,75,55;,
  4;55,75,76,56;,
  4;56,76,77,57;,
  4;57,77,78,58;,
  4;58,78,79,59;,
  4;59,79,60,40;,
  3;1,81,0;,
  3;2,81,1;,
  3;3,81,2;,
  3;4,81,3;,
  3;5,81,4;,
  3;6,81,5;,
  3;7,81,6;,
  3;8,81,7;,
  3;9,81,8;,
  3;10,81,9;,
  3;11,81,10;,
  3;12,81,11;,
  3;13,81,12;,
  3;14,81,13;,
  3;15,81,14;,
  3;16,81,15;,
  3;17,81,16;,
  3;18,81,17;,
  3;19,81,18;,
  3;0,81,19;,
  3;82,82,82;,
  3;82,82,82;,
  3;82,82,82;,
  3;82,82,82;,
  3;82,82,82;,
  3;82,82,82;,
  3;82,82,82;,
  3;82,82,82;,
  3;82,82,82;,
  3;82,82,82;,
  3;82,82,82;,
  3;82,82,82;,
  3;82,82,82;,
  3;82,82,82;,
  3;82,82,82;,
  3;82,82,82;,
  3;82,82,82;,
  3;82,82,82;,
  3;84,83,85;,
  3;85,83,86;,
  3;86,83,87;,
  3;87,83,88;,
  3;88,83,89;,
  3;89,83,90;,
  3;90,91,92;,
  3;92,91,93;,
  3;93,91,94;,
  3;94,91,95;,
  3;95,91,96;,
  3;96,91,97;,
  3;97,98,99;,
  3;99,98,100;,
  3;100,98,101;,
  3;101,98,102;,
  3;102,98,103;,
  3;103,98,104;,
  3;104,105,105;,
  3;105,105,84;,
  3;80,61,60;,
  3;80,62,61;,
  3;80,63,62;,
  3;80,64,63;,
  3;80,65,64;,
  3;80,66,65;,
  3;80,67,66;,
  3;80,68,67;,
  3;80,69,68;,
  3;80,70,69;,
  3;80,71,70;,
  3;80,72,71;,
  3;80,73,72;,
  3;80,74,73;,
  3;80,75,74;,
  3;80,76,75;,
  3;80,77,76;,
  3;80,78,77;,
  3;80,79,78;,
  3;80,60,79;;
 }
 MeshTextureCoords {
  166;
  0.395341;0.860604;,
  0.395341;0.673531;,
  0.423953;0.673531;,
  0.423953;0.860604;,
  0.452566;0.673531;,
  0.452566;0.860604;,
  0.481179;0.673531;,
  0.481179;0.860604;,
  0.509791;0.673531;,
  0.509791;0.860604;,
  0.538404;0.673531;,
  0.538404;0.860604;,
  0.567017;0.673531;,
  0.567017;0.860604;,
  0.595630;0.673531;,
  0.595630;0.860604;,
  0.624242;0.673531;,
  0.624242;0.860604;,
  0.652855;0.673531;,
  0.652855;0.860604;,
  0.681468;0.673531;,
  0.681468;0.860604;,
  0.710080;0.673531;,
  0.710080;0.860604;,
  0.738693;0.673531;,
  0.738693;0.860604;,
  0.767306;0.673531;,
  0.767306;0.860604;,
  0.795919;0.673531;,
  0.795919;0.860604;,
  0.824531;0.673531;,
  0.824531;0.860604;,
  0.853144;0.673531;,
  0.853144;0.860604;,
  0.881757;0.673531;,
  0.881757;0.860604;,
  0.910369;0.673531;,
  0.910369;0.860604;,
  0.938982;0.673531;,
  0.938982;0.860604;,
  0.967595;0.673531;,
  0.967595;0.860604;,
  0.395341;0.486458;,
  0.423953;0.486458;,
  0.452566;0.486458;,
  0.481179;0.486458;,
  0.509791;0.486458;,
  0.538404;0.486458;,
  0.567017;0.486458;,
  0.595630;0.486458;,
  0.624242;0.486458;,
  0.652855;0.486458;,
  0.681468;0.486458;,
  0.710080;0.486458;,
  0.738693;0.486458;,
  0.767306;0.486458;,
  0.795919;0.486458;,
  0.824531;0.486458;,
  0.853144;0.486458;,
  0.881757;0.486458;,
  0.910369;0.486458;,
  0.938982;0.486458;,
  0.967595;0.486458;,
  0.395341;0.299384;,
  0.423953;0.299384;,
  0.452566;0.299384;,
  0.481179;0.299384;,
  0.509791;0.299384;,
  0.538404;0.299384;,
  0.567017;0.299384;,
  0.595630;0.299384;,
  0.624242;0.299384;,
  0.652855;0.299384;,
  0.681468;0.299384;,
  0.710080;0.299384;,
  0.738693;0.299384;,
  0.767306;0.299384;,
  0.795919;0.299384;,
  0.824531;0.299384;,
  0.853144;0.299384;,
  0.881757;0.299384;,
  0.910369;0.299384;,
  0.938982;0.299384;,
  0.967595;0.299384;,
  0.409647;0.954141;,
  0.438260;0.954141;,
  0.466872;0.954141;,
  0.495485;0.954141;,
  0.524098;0.954141;,
  0.552710;0.954141;,
  0.581323;0.954141;,
  0.609936;0.954141;,
  0.638549;0.954141;,
  0.667161;0.954141;,
  0.695774;0.954141;,
  0.724387;0.954141;,
  0.752999;0.954141;,
  0.781612;0.954141;,
  0.810225;0.954141;,
  0.838838;0.954141;,
  0.867450;0.954141;,
  0.896063;0.954141;,
  0.924676;0.954141;,
  0.953288;0.954141;,
  0.033951;0.744802;,
  0.041384;0.669154;,
  0.041384;0.820450;,
  0.062955;0.888694;,
  0.062955;0.600911;,
  0.096552;0.942852;,
  0.096552;0.546753;,
  0.138888;0.977623;,
  0.138888;0.511981;,
  0.185817;0.989605;,
  0.185817;0.500000;,
  0.232747;0.977623;,
  0.232747;0.511981;,
  0.275082;0.942852;,
  0.275082;0.546753;,
  0.308680;0.888694;,
  0.308680;0.600911;,
  0.330251;0.820450;,
  0.330251;0.669154;,
  0.337684;0.744802;,
  0.033951;0.500000;,
  0.185817;0.010395;,
  0.049137;0.500000;,
  0.064324;0.500000;,
  0.079511;0.500000;,
  0.094697;0.500000;,
  0.109884;0.500000;,
  0.125071;0.500000;,
  0.140257;0.500000;,
  0.155444;0.500000;,
  0.170631;0.500000;,
  0.185817;0.500000;,
  0.201004;0.500000;,
  0.216191;0.500000;,
  0.231377;0.500000;,
  0.246564;0.500000;,
  0.261751;0.500000;,
  0.276937;0.500000;,
  0.292124;0.500000;,
  0.307310;0.500000;,
  0.322497;0.500000;,
  0.337684;0.500000;,
  0.409647;0.112311;,
  0.438259;0.112311;,
  0.466873;0.112311;,
  0.495485;0.112311;,
  0.524098;0.112311;,
  0.552711;0.112311;,
  0.581324;0.112311;,
  0.609936;0.112311;,
  0.638548;0.112311;,
  0.667161;0.112311;,
  0.695774;0.112311;,
  0.724387;0.112311;,
  0.753000;0.112311;,
  0.781613;0.112311;,
  0.810225;0.112311;,
  0.838838;0.112311;,
  0.867450;0.112311;,
  0.896063;0.112311;,
  0.924675;0.112311;,
  0.953288;0.112311;;
 }
}
