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

const int kWindowPositionX = 0;
const int kWindowPositionY = 0;
const int kWindowSizeWidth = 1000;
const int kWindowSizeHeight = 500;

const int kMaxVertexNum = 100000;
const int kMaxFaceNum = 200000;
const int kMaxEdgeNum = 300000;

const int kFlagVertex = 0;
const int kFlagFace = 1;

const int kImportLockOff = 0;
const int kImportLockOn = 1;
const int kExportLockOff = 0;
const int kExportLockOn = 1;

const int kMeshImportedFalse = 0;
const int kMeshImportedTrue = 1;

const int kMeshTypeFlatShaded = 0;
const int kMeshTypeSmoothShaded = 1;
const int kMeshTypeWireFrame = 2;
const int kMeshTypeShadedEdges = 3;

const int kElementVecLenPosition = 3;
const int kElementVecLenColor = 4;
const int kElementVecLenTotal = 7;

} // namespace libconsts

#endif // SMFVIEW_LIB_CONSTS_H_
