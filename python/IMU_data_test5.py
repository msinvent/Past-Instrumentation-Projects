from __future__ import division
from visual import *
import serial
import math
import time

ser = serial.Serial('com19', 38400, timeout=1)

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
    dcm[1] = vector3d_add(dcm[0],scaled_dcm_0);
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


dcmGyro =[[1,0,0],
          [0,1,0],
          [0,0,1]];
interval_ms = 20;
Kacc=[1,1,1];
Imag=[1,1,1];
w=[0,0,0];

ACC_WEIGHT = 0.02;
MAG_WEIGHT = 0.5;

counter=0;

while 1:
    counter+=1;
    rate(50);
    t=time.time();
    Kacc= [0,0,1]    
    Kacc=vector3d_normalize(Kacc);
    #calculate correction vector to bring dcmGyro's k vector closer to Acc vectr
    wA = vector3d_cross(dcmGyro[2],Kacc)
    #in the absence of magnetometer let's asssume North vector(I) is always in XZ
    #plane of the device ( y coordinate is 0)
    Imag[0] = pow((1-dcmGyro[0][2]**2),.5);
    Imag[1] = 0;
    Imag[2] = dcmGyro[0][2];

    wM = vector3d_cross(dcmGyro[0],Imag)

    #--------
    #dcmGyro
    #--------
    #get_Acc_data() in w[3]
    w = [0,0,0];
    for i in range(0,3):
        w[i] *= .02;    #scaled by elapsed time to get angles in radians
        w[i] = (w[i] + ACC_WEIGHT*wA[i] + MAG_WEIGHT*wM[i])/(1.0+ACC_WEIGHT+MAG_WEIGHT);

    dcmGyro=dcm_rotate(dcmGyro,w);
    if(counter%50 == 0):
        print dcmGyro[0][0],dcmGyro[0][1],dcmGyro[0][2],'\n',dcmGyro[1][0],dcmGyro[1][1],dcmGyro[1][2],'\n',dcmGyro[2][0],dcmGyro[2][1],dcmGyro[2][2],'\n\n';

    
