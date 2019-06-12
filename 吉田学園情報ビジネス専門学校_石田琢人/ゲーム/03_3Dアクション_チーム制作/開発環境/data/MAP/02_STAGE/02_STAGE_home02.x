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
 197;
 343.32703;1.21118;-228.88467;,
 -343.32703;1.21118;-228.88467;,
 -343.32703;230.09588;-228.88467;,
 343.32703;230.09588;-228.88467;,
 343.32703;230.09588;-228.88467;,
 -343.32703;230.09588;-228.88467;,
 -343.32703;230.09588;228.88467;,
 343.32703;230.09588;228.88467;,
 -343.32703;1.21118;228.88467;,
 343.32703;1.21118;228.88467;,
 -343.32703;1.21118;-228.88467;,
 -343.32703;1.21118;228.88467;,
 -343.32703;230.09588;228.88467;,
 343.32703;1.21118;228.88467;,
 343.32703;1.21118;-228.88467;,
 343.32703;230.09588;228.88467;,
 354.77127;1.21118;-240.32893;,
 331.88279;1.21118;-240.32893;,
 331.88279;230.09588;-240.32893;,
 354.77127;230.09588;-240.32893;,
 331.88279;230.09588;-217.44046;,
 354.77127;230.09588;-217.44046;,
 331.88279;1.21118;-217.44046;,
 354.77127;1.21118;-217.44046;,
 331.88279;1.21118;-217.44046;,
 331.88279;230.09588;-217.44046;,
 354.77127;1.21118;-217.44046;,
 354.77127;230.09588;-217.44046;,
 -331.88279;1.21118;-240.32893;,
 -354.77127;1.21118;-240.32893;,
 -354.77127;458.98057;-240.32893;,
 -331.88279;458.98057;-240.32893;,
 -354.77127;458.98057;-217.44046;,
 -331.88279;458.98057;-217.44046;,
 -354.77127;1.21118;-217.44046;,
 -331.88279;1.21118;-217.44046;,
 -354.77127;1.21118;-217.44046;,
 -354.77127;458.98057;-217.44046;,
 -331.88279;1.21118;-217.44046;,
 -331.88279;458.98057;-217.44046;,
 354.77127;1.21118;217.44046;,
 331.88279;1.21118;217.44046;,
 331.88279;230.09588;217.44046;,
 354.77127;230.09588;217.44046;,
 331.88279;230.09588;240.32893;,
 354.77127;230.09588;240.32893;,
 331.88279;1.21118;240.32893;,
 354.77127;1.21118;240.32893;,
 331.88279;1.21118;240.32893;,
 331.88279;230.09588;240.32893;,
 354.77127;1.21118;240.32893;,
 354.77127;230.09588;240.32893;,
 354.77127;230.09588;217.44046;,
 -354.77127;230.09588;217.44046;,
 -354.77127;252.98435;217.44046;,
 354.77127;252.98435;217.44046;,
 -354.77127;252.98435;240.32893;,
 354.77127;252.98435;240.32893;,
 -354.77127;230.09588;240.32893;,
 354.77127;230.09588;240.32893;,
 -354.77127;230.09588;217.44046;,
 354.77127;230.09588;217.44046;,
 -354.77127;230.09588;240.32893;,
 -354.77127;252.98435;240.32893;,
 354.77127;230.09588;240.32893;,
 354.77127;252.98435;240.32893;,
 -354.77127;230.09588;-216.29602;,
 -354.77127;230.09588;216.29602;,
 -354.77127;252.98435;216.29602;,
 -354.77127;252.98435;-216.29602;,
 -331.88279;252.98435;216.29602;,
 -331.88279;252.98435;-216.29602;,
 -331.88279;230.09588;216.29602;,
 -331.88279;230.09588;-216.29602;,
 -354.77127;230.09588;216.29602;,
 -354.77127;230.09588;-216.29602;,
 -331.88279;230.09588;216.29602;,
 -331.88279;252.98435;216.29602;,
 -331.88279;230.09588;-216.29602;,
 -331.88279;252.98435;-216.29602;,
 331.88279;230.09588;-216.29602;,
 331.88279;230.09588;216.29602;,
 331.88279;252.98435;216.29602;,
 331.88279;252.98435;-216.29602;,
 354.77127;252.98435;216.29602;,
 354.77127;252.98435;-216.29602;,
 354.77127;230.09588;216.29602;,
 354.77127;230.09588;-216.29602;,
 331.88279;230.09588;216.29602;,
 331.88279;230.09588;-216.29602;,
 354.77127;230.09588;216.29602;,
 354.77127;252.98435;216.29602;,
 354.77127;230.09588;-216.29602;,
 354.77127;252.98435;-216.29602;,
 354.77127;230.09588;-240.32893;,
 -354.77127;230.09588;-240.32893;,
 -354.77127;252.98435;-240.32893;,
 354.77127;252.98435;-240.32893;,
 -354.77127;252.98435;-217.44046;,
 354.77127;252.98435;-217.44046;,
 -354.77127;230.09588;-217.44046;,
 354.77127;230.09588;-217.44046;,
 -354.77127;230.09588;-240.32893;,
 354.77127;230.09588;-240.32893;,
 -354.77127;230.09588;-217.44046;,
 -354.77127;252.98435;-217.44046;,
 354.77127;230.09588;-217.44046;,
 354.77127;252.98435;-217.44046;,
 -331.88279;1.21118;217.44046;,
 -354.77127;1.21118;217.44046;,
 -354.77127;458.98057;217.44046;,
 -331.88279;458.98057;217.44046;,
 -354.77127;458.98057;240.32893;,
 -331.88279;458.98057;240.32893;,
 -354.77127;1.21118;240.32893;,
 -331.88279;1.21118;240.32893;,
 -354.77127;1.21118;240.32893;,
 -354.77127;458.98057;240.32893;,
 -331.88279;1.21118;240.32893;,
 -331.88279;458.98057;240.32893;,
 339.98209;230.09580;-228.88440;,
 -331.88279;230.09588;-228.88467;,
 -331.88279;458.98057;-228.88467;,
 339.98209;458.98077;-228.88440;,
 339.98209;458.98077;228.88440;,
 -331.88279;458.98057;228.88467;,
 -331.88279;230.09588;228.88467;,
 339.98209;230.09580;228.88440;,
 -331.88279;230.09588;-228.88467;,
 -331.88279;230.09588;228.88467;,
 -331.88279;458.98057;228.88467;,
 -331.88279;458.98057;-228.88467;,
 339.98209;230.09580;228.88440;,
 339.98209;230.09580;-228.88440;,
 339.98209;458.98077;-228.88440;,
 339.98209;458.98077;228.88440;,
 351.87303;252.98435;217.44046;,
 328.98453;252.98435;217.44046;,
 328.98453;458.98057;217.44046;,
 351.87303;458.98057;217.44046;,
 328.98453;458.98057;240.32893;,
 351.87303;458.98057;240.32893;,
 328.98453;252.98435;240.32893;,
 351.87303;252.98435;240.32893;,
 328.98453;252.98435;217.44046;,
 351.87303;252.98435;217.44046;,
 328.98453;252.98435;240.32893;,
 328.98453;458.98057;240.32893;,
 351.87303;252.98435;240.32893;,
 351.87303;458.98057;240.32893;,
 351.87303;252.98435;-240.32893;,
 328.98453;252.98435;-240.32893;,
 328.98453;458.98057;-240.32893;,
 351.87303;458.98057;-240.32893;,
 328.98453;458.98057;-217.44046;,
 351.87303;458.98057;-217.44046;,
 328.98453;252.98435;-217.44046;,
 351.87303;252.98435;-217.44046;,
 328.98453;252.98435;-240.32893;,
 351.87303;252.98435;-240.32893;,
 328.98453;252.98435;-217.44046;,
 328.98453;458.98057;-217.44046;,
 351.87303;252.98435;-217.44046;,
 351.87303;458.98057;-217.44046;,
 331.88279;436.09210;-240.32893;,
 -327.30518;436.09210;-240.32893;,
 -327.30518;458.98057;-240.32893;,
 331.88279;458.98057;-240.32893;,
 -327.30518;458.98057;-217.44046;,
 331.88279;458.98057;-217.44046;,
 331.88279;436.09210;-217.44046;,
 -327.30518;436.09210;-217.44046;,
 -327.30518;436.09210;-240.32893;,
 331.88279;436.09210;-240.32893;,
 331.88279;458.98057;217.44046;,
 -327.30518;458.98057;217.44046;,
 -327.30518;458.98057;240.32893;,
 331.88279;458.98057;240.32893;,
 -327.30518;436.09210;240.32893;,
 331.88279;436.09210;240.32893;,
 -327.30518;436.09210;217.44046;,
 331.88279;436.09210;217.44046;,
 389.15489;458.90836;-338.46497;,
 -389.15489;458.90836;-338.46497;,
 -389.15489;481.79683;-338.46497;,
 389.15489;481.79683;-338.46497;,
 -389.15489;481.79683;338.46497;,
 0.00000;682.11769;0.00000;,
 389.15489;481.79683;338.46497;,
 -389.15489;458.90836;338.46497;,
 389.15489;458.90836;338.46497;,
 -389.15489;458.90836;-338.46497;,
 389.15489;458.90836;-338.46497;,
 -389.15489;458.90836;338.46497;,
 -389.15489;481.79683;338.46497;,
 389.15489;458.90836;338.46497;,
 389.15489;481.79683;338.46497;;
 
 80;
 4;0,1,2,3;,
 4;4,5,6,7;,
 4;7,6,8,9;,
 4;10,11,12,5;,
 4;13,14,4,15;,
 4;16,17,18,19;,
 4;19,18,20,21;,
 4;21,20,22,23;,
 4;17,24,25,18;,
 4;26,16,19,27;,
 4;28,29,30,31;,
 4;31,30,32,33;,
 4;33,32,34,35;,
 4;29,36,37,30;,
 4;38,28,31,39;,
 4;40,41,42,43;,
 4;43,42,44,45;,
 4;45,44,46,47;,
 4;41,48,49,42;,
 4;50,40,43,51;,
 4;52,53,54,55;,
 4;55,54,56,57;,
 4;57,56,58,59;,
 4;59,58,60,61;,
 4;53,62,63,54;,
 4;64,52,55,65;,
 4;66,67,68,69;,
 4;69,68,70,71;,
 4;71,70,72,73;,
 4;73,72,74,75;,
 4;67,76,77,68;,
 4;78,66,69,79;,
 4;80,81,82,83;,
 4;83,82,84,85;,
 4;85,84,86,87;,
 4;87,86,88,89;,
 4;81,90,91,82;,
 4;92,80,83,93;,
 4;94,95,96,97;,
 4;97,96,98,99;,
 4;99,98,100,101;,
 4;101,100,102,103;,
 4;95,104,105,96;,
 4;106,94,97,107;,
 4;108,109,110,111;,
 4;111,110,112,113;,
 4;113,112,114,115;,
 4;109,116,117,110;,
 4;118,108,111,119;,
 4;120,121,122,123;,
 4;124,125,126,127;,
 4;128,129,130,131;,
 4;132,133,134,135;,
 4;136,137,138,139;,
 4;139,138,140,141;,
 4;141,140,142,143;,
 4;143,142,144,145;,
 4;137,146,147,138;,
 4;148,136,139,149;,
 4;150,151,152,153;,
 4;153,152,154,155;,
 4;155,154,156,157;,
 4;157,156,158,159;,
 4;151,160,161,152;,
 4;162,150,153,163;,
 4;164,165,166,167;,
 4;167,166,168,169;,
 4;170,171,172,173;,
 4;174,175,176,177;,
 4;177,176,178,179;,
 4;179,178,180,181;,
 4;182,183,184,185;,
 3;186,187,184;,
 4;188,186,189,190;,
 4;190,189,191,192;,
 4;183,193,194,184;,
 4;195,182,185,196;,
 3;185,184,187;,
 3;188,187,186;,
 3;185,187,188;;
 
 MeshMaterialList {
  1;
  80;
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
    "data\\MAP\\TEXTURE\\02_STAGE\\home02.jpg";
   }
  }
 }
 MeshNormals {
  13;
  0.000000;0.000000;-1.000000;,
  0.000000;1.000000;0.000000;,
  0.000000;0.000000;1.000000;,
  -1.000000;0.000000;0.000000;,
  1.000000;0.000000;0.000000;,
  0.000000;0.000000;-1.000000;,
  0.000000;0.000000;1.000000;,
  0.000000;-1.000000;0.000000;,
  0.000000;0.495313;-0.868715;,
  0.000000;0.495313;0.868715;,
  -0.243589;0.931226;0.271078;,
  -0.457681;0.889117;0.000000;,
  0.457681;0.889117;0.000000;;
  80;
  4;0,0,0,0;,
  4;1,1,1,1;,
  4;2,2,2,2;,
  4;3,3,3,3;,
  4;4,4,4,4;,
  4;0,0,0,0;,
  4;1,1,1,1;,
  4;2,2,2,2;,
  4;3,3,3,3;,
  4;4,4,4,4;,
  4;0,0,0,0;,
  4;1,1,1,1;,
  4;2,2,2,2;,
  4;3,3,3,3;,
  4;4,4,4,4;,
  4;0,0,0,0;,
  4;1,1,1,1;,
  4;2,2,2,2;,
  4;3,3,3,3;,
  4;4,4,4,4;,
  4;0,0,0,0;,
  4;1,1,1,1;,
  4;2,2,2,2;,
  4;7,7,7,7;,
  4;3,3,3,3;,
  4;4,4,4,4;,
  4;3,3,3,3;,
  4;1,1,1,1;,
  4;4,4,4,4;,
  4;7,7,7,7;,
  4;2,2,2,2;,
  4;0,0,0,0;,
  4;3,3,3,3;,
  4;1,1,1,1;,
  4;4,4,4,4;,
  4;7,7,7,7;,
  4;2,2,2,2;,
  4;0,0,0,0;,
  4;0,0,0,0;,
  4;1,1,1,1;,
  4;2,2,2,2;,
  4;7,7,7,7;,
  4;3,3,3,3;,
  4;4,4,4,4;,
  4;0,0,0,0;,
  4;1,1,1,1;,
  4;2,2,2,2;,
  4;3,3,3,3;,
  4;4,4,4,4;,
  4;5,5,5,5;,
  4;6,6,6,6;,
  4;3,3,3,3;,
  4;4,4,4,4;,
  4;0,0,0,0;,
  4;1,1,1,1;,
  4;2,2,2,2;,
  4;7,7,7,7;,
  4;3,3,3,3;,
  4;4,4,4,4;,
  4;0,0,0,0;,
  4;1,1,1,1;,
  4;2,2,2,2;,
  4;7,7,7,7;,
  4;3,3,3,3;,
  4;4,4,4,4;,
  4;0,0,0,0;,
  4;1,1,1,1;,
  4;7,7,7,7;,
  4;1,1,1,1;,
  4;2,2,2,2;,
  4;7,7,7,7;,
  4;0,0,8,8;,
  3;10,1,11;,
  4;9,2,2,2;,
  4;7,7,7,7;,
  4;3,3,3,3;,
  4;4,4,4,4;,
  3;8,8,1;,
  3;9,1,10;,
  3;12,1,12;;
 }
 MeshTextureCoords {
  197;
  0.343512;0.329701;,
  0.603618;0.329701;,
  0.603618;0.191729;,
  0.343512;0.191729;,
  0.187815;0.831446;,
  0.325788;0.831446;,
  0.325788;0.693474;,
  0.187815;0.693474;,
  0.325788;0.555501;,
  0.187815;0.555501;,
  0.325788;0.969419;,
  0.463760;0.969419;,
  0.463760;0.831446;,
  0.049843;0.969419;,
  0.187815;0.969419;,
  0.049843;0.831446;,
  0.177155;0.459075;,
  0.288866;0.459075;,
  0.288866;0.347364;,
  0.177155;0.347364;,
  0.288866;0.235654;,
  0.177155;0.235654;,
  0.288866;0.123944;,
  0.177155;0.123944;,
  0.400576;0.459075;,
  0.400576;0.347364;,
  0.065445;0.459075;,
  0.065445;0.347364;,
  0.177155;0.459075;,
  0.288866;0.459075;,
  0.288866;0.347364;,
  0.177155;0.347364;,
  0.288866;0.235654;,
  0.177155;0.235654;,
  0.288866;0.123944;,
  0.177155;0.123944;,
  0.400576;0.459075;,
  0.400576;0.347364;,
  0.065445;0.459075;,
  0.065445;0.347364;,
  0.177155;0.459075;,
  0.288866;0.459075;,
  0.288866;0.347364;,
  0.177155;0.347364;,
  0.288866;0.235654;,
  0.177155;0.235654;,
  0.288866;0.123944;,
  0.177155;0.123944;,
  0.400576;0.459075;,
  0.400576;0.347364;,
  0.065445;0.459075;,
  0.065445;0.347364;,
  0.177155;0.459075;,
  0.288866;0.459075;,
  0.288866;0.347364;,
  0.177155;0.347364;,
  0.288866;0.235654;,
  0.177155;0.235654;,
  0.288866;0.123944;,
  0.177155;0.123944;,
  0.288866;0.012233;,
  0.177155;0.012233;,
  0.400576;0.459075;,
  0.400576;0.347364;,
  0.065445;0.459075;,
  0.065445;0.347364;,
  0.177155;0.459075;,
  0.288866;0.459075;,
  0.288866;0.347364;,
  0.177155;0.347364;,
  0.288866;0.235654;,
  0.177155;0.235654;,
  0.288866;0.123944;,
  0.177155;0.123944;,
  0.288866;0.012233;,
  0.177155;0.012233;,
  0.400576;0.459075;,
  0.400576;0.347364;,
  0.065445;0.459075;,
  0.065445;0.347364;,
  0.177155;0.459075;,
  0.288866;0.459075;,
  0.288866;0.347364;,
  0.177155;0.347364;,
  0.288866;0.235654;,
  0.177155;0.235654;,
  0.288866;0.123944;,
  0.177155;0.123944;,
  0.288866;0.012233;,
  0.177155;0.012233;,
  0.400576;0.459075;,
  0.400576;0.347364;,
  0.065445;0.459075;,
  0.065445;0.347364;,
  0.177155;0.459075;,
  0.288866;0.459075;,
  0.288866;0.347364;,
  0.177155;0.347364;,
  0.288866;0.235654;,
  0.177155;0.235654;,
  0.288866;0.123944;,
  0.177155;0.123944;,
  0.288866;0.012233;,
  0.177155;0.012233;,
  0.400576;0.459075;,
  0.400576;0.347364;,
  0.065445;0.459075;,
  0.065445;0.347364;,
  0.177155;0.459075;,
  0.288866;0.459075;,
  0.288866;0.347364;,
  0.177155;0.347364;,
  0.288866;0.235654;,
  0.177155;0.235654;,
  0.288866;0.123944;,
  0.177155;0.123944;,
  0.400576;0.459075;,
  0.400576;0.347364;,
  0.065445;0.459075;,
  0.065445;0.347364;,
  0.315494;0.168450;,
  0.494177;0.168450;,
  0.494177;0.030478;,
  0.315494;0.030478;,
  0.187815;0.693474;,
  0.325788;0.693474;,
  0.325788;0.555501;,
  0.187815;0.555501;,
  0.325788;0.969419;,
  0.463760;0.969419;,
  0.463760;0.831446;,
  0.325788;0.831446;,
  0.049843;0.969419;,
  0.187815;0.969419;,
  0.187815;0.831446;,
  0.049843;0.831446;,
  0.177155;0.459075;,
  0.288866;0.459075;,
  0.288866;0.347364;,
  0.177155;0.347364;,
  0.288866;0.235654;,
  0.177155;0.235654;,
  0.288866;0.123944;,
  0.177155;0.123944;,
  0.288866;0.012233;,
  0.177155;0.012233;,
  0.400576;0.459075;,
  0.400576;0.347364;,
  0.065445;0.459075;,
  0.065445;0.347364;,
  0.177155;0.459075;,
  0.288866;0.459075;,
  0.288866;0.347364;,
  0.177155;0.347364;,
  0.288866;0.235654;,
  0.177155;0.235654;,
  0.288866;0.123944;,
  0.177155;0.123944;,
  0.288866;0.012233;,
  0.177155;0.012233;,
  0.400576;0.459075;,
  0.400576;0.347364;,
  0.065445;0.459075;,
  0.065445;0.347364;,
  0.177155;0.459075;,
  0.288866;0.459075;,
  0.288866;0.347364;,
  0.177155;0.347364;,
  0.288866;0.235654;,
  0.177155;0.235654;,
  0.177155;0.123944;,
  0.288866;0.123944;,
  0.288866;0.012233;,
  0.177155;0.012233;,
  0.177155;0.347364;,
  0.288866;0.347364;,
  0.288866;0.235654;,
  0.177155;0.235654;,
  0.288866;0.123944;,
  0.177155;0.123944;,
  0.288866;0.012233;,
  0.177155;0.012233;,
  0.704216;0.475996;,
  0.839892;0.475996;,
  0.839892;0.364284;,
  0.704216;0.364284;,
  0.839892;0.252573;,
  0.772054;0.308429;,
  0.704216;0.252573;,
  0.839892;0.140861;,
  0.704216;0.140861;,
  0.839892;0.029150;,
  0.704216;0.029150;,
  0.975568;0.475996;,
  0.975568;0.364284;,
  0.568541;0.475996;,
  0.568541;0.364284;;
 }
}
