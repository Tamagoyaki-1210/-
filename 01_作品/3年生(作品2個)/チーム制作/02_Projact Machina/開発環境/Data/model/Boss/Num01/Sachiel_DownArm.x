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
 -4.55818;7.23402;-0.81824;,
 -1.65503;7.23402;-3.88718;,
 0.45830;-20.14682;-3.88718;,
 -0.33152;-20.14682;-0.81824;,
 4.15127;-0.14682;-3.88718;,
 2.03794;-20.14682;-3.88718;,
 4.62495;-0.14682;-0.81824;,
 2.82776;-20.14682;-0.81824;,
 4.15127;-0.14682;2.25070;,
 2.03794;-20.14682;2.25070;,
 -1.65503;7.23402;2.25070;,
 0.45830;-20.14682;2.25070;,
 -4.55818;7.23402;-0.81824;,
 -0.33152;-20.14682;-0.81824;,
 1.24812;-0.14682;-0.81824;,
 1.24812;-0.14682;-0.81824;,
 1.24812;-0.14682;-0.81824;,
 1.24812;-0.14682;-0.81824;,
 1.24812;-0.14682;-0.81824;,
 1.24812;-0.14682;-0.81824;,
 1.24812;-20.14682;-0.81824;,
 1.24812;-20.14682;-0.81824;,
 1.24812;-20.14682;-0.81824;,
 1.24812;-20.14682;-0.81824;,
 1.24812;-20.14682;-0.81824;,
 1.24812;-20.14682;-0.81824;;
 
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
  1,
  3,
  3,
  3,
  3,
  1,
  3,
  3,
  3,
  3,
  3,
  3,
  3,
  3,
  3,
  3,
  3,
  3;;
  Material {
   0.633600;0.633600;0.633600;1.000000;;
   5.000000;
   0.780000;0.780000;0.780000;;
   0.000000;0.000000;0.000000;;
  }
  Material {
   0.000000;0.392000;0.558400;1.000000;;
   5.000000;
   0.880000;0.880000;0.880000;;
   0.000000;0.117600;0.167520;;
  }
  Material {
   0.800000;0.006400;0.000000;1.000000;;
   5.000000;
   0.860000;0.860000;0.860000;;
   0.000000;0.000000;0.000000;;
  }
  Material {
   0.081600;0.081600;0.081600;1.000000;;
   5.000000;
   0.570000;0.570000;0.570000;;
   0.000000;0.000000;0.000000;;
  }
 }
 MeshNormals {
  13;
  0.630726;0.496175;0.596653;,
  -0.993983;-0.109536;0.000000;,
  -0.871456;-0.096034;-0.480980;,
  -0.000000;-0.000000;-1.000000;,
  0.995263;-0.097215;0.000000;,
  0.975117;-0.095247;0.200189;,
  0.000000;0.000000;1.000000;,
  0.000000;-1.000000;-0.000000;,
  0.975117;-0.095247;-0.200189;,
  -0.871456;-0.096034;0.480980;,
  0.000000;1.000000;0.000000;,
  0.630726;0.496175;-0.596653;,
  0.630726;0.496175;-0.596653;;
  18;
  4;1,2,2,1;,
  4;3,3,3,3;,
  4;8,4,4,8;,
  4;4,5,5,4;,
  4;6,6,6,6;,
  4;9,1,1,9;,
  3;0,0,0;,
  3;0,0,0;,
  3;10,10,10;,
  3;10,10,10;,
  3;11,11,11;,
  3;11,12,11;,
  3;7,7,7;,
  3;7,7,7;,
  3;7,7,7;,
  3;7,7,7;,
  3;7,7,7;,
  3;7,7,7;;
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
