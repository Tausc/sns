#pragma once
#include <vtkVertexGlyphFilter.h>
#include <iostream>
vtkSmartPointer<vtkVertexGlyphFilter> InputDataExtract(std::string filename);