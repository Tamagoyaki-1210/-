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
 20;
 0.00922;-0.03325;1.88305;,
 0.00922;-25.58507;-0.53059;,
 2.08909;-0.03325;0.67623;,
 0.00922;-25.58507;-0.53059;,
 2.08909;4.69164;-1.73741;,
 0.00922;-25.58507;-0.53059;,
 0.00922;6.56874;-2.94423;,
 0.00922;-25.58507;-0.53059;,
 -2.07065;4.69164;-1.73741;,
 0.00922;-25.58507;-0.53059;,
 -2.07065;-0.03325;0.67623;,
 0.00922;-25.58507;-0.53059;,
 0.00922;-0.03325;1.88305;,
 0.00922;-0.03325;-0.53059;,
 0.00922;-0.03325;1.88305;,
 2.08909;-0.03325;0.67623;,
 2.08909;4.69164;-1.73741;,
 0.00922;6.56874;-2.94423;,
 -2.07065;4.69164;-1.73741;,
 -2.07065;-0.03325;0.67623;;
 
 12;
 3;0,1,2;,
 3;2,3,4;,
 3;4,5,6;,
 3;6,7,8;,
 3;8,9,10;,
 3;10,11,12;,
 3;13,14,15;,
 3;13,15,16;,
 3;13,16,17;,
 3;13,17,18;,
 3;13,18,19;,
 3;13,19,14;;
 
 MeshMaterialList {
  6;
  12;
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
   0.656000;0.656000;0.656000;1.000000;;
   5.000000;
   0.840000;0.840000;0.840000;;
   0.000000;0.000000;0.000000;;
  }
  Material {
   0.344800;0.344800;0.344800;1.000000;;
   5.000000;
   0.550000;0.550000;0.550000;;
   0.000000;0.000000;0.000000;;
  }
  Material {
   0.737600;0.737600;0.737600;1.000000;;
   5.000000;
   0.750000;0.750000;0.750000;;
   0.378020;0.378020;0.378020;;
  }
  Material {
   0.344800;0.344800;0.344800;1.000000;;
   5.000000;
   0.800000;0.800000;0.800000;;
   0.000000;0.000000;0.000000;;
  }
  Material {
   0.800000;0.019200;0.000000;1.000000;;
   5.000000;
   0.630000;0.630000;0.630000;;
   0.000000;0.000000;0.000000;;
  }
  Material {
   0.800000;0.800000;0.800000;1.000000;;
   5.000000;
   0.910000;0.910000;0.910000;;
   0.000000;0.000000;0.000000;;
  }
 }
 MeshNormals {
  15;
  0.500206;-0.081432;0.862069;,
  0.810682;-0.078815;-0.580157;,
  0.000000;-0.074855;-0.997194;,
  -0.810682;-0.078815;-0.580157;,
  -0.986851;-0.073530;-0.143940;,
  0.000000;1.000000;0.000000;,
  0.986851;-0.073530;-0.143940;,
  -0.500205;-0.081432;0.862069;,
  0.000000;1.000000;0.000000;,
  -0.000000;0.397328;0.917677;,
  -0.459059;0.404150;0.791156;,
  -0.347130;0.372620;0.860613;,
  -0.000000;0.343365;0.939202;,
  0.347130;0.372621;0.860613;,
  0.459059;0.404150;0.791156;;
  12;
  3;0,0,0;,
  3;6,1,1;,
  3;1,1,2;,
  3;2,3,3;,
  3;3,3,4;,
  3;7,7,7;,
  3;5,5,8;,
  3;9,10,11;,
  3;9,11,12;,
  3;9,12,13;,
  3;9,13,14;,
  3;5,5,5;;
 }
 MeshTextureCoords {
  20;
  0.000000;1.000000;,
  0.083330;0.000000;,
  0.166670;1.000000;,
  0.166670;0.000000;,
  0.333330;1.000000;,
  0.250000;0.000000;,
  0.500000;1.000000;,
  0.333330;0.000000;,
  0.666670;1.000000;,
  0.416670;0.000000;,
  0.833330;1.000000;,
  0.500000;0.000000;,
  1.000000;1.000000;,
  0.000000;0.000000;,
  0.000000;0.000000;,
  0.000000;0.000000;,
  0.000000;0.000000;,
  0.000000;0.000000;,
  0.000000;0.000000;,
  0.000000;0.000000;;
 }
}
