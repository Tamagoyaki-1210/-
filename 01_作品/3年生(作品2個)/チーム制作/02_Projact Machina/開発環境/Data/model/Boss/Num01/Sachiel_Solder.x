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
 5.59032;-0.06147;-5.50302;,
 7.06373;-8.63574;-2.88856;,
 1.19997;-8.63574;-2.88856;,
 2.67338;-0.06147;-5.50302;,
 7.06373;-8.63574;2.34036;,
 1.19997;-8.63574;2.34036;,
 5.59032;-0.06147;4.95482;,
 2.67338;-0.06147;4.95482;,
 5.59032;8.51282;2.34036;,
 2.67338;8.51282;2.34036;,
 5.59032;8.51282;-2.88856;,
 2.67338;8.51282;-2.88856;,
 5.59032;-0.06147;-5.50302;,
 2.67338;-0.06147;-5.50302;,
 14.04858;-0.06147;-0.27410;,
 14.04858;-0.06147;-0.27410;,
 14.04858;-0.06147;-0.27410;,
 14.04858;-0.06147;-0.27410;,
 14.04858;-0.06147;-0.27410;,
 14.04858;-0.06147;-0.27410;,
 0.31127;-0.06147;-0.27410;,
 0.31127;-0.06147;-0.27410;,
 0.31127;-0.06147;-0.27410;,
 0.31127;-0.06147;-0.27410;,
 0.31127;-0.06147;-0.27410;,
 0.31127;-0.06147;-0.27410;;
 
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
  1,
  1,
  1,
  1,
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
  27;
  0.525345;0.043162;-0.849794;,
  0.276147;0.022688;-0.960847;,
  0.269486;-0.238205;-0.933079;,
  0.429193;-0.903213;-0.000000;,
  0.276147;0.022688;0.960847;,
  0.263847;0.281326;0.922627;,
  0.385828;0.922571;0.000000;,
  0.000000;-0.000000;-1.000000;,
  0.000000;-0.291662;-0.956522;,
  0.000000;-1.000000;-0.000000;,
  0.000000;-0.000000;1.000000;,
  0.000000;0.291661;0.956522;,
  0.000000;1.000000;0.000000;,
  -0.996427;0.084457;0.000000;,
  0.269486;-0.238205;0.933079;,
  0.000000;-0.291662;0.956522;,
  0.263847;0.281326;-0.922627;,
  0.000000;0.291661;-0.956522;,
  0.775306;-0.631586;-0.000000;,
  0.525345;0.043162;0.849794;,
  0.711907;0.702274;0.000000;,
  -0.908559;0.077891;-0.410432;,
  -0.976804;-0.036927;-0.210930;,
  -0.976804;-0.036927;0.210930;,
  -0.908558;0.077891;0.410432;,
  -0.957209;0.199886;0.209278;,
  -0.957209;0.199886;-0.209278;;
  18;
  4;1,2,8,7;,
  4;3,3,9,9;,
  4;14,4,10,15;,
  4;4,5,11,10;,
  4;6,6,12,12;,
  4;16,1,7,17;,
  3;0,2,1;,
  3;18,3,3;,
  3;19,4,14;,
  3;19,5,4;,
  3;20,6,6;,
  3;0,1,16;,
  3;13,21,22;,
  3;13,22,23;,
  3;13,23,24;,
  3;13,24,25;,
  3;13,25,26;,
  3;13,26,21;;
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
