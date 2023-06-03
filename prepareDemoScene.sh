#! /bin/sh
cd build
make
./generator sphere 1 20 20 sphere.3d
./generator sphereInv 1 20 20 stars.3d
./generator washer 1 1.2 0.0005 20 saturnRing.3d
./generator bezier teapot.patch 10 teapot.3d
./engine SolarSystem.xml
