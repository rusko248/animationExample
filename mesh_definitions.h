//
//  mesh_definitions.h
//  hw_animation
//
//  Created by Pamela Martinez on 5/25/13.
//
//

#ifndef hw_animation_mesh_definitions_h
#define hw_animation_mesh_definitions_h

#include <cassert>
#include <OpenMesh/Core/Mesh/TriMesh_ArrayKernelT.hh>
#include <OpenMesh/Tools/Utils/getopt.h>

struct MyTraits : public OpenMesh::DefaultTraits
{
    HalfedgeAttributes(OpenMesh::Attributes::PrevHalfedge);
};

typedef OpenMesh::TriMesh_ArrayKernelT<MyTraits> Mesh;


#endif
