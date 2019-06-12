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
 -6.13995;0.58500;0.68058;,
 -5.84245;0.33579;-1.17932;,
 -5.84245;0.83489;2.54118;,
 -4.98005;1.06029;4.21908;,
 -4.98005;0.11109;-2.85792;,
 -3.63605;1.23809;5.55048;,
 -3.63605;-0.06671;-4.18932;,
 -1.94345;1.35219;6.40588;,
 -1.94345;-0.18081;-5.04472;,
 -0.06605;1.39279;6.70058;,
 -0.06605;-0.22141;-5.33942;,
 1.81065;1.35219;6.40588;,
 1.81065;-0.18081;-5.04472;,
 3.50395;1.23809;5.55048;,
 3.50395;-0.06671;-4.18932;,
 4.84725;0.11109;-2.85792;,
 4.84725;1.06029;4.21908;,
 5.71035;0.33579;-1.17932;,
 5.71035;0.83489;2.54118;,
 6.00715;0.58500;0.68058;,
 5.71035;0.83489;2.54118;,
 5.48565;6.50839;-0.13632;,
 4.65335;6.74219;1.47158;,
 4.84725;1.06029;4.21908;,
 3.36395;6.90389;2.75398;,
 3.50395;1.23809;5.55048;,
 1.73995;6.97809;3.58838;,
 1.81065;1.35219;6.40588;,
 -0.06605;6.95569;3.89288;,
 -0.06605;1.39279;6.70058;,
 -1.88185;6.84019;3.63598;,
 -1.94345;1.35219;6.40588;,
 -3.53105;6.64139;2.83658;,
 -3.63605;1.23809;5.55048;,
 -4.85125;6.37679;1.56818;,
 -4.98005;1.06029;4.21908;,
 -5.70875;6.07299;-0.04882;,
 -5.84245;0.83489;2.54118;,
 -6.01395;5.76009;-1.85622;,
 -6.13995;0.58500;0.68058;,
 -5.73255;5.46889;-3.67482;,
 -5.84245;0.33579;-1.17932;,
 -4.88975;5.22809;-5.32122;,
 -4.98005;0.11109;-2.85792;,
 -3.56955;5.06359;-6.62812;,
 -3.63605;-0.06671;-4.18932;,
 -1.90565;4.99149;-7.46392;,
 -1.94345;-0.18081;-5.04472;,
 -0.06605;5.01809;-7.74812;,
 -0.06605;-0.22141;-5.33942;,
 1.76305;5.14059;-7.45412;,
 1.81065;-0.18081;-5.04472;,
 3.40175;5.34569;-6.61692;,
 3.50395;-0.06671;-4.18932;,
 4.69115;5.61239;-5.32402;,
 4.84725;0.11109;-2.85792;,
 5.50945;5.91479;-3.70492;,
 5.71035;0.33579;-1.17932;,
 5.78175;6.22349;-1.91852;,
 6.00715;0.58500;0.68058;,
 5.48565;6.50839;-0.13632;,
 5.71035;0.83489;2.54118;,
 3.61525;15.57759;-3.89602;,
 3.05665;15.76869;-2.83902;,
 2.19845;15.91779;-2.00392;,
 1.12255;16.01229;-1.46912;,
 -0.06605;16.04239;-1.28432;,
 -1.25605;16.00599;-1.46702;,
 -2.33265;15.90589;-2.00042;,
 -3.19225;15.75189;-2.83482;,
 -3.75225;15.55799;-3.89252;,
 -3.95385;15.34309;-5.07202;,
 -3.77465;15.12749;-6.25852;,
 -3.22935;14.93359;-7.33582;,
 -2.36905;14.77959;-8.19472;,
 -1.27845;14.68159;-8.74772;,
 -0.06605;14.65079;-8.93812;,
 1.14565;14.68859;-8.74702;,
 2.23485;14.79219;-8.19402;,
 3.09375;14.95109;-7.33582;,
 3.63765;15.14779;-6.25992;,
 3.81615;15.36409;-5.07482;,
 3.61525;15.57759;-3.89602;,
 1.79385;24.58379;-2.57372;,
 1.50895;24.69790;-2.04382;,
 1.07285;24.79380;-1.62942;,
 0.53035;24.86309;-1.36762;,
 -0.06605;24.89950;-1.28292;,
 -0.66035;24.90019;-1.38092;,
 -1.19655;24.86519;-1.65182;,
 -1.62355;24.79729;-2.07042;,
 -1.90145;24.70280;-2.59752;,
 -2.00295;24.59079;-3.18412;,
 -1.91615;24.47249;-3.77492;,
 -1.64735;24.35769;-4.31182;,
 -1.22035;24.25899;-4.74232;,
 -0.67505;24.18619;-5.02092;,
 -0.06605;24.14629;-5.11822;,
 0.54505;24.14490;-5.02372;,
 1.09735;24.18060;-4.74512;,
 1.53275;24.25129;-4.31112;,
 1.80855;24.34929;-3.76652;,
 1.89815;24.46409;-3.16732;,
 1.79385;24.58379;-2.57372;,
 -0.06605;34.29629;6.37578;;
 
 98;
 3;0,1,2;,
 3;2,1,3;,
 3;1,4,3;,
 3;3,4,5;,
 3;4,6,5;,
 3;5,6,7;,
 3;6,8,7;,
 3;7,8,9;,
 3;8,10,9;,
 3;9,10,11;,
 3;10,12,11;,
 3;11,12,13;,
 3;12,14,13;,
 3;14,15,13;,
 3;13,15,16;,
 3;15,17,16;,
 3;16,17,18;,
 3;18,17,19;,
 4;20,21,22,23;,
 4;23,22,24,25;,
 4;25,24,26,27;,
 4;27,26,28,29;,
 4;29,28,30,31;,
 4;31,30,32,33;,
 4;33,32,34,35;,
 4;35,34,36,37;,
 4;37,36,38,39;,
 4;39,38,40,41;,
 4;41,40,42,43;,
 4;43,42,44,45;,
 4;45,44,46,47;,
 4;47,46,48,49;,
 4;49,48,50,51;,
 4;51,50,52,53;,
 4;53,52,54,55;,
 4;55,54,56,57;,
 4;57,56,58,59;,
 4;59,58,60,61;,
 4;21,62,63,22;,
 4;22,63,64,24;,
 4;24,64,65,26;,
 4;26,65,66,28;,
 4;28,66,67,30;,
 4;30,67,68,32;,
 4;32,68,69,34;,
 4;34,69,70,36;,
 4;36,70,71,38;,
 4;38,71,72,40;,
 4;40,72,73,42;,
 4;42,73,74,44;,
 4;44,74,75,46;,
 4;46,75,76,48;,
 4;48,76,77,50;,
 4;50,77,78,52;,
 4;52,78,79,54;,
 4;54,79,80,56;,
 4;56,80,81,58;,
 4;58,81,82,60;,
 4;62,83,84,63;,
 4;63,84,85,64;,
 4;64,85,86,65;,
 4;65,86,87,66;,
 4;66,87,88,67;,
 4;67,88,89,68;,
 4;68,89,90,69;,
 4;69,90,91,70;,
 4;70,91,92,71;,
 4;71,92,93,72;,
 4;72,93,94,73;,
 4;73,94,95,74;,
 4;74,95,96,75;,
 4;75,96,97,76;,
 4;76,97,98,77;,
 4;77,98,99,78;,
 4;78,99,100,79;,
 4;79,100,101,80;,
 4;80,101,102,81;,
 4;81,102,103,82;,
 3;83,104,84;,
 3;84,104,85;,
 3;85,104,86;,
 3;86,104,87;,
 3;87,104,88;,
 3;88,104,89;,
 3;89,104,90;,
 3;90,104,91;,
 3;91,104,92;,
 3;92,104,93;,
 3;93,104,94;,
 3;94,104,95;,
 3;95,104,96;,
 3;96,104,97;,
 3;97,104,98;,
 3;98,104,99;,
 3;99,104,100;,
 3;100,104,101;,
 3;101,104,102;,
 3;102,104,103;;
 
 MeshMaterialList {
  1;
  98;
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
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
    "data\\MODEL\\attack\\TEXTURE\\UV_attack.jpg";
   }
  }
 }
 MeshNormals {
  103;
  -0.000683;-0.991122;0.132958;,
  -0.000098;-0.991124;0.132942;,
  0.000028;-0.991143;0.132797;,
  0.000092;-0.991149;0.132757;,
  -0.000182;-0.991140;0.132819;,
  0.000187;-0.991153;0.132723;,
  -0.000149;-0.991139;0.132831;,
  0.000099;-0.991124;0.132942;,
  0.000682;-0.991122;0.132958;,
  0.001005;-0.991121;0.132958;,
  0.000624;-0.991123;0.132950;,
  0.000117;-0.991132;0.132883;,
  0.000124;-0.991149;0.132752;,
  0.000092;-0.991149;0.132757;,
  -0.000182;-0.991140;0.132819;,
  0.000187;-0.991153;0.132723;,
  -0.000352;-0.991146;0.132780;,
  -0.000117;-0.991132;0.132883;,
  -0.000625;-0.991123;0.132950;,
  -0.001008;-0.991121;0.132958;,
  0.108270;-0.606606;0.787595;,
  0.932883;0.234962;0.272987;,
  0.781966;0.334574;0.525919;,
  0.559556;0.410359;0.720071;,
  0.289595;0.457315;0.840832;,
  -0.003050;0.472998;0.881058;,
  -0.294711;0.456803;0.839331;,
  -0.562541;0.409331;0.718328;,
  -0.783253;0.332513;0.525309;,
  -0.933677;0.230875;0.273758;,
  -0.993486;0.113054;-0.014284;,
  -0.951214;-0.008200;-0.308424;,
  -0.809313;-0.118374;-0.575326;,
  -0.584329;-0.204446;-0.785342;,
  -0.302507;-0.257475;-0.917712;,
  0.007150;-0.273284;-0.961907;,
  0.315233;-0.251335;-0.915128;,
  0.592854;-0.194036;-0.781584;,
  0.812914;-0.106693;-0.572527;,
  0.951487;0.001961;-0.307684;,
  0.992640;0.120160;-0.015102;,
  0.934220;0.222896;0.278479;,
  0.796301;0.236502;0.556751;,
  0.577784;0.247062;0.777898;,
  0.301289;0.253963;0.919091;,
  -0.004561;0.256110;0.966637;,
  -0.309013;0.253387;0.916682;,
  -0.582657;0.246441;0.774453;,
  -0.798279;0.235974;0.554136;,
  -0.934550;0.222608;0.277599;,
  -0.978088;0.206381;-0.027405;,
  -0.925297;0.187426;-0.329692;,
  -0.782665;0.167063;-0.599604;,
  -0.564853;0.148249;-0.811766;,
  -0.293528;0.134759;-0.946404;,
  0.004591;0.129833;-0.991525;,
  0.301610;0.134525;-0.943893;,
  0.569986;0.147620;-0.808285;,
  0.784810;0.166348;-0.596993;,
  0.925756;0.186949;-0.328672;,
  0.978108;0.206308;-0.027238;,
  0.946066;0.005103;0.323932;,
  0.769368;-0.168968;0.616055;,
  0.520914;-0.273450;0.808624;,
  0.252858;-0.323426;0.911843;,
  -0.017742;-0.336401;0.941552;,
  -0.284985;-0.318247;0.904158;,
  -0.544730;-0.264598;0.795774;,
  -0.780862;-0.160796;0.603654;,
  -0.947934;0.008203;0.318360;,
  -0.976350;0.214714;-0.025259;,
  -0.856345;0.387913;-0.340876;,
  -0.656918;0.492923;-0.570513;,
  -0.434403;0.546878;-0.715695;,
  -0.208479;0.571390;-0.793757;,
  0.014500;0.577353;-0.816366;,
  0.235560;0.568129;-0.788506;,
  0.455270;0.540871;-0.707240;,
  0.668907;0.486077;-0.562398;,
  0.859810;0.383639;-0.336969;,
  0.976573;0.213807;-0.024309;,
  0.942674;0.172000;0.285974;,
  0.790907;0.288050;0.539901;,
  0.565545;0.376319;0.733854;,
  0.292615;0.431334;0.853421;,
  -0.002405;0.450593;0.892726;,
  -0.296298;0.432989;0.851310;,
  -0.566987;0.378358;0.731690;,
  -0.791185;0.287835;0.539608;,
  -0.943230;0.166017;0.287673;,
  -0.999714;0.023907;-0.000604;,
  -0.948871;-0.121146;-0.291491;,
  -0.797447;-0.249820;-0.549243;,
  -0.568263;-0.346605;-0.746285;,
  -0.290484;-0.403145;-0.867810;,
  0.009845;-0.417356;-0.908690;,
  0.308882;-0.389978;-0.867473;,
  0.582613;-0.323696;-0.745509;,
  0.805564;-0.223622;-0.548690;,
  0.951290;-0.098568;-0.292116;,
  0.999259;0.038403;-0.002512;,
  -0.564811;0.666497;-0.486590;,
  0.835302;0.465046;-0.293262;;
  98;
  3;9,10,8;,
  3;8,10,7;,
  3;10,11,7;,
  3;7,11,6;,
  3;11,12,6;,
  3;6,12,5;,
  3;12,13,5;,
  3;5,13,4;,
  3;13,14,4;,
  3;4,14,3;,
  3;14,15,3;,
  3;3,15,2;,
  3;15,16,2;,
  3;16,17,2;,
  3;2,17,1;,
  3;17,18,1;,
  3;1,18,0;,
  3;0,18,19;,
  4;81,21,22,82;,
  4;82,22,23,83;,
  4;83,23,24,84;,
  4;84,24,25,85;,
  4;85,25,26,86;,
  4;86,26,27,87;,
  4;87,27,28,88;,
  4;88,28,29,89;,
  4;89,29,30,90;,
  4;90,30,31,91;,
  4;91,31,32,92;,
  4;92,32,33,93;,
  4;93,33,34,94;,
  4;94,34,35,95;,
  4;95,35,36,96;,
  4;96,36,37,97;,
  4;97,37,38,98;,
  4;98,38,39,99;,
  4;99,39,40,100;,
  4;100,40,21,81;,
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
  4;39,59,60,40;,
  4;40,60,41,21;,
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
  4;59,79,80,60;,
  4;60,80,61,41;,
  3;61,20,62;,
  3;62,20,63;,
  3;63,20,64;,
  3;64,20,65;,
  3;65,20,66;,
  3;66,20,67;,
  3;67,20,68;,
  3;68,101,69;,
  3;69,101,70;,
  3;70,101,71;,
  3;71,101,72;,
  3;72,101,73;,
  3;73,101,74;,
  3;74,101,75;,
  3;75,101,76;,
  3;76,101,77;,
  3;77,102,78;,
  3;78,102,79;,
  3;79,102,80;,
  3;80,102,61;;
 }
 MeshTextureCoords {
  105;
  0.132830;0.869070;,
  0.139810;0.847650;,
  0.134640;0.891360;,
  0.145040;0.912370;,
  0.154880;0.829230;,
  0.163030;0.930020;,
  0.176570;0.815580;,
  0.186840;0.942590;,
  0.202760;0.808060;,
  0.214140;0.948850;,
  0.230880;0.807400;,
  0.242260;0.948190;,
  0.258180;0.813660;,
  0.268450;0.940670;,
  0.281990;0.826230;,
  0.299980;0.843890;,
  0.290140;0.927030;,
  0.310390;0.864890;,
  0.305210;0.908600;,
  0.312190;0.887190;,
  0.141200;0.798340;,
  0.158870;0.776360;,
  0.166410;0.777040;,
  0.150170;0.799250;,
  0.173850;0.777840;,
  0.159140;0.800150;,
  0.181240;0.778750;,
  0.168110;0.801060;,
  0.188590;0.779770;,
  0.177080;0.801960;,
  0.195940;0.780860;,
  0.186040;0.802870;,
  0.203320;0.782010;,
  0.195010;0.803780;,
  0.210750;0.783160;,
  0.203980;0.804680;,
  0.218230;0.784300;,
  0.212950;0.805590;,
  0.225780;0.785370;,
  0.221910;0.806500;,
  0.233370;0.786340;,
  0.230880;0.807400;,
  0.241000;0.787200;,
  0.239850;0.808310;,
  0.248640;0.787920;,
  0.248820;0.809210;,
  0.256260;0.788520;,
  0.257780;0.810120;,
  0.263850;0.789010;,
  0.266750;0.811030;,
  0.271390;0.789410;,
  0.275720;0.811930;,
  0.278870;0.789760;,
  0.284690;0.812840;,
  0.286290;0.790110;,
  0.293660;0.813740;,
  0.293680;0.790480;,
  0.302620;0.814650;,
  0.301060;0.790930;,
  0.311590;0.815560;,
  0.308450;0.791480;,
  0.320560;0.816460;,
  0.188770;0.739160;,
  0.193880;0.739460;,
  0.198930;0.739810;,
  0.203910;0.740220;,
  0.208860;0.740690;,
  0.213760;0.741240;,
  0.218650;0.741860;,
  0.223530;0.742530;,
  0.228430;0.743260;,
  0.233350;0.744000;,
  0.238300;0.744740;,
  0.243280;0.745460;,
  0.248290;0.746140;,
  0.253320;0.746750;,
  0.258350;0.747280;,
  0.263360;0.747730;,
  0.268360;0.748110;,
  0.273310;0.748420;,
  0.278230;0.748690;,
  0.283100;0.748930;,
  0.287960;0.749180;,
  0.217890;0.702940;,
  0.220520;0.703020;,
  0.223120;0.703110;,
  0.225690;0.703220;,
  0.228220;0.703360;,
  0.230710;0.703550;,
  0.233170;0.703800;,
  0.235620;0.704100;,
  0.238060;0.704450;,
  0.240510;0.704840;,
  0.242970;0.705260;,
  0.245450;0.705690;,
  0.247960;0.706130;,
  0.250480;0.706540;,
  0.253020;0.706910;,
  0.255570;0.707230;,
  0.258110;0.707490;,
  0.260620;0.707690;,
  0.263110;0.707830;,
  0.265570;0.707930;,
  0.268000;0.708000;,
  0.247620;0.665950;;
 }
}
