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
 43;
 0.00000;8.43951;-2.54976;,
 3.45411;8.43951;-1.80295;,
 1.66466;-0.50508;-1.31025;,
 0.00000;-0.50508;-1.85298;,
 3.37130;8.43951;0.00000;,
 2.35419;-0.50508;-0.00000;,
 3.45411;8.43951;1.80295;,
 1.66466;-0.50508;1.31025;,
 0.00000;8.43951;2.26374;,
 0.00000;-0.50508;1.85298;,
 -3.45059;8.43951;1.80295;,
 -1.66466;-0.50508;1.31025;,
 -3.37130;8.43951;0.00000;,
 -2.35419;-0.50508;0.00000;,
 -3.45059;8.43951;-1.80295;,
 -1.66466;-0.50508;-1.31025;,
 0.00000;8.43951;-2.54976;,
 0.00000;-0.50508;-1.85298;,
 2.23421;-3.69005;-1.75854;,
 0.00000;-3.61878;-2.48696;,
 3.15965;-5.36742;-0.00000;,
 2.23421;-3.69005;1.75854;,
 0.00000;-3.61878;2.48696;,
 -2.23421;-3.69005;1.75854;,
 -3.15965;-5.36742;0.00000;,
 -2.23421;-3.69005;-1.75854;,
 0.00000;-3.61878;-2.48696;,
 0.00000;13.14877;0.92486;,
 0.00000;13.14877;0.92486;,
 0.00000;13.14877;0.92486;,
 0.00000;13.14877;0.92486;,
 0.00000;13.14877;0.92486;,
 0.00000;13.14877;0.92486;,
 0.00000;13.14877;0.92486;,
 0.00000;13.14877;0.92486;,
 0.00000;-2.98642;-0.00000;,
 0.00000;-2.98642;-0.00000;,
 0.00000;-2.98642;-0.00000;,
 0.00000;-2.98642;-0.00000;,
 0.00000;-2.98642;-0.00000;,
 0.00000;-2.98642;-0.00000;,
 0.00000;-2.98642;-0.00000;,
 0.00000;-2.98642;-0.00000;;
 
 32;
 4;0,1,2,3;,
 4;1,4,5,2;,
 4;4,6,7,5;,
 4;6,8,9,7;,
 4;8,10,11,9;,
 4;10,12,13,11;,
 4;12,14,15,13;,
 4;14,16,17,15;,
 4;3,2,18,19;,
 4;2,5,20,18;,
 4;5,7,21,20;,
 4;7,9,22,21;,
 4;9,11,23,22;,
 4;11,13,24,23;,
 4;13,15,25,24;,
 4;15,17,26,25;,
 3;27,1,0;,
 3;28,4,1;,
 3;29,6,4;,
 3;30,8,6;,
 3;31,10,8;,
 3;32,12,10;,
 3;33,14,12;,
 3;34,16,14;,
 3;35,19,18;,
 3;36,18,20;,
 3;37,20,21;,
 3;38,21,22;,
 3;39,22,23;,
 3;40,23,24;,
 3;41,24,25;,
 3;42,25,26;;
 
 MeshMaterialList {
  4;
  32;
  0,
  1,
  1,
  0,
  0,
  1,
  1,
  0,
  0,
  1,
  1,
  0,
  0,
  1,
  1,
  0,
  0,
  1,
  1,
  0,
  0,
  1,
  1,
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
   0.401600;0.401600;0.401600;1.000000;;
   5.000000;
   0.910000;0.910000;0.910000;;
   0.000000;0.000000;0.000000;;
  }
  Material {
   0.191200;0.000000;0.276000;1.000000;;
   5.000000;
   0.720000;0.720000;0.720000;;
   0.000000;0.000000;0.000000;;
  }
  Material {
   0.062400;0.062400;0.062400;1.000000;;
   5.000000;
   0.830000;0.830000;0.830000;;
   0.000000;0.000000;0.000000;;
  }
  Material {
   0.282400;0.000000;0.006400;1.000000;;
   5.000000;
   0.960000;0.960000;0.960000;;
   0.000000;0.000000;0.000000;;
  }
 }
 MeshNormals {
  45;
  0.320450;0.695771;-0.642817;,
  -0.000074;0.274079;-0.961707;,
  0.230156;0.266717;-0.935890;,
  0.973133;0.230245;0.000087;,
  0.967946;0.232114;0.095938;,
  -0.000050;0.107704;0.994183;,
  -0.177275;0.106001;0.978436;,
  -0.973099;0.230388;0.000080;,
  -0.969176;0.226502;-0.096922;,
  -0.000027;0.054531;-0.998512;,
  0.535247;0.105937;-0.838026;,
  0.999909;0.013473;0.000000;,
  0.811881;0.079905;0.578329;,
  -0.000017;0.065901;0.997826;,
  -0.522835;0.114478;0.844712;,
  -0.999908;0.013585;0.000000;,
  -0.811684;0.079969;-0.578596;,
  0.000000;0.197910;-0.980220;,
  0.656725;0.204303;-0.725928;,
  0.979100;0.203382;0.000000;,
  0.656725;0.204303;0.725928;,
  0.000000;0.197910;0.980220;,
  -0.656725;0.204303;0.725928;,
  -0.979100;0.203382;0.000000;,
  -0.656725;0.204303;-0.725928;,
  0.000000;-1.000000;-0.000000;,
  0.969369;0.226267;-0.095532;,
  0.963657;-0.161409;-0.212868;,
  0.177178;0.105997;0.978454;,
  0.221696;-0.062474;0.973113;,
  -0.967812;0.232100;0.097307;,
  -0.963479;-0.161202;0.213830;,
  -0.230298;0.266717;-0.935855;,
  -0.260196;-0.084861;-0.961820;,
  0.816015;0.576814;0.037480;,
  -0.000064;0.273470;0.961881;,
  -0.813131;0.582081;0.000146;,
  0.000000;-0.969161;0.246429;,
  -0.362006;-0.927953;-0.088632;,
  -0.601821;-0.798631;0.000000;,
  -0.362006;-0.927953;0.088632;,
  0.000000;-0.969161;-0.246429;,
  0.362006;-0.927953;0.088632;,
  0.601821;-0.798631;-0.000000;,
  0.362006;-0.927953;-0.088632;;
  32;
  4;1,2,10,9;,
  4;26,3,11,27;,
  4;3,4,12,11;,
  4;28,5,13,29;,
  4;5,6,14,13;,
  4;30,7,15,31;,
  4;7,8,16,15;,
  4;32,1,9,33;,
  4;9,10,18,17;,
  4;10,11,19,18;,
  4;11,12,20,19;,
  4;12,13,21,20;,
  4;13,14,22,21;,
  4;14,15,23,22;,
  4;15,16,24,23;,
  4;16,9,17,24;,
  3;0,2,1;,
  3;34,3,26;,
  3;0,4,3;,
  3;35,5,28;,
  3;35,6,5;,
  3;36,7,30;,
  3;36,8,7;,
  3;0,1,32;,
  3;25,37,38;,
  3;25,38,39;,
  3;25,39,40;,
  3;25,40,41;,
  3;25,41,42;,
  3;25,42,43;,
  3;25,43,44;,
  3;25,44,37;;
 }
 MeshTextureCoords {
  43;
  0.000000;0.000000;,
  0.125000;0.000000;,
  0.125000;0.500000;,
  0.000000;0.500000;,
  0.250000;0.000000;,
  0.250000;0.500000;,
  0.375000;0.000000;,
  0.375000;0.500000;,
  0.500000;0.000000;,
  0.500000;0.500000;,
  0.625000;0.000000;,
  0.625000;0.500000;,
  0.750000;0.000000;,
  0.750000;0.500000;,
  0.875000;0.000000;,
  0.875000;0.500000;,
  1.000000;0.000000;,
  1.000000;0.500000;,
  0.125000;1.000000;,
  0.000000;1.000000;,
  0.250000;1.000000;,
  0.375000;1.000000;,
  0.500000;1.000000;,
  0.625000;1.000000;,
  0.750000;1.000000;,
  0.875000;1.000000;,
  1.000000;1.000000;,
  0.062500;0.000000;,
  0.187500;0.000000;,
  0.312500;0.000000;,
  0.437500;0.000000;,
  0.562500;0.000000;,
  0.687500;0.000000;,
  0.812500;0.000000;,
  0.937500;0.000000;,
  0.062500;1.000000;,
  0.187500;1.000000;,
  0.312500;1.000000;,
  0.437500;1.000000;,
  0.562500;1.000000;,
  0.687500;1.000000;,
  0.812500;1.000000;,
  0.937500;1.000000;;
 }
}
