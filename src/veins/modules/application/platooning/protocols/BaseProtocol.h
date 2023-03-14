//
// Copyright (c) 2012-2018 Michele Segata <segata@ccs-labs.org>
// Edited by Shahriar Hasan and Joseba Gorospe
//
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU Lesser General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU Lesser General Public License for more details.
//
// You should have received a copy of the GNU Lesser General Public License
// along with this program.  If not, see http://www.gnu.org/licenses/.
//

#ifndef BASEPROTOCOL_H_
#define BASEPROTOCOL_H_

#include "veins/base/modules/BaseApplLayer.h"

#include "veins/modules/application/platooning/UnicastProtocol.h"
#include "veins/modules/application/platooning/messages/PlatooningBeacon_m.h"
#include "veins/modules/application/platooning/messages/BrakingPacket_m.h"
#include "veins/modules/application/platooning/messages/ackPkt_m.h"
#include "veins/modules/application/platooning/messages/delayMessage_m.h"
#include "veins/modules/application/platooning/messages/relayMessage_m.h"
#include "veins/modules/application/platooning/messages/relayDENMs_m.h"

#include "veins/modules/mobility/traci/TraCIMobility.h"

#include "veins/modules/application/platooning/utilities/BasePositionHelper.h"

#include "veins/modules/AI/AIAlgorithms.h"

#include <tuple>

// maximum number of upper layer apps that can connect (see .ned file)
#define MAX_GATES_COUNT 10

using namespace Veins;

class BaseProtocol : public Veins::BaseApplLayer {

private:
    // signals for busy channel and collisions
    static const simsignalwrap_t sigChannelBusy;
    static const simsignalwrap_t sigCollision;

    // amount of time channel has been observed busy during the last "statisticsPeriod" seconds
    SimTime busyTime;
    // count the number of collision at the phy layer
    int nCollisions;
    // time at which channel turned busy
    SimTime startBusy;
    // indicates whether channel is busy or not
    bool channelBusy;

    // record the delay between each pair of messages received from leader and car in front
    SimTime lastLeaderMsgTime;
    SimTime lastFrontMsgTime;

    // record the no. of transmissions required to successfully receive a message from the leader and the front vehicle
    int lastLeaderMsgSeqN;
    int lastFrontMsgSeqN;

    // own id for statistics
    cOutVector nodeIdOut;

    // output vectors for busy time and collisions
    cOutVector busyTimeOut, collisionsOut;

    // output vector for delays
    cOutVector leaderDelayIdOut, frontDelayIdOut, leaderDelayOut, frontDelayOut;

    //output vector for no. of retransmissions
    cOutVector leaderTransmissionN, frontTransmissionN;

    /****************************SVR-jgorospe****************************/
    //delay experienced during last beacon interval
    double lastDelay;
    //predicted delay at LV
    double predDelay;
    //delay prediction error
    double predError;
    //last delay experienced by the last platoon vehicle obtained in the LV
    double lastDelayAtLV;
    OnlineSVR* SVR;
    ofstream resultFile;
    //record predicted delays and prediction errors
    cOutVector predictedDelayOut, predictionErrorOut;
    //Variables for predicting with SVR
    int SVRSizeLimit;
    double SVRC, SVREpsilon;

    /********************************************************************/
    //Variables used for predicting delays with SVR and NN
    double lastRecordedDelayAtLV, predictedDelayAtLV, PredErrorAtLV, realDelayOfLastVeh, relayedPredictedDelayAtLV;
    //the mid vehicle in the platoon
    int platoonMidVehicleID;

protected:
        //records the delay of the last vehicle
        double delay_us;
        //packet size of the delay packet to be sent from the last vehicle
        int delayPktSize;
        //Sequence number of delay and DENM messages
        int delay_seq_no, DENM_seq_no;
        //sequence number of last delay message
        int lastDelaySeqNo = -1;
        //last delay message generation time received at the LV
        double lastDelMsgGenTime = 0;
        //DENM generation time
        double DENMGenTime;

        //relay the last vehicle's delay using mid vehicle to be sent to the LV
        double relayedLastVehDelay = 0;
        //sequence number of relay message
        int relayedMsgSeqNo;
        //Delay message generation time
        double delayMsgGenTime = 0;

        //Total CAM delay, Avg CAM delay, relayed avg CAM delay; used for SB
        double SBTotalDelay = 0, SBAvgWaitTime = 0, relayedSBAvgWaitTime = 0;
        //counter used for calculating avg delay
        int SBDelayCounter = 0;



        //Synchronized braking method
        void synchronizedBraking (double SBWaitTime);
        //Enhanced synchronized braking method; the vehicles perform slow deceleration during tau_wait
        void EnhancedSynchronizedBraking (double SBWaitTime);
        //method for adaptive emergency braking
        void adaptiveBraking();
        //method for cooperative emergency brake protocol
        void CooperativeBrakingProtocol();
        //method for calculating avg waiting time for SB. waiting time calculated using Avg CAM delays.
        void calculateSBAvgWaitTime (double SBDelay);
    /*********************** cooperative emergency braking ******************************/
        //braking time due to imaginary road hazard
        SimTime brakeAtTime;
        //deceleration rate during braking
        double decelarationRate;
        //lead vehicle speed during cruising
        double speedOfLeader;
        //fixed waiting time during synchronized braking
        double sbWaitTime;
        //interval of DENM dissemination
        double DENMInterval;
        //braking message to initiate braking
        cMessage* brakingMessage;
        //event driven braking message, i.e., DENM
        void sendBrakingMessage(int destinationAddress);
        //ACK message used for AEB and CEBP
        void sendAckMessage(int destinationAddress);
        //Message used for sending delays experienced by the last vehicle to the LV; used for prediction with ML algorithms
        void sendDelayMessage (int destinationAddress);
        //Message used for relaying the delay messages
        void sendRelayMessage(int destinationAddress);
        //Message used for relaying the DENMs
        void sendDENMRelayMessage(int destinationAddress);
        //counters used for different braking strategies
        int brakingCounter = 0;
        int adaptiveCounter = 0;
        int progCounter = 0;
        //messages for scheduleAt function
        cMessage* synBrkMsg;
        cMessage* synLVBrkMsg;

        // boolean variables to enable or disable the braking strategies
        bool isCoopEmBrkEnabled;
        bool isSynchronizedBraking;
        bool isCEBPEnabled;
        bool isGDEnabled;
        bool isRelayEnabled;
        bool isSBwithAvgDelay;
        bool isAEBEnabled;
        //soft deceleration rate for AEB and ESB strategies
        double softDecelRate;

        //Enhanced synchronized braking
        bool isESynchBrkEnabled;
        int synchCounter = 0;
        cMessage* ESynchMsg;

        //Set used for discarding duplicated DENM or ACK messages
        std::set<int> duplicateSet;
        std::set <int> :: iterator itr;
        std::set<int> ackSet;
        cOutVector firstDENMDelay, seqOutVector;
        cOutVector firstAckDelay;

        /*************messages used for scheduleAt function (AEB and CEBP braking strategies)****************/
        cMessage* startDecelerating0;
        cMessage* startDecelerating1;
        cMessage* startDecelerating2;
        cMessage* startDecelerating3;
        cMessage* startDecelerating4;
        cMessage* startDecelerating5;
        cMessage* startDecelerating6;
        cMessage* startDecelerating7;

        /*************ACK braking messages for AEB and CEBP strategies ****************/
        //cMessage* ackMsg7;
        cMessage* ackMsg6;
        cMessage* ackMsg5;
        cMessage* ackMsg4;
        cMessage* ackMsg3;
        cMessage* ackMsg2;
        cMessage* ackMsg1;

        cMessage* timeOutMsg;
        double CEBPTimeOut;

        cMessage* alagMsg;
        cMessage* leaderAdaptive;
        cMessage* hardDeceleration;

    // determines position and role of each vehicle
    BasePositionHelper* positionHelper;

    // id of this vehicle
    int myId;
    // sequence number of sent messages
    int seq_n;

    int seq_denm;
    // vehicle length
    double length;

    // beaconing interval (i.e., update frequency)
    SimTime beaconingInterval;
    // priority used for messages (i.e., the access category)
    int priority;
    // packet size of the platooning message
    int packetSize;
    // ack packet size for AEB and CEBP strategies
    int ackPacketSize;

    // input/output gates from/to upper layer
    int upperControlIn, upperControlOut, lowerLayerIn, lowerLayerOut;
    // id range of input gates from upper layer
    int minUpperId, maxUpperId, minUpperControlId, maxUpperControlId;

    // registered upper layer applications. this is a mapping between
    // beacon id inside packets coming from upper layer and the gate they
    // the application is connected to. convention: id, from app, to app
    typedef cGate OutputGate;
    typedef cGate InputGate;
    typedef cGate ControlInputGate;
    typedef cGate ControlOutputGate;
    typedef std::tuple<InputGate*, OutputGate*, ControlInputGate*, ControlOutputGate*> AppInOut;
    typedef std::vector<AppInOut> AppList;
    typedef std::map<int, AppList> ApplicationMap;
    ApplicationMap apps;
    // number of gates from the array used
    int usedGates;
    // maps of already existing connections
    typedef cGate ThisGate;
    typedef cGate OtherGate;
    typedef std::map<OtherGate*, ThisGate*> GateConnections;
    GateConnections connections;

    // messages for scheduleAt
    cMessage *sendBeacon, *recordData;

    int cliSockDes, readStatus;
        struct sockaddr_in serAddr;
        bool runSVR;
        bool runNN;
        int numPrev = 0;

    /**
     * NB: this method must be overridden by inheriting classes, BUT THEY MUST invoke the super class
     * method prior processing the message. For example, the start communication event is handled by the
     * BaseProtocol which then calls the startCommunications method. Also statistics are handled
     * by BaseProtocol and are recorder periodically.
     */
    virtual void handleSelfMsg(cMessage* msg) override;

    // TODO: implement method and pass info to upper layer (bogus platooning) as it is (msg)
    virtual void handleLowerMsg(cMessage* msg) override;

    // handle unicast messages coming from above layers
    virtual void handleUpperMsg(cMessage* msg) override;

    // handle control messages coming from above
    virtual void handleUpperControl(cMessage* msg) override;

    // handle control messages coming from below
    virtual void handleLowerControl(cMessage* msg) override;

    // handles and application layer message
    void handleUnicastMsg(UnicastMessage* unicast);

    // override handleMessage to manager upper layer gate array
    virtual void handleMessage(cMessage* msg) override;

    // signal handler
    using BaseApplLayer::receiveSignal;
    void receiveSignal(cComponent* source, simsignal_t signalID, bool v, cObject* details) override;
    void receiveSignal(cComponent* source, simsignal_t signalID, bool v)
    {
        receiveSignal(source, signalID, v, 0);
    }

    /**
     * Sends a platooning message with all information about the car. This is an utility function for
     * subclasses
     */
    void sendPlatooningMessage(int destinationAddress);

    /**
     * This method must be overridden by subclasses to take decisions
     * about what to do.
     * Passed packet MUST NOT be freed, but just be read. Freeing is a duty of the
     * superclass
     *
     * \param pkt the platooning beacon
     * \param unicast the original unicast packet which was containing pkt
     */
    virtual void messageReceived(PlatooningBeacon* pkt, UnicastMessage* unicast);

    /**
     * These methods signal changes in channel busy status to subclasses
     * or occurrences of collisions.
     * Subclasses which are interested should ovverride these methods.
     */
    virtual void channelBusyStart()
    {
    }
    virtual void channelIdleStart()
    {
    }
    virtual void collision()
    {
    }

    // traci mobility. used for getting/setting info about the car
    Veins::TraCIMobility* mobility;
    Veins::TraCICommandInterface* traci;
    Veins::TraCICommandInterface::Vehicle* traciVehicle;

public:
    // id for beacon message
    static const int BRAKING_TYPE = 1391;
    static const int BEACON_TYPE = 12345;

    BaseProtocol()
    {
        sendBeacon = nullptr;
        recordData = nullptr;
        usedGates = 0;
    }
    virtual ~BaseProtocol();

    virtual void initialize(int stage) override;

    // register a higher level application by its id
    void registerApplication(int applicationId, InputGate* appInputGate, OutputGate* appOutputGate, ControlInputGate* appControlInputGate, ControlOutputGate* appControlOutputGate);
};

#endif /* BASEPROTOCOL_H_ */
