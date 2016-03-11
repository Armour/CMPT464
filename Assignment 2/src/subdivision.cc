//////////////////////////////////////////////////////////////////////////////
//
//  subdivision.cc
//
//  The source file for function that do the subdivision
//
//  Project         : SmfView
//  Name            : Chong Guo
//  Student ID      : 301295753
//  SFU username    : armourg
//  Instructor      : Richard Zhang
//
//  Created by Armour on 2/11/2016
//  Copyright (c) 2016 Armour. All rights reserved.
//
//////////////////////////////////////////////////////////////////////////////

#include "subdivision.h"
#include <iostream>

namespace subdivision {

// Subdivision mesh data
vector<smfparser::Vertex *> old_mesh_vertex;     // The old vertex data that stored in memory
vector<smfparser::Face *> old_mesh_faces;        // The old faces data that stored in memory
map<pair<int, int>, smfparser::W_edge *> old_mesh_edges;        // The old edges data that stored in memory
map<smfparser::Vertex *, GLuint> old_vertex_index_map;          // The old vertex index map

vector<smfparser::Vertex *> new_mesh_vertex;     // The new vertex data that stored in memory
vector<smfparser::Face *> new_mesh_faces;        // The new faces data that stored in memory
map<pair<int, int>, smfparser::W_edge *> new_mesh_edges;        // The new edges data that stored in memory
map<smfparser::Vertex *, GLuint> new_vertex_index_map;          // The new vertex index map

map<pair<int, int>, GLuint> edge_midpoint_index_map;            // The subdivided edge midpoint index map
vector<vector<float>> vertex_input;
vector<vector<int>> face_input;

//
// Function: LoopVertexMaskBeta
// ---------------------------
//
//   Calculate beta for loop vertex mask with k adjacent vertex
//
//   Parameters:
//       k: the number of adjacent vertex
//
//   Returns:
//       void
//

double LoopVertexMaskBeta(int k) {
    double tmp = 0.375 + 0.25 * cos(2 * M_PI / k);
    double beta = (0.625 - tmp * tmp) / k;
    return beta;
}

//
// Function:  ButterflyVertexMaskSj
// ---------------------------
//
//   Calculate Sj for butterfly edge mask with k adjacent vertex
//
//   Parameters:
//       k: the number of adjacent vertex
//
//   Returns:
//       void
//

double ButterflyVertexMaskSj(int k, int j) {
    double tmp = 2 * j * M_PI / k;
    double Sj = (0.25 + cos(tmp) + 0.5 * cos(2 * tmp)) / k;
    return Sj;
}

//
// Function: LoopSubdivision
// ---------------------------
//
//   The function that do the loop subdivision
//
//   Parameters:
//       level: the subdivision recursive level
//
//   Returns:
//       void
//

void LoopSubdivision(int level) {
    subdivision_semaphore = libconsts::kSubdivisionLockOn;

    // Initial loop start state
    old_mesh_vertex = mesh_vertex;
    old_mesh_edges = mesh_edges;
    old_mesh_faces = mesh_faces;
    old_vertex_index_map = vertex_index_map;

    // Start loop subdivision
    for (int i = 0; i < level; i++) {
        double x, y, z;
        int edge_midpoint_count = 0;

        // Clear vertex and face input data
        vertex_input.clear();
        face_input.clear();
        edge_midpoint_index_map.clear();

        // Put updated old vertex to vertex_input
        for (auto vertex : old_mesh_vertex) {

            // Get k value
            int k = 0;
            smfparser::W_edge *e0 = vertex->edge;
            smfparser::W_edge *e1 = old_mesh_edges[make_pair(old_vertex_index_map[vertex->edge->end] + 1,
                                                         old_vertex_index_map[vertex->edge->start] + 1)];
            smfparser::W_edge *edge = e0;
            do {
                k++;
                if (edge->end == vertex)
                    edge = edge->right_prev;
                else
                    edge = edge->left_prev;
            } while (edge != e0 && edge != e1);

            // Calculate new vertex
            double beta = subdivision::LoopVertexMaskBeta(k);
            x = (1 - k * beta) * vertex->x;
            y = (1 - k * beta) * vertex->y;
            z = (1 - k * beta) * vertex->z;
            edge = e0;
            do {
                if (edge->end == vertex) {
                    x += beta * edge->start->x;
                    y += beta * edge->start->y;
                    z += beta * edge->start->z;
                    edge = edge->right_prev;
                } else {
                    x += beta * edge->end->x;
                    y += beta * edge->end->y;
                    z += beta * edge->end->z;
                    edge = edge->left_prev;
                }
            } while (edge != e0 && edge != e1);

            // Push new vertex into vertex_input
            vector<float> tmpVec;
            tmpVec.push_back((float)x);
            tmpVec.push_back((float)y);
            tmpVec.push_back((float)z);
            vertex_input.push_back(tmpVec);
        }

        // Put all edge mid points to vertex_input
        for (auto edge : old_mesh_edges) {
            smfparser::Vertex *st = edge.second->start;
            smfparser::Vertex *ed = edge.second->end;
            smfparser::Vertex *left = edge.second->left_next->end;
            smfparser::Vertex *right = edge.second->right_next->end;
            x = 0.125 * (left->x + right->x) + 0.375 * (st->x + ed->x);
            y = 0.125 * (left->y + right->y) + 0.375 * (st->y + ed->y);
            z = 0.125 * (left->z + right->z) + 0.375 * (st->z + ed->z);

            if (edge_midpoint_index_map.find(make_pair(old_vertex_index_map[ed], old_vertex_index_map[st])) != edge_midpoint_index_map.end()) {
                edge_midpoint_index_map[make_pair(old_vertex_index_map[st], old_vertex_index_map[ed])] =
                        edge_midpoint_index_map.find(make_pair(old_vertex_index_map[ed], old_vertex_index_map[st]))->second;
            } else {
                vector<float> tmpVec;
                tmpVec.push_back((float)x);
                tmpVec.push_back((float)y);
                tmpVec.push_back((float)z);
                vertex_input.push_back(tmpVec);
                edge_midpoint_index_map[make_pair(old_vertex_index_map[st], old_vertex_index_map[ed])] =
                        (unsigned int)old_mesh_vertex.size() + edge_midpoint_count++;
            }
        }

        // Put all faces data to face_input
        for (auto face : old_mesh_faces) {
            vector<int> tmpVec;
            int v1 = old_vertex_index_map[face->edge->start];
            int v2 = old_vertex_index_map[face->edge->end];
            int v3 = old_vertex_index_map[face->edge->left_next->end];
            int m1 = edge_midpoint_index_map[make_pair(v1, v2)];
            int m2 = edge_midpoint_index_map[make_pair(v2, v3)];
            int m3 = edge_midpoint_index_map[make_pair(v3, v1)];
            tmpVec = {v1 + 1, m1 + 1, m3 + 1}; face_input.push_back(tmpVec);
            tmpVec = {m1 + 1, v2 + 1, m2 + 1}; face_input.push_back(tmpVec);
            tmpVec = {m1 + 1, m2 + 1, m3 + 1}; face_input.push_back(tmpVec);
            tmpVec = {m3 + 1, m2 + 1, v3 + 1}; face_input.push_back(tmpVec);
        }

        // Update new mesh data from vertex_input and face_input
        UpdateWingedEdgeMeshData();

        // Continue loop
        old_mesh_vertex = new_mesh_vertex;
        old_mesh_edges = new_mesh_edges;
        old_mesh_faces = new_mesh_faces;
        old_vertex_index_map = new_vertex_index_map;
    }

    // Update render mesh and its buffer data
    UpdateRenderMeshData();
    UpdateMeshBufferData();

    // Semaphore set to off
    subdivision_semaphore = libconsts::kSubdivisionLockOff;
    cout << "Loop Subdivision Level " << level << " Finished!   " <<
            "Vertex: " << old_mesh_vertex.size() << "  Faces: " << old_mesh_faces.size() << endl;
}

//
// Function: ButterflySubdivision
// ---------------------------
//
//   The function that do the butterfly subdivision
//
//   Parameters:
//       level: the subdivision recursive level
//
//   Returns:
//       void
//

void ButterflySubdivision(int level) {
    subdivision_semaphore = libconsts::kSubdivisionLockOn;

    // Initial butterfly start state
    old_mesh_vertex = mesh_vertex;
    old_mesh_edges = mesh_edges;
    old_mesh_faces = mesh_faces;
    old_vertex_index_map = vertex_index_map;

    // Start butterfly subdivision
    for (int i = 0; i < level; i++) {
        double x, y, z;
        int edge_midpoint_count = 0;

        // Clear vertex and face input data
        vertex_input.clear();
        face_input.clear();
        edge_midpoint_index_map.clear();

        // Put all old vertex to vertex_input
        for (auto vertex : old_mesh_vertex) {
            vector<float> tmpVec;
            tmpVec.push_back(vertex->x);
            tmpVec.push_back(vertex->y);
            tmpVec.push_back(vertex->z);
            vertex_input.push_back(tmpVec);
        }

        // Put all edge mid points to vertex_input
        for (auto edge : old_mesh_edges) {
            smfparser::Vertex *st = edge.second->start;
            smfparser::Vertex *ed = edge.second->end;
            smfparser::Vertex *left = edge.second->left_next->end;
            smfparser::Vertex *right = edge.second->right_next->end;
            smfparser::Vertex *leftUp = edge.second->left_next->right_next->end;
            smfparser::Vertex *rightUp = edge.second->right_prev->right_next->end;
            smfparser::Vertex *leftDown = edge.second->left_prev->right_next->end;
            smfparser::Vertex *rightDown = edge.second->right_next->right_next->end;
            smfparser::W_edge *e0;
            smfparser::W_edge *e1;

            // Get k1 value
            int k1 = 0;
            e0 = st->edge;
            e1 = old_mesh_edges[make_pair(old_vertex_index_map[st->edge->end] + 1, old_vertex_index_map[st->edge->start] + 1)];
            smfparser::W_edge *p = e0;
            do {
                k1++;
                if (p->end == st)
                    p = p->right_prev;
                else
                    p = p->left_prev;
            } while (p != e0 && p != e1);

            // Get k2 value
            int k2 = 0;
            e0 = ed->edge;
            e1 = old_mesh_edges[make_pair(old_vertex_index_map[ed->edge->end] + 1, old_vertex_index_map[ed->edge->start] + 1)];
            p = e0;
            do {
                k2++;
                if (p->end == ed)
                    p = p->right_prev;
                else
                    p = p->left_prev;
            } while (p != e0 && p != e1);

            // Add new edge midpoint
            if (edge_midpoint_index_map.find(make_pair(old_vertex_index_map[ed], old_vertex_index_map[st]))
                != edge_midpoint_index_map.end()) {         // If this edge's inverse edge has alrady added to vertex_input
                edge_midpoint_index_map[make_pair(old_vertex_index_map[st], old_vertex_index_map[ed])] =
                        edge_midpoint_index_map.find(make_pair(old_vertex_index_map[ed], old_vertex_index_map[st]))->second;
            } else {        // Calculate and add this new edge
                x = 0;
                y = 0;
                z = 0;
                if (k1 == 6 && k2 == 6) {               // Regular cases
                    x = 0.125 * (left->x + right->x) + 0.5 * (st->x + ed->x) - 0.0625 * (leftUp->x + leftDown->x + rightUp->x + rightDown->x);
                    y = 0.125 * (left->y + right->y) + 0.5 * (st->y + ed->y) - 0.0625 * (leftUp->y + leftDown->y + rightUp->y + rightDown->y);
                    z = 0.125 * (left->z + right->z) + 0.5 * (st->z + ed->z) - 0.0625 * (leftUp->z + leftDown->z + rightUp->z + rightDown->z);
                } else if (k1 != 6) {               // Extraordinary cases
                    if (k1 > 4) {
                        int j = 0;
                        double q = 1.0;
                        e0 = edge.second;
                        e1 = old_mesh_edges[make_pair(old_vertex_index_map[ed] + 1, old_vertex_index_map[st] + 1)];
                        p = e0;
                        do {
                            double Sj = ButterflyVertexMaskSj(k1, j);
                            if (p->end == st) {
                                x += Sj * p->start->x;
                                y += Sj * p->start->y;
                                z += Sj * p->start->z;
                                p = p->right_prev;
                            } else {
                                x += Sj * p->end->x;
                                y += Sj * p->end->y;
                                z += Sj * p->end->z;
                                p = p->left_prev;
                            }
                            q = q - Sj;
                            j++;
                        } while (p != e0 && p != e1);
                        x += q * st->x;
                        y += q * st->y;
                        z += q * st->z;
                    } else if (k1 == 4) {           // If k1 is 4
                        x = 0.375 * ed->x - 0.125 * leftDown->x + 0.75 * st->x;
                        y = 0.375 * ed->y - 0.125 * leftDown->y + 0.75 * st->y;
                        z = 0.375 * ed->z - 0.125 * leftDown->z + 0.75 * st->z;
                    } else if (k1 == 3) {           // If k1 is 3
                        x = 5.0 / 12.0 * ed->x - 1.0 / 12.0 * (left->x + right->x) + 0.75 * st->x;
                        y = 5.0 / 12.0 * ed->y - 1.0 / 12.0 * (left->y + right->y) + 0.75 * st->y;
                        z = 5.0 / 12.0 * ed->z - 1.0 / 12.0 * (left->z + right->z) + 0.75 * st->z;
                    }
                } else {                            // Extraordinary cases
                    if (k2 > 4) {
                        int j = 0;
                        double q = 1.0;
                        e0 = old_mesh_edges[make_pair(old_vertex_index_map[ed] + 1, old_vertex_index_map[st] + 1)];
                        e1 = edge.second;
                        p = e0;
                        do {
                            double Sj = ButterflyVertexMaskSj(k2, j);
                            if (p->start == ed) {
                                x += Sj * p->end->x;
                                y += Sj * p->end->y;
                                z += Sj * p->end->z;
                                p = p->left_prev;
                            } else {
                                x += Sj * p->start->x;
                                y += Sj * p->start->y;
                                z += Sj * p->start->z;
                                p = p->right_prev;
                            }
                            q = q - Sj;
                            j++;
                        } while (p != e0 && p != e1);
                        x += q * ed->x;
                        y += q * ed->y;
                        z += q * ed->z;
                    } else if (k2 == 4) {           // If k2 is 4
                        x = 0.375 * st->x - 0.125 * leftDown->x + 0.75 * ed->x;
                        y = 0.375 * st->y - 0.125 * leftDown->y + 0.75 * ed->y;
                        z = 0.375 * st->z - 0.125 * leftDown->z + 0.75 * ed->z;
                    } else if (k2 == 3) {           // If k2 is 3
                        x = 5.0 / 12.0 * st->x - 1.0 / 12.0 * (left->x + right->x) + 0.75 * ed->x;
                        y = 5.0 / 12.0 * st->y - 1.0 / 12.0 * (left->y + right->y) + 0.75 * ed->y;
                        z = 5.0 / 12.0 * st->z - 1.0 / 12.0 * (left->z + right->z) + 0.75 * ed->z;
                    }
                }
                vector<float > tmpVec;              // Push new vertex into vertex_input
                tmpVec.push_back((float)x);
                tmpVec.push_back((float)y);
                tmpVec.push_back((float)z);
                vertex_input.push_back(tmpVec);
                edge_midpoint_index_map[make_pair(old_vertex_index_map[st], old_vertex_index_map[ed])] =
                        (unsigned int)old_mesh_vertex.size() + edge_midpoint_count++;
            }
        }

        // Put all faces data to face_input
        for (auto face : old_mesh_faces) {
            vector<int> tmpVec;
            int v1 = old_vertex_index_map[face->edge->start];
            int v2 = old_vertex_index_map[face->edge->end];
            int v3 = old_vertex_index_map[face->edge->left_next->end];
            int m1 = edge_midpoint_index_map[make_pair(v1, v2)];
            int m2 = edge_midpoint_index_map[make_pair(v2, v3)];
            int m3 = edge_midpoint_index_map[make_pair(v3, v1)];
            tmpVec = {v1 + 1, m1 + 1, m3 + 1}; face_input.push_back(tmpVec);
            tmpVec = {m1 + 1, v2 + 1, m2 + 1}; face_input.push_back(tmpVec);
            tmpVec = {m1 + 1, m2 + 1, m3 + 1}; face_input.push_back(tmpVec);
            tmpVec = {m3 + 1, m2 + 1, v3 + 1}; face_input.push_back(tmpVec);
        }

        // Update new mesh data from vertex_input and face_input
        UpdateWingedEdgeMeshData();

        // Continue loop
        old_mesh_vertex = new_mesh_vertex;
        old_mesh_edges = new_mesh_edges;
        old_mesh_faces = new_mesh_faces;
        old_vertex_index_map = new_vertex_index_map;
    }

    // Update render mesh and its buffer data
    UpdateRenderMeshData();
    UpdateMeshBufferData();

    // Semaphore set to off
    subdivision_semaphore = libconsts::kSubdivisionLockOff;
    cout << "Butterfly Subdivision Level " << level << " Finished!   " <<
        "Vertex: " << old_mesh_vertex.size() << "  Faces: " << old_mesh_faces.size() << endl;
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

    for (auto vertex : old_mesh_vertex) {       // Update vertex data for rendering
        data_vertex.push_back(vertex->x);
        data_vertex.push_back(vertex->y);
        data_vertex.push_back(vertex->z);
        data_vertex.push_back((float)(rand() % 100) / 100);
        data_vertex.push_back((float)(rand() % 100) / 100);
        data_vertex.push_back((float)(rand() % 100) / 100);
        data_vertex.push_back(0.0f);
    }

    for (auto face : old_mesh_faces) {      // Update faces data for rendering
        smfparser::W_edge *e0 = face->edge;
        smfparser::W_edge *edge = e0;
        do {
            data_faces.push_back(old_vertex_index_map[edge->start]);
            if (edge->left == face)
                edge = edge->left_next;
            else
                edge = edge->right_next;
        } while (edge != e0);
    }

    for (auto edge : old_mesh_edges) {      // Update edges data for rendering
        data_edges.push_back(old_vertex_index_map[edge.second->start]);
        data_edges.push_back(old_vertex_index_map[edge.second->end]);
    }
}

//
// Function: UpdateWingedEdgeMeshData
// ---------------------------
//
//   Update new mesh data from vertex_input and face_input
//
//   Parameters:
//       void
//
//   Returns:
//       void
//

void UpdateWingedEdgeMeshData() {
    // Clear new mesh data
    new_mesh_vertex.clear();
    new_mesh_edges.clear();
    new_mesh_faces.clear();

    // Update all vertex
    for (auto vertex : vertex_input) {
        smfparser::Vertex *new_vertex = new smfparser::Vertex(vertex[0], vertex[1], vertex[2]);      // Add new vertex
        new_mesh_vertex.push_back(new_vertex);
        new_vertex_index_map[new_vertex] = (GLuint)(new_mesh_vertex.size() - 1);
    }

    // Update all faces
    for (auto face : face_input) {
        smfparser::Face *new_face = new smfparser::Face();          // Add new face
        new_mesh_faces.push_back(new_face);
        int size = (int)face.size();
        for (int i = 0; i < size; i++) {    // Add all edges around this face
            smfparser::Vertex *st = new_mesh_vertex[face[i] - 1];
            smfparser::Vertex *ed = new_mesh_vertex[face[(i + 1) % size] - 1];
            smfparser::W_edge *new_edge = new smfparser::W_edge(st, ed);        // Add new edge from st to ed
            pair<int, int> p(face[i], face[(i + 1) % size]);
            new_mesh_edges[p] = new_edge;
            new_face->edge = new_edge;      // Choose any adjacent edge
            new_edge->left = new_face;      // Edge left face
            st->edge = new_edge;            // Choose any adjacent edge
        }
        for (int i = 0; i < size; i++) {    // Update all edges data around this face
            pair<int, int> p(face[i], face[(i + 1) % size]);
            pair<int, int> p_prev(face[(i - 1 + size) % size], face[i]);
            pair<int, int> p_next(face[(i + 1) % size], face[(i + 2) % size]);
            pair<int, int> p_reverse(face[(i + 1) % size], face[i]);
            smfparser::W_edge *edge = new_mesh_edges.find(p)->second;               // This edge
            smfparser::W_edge *edge_prev = new_mesh_edges.find(p_prev)->second;     // The previous edge
            smfparser::W_edge *edge_next = new_mesh_edges.find(p_next)->second;     // The next edge
            edge->left_prev = edge_prev;
            edge->left_next = edge_next;
            if (new_mesh_edges.find(p_reverse) != new_mesh_edges.end()) {       // Update edge information according to its reverse
                smfparser::W_edge *edge2 = new_mesh_edges.find(p_reverse)->second;
                edge->right = edge2->left;
                edge->right_prev = edge2->left_prev;
                edge->right_next = edge2->left_next;
                edge2->right = edge->left;
                edge2->right_prev = edge->left_prev;
                edge2->right_next = edge->left_next;
            }
        }
    }
}

} // namespace subdivision
