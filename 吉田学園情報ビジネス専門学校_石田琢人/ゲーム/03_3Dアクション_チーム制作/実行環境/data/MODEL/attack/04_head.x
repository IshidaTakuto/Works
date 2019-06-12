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
 323;
 2.48220;3.72680;-16.69990;,
 -0.14910;3.72680;-16.69990;,
 -0.18550;9.13290;-8.78570;,
 3.09190;9.13290;-8.78570;,
 -4.16920;13.77600;6.34340;,
 -2.64320;14.08540;6.74450;,
 -4.01170;11.63890;2.53260;,
 -2.67050;11.39110;4.69350;,
 -0.17990;16.27220;9.76010;,
 1.50360;13.74590;6.36510;,
 -1.30480;13.93140;6.50300;,
 -0.40530;12.19050;2.87560;,
 3.05340;12.18420;4.55700;,
 3.71000;10.24450;2.41220;,
 -7.49980;-0.71820;0.87430;,
 -7.45290;-1.38810;-9.83290;,
 -3.93470;-0.71820;3.29140;,
 -4.95460;-1.35170;-16.45630;,
 -3.90040;-1.30900;-22.02410;,
 -0.26110;-0.71820;3.29140;,
 -3.16120;-0.85330;-26.04560;,
 -1.65830;-0.85330;-26.04560;,
 -0.10990;-0.85330;-26.04560;,
 4.35400;-0.71820;3.29140;,
 1.83540;-0.85330;-26.04560;,
 3.16120;-0.85330;-26.04560;,
 3.90040;-1.30900;-22.24810;,
 4.95460;-1.35170;-17.07090;,
 7.49980;-0.71820;1.65480;,
 7.45290;-1.38810;-10.14020;,
 6.45190;7.72030;-4.30710;,
 7.03150;5.31370;-5.99410;,
 6.56390;5.35850;-6.78160;,
 6.47990;5.52160;-6.74870;,
 -6.25660;5.07780;-7.29400;,
 -6.43580;4.49470;-6.95450;,
 -6.41970;6.21600;-5.36130;,
 -2.79440;9.13290;-8.78570;,
 -2.24280;3.72680;-16.69990;,
 -0.08680;2.95330;-25.07540;,
 -1.30830;2.95330;-25.07540;,
 1.44760;2.95330;-25.07540;,
 7.41720;3.70930;-7.11970;,
 6.71370;1.30690;-10.11710;,
 6.87330;3.30400;-9.32470;,
 4.26860;1.24600;-17.00300;,
 6.32870;2.89870;-11.52970;,
 -6.67100;3.12270;-12.64060;,
 -6.49250;3.70580;-12.98080;,
 -5.81490;2.66140;-12.82260;,
 -5.81350;2.48570;-12.89820;,
 6.31470;7.22190;-2.32750;,
 6.85090;8.62820;-0.00210;,
 5.58670;7.24850;-6.39520;,
 -6.85090;8.62820;-0.00210;,
 -6.31470;7.77420;-0.95340;,
 -5.58670;7.56420;-6.54220;,
 -6.45190;7.72030;-4.30710;,
 -0.08120;1.72200;-28.80990;,
 -1.20680;1.76330;-28.80990;,
 1.02340;1.72200;-28.80990;,
 6.60870;0.04480;-11.00330;,
 4.10550;0.13860;-18.62000;,
 -6.71370;1.30690;-10.11710;,
 -4.26860;1.24600;-17.00300;,
 -4.10550;0.13860;-18.62000;,
 -6.60870;0.04480;-11.00330;,
 -4.27560;1.94880;-16.69990;,
 -5.22130;6.22090;-9.62850;,
 -5.32630;7.37380;-9.04470;,
 -2.49340;1.17530;-25.07540;,
 -1.79480;1.40070;-28.80990;,
 2.49340;1.17530;-25.07540;,
 1.79480;1.40070;-28.80990;,
 4.27560;1.94880;-16.69990;,
 5.32630;7.37380;-9.04470;,
 6.56110;-0.76650;-10.46500;,
 4.03270;-0.80640;-17.63720;,
 -4.03270;-0.80640;-17.63720;,
 -6.56110;-0.76650;-10.46500;,
 7.45290;-1.38810;-10.14020;,
 4.95460;-1.35170;-17.07090;,
 -4.95460;-1.35170;-16.45630;,
 -7.45290;-1.38810;-9.83290;,
 5.68680;8.47840;-4.96650;,
 -3.90040;-1.30900;-22.02410;,
 7.49980;-0.71820;1.65480;,
 9.30090;11.49750;9.86720;,
 7.22400;4.51150;-6.55690;,
 6.93980;12.54540;6.59820;,
 3.71000;10.24450;2.41220;,
 -9.46330;10.83740;9.14130;,
 -4.01170;11.63890;2.53260;,
 -6.65420;12.40680;6.27900;,
 -6.40850;3.47200;-8.21940;,
 -6.38330;2.75450;-10.83460;,
 -6.45400;3.10870;-7.86870;,
 -6.65000;3.95430;-4.51710;,
 -7.49980;-0.71820;0.87430;,
 6.43440;4.92800;-8.59670;,
 6.49950;5.40890;-6.87960;,
 6.52120;5.36830;-6.88800;,
 6.73960;4.36450;-8.41890;,
 5.96050;5.06310;-10.11500;,
 5.80790;4.48770;-11.64030;,
 5.80930;5.94370;-7.93520;,
 7.02870;4.46040;-6.96570;,
 6.28250;2.63900;-12.99550;,
 6.16140;3.87730;-10.37470;,
 5.97730;3.20180;-13.17260;,
 6.72280;2.79720;-11.10760;,
 6.87540;3.37260;-9.58230;,
 6.77110;4.38900;-7.81060;,
 6.73190;4.09990;-8.40000;,
 6.43440;4.92800;-8.59670;,
 6.73960;4.36450;-8.41890;,
 5.96050;5.06310;-10.11500;,
 6.87540;3.37260;-9.58230;,
 5.80790;4.48770;-11.64030;,
 6.72280;2.79720;-11.10760;,
 5.97730;3.20180;-13.17260;,
 6.28250;2.63900;-12.99550;,
 -5.87230;5.22060;-9.23790;,
 -5.74700;6.30840;-7.50470;,
 -5.65530;6.05150;-8.31320;,
 -5.84710;4.50310;-11.85310;,
 -6.49250;3.70580;-12.98080;,
 -6.67100;3.12270;-12.64060;,
 -5.84710;4.50310;-11.85310;,
 -6.38330;2.75450;-10.83460;,
 -5.87230;5.22060;-9.23790;,
 -6.40850;3.47200;-8.21940;,
 -6.25660;5.07780;-7.29400;,
 -6.43580;4.49470;-6.95450;,
 2.84200;13.78090;6.55410;,
 2.34430;18.79360;14.62020;,
 4.16990;14.29120;6.08090;,
 4.16990;14.29120;6.08090;,
 2.34430;18.79360;14.62020;,
 4.48980;13.28530;6.93070;,
 -4.34420;13.69550;7.12670;,
 -1.32230;19.27100;14.62020;,
 -4.16920;13.77600;6.34340;,
 6.85090;8.62820;-0.00210;,
 -0.60620;9.71950;0.67340;,
 6.31470;7.22190;-2.32750;,
 -6.85090;8.62820;-0.00210;,
 -1.32230;19.27100;14.62020;,
 0.83370;4.22170;4.66060;,
 2.18190;5.25210;5.09880;,
 1.85640;5.25210;5.73860;,
 0.70910;4.22170;4.90490;,
 1.34820;5.25210;6.24610;,
 0.51520;4.22170;5.09880;,
 0.70910;5.25210;6.57160;,
 0.27090;4.22170;5.22340;,
 0.00000;5.25210;6.68430;,
 0.00000;4.22170;5.26610;,
 -0.70910;5.25210;6.57160;,
 -0.27090;4.22170;5.22340;,
 -1.34820;5.25210;6.24610;,
 -0.51520;4.22170;5.09880;,
 -1.85640;5.25210;5.73860;,
 -0.70910;4.22170;4.90490;,
 -2.18190;5.25210;5.09880;,
 -0.83370;4.22170;4.66060;,
 -2.29390;5.25210;4.38970;,
 -0.87640;4.22170;4.38970;,
 -2.18190;5.25210;3.68130;,
 -0.83370;4.22170;4.11880;,
 -1.85640;5.25210;3.04150;,
 -0.70910;4.22170;3.87450;,
 -1.34820;5.25210;2.53400;,
 -0.51520;4.22170;3.68130;,
 -0.70910;5.25210;2.20780;,
 -0.27090;4.22170;3.55670;,
 0.00000;5.25210;2.09580;,
 0.00000;4.22170;3.51330;,
 0.70910;5.25210;2.20780;,
 0.27090;4.22170;3.55670;,
 1.34820;5.25210;2.53400;,
 0.51520;4.22170;3.68130;,
 1.85640;5.25210;3.04150;,
 0.70910;4.22170;3.87450;,
 2.18190;5.25210;3.68130;,
 0.83370;4.22170;4.11880;,
 2.29390;5.25210;4.38970;,
 0.87640;4.22170;4.38970;,
 2.18190;5.25210;5.09880;,
 0.83370;4.22170;4.66060;,
 2.69710;6.91880;5.26610;,
 2.29390;6.91880;6.05710;,
 1.66670;6.91880;6.68430;,
 0.87640;6.91880;7.08680;,
 0.00000;6.91880;7.22610;,
 -0.87640;6.91880;7.08680;,
 -1.66670;6.91880;6.68430;,
 -2.29390;6.91880;6.05710;,
 -2.69710;6.91880;5.26610;,
 -2.83570;6.91880;4.38970;,
 -2.69710;6.91880;3.51330;,
 -2.29390;6.91880;2.72300;,
 -1.66670;6.91880;2.09580;,
 -0.87640;6.91880;1.69260;,
 0.00000;6.91880;1.55400;,
 0.87640;6.91880;1.69260;,
 1.66670;6.91880;2.09580;,
 2.29390;6.91880;2.72300;,
 2.69710;6.91880;3.51330;,
 2.83570;6.91880;4.38970;,
 2.69710;6.91880;5.26610;,
 2.18190;8.58550;5.09880;,
 1.85640;8.58550;5.73860;,
 1.34820;8.58550;6.24610;,
 0.70910;8.58550;6.57160;,
 0.00000;8.58550;6.68430;,
 -0.70910;8.58550;6.57160;,
 -1.34820;8.58550;6.24610;,
 -1.85640;8.58550;5.73860;,
 -2.18190;8.58550;5.09880;,
 -2.29390;8.58550;4.38970;,
 -2.18190;8.58550;3.68130;,
 -1.85640;8.58550;3.04150;,
 -1.34820;8.58550;2.53400;,
 -0.70910;8.58550;2.20780;,
 0.00000;8.58550;2.09580;,
 0.70910;8.58550;2.20780;,
 1.34820;8.58550;2.53400;,
 1.85640;8.58550;3.04150;,
 2.18190;8.58550;3.68130;,
 2.29390;8.58550;4.38970;,
 2.18190;8.58550;5.09880;,
 0.00000;4.08310;4.38970;,
 0.00000;4.08310;4.38970;,
 0.00000;4.08310;4.38970;,
 0.00000;4.08310;4.38970;,
 0.00000;4.08310;4.38970;,
 0.00000;4.08310;4.38970;,
 0.00000;4.08310;4.38970;,
 0.00000;4.08310;4.38970;,
 0.00000;4.08310;4.38970;,
 0.00000;4.08310;4.38970;,
 0.00000;4.08310;4.38970;,
 0.00000;4.08310;4.38970;,
 0.00000;4.08310;4.38970;,
 0.00000;4.08310;4.38970;,
 0.00000;4.08310;4.38970;,
 0.00000;4.08310;4.38970;,
 0.00000;4.08310;4.38970;,
 0.00000;4.08310;4.38970;,
 0.00000;4.08310;4.38970;,
 0.00000;4.08310;4.38970;,
 0.00000;9.75450;4.38970;,
 0.00000;9.75450;4.38970;,
 0.00000;9.75450;4.38970;,
 0.00000;9.75450;4.38970;,
 0.00000;9.75450;4.38970;,
 0.00000;9.75450;4.38970;,
 0.00000;9.75450;4.38970;,
 0.00000;9.75450;4.38970;,
 0.00000;9.75450;4.38970;,
 0.00000;9.75450;4.38970;,
 0.00000;9.75450;4.38970;,
 0.00000;9.75450;4.38970;,
 0.00000;9.75450;4.38970;,
 0.00000;9.75450;4.38970;,
 0.00000;9.75450;4.38970;,
 0.00000;9.75450;4.38970;,
 0.00000;9.75450;4.38970;,
 0.00000;9.75450;4.38970;,
 0.00000;9.75450;4.38970;,
 0.00000;9.75450;4.38970;,
 0.18620;4.24060;7.29120;,
 -2.92950;0.37940;-15.47350;,
 -1.13610;-6.06970;-15.69960;,
 -0.87920;0.38500;-12.99410;,
 -1.49100;0.40250;-18.35120;,
 -2.92950;0.37940;-15.47350;,
 -4.37080;0.29400;-10.44680;,
 -2.57810;-6.15580;-10.67220;,
 -2.32120;0.29890;-7.96740;,
 -2.93300;0.31640;-13.32380;,
 -4.37080;0.29400;-10.44680;,
 -3.77020;0.37940;-12.16110;,
 -1.97750;-6.06970;-12.38650;,
 -1.72060;0.38500;-9.68170;,
 -2.33240;0.40250;-15.03810;,
 -3.77020;0.37940;-12.16110;,
 -3.38240;0.37940;-13.95660;,
 -1.58970;-6.06970;-14.18270;,
 -1.33280;0.38500;-11.47720;,
 -1.94460;0.40250;-16.83430;,
 -3.38240;0.37940;-13.95660;,
 3.06810;-0.21560;-16.12800;,
 1.37130;-5.41100;-15.56730;,
 0.60550;-0.22050;-18.13700;,
 2.17910;-0.18900;-13.07740;,
 3.06810;-0.21560;-16.12800;,
 3.55950;0.29680;-14.64890;,
 1.85710;-6.15580;-14.07910;,
 1.08360;0.29120;-16.70200;,
 2.68450;0.32410;-11.55420;,
 3.55950;0.29680;-14.64890;,
 3.76600;0.38290;-12.88560;,
 2.06360;-6.06970;-12.31510;,
 1.28940;0.37730;-14.93800;,
 2.89030;0.41020;-9.79020;,
 3.76600;0.38290;-12.88560;,
 4.09080;0.38290;-11.07820;,
 2.38840;-6.06970;-10.50770;,
 1.61490;0.37730;-13.13060;,
 3.21580;0.41020;-7.98280;,
 4.09080;0.38290;-11.07820;,
 -2.93090;-0.33880;-17.69810;,
 -1.13470;-7.20440;-17.92630;,
 -0.88130;-0.33320;-15.21940;,
 -1.49310;-0.31570;-20.57580;,
 -2.93090;-0.33880;-17.69810;,
 2.76850;-0.33670;-17.63720;,
 0.95900;-7.20300;-17.63020;,
 1.54560;-0.46620;-20.60940;,
 0.54180;-0.19670;-15.31950;,
 2.76850;-0.33670;-17.63720;;
 
 299;
 4;0,1,2,3;,
 3;4,5,6;,
 3;5,7,6;,
 3;8,9,10;,
 3;6,7,11;,
 3;10,9,7;,
 3;7,9,11;,
 3;9,12,11;,
 3;11,12,13;,
 3;14,15,16;,
 3;15,17,16;,
 3;17,18,16;,
 3;16,18,19;,
 3;18,20,19;,
 3;20,21,19;,
 3;21,22,19;,
 3;19,22,23;,
 3;22,24,23;,
 3;24,25,23;,
 3;25,26,23;,
 3;26,27,23;,
 3;23,27,28;,
 3;28,27,29;,
 4;30,31,32,33;,
 3;34,35,36;,
 4;37,2,1,38;,
 4;38,1,39,40;,
 4;41,39,1,0;,
 3;42,43,44;,
 3;43,45,44;,
 3;44,45,46;,
 4;47,48,49,50;,
 4;51,52,30,53;,
 4;54,55,56,57;,
 4;40,39,58,59;,
 4;58,39,41,60;,
 4;45,43,61,62;,
 4;63,64,65,66;,
 3;67,68,38;,
 3;38,68,37;,
 3;37,68,69;,
 4;70,67,38,40;,
 4;71,70,40,59;,
 4;72,73,60,41;,
 4;74,72,41,0;,
 4;75,74,0,3;,
 4;20,71,59,21;,
 4;58,22,21,59;,
 4;22,58,60,24;,
 4;73,25,24,60;,
 4;62,61,76,77;,
 4;66,65,78,79;,
 4;77,76,80,81;,
 4;79,78,82,83;,
 4;75,84,51,53;,
 4;72,74,46,45;,
 4;73,72,45,62;,
 4;25,73,62,77;,
 4;26,25,77,81;,
 4;78,20,85,82;,
 4;65,71,20,78;,
 4;64,70,71,65;,
 3;76,86,80;,
 3;61,86,76;,
 3;43,86,61;,
 4;42,87,86,43;,
 3;87,31,30;,
 3;87,88,31;,
 3;88,87,42;,
 3;89,87,90;,
 3;87,52,90;,
 3;52,87,30;,
 3;57,91,54;,
 3;54,91,92;,
 3;92,91,93;,
 4;94,95,96,97;,
 3;98,63,66;,
 3;98,66,79;,
 3;98,79,83;,
 4;99,100,101,102;,
 3;53,30,33;,
 3;103,104,105;,
 4;31,88,106,32;,
 3;88,42,106;,
 3;107,108,109;,
 4;110,111,112,113;,
 4;33,32,101,100;,
 3;53,33,105;,
 3;33,100,105;,
 3;100,99,105;,
 3;105,99,103;,
 3;53,105,75;,
 3;105,104,75;,
 3;75,104,74;,
 3;108,46,109;,
 3;104,109,74;,
 3;74,109,46;,
 3;107,110,108;,
 3;110,113,108;,
 3;46,108,44;,
 3;44,108,113;,
 3;106,42,112;,
 3;42,44,112;,
 3;112,44,113;,
 3;32,106,101;,
 3;101,112,102;,
 3;102,112,111;,
 3;112,101,106;,
 3;114,115,116;,
 3;115,117,116;,
 3;116,117,118;,
 3;117,119,118;,
 3;118,119,120;,
 3;120,119,121;,
 3;70,64,67;,
 3;64,50,67;,
 3;67,50,49;,
 3;63,98,96;,
 3;96,98,97;,
 3;97,98,91;,
 3;64,63,50;,
 3;50,63,47;,
 3;47,63,95;,
 3;95,63,96;,
 3;94,97,35;,
 3;35,97,36;,
 3;36,97,57;,
 3;57,97,91;,
 4;122,123,124,125;,
 3;126,127,128;,
 3;127,129,128;,
 3;128,129,130;,
 3;129,131,130;,
 3;130,131,132;,
 3;132,131,133;,
 3;67,125,68;,
 3;68,125,124;,
 3;125,67,48;,
 3;48,67,49;,
 3;68,124,69;,
 3;69,124,56;,
 3;56,124,123;,
 3;122,34,123;,
 3;34,36,123;,
 3;123,36,56;,
 3;56,36,57;,
 3;134,135,136;,
 3;137,138,139;,
 3;140,141,142;,
 4;12,134,136,13;,
 4;90,137,139,89;,
 4;93,140,142,92;,
 4;11,13,143,144;,
 3;144,143,2;,
 3;2,143,3;,
 3;3,84,75;,
 3;3,143,84;,
 3;84,143,145;,
 4;146,6,11,144;,
 3;69,56,37;,
 3;56,55,37;,
 3;55,146,37;,
 3;146,144,37;,
 3;37,144,2;,
 3;4,147,5;,
 4;148,149,150,151;,
 4;151,150,152,153;,
 4;153,152,154,155;,
 4;155,154,156,157;,
 4;157,156,158,159;,
 4;159,158,160,161;,
 4;161,160,162,163;,
 4;163,162,164,165;,
 4;165,164,166,167;,
 4;167,166,168,169;,
 4;169,168,170,171;,
 4;171,170,172,173;,
 4;173,172,174,175;,
 4;175,174,176,177;,
 4;177,176,178,179;,
 4;179,178,180,181;,
 4;181,180,182,183;,
 4;183,182,184,185;,
 4;185,184,186,187;,
 4;187,186,188,189;,
 4;149,190,191,150;,
 4;150,191,192,152;,
 4;152,192,193,154;,
 4;154,193,194,156;,
 4;156,194,195,158;,
 4;158,195,196,160;,
 4;160,196,197,162;,
 4;162,197,198,164;,
 4;164,198,199,166;,
 4;166,199,200,168;,
 4;168,200,201,170;,
 4;170,201,202,172;,
 4;172,202,203,174;,
 4;174,203,204,176;,
 4;176,204,205,178;,
 4;178,205,206,180;,
 4;180,206,207,182;,
 4;182,207,208,184;,
 4;184,208,209,186;,
 4;186,209,210,188;,
 4;190,211,212,191;,
 4;191,212,213,192;,
 4;192,213,214,193;,
 4;193,214,215,194;,
 4;194,215,216,195;,
 4;195,216,217,196;,
 4;196,217,218,197;,
 4;197,218,219,198;,
 4;198,219,220,199;,
 4;199,220,221,200;,
 4;200,221,222,201;,
 4;201,222,223,202;,
 4;202,223,224,203;,
 4;203,224,225,204;,
 4;204,225,226,205;,
 4;205,226,227,206;,
 4;206,227,228,207;,
 4;207,228,229,208;,
 4;208,229,230,209;,
 4;209,230,231,210;,
 3;151,232,148;,
 3;153,233,151;,
 3;155,234,153;,
 3;157,235,155;,
 3;159,236,157;,
 3;161,237,159;,
 3;163,238,161;,
 3;165,239,163;,
 3;167,240,165;,
 3;169,241,167;,
 3;171,242,169;,
 3;173,243,171;,
 3;175,244,173;,
 3;177,245,175;,
 3;179,246,177;,
 3;181,247,179;,
 3;183,248,181;,
 3;185,249,183;,
 3;187,250,185;,
 3;189,251,187;,
 3;211,252,212;,
 3;212,253,213;,
 3;213,254,214;,
 3;214,255,215;,
 3;215,256,216;,
 3;216,257,217;,
 3;217,258,218;,
 3;218,259,219;,
 3;219,260,220;,
 3;220,261,221;,
 3;221,262,222;,
 3;222,263,223;,
 3;223,264,224;,
 3;224,265,225;,
 3;225,266,226;,
 3;226,267,227;,
 3;227,268,228;,
 3;228,269,229;,
 3;229,270,230;,
 3;230,271,231;,
 3;91,98,272;,
 4;91,87,89,93;,
 4;87,91,272,86;,
 3;98,86,272;,
 3;273,274,275;,
 3;275,274,276;,
 3;276,274,277;,
 3;278,279,280;,
 3;280,279,281;,
 3;281,279,282;,
 3;283,284,285;,
 3;285,284,286;,
 3;286,284,287;,
 3;288,289,290;,
 3;290,289,291;,
 3;291,289,292;,
 3;293,294,295;,
 3;295,294,296;,
 3;296,294,297;,
 3;298,299,300;,
 3;300,299,301;,
 3;301,299,302;,
 3;303,304,305;,
 3;305,304,306;,
 3;306,304,307;,
 3;308,309,310;,
 3;310,309,311;,
 3;311,309,312;,
 3;313,314,315;,
 3;315,314,316;,
 3;316,314,317;,
 3;318,319,320;,
 3;320,319,321;,
 3;321,319,322;;
 
 MeshMaterialList {
  1;
  299;
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
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
  272;
  0.184988;-0.982731;0.004318;,
  -0.411029;-0.911181;0.028378;,
  0.838991;0.500846;-0.212714;,
  -0.463748;0.803261;-0.373777;,
  -0.459258;0.741499;-0.489144;,
  0.077939;-0.996958;0.000680;,
  0.024435;0.807783;-0.588973;,
  -0.731544;0.533332;-0.424736;,
  0.000000;0.940842;-0.338845;,
  0.730769;0.542274;-0.414627;,
  -0.644416;0.642184;-0.415124;,
  0.864084;0.366410;-0.345113;,
  0.843427;0.457778;-0.281196;,
  0.029174;0.968308;-0.248048;,
  -0.431259;0.703829;-0.564482;,
  -0.862510;0.465173;-0.199227;,
  0.004381;0.979249;-0.202612;,
  0.852096;0.468286;-0.233756;,
  -0.065819;-0.965068;0.253599;,
  -0.118943;-0.843899;0.523151;,
  0.052116;-0.998056;0.034166;,
  0.011560;0.703372;-0.710727;,
  -0.330144;0.875345;-0.353238;,
  -0.418028;0.820908;-0.389053;,
  -0.383189;0.898947;-0.212276;,
  0.387553;0.893602;-0.226448;,
  0.366524;0.851628;-0.374685;,
  0.330504;0.893977;-0.302608;,
  -0.045087;0.831907;-0.553081;,
  -0.133913;-0.990843;0.017246;,
  -0.899436;0.398989;-0.178390;,
  -0.358736;0.891765;-0.275795;,
  0.008708;0.950459;-0.310728;,
  0.342407;0.891880;-0.295478;,
  0.891427;0.409921;-0.193188;,
  0.214065;-0.948976;-0.231563;,
  0.055066;-0.935475;-0.349076;,
  -0.002879;-0.956976;-0.290152;,
  -0.041761;-0.936965;-0.346919;,
  -0.287808;-0.932995;-0.216070;,
  0.519971;0.853467;-0.034989;,
  -0.843118;0.532387;0.075604;,
  0.970287;-0.053733;-0.235914;,
  0.967189;0.004846;-0.254013;,
  0.966037;0.091524;-0.241654;,
  0.962065;0.182450;-0.202839;,
  -0.107488;-0.993476;0.038110;,
  0.125068;-0.991824;0.025358;,
  -0.966511;-0.117222;-0.228288;,
  -0.966037;0.091524;-0.241654;,
  -0.945099;0.153006;-0.288752;,
  0.022591;-0.998735;0.044922;,
  -0.171197;-0.983613;0.056545;,
  0.956592;-0.217215;-0.194290;,
  0.978386;-0.060178;-0.197837;,
  0.977035;0.027146;-0.211345;,
  0.910618;0.303242;-0.280747;,
  0.802808;0.459287;-0.380203;,
  -0.170586;0.984392;-0.043280;,
  -0.012085;0.954219;-0.298865;,
  0.344600;0.931671;-0.115070;,
  -0.985838;0.030322;-0.164934;,
  -0.977887;-0.060214;-0.200280;,
  -0.955710;-0.218821;-0.196814;,
  0.802207;0.486319;-0.346350;,
  0.936830;0.204647;-0.283672;,
  0.796854;0.486703;-0.357972;,
  0.820539;0.446874;-0.356398;,
  0.802748;0.486003;-0.345537;,
  0.820423;0.447241;-0.356204;,
  0.802771;0.549699;-0.231062;,
  0.825612;0.512151;-0.236784;,
  0.715426;0.594469;-0.367114;,
  -0.887911;-0.430419;0.162340;,
  0.921273;0.357138;-0.153975;,
  -0.604459;-0.709182;0.362893;,
  0.764625;0.584244;-0.272043;,
  0.830476;0.520388;-0.198759;,
  0.866181;0.448622;-0.220157;,
  -0.816063;-0.531265;0.227592;,
  -0.684833;-0.651816;0.325790;,
  -0.842394;-0.501210;0.197892;,
  -0.722135;-0.625589;0.295227;,
  -0.632722;0.572427;-0.521527;,
  -0.991715;0.055567;-0.115818;,
  -0.760830;-0.152325;-0.630820;,
  -0.809761;-0.084889;-0.580587;,
  -0.212609;0.490092;-0.845344;,
  -0.790268;0.589969;-0.165568;,
  -0.967130;0.252480;0.030210;,
  -0.459432;-0.352260;-0.815374;,
  -0.954944;0.203554;-0.215979;,
  -0.989068;0.137100;-0.054286;,
  -0.983183;0.129865;-0.128395;,
  -0.981446;0.155499;-0.112177;,
  -0.914723;0.342359;-0.214643;,
  -0.947896;0.252060;-0.194832;,
  -0.945045;0.297881;-0.134750;,
  -0.930612;0.305302;-0.201870;,
  -0.036847;0.854163;-0.518699;,
  -0.646796;0.739817;-0.185271;,
  0.855046;0.459714;-0.239917;,
  -0.424131;0.706756;-0.566223;,
  -0.361220;0.852308;-0.378273;,
  0.052821;0.790438;-0.610261;,
  0.341688;0.698432;-0.628842;,
  -0.375354;0.829631;-0.413306;,
  0.457358;0.521673;-0.720195;,
  -0.655905;0.607075;-0.448607;,
  0.779576;0.264075;-0.567914;,
  0.009686;0.896671;-0.442591;,
  0.019713;0.946320;-0.322630;,
  0.363523;-0.924083;0.117988;,
  0.309143;-0.924097;0.224667;,
  0.224620;-0.924090;0.309198;,
  0.118086;-0.924089;0.363476;,
  0.000000;-0.924077;0.382207;,
  -0.118086;-0.924089;0.363476;,
  -0.224620;-0.924090;0.309198;,
  -0.309143;-0.924097;0.224667;,
  -0.363523;-0.924083;0.117988;,
  -0.382268;-0.924051;-0.000012;,
  -0.363533;-0.924077;-0.118003;,
  -0.309022;-0.924095;-0.224841;,
  -0.224545;-0.924062;-0.309335;,
  -0.118362;-0.924086;-0.363395;,
  0.000000;-0.924114;-0.382116;,
  0.118362;-0.924086;-0.363395;,
  0.224545;-0.924062;-0.309335;,
  0.309022;-0.924095;-0.224841;,
  0.363533;-0.924077;-0.118003;,
  0.382268;-0.924051;-0.000012;,
  0.768917;-0.588624;0.249578;,
  0.653966;-0.588644;0.475212;,
  0.475020;-0.588659;0.654093;,
  0.249891;-0.588660;0.768787;,
  0.000000;-0.588691;0.808358;,
  -0.249891;-0.588660;0.768787;,
  -0.475020;-0.588659;0.654093;,
  -0.653966;-0.588644;0.475212;,
  -0.768917;-0.588624;0.249578;,
  -0.808411;-0.588618;-0.000031;,
  -0.768886;-0.588635;-0.249647;,
  -0.653855;-0.588647;-0.475362;,
  -0.475134;-0.588634;-0.654031;,
  -0.250034;-0.588735;-0.768683;,
  0.000000;-0.588732;-0.808328;,
  0.250034;-0.588735;-0.768683;,
  0.475134;-0.588635;-0.654031;,
  0.653855;-0.588647;-0.475362;,
  0.768886;-0.588634;-0.249647;,
  0.808411;-0.588618;-0.000031;,
  0.951140;-0.000000;0.308759;,
  0.808983;-0.000000;0.587832;,
  0.587577;0.000000;0.809168;,
  0.309192;0.000000;0.951000;,
  0.000000;0.000000;1.000000;,
  -0.309192;0.000000;0.951000;,
  -0.587577;0.000000;0.809168;,
  -0.808983;-0.000000;0.587832;,
  -0.951140;-0.000000;0.308759;,
  -1.000000;-0.000000;-0.000037;,
  -0.951101;-0.000000;-0.308879;,
  -0.808929;-0.000000;-0.587906;,
  -0.587895;-0.000002;-0.808937;,
  -0.309152;-0.000002;-0.951013;,
  0.000000;-0.000000;-1.000000;,
  0.309152;-0.000002;-0.951013;,
  0.587895;-0.000002;-0.808937;,
  0.808929;-0.000000;-0.587906;,
  0.951101;-0.000000;-0.308879;,
  1.000000;-0.000000;-0.000037;,
  0.722167;0.650807;0.234362;,
  0.614208;0.650821;0.446296;,
  0.446072;0.650819;0.614373;,
  0.234747;0.650827;0.722024;,
  0.000000;0.650889;0.759172;,
  -0.234747;0.650827;0.722024;,
  -0.446072;0.650819;0.614373;,
  -0.614208;0.650821;0.446296;,
  -0.722167;0.650807;0.234362;,
  -0.759232;0.650820;-0.000038;,
  -0.722134;0.650814;-0.234441;,
  -0.614191;0.650804;-0.446344;,
  -0.446350;0.650766;-0.614227;,
  -0.234708;0.650855;-0.722011;,
  0.000000;0.650814;-0.759237;,
  0.234708;0.650855;-0.722011;,
  0.446350;0.650766;-0.614227;,
  0.614191;0.650804;-0.446344;,
  0.722134;0.650814;-0.234441;,
  0.759232;0.650820;-0.000038;,
  0.000000;-1.000000;-0.000034;,
  0.000000;1.000000;-0.000015;,
  -0.064297;-0.618045;0.783509;,
  -0.749684;-0.230229;0.620459;,
  0.993513;0.008005;-0.113437;,
  -0.749844;-0.230095;0.620315;,
  0.993511;0.007999;-0.113451;,
  -0.749809;-0.230111;0.620351;,
  0.993511;0.007999;-0.113451;,
  -0.749797;-0.230175;0.620341;,
  0.993513;0.008005;-0.113437;,
  0.607427;-0.278666;-0.743894;,
  -0.954876;0.006127;0.296940;,
  0.621500;-0.230100;-0.748860;,
  -0.954883;0.006232;0.296917;,
  0.621266;-0.230136;-0.749044;,
  -0.954883;0.006129;0.296917;,
  0.621367;-0.230154;-0.748954;,
  -0.954883;0.006232;0.296917;,
  -0.751983;-0.217420;0.622295;,
  0.993511;0.008055;-0.113451;,
  0.901808;-0.238024;-0.360675;,
  -0.982435;0.003034;-0.186579;,
  0.814358;0.346283;-0.465735;,
  -0.310971;0.949938;0.030238;,
  -0.675263;0.736719;-0.035561;,
  0.218256;-0.751024;-0.623159;,
  0.103169;-0.745344;-0.658649;,
  -0.006709;-0.730271;-0.683124;,
  -0.078245;-0.751896;-0.654622;,
  -0.155922;-0.766656;-0.622838;,
  0.600526;0.770357;-0.214287;,
  0.593931;0.793643;-0.131822;,
  0.647670;0.745659;-0.156580;,
  -0.648023;0.746046;-0.153234;,
  -0.709579;0.682254;-0.176146;,
  0.982356;0.041850;-0.182276;,
  0.910428;0.336260;-0.240935;,
  0.893800;0.430537;-0.125534;,
  0.690822;0.716365;-0.097907;,
  -0.904147;0.408366;-0.125522;,
  -0.725018;0.681441;-0.099937;,
  0.967417;0.241839;-0.074961;,
  0.991129;-0.108797;-0.076331;,
  0.192588;0.857510;-0.477060;,
  -0.257391;0.958088;0.125771;,
  -0.982988;0.159637;-0.090830;,
  0.963464;0.249155;-0.098275;,
  0.866592;0.435578;-0.243494;,
  0.903584;0.301362;-0.304494;,
  0.863541;-0.429670;-0.263973;,
  0.822807;0.486842;-0.293212;,
  0.883611;0.414174;-0.218385;,
  0.913931;0.355241;-0.196300;,
  0.883280;0.402759;-0.240002;,
  0.883046;0.403421;-0.239751;,
  -0.982158;-0.034650;-0.184839;,
  -0.901019;0.373938;0.219851;,
  -0.963401;0.120841;-0.239281;,
  -0.887777;0.402270;0.223678;,
  -0.803542;-0.554226;-0.217149;,
  -0.969479;0.124311;-0.211323;,
  -0.857392;0.471272;-0.206835;,
  0.944901;0.325965;0.030142;,
  0.973526;0.006618;-0.228481;,
  -0.548617;0.829750;-0.102641;,
  -0.901544;-0.096105;-0.421879;,
  -0.079098;-0.528770;0.845071;,
  -0.032948;0.924117;0.380687;,
  0.102404;-0.444511;0.889901;,
  -0.870445;-0.226739;-0.436937;,
  -0.870520;-0.226693;-0.436813;,
  -0.870494;-0.226709;-0.436856;,
  -0.870543;-0.226678;-0.436774;,
  0.923169;-0.272214;0.271402;,
  0.937365;-0.223729;0.267006;,
  0.937336;-0.223679;0.267149;,
  0.937385;-0.223710;0.266951;,
  -0.873169;-0.213884;-0.437983;,
  0.703134;-0.184599;0.686677;;
  299;
  4;26,8,13,27;,
  3;103,99,106;,
  3;215,107,215;,
  3;6,28,21;,
  3;106,216,110;,
  3;21,28,107;,
  3;216,28,110;,
  3;28,104,110;,
  3;110,104,105;,
  3;19,51,20;,
  3;51,47,20;,
  3;47,0,20;,
  3;20,0,5;,
  3;0,35,5;,
  3;35,36,5;,
  3;36,37,5;,
  3;5,37,29;,
  3;37,38,29;,
  3;38,39,29;,
  3;39,1,29;,
  3;1,46,29;,
  3;29,46,18;,
  3;18,46,52;,
  4;57,67,66,64;,
  3;96,93,91;,
  4;22,13,8,23;,
  4;23,8,16,24;,
  4;25,16,8,26;,
  3;56,55,65;,
  3;55,43,65;,
  3;65,43,42;,
  4;90,87,85,86;,
  4;41,58,217,41;,
  4;59,40,40,60;,
  4;24,16,32,31;,
  4;32,16,25,33;,
  4;43,55,54,44;,
  4;61,50,49,62;,
  3;7,83,23;,
  3;23,83,22;,
  3;22,83,14;,
  4;15,7,23,24;,
  4;30,15,24,31;,
  4;17,34,33,25;,
  4;9,17,25,26;,
  4;12,9,26,27;,
  4;35,218,219,36;,
  4;220,37,36,219;,
  4;37,220,221,38;,
  4;222,39,38,221;,
  4;44,54,53,45;,
  4;62,49,48,63;,
  4;223,224,224,225;,
  4;226,227,227,226;,
  4;12,2,228,229;,
  4;17,9,42,43;,
  4;34,17,43,44;,
  4;230,34,44,45;,
  4;231,230,45,225;,
  4;227,232,233,227;,
  4;49,30,232,48;,
  4;50,15,30,49;,
  3;224,234,224;,
  3;54,235,53;,
  3;55,234,54;,
  4;56,11,234,55;,
  3;11,67,57;,
  3;11,69,67;,
  3;69,11,56;,
  3;109,11,105;,
  3;11,236,105;,
  3;58,237,217;,
  3;60,10,59;,
  3;59,10,106;,
  3;106,10,108;,
  4;94,89,92,84;,
  3;238,61,62;,
  3;238,62,63;,
  3;238,226,226;,
  4;77,71,78,74;,
  3;229,57,64;,
  3;76,72,70;,
  4;67,69,68,66;,
  3;69,56,68;,
  3;81,79,73;,
  4;82,75,75,80;,
  4;64,66,78,71;,
  3;229,64,70;,
  3;64,71,70;,
  3;71,77,70;,
  3;70,77,76;,
  3;229,70,12;,
  3;70,239,12;,
  3;12,72,9;,
  3;240,42,241;,
  3;72,241,9;,
  3;242,241,42;,
  3;81,82,79;,
  3;82,80,79;,
  3;42,240,65;,
  3;65,240,243;,
  3;68,56,244;,
  3;56,65,244;,
  3;244,65,243;,
  3;66,68,78;,
  3;78,244,74;,
  3;74,244,245;,
  3;244,78,68;,
  3;77,74,76;,
  3;74,245,76;,
  3;76,245,72;,
  3;245,246,72;,
  3;72,246,241;,
  3;241,246,247;,
  3;15,50,7;,
  3;50,86,7;,
  3;7,86,85;,
  3;61,238,92;,
  3;92,238,84;,
  3;84,238,248;,
  3;50,61,86;,
  3;86,61,90;,
  3;249,61,89;,
  3;89,61,92;,
  3;94,84,93;,
  3;93,84,91;,
  3;91,84,250;,
  3;250,84,10;,
  4;97,95,98,88;,
  3;251,249,88;,
  3;249,89,88;,
  3;88,89,97;,
  3;89,94,97;,
  3;97,94,96;,
  3;96,94,93;,
  3;7,88,83;,
  3;83,88,98;,
  3;88,7,87;,
  3;252,252,85;,
  3;83,98,253;,
  3;253,98,254;,
  3;254,98,95;,
  3;97,96,95;,
  3;96,91,95;,
  3;95,91,254;,
  3;254,91,250;,
  3;102,4,102;,
  3;101,255,101;,
  3;100,3,103;,
  4;104,102,102,105;,
  4;105,101,101,109;,
  4;108,100,103,106;,
  4;110,105,236,111;,
  3;111,58,13;,
  3;13,58,27;,
  3;27,2,12;,
  3;27,58,2;,
  3;2,256,228;,
  4;59,106,110,111;,
  3;14,254,22;,
  3;254,257,22;,
  3;258,258,258;,
  3;59,111,22;,
  3;22,111,13;,
  3;103,3,99;,
  4;112,132,133,113;,
  4;113,133,134,114;,
  4;114,134,135,115;,
  4;115,135,136,116;,
  4;116,136,137,117;,
  4;117,137,138,118;,
  4;118,138,139,119;,
  4;119,139,140,120;,
  4;120,140,141,121;,
  4;121,141,142,122;,
  4;122,142,143,123;,
  4;123,143,144,124;,
  4;124,144,145,125;,
  4;125,145,146,126;,
  4;126,146,147,127;,
  4;127,147,148,128;,
  4;128,148,149,129;,
  4;129,149,150,130;,
  4;130,150,151,131;,
  4;131,151,132,112;,
  4;132,152,153,133;,
  4;133,153,154,134;,
  4;134,154,155,135;,
  4;135,155,156,136;,
  4;136,156,157,137;,
  4;137,157,158,138;,
  4;138,158,159,139;,
  4;139,159,160,140;,
  4;140,160,161,141;,
  4;141,161,162,142;,
  4;142,162,163,143;,
  4;143,163,164,144;,
  4;144,164,165,145;,
  4;145,165,166,146;,
  4;146,166,167,147;,
  4;147,167,168,148;,
  4;148,168,169,149;,
  4;149,169,170,150;,
  4;150,170,171,151;,
  4;151,171,152,132;,
  4;152,172,173,153;,
  4;153,173,174,154;,
  4;154,174,175,155;,
  4;155,175,176,156;,
  4;156,176,177,157;,
  4;157,177,178,158;,
  4;158,178,179,159;,
  4;159,179,180,160;,
  4;160,180,181,161;,
  4;161,181,182,162;,
  4;162,182,183,163;,
  4;163,183,184,164;,
  4;164,184,185,165;,
  4;165,185,186,166;,
  4;166,186,187,167;,
  4;167,187,188,168;,
  4;168,188,189,169;,
  4;169,189,190,170;,
  4;170,190,191,171;,
  4;171,191,172,152;,
  3;113,192,112;,
  3;114,192,113;,
  3;115,192,114;,
  3;116,192,115;,
  3;117,192,116;,
  3;118,192,117;,
  3;119,192,118;,
  3;120,192,119;,
  3;121,192,120;,
  3;122,192,121;,
  3;123,192,122;,
  3;124,192,123;,
  3;125,192,124;,
  3;126,192,125;,
  3;127,192,126;,
  3;128,192,127;,
  3;129,192,128;,
  3;130,192,129;,
  3;131,192,130;,
  3;112,192,131;,
  3;172,193,173;,
  3;173,193,174;,
  3;174,193,175;,
  3;175,193,176;,
  3;176,193,177;,
  3;177,193,178;,
  3;178,193,179;,
  3;179,193,180;,
  3;180,193,181;,
  3;181,193,182;,
  3;182,193,183;,
  3;183,193,184;,
  3;184,193,185;,
  3;185,193,186;,
  3;186,193,187;,
  3;187,193,188;,
  3;188,193,189;,
  3;189,193,190;,
  3;190,193,191;,
  3;191,193,172;,
  3;259,19,194;,
  4;260,237,260,260;,
  4;261,259,194,261;,
  3;19,18,194;,
  3;195,195,195;,
  3;196,196,196;,
  3;262,262,262;,
  3;197,197,197;,
  3;198,198,198;,
  3;263,263,263;,
  3;199,199,199;,
  3;200,200,200;,
  3;264,264,264;,
  3;201,201,201;,
  3;202,202,202;,
  3;265,265,265;,
  3;203,203,203;,
  3;204,204,204;,
  3;266,266,266;,
  3;205,205,205;,
  3;206,206,206;,
  3;267,267,267;,
  3;207,207,207;,
  3;208,208,208;,
  3;268,268,268;,
  3;209,209,209;,
  3;210,210,210;,
  3;269,269,269;,
  3;211,211,211;,
  3;212,212,212;,
  3;270,270,270;,
  3;213,213,213;,
  3;214,214,214;,
  3;271,271,271;;
 }
 MeshTextureCoords {
  323;
  0.110760;0.817600;,
  0.096990;0.817600;,
  0.096990;0.800960;,
  0.110760;0.800960;,
  0.075380;0.717430;,
  0.078290;0.711410;,
  0.075380;0.726550;,
  0.082430;0.711410;,
  0.096990;0.711410;,
  0.110760;0.711410;,
  0.086020;0.711410;,
  0.097770;0.726360;,
  0.114040;0.711410;,
  0.120150;0.726180;,
  0.075380;0.645300;,
  0.075380;0.616260;,
  0.086020;0.645300;,
  0.075380;0.595550;,
  0.075380;0.571270;,
  0.096990;0.645300;,
  0.075380;0.847580;,
  0.086020;0.847580;,
  0.096990;0.847580;,
  0.110760;0.645300;,
  0.110760;0.847580;,
  0.120150;0.847580;,
  0.120150;0.851540;,
  0.120150;0.595550;,
  0.120150;0.645300;,
  0.120150;0.616260;,
  0.148890;0.800960;,
  0.148890;0.810940;,
  0.142730;0.809840;,
  0.142120;0.809080;,
  0.051050;0.808100;,
  0.046640;0.812050;,
  0.046640;0.805120;,
  0.086020;0.800960;,
  0.086020;0.817600;,
  0.096990;0.835950;,
  0.086020;0.835950;,
  0.110760;0.835950;,
  0.148890;0.817600;,
  0.148890;0.835950;,
  0.142280;0.817600;,
  0.135660;0.835950;,
  0.135660;0.817600;,
  0.055460;0.817600;,
  0.059870;0.813650;,
  0.067630;0.817600;,
  0.059870;0.826780;,
  0.135660;0.781470;,
  0.148890;0.781470;,
  0.135660;0.800960;,
  0.046640;0.781470;,
  0.059870;0.781470;,
  0.059870;0.800960;,
  0.046640;0.800960;,
  0.096990;0.843990;,
  0.086020;0.843990;,
  0.110760;0.843990;,
  0.148890;0.843990;,
  0.135660;0.843990;,
  0.046640;0.835950;,
  0.059870;0.835950;,
  0.059870;0.843990;,
  0.046640;0.843990;,
  0.075380;0.817600;,
  0.075380;0.802620;,
  0.075380;0.800960;,
  0.075380;0.835950;,
  0.075380;0.843990;,
  0.120150;0.835950;,
  0.120150;0.843990;,
  0.120150;0.817600;,
  0.120150;0.800960;,
  0.148890;0.847580;,
  0.135660;0.847580;,
  0.059870;0.847580;,
  0.046640;0.847580;,
  0.148890;0.851540;,
  0.135660;0.851540;,
  0.059870;0.851540;,
  0.046640;0.851540;,
  0.120150;0.781470;,
  0.075380;0.851540;,
  0.164920;0.847580;,
  0.164920;0.817600;,
  0.148890;0.814270;,
  0.164920;0.801420;,
  0.155480;0.781470;,
  0.030610;0.817600;,
  0.040290;0.781470;,
  0.030610;0.801930;,
  0.046640;0.814830;,
  0.051050;0.817600;,
  0.046640;0.826780;,
  0.038630;0.817600;,
  0.030610;0.847580;,
  0.139970;0.808970;,
  0.142120;0.809370;,
  0.142430;0.809750;,
  0.142150;0.810950;,
  0.137820;0.808300;,
  0.135660;0.809280;,
  0.135660;0.804290;,
  0.146090;0.814070;,
  0.137840;0.812910;,
  0.135660;0.815110;,
  0.135660;0.810940;,
  0.140020;0.813220;,
  0.142180;0.812230;,
  0.144140;0.813800;,
  0.142240;0.815560;,
  0.376270;0.033890;,
  0.412230;0.041590;,
  0.344350;0.037530;,
  0.416270;0.052940;,
  0.317210;0.055960;,
  0.389130;0.071370;,
  0.321990;0.070750;,
  0.357950;0.078450;,
  0.055460;0.806930;,
  0.059870;0.803360;,
  0.067630;0.804190;,
  0.059870;0.809700;,
  0.410260;0.058000;,
  0.378650;0.076080;,
  0.410260;0.039910;,
  0.347030;0.076080;,
  0.378650;0.027210;,
  0.315420;0.063380;,
  0.347030;0.032590;,
  0.315420;0.050680;,
  0.117390;0.711410;,
  0.120150;0.711410;,
  0.120150;0.717990;,
  0.160720;0.781470;,
  0.164920;0.781470;,
  0.164920;0.790790;,
  0.030610;0.789970;,
  0.030610;0.781470;,
  0.034460;0.781470;,
  0.120150;0.736490;,
  0.095780;0.736490;,
  0.120150;0.757200;,
  0.075380;0.736490;,
  0.075380;0.711410;,
  0.008230;0.823510;,
  0.008230;0.767460;,
  0.017180;0.767460;,
  0.017180;0.823510;,
  0.026140;0.767460;,
  0.026140;0.823510;,
  0.035090;0.767460;,
  0.035090;0.823510;,
  0.044040;0.767460;,
  0.044040;0.823510;,
  0.053000;0.767460;,
  0.053000;0.823510;,
  0.061950;0.767460;,
  0.061950;0.823510;,
  0.070910;0.767460;,
  0.070910;0.823510;,
  0.079860;0.767460;,
  0.079860;0.823510;,
  0.088810;0.767460;,
  0.088810;0.823510;,
  0.097770;0.767460;,
  0.097770;0.823510;,
  0.106720;0.767460;,
  0.106720;0.823510;,
  0.115680;0.767460;,
  0.115680;0.823510;,
  0.124630;0.767460;,
  0.124630;0.823510;,
  0.133580;0.767460;,
  0.133580;0.823510;,
  0.142540;0.767460;,
  0.142540;0.823510;,
  0.151490;0.767460;,
  0.151490;0.823510;,
  0.160440;0.767460;,
  0.160440;0.823510;,
  0.169400;0.767460;,
  0.169400;0.823510;,
  0.178350;0.767460;,
  0.178350;0.823510;,
  0.187310;0.767460;,
  0.187310;0.823510;,
  0.008230;0.711410;,
  0.017180;0.711410;,
  0.026140;0.711410;,
  0.035090;0.711410;,
  0.044040;0.711410;,
  0.053000;0.711410;,
  0.061950;0.711410;,
  0.070910;0.711410;,
  0.079860;0.711410;,
  0.088810;0.711410;,
  0.097770;0.711410;,
  0.106720;0.711410;,
  0.115680;0.711410;,
  0.124630;0.711410;,
  0.133580;0.711410;,
  0.142540;0.711410;,
  0.151490;0.711410;,
  0.160440;0.711410;,
  0.169400;0.711410;,
  0.178350;0.711410;,
  0.187310;0.711410;,
  0.008230;0.655350;,
  0.017180;0.655350;,
  0.026140;0.655350;,
  0.035090;0.655350;,
  0.044040;0.655350;,
  0.053000;0.655350;,
  0.061950;0.655350;,
  0.070910;0.655350;,
  0.079860;0.655350;,
  0.088810;0.655350;,
  0.097770;0.655350;,
  0.106720;0.655350;,
  0.115680;0.655350;,
  0.124630;0.655350;,
  0.133580;0.655350;,
  0.142540;0.655350;,
  0.151490;0.655350;,
  0.160440;0.655350;,
  0.169400;0.655350;,
  0.178350;0.655350;,
  0.187310;0.655350;,
  0.012710;0.851540;,
  0.021660;0.851540;,
  0.030610;0.851540;,
  0.039570;0.851540;,
  0.048520;0.851540;,
  0.057480;0.851540;,
  0.066430;0.851540;,
  0.075380;0.851540;,
  0.084340;0.851540;,
  0.093290;0.851540;,
  0.102240;0.851540;,
  0.111200;0.851540;,
  0.120150;0.851540;,
  0.129110;0.851540;,
  0.138060;0.851540;,
  0.147010;0.851540;,
  0.155970;0.851540;,
  0.164920;0.851540;,
  0.173880;0.851540;,
  0.182830;0.851540;,
  0.012710;0.571270;,
  0.021660;0.571270;,
  0.030610;0.571270;,
  0.039570;0.571270;,
  0.048520;0.571270;,
  0.057480;0.571270;,
  0.066430;0.571270;,
  0.075380;0.571270;,
  0.084340;0.571270;,
  0.093290;0.571270;,
  0.102240;0.571270;,
  0.111200;0.571270;,
  0.120150;0.571270;,
  0.129110;0.571270;,
  0.138060;0.571270;,
  0.147010;0.571270;,
  0.155970;0.571270;,
  0.164920;0.571270;,
  0.173880;0.571270;,
  0.182830;0.571270;,
  0.107010;0.834660;,
  0.920330;0.072270;,
  0.943410;0.026120;,
  0.931870;0.072270;,
  0.954950;0.072270;,
  0.966490;0.072270;,
  0.920330;0.072270;,
  0.943410;0.026120;,
  0.931870;0.072270;,
  0.954950;0.072270;,
  0.966490;0.072270;,
  0.920330;0.072270;,
  0.943410;0.026120;,
  0.931870;0.072270;,
  0.954950;0.072270;,
  0.966490;0.072270;,
  0.920330;0.072270;,
  0.943410;0.026120;,
  0.931870;0.072270;,
  0.954950;0.072270;,
  0.966490;0.072270;,
  0.920330;0.072270;,
  0.943410;0.026120;,
  0.931870;0.072270;,
  0.954950;0.072270;,
  0.966490;0.072270;,
  0.920330;0.072270;,
  0.943410;0.026120;,
  0.931870;0.072270;,
  0.954950;0.072270;,
  0.966490;0.072270;,
  0.920330;0.072270;,
  0.943410;0.026120;,
  0.931870;0.072270;,
  0.954950;0.072270;,
  0.966490;0.072270;,
  0.920330;0.072270;,
  0.943410;0.026120;,
  0.931870;0.072270;,
  0.954950;0.072270;,
  0.966490;0.072270;,
  0.920330;0.072270;,
  0.943410;0.026120;,
  0.931870;0.072270;,
  0.954950;0.072270;,
  0.966490;0.072270;,
  0.920330;0.072270;,
  0.943410;0.026120;,
  0.931870;0.072270;,
  0.954950;0.072270;,
  0.966490;0.072270;;
 }
}
