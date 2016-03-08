//////////////////////////////////////////////////////////////////////////////
//
//  decimation.cc
//
//  The source file for function that do the decimation to mesh data
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

#include "decimation.h"
#include <iostream>

namespace decimation {

//
// Function:  CalculateAllFaceK
// ---------------------------
//
//   Calculate K matrix for each face
//
//   Parameters:
//       void
//
//   Returns:
//       void
//

void CalculateAllFaceK(void) {
    for (auto face: mesh_faces) {               // For each face
        glm::vec3 v1 = glm::vec3(face->edge->start->x, face->edge->start->y, face->edge->start->z);
        glm::vec3 v2 = glm::vec3(face->edge->end->x, face->edge->end->y, face->edge->end->z);
        glm::vec3 v3 = glm::vec3(face->edge->left_next->end->x, face->edge->left_next->end->y, face->edge->left_next->end->z);
        glm::vec3 cross = glm::normalize(glm::cross(v3 - v1, v2 - v1));         // Get the normal of the plane
        float a = cross.x;
        float b = cross.y;
        float c = cross.z;
        float d = -(v1.x * a + v1.y * b + v1.z * c);
        face->K = new glm::mat4x4(              // Get K matrix
                a * a, a * b, a * c, a * d,
                a * b, b * b, b * c, b * d,
                a * c, b * c, c * c, c * d,
                a * d, b * d, c * d, d * d
        );
        //std::cout << a << " " << b << " " << c << " " << d << std::endl;
        /*for (int i = 0; i < 4; i++) {
            for (int j = 0; j < 4; j++) {
                std::cout << (*(face->K))[i][j] << " ";
            }
            std::cout << std::endl;
        }
        std::cout << std::endl;*/
    }
}

//
// Function:  CalculateAllVertexQ
// ---------------------------
//
//   Calculate Q matrix for each vertex
//
//   Parameters:
//       void
//
//   Returns:
//       void
//

void CalculateAllVertexQ(void) {
    for (auto vertex: mesh_vertex) {                // For each vertex
        vertex->Q = new glm::mat4x4(0.0);
        smfparser::W_edge *e0 = vertex->edge;
        smfparser::W_edge *e1 = mesh_edges[make_pair(vertex_index_map[vertex->edge->end] + 1, vertex_index_map[vertex->edge->start] + 1)];
        smfparser::W_edge *edge = e0;
        do {
            if (edge->end == vertex)
                edge = edge->right_prev;
            else
                edge = edge->left_prev;
            *(vertex->Q) = *(vertex->Q) + *(edge->left->K);
        } while (edge != e0 && edge != e1);
        /*for (int i = 0; i < 4; i++) {
            for (int j = 0; j < 4; j++) {
                std::cout << (*(vertex->Q))[i][j] << " ";
            }
            std::cout << std::endl;
        }
        std::cout << std::endl;*/
    }
}

//
// Function:  QuadricMatricsDecimation
// ---------------------------
//
//   The quadric error matrics decimation using multiple choice scheme
//
//   Parameters:
//       k: the number of random choosed pairs
//       target: the target edge number
//
//   Returns:
//       void
//

void QuadricMatricsDecimation(int k, int target) {
    CalculateAllFaceK();
    CalculateAllVertexQ();
    while (target > mesh_edges.size() / 2) {
        // RandomChooseKPairs(k);
        // CollpasePair();
    }
}

} // namespace decimation

