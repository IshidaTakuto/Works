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
 26;
 2799.99965;0.00000;-2799.99965;,
 -2800.00063;0.00000;-2799.99965;,
 -2800.00063;0.00000;2800.00063;,
 2799.99965;0.00000;2800.00063;,
 -2744.56547;0.00000;2750.87406;,
 -2744.56547;839.34435;2750.87406;,
 -1848.41565;839.34435;2616.23656;,
 -1848.41565;0.00000;2616.23656;,
 -1039.98752;839.34435;2225.50374;,
 -1039.98752;0.00000;2225.50374;,
 -398.41539;839.34435;1616.92297;,
 -398.41539;0.00000;1616.92297;,
 13.49930;839.34435;850.06661;,
 13.49930;0.00000;850.06661;,
 155.43511;839.34435;0.00000;,
 155.43511;0.00000;0.00000;,
 13.49907;839.34435;-850.06661;,
 13.49907;0.00000;-850.06661;,
 -398.41539;839.34435;-1616.92278;,
 -398.41539;0.00000;-1616.92278;,
 -1039.98800;839.34435;-2225.50335;,
 -1039.98800;0.00000;-2225.50335;,
 -1848.41604;839.34435;-2616.23598;,
 -1848.41604;0.00000;-2616.23598;,
 -2744.56547;839.34435;-2750.87309;,
 -2744.56547;0.00000;-2750.87309;;
 
 11;
 4;0,1,2,3;,
 4;4,5,6,7;,
 4;7,6,8,9;,
 4;9,8,10,11;,
 4;11,10,12,13;,
 4;13,12,14,15;,
 4;15,14,16,17;,
 4;17,16,18,19;,
 4;19,18,20,21;,
 4;21,20,22,23;,
 4;23,22,24,25;;
 
 MeshMaterialList {
  1;
  11;
  0,
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
    "data\\MAP\\TEXTURE\\02_STAGE\\TOWN_SIDE_FIELD_ï“èW.png";
   }
  }
 }
 MeshNormals {
  12;
  0.000000;1.000000;0.000000;,
  -0.148573;0.000000;-0.988901;,
  -0.295206;0.000000;-0.955433;,
  -0.568446;0.000000;-0.822720;,
  -0.794658;0.000000;-0.607057;,
  -0.946307;0.000000;-0.323270;,
  -1.000000;0.000000;0.000000;,
  -0.946307;0.000000;0.323270;,
  -0.794658;0.000000;0.607057;,
  -0.568446;0.000000;0.822721;,
  -0.295206;0.000000;0.955434;,
  -0.148572;0.000000;0.988902;;
  11;
  4;0,0,0,0;,
  4;1,1,2,2;,
  4;2,2,3,3;,
  4;3,3,4,4;,
  4;4,4,5,5;,
  4;5,5,6,6;,
  4;6,6,7,7;,
  4;7,7,8,8;,
  4;8,8,9,9;,
  4;9,9,10,10;,
  4;10,10,11,11;;
 }
 MeshTextureCoords {
  26;
  0.742222;0.976791;,
  0.742222;0.492346;,
  0.257778;0.492346;,
  0.257778;0.976791;,
  0.023068;0.415686;,
  0.023068;0.039746;,
  0.115954;0.039746;,
  0.115954;0.415686;,
  0.208841;0.039746;,
  0.208841;0.415686;,
  0.301727;0.039746;,
  0.301727;0.415686;,
  0.394614;0.039746;,
  0.394614;0.415686;,
  0.487500;0.039746;,
  0.487500;0.415686;,
  0.580386;0.039746;,
  0.580386;0.415686;,
  0.673273;0.039746;,
  0.673273;0.415686;,
  0.766159;0.039746;,
  0.766159;0.415686;,
  0.859045;0.039746;,
  0.859045;0.415686;,
  0.951932;0.039746;,
  0.951932;0.415686;;
 }
}
