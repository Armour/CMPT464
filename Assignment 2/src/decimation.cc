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
// Function:  CalculateFaceK
// ---------------------------
//
//   Calculate K matrix for one face
//
//   Parameters:
//       face: the face we need to calculate K matrix
//
//   Returns:
//       void
//

void CalculateFaceK(smfparser::Face *face) {
    if (face->render_flag == 1) return;
    smfparser::W_edge *edge = face->edge;
    if (edge->left != face) edge = edge->right_next->left_prev;
    glm::vec3 v1 = glm::vec3(edge->start->x, edge->start->y, edge->start->z);
    glm::vec3 v2 = glm::vec3(edge->end->x, edge->end->y, edge->end->z);
    glm::vec3 v3 = glm::vec3(edge->left_next->end->x, edge->left_next->end->y, edge->left_next->end->z);
    glm::vec3 cross = glm::normalize(glm::cross(v3 - v1, v2 - v1));         // Get the normal of the plane
    float a = cross.x;
    float b = cross.y;
    float c = cross.z;
    float d = -(v1.x * a + v1.y * b + v1.z * c);
    if (face->K != nullptr) delete(face->K);
    face->K = new glm::mat4x4(              // Get K matrix
            a * a, a * b, a * c, a * d,
            a * b, b * b, b * c, b * d,
            a * c, b * c, c * c, c * d,
            a * d, b * d, c * d, d * d
    );
    /*
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            std::cout << (*face->K)[i][j] << " ";
        }
        std::cout << std::endl;
    }
    std::cout << std::endl;
    */
}

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
        if (face->render_flag == 1) return;
        CalculateFaceK(face);
    }
}

//
// Function:  CalculateVertexQ
// ---------------------------
//
//   Calculate Q matrix for one vertex
//
//   Parameters:
//       vertex: the vertex we need to calculate Q matrix
//
//   Returns:
//       void
//

void CalculateVertexQ(smfparser::Vertex *vertex) {
    if (vertex->render_flag == 1) return;
    if (vertex->Q != nullptr) delete(vertex->Q);
    vertex->Q = new glm::mat4x4(0.0);
    smfparser::W_edge *e0 = vertex->edge;
    smfparser::W_edge *e1 = mesh_edges[make_pair(vertex_index_map[vertex->edge->end] + 1,
                                                 vertex_index_map[vertex->edge->start] + 1)];
    smfparser::W_edge *edge = e0;
    do {
        if (edge->end == vertex) {
            *(vertex->Q) = *(vertex->Q) + *(edge->left->K);
            edge = edge->right_prev;
        } else {
            *(vertex->Q) = *(vertex->Q) + *(edge->right->K);
            edge = edge->left_prev;
        }
    } while (edge != e0 && edge != e1);
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
        if (vertex->render_flag == 1) return;
        CalculateVertexQ(vertex);
    }
}

//
// Function:  CalculatePairVertex
// ---------------------------
//
//   Calculate vertex for one pair
//
//   Parameters:
//       pair: the pair the we need to calculate vertex
//
//   Returns:
//       the vertex of this pair
//

glm::vec4 CalculatePairVertex(smfparser::W_edge *pair) {
    glm::mat4x4 Q = *(pair->start->Q) + *(pair->end->Q);
    glm::mat4x4 tmp = Q;
    tmp[3] = glm::vec4(0.0f, 0.0f, 0.0f, 1.0f);
    glm::mat4x4 inverseQ = glm::inverse(tmp);
    if (glm::isnan(inverseQ[0][0])) {
        std::cout << "Got nan !!!!!!!!!!!!!!!!" << std::endl;
        glm::vec4 v1 = glm::vec4(pair->start->x, pair->start->y, pair->start->z, 1.0);
        glm::vec4 v2 = glm::vec4(pair->end->x, pair->end->y, pair->end->z, 1.0);
        glm::vec4 vm = glm::vec4((v1.x + v2.x) / 2, (v1.y + v2.y) / 2, (v1.z + v2.z) / 2, 1.0);
        glm::vec4 costv1 = v1 * Q * v1;
        glm::vec4 costv2 = v2 * Q * v2;
        glm::vec4 costvm = vm * Q * vm;
        float cost1 = costv1.x + costv1.y + costv1.z + costv1.w;
        float cost2 = costv2.x + costv2.y + costv2.z + costv2.w;
        float costm = costvm.x + costvm.y + costvm.z + costvm.w;
        if (cost1 <= cost2 && cost1 <= costm) return costv1;
        if (cost2 <= cost1 && cost2 <= costm) return costv2;
        return costvm;
    } else {
        return glm::vec4(0.0f, 0.0f, 0.0f, 1.0f) * inverseQ;
    }
 }

//
// Function:  CalculatePairCost
// ---------------------------
//
//   Calculate cost for one pair
//
//   Parameters:
//       pair: the pair the we need to calculate cost
//
//   Returns:
//       void
//

void CalculatePairCost(smfparser::W_edge *pair) {
    glm::mat4x4 Q = *(pair->start->Q) + *(pair->end->Q);
    glm::vec4 v = CalculatePairVertex(pair);
    std::cout << "v:" << std::endl;
    std::cout << v.x << " " << v.y << " " << v.z << " " << v.w << std::endl;
    std::cout << "Q:" << std::endl;
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            std::cout << Q[i][j] << " ";
        }
        std::cout << std::endl;
    }
    std::cout << "cost:" << std::endl;
    glm::vec4 costv = v * Q * v;
    std::cout << costv.x << " " << costv.y << " " << costv.z << " " << costv.w << std::endl;
    std::cout << std::endl;
    pair->cost = costv.x + costv.y + costv.z + costv.w;
}

//
// Function:  MultipleChoice
// ---------------------------
//
//   Get the pair that need collapse using Multiple-Choice algorithm
//
//   Parameters:
//       k: the number of random chosen pairs
//
//   Returns:
//       The pair with the least cost in chosen pairs
//

smfparser::W_edge *MultipleChoice(int k) {
    // Initialize the time seed
    srand((unsigned int)time(0));

    // Clamp k value
    if (k > mesh_edges.size())
        k = mesh_edges.size();

    // Multiple Choice
    vector<smfparser::W_edge *> chosen_pairs;
    smfparser::W_edge *edge;
    for (int i = 0; i < k; i++) {
        int r1, r2;
        int time = 0;
        do {
            time++;
            if (time >= 50000000) break;
            r1 = rand() % mesh_vertex.size() + 1;
            r2 = rand() % mesh_vertex.size() + 1;
            if (r1 == r2) continue;
            if (mesh_edges.find(make_pair(r1, r2)) != mesh_edges.end()) {
                edge = mesh_edges[make_pair(r1, r2)];
                if (edge->left_prev->start == edge->right_next->end) continue;
                if (edge->left_prev->right_next->end == edge->left_next->right_next->end) continue;
                if (edge->right_prev->right_next->end == edge->right_next->right_next->end) continue;
                break;
            }
        } while (true);
        if (time != 50000000) {
            CalculatePairCost(edge);
            chosen_pairs.push_back(edge);
        }
    }

    // Get least cost
    smfparser::W_edge *least_cost_pair;
    if (chosen_pairs.empty()) return nullptr;
    std::cout << "Size: " << chosen_pairs.size() << std::endl;
    least_cost_pair = chosen_pairs[0];
    for (int i = 1; i < chosen_pairs.size(); i++) {
        if (chosen_pairs[i]->cost < least_cost_pair->cost)
            least_cost_pair = chosen_pairs[i];
        std::cout << chosen_pairs[i]->cost << " ";
    }
    std::cout << std::endl;
    std::cout << least_cost_pair->cost << std::endl;

    return least_cost_pair;
}

//
// Function:  EdgeContractionOnPair
// ---------------------------
//
//   Do edge contraction on specific pair
//
//   Parameters:
//       pair: the pair that we need to do edge contraction
//
//   Returns:
//       void
//

void EdgeContractionOnPair(smfparser::W_edge *pair) {
    smfparser::Vertex *v1 = pair->start;
    smfparser::Vertex *v2 = pair->end;
    smfparser::Vertex *v_up = pair->left_prev->start;
    smfparser::Vertex *v_down = pair->right_next->end;

    std::cout << vertex_index_map[pair->start] + 1 << "->" << vertex_index_map[pair->end] + 1 << std::endl;

    int v1_index = vertex_index_map[v1] + 1;
    int v2_index = vertex_index_map[v2] + 1;
    int v_up_index = vertex_index_map[v_up] + 1;
    int v_down_index = vertex_index_map[v_down] + 1;
    glm::vec4 v = CalculatePairVertex(pair);

    // Get the edges that need to update prev and next before change them
    smfparser::W_edge *edgel0 = pair->left_prev->right_next->left_prev;
    smfparser::W_edge *edgel1 = pair->right_next->right_next->left_prev;
    smfparser::W_edge *edger0 = pair->left_next->right_next->left_prev;
    smfparser::W_edge *edger1 = pair->right_prev->right_next->left_prev;

    // Update v1 position to v
    v1->x = v.x;
    v1->y = v.y;
    v1->z = v.z;

    // Update vertex edge
    v1->edge = edgel0;
    v_up->edge = edger0;
    v_down->edge = edgel1;

    // Update edge face
    edgel0->right = edger0->left;
    edgel1->right = edger1->left;
    edger0->right = edgel0->left;
    edger1->right = edgel1->left;

    // Erase deleted face
    pair->left->render_flag = 1;
    pair->right->render_flag = 1;

    // Erase deleted vertex
    v2->render_flag = 1;

    // Update edges vertex
    smfparser::W_edge *e0 = mesh_edges[make_pair(v2_index, v1_index)];
    smfparser::W_edge *e1 = pair;
    smfparser::W_edge *edge = e0;
    do {
        if (edge->end == v2) {
            smfparser::W_edge *next_edge = edge->right_prev;
            edge->end = v1;
            edge->right_next->left_prev->start = v1;
            int start_index = vertex_index_map[edge->start] + 1;
            int end_index = vertex_index_map[edge->end] + 1;
            if (start_index == v_down_index) {
                mesh_edges[make_pair(end_index, start_index)] = edge->right_next->left_prev;
            } else if (start_index == v_up_index) {
                mesh_edges[make_pair(start_index, end_index)] = edge;
            } else {
                mesh_edges[make_pair(start_index, end_index)] = edge;
                mesh_edges[make_pair(end_index, start_index)] = edge->right_next->left_prev;
            }
            mesh_edges.erase(make_pair(start_index, v2_index));
            mesh_edges.erase(make_pair(v2_index, start_index));
            edge = next_edge;
        } else {
            mesh_edges.erase(make_pair(v1_index, v2_index));
            mesh_edges.erase(make_pair(v2_index, v1_index));
            edge = edge->left_prev;
        }
    } while (edge != e0 && edge != e1);

    // Update edges' prev and next
    edgel0->right_next = edger0->left_next;
    edgel0->right_prev = edger0->left_prev;
    edgel1->right_next = edger1->left_next;
    edgel1->right_prev = edger1->left_prev;
    edger0->right_next = edgel0->left_next;
    edger0->right_prev = edgel0->left_prev;
    edger1->right_next = edgel1->left_next;
    edger1->right_prev = edgel1->left_prev;

    // Update face's edge
    edgel0->left->edge = edgel0;
    edgel1->left->edge = edgel1;
    edger0->left->edge = edger0;
    edger1->left->edge = edger1;

    // Update face K matrix and vertex Q matrix

    e0 = v1->edge;
    e1 = mesh_edges[make_pair(vertex_index_map[v1->edge->end] + 1, vertex_index_map[v1->edge->start] + 1)];
    edge = e0;
    do {
        std::cout << "!?" << vertex_index_map[edge->start] + 1 << " to " << vertex_index_map[edge->end] + 1 << std::endl;
        if (edge->end == v1) {
            CalculateFaceK(edge->left);
            CalculateVertexQ(edge->start);
            edge = edge->right_prev;
        } else {
            CalculateFaceK(edge->right);
            CalculateVertexQ(edge->end);
            edge = edge->left_prev;
        }
    } while (edge != e0 && edge != e1);
    CalculateVertexQ(v1);
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
    for (int i = 0; i < target; i++) {
        if (mesh_edges.size() / 2 <= 6) break;
        smfparser::W_edge *pair = MultipleChoice(k);
        if (pair == nullptr) continue;
        EdgeContractionOnPair(pair);
    }
    UpdateRenderMeshData();
    UpdateMeshBufferData();
}

//
// Function: UpdateRenderMeshData
// ---------------------------
//
//   Update mesh data for rendering
//
//   Parameters:
//       void
//
//   Returns:
//       void
//

void UpdateRenderMeshData() {
    data_vertex.clear();    // Clear mesh data for rendering
    data_faces.clear();
    data_edges.clear();

    for (auto vertex : mesh_vertex) {       // Update vertex data for rendering
        data_vertex.push_back(vertex->x);
        data_vertex.push_back(vertex->y);
        data_vertex.push_back(vertex->z);
        data_vertex.push_back((float)(rand() % 100) / 100);
        data_vertex.push_back((float)(rand() % 100) / 100);
        data_vertex.push_back((float)(rand() % 100) / 100);
        data_vertex.push_back(0.0f);
    }

    for (auto face : mesh_faces) {      // Update faces data for rendering
        if (face->render_flag != 1) {
            smfparser::W_edge *e0 = face->edge;
            smfparser::W_edge *edge = e0;
            do {
                data_faces.push_back(vertex_index_map[edge->start]);
                if (edge->left == face)
                    edge = edge->left_next;
                else
                    edge = edge->right_next;
            } while (edge != e0);
        }
    }

    for (auto edge : mesh_edges) {      // Update edges data for rendering
        data_edges.push_back(vertex_index_map[edge.second->start]);
        data_edges.push_back(vertex_index_map[edge.second->end]);
    }
}

} // namespace decimation

