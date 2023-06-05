#pragma once

#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <fstream>
#include <vector>
#include <math.h>
#include "../../common/headers/Shape.h"
#include "../../common/headers/Point.h"
#include <sys/stat.h>

void geraBezier(char *filePath, int tesselation, char *dot3DFile);
