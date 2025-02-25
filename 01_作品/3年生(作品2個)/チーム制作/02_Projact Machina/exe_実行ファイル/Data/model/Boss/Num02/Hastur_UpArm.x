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
 0.25015;-70.29290;-4.62834;,
 -3.66834;-70.29290;-2.36600;,
 -10.16506;0.34643;-6.11693;,
 0.25015;0.34643;-10.15361;,
 -3.66834;-70.29290;2.15881;,
 -10.16506;0.34643;5.90973;,
 0.25015;-70.29290;4.42114;,
 0.25015;0.34643;9.94642;,
 4.16868;-70.29290;2.15881;,
 10.66565;8.17089;5.90973;,
 4.16868;-70.29290;-2.36600;,
 10.66565;8.17089;-6.11693;,
 0.25015;-70.29290;-4.62834;,
 0.25015;0.34643;-10.15361;,
 0.25014;-92.69975;-0.10360;,
 0.25014;-92.69975;-0.10360;,
 0.25014;-92.69975;-0.10360;,
 0.25014;-92.69975;-0.10360;,
 0.25014;-92.69975;-0.10360;,
 0.25014;-92.69975;-0.10360;,
 0.25015;0.34643;-0.10360;,
 0.25015;0.34643;-0.10360;,
 0.25015;0.34643;-0.10360;,
 0.25015;0.34643;-0.10360;,
 0.25015;0.34643;-0.10360;,
 0.25015;0.34643;-0.10360;;
 
 18;
 4;0,1,2,3;,
 4;1,4,5,2;,
 4;4,6,7,5;,
 4;6,8,9,7;,
 4;8,10,11,9;,
 4;10,12,13,11;,
 3;14,1,0;,
 3;15,4,1;,
 3;16,6,4;,
 3;17,8,6;,
 3;18,10,8;,
 3;19,12,10;,
 3;20,3,2;,
 3;21,2,5;,
 3;22,5,7;,
 3;23,7,9;,
 3;24,9,11;,
 3;25,11,13;;
 
 MeshMaterialList {
  4;
  18;
  0,
  0,
  0,
  2,
  2,
  2,
  0,
  0,
  0,
  2,
  2,
  2,
  0,
  0,
  0,
  0,
  0,
  0;;
  Material {
   0.483200;0.483200;0.483200;1.000000;;
   5.000000;
   0.730000;0.730000;0.730000;;
   0.000000;0.000000;0.000000;;
  }
  Material {
   0.132000;0.132000;0.132000;1.000000;;
   5.000000;
   0.830000;0.830000;0.830000;;
   0.000000;0.000000;0.000000;;
  }
  Material {
   0.448800;0.000000;0.000000;1.000000;;
   5.000000;
   0.900000;0.900000;0.900000;;
   0.000000;0.000000;0.000000;;
  }
  Material {
   0.800000;0.800000;0.800000;1.000000;;
   5.000000;
   0.000000;0.000000;0.000000;;
   0.000000;0.000000;0.000000;;
  }
 }
 MeshNormals {
  24;
  -0.848891;-0.197938;-0.490107;,
  0.006906;-0.139579;-0.990187;,
  -0.462543;-0.123996;-0.877883;,
  -0.932485;-0.164419;0.321619;,
  0.006906;-0.139579;0.990187;,
  0.474578;-0.122620;0.871631;,
  0.933042;-0.161048;-0.321709;,
  0.013589;-0.082451;-0.996503;,
  -0.430783;-0.075248;-0.899313;,
  -0.995797;-0.091584;0.000000;,
  0.013589;-0.082451;0.996502;,
  0.455150;-0.072589;0.887451;,
  0.996589;-0.082520;-0.000000;,
  -0.316601;0.948559;0.000000;,
  -0.991245;-0.132033;0.000000;,
  -0.430783;-0.075248;0.899313;,
  0.991836;-0.127524;-0.000000;,
  0.455149;-0.072589;-0.887451;,
  -0.000003;-0.197940;0.980214;,
  0.848889;-0.197940;-0.490109;,
  0.000000;1.000000;0.000000;,
  -0.316601;0.948559;0.000000;,
  -0.600630;0.799527;0.000000;,
  -0.600630;0.799527;0.000000;;
  18;
  4;1,2,8,7;,
  4;14,3,9,9;,
  4;15,4,10,15;,
  4;4,5,11,10;,
  4;16,6,12,12;,
  4;17,1,7,17;,
  3;0,2,1;,
  3;0,3,14;,
  3;18,4,3;,
  3;18,5,4;,
  3;19,6,16;,
  3;19,1,6;,
  3;13,13,20;,
  3;13,20,20;,
  3;13,20,21;,
  3;13,21,22;,
  3;13,22,23;,
  3;13,23,13;;
 }
 MeshTextureCoords {
  26;
  0.000000;0.000000;,
  0.166670;0.000000;,
  0.166670;1.000000;,
  0.000000;1.000000;,
  0.333330;0.000000;,
  0.333330;1.000000;,
  0.500000;0.000000;,
  0.500000;1.000000;,
  0.666670;0.000000;,
  0.666670;1.000000;,
  0.833330;0.000000;,
  0.833330;1.000000;,
  1.000000;0.000000;,
  1.000000;1.000000;,
  0.083330;0.000000;,
  0.250000;0.000000;,
  0.416670;0.000000;,
  0.583330;0.000000;,
  0.750000;0.000000;,
  0.916670;0.000000;,
  0.083330;1.000000;,
  0.250000;1.000000;,
  0.416670;1.000000;,
  0.583330;1.000000;,
  0.750000;1.000000;,
  0.916670;1.000000;;
 }
}
