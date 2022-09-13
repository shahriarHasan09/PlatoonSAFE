/*
 * AIAlgorithms.h
 *
 *  Created on: Aug 12, 2022
 *      Author: jgorospe
 */

#ifndef SRC_VEINS_MODULES_AI_AIALGORITHMS_H_
#define SRC_VEINS_MODULES_AI_AIALGORITHMS_H_


/****************************SVR-jgorospe****************************/
#include "veins/modules/AI/OnlineSVR/OnlineSVR.h"
#include <iostream>
#include <fstream>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
/********************************************************************/
namespace Veins
{

void setupSVR(double SVREpsilon, double SVRC, OnlineSVR *SVR);

void predictWithSVRAndTrain (double *lastRecordedDelayAtLV, double *predictedDelayAtLV, double *PredErrorAtLV, double realDelayOfLastVeh, int SVRSizeLimit, OnlineSVR *SVR);


int setupNNConnection();
sockaddr_in getNNServerAdd();
void predictWithNN(int cliSockDes, double realDelayOfLastVeh, int *numPrev, sockaddr_in serAddr, double *lastRecordedDelayAtLV, double *predictedDelayAtLV, double *PredErrorAtLV);
}

#endif /* SRC_VEINS_MODULES_AI_AIALGORITHMS_H_ */
