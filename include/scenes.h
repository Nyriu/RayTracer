#ifndef SCENES_H
#define SCENES_H

#include <vector>
#include <memory>

#include "ImplicitShape.h"
#include "Light.h"

std::vector<std::shared_ptr<ImplicitShape>> makeShapes();
std::vector<std::shared_ptr<Light>> makeLights();

#endif
