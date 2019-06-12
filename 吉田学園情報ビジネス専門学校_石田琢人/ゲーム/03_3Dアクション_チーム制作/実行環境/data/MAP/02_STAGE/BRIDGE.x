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
 24;
 1288.73967;9.79792;-2238.72210;,
 -1144.36271;9.79792;-2238.72210;,
 -1144.36271;239.34136;-2238.72210;,
 1288.73967;239.34136;-2238.72210;,
 1288.73967;239.34136;-2238.72210;,
 -1144.36271;239.34136;-2238.72210;,
 -1144.36271;239.34136;2252.51742;,
 1288.73967;239.34136;2252.51742;,
 1288.73967;239.34136;2252.51742;,
 -1144.36271;239.34136;2252.51742;,
 -1144.36271;9.79792;2252.51742;,
 1288.73967;9.79792;2252.51742;,
 1288.73967;9.79792;2252.51742;,
 -1144.36271;9.79792;2252.51742;,
 -1144.36271;9.79792;-2238.72210;,
 1288.73967;9.79792;-2238.72210;,
 -1144.36271;9.79792;-2238.72210;,
 -1144.36271;9.79792;2252.51742;,
 -1144.36271;239.34136;2252.51742;,
 -1144.36271;239.34136;-2238.72210;,
 1288.73967;9.79792;2252.51742;,
 1288.73967;9.79792;-2238.72210;,
 1288.73967;239.34136;-2238.72210;,
 1288.73967;239.34136;2252.51742;;
 
 6;
 4;0,1,2,3;,
 4;4,5,6,7;,
 4;8,9,10,11;,
 4;12,13,14,15;,
 4;16,17,18,19;,
 4;20,21,22,23;;
 
 MeshMaterialList {
  1;
  6;
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
    "data\\MAP\\TEXTURE\\02_STAGE\\‹´.jpg";
   }
  }
 }
 MeshNormals {
  6;
  0.000000;0.000000;-1.000000;,
  0.000000;1.000000;0.000000;,
  0.000000;0.000000;1.000000;,
  0.000000;-1.000000;0.000000;,
  -1.000000;0.000000;0.000000;,
  1.000000;0.000000;0.000000;;
  6;
  4;0,0,0,0;,
  4;1,1,1,1;,
  4;2,2,2,2;,
  4;3,3,3,3;,
  4;4,4,4,4;,
  4;5,5,5,5;;
 }
 MeshTextureCoords {
  24;
  0.003099;0.988554;,
  0.994855;0.992938;,
  0.991612;0.003872;,
  -0.001338;0.010886;,
  -0.001125;0.994933;,
  0.995429;0.994352;,
  0.990854;0.000106;,
  0.009547;0.009428;,
  -0.003155;0.993001;,
  1.000638;0.998687;,
  0.998676;0.000923;,
  0.001307;0.001008;,
  0.001695;0.994569;,
  0.993843;0.992255;,
  0.998472;0.004736;,
  -0.005248;0.004736;,
  0.007916;0.987010;,
  0.996061;0.990656;,
  0.993957;0.008480;,
  -0.002755;0.008969;,
  -0.002889;0.992963;,
  0.994973;0.996515;,
  0.993225;0.007114;,
  0.003881;0.007520;;
 }
}
