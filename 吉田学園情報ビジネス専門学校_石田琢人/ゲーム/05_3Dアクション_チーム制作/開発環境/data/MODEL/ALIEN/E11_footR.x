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
 99;
 0.71823;-10.41636;-6.27722;,
 -0.55366;-10.68741;-12.55479;,
 0.12971;-7.38788;-7.22434;,
 -5.45798;-11.69296;-10.54936;,
 -5.75370;-10.65282;-5.39293;,
 -5.03644;-7.46584;-6.34439;,
 -0.23402;-8.53692;5.92473;,
 -0.26536;-3.71979;5.00751;,
 -2.81197;-6.50369;4.32471;,
 1.61669;-6.54259;3.55045;,
 0.42390;-9.68945;2.19863;,
 0.09050;-9.56436;3.07321;,
 -3.50003;-9.95086;2.29175;,
 -2.61231;-9.72919;3.36318;,
 -4.40131;-6.56054;-3.13253;,
 -5.40845;-10.28338;-2.65895;,
 0.43963;-6.46279;-4.34726;,
 1.34500;-10.03782;-3.79650;,
 0.42390;-9.68945;2.19863;,
 1.37077;-3.62322;-0.04204;,
 1.39512;-3.20188;2.17201;,
 1.39512;-3.20188;2.17201;,
 -3.39724;-3.11597;2.79406;,
 -2.85059;-2.88619;0.09372;,
 -3.50003;-9.95086;2.29175;,
 -3.39724;-3.11597;2.79406;,
 -0.67272;-9.55461;4.71812;,
 -1.83385;-0.20236;0.56665;,
 -2.03644;-0.91644;-1.28686;,
 1.33225;-0.41648;0.10173;,
 -0.47356;0.18946;1.62360;,
 0.42390;-9.68945;2.19863;,
 0.51884;-2.96231;-2.23863;,
 0.78191;-9.79662;0.80801;,
 1.30192;-9.89788;-0.48464;,
 0.13298;-4.70459;-2.62201;,
 0.39622;-3.88428;-2.19988;,
 0.42390;-9.68945;2.19863;,
 -4.03726;-10.02475;1.36811;,
 -3.50003;-9.95086;2.29175;,
 -3.18536;-3.82613;-0.69169;,
 -3.52575;-4.85237;-1.40281;,
 -4.66328;-10.06170;0.11196;,
 -5.00295;-10.02475;-0.99640;,
 -3.95173;-5.55433;-2.21561;,
 -5.40845;-10.28338;-2.65895;,
 1.31696;-9.79170;-1.99513;,
 1.34500;-10.03782;-3.79650;,
 0.26793;-5.45302;-3.16310;,
 -3.83051;-12.60132;-13.75466;,
 -2.34893;-7.42528;-6.80215;,
 -1.88298;-6.50969;-3.76445;,
 -2.38691;-10.52982;-5.85295;,
 -5.75370;-10.65282;-5.39293;,
 -5.45798;-11.69296;-10.54936;,
 -3.83051;-12.60132;-13.75466;,
 -0.55366;-10.68741;-12.55479;,
 0.71823;-10.41636;-6.27722;,
 -1.75660;-5.50162;-2.70851;,
 -1.89521;-10.15563;-3.25072;,
 -5.40845;-10.28338;-2.65895;,
 1.34500;-10.03782;-3.79650;,
 -1.62242;-4.77549;-2.03706;,
 -1.71524;-9.90351;-1.51595;,
 -5.00295;-10.02475;-0.99640;,
 1.31696;-9.79170;-1.99513;,
 -1.54066;-3.74496;-1.50367;,
 -1.56010;-9.97648;-0.19840;,
 -4.66328;-10.06170;0.11196;,
 1.30192;-9.89788;-0.48464;,
 -1.44897;-2.73663;-0.28138;,
 -1.53026;-9.90607;1.07674;,
 -4.03726;-10.02475;1.36811;,
 0.78191;-9.79662;0.80801;,
 -1.21340;-0.93342;-1.34864;,
 -1.45874;-9.81487;2.24331;,
 -3.50003;-9.95086;2.29175;,
 0.42390;-9.68945;2.19863;,
 -1.20627;-9.64344;3.21233;,
 -2.61231;-9.72919;3.36318;,
 0.09050;-9.56436;3.07321;,
 -0.67272;-9.55461;4.71812;,
 1.33225;-0.41648;0.10173;,
 1.33225;-0.41648;0.10173;,
 1.37077;-3.62322;-0.04204;,
 0.51884;-2.96231;-2.23863;,
 0.48635;-0.83880;-1.55028;,
 -2.85059;-2.88619;0.09372;,
 -1.21340;-0.93342;-1.34864;,
 -2.03767;-3.37810;3.75493;,
 -1.24334;-0.03226;1.02548;,
 -1.83385;-0.20236;0.56665;,
 -0.00544;0.00079;-0.01245;,
 -0.00544;0.00079;-0.01245;,
 -0.00544;0.00079;-0.01245;,
 -0.00544;0.00079;-0.01245;,
 -0.00544;0.00079;-0.01245;,
 -0.00544;0.00079;-0.01245;,
 -0.00544;0.00079;-0.01245;;
 
 71;
 3;0,1,2;,
 3;3,4,5;,
 3;6,7,8;,
 3;7,6,9;,
 3;10,9,11;,
 3;8,12,13;,
 4;14,5,4,15;,
 4;0,2,16,17;,
 3;18,19,20;,
 3;9,10,21;,
 3;7,9,21;,
 3;12,8,22;,
 3;23,24,25;,
 3;8,13,26;,
 3;6,8,26;,
 3;9,6,26;,
 3;11,9,26;,
 4;25,27,28,23;,
 4;21,29,30,7;,
 3;19,31,32;,
 4;33,34,35,36;,
 4;32,37,33,36;,
 4;38,39,23,40;,
 4;41,42,38,40;,
 4;43,42,41,44;,
 4;14,45,43,44;,
 4;46,47,16,48;,
 4;35,34,46,48;,
 4;49,3,5,50;,
 4;2,1,49,50;,
 4;51,16,2,50;,
 4;5,14,51,50;,
 4;52,53,54,55;,
 4;56,57,52,55;,
 4;58,48,16,51;,
 4;14,44,58,51;,
 4;59,60,53,52;,
 4;57,61,59,52;,
 4;62,35,48,58;,
 4;44,41,62,58;,
 4;63,64,60,59;,
 4;61,65,63,59;,
 4;66,36,35,62;,
 4;41,40,66,62;,
 4;67,68,64,63;,
 4;65,69,67,63;,
 4;70,32,36,66;,
 4;40,23,70,66;,
 4;71,72,68,67;,
 4;69,73,71,67;,
 4;23,28,74,70;,
 4;75,76,72,71;,
 4;73,77,75,71;,
 4;78,79,76,75;,
 4;77,80,78,75;,
 3;80,81,78;,
 3;81,79,78;,
 3;20,19,82;,
 4;83,84,85,86;,
 4;86,85,87,88;,
 4;89,7,30,90;,
 4;91,22,89,90;,
 3;22,8,89;,
 3;8,7,89;,
 3;86,88,92;,
 3;74,28,93;,
 3;28,27,94;,
 3;91,90,95;,
 3;90,30,96;,
 3;30,29,97;,
 3;83,86,98;;
 
 MeshMaterialList {
  1;
  71;
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
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
  59;
  0.248411;0.632675;-0.733494;,
  0.573860;0.711108;-0.406214;,
  0.803303;-0.591860;-0.066379;,
  0.569072;0.708120;-0.417998;,
  -0.351713;-0.782750;0.513421;,
  0.172992;-0.268437;0.947637;,
  0.171760;0.417134;0.892467;,
  -0.818078;0.205757;-0.537040;,
  -0.682895;0.699422;-0.210863;,
  -0.767761;-0.622860;0.150296;,
  0.852987;-0.006485;-0.521892;,
  -0.941327;-0.027012;-0.336413;,
  -0.707761;0.657089;-0.259439;,
  -0.664241;-0.031989;0.746833;,
  0.920435;-0.231108;0.315260;,
  0.495022;-0.826469;0.268147;,
  0.973485;-0.090207;-0.210213;,
  0.984573;0.106040;0.139181;,
  -0.890608;0.307701;0.334869;,
  0.122835;-0.871312;0.475107;,
  -0.515099;0.857130;0.000446;,
  0.105563;0.942723;0.316434;,
  0.800823;0.595378;-0.064864;,
  -0.464371;0.583321;-0.666405;,
  0.561543;0.554123;-0.614506;,
  -0.766284;0.582110;-0.271950;,
  -0.818789;-0.474771;0.322765;,
  0.840988;-0.539498;-0.041008;,
  0.776543;-0.630057;-0.003177;,
  0.792887;-0.609277;0.010580;,
  -0.750145;-0.617922;0.235489;,
  -0.757022;-0.611258;0.230828;,
  0.554080;0.438087;-0.707867;,
  -0.795231;0.553807;-0.246790;,
  0.749080;-0.659787;0.059673;,
  -0.713418;-0.629271;0.308306;,
  -0.716044;0.640070;-0.278552;,
  -0.759340;-0.609327;0.228304;,
  0.555281;0.647769;-0.521592;,
  0.798227;-0.602210;0.013281;,
  -0.164960;0.889451;-0.426222;,
  -0.248848;-0.400923;-0.881666;,
  -0.128514;0.864003;-0.486810;,
  0.096944;-0.984320;0.147365;,
  -0.196000;0.701421;-0.685268;,
  0.057431;-0.989228;0.134646;,
  -0.264477;0.598428;-0.756264;,
  0.041932;-0.998298;0.040527;,
  -0.340558;0.534495;-0.773521;,
  0.034006;-0.999420;-0.001592;,
  -0.337040;0.231512;-0.912582;,
  0.052408;-0.996643;0.062893;,
  -0.195722;0.400884;-0.894978;,
  0.067394;-0.991204;0.113894;,
  0.068957;-0.992994;0.095954;,
  0.611951;0.089713;-0.785791;,
  -0.358109;0.898650;0.253351;,
  -0.510628;0.462243;0.724976;,
  -0.007659;0.949405;-0.313961;;
  71;
  3;2,10,3;,
  3;11,9,12;,
  3;5,6,13;,
  3;6,5,14;,
  3;27,14,15;,
  3;13,26,4;,
  4;8,12,9,31;,
  4;2,3,1,29;,
  3;27,16,17;,
  3;14,27,17;,
  3;6,14,17;,
  3;26,13,18;,
  3;7,26,18;,
  3;13,4,19;,
  3;5,13,19;,
  3;14,5,19;,
  3;15,14,19;,
  4;18,20,23,7;,
  4;17,22,21,6;,
  3;16,27,55;,
  4;34,28,24,32;,
  4;55,27,34,32;,
  4;35,26,7,33;,
  4;25,30,35,33;,
  4;37,30,25,36;,
  4;8,31,37,36;,
  4;39,29,1,38;,
  4;24,28,39,38;,
  4;41,11,12,40;,
  4;3,10,41,40;,
  4;42,1,3,40;,
  4;12,8,42,40;,
  4;43,9,11,41;,
  4;10,2,43,41;,
  4;44,38,1,42;,
  4;8,36,44,42;,
  4;45,31,9,43;,
  4;2,29,45,43;,
  4;46,24,38,44;,
  4;36,25,46,44;,
  4;47,37,31,45;,
  4;29,39,47,45;,
  4;48,32,24,46;,
  4;25,33,48,46;,
  4;49,30,37,47;,
  4;39,28,49,47;,
  4;50,55,32,48;,
  4;33,7,50,48;,
  4;51,35,30,49;,
  4;28,34,51,49;,
  4;7,23,52,50;,
  4;53,26,35,51;,
  4;34,27,53,51;,
  4;54,4,26,53;,
  4;27,15,54,53;,
  3;15,19,54;,
  3;19,4,54;,
  3;17,16,22;,
  4;22,16,55,0;,
  4;0,55,7,52;,
  4;57,6,21,56;,
  4;20,18,57,56;,
  3;18,13,57;,
  3;13,6,57;,
  3;0,52,58;,
  3;52,23,58;,
  3;23,20,58;,
  3;20,56,58;,
  3;56,21,58;,
  3;21,22,58;,
  3;22,0,58;;
 }
 MeshTextureCoords {
  99;
  0.735340;0.441890;,
  0.719520;0.461700;,
  0.735340;0.461700;,
  0.719520;0.481520;,
  0.735340;0.505310;,
  0.735340;0.481520;,
  0.822310;0.568730;,
  0.853930;0.568730;,
  0.822310;0.548910;,
  0.822310;0.588540;,
  0.822310;0.608360;,
  0.778820;0.588540;,
  0.822310;0.529090;,
  0.778820;0.548910;,
  0.774870;0.481520;,
  0.774870;0.509270;,
  0.774870;0.461700;,
  0.774870;0.433960;,
  0.818740;0.405110;,
  0.834170;0.437920;,
  0.849980;0.422070;,
  0.848950;0.603590;,
  0.847060;0.532560;,
  0.834170;0.481520;,
  0.830210;0.521160;,
  0.856750;0.510110;,
  0.778820;0.568730;,
  0.856750;0.510110;,
  0.834170;0.481520;,
  0.848950;0.603590;,
  0.853930;0.568730;,
  0.834170;0.437920;,
  0.834170;0.461700;,
  0.820980;0.461700;,
  0.805390;0.461700;,
  0.805390;0.461700;,
  0.820980;0.461700;,
  0.834170;0.461700;,
  0.820980;0.481520;,
  0.834170;0.481520;,
  0.820980;0.481520;,
  0.805390;0.481520;,
  0.805390;0.481520;,
  0.791960;0.481520;,
  0.791960;0.481520;,
  0.774870;0.481520;,
  0.791960;0.461700;,
  0.774870;0.461700;,
  0.791960;0.461700;,
  0.719520;0.471210;,
  0.735340;0.471210;,
  0.774870;0.471210;,
  0.738710;0.590680;,
  0.724970;0.587260;,
  0.734880;0.607020;,
  0.740040;0.607020;,
  0.744790;0.607020;,
  0.751390;0.593840;,
  0.791960;0.471210;,
  0.738850;0.574210;,
  0.721660;0.570780;,
  0.754700;0.577370;,
  0.805390;0.471210;,
  0.738880;0.568780;,
  0.722640;0.565920;,
  0.753850;0.571420;,
  0.820980;0.471210;,
  0.738890;0.564510;,
  0.723400;0.562100;,
  0.753170;0.566740;,
  0.834170;0.471210;,
  0.738800;0.558980;,
  0.724250;0.557880;,
  0.752210;0.560000;,
  0.834170;0.471210;,
  0.738710;0.554310;,
  0.724970;0.554310;,
  0.751390;0.554310;,
  0.738580;0.541130;,
  0.728270;0.541130;,
  0.748090;0.541130;,
  0.738180;0.531250;,
  0.842070;0.430000;,
  0.654460;0.858490;,
  0.654460;0.858490;,
  0.654460;0.858490;,
  0.654460;0.858490;,
  0.666680;0.851770;,
  0.666680;0.851770;,
  0.850040;0.548260;,
  0.850040;0.548260;,
  0.847060;0.532560;,
  0.660570;0.855130;,
  0.834170;0.476370;,
  0.845460;0.495820;,
  0.848550;0.540410;,
  0.851980;0.558490;,
  0.851440;0.586160;,
  0.654460;0.858490;;
 }
}
