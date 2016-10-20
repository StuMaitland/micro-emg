#include "needle.h"


QVector<double> Needle::orderChannels(QVector<double> orig){
    QVector<double> results;

    int* order= new int[orig.size()];

    int ch64Order[]={16,15,14,13,12,11,10,9,8,7,6,5,4,3,2,1,18,17,20,19,22,21,24,23,26,25,28,27,30,29,32,31,34,33,64,63,62,61,60,59,58,57,56,55,54,53,52,51,50,49,48,47,46,45,44,43,42,41,40,39,38,37,36,35};
    int ch48Order[]={12,11,10,9,8,7,6,5,4,3,2,1,14,13,16,15,18,17,20,19,22,21,24,23,26,25,28,27,30,29,32,31,34,33,64,63,62,61,60,59,58,57,56,55,54,53,52,51};
    int ch32Order[]={8,7,6,5,4,3,2,1,10,9,12,11,14,13,16,15,18,17,20,19,22,21,24,23,26,25,28,27,30,29,32,31};

    switch(orig.size()){
    case 64: order = ch64Order;
        break;
    case 48: order = ch48Order;
        break;
    case 32: order = ch32Order;
        break;
    }

    int i;

    for(i=0;i<orig.size();i++){
        results<<orig.at(order[i]-1);
    }
    return results;
}


