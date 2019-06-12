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
 12;
 22.72701;-11.42872;-0.53344;,
 0.62927;-2.17352;-2.24396;,
 1.47201;-0.81858;-2.72110;,
 2.12897;0.47355;-2.55568;,
 2.33115;1.05813;-1.76887;,
 2.48058;1.46291;-0.52103;,
 -0.09578;-3.81159;0.13368;,
 0.84392;0.01374;3.25358;,
 -0.05035;-1.91632;3.01964;,
 2.02987;0.81211;2.04450;,
 2.33115;1.05813;0.70199;,
 -0.09140;0.04684;-0.03725;;
 
 20;
 3;0,1,2;,
 3;0,2,3;,
 3;0,3,4;,
 3;0,4,5;,
 3;0,6,1;,
 3;7,8,0;,
 3;9,7,0;,
 3;10,9,0;,
 3;5,10,0;,
 3;8,6,0;,
 3;11,7,9;,
 3;11,8,7;,
 3;11,2,1;,
 3;11,6,8;,
 3;11,9,10;,
 3;11,10,5;,
 3;11,5,4;,
 3;11,4,3;,
 3;11,3,2;,
 3;11,1,6;;
 
 MeshMaterialList {
  2;
  20;
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1;;
  Material {
   0.000000;0.000000;0.000000;1.000000;;
   5.000000;
   0.000000;0.000000;0.000000;;
   1.000000;1.000000;1.000000;;
   TextureFilename {
    "1289475i.jpg";
   }
  }
  Material {
   0.800000;0.800000;0.800000;1.000000;;
   5.000000;
   0.000000;0.000000;0.000000;;
   0.000000;0.000000;0.000000;;
  }
 }
 MeshNormals {
  23;
  0.313762;0.371458;-0.873827;,
  0.311377;0.391771;-0.865771;,
  0.537098;0.843514;0.003060;,
  0.481656;0.740333;-0.468950;,
  -0.268845;-0.752916;-0.600700;,
  0.040218;-0.123862;-0.991484;,
  -0.168951;-0.554044;-0.815163;,
  0.480177;0.700790;0.527563;,
  0.052482;0.965111;0.256529;,
  0.268077;0.200840;0.942230;,
  -0.092348;-0.534842;0.839890;,
  -0.774971;0.614379;-0.148182;,
  0.444744;0.581554;0.681174;,
  -0.253635;-0.806699;0.533766;,
  -0.712315;0.669959;0.209193;,
  -0.428716;0.897071;0.107083;,
  -0.970560;0.196530;0.139246;,
  -0.749066;0.363810;-0.553662;,
  -0.888955;0.143672;-0.434877;,
  -0.989533;-0.005265;-0.144208;,
  -0.480591;0.876868;0.011649;,
  -0.542605;0.784146;-0.301157;,
  -0.654662;0.585643;-0.477954;;
  20;
  3;0,6,5;,
  3;0,5,1;,
  3;0,1,3;,
  3;0,3,2;,
  3;4,4,6;,
  3;9,10,12;,
  3;7,9,12;,
  3;8,7,12;,
  3;2,8,12;,
  3;10,13,13;,
  3;11,14,15;,
  3;11,16,14;,
  3;11,17,18;,
  3;11,19,16;,
  3;11,15,8;,
  3;11,8,20;,
  3;11,20,21;,
  3;11,21,22;,
  3;11,22,17;,
  3;11,18,19;;
 }
 MeshTextureCoords {
  12;
  0.000000;0.000000;,
  0.000000;0.000000;,
  0.000000;0.000000;,
  0.000000;0.000000;,
  0.000000;0.000000;,
  0.000000;0.000000;,
  0.000000;0.000000;,
  0.000000;0.000000;,
  0.000000;0.000000;,
  0.000000;0.000000;,
  0.000000;0.000000;,
  0.000000;0.000000;;
 }
}
