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
 107;
 8.26499;5.78041;-2.14210;,
 5.63038;3.40284;-3.26449;,
 3.30258;3.85141;-5.33152;,
 2.97799;5.08246;-2.98805;,
 5.69464;1.21482;-5.89796;,
 2.90038;1.67571;-5.94503;,
 5.68630;-2.12021;-5.27141;,
 2.66507;-1.68399;-6.29548;,
 6.59516;-5.85021;-3.29408;,
 2.83214;-5.13710;-3.88441;,
 23.60515;-4.62677;-1.75464;,
 21.16994;-4.48821;-4.39755;,
 32.07756;-3.66595;-3.65663;,
 33.99155;-4.28387;-2.45869;,
 37.07122;-4.87256;-0.09724;,
 23.46814;-4.79652;-0.07362;,
 22.42249;-1.07925;-3.91285;,
 23.79737;-1.37805;-2.34344;,
 24.46953;-1.10895;-0.07362;,
 26.15095;0.62573;-3.13054;,
 29.11267;0.71407;-1.18114;,
 25.90926;0.81654;-0.07362;,
 20.69209;-0.10434;-2.47289;,
 20.92415;1.29950;-2.05956;,
 20.79068;0.22278;-0.07362;,
 13.25533;0.59006;-4.41541;,
 10.21810;0.61878;-2.02193;,
 10.43246;0.74809;-0.07362;,
 16.89760;1.99612;-2.51396;,
 19.72581;1.91105;-1.83416;,
 7.16107;2.24938;-1.99251;,
 7.67007;3.50687;-3.23263;,
 12.55735;3.52044;-1.95656;,
 18.25235;4.00138;0.03509;,
 7.26769;2.31377;-0.07362;,
 5.26223;3.64690;-2.72603;,
 5.04509;3.91267;-0.07362;,
 12.28250;7.63393;-0.06330;,
 2.87803;4.62476;-0.07362;,
 9.97784;-9.23456;0.15085;,
 5.36246;-7.33868;-0.11285;,
 11.23348;-7.83279;-3.63766;,
 2.85499;-7.30013;-0.04494;,
 18.90150;-3.06004;-4.88963;,
 12.64438;-3.62840;-5.74386;,
 18.27086;-7.24496;-5.46152;,
 15.55214;-9.87777;-4.47133;,
 8.12368;-2.58797;-6.21462;,
 9.31266;1.41566;-4.79252;,
 10.66563;3.04112;-2.93891;,
 11.47170;2.67708;-1.96015;,
 11.07514;0.95336;-0.03031;,
 14.45876;-11.57721;-0.15430;,
 2.97799;5.08246;2.84081;,
 3.30258;3.85141;5.18428;,
 5.63038;3.40284;3.11725;,
 8.26499;5.78041;1.99486;,
 2.90038;1.67571;5.79779;,
 5.69464;1.21482;5.75072;,
 2.66507;-1.68399;6.14824;,
 5.68630;-2.12021;5.12417;,
 2.83214;-5.13710;3.73717;,
 6.59516;-5.85021;3.14684;,
 33.99155;-4.28387;2.31145;,
 32.07756;-3.66595;3.50939;,
 21.16994;-4.48821;4.25031;,
 22.15053;-4.54301;2.12939;,
 37.07122;-4.87256;-0.05000;,
 23.79737;-1.37805;2.19620;,
 22.42249;-1.07925;3.76561;,
 29.11267;0.71407;1.03390;,
 26.15095;0.62573;2.98330;,
 20.92415;1.29950;1.91232;,
 20.69209;-0.10434;2.32565;,
 10.21810;0.61878;1.87469;,
 13.25533;0.59006;4.26817;,
 19.72581;1.91105;1.68692;,
 16.89760;1.99612;2.36672;,
 12.55735;3.52044;1.80932;,
 7.67007;3.50687;3.08539;,
 7.16107;2.24938;1.84527;,
 18.25235;4.00138;-0.18233;,
 5.26223;3.64690;2.57879;,
 12.28250;7.63393;-0.08394;,
 11.23348;-7.83279;3.49042;,
 2.85499;-7.30013;-0.10230;,
 18.90150;-3.06004;4.74239;,
 13.53417;-3.89806;5.31464;,
 15.55214;-9.87777;4.32409;,
 18.42874;-7.02013;5.64013;,
 8.12368;-2.58797;6.06738;,
 9.31266;1.41566;4.64528;,
 10.66563;3.04112;2.79167;,
 11.47170;2.67708;1.81291;,
 11.07514;0.95336;-0.11693;,
 29.52910;-9.04847;0.02359;,
 29.02927;-8.76145;-1.76228;,
 28.86942;-9.18763;1.72382;,
 28.26182;-9.31499;-3.43007;,
 28.26182;-9.31499;3.46201;,
 26.35237;-10.89638;-4.77122;,
 26.35237;-10.89638;4.80316;,
 25.14034;-13.28794;-3.96878;,
 24.56353;-13.31275;4.09230;,
 24.35170;-15.39332;-0.07777;,
 -0.00134;-0.00091;-0.00164;,
 28.12916;-12.73230;-0.62086;;
 
 116;
 4;0,1,2,3;,
 4;1,4,5,2;,
 4;4,6,7,5;,
 4;6,8,9,7;,
 4;10,11,12,13;,
 4;10,13,14,15;,
 4;13,12,16,17;,
 4;13,17,18,14;,
 4;17,16,19,20;,
 4;17,20,21,18;,
 4;20,19,22,23;,
 4;20,23,24,21;,
 4;23,22,25,26;,
 4;23,26,27,24;,
 4;26,28,29,27;,
 4;30,1,31,32;,
 4;30,32,33,34;,
 4;32,31,1,35;,
 4;32,35,36,33;,
 4;35,0,37,36;,
 4;0,3,38,37;,
 4;39,40,8,41;,
 3;0,35,1;,
 4;40,42,9,8;,
 3;16,22,19;,
 4;43,22,16,11;,
 3;11,16,12;,
 4;25,22,43,44;,
 4;45,46,41,44;,
 4;45,44,43,11;,
 4;44,41,8,47;,
 4;47,6,4,48;,
 4;47,48,25,44;,
 4;48,4,1,49;,
 4;48,49,28,25;,
 4;49,1,30,50;,
 4;50,30,34,51;,
 4;50,51,29,28;,
 4;46,52,39,41;,
 3;28,26,25;,
 3;28,49,50;,
 3;47,8,6;,
 4;53,54,55,56;,
 4;54,57,58,55;,
 4;57,59,60,58;,
 4;59,61,62,60;,
 4;63,64,65,66;,
 4;15,67,63,66;,
 4;68,69,64,63;,
 4;67,18,68,63;,
 4;70,71,69,68;,
 4;18,21,70,68;,
 4;72,73,71,70;,
 4;21,24,72,70;,
 4;74,75,73,72;,
 4;24,27,74,72;,
 4;27,76,77,74;,
 4;78,79,55,80;,
 4;34,81,78,80;,
 4;82,55,79,78;,
 4;81,36,82,78;,
 4;36,83,56,82;,
 4;83,38,53,56;,
 4;84,62,40,39;,
 3;55,82,56;,
 4;62,61,85,40;,
 3;71,73,69;,
 4;65,69,73,86;,
 3;64,69,65;,
 4;87,86,73,75;,
 4;87,84,88,89;,
 4;65,86,87,89;,
 4;90,62,84,87;,
 4;91,58,60,90;,
 4;87,75,91,90;,
 4;92,55,58,91;,
 4;75,77,92,91;,
 4;93,80,55,92;,
 4;94,34,80,93;,
 4;77,76,94,93;,
 4;84,39,52,88;,
 3;75,74,77;,
 3;93,92,77;,
 3;60,62,90;,
 4;95,96,10,15;,
 4;95,15,66,97;,
 4;96,98,11,10;,
 4;97,66,65,99;,
 4;98,100,45,11;,
 4;99,65,89,101;,
 4;100,102,46,45;,
 4;101,89,88,103;,
 4;102,104,52,46;,
 4;103,88,52,104;,
 3;105,5,7;,
 3;105,2,5;,
 3;105,3,2;,
 3;105,38,3;,
 3;105,53,38;,
 3;105,7,9;,
 3;105,9,42;,
 3;105,85,61;,
 3;105,61,59;,
 3;105,59,57;,
 3;105,57,54;,
 3;105,54,53;,
 3;106,102,100;,
 3;106,100,98;,
 3;106,98,96;,
 3;106,96,95;,
 3;106,95,97;,
 3;106,97,99;,
 3;106,99,101;,
 3;106,101,103;,
 3;106,103,104;,
 3;106,104,102;;
 
 MeshMaterialList {
  2;
  116;
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
  157;
  -0.583221;0.812313;0.000058;,
  0.011574;0.999933;-0.000000;,
  0.191216;-0.977483;-0.089241;,
  0.191982;-0.981399;0.000000;,
  0.131855;-0.979598;-0.151663;,
  -0.010183;0.999948;-0.000000;,
  0.575993;-0.817455;-0.000000;,
  0.279056;0.960275;0.000000;,
  0.025144;0.622805;-0.781972;,
  0.510654;-0.859582;0.018748;,
  0.205635;0.735149;-0.645965;,
  0.111624;-0.720323;-0.684599;,
  0.177167;-0.981897;0.067003;,
  -0.015723;0.999876;-0.000000;,
  0.423907;-0.715857;-0.554844;,
  0.008717;-0.999938;0.006950;,
  0.010194;-0.985915;-0.166935;,
  0.334000;0.556224;-0.760960;,
  0.267910;0.234087;-0.934573;,
  0.149597;-0.282344;-0.947577;,
  0.032237;-0.723335;-0.689745;,
  0.154922;0.331871;-0.930516;,
  0.166644;0.535376;-0.828011;,
  -0.044256;-0.268989;-0.962126;,
  -0.174620;-0.676170;-0.715753;,
  -0.308741;-0.635757;-0.707454;,
  0.035068;-0.002566;-0.999382;,
  0.025898;-0.972232;-0.232583;,
  0.272313;0.959042;-0.077995;,
  0.537885;-0.825940;-0.168827;,
  0.016230;0.974434;-0.224087;,
  0.064648;0.970004;-0.234335;,
  0.160210;-0.898186;-0.409383;,
  0.003589;0.974795;-0.223075;,
  0.051138;0.899763;-0.433371;,
  -0.442579;-0.896674;0.009952;,
  -0.237833;-0.971293;0.005034;,
  -0.061905;-0.879498;-0.471859;,
  0.047160;-0.058373;-0.997180;,
  -0.275866;-0.599416;-0.751398;,
  0.041479;-0.953936;-0.297128;,
  0.051473;0.645285;-0.762206;,
  0.137380;0.307856;-0.941462;,
  -0.124180;-0.187742;-0.974337;,
  0.105805;0.450847;-0.886308;,
  0.169534;0.881400;-0.440899;,
  0.061150;0.907801;0.414920;,
  0.069607;0.900015;0.430264;,
  0.191216;-0.977483;0.089241;,
  0.131855;-0.979598;0.151663;,
  0.021542;0.618205;0.785722;,
  0.510654;-0.859582;-0.018748;,
  0.205635;0.735149;0.645965;,
  0.111624;-0.720323;0.684598;,
  0.177167;-0.981897;-0.067003;,
  0.423907;-0.715857;0.554844;,
  0.006975;-0.983527;0.180624;,
  0.334000;0.556224;0.760959;,
  0.267910;0.234087;0.934573;,
  0.149597;-0.282345;0.947577;,
  0.035701;-0.724864;0.687966;,
  0.154922;0.331871;0.930516;,
  0.166099;0.525211;0.834604;,
  -0.044256;-0.268989;0.962126;,
  -0.174474;-0.670892;0.720737;,
  -0.311926;-0.618035;0.721620;,
  0.029929;0.019968;0.999353;,
  0.026883;-0.973334;0.227811;,
  0.272313;0.959042;0.077995;,
  0.537885;-0.825940;0.168827;,
  0.016230;0.974434;0.224087;,
  0.064649;0.970004;0.234335;,
  0.160210;-0.898186;0.409382;,
  0.003589;0.974795;0.223074;,
  0.051138;0.899764;0.433371;,
  -0.413649;-0.910409;-0.007122;,
  -0.055278;-0.881986;0.468022;,
  0.041547;-0.009845;0.999088;,
  -0.288537;-0.594278;0.750720;,
  0.046717;-0.960487;0.274377;,
  0.051473;0.645285;0.762206;,
  0.129836;0.327981;0.935719;,
  -0.116016;-0.191004;0.974709;,
  0.109593;0.446239;0.888178;,
  0.169534;0.881400;0.440899;,
  0.061150;0.907801;-0.414920;,
  0.069607;0.900015;-0.430264;,
  0.575787;0.802502;0.156397;,
  0.584408;0.781719;-0.217678;,
  0.558739;0.789454;0.254112;,
  0.424199;0.514162;-0.745448;,
  0.467995;0.607545;0.641770;,
  0.464353;-0.287708;-0.837616;,
  0.115589;0.004639;0.993286;,
  0.244995;-0.712581;-0.657423;,
  0.249753;-0.736428;0.628727;,
  -0.041953;-0.988452;-0.145612;,
  -0.993282;0.115696;0.002300;,
  0.830613;-0.556409;-0.022133;,
  0.267296;0.963592;0.006598;,
  0.275368;0.947586;-0.162027;,
  -0.033264;0.976475;-0.213049;,
  -0.107708;0.729766;-0.675159;,
  -0.021362;0.999772;-0.000000;,
  0.133398;-0.991063;-0.000000;,
  -0.004261;0.958463;-0.285186;,
  0.011424;0.987046;-0.160030;,
  0.471897;-0.822334;0.317931;,
  0.510744;-0.859733;-0.000000;,
  -0.228989;0.971967;-0.053325;,
  0.389536;-0.709103;0.587737;,
  0.229308;0.339468;-0.912238;,
  0.058316;0.119495;-0.991121;,
  0.182027;0.641325;-0.745365;,
  -0.041658;0.836855;0.545838;,
  0.018757;0.890846;0.453918;,
  0.084393;0.996433;0.000000;,
  0.059857;0.862528;0.502457;,
  0.103468;0.921523;0.374285;,
  0.213644;-0.935226;0.282328;,
  0.267296;0.963592;-0.006598;,
  0.275368;0.947586;0.162027;,
  -0.107708;0.729767;0.675159;,
  -0.033264;0.976475;0.213049;,
  -0.004261;0.958463;0.285185;,
  0.011424;0.987046;0.160030;,
  0.471897;-0.822334;-0.317930;,
  -0.228989;0.971967;0.053324;,
  0.389537;-0.709103;-0.587736;,
  0.141329;0.259230;0.955419;,
  0.058316;0.119495;0.991121;,
  0.182027;0.641326;0.745364;,
  0.018757;0.890846;-0.453918;,
  -0.041658;0.836855;-0.545837;,
  0.103468;0.921523;-0.374285;,
  0.059857;0.862528;-0.502457;,
  0.213644;-0.935226;-0.282328;,
  0.562324;0.789144;0.247070;,
  -0.902514;0.187325;-0.387787;,
  -0.925339;-0.080484;-0.370501;,
  -0.892330;0.305282;-0.332492;,
  -0.886537;0.444793;-0.127319;,
  -0.885279;0.446908;0.128667;,
  -0.915164;-0.311253;-0.256118;,
  -0.913691;-0.356386;-0.195340;,
  -0.912032;-0.359614;0.197169;,
  -0.912999;-0.314654;0.259666;,
  -0.922382;-0.082005;0.377475;,
  -0.899010;0.188307;0.395376;,
  -0.889348;0.308745;0.337249;,
  0.826054;-0.376686;-0.419217;,
  0.904568;-0.293839;-0.308895;,
  0.934609;-0.355667;0.002816;,
  0.877820;-0.377956;0.294245;,
  0.816909;-0.455077;0.354350;,
  0.700250;-0.574470;0.423832;,
  0.573516;-0.744290;0.342216;;
  116;
  4;34,10,17,34;,
  4;10,21,18,17;,
  4;21,23,19,18;,
  4;23,24,20,19;,
  4;27,40,40,27;,
  4;27,27,16,15;,
  4;28,99,41,28;,
  4;28,28,7,100;,
  4;29,14,14,29;,
  4;29,29,6,6;,
  4;101,102,8,30;,
  4;101,30,13,103;,
  4;30,8,22,31;,
  4;30,31,5,13;,
  4;12,12,4,104;,
  4;32,11,11,32;,
  4;32,32,2,3;,
  4;33,105,10,33;,
  4;33,33,1,106;,
  4;107,107,9,108;,
  4;34,34,0,109;,
  4;35,36,24,25;,
  3;107,107,110;,
  4;36,37,20,24;,
  3;41,8,102;,
  4;42,8,41,111;,
  3;111,41,112;,
  4;22,8,42,26;,
  4;38,39,25,26;,
  4;38,26,42,111;,
  4;26,25,24,43;,
  4;43,23,21,44;,
  4;43,44,22,26;,
  4;44,21,10,45;,
  4;44,45,113,22;,
  4;114,114,115,46;,
  4;46,115,116,47;,
  4;46,47,117,118;,
  4;39,75,35,25;,
  3;12,12,119;,
  3;118,45,46;,
  3;43,24,23;,
  4;74,57,52,74;,
  4;57,58,61,52;,
  4;58,59,63,61;,
  4;59,60,64,63;,
  4;67,79,79,67;,
  4;15,56,67,67;,
  4;68,80,120,68;,
  4;121,7,68,68;,
  4;69,55,55,69;,
  4;6,6,69,69;,
  4;70,50,122,123;,
  4;103,13,70,123;,
  4;71,62,50,70;,
  4;13,5,71,70;,
  4;104,49,54,54;,
  4;72,53,53,72;,
  4;3,48,72,72;,
  4;73,52,124,73;,
  4;125,1,73,73;,
  4;108,51,126,126;,
  4;127,0,74,74;,
  4;65,64,36,35;,
  3;128,126,126;,
  4;64,60,76,36;,
  3;122,50,80;,
  4;129,80,50,81;,
  3;130,80,129;,
  4;66,81,50,62;,
  4;66,65,78,77;,
  4;129,81,66,77;,
  4;82,64,65,66;,
  4;83,61,63,82;,
  4;66,62,83,82;,
  4;84,52,61,83;,
  4;62,131,84,83;,
  4;85,132,133,133;,
  4;86,116,132,85;,
  4;134,135,86,85;,
  4;65,35,75,78;,
  3;136,54,54;,
  3;85,84,134;,
  3;63,64,82;,
  4;87,88,88,87;,
  4;87,87,89,89;,
  4;88,90,111,88;,
  4;89,89,137,91;,
  4;90,92,38,111;,
  4;91,129,77,93;,
  4;92,94,39,38;,
  4;93,77,78,95;,
  4;94,96,75,39;,
  4;95,78,75,96;,
  3;97,138,139;,
  3;97,140,138;,
  3;97,141,140;,
  3;97,0,141;,
  3;97,142,0;,
  3;97,139,143;,
  3;97,143,144;,
  3;97,145,146;,
  3;97,146,147;,
  3;97,147,148;,
  3;97,148,149;,
  3;97,149,142;,
  3;98,94,92;,
  3;98,92,150;,
  3;98,150,151;,
  3;98,151,152;,
  3;98,152,153;,
  3;98,153,154;,
  3;98,154,155;,
  3;98,155,95;,
  3;98,95,156;,
  3;98,96,94;;
 }
 MeshTextureCoords {
  107;
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
