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
 32;
 10.62208;2.73928;-4.06836;,
 10.62208;2.73928;3.93826;,
 10.62208;-1.65104;3.93826;,
 10.62208;-1.65104;-4.06836;,
 0.42304;2.73928;3.93826;,
 0.42304;2.73928;-4.06836;,
 0.42304;-1.65104;-4.06836;,
 0.42304;-1.65104;3.93826;,
 5.29700;2.73928;-4.06836;,
 5.29700;-1.65104;-4.06836;,
 5.29700;2.73928;3.93826;,
 5.29700;-1.65104;3.93826;,
 5.29700;-3.95668;3.93826;,
 5.29700;-3.95668;-4.06836;,
 0.42304;-3.95668;3.93826;,
 0.42304;-3.95668;-4.06836;,
 11.50450;3.31864;-4.76110;,
 11.50450;-1.83140;-4.76110;,
 11.50450;-1.83140;4.63100;,
 11.50450;3.31864;4.63100;,
 -0.45938;3.31864;4.63100;,
 -0.45938;-1.83140;4.63100;,
 -0.45938;-1.83140;-4.76110;,
 -0.45938;3.31864;-4.76110;,
 5.25798;3.31864;-4.76110;,
 5.25798;-1.83140;-4.76110;,
 5.25798;3.31864;4.63100;,
 5.25798;-1.83140;4.63100;,
 5.25798;-4.53604;4.63100;,
 5.25798;-4.53604;-4.76110;,
 -0.45938;-4.53604;4.63100;,
 -0.45938;-4.53604;-4.76110;;
 
 28;
 4;0,1,2,3;,
 4;4,5,6,7;,
 4;8,0,3,9;,
 4;8,9,6,5;,
 4;8,5,4,10;,
 4;8,10,1,0;,
 4;9,3,2,11;,
 4;10,4,7,11;,
 4;10,11,2,1;,
 4;12,13,9,11;,
 4;14,12,11,7;,
 4;15,14,7,6;,
 4;13,15,6,9;,
 4;13,12,14,15;,
 4;16,17,18,19;,
 4;20,21,22,23;,
 4;24,25,17,16;,
 4;24,23,22,25;,
 4;24,26,20,23;,
 4;24,16,19,26;,
 4;25,27,18,17;,
 4;26,27,21,20;,
 4;26,19,18,27;,
 4;28,27,25,29;,
 4;30,21,27,28;,
 4;31,22,21,30;,
 4;29,25,22,31;,
 4;29,31,30,28;;
 
 MeshMaterialList {
  4;
  28;
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
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
   1.000000;1.000000;1.000000;1.000000;;
   0.000000;
   0.000000;0.000000;0.000000;;
   0.150000;0.150000;0.150000;;
  }
  Material {
   0.800000;0.800000;0.800000;1.000000;;
   5.000000;
   0.000000;0.000000;0.000000;;
   0.000000;0.000000;0.000000;;
   TextureFilename {
    "Data\\texture\\black.png";
   }
  }
  Material {
   0.668000;0.690400;0.210400;1.000000;;
   5.000000;
   0.160000;0.160000;0.160000;;
   0.000000;0.000000;0.000000;;
  }
  Material {
   0.000000;0.000000;0.000000;1.000000;;
   5.000000;
   0.000000;0.000000;0.000000;;
   0.000000;0.000000;0.000000;;
  }
 }
 MeshNormals {
  12;
  -1.000000;0.000000;0.000000;,
  1.000000;0.000000;0.000000;,
  0.000000;0.000000;-1.000000;,
  0.000000;1.000000;0.000000;,
  0.000000;-1.000000;-0.000000;,
  0.000000;0.000000;1.000000;,
  1.000000;0.000000;0.000000;,
  -1.000000;0.000000;0.000000;,
  0.000000;0.000000;1.000000;,
  0.000000;-1.000000;-0.000000;,
  0.000000;1.000000;0.000000;,
  0.000000;0.000000;-1.000000;;
  28;
  4;1,1,1,1;,
  4;0,0,0,0;,
  4;2,2,2,2;,
  4;2,2,2,2;,
  4;3,3,3,3;,
  4;3,3,3,3;,
  4;4,4,4,4;,
  4;5,5,5,5;,
  4;5,5,5,5;,
  4;1,1,1,1;,
  4;5,5,5,5;,
  4;0,0,0,0;,
  4;2,2,2,2;,
  4;4,4,4,4;,
  4;7,7,7,7;,
  4;6,6,6,6;,
  4;8,8,8,8;,
  4;8,8,8,8;,
  4;9,9,9,9;,
  4;9,9,9,9;,
  4;10,10,10,10;,
  4;11,11,11,11;,
  4;11,11,11,11;,
  4;7,7,7,7;,
  4;11,11,11,11;,
  4;6,6,6,6;,
  4;8,8,8,8;,
  4;10,10,10,10;;
 }
}
