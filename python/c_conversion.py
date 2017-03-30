from __future__ import division
from visual import *
import serial
import math
import time



scene.width = 1040
scene.height = 480
#scene.autoscale = 0

x_axes = box(pos = (10,0,0) ,size = (20,.2,.2),color = color.green)
y_axes = box(pos = (0,0,-10) ,size = (.2,.2,20),color = color.blue)
z_axes = box(pos = (0,10,0) ,size = (.2,20,.2),color = color.white)



IMU_front = box(pos=(0,0,0), size=(2,2,2),color=color.red)
IMU_back = box(pos=(0,0,0), size=(2,2,2),color=color.green)
IMU_right = box(pos=(0,0,0), size=(2,2,2),color=color.blue)
IMU_left = box(pos=(0,0,0), size=(2,2,2),color=color.white)


def vector3d_add(vector1,vector2):
    vector_addition=[0,0,0];
    for i in range(0,3):
        vector_addition[i]=vector1[i]+vector2[i];
    return vector_addition;

def vector3d_sub(vector1,vector2):
    vector_addition=[0,0,0];
    for i in range(0,3):
        vector_addition[i]=vector1[i]-vector2[i];
    return vector_addition;

def vector3d_scale(vector1,scaling_factor):
    vector_addition=[0,0,0];
    for i in range(0,3):
        vector_addition[i]=vector1[i]*scaling_factor;
    return vector_addition;


def vector3d_dot(vector1,vector2):
    dot_product = 0;
    for i in range(0,3):
        dot_product += vector1[i]*vector2[i];
    return dot_product;

def vector3d_cross(vector1,vector2):
    cross_product=[0,0,0];
    cross_product[0]=vector1[1]*vector2[2] - vector1[2]*vector2[1];
    cross_product[1]=vector1[2]*vector2[0] - vector1[0]*vector2[2];
    cross_product[2]=vector1[0]*vector2[1] - vector1[1]*vector2[0];
    return cross_product;

def vector3d_normalize(vector):
    norm_vector=[0,0,0]
    mag_vector = pow((vector[0]**2 + vector[1]**2 + vector[2]**2),.5);
    norm_vector[0] = vector[0]/mag_vector;
    norm_vector[1] = vector[1]/mag_vector;
    norm_vector[2] = vector[2]/mag_vector;
    return norm_vector

def dcm_orthonormalize(dcm):
    err = vector3d_dot(dcm[0],dcm[1]);
    scaled_dcm_0 = vector3d_scale(dcm[0],-err/2);
    scaled_dcm_1 = vector3d_scale(dcm[1],-err/2);
    dcm[0] = vector3d_add(dcm[0],scaled_dcm_1);
    dcm[1] = vector3d_add(dcm[1],scaled_dcm_0);
    #z=X x Y
    dcm[2] = vector3d_cross(dcm[0],dcm[1]);
    #re-normalize
    dcm[0]=vector3d_normalize(dcm[0]);
    dcm[1]=vector3d_normalize(dcm[1]);
    dcm[2]=vector3d_normalize(dcm[2]);
    return dcm;

def dcm_rotate(dcm,vector):
    #update matrix using formula R(t+1) = R(t)+dR(t) = R(t)+ w*R(t)
    for i in range(0,3):
        dr=vector3d_cross(vector,dcm[i]);
        dcm[i]=vector3d_add(dcm[i],dr);
    #make matrix orthonormal again
    dcm_orthonormalize(dcm)
    return dcm

def dcm_transform(dcm,vector):
    vector_2=[0,0,0]
    for i in range(0,3):
        for j in range(0,3):
            vector_2[i] += dcm[i][j]*vector[j];
    return vector_2;

dcmGyro =[[1,1,0],
          [0,1,1],
          [0,0,1]];

interval_ms = 20;
Kacc=[0,0,1];
Imag=[1,1,1];

ACC_WEIGHT = 0.020;
MAG_WEIGHT = .03;
w=[0,0,0];
counter=0;
i=0;
front=[0,0,1];

for i in range(0,1):
    print dcmGyro[0][0],dcmGyro[0][1],dcmGyro[0][2],'\n',dcmGyro[1][0],dcmGyro[1][1],dcmGyro[1][2],'\n',dcmGyro[2][0],dcmGyro[2][1],dcmGyro[2][2],'\n\n'
    dcmGyro=dcm_orthonormalize(dcmGyro);
    dcmGyro=dcm_rotate(dcmGyro,(2,1,2));
    front=dcm_transform(dcmGyro,front)    
    print dcmGyro[0][0],dcmGyro[0][1],dcmGyro[0][2],'\n',dcmGyro[1][0],dcmGyro[1][1],dcmGyro[1][2],'\n',dcmGyro[2][0],dcmGyro[2][1],dcmGyro[2][2],'\n\n'
            #print t,'\n'
    print front[0],front[1],front[2];
            #print Kacc[0],Kacc[1],Kacc[2]
            #print Kacc[0],Kacc[1],Kacc[2],w[0],w[1],w[2],'\n\n';
            #print w[0],w[1],w[2]
