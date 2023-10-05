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
 -10.62208;2.73928;-4.06836;,
 -10.62208;-1.65104;-4.06836;,
 -10.62208;-1.65104;3.93826;,
 -10.62208;2.73928;3.93826;,
 -0.42304;2.73928;3.93826;,
 -0.42304;-1.65104;3.93826;,
 -0.42304;-1.65104;-4.06836;,
 -0.42304;2.73928;-4.06836;,
 -5.29700;2.73928;-4.06836;,
 -5.29700;-1.65104;-4.06836;,
 -5.29700;2.73928;3.93826;,
 -5.29700;-1.65104;3.93826;,
 -5.29700;-3.95668;3.93826;,
 -5.29700;-3.95668;-4.06836;,
 -0.42304;-3.95668;3.93826;,
 -0.42304;-3.95668;-4.06836;,
 -11.59468;3.37784;-4.83190;,
 -11.59468;3.37784;4.70180;,
 -11.59468;-1.84982;4.70180;,
 -11.59468;-1.84982;-4.83190;,
 0.54956;3.37784;4.70180;,
 0.54956;3.37784;-4.83190;,
 0.54956;-1.84982;-4.83190;,
 0.54956;-1.84982;4.70180;,
 -5.25398;3.37784;-4.83190;,
 -5.25398;-1.84982;-4.83190;,
 -5.25398;3.37784;4.70180;,
 -5.25398;-1.84982;4.70180;,
 -5.25398;-4.59524;4.70180;,
 -5.25398;-4.59524;-4.83190;,
 0.54956;-4.59524;4.70180;,
 0.54956;-4.59524;-4.83190;;
 
 28;
 4;0,1,2,3;,
 4;4,5,6,7;,
 4;8,9,1,0;,
 4;8,7,6,9;,
 4;8,10,4,7;,
 4;8,0,3,10;,
 4;9,11,2,1;,
 4;10,11,5,4;,
 4;10,3,2,11;,
 4;12,11,9,13;,
 4;14,5,11,12;,
 4;15,6,5,14;,
 4;13,9,6,15;,
 4;13,15,14,12;,
 4;16,17,18,19;,
 4;20,21,22,23;,
 4;24,16,19,25;,
 4;24,25,22,21;,
 4;24,21,20,26;,
 4;24,26,17,16;,
 4;25,19,18,27;,
 4;26,20,23,27;,
 4;26,27,18,17;,
 4;28,29,25,27;,
 4;30,28,27,23;,
 4;31,30,23,22;,
 4;29,31,22,25;,
 4;29,28,30,31;;
 
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
  1.000000;0.000000;0.000000;,
  -1.000000;0.000000;0.000000;,
  0.000000;0.000000;-1.000000;,
  0.000000;1.000000;0.000000;,
  0.000000;-1.000000;-0.000000;,
  0.000000;0.000000;1.000000;,
  -1.000000;0.000000;0.000000;,
  1.000000;0.000000;0.000000;,
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
