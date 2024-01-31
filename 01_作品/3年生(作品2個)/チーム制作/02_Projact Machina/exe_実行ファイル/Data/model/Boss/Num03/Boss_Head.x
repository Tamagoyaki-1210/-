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
 18;
 0.00000;5.01370;0.30841;,
 2.12009;2.81643;0.09683;,
 3.30471;2.98537;-1.65767;,
 0.00000;7.64087;-1.20939;,
 0.00000;-0.48954;-0.22149;,
 0.00000;-1.59191;-2.09841;,
 -2.12009;2.81643;0.09683;,
 -3.30471;2.98537;-1.65767;,
 0.00000;5.01370;0.30841;,
 0.00000;7.64087;-1.20939;,
 0.00000;2.19766;1.15879;,
 0.00000;2.19766;1.15879;,
 0.00000;2.19766;1.15879;,
 0.00000;2.19766;1.15879;,
 0.00000;4.83940;-3.42804;,
 0.00000;4.83940;-3.42804;,
 0.00000;4.83940;-3.42804;,
 0.00000;4.83940;-3.42804;;
 
 12;
 4;0,1,2,3;,
 4;1,4,5,2;,
 4;4,6,7,5;,
 4;6,8,9,7;,
 3;10,1,0;,
 3;11,4,1;,
 3;12,6,4;,
 3;13,8,6;,
 3;14,3,2;,
 3;15,2,5;,
 3;16,5,7;,
 3;17,7,9;;
 
 MeshMaterialList {
  4;
  12;
  1,
  1,
  1,
  1,
  2,
  2,
  2,
  2,
  1,
  2,
  2,
  1;;
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
  17;
  0.192469;0.054662;0.979779;,
  0.243513;0.385220;0.890116;,
  0.554515;0.131712;0.821684;,
  0.639666;-0.483319;0.597688;,
  -0.598017;-0.248928;0.761847;,
  0.647553;0.486670;0.586369;,
  0.742124;-0.543871;0.391734;,
  -0.742124;-0.543871;0.391734;,
  0.242620;0.046914;-0.968986;,
  -0.639666;-0.483319;0.597688;,
  -0.647553;0.486670;0.586369;,
  -0.501014;-0.395420;0.769823;,
  0.609233;0.492324;-0.621653;,
  0.536186;0.165323;-0.827752;,
  0.000000;-0.202462;-0.979290;,
  -0.536186;0.165323;-0.827752;,
  -0.609233;0.492324;-0.621653;;
  12;
  4;1,2,5,5;,
  4;6,3,6,6;,
  4;9,4,7,7;,
  4;10,10,10,10;,
  3;0,2,1;,
  3;0,3,2;,
  3;11,4,9;,
  3;0,1,4;,
  3;8,12,13;,
  3;8,13,14;,
  3;8,14,15;,
  3;16,15,16;;
 }
 MeshTextureCoords {
  18;
  0.000000;0.000000;,
  0.250000;0.000000;,
  0.250000;1.000000;,
  0.000000;1.000000;,
  0.500000;0.000000;,
  0.500000;1.000000;,
  0.750000;0.000000;,
  0.750000;1.000000;,
  1.000000;0.000000;,
  1.000000;1.000000;,
  0.125000;0.000000;,
  0.375000;0.000000;,
  0.625000;0.000000;,
  0.875000;0.000000;,
  0.125000;1.000000;,
  0.375000;1.000000;,
  0.625000;1.000000;,
  0.875000;1.000000;;
 }
}
