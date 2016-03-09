//////////////////////////////////////////////////////////////////////////////
//
//  decimation.h
//
//  The header file for function that do the decimation to mesh data
//
//  Project         : SmfView
//  Name            : Chong Guo
//  Student ID      : 301295753
//  SFU username    : armourg
//  Instructor      : Richard Zhang
//
//  Created by Armour on 3/8/2016
//  Copyright (c) 2016 Armour. All rights reserved.
//
//////////////////////////////////////////////////////////////////////////////

#ifndef SMFVIEW_DECIMATION_H_
#define SMFVIEW_DECIMATION_H_

#include <vector>
#include <map>

#include "lib_gl.h"
#include "lib_consts.h"
#include "winged_edge.h"

using namespace std;

extern vector<smfparser::Vertex *> mesh_vertex;     // The vertex data that stored in memory
extern vector<smfparser::Face *> mesh_faces;        // The faces data that stored in memory
extern map<pair<int, int>, smfparser::W_edge *> mesh_edges;     // The edges data that stored in memory
extern map<smfparser::Vertex *, GLuint> vertex_index_map;       // The vertex index map
extern vector<GLfloat> data_vertex;     // The vertex data used for rendering
extern vector<GLuint> data_faces;       // The faces data used for rendering
extern vector<GLuint> data_edges;       // The edges data used for rendering
extern void UpdateMeshBufferData();

namespace decimation {

// Calculate K matrix for one face
void CalculateFaceK(smfparser::Face *face);

// Calculate K matrix for each face
void CalculateAllFaceK(void);

// Calculate Q matrix for one vertex
void CalculateVertexQ(smfparser::Vertex *vertex);

// Calculate Q matrix for each vertex
void CalculateAllVertexQ(void);

// Calculate vertex for one pair
glm::vec4 CalculatePairVertex(smfparser::W_edge *pair);

// Calculate cost for one pair
float CalculatePairCost(smfparser::W_edge *pair);

// Get the pair that need collapse using Multiple-Choice algorithm
smfparser::W_edge *MultipleChoice(int k);

// Do edge contraction on specific pair
void EdgeContractionOnPair(smfparser::W_edge *pair);

// The quadric error matrics decimation using multiple choice scheme
void QuadricMatricsDecimation(int k, int target);

// Update mesh data for rendering
void UpdateRenderMeshData(void);

} // namespace decimation

#endif //SMFVIEW_DECIMATION_H_
