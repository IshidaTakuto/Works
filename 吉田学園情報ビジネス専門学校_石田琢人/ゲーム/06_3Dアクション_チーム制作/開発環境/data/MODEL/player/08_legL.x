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
 105;
 1.49300;-6.78410;-5.32002;,
 2.61132;-7.15933;-4.72531;,
 2.10394;-13.87779;-3.29317;,
 1.16742;-13.86226;-4.38992;,
 3.50692;-6.96930;-4.29746;,
 2.84716;-13.89011;-3.58369;,
 4.07974;-6.97880;-3.13028;,
 3.32434;-13.89803;-2.61139;,
 4.27711;-6.98207;-1.83644;,
 3.48876;-13.90075;-1.53360;,
 4.07974;-6.97880;-0.54261;,
 3.32434;-13.89803;-0.45580;,
 3.50692;-6.96930;0.10113;,
 2.84716;-13.89011;0.51649;,
 2.61472;-6.95450;0.78352;,
 2.10394;-13.87779;0.76840;,
 1.48709;-7.14069;1.37823;,
 1.16742;-13.86226;1.26380;,
 0.24678;-6.76343;1.58315;,
 0.12929;-13.84504;1.43451;,
 -1.00549;-7.10717;2.87381;,
 -0.90884;-13.82782;1.78352;,
 -1.90423;-7.09227;2.27910;,
 -1.36485;-13.82026;1.28811;,
 -2.79643;-7.07747;1.35281;,
 -2.10807;-13.80793;0.51649;,
 -3.59474;-7.06423;0.70908;,
 -3.06575;-13.79205;-0.45580;,
 -3.79213;-7.06096;-0.58476;,
 -3.23018;-13.78932;-1.53360;,
 -3.38895;-7.06764;-1.69089;,
 -2.62722;-13.79932;-2.21141;,
 -2.81612;-7.07714;-2.85808;,
 -2.15005;-13.80724;-3.18370;,
 -2.12972;-7.08853;-3.97206;,
 -1.84535;-13.81229;-4.35530;,
 -0.99944;-6.74276;-4.56677;,
 -0.90884;-13.82782;-4.85071;,
 0.24678;-6.76343;-4.77170;,
 0.12929;-13.84504;-5.02141;,
 1.49300;-6.78410;-5.32002;,
 1.16742;-13.86226;-4.38992;,
 3.05094;-0.00229;-3.92094;,
 1.78570;0.68350;-4.59607;,
 0.37095;0.70697;-0.07557;,
 4.07012;0.36269;-2.86939;,
 4.72040;0.35190;-1.54437;,
 4.94448;0.34819;-0.07557;,
 4.72040;0.35190;1.39323;,
 4.07012;0.36269;2.71825;,
 3.05727;0.37949;3.76980;,
 1.77468;0.01888;4.44493;,
 0.37095;0.70697;4.67756;,
 -1.05506;0.05123;4.44493;,
 -2.33132;0.07240;3.76980;,
 -3.34417;0.08919;2.71825;,
 -3.99445;0.09998;1.39323;,
 -4.21853;0.10369;-0.07557;,
 -3.99445;0.09998;-1.54437;,
 -3.34417;0.08919;-2.86939;,
 -2.33132;0.07240;-3.92093;,
 -1.04379;0.73043;-4.59607;,
 0.37095;0.70697;-4.82870;,
 1.16742;-13.86226;-4.38992;,
 2.10394;-13.87779;-3.29317;,
 0.12929;-13.84504;-1.53360;,
 2.84716;-13.89011;-3.58369;,
 3.32434;-13.89803;-2.61139;,
 3.48876;-13.90075;-1.53360;,
 3.32434;-13.89803;-0.45580;,
 2.84716;-13.89011;0.51649;,
 2.10394;-13.87779;0.76840;,
 1.16742;-13.86226;1.26380;,
 0.12929;-13.84504;1.43451;,
 -0.90884;-13.82782;1.78352;,
 -1.36485;-13.82026;1.28811;,
 -2.10807;-13.80793;0.51649;,
 -3.06575;-13.79205;-0.45580;,
 -3.23018;-13.78932;-1.53360;,
 -2.62722;-13.79932;-2.21141;,
 -2.15005;-13.80724;-3.18370;,
 -1.84535;-13.81229;-4.35530;,
 -0.90884;-13.82782;-4.85071;,
 0.12929;-13.84504;-5.02141;,
 1.78570;0.68350;-4.59607;,
 3.05094;-0.00229;-3.92094;,
 4.07012;0.36269;-2.86939;,
 4.72040;0.35190;-1.54437;,
 4.94448;0.34819;-0.07557;,
 4.72040;0.35190;1.39323;,
 4.07012;0.36269;2.71825;,
 3.05727;0.37949;3.76980;,
 1.77468;0.01888;4.44493;,
 0.37095;0.70697;4.67756;,
 -1.05506;0.05123;4.44493;,
 -2.33132;0.07240;3.76980;,
 -3.34417;0.08919;2.71825;,
 -3.99445;0.09998;1.39323;,
 -4.21853;0.10369;-0.07557;,
 -3.99445;0.09998;-1.54437;,
 -3.34417;0.08919;-2.86939;,
 -2.33132;0.07240;-3.92093;,
 -1.04379;0.73043;-4.59607;,
 0.37095;0.70697;-4.82870;,
 1.78570;0.68350;-4.59607;;
 
 80;
 4;0,1,2,3;,
 4;1,4,5,2;,
 4;4,6,7,5;,
 4;6,8,9,7;,
 4;8,10,11,9;,
 4;10,12,13,11;,
 4;12,14,15,13;,
 4;14,16,17,15;,
 4;16,18,19,17;,
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
 3;42,43,44;,
 3;45,42,44;,
 3;46,45,44;,
 3;47,46,44;,
 3;48,47,44;,
 3;49,48,44;,
 3;50,49,44;,
 3;51,50,44;,
 3;52,51,44;,
 3;53,52,44;,
 3;54,53,44;,
 3;55,54,44;,
 3;56,55,44;,
 3;57,56,44;,
 3;58,57,44;,
 3;59,58,44;,
 3;60,59,44;,
 3;61,60,44;,
 3;62,61,44;,
 3;43,62,44;,
 3;63,64,65;,
 3;64,66,65;,
 3;66,67,65;,
 3;67,68,65;,
 3;68,69,65;,
 3;69,70,65;,
 3;70,71,65;,
 3;71,72,65;,
 3;72,73,65;,
 3;73,74,65;,
 3;74,75,65;,
 3;75,76,65;,
 3;76,77,65;,
 3;77,78,65;,
 3;78,79,65;,
 3;79,80,65;,
 3;80,81,65;,
 3;81,82,65;,
 3;82,83,65;,
 3;83,63,65;,
 4;84,85,1,0;,
 4;85,86,4,1;,
 4;86,87,6,4;,
 4;87,88,8,6;,
 4;88,89,10,8;,
 4;89,90,12,10;,
 4;90,91,14,12;,
 4;91,92,16,14;,
 4;92,93,18,16;,
 4;93,94,20,18;,
 4;94,95,22,20;,
 4;95,96,24,22;,
 4;96,97,26,24;,
 4;97,98,28,26;,
 4;98,99,30,28;,
 4;99,100,32,30;,
 4;100,101,34,32;,
 4;101,102,36,34;,
 4;102,103,38,36;,
 4;103,104,40,38;;
 
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
   0.097600;0.097600;0.097600;1.000000;;
   5.000000;
   0.000000;0.000000;0.000000;;
   0.000000;0.000000;0.000000;;
   TextureFilename {
    "data\\TEXTURE\\COMMON\\�S����.png";
   }
  }
 }
 MeshNormals {
  103;
  0.223428;0.972559;0.064875;,
  0.171624;0.983226;-0.061740;,
  -0.011131;0.990536;-0.136801;,
  0.075191;0.997000;-0.018358;,
  0.078207;0.996937;0.000000;,
  0.075191;0.997000;0.018358;,
  0.066436;0.997180;0.034916;,
  -0.053607;0.991099;0.121853;,
  0.144285;0.983953;0.104969;,
  0.012378;0.999923;0.000000;,
  -0.239178;0.968792;0.065083;,
  -0.069876;0.990971;0.114427;,
  -0.102331;0.991272;0.083118;,
  -0.123153;0.991426;0.043690;,
  -0.130327;0.991471;0.000000;,
  -0.123153;0.991426;-0.043690;,
  -0.102331;0.991272;-0.083118;,
  -0.247594;0.968761;0.014126;,
  -0.194972;0.978579;0.066097;,
  0.016584;0.999863;0.000000;,
  0.348437;-0.111565;-0.930669;,
  0.339039;-0.174524;-0.924443;,
  0.048639;-0.165369;-0.985032;,
  0.946491;-0.125626;-0.297277;,
  0.993571;-0.113209;0.000095;,
  0.927856;-0.088141;0.362373;,
  0.669070;-0.043710;0.741913;,
  0.469549;-0.027586;0.882475;,
  0.318183;-0.030845;0.947528;,
  0.353532;-0.058096;0.933617;,
  0.533320;-0.089856;0.841128;,
  -0.678291;-0.154215;0.718428;,
  -0.688230;-0.162527;0.707053;,
  -0.872986;-0.143305;0.466217;,
  -0.978636;-0.072007;-0.192579;,
  -0.877966;-0.049888;-0.476116;,
  -0.907860;-0.055325;-0.415608;,
  -0.736871;-0.007027;-0.675997;,
  -0.319885;0.034152;-0.946841;,
  -0.047850;-0.003789;-0.998847;,
  -0.027838;0.999421;0.019549;,
  -0.016582;-0.999862;0.000001;,
  0.272866;-0.026637;-0.961683;,
  0.442384;-0.048210;-0.895529;,
  0.558840;-0.024786;-0.828905;,
  0.951616;-0.072258;-0.298672;,
  0.994795;-0.101900;-0.000153;,
  0.923248;-0.135248;0.359612;,
  0.702868;-0.161391;0.692770;,
  0.525021;-0.184184;0.830921;,
  0.292408;-0.209723;0.933013;,
  0.285231;-0.204005;0.936496;,
  0.166430;-0.216299;0.962037;,
  -0.645605;-0.173474;0.743708;,
  -0.716190;-0.160656;0.679162;,
  -0.893319;-0.123861;0.432017;,
  -0.990376;-0.069239;-0.119836;,
  -0.909517;-0.057032;-0.411736;,
  -0.879528;-0.053938;-0.472780;,
  -0.689788;-0.014912;-0.723858;,
  -0.318328;0.014209;-0.947874;,
  -0.096119;0.008519;-0.995333;,
  0.889684;-0.129954;-0.437692;,
  -0.642496;-0.150236;0.751417;,
  -0.016584;-0.999862;0.000001;,
  -0.016574;-0.999863;0.000010;,
  -0.016576;-0.999863;0.000006;,
  -0.016583;-0.999862;-0.000000;,
  -0.016581;-0.999863;0.000000;,
  -0.016583;-0.999862;0.000000;,
  -0.016581;-0.999863;0.000000;,
  -0.016578;-0.999863;-0.000004;,
  -0.016583;-0.999862;-0.000001;,
  -0.016586;-0.999863;0.000000;,
  -0.016581;-0.999863;0.000001;,
  -0.016582;-0.999863;0.000000;,
  -0.016584;-0.999862;0.000000;,
  -0.016583;-0.999862;-0.000000;,
  -0.016583;-0.999862;-0.000002;,
  -0.016584;-0.999863;0.000002;,
  -0.016582;-0.999863;0.000002;,
  -0.016580;-0.999863;-0.000002;,
  -0.016583;-0.999862;-0.000001;,
  0.192940;0.059257;-0.979420;,
  0.529904;0.073908;-0.844831;,
  0.757178;0.050779;-0.651232;,
  0.954006;-0.018645;-0.299208;,
  0.995890;-0.090571;-0.000402;,
  0.916559;-0.182086;0.356041;,
  0.724092;-0.273785;0.633034;,
  0.564709;-0.335687;0.753935;,
  0.256174;-0.385240;0.886547;,
  0.209345;-0.344929;0.914986;,
  -0.051977;-0.271899;0.960921;,
  -0.611315;-0.192513;0.767615;,
  -0.743051;-0.158532;0.650187;,
  -0.911480;-0.104644;0.397810;,
  -0.996582;-0.066228;-0.049382;,
  -0.936450;-0.063908;-0.344929;,
  -0.847450;-0.052323;-0.528291;,
  -0.641379;-0.022454;-0.766896;,
  -0.316645;-0.005727;-0.948527;,
  -0.143819;0.020722;-0.989387;;
  80;
  4;42,43,21,20;,
  4;43,44,22,21;,
  4;62,45,23,62;,
  4;45,46,24,23;,
  4;46,47,25,24;,
  4;47,48,26,25;,
  4;48,49,27,26;,
  4;49,50,28,27;,
  4;50,51,29,28;,
  4;51,52,30,29;,
  4;63,53,31,63;,
  4;53,54,32,31;,
  4;54,55,33,32;,
  4;55,56,34,33;,
  4;56,57,35,34;,
  4;57,58,36,35;,
  4;58,59,37,36;,
  4;59,60,38,37;,
  4;60,61,39,38;,
  4;61,42,20,39;,
  3;1,0,40;,
  3;2,1,40;,
  3;3,2,40;,
  3;4,3,40;,
  3;5,4,40;,
  3;6,5,40;,
  3;7,6,40;,
  3;8,7,40;,
  3;9,8,40;,
  3;10,9,40;,
  3;11,10,40;,
  3;12,11,40;,
  3;13,12,40;,
  3;14,13,40;,
  3;15,14,40;,
  3;16,15,40;,
  3;17,16,40;,
  3;18,17,40;,
  3;19,18,40;,
  3;0,19,40;,
  3;64,65,41;,
  3;65,66,41;,
  3;66,67,41;,
  3;67,68,41;,
  3;68,69,41;,
  3;69,70,41;,
  3;70,71,41;,
  3;71,72,41;,
  3;72,73,41;,
  3;73,74,41;,
  3;74,75,41;,
  3;75,76,41;,
  3;76,77,41;,
  3;77,78,41;,
  3;78,79,41;,
  3;79,80,41;,
  3;80,81,41;,
  3;81,82,41;,
  3;82,73,41;,
  3;73,64,41;,
  4;83,84,43,42;,
  4;84,85,44,43;,
  4;85,86,45,44;,
  4;86,87,46,45;,
  4;87,88,47,46;,
  4;88,89,48,47;,
  4;89,90,49,48;,
  4;90,91,50,49;,
  4;91,92,51,50;,
  4;92,93,52,51;,
  4;93,94,53,52;,
  4;94,95,54,53;,
  4;95,96,55,54;,
  4;96,97,56,55;,
  4;97,98,57,56;,
  4;98,99,58,57;,
  4;99,100,59,58;,
  4;100,101,60,59;,
  4;101,102,61,60;,
  4;102,83,42,61;;
 }
 MeshTextureCoords {
  105;
  0.375000;0.485120;,
  0.387500;0.485120;,
  0.387500;0.688440;,
  0.375000;0.688440;,
  0.400000;0.485120;,
  0.400000;0.688440;,
  0.412500;0.485120;,
  0.412500;0.688440;,
  0.425000;0.485120;,
  0.425000;0.688440;,
  0.437500;0.485120;,
  0.437500;0.688440;,
  0.450000;0.485120;,
  0.450000;0.688440;,
  0.462500;0.485120;,
  0.462500;0.688440;,
  0.475000;0.485120;,
  0.475000;0.688440;,
  0.487500;0.485120;,
  0.487500;0.688440;,
  0.500000;0.485120;,
  0.500000;0.688440;,
  0.512500;0.485120;,
  0.512500;0.688440;,
  0.525000;0.485120;,
  0.525000;0.688440;,
  0.537500;0.485120;,
  0.537500;0.688440;,
  0.550000;0.485120;,
  0.550000;0.688440;,
  0.562500;0.485120;,
  0.562500;0.688440;,
  0.575000;0.485120;,
  0.575000;0.688440;,
  0.587500;0.485120;,
  0.587500;0.688440;,
  0.600000;0.485120;,
  0.600000;0.688440;,
  0.612500;0.485120;,
  0.612500;0.688440;,
  0.625000;0.485120;,
  0.625000;0.688440;,
  0.626410;0.064410;,
  0.648600;0.107970;,
  0.500000;0.150000;,
  0.591840;0.029840;,
  0.548280;0.007650;,
  0.500000;-0.000000;,
  0.451720;0.007650;,
  0.408160;0.029840;,
  0.373590;0.064410;,
  0.351400;0.107970;,
  0.343750;0.156250;,
  0.351400;0.204530;,
  0.373590;0.248090;,
  0.408160;0.282660;,
  0.451720;0.304850;,
  0.500000;0.312500;,
  0.548280;0.304850;,
  0.591840;0.282660;,
  0.626410;0.248090;,
  0.648600;0.204530;,
  0.656250;0.156250;,
  0.648600;0.892030;,
  0.626410;0.935590;,
  0.500000;0.837500;,
  0.591840;0.970160;,
  0.548280;0.992350;,
  0.500000;1.000000;,
  0.451720;0.992350;,
  0.408160;0.970160;,
  0.373590;0.935590;,
  0.351400;0.892030;,
  0.343750;0.843750;,
  0.351400;0.795470;,
  0.373590;0.751910;,
  0.408160;0.717340;,
  0.451720;0.695150;,
  0.500000;0.687500;,
  0.548280;0.695150;,
  0.591840;0.717340;,
  0.626410;0.751910;,
  0.648600;0.795470;,
  0.656250;0.843750;,
  0.375000;0.312500;,
  0.387500;0.312500;,
  0.400000;0.312500;,
  0.412500;0.312500;,
  0.425000;0.312500;,
  0.437500;0.312500;,
  0.450000;0.312500;,
  0.462500;0.312500;,
  0.475000;0.312500;,
  0.487500;0.312500;,
  0.500000;0.312500;,
  0.512500;0.312500;,
  0.525000;0.312500;,
  0.537500;0.312500;,
  0.550000;0.312500;,
  0.562500;0.312500;,
  0.575000;0.312500;,
  0.587500;0.312500;,
  0.600000;0.312500;,
  0.612500;0.312500;,
  0.625000;0.312500;;
 }
}
