# This file should be viewed as a -*- mode: Makefile -*-
# ########################################
# The goal part: what to make
# ########################################
TEMPLATE = app
CONFIG = console \
    glut \ 
	debug_and_release\
	

INCLUDEPATH += .
HEADERS = matrix.h \
    traqueboule.h \
    Vec3D.h \
    mesh.h \
	Vertex.h\
	
SOURCES = main.cpp \
    mesh.cpp \

	