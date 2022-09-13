#include "veins/modules/AI/AIAlgorithms.h"

namespace Veins {

void setupSVR(double SVREpsilon, double SVRC, OnlineSVR *SVR) {
    SVR = new OnlineSVR();
    // Set parameters
    SVR->SetEpsilon(SVREpsilon);
    SVR->SetC(SVRC);
    SVR->SetKernelType(OnlineSVR::KERNEL_RBF);
    SVR->SetAutoErrorTollerance(false);
    SVR->SetVerbosity(OnlineSVR::VERBOSITY_NORMAL);
}

int setupNNConnection() {
    int cliSockDes = 0;
    if ((cliSockDes = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
        perror("socket creation error...\n");
    }
    return cliSockDes;
}

void predictWithSVRAndTrain(double *lastRecordedDelayAtLV,
        double *predictedDelayAtLV, double *PredErrorAtLV,
        double realDelayOfLastVeh, int SVRSizeLimit, OnlineSVR *SVR) {
    Matrix<double> *TestSetX = new Matrix<double>();
    Vector<double> *X1 = new Vector<double>();

    X1->Add(*lastRecordedDelayAtLV);
    TestSetX->AddRowRef(X1);
    *lastRecordedDelayAtLV = realDelayOfLastVeh;
    Vector<double> *y = SVR->Predict(TestSetX);
    *predictedDelayAtLV = y->GetValue(0);
    *PredErrorAtLV = *predictedDelayAtLV - realDelayOfLastVeh;
    SVR->Train(X1, realDelayOfLastVeh);
//  predictedDelayOut.record(predictedDelayAtLV);
//  predictionErrorOut.record(*PredErrorAtLV);
    if (SVR->GetSamplesTrainedNumber() > SVRSizeLimit) {
        SVR->Forget(0);
    }
}

sockaddr_in getNNServerAdd() {

    struct sockaddr_in serAddr;
    //server socket address
    serAddr.sin_family = AF_INET;
    serAddr.sin_port = htons(1234);
    serAddr.sin_addr.s_addr = inet_addr("127.0.0.1");
    return serAddr;
}

void predictWithNN(int cliSockDes, double realDelayOfLastVeh, int *numPrev,
        sockaddr_in serAddr, double *lastRecordedDelayAtLV,
        double *predictedDelayAtLV, double *PredErrorAtLV) {
    char buff[1024] = { 0 };
    sendto(cliSockDes, std::to_string(realDelayOfLastVeh).c_str(),
            std::to_string(realDelayOfLastVeh).size(), 0,
            (struct sockaddr*) &serAddr, sizeof(serAddr));
    (*numPrev)++;
    *lastRecordedDelayAtLV = realDelayOfLastVeh;
    socklen_t serAddrLen = sizeof(serAddr);
    if ((*numPrev) > 10) {
        recvfrom(cliSockDes, buff, 1024, 0, (struct sockaddr*) &serAddr,
                &serAddrLen);
        *predictedDelayAtLV = std::stod(buff);
        *PredErrorAtLV = *predictedDelayAtLV - realDelayOfLastVeh;
        //predictedDelayOut.record(predictedDelayAtLV);
        //predictionErrorOut.record(PredErrorAtLV);
    }
}
}
