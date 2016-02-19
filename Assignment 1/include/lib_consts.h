//////////////////////////////////////////////////////////////////////////////
//
//  lib_consts.h
//
//  The header file for all global constants
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

#ifndef SMFVIEW_LIB_CONSTS_H_
#define SMFVIEW_LIB_CONSTS_H_

namespace libconsts {

// Windows property
const int kWindowPositionX = 0;
const int kWindowPositionY = 0;
const int kWindowSizeWidth = 1000;
const int kWindowSizeHeight = 650;

// Max number of vertex, faces and edges
const int kMaxVertexNum = 200000;
const int kMaxFaceNum = 400000;
const int kMaxEdgeNum = 600000;

// Subdivision type
const int kSubdivisionLoop = 0;
const int kSubdivisionButterfly = 1;

// Flag indicate input is vertex or face data
const int kFlagVertex = 0;
const int kFlagFace = 1;

// Import and export semaphore
const int kImportLockOff = 0;
const int kImportLockOn = 1;
const int kExportLockOff = 0;
const int kExportLockOn = 1;

// Subdivision semaphore
const int kSubdivisionLockOff = 0;
const int kSubdivisionLockOn = 1;

// Import mesh flag
const int kMeshImportedFalse = 0;
const int kMeshImportedTrue = 1;

// Mesh type
const int kMeshTypeFlatShaded = 0;
const int kMeshTypeSmoothShaded = 1;
const int kMeshTypeWireFrame = 2;
const int kMeshTypeShadedEdges = 3;

// Bind buffer element vector length
const int kElementVecLenPosition = 3;
const int kElementVecLenColor = 4;
const int kElementVecLenTotal = 7;

} // namespace libconsts

#endif // SMFVIEW_LIB_CONSTS_H_
