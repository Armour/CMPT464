//////////////////////////////////////////////////////////////////////////////
//
//  subdivision.h
//
//  The header file for function that do the subdivision
//
//  Project         : SmfView
//  Name            : Chong Guo
//  Student ID      : 301295753
//  SFU username    : armourg
//  Instructor      : Richard Zhang
//
//  Created by Armour on 2/18/2016
//  Copyright (c) 2016 Armour. All rights reserved.
//
//////////////////////////////////////////////////////////////////////////////

#ifndef SMFVIEW_SUBDIVISION_H_
#define SMFVIEW_SUBDIVISION_H_

#include <vector>
#include <map>

#include "lib_gl.h"
#include "lib_consts.h"
#include "winged_edge.h"

using namespace std;

// Some extern variables
extern int mesh_imported;
extern int subdivision_semaphore;       // Subdivision semaphore
extern vector<smfparser::Vertex *> mesh_vertex;     // The vertex data that stored in memory
extern vector<smfparser::Face *> mesh_faces;        // The faces data that stored in memory
extern map<pair<int, int>, smfparser::W_edge *> mesh_edges;     // The edges data that stored in memory
extern map<smfparser::Vertex *, GLuint> vertex_index_map;       // The vertex index map
extern vector<GLfloat> data_vertex;     // The vertex data used for rendering
extern vector<GLuint> data_faces;       // The faces data used for rendering
extern vector<GLuint> data_edges;       // The edges data used for rendering
extern void UpdateMeshBufferData();

namespace subdivision {

// Update new mesh data from vertex_input and face_input
void UpdateWingedEdgeMeshData();

// The loop subdivision
void LoopSubdivision(int level);

// Calculate beta for loop vertex mask with k adjacent vertex
float LoopVertexMaskBeta(int k);

// The butterfly subdivision
void ButterflySubdivision(int level);

// Calculate Sj for butterfly edge mask with k adjacent vertex
float ButterflyVertexMaskSj(int k, int j);

// Init mesh data for rendering
void UpdateRenderMeshData();

} // namespace subdivision

#endif // SMFVIEW_SUBDIVISION_H_
