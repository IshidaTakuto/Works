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
 138;
 -13.90090;0.67663;-2.48506;,
 -1.56638;0.77280;-3.94481;,
 -2.06498;-5.51909;-0.23011;,
 -14.33297;-2.96675;-0.14433;,
 -1.58481;-0.30136;3.94232;,
 -13.91292;0.00327;2.46013;,
 -1.87420;5.52401;0.33183;,
 -14.22315;3.40214;0.17952;,
 -1.56638;0.77280;-3.94481;,
 -13.90090;0.67663;-2.48506;,
 -1.87420;5.52401;0.33183;,
 -1.58481;-0.30136;3.94232;,
 -14.22315;3.40214;0.17952;,
 -13.91292;0.00327;2.46013;,
 -2.67662;-2.43607;0.39195;,
 -2.55959;-2.43607;0.62121;,
 -3.63456;-1.47093;1.40199;,
 -3.93996;-1.47093;0.80238;,
 -2.37842;-2.43607;0.80238;,
 -3.15920;-1.47093;1.87735;,
 -2.14915;-2.43607;0.91942;,
 -2.55959;-1.47093;2.18277;,
 -1.89583;-2.43607;0.95950;,
 -1.89583;-1.47093;2.28778;,
 -1.64173;-2.43607;0.91942;,
 -1.23130;-1.47093;2.18277;,
 -1.41326;-2.43607;0.80238;,
 -0.63250;-1.47093;1.87735;,
 -1.23130;-2.43607;0.62121;,
 -0.15712;-1.47093;1.40199;,
 -1.11506;-2.43607;0.39195;,
 0.14828;-1.47093;0.80238;,
 -1.07499;-2.43607;0.13864;,
 0.25410;-1.47093;0.13864;,
 -1.11506;-2.43607;-0.11548;,
 0.14828;-1.47093;-0.52591;,
 -1.23130;-2.43607;-0.34394;,
 -0.15712;-1.47093;-1.12472;,
 -1.41326;-2.43607;-0.52591;,
 -0.63250;-1.47093;-1.60088;,
 -1.64173;-2.43607;-0.64214;,
 -1.23130;-1.47093;-1.90549;,
 -1.89583;-2.43607;-0.68302;,
 -1.89583;-1.47093;-2.01131;,
 -2.14915;-2.43607;-0.64214;,
 -2.55959;-1.47093;-1.90549;,
 -2.37842;-2.43607;-0.52591;,
 -3.15920;-1.47093;-1.60088;,
 -2.55959;-2.43607;-0.34394;,
 -3.63456;-1.47093;-1.12472;,
 -2.67662;-2.43607;-0.11548;,
 -3.93996;-1.47093;-0.52591;,
 -2.71670;-2.43607;0.13864;,
 -4.04498;-1.47093;0.13864;,
 -2.67662;-2.43607;0.39195;,
 -3.93996;-1.47093;0.80238;,
 -4.04498;0.09064;1.70019;,
 -4.42254;0.09064;0.95950;,
 -3.45739;0.09064;2.28778;,
 -2.71670;0.09064;2.66534;,
 -1.89583;0.09064;2.79520;,
 -1.07499;0.09064;2.66534;,
 -0.33429;0.09064;2.28778;,
 0.25410;0.09064;1.70019;,
 0.63086;0.09064;0.95950;,
 0.76072;0.09064;0.13864;,
 0.63086;0.09064;-0.68302;,
 0.25410;0.09064;-1.42372;,
 -0.33429;0.09064;-2.01131;,
 -1.07499;0.09064;-2.38807;,
 -1.89583;0.09064;-2.51873;,
 -2.71670;0.09064;-2.38807;,
 -3.45739;0.09064;-2.01131;,
 -4.04498;0.09064;-1.42372;,
 -4.42254;0.09064;-0.68302;,
 -4.55241;0.09064;0.13864;,
 -4.42254;0.09064;0.95950;,
 -3.63456;1.65220;1.40199;,
 -3.93996;1.65220;0.80238;,
 -3.15920;1.65220;1.87735;,
 -2.55959;1.65220;2.18277;,
 -1.89583;1.65220;2.28778;,
 -1.23130;1.65220;2.18277;,
 -0.63250;1.65220;1.87735;,
 -0.15712;1.65220;1.40199;,
 0.14828;1.65220;0.80238;,
 0.25410;1.65220;0.13864;,
 0.14828;1.65220;-0.52591;,
 -0.15712;1.65220;-1.12472;,
 -0.63250;1.65220;-1.60088;,
 -1.23130;1.65220;-1.90549;,
 -1.89583;1.65220;-2.01131;,
 -2.55959;1.65220;-1.90549;,
 -3.15920;1.65220;-1.60088;,
 -3.63456;1.65220;-1.12472;,
 -3.93996;1.65220;-0.52591;,
 -4.04498;1.65220;0.13864;,
 -3.93996;1.65220;0.80238;,
 -1.89583;-2.56594;0.13864;,
 -1.89583;-2.56594;0.13864;,
 -1.89583;-2.56594;0.13864;,
 -1.89583;-2.56594;0.13864;,
 -1.89583;-2.56594;0.13864;,
 -1.89583;-2.56594;0.13864;,
 -1.89583;-2.56594;0.13864;,
 -1.89583;-2.56594;0.13864;,
 -1.89583;-2.56594;0.13864;,
 -1.89583;-2.56594;0.13864;,
 -1.89583;-2.56594;0.13864;,
 -1.89583;-2.56594;0.13864;,
 -1.89583;-2.56594;0.13864;,
 -1.89583;-2.56594;0.13864;,
 -1.89583;-2.56594;0.13864;,
 -1.89583;-2.56594;0.13864;,
 -1.89583;-2.56594;0.13864;,
 -1.89583;-2.56594;0.13864;,
 -1.89583;-2.56594;0.13864;,
 -1.89583;-2.56594;0.13864;,
 -1.89583;2.74722;0.13864;,
 -1.89583;2.74722;0.13864;,
 -1.89583;2.74722;0.13864;,
 -1.89583;2.74722;0.13864;,
 -1.89583;2.74722;0.13864;,
 -1.89583;2.74722;0.13864;,
 -1.89583;2.74722;0.13864;,
 -1.89583;2.74722;0.13864;,
 -1.89583;2.74722;0.13864;,
 -1.89583;2.74722;0.13864;,
 -1.89583;2.74722;0.13864;,
 -1.89583;2.74722;0.13864;,
 -1.89583;2.74722;0.13864;,
 -1.89583;2.74722;0.13864;,
 -1.89583;2.74722;0.13864;,
 -1.89583;2.74722;0.13864;,
 -1.89583;2.74722;0.13864;,
 -1.89583;2.74722;0.13864;,
 -1.89583;2.74722;0.13864;,
 -1.89583;2.74722;0.13864;;
 
 106;
 4;0,1,2,3;,
 4;3,2,4,5;,
 4;5,4,6,7;,
 4;7,6,8,9;,
 4;1,10,11,2;,
 4;12,0,3,13;,
 4;14,15,16,17;,
 4;15,18,19,16;,
 4;18,20,21,19;,
 4;20,22,23,21;,
 4;22,24,25,23;,
 4;24,26,27,25;,
 4;26,28,29,27;,
 4;28,30,31,29;,
 4;30,32,33,31;,
 4;32,34,35,33;,
 4;34,36,37,35;,
 4;36,38,39,37;,
 4;38,40,41,39;,
 4;40,42,43,41;,
 4;42,44,45,43;,
 4;44,46,47,45;,
 4;46,48,49,47;,
 4;48,50,51,49;,
 4;50,52,53,51;,
 4;52,54,55,53;,
 4;17,16,56,57;,
 4;16,19,58,56;,
 4;19,21,59,58;,
 4;21,23,60,59;,
 4;23,25,61,60;,
 4;25,27,62,61;,
 4;27,29,63,62;,
 4;29,31,64,63;,
 4;31,33,65,64;,
 4;33,35,66,65;,
 4;35,37,67,66;,
 4;37,39,68,67;,
 4;39,41,69,68;,
 4;41,43,70,69;,
 4;43,45,71,70;,
 4;45,47,72,71;,
 4;47,49,73,72;,
 4;49,51,74,73;,
 4;51,53,75,74;,
 4;53,55,76,75;,
 4;57,56,77,78;,
 4;56,58,79,77;,
 4;58,59,80,79;,
 4;59,60,81,80;,
 4;60,61,82,81;,
 4;61,62,83,82;,
 4;62,63,84,83;,
 4;63,64,85,84;,
 4;64,65,86,85;,
 4;65,66,87,86;,
 4;66,67,88,87;,
 4;67,68,89,88;,
 4;68,69,90,89;,
 4;69,70,91,90;,
 4;70,71,92,91;,
 4;71,72,93,92;,
 4;72,73,94,93;,
 4;73,74,95,94;,
 4;74,75,96,95;,
 4;75,76,97,96;,
 3;15,14,98;,
 3;18,15,99;,
 3;20,18,100;,
 3;22,20,101;,
 3;24,22,102;,
 3;26,24,103;,
 3;28,26,104;,
 3;30,28,105;,
 3;32,30,106;,
 3;34,32,107;,
 3;36,34,108;,
 3;38,36,109;,
 3;40,38,110;,
 3;42,40,111;,
 3;44,42,112;,
 3;46,44,113;,
 3;48,46,114;,
 3;50,48,115;,
 3;52,50,116;,
 3;54,52,117;,
 3;78,77,118;,
 3;77,79,119;,
 3;79,80,120;,
 3;80,81,121;,
 3;81,82,122;,
 3;82,83,123;,
 3;83,84,124;,
 3;84,85,125;,
 3;85,86,126;,
 3;86,87,127;,
 3;87,88,128;,
 3;88,89,129;,
 3;89,90,130;,
 3;90,91,131;,
 3;91,92,132;,
 3;92,93,133;,
 3;93,94,134;,
 3;94,95,135;,
 3;95,96,136;,
 3;96,97,137;;
 
 MeshMaterialList {
  1;
  106;
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
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
   0.190000;0.190000;0.190000;;
   TextureFilename {
    "data/MODEL/TEXTURE/NINNJYA.jpg";
   }
  }
 }
 MeshNormals {
  88;
  -0.106760;-0.513767;-0.851261;,
  -0.118887;-0.629233;0.768070;,
  -0.096746;0.534618;0.839538;,
  -0.363430;-0.924101;0.118130;,
  -0.309128;-0.924096;0.224690;,
  -0.224697;-0.924090;0.309143;,
  -0.118140;-0.924095;0.363443;,
  -0.000070;-0.924085;0.382187;,
  0.118257;-0.924113;0.363360;,
  0.224715;-0.924051;0.309245;,
  0.309131;-0.924113;0.224617;,
  0.363538;-0.924085;0.117926;,
  0.382098;-0.924122;0.000074;,
  0.363450;-0.924105;-0.118045;,
  0.309242;-0.924093;-0.224547;,
  0.224549;-0.924120;-0.309159;,
  0.118319;-0.924079;-0.363427;,
  -0.000066;-0.924150;-0.382031;,
  -0.118204;-0.924069;-0.363489;,
  -0.224548;-0.924147;-0.309080;,
  -0.309248;-0.924071;-0.224630;,
  -0.363367;-0.924111;-0.118254;,
  -0.382193;-0.924083;0.000070;,
  -0.768781;-0.588719;0.249771;,
  -0.653935;-0.588710;0.475174;,
  -0.475179;-0.588699;0.653941;,
  -0.249777;-0.588708;0.768787;,
  -0.000073;-0.588673;0.808372;,
  0.249922;-0.588698;0.768748;,
  0.475180;-0.588628;0.654004;,
  0.653931;-0.588774;0.475099;,
  0.768832;-0.588682;0.249700;,
  0.808359;-0.588690;0.000098;,
  0.768767;-0.588719;-0.249816;,
  0.654006;-0.588776;-0.474992;,
  0.474962;-0.588758;-0.654045;,
  0.249992;-0.588609;-0.768793;,
  -0.000069;-0.588761;-0.808307;,
  -0.249852;-0.588633;-0.768820;,
  -0.474969;-0.588809;-0.653993;,
  -0.654006;-0.588717;-0.475067;,
  -0.768727;-0.588741;-0.249886;,
  -0.808370;-0.588674;0.000094;,
  -0.951081;0.000004;0.308943;,
  -0.809014;0.000004;0.587790;,
  -0.587794;0.000000;0.809011;,
  -0.308943;0.000000;0.951081;,
  -0.000044;0.000000;1.000000;,
  0.309033;-0.000003;0.951051;,
  0.587757;0.000005;0.809037;,
  0.809036;0.000008;0.587759;,
  0.951060;0.000000;0.309007;,
  1.000000;0.000000;0.000085;,
  0.951045;-0.000000;-0.309053;,
  0.809081;-0.000000;-0.587697;,
  0.587536;0.000001;-0.809198;,
  0.309082;0.000001;-0.951035;,
  -0.000044;-0.000000;-1.000000;,
  -0.308993;0.000000;-0.951064;,
  -0.587572;0.000000;-0.809172;,
  -0.809059;0.000002;-0.587728;,
  -0.951066;0.000002;-0.308989;,
  -1.000000;0.000000;0.000084;,
  -0.722086;0.650833;0.234538;,
  -0.614242;0.650819;0.446253;,
  -0.446257;0.650818;0.614240;,
  -0.234539;0.650832;0.722086;,
  -0.000045;0.650811;0.759240;,
  0.234629;0.650827;0.722062;,
  0.446292;0.650818;0.614215;,
  0.614190;0.650909;0.446193;,
  0.722044;0.650816;0.234716;,
  0.759237;0.650814;0.000067;,
  0.721992;0.650854;-0.234771;,
  0.614218;0.650933;-0.446119;,
  0.446110;0.650911;-0.614248;,
  0.234677;0.650793;-0.722077;,
  -0.000046;0.650904;-0.759160;,
  -0.234589;0.650793;-0.722105;,
  -0.446071;0.650918;-0.614269;,
  -0.614257;0.650863;-0.446168;,
  -0.722029;0.650877;-0.234590;,
  -0.759234;0.650818;0.000067;,
  -0.000003;-1.000000;0.000019;,
  -0.000003;1.000000;0.000019;,
  -0.101275;0.675632;-0.730250;,
  0.999897;-0.014345;0.000383;,
  -0.999917;0.012830;-0.000684;;
  106;
  4;0,0,0,0;,
  4;1,1,1,1;,
  4;2,2,2,2;,
  4;85,85,85,85;,
  4;86,86,86,86;,
  4;87,87,87,87;,
  4;3,4,24,23;,
  4;4,5,25,24;,
  4;5,6,26,25;,
  4;6,7,27,26;,
  4;7,8,28,27;,
  4;8,9,29,28;,
  4;9,10,30,29;,
  4;10,11,31,30;,
  4;11,12,32,31;,
  4;12,13,33,32;,
  4;13,14,34,33;,
  4;14,15,35,34;,
  4;15,16,36,35;,
  4;16,17,37,36;,
  4;17,18,38,37;,
  4;18,19,39,38;,
  4;19,20,40,39;,
  4;20,21,41,40;,
  4;21,22,42,41;,
  4;22,3,23,42;,
  4;23,24,44,43;,
  4;24,25,45,44;,
  4;25,26,46,45;,
  4;26,27,47,46;,
  4;27,28,48,47;,
  4;28,29,49,48;,
  4;29,30,50,49;,
  4;30,31,51,50;,
  4;31,32,52,51;,
  4;32,33,53,52;,
  4;33,34,54,53;,
  4;34,35,55,54;,
  4;35,36,56,55;,
  4;36,37,57,56;,
  4;37,38,58,57;,
  4;38,39,59,58;,
  4;39,40,60,59;,
  4;40,41,61,60;,
  4;41,42,62,61;,
  4;42,23,43,62;,
  4;43,44,64,63;,
  4;44,45,65,64;,
  4;45,46,66,65;,
  4;46,47,67,66;,
  4;47,48,68,67;,
  4;48,49,69,68;,
  4;49,50,70,69;,
  4;50,51,71,70;,
  4;51,52,72,71;,
  4;52,53,73,72;,
  4;53,54,74,73;,
  4;54,55,75,74;,
  4;55,56,76,75;,
  4;56,57,77,76;,
  4;57,58,78,77;,
  4;58,59,79,78;,
  4;59,60,80,79;,
  4;60,61,81,80;,
  4;61,62,82,81;,
  4;62,43,63,82;,
  3;4,3,83;,
  3;5,4,83;,
  3;6,5,83;,
  3;7,6,83;,
  3;8,7,83;,
  3;9,8,83;,
  3;10,9,83;,
  3;11,10,83;,
  3;12,11,83;,
  3;13,12,83;,
  3;14,13,83;,
  3;15,14,83;,
  3;16,15,83;,
  3;17,16,83;,
  3;18,17,83;,
  3;19,18,83;,
  3;20,19,83;,
  3;21,20,83;,
  3;22,21,83;,
  3;3,22,83;,
  3;63,64,84;,
  3;64,65,84;,
  3;65,66,84;,
  3;66,67,84;,
  3;67,68,84;,
  3;68,69,84;,
  3;69,70,84;,
  3;70,71,84;,
  3;71,72,84;,
  3;72,73,84;,
  3;73,74,84;,
  3;74,75,84;,
  3;75,76,84;,
  3;76,77,84;,
  3;77,78,84;,
  3;78,79,84;,
  3;79,80,84;,
  3;80,81,84;,
  3;81,82,84;,
  3;82,63,84;;
 }
 MeshTextureCoords {
  138;
  0.203420;1.320300;,
  0.235390;1.320300;,
  0.235390;1.288330;,
  0.203420;1.288330;,
  0.235390;1.256350;,
  0.203420;1.256350;,
  0.235390;1.224380;,
  0.203420;1.224380;,
  0.235390;1.192400;,
  0.203420;1.192400;,
  0.267370;1.320300;,
  0.267370;1.288330;,
  0.171440;1.320300;,
  0.171440;1.288330;,
  0.320440;0.810740;,
  0.325720;0.810740;,
  0.325720;0.789610;,
  0.320440;0.789610;,
  0.331000;0.810740;,
  0.331000;0.789610;,
  0.336280;0.810740;,
  0.336280;0.789610;,
  0.341560;0.810740;,
  0.341560;0.789610;,
  0.346840;0.810740;,
  0.346840;0.789610;,
  0.352120;0.810740;,
  0.352120;0.789610;,
  0.357410;0.810740;,
  0.357410;0.789610;,
  0.362690;0.810740;,
  0.362690;0.789610;,
  0.367970;0.810740;,
  0.367970;0.789610;,
  0.373250;0.810740;,
  0.373250;0.789610;,
  0.378530;0.810740;,
  0.378530;0.789610;,
  0.383810;0.810740;,
  0.383810;0.789610;,
  0.389090;0.810740;,
  0.389090;0.789610;,
  0.394370;0.810740;,
  0.394370;0.789610;,
  0.399660;0.810740;,
  0.399660;0.789610;,
  0.404940;0.810740;,
  0.404940;0.789610;,
  0.410220;0.810740;,
  0.410220;0.789610;,
  0.415500;0.810740;,
  0.415500;0.789610;,
  0.420780;0.810740;,
  0.420780;0.789610;,
  0.426060;0.810740;,
  0.426060;0.789610;,
  0.325720;0.768490;,
  0.320440;0.768490;,
  0.331000;0.768490;,
  0.336280;0.768490;,
  0.341560;0.768490;,
  0.346840;0.768490;,
  0.352120;0.768490;,
  0.357410;0.768490;,
  0.362690;0.768490;,
  0.367970;0.768490;,
  0.373250;0.768490;,
  0.378530;0.768490;,
  0.383810;0.768490;,
  0.389090;0.768490;,
  0.394370;0.768490;,
  0.399660;0.768490;,
  0.404940;0.768490;,
  0.410220;0.768490;,
  0.415500;0.768490;,
  0.420780;0.768490;,
  0.426060;0.768490;,
  0.325720;0.747360;,
  0.320440;0.747360;,
  0.331000;0.747360;,
  0.336280;0.747360;,
  0.341560;0.747360;,
  0.346840;0.747360;,
  0.352120;0.747360;,
  0.357410;0.747360;,
  0.362690;0.747360;,
  0.367970;0.747360;,
  0.373250;0.747360;,
  0.378530;0.747360;,
  0.383810;0.747360;,
  0.389090;0.747360;,
  0.394370;0.747360;,
  0.399660;0.747360;,
  0.404940;0.747360;,
  0.410220;0.747360;,
  0.415500;0.747360;,
  0.420780;0.747360;,
  0.426060;0.747360;,
  0.323080;0.821300;,
  0.328360;0.821300;,
  0.333640;0.821300;,
  0.338920;0.821300;,
  0.344200;0.821300;,
  0.349480;0.821300;,
  0.354760;0.821300;,
  0.360050;0.821300;,
  0.365330;0.821300;,
  0.370610;0.821300;,
  0.375890;0.821300;,
  0.381170;0.821300;,
  0.386450;0.821300;,
  0.391730;0.821300;,
  0.397020;0.821300;,
  0.402300;0.821300;,
  0.407580;0.821300;,
  0.412860;0.821300;,
  0.418140;0.821300;,
  0.423420;0.821300;,
  0.323080;0.715670;,
  0.328360;0.715670;,
  0.333640;0.715670;,
  0.338920;0.715670;,
  0.344200;0.715670;,
  0.349480;0.715670;,
  0.354760;0.715670;,
  0.360050;0.715670;,
  0.365330;0.715670;,
  0.370610;0.715670;,
  0.375890;0.715670;,
  0.381170;0.715670;,
  0.386450;0.715670;,
  0.391730;0.715670;,
  0.397020;0.715670;,
  0.402300;0.715670;,
  0.407580;0.715670;,
  0.412860;0.715670;,
  0.418140;0.715670;,
  0.423420;0.715670;;
 }
}
