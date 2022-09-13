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

#include "veins/modules/application/platooning/protocols/BaseProtocol.h"

#include "veins/modules/mac/ieee80211p/Mac1609_4.h"

#include <iostream>
#include <fstream>

using namespace Veins;

Define_Module(BaseProtocol)

// set signals for channel busy and collisions
const simsignalwrap_t BaseProtocol::sigChannelBusy = simsignalwrap_t(
        "sigChannelBusy");
const simsignalwrap_t BaseProtocol::sigCollision = simsignalwrap_t(
        "sigCollision");

void BaseProtocol::initialize(int stage) {

    BaseApplLayer::initialize(stage);

    if (stage == 0) {

        // init class variables
        sendBeacon = 0;
        channelBusy = false;
        nCollisions = 0;
        busyTime = SimTime(0);
        seq_n = 0;
        seq_denm = 0;
        delay_seq_no = 0;
        recordData = 0;

        // get gates
        lowerControlIn = findGate("lowerControlIn");
        lowerControlOut = findGate("lowerControlOut");
        lowerLayerIn = findGate("lowerLayerIn");
        lowerLayerOut = findGate("lowerLayerOut");
        minUpperId = gate("upperLayerIn", 0)->getId();
        maxUpperId = gate("upperLayerIn", MAX_GATES_COUNT - 1)->getId();
        minUpperControlId = gate("upperControlIn", 0)->getId();
        maxUpperControlId =
                gate("upperControlIn", MAX_GATES_COUNT - 1)->getId();

        // beaconing interval in seconds
        beaconingInterval = SimTime(par("beaconingInterval").doubleValue());
        // platooning message packet size
        packetSize = par("packetSize").intValue();

        ackPacketSize = par("ackPacketSize").intValue(); // shahriar
        delayPktSize = par("delayPktSize").intValue();
        // priority of platooning message
        priority = par("priority").intValue();
        ASSERT2(priority >= 0 && priority <= 7,
                "priority value must be between 0 and 7");

        // init messages for scheduleAt
        sendBeacon = new cMessage("sendBeacon");
        recordData = new cMessage("recordData");

        // own id
        nodeIdOut.setName("nodeId");
        // channel busy time
        busyTimeOut.setName("busyTime");
        // mac layer collisions
        collisionsOut.setName("collisions");
        // delay metrics
        lastLeaderMsgTime = SimTime(-1);
        lastFrontMsgTime = SimTime(-1);
        leaderDelayIdOut.setName("leaderDelayId");
        frontDelayIdOut.setName("frontDelayId");
        leaderDelayOut.setName("leaderDelay");
        frontDelayOut.setName("frontDelay");

        // DENM and ACK delay metrics
        lastLeaderMsgSeqN = -1;
        lastFrontMsgSeqN = -1;
        //different delay metrics for statistics collection
        firstDENMDelay.setName("firstDENMDelay");
        seqOutVector.setName("noOfRetransmissions");
        firstAckDelay.setName("firstAckDelay");
        leaderTransmissionN.setName("leaderTransmissionN");
        frontTransmissionN.setName("frontTransmissionN");
        predictedDelayOut.setName("predictedDelayAtLV");
        predictionErrorOut.setName("predictionErrorAtLV");

        // subscribe to signals for channel busy state and collisions
        findHost()->subscribe(sigChannelBusy, this);
        findHost()->subscribe(sigCollision, this);

        // init statistics collection. round to second
        SimTime rounded = SimTime(floor(simTime().dbl() + 1), SIMTIME_S);
        scheduleAt(rounded, recordData);
    }

    if (stage == 1) {
        // get traci interface
        mobility = Veins::TraCIMobilityAccess().get(getParentModule());
        ASSERT(mobility);
        traci = mobility->getCommandInterface();
        ASSERT(traci);
        traciVehicle = mobility->getVehicleCommandInterface();
        ASSERT(traciVehicle);
        positionHelper = FindModule<BasePositionHelper*>::findSubModule(
                getParentModule());
        ASSERT(positionHelper);

        /********** Cooperative emergency braking and ML parameters read from the .NED file*******/
        brakeAtTime = SimTime(par("brakeAtTime").doubleValue());
        decelarationRate = par("decelarationRate").doubleValue();
        speedOfLeader = par("speedOfLeader").doubleValue() / 3.6;
        isCoopEmBrkEnabled = par("isCoopEmBrkEnabled");
        sbWaitTime = par("sbWaitTime").doubleValue();
        DENMInterval = par("DENMInterval").doubleValue();
        isSynchronizedBraking = par("isSynchronizedBraking");
        softDecelRate = par("softDecelRate").doubleValue();
        isAEBEnabled = par("isAEBEnabled");
        isCEBPEnabled = par("isCEBPEnabled");
        isGDEnabled = par("isGDEnabled");
        isRelayEnabled = par("isRelayEnabled");
        isESynchBrkEnabled = par("isESynchBrkEnabled");
        isSBwithAvgDelay = par("isSBwithAvgDelay");
        runSVR = par("runSVR");
        runNN = par("runNN");
        if (runSVR == true) {
            SVRSizeLimit = par("SVRSizeLimit");
            SVRC = par("SVRC");
            SVREpsilon = par("SVREpsilon");
        }

        /*If CEB module is enabled, schedule the emergency braking at the time the imaginary road hazard takes place*/
        if (isCoopEmBrkEnabled) {
            if (positionHelper->getId() < positionHelper->getLanesCount()) {

                brakingMessage = new cMessage("brakingMessage");
                if (simTime() > brakeAtTime) {
                    brakeAtTime = simTime();
                    scheduleAt(simTime(), brakingMessage);
                } else {
                    scheduleAt(brakeAtTime, brakingMessage);
                }

                traciVehicle->setCruiseControlDesiredSpeed(speedOfLeader);
            } else {
                traciVehicle->setCruiseControlDesiredSpeed(speedOfLeader + 10);
            }
        }

        /****************************SVR-jgorospe****************************/
        //Initialise SVR
        if (runSVR == true) {
            SVR = new OnlineSVR();
            // Set parameters
                            SVR->SetEpsilon(SVREpsilon);
                            SVR->SetC(SVRC);
                            SVR->SetKernelType(OnlineSVR::KERNEL_RBF);
                            SVR->SetAutoErrorTollerance(false);
                            SVR->SetVerbosity(OnlineSVR::VERBOSITY_NORMAL);
                            //setupSVR(SVREpsilon, SVRC, SVR);
        }

        if (runNN == true) {
            //create a socket
            cliSockDes = setupNNConnection();
            serAddr = getNNServerAdd();
        }

        /********************************************************************/

        // this is the id of the vehicle. used also as network address
        myId = positionHelper->getId();
        length = traciVehicle->getLength();
        // tell the unicast protocol below which mac address to use via control message
        UnicastProtocolControlMessage *setMacAddress =
                new UnicastProtocolControlMessage("");
        setMacAddress->setControlCommand(SET_MAC_ADDRESS);
        setMacAddress->setCommandValue(myId);
        send(setMacAddress, lowerControlOut);
    }
}

BaseProtocol::~BaseProtocol() {
    cancelAndDelete(sendBeacon);
    sendBeacon = nullptr;
    cancelAndDelete(recordData);
    recordData = nullptr;

    /**************CEB*****************/
    cancelAndDelete(brakingMessage);
    brakingMessage = nullptr;
    cancelAndDelete(synBrkMsg);
    synBrkMsg = nullptr;

    cancelAndDelete(startDecelerating0);
    startDecelerating0 = nullptr;
    cancelAndDelete(startDecelerating1);
    startDecelerating1 = nullptr;
    cancelAndDelete(startDecelerating2);
    startDecelerating2 = nullptr;
    cancelAndDelete(startDecelerating3);
    startDecelerating3 = nullptr;
    cancelAndDelete(startDecelerating4);
    startDecelerating4 = nullptr;
    cancelAndDelete(startDecelerating5);
    startDecelerating5 = nullptr;
    cancelAndDelete(startDecelerating6);
    startDecelerating6 = nullptr;
    cancelAndDelete(startDecelerating7);
    startDecelerating7 = nullptr;

    cancelAndDelete(ESynchMsg);
    ESynchMsg = nullptr;

    cancelAndDelete(synLVBrkMsg);
    synLVBrkMsg = nullptr;

    cancelAndDelete(ackMsg6);
    ackMsg6 = nullptr;
    cancelAndDelete(ackMsg5);
    ackMsg5 = nullptr;
    cancelAndDelete(ackMsg4);
    ackMsg4 = nullptr;
    cancelAndDelete(ackMsg3);
    ackMsg3 = nullptr;
    cancelAndDelete(ackMsg2);
    ackMsg2 = nullptr;
    cancelAndDelete(ackMsg1);
    ackMsg1 = nullptr;

    cancelAndDelete(alagMsg);
    alagMsg = nullptr;
    cancelAndDelete(leaderAdaptive);
    leaderAdaptive = nullptr;
    cancelAndDelete(hardDeceleration);
    hardDeceleration = nullptr;

    sendto(cliSockDes, "end", 3, 0, (struct sockaddr*) &serAddr, sizeof(serAddr));
}

void BaseProtocol::handleSelfMsg(cMessage *msg) {
    Plexe::VEHICLE_DATA data;
    traciVehicle->getVehicleData(&data);

    if (msg == recordData) {

        // if channel is currently busy, we have to split the amount of time between
        // this period and the successive. so we just compute the channel busy time
        // up to now, and then reset the "startBusy" timer to now
        if (channelBusy) {
            busyTime += simTime() - startBusy;
            startBusy = simTime();
        }

        // time for writing statistics
        // node id
        nodeIdOut.record(myId);
        // record busy time for this period
        busyTimeOut.record(busyTime);
        // record collisions for this period
        collisionsOut.record(nCollisions);

        // and reset counter
        busyTime = SimTime(0);
        nCollisions = 0;

        scheduleAt(simTime() + SimTime(1, SIMTIME_S), recordData);
    }

    /*************Braking selfMessages ****************/
    else if (msg == brakingMessage) {
        //LV braking with the SB at tau_wait
        if (isSynchronizedBraking == true && synchCounter == 0) {
            SBAvgWaitTime = SBTotalDelay/SBDelayCounter;
            synLVBrkMsg = new cMessage("synLVBrkMsg");
            if (isSBwithAvgDelay){
                scheduleAt(simTime() + SBAvgWaitTime, synLVBrkMsg);
            }
            else {
                scheduleAt(simTime() + predictedDelayAtLV, synLVBrkMsg);
            }
            ++synchCounter;
        }
        //LV braking with the ESB strategy at soft deceleration rate and scheduling full deceleration at tau_wait
        if (isESynchBrkEnabled == true && synchCounter == 0) {
            SBAvgWaitTime = SBTotalDelay/SBDelayCounter;
            traciVehicle->setFixedAcceleration(1, -softDecelRate);
            synLVBrkMsg = new cMessage("synLVBrkMsg");
            if (isSBwithAvgDelay){
                scheduleAt(simTime() + SBAvgWaitTime, synLVBrkMsg);
            }
            else {
                scheduleAt(simTime() + predictedDelayAtLV, synLVBrkMsg);
            }
            ++synchCounter;
        }

        /*The lead vehicle starts by sending braking message, i.e., DENM, and the message is repeated at each DENM interval*/
        sendBrakingMessage(-1);
        //LV braking with the AEB strategy
        if (isAEBEnabled == true && adaptiveCounter == 0) {
            leaderAdaptive = new cMessage("leaderAdaptive");
            scheduleAt(simTime(), leaderAdaptive);
            ++adaptiveCounter;
        } else if (isGDEnabled == true && progCounter == 0) { //LV braking with GD strategy
            traciVehicle->setFixedAcceleration(1, -decelarationRate);
            ++progCounter;
        }

        ++brakingCounter;
        //repeat the DENMs at DENM interval
        scheduleAt(simTime() + DENMInterval, brakingMessage);
    } else if (msg == synLVBrkMsg) {
        traciVehicle->setFixedAcceleration(1, -decelarationRate);
    }
    /*full deceleration upon reception of ACK message*/
    else if (msg == startDecelerating0 || msg == startDecelerating1
            || msg == startDecelerating2 || msg == startDecelerating3
            || msg == startDecelerating4 || msg == startDecelerating5
            || msg == startDecelerating6 || msg == startDecelerating7
            || msg == synBrkMsg || msg == ESynchMsg) {
        traciVehicle->setFixedAcceleration(1, -decelarationRate);
    /*The ACK messages are repeated every DENM Interval as well. Required for AEB and CEBP strategies*/
    } else if (msg == ackMsg6) {
        sendAckMessage(-1);
        scheduleAt(simTime() + DENMInterval, ackMsg6);
    } else if (msg == ackMsg5) {
        sendAckMessage(-1);
        scheduleAt(simTime() + DENMInterval, ackMsg5);
    } else if (msg == ackMsg4) {
        sendAckMessage(-1);
        scheduleAt(simTime() + DENMInterval, ackMsg4);
    } else if (msg == ackMsg3) {
        sendAckMessage(-1);
        scheduleAt(simTime() + DENMInterval, ackMsg3);
    } else if (msg == ackMsg2) {
        sendAckMessage(-1);
        scheduleAt(simTime() + DENMInterval, ackMsg2);
    } else if (msg == ackMsg1) {
        sendAckMessage(-1);
        scheduleAt(simTime() + DENMInterval, ackMsg1);
    }
    /*the platooning vehicles start soft deceleration if they have not started doing so already*/
    else if (msg == alagMsg) {
        if (data.acceleration > softDecelRate && data.acceleration >= 0 && ackSet.count(positionHelper->getPosition()) == 0
            && positionHelper->getPosition() != positionHelper->getPlatoonSize() - 1) {
            traciVehicle->setFixedAcceleration(1, -softDecelRate);
        }
    }
    /*Leader's soft decelaration*/
    else if (msg == leaderAdaptive && ackSet.count(0) == 0) {
        traciVehicle->setFixedAcceleration(1, -softDecelRate);
    }
    /*Hard deceleration upon reception of ACK messages*/
    else if (msg == hardDeceleration) {
        traciVehicle->setFixedAcceleration(1, -decelarationRate);
    }
}

void BaseProtocol::sendBrakingMessage(int destinationAddress) { /*DENM*/
    double currentTime = simTime().dbl();
    //double startBrakingTime = brakeAtTime.dbl();

    UnicastMessage *unicast = new UnicastMessage("", BRAKING_TYPE);
    unicast->setDestination(-1);
    unicast->setPriority(priority);
    unicast->setChannel(Channels::CCH);

    BrakingPacket *pkt = new BrakingPacket();
    pkt->setBrakingTime(brakeAtTime.dbl());
    pkt->setDecelarationRate(decelarationRate);
    pkt->setSpeedOfLeader(speedOfLeader);
    pkt->setCurrentSimTime(currentTime);
    pkt->setIdOfVehicle(myId);
    pkt->setSeqNumber(seq_denm++);
    pkt->setPredictedDelayOfLastVeh(predictedDelayAtLV);
    pkt->setSynchBrkAvgWaitTime(SBAvgWaitTime);

    pkt->setKind(BRAKING_TYPE);
    pkt->setByteLength(packetSize);
    unicast->encapsulate(pkt);
    sendDown(unicast);
}

void BaseProtocol::sendDENMRelayMessage(int destinationAddress) {
    double currentTime = simTime().dbl();

    UnicastMessage *unicast = new UnicastMessage("", BRAKING_TYPE);
    unicast->setDestination(-1);
    unicast->setPriority(priority);
    unicast->setChannel(Channels::CCH);

    RelayDENMs *pkt = new RelayDENMs();
    pkt->setRelayedBrakingTime(DENMGenTime);
    pkt->setRelayedDecelarationRate(decelarationRate);
    pkt->setRelayedSpeedOfLeader(speedOfLeader);
    pkt->setRelayedCurrentSimTime(currentTime);
    pkt->setRelayedIdOfVehicle(myId);
    pkt->setRelayedSeqNumber(DENM_seq_no);
    pkt->setRelayedPredictedDelayOfLastVeh(relayedPredictedDelayAtLV);
    pkt->setRelayedSBAvgWaitTime(relayedSBAvgWaitTime);

    pkt->setKind(BRAKING_TYPE);
    pkt->setByteLength(packetSize);
    unicast->encapsulate(pkt);
    sendDown(unicast);
}

void BaseProtocol::sendAckMessage(int destinationAddress) { /*ACK message*/
    double currentTime = simTime().dbl();
    //double startBrakingTime = brakeAtTime.dbl();

    UnicastMessage *unicast = new UnicastMessage("", BRAKING_TYPE);
    unicast->setDestination(-1);
    unicast->setPriority(priority);
    unicast->setChannel(Channels::CCH);

    ackPkt *pkt = new ackPkt();
    pkt->setBrakingTime(brakeAtTime.dbl());
    pkt->setDecelarationRate(decelarationRate);
    pkt->setSpeedOfLeader(speedOfLeader);
    pkt->setCurrentSimTime(currentTime);
    pkt->setIdOfVehicle(myId);
    pkt->setSeqNumber(seq_denm++);

    pkt->setKind(BRAKING_TYPE);
    pkt->setByteLength(ackPacketSize);
    unicast->encapsulate(pkt);
    sendDown(unicast);

}

/*delay experienced by the last vehicle with respect to the Lead Vehicle to be sent to the lead vehicle again*/
void BaseProtocol::sendDelayMessage(int destinationAddress) {
    double currentTime = simTime().dbl();

    UnicastMessage *unicast = new UnicastMessage("", BRAKING_TYPE);
    unicast->setDestination(-1);
    unicast->setPriority(priority);
    unicast->setChannel(Channels::CCH);

    DelayMessage *delPkt = new DelayMessage();
    delPkt->setDelayMsgGenTime(currentTime);
    delPkt->setDelayofLastVeh(delay_us);
    delPkt->setDelSeqNumber(delay_seq_no);
    delPkt->setVehicleID(myId);

    delPkt->setKind(BRAKING_TYPE);
    delPkt->setByteLength(delayPktSize);
    unicast->encapsulate(delPkt);
    sendDown(unicast);
}

//relay message for the delay messages
void BaseProtocol::sendRelayMessage(int destinationAddress) {
    double currentTime = simTime().dbl();

    UnicastMessage *unicast = new UnicastMessage("", BRAKING_TYPE);
    unicast->setDestination(-1);
    unicast->setPriority(priority);
    unicast->setChannel(Channels::CCH);

    RelayMessage *relayPkt = new RelayMessage();
    relayPkt->setRelayMsgGenTime(delayMsgGenTime);
    relayPkt->setRelayedDelay(relayedLastVehDelay);
    relayPkt->setRelayedMsgSeqNumber(relayedMsgSeqNo);
    relayPkt->setRelayVehicleID(myId);

    relayPkt->setKind(BRAKING_TYPE);
    relayPkt->setByteLength(delayPktSize);
    unicast->encapsulate(relayPkt);
    sendDown(unicast);
}

void BaseProtocol::sendPlatooningMessage(int destinationAddress) /*CAM*/
{

    // vehicle's data to be included in the message
    Plexe::VEHICLE_DATA data;
    // get information about the vehicle via traci
    traciVehicle->getVehicleData(&data);

    // create and send beacon
    UnicastMessage *unicast = new UnicastMessage("", BEACON_TYPE);
    unicast->setDestination(-1);
    unicast->setPriority(priority);
    unicast->setChannel(Channels::CCH);

    // create platooning beacon with data about the car
    PlatooningBeacon *pkt = new PlatooningBeacon();
    pkt->setControllerAcceleration(data.u);
    pkt->setAcceleration(data.acceleration);
    pkt->setSpeed(data.speed);
    pkt->setVehicleId(myId);
    pkt->setPositionX(data.positionX);
    pkt->setPositionY(data.positionY);
    // set the time to now
    pkt->setTime(data.time);
    pkt->setLength(length);
    pkt->setSpeedX(data.speedX);
    pkt->setSpeedY(data.speedY);
    pkt->setAngle(data.angle);
    // i generated the message, i send it
    pkt->setRelayerId(myId);
    pkt->setKind(BEACON_TYPE);
    pkt->setByteLength(packetSize);
    pkt->setSequenceNumber(seq_n++);

    // put platooning beacon into the message for the UnicastProtocol
    unicast->encapsulate(pkt);
    sendDown(unicast);

}

void BaseProtocol::handleUnicastMsg(UnicastMessage *unicast) {

    ASSERT2(unicast, "received a frame not of type UnicastMessage");

    cPacket *enc = unicast->getEncapsulatedPacket();
    ASSERT2(enc, "received a UnicastMessage with nothing inside");

    if (positionHelper->getPlatoonSize() % 2 == 0)
        platoonMidVehicleID = (positionHelper->getPlatoonSize() / 2) - 1;
    else
        platoonMidVehicleID = positionHelper->getPlatoonSize() / 2;

    if (PlatooningBeacon *epkt = dynamic_cast<PlatooningBeacon*>(enc)) {
        SimTime delay;
        // invoke messageReceived() method of subclass
        messageReceived(epkt, unicast);

        if (positionHelper->getLeaderId() == epkt->getVehicleId()) {
            // check if this is at least the second message we have received
            if (lastLeaderMsgTime.dbl() > 0) {
                delay = simTime() - lastLeaderMsgTime;
                leaderDelayOut.record(delay);
                leaderDelayIdOut.record(myId);

                leaderTransmissionN.record(
                        epkt->getSequenceNumber() - lastLeaderMsgSeqN);
            }
            lastLeaderMsgTime = simTime();
            lastLeaderMsgSeqN = epkt->getSequenceNumber();

            /****************************SVR-jgorospe****************************/
            //if current vehicle is the last vehicle in the platoon
            if (positionHelper->getPosition() == positionHelper->getPlatoonSize() - 1 && simTime() < brakeAtTime) {
                /********TODO: Change to lastFrontMsgTime*********/
                // delay experienced by the last vehicle
                delay_us = delay.dbl();
                //broadcast the delay experienced by the last vehicle
                sendDelayMessage(-1);
            }
            /********************************************************************/
        }
        if (positionHelper->getFrontId() == epkt->getVehicleId()) {
            // check if this is at least the second message we have received
            if (lastFrontMsgTime.dbl() > 0) {
                frontDelayOut.record(simTime() - lastFrontMsgTime);
                frontDelayIdOut.record(myId);

                frontTransmissionN.record(epkt->getSequenceNumber() - lastFrontMsgSeqN);
            }
            lastFrontMsgTime = simTime();
            lastFrontMsgSeqN = epkt->getSequenceNumber();
        }

    }

    /***********************************/
    else if (DelayMessage *delMsg = dynamic_cast<DelayMessage*>(enc)) {
        cPacket *decap = unicast->decapsulate();
        DelayMessage *delayPkt = check_and_cast<DelayMessage*>(decap);

        Plexe::VEHICLE_DATA data;
        traciVehicle->getVehicleData(&data);

        // the mid vehicle relays the message containing the delay experienced by the last vehicle with respect to the LV
        if (positionHelper->getPosition() == platoonMidVehicleID) {
            relayedLastVehDelay = delayPkt->getDelayofLastVeh();
            relayedMsgSeqNo = delayPkt->getDelSeqNumber();
            delayMsgGenTime = delayPkt->getDelayMsgGenTime();
            sendRelayMessage(-1);
        } //the prediction of the delays happens at the LV
        if (positionHelper->getPosition() == 0 && delayPkt->getDelayofLastVeh() != lastRecordedDelayAtLV) {
            realDelayOfLastVeh = delayPkt->getDelayofLastVeh();
            if (runSVR == true) { // predict with SVR
                predictWithSVRAndTrain(&lastRecordedDelayAtLV, &predictedDelayAtLV, &PredErrorAtLV, realDelayOfLastVeh, SVRSizeLimit, SVR);
                predictedDelayOut.record(predictedDelayAtLV);
                predictionErrorOut.record(PredErrorAtLV);
            }
            if (runNN == true) { // predict with neural network
                predictWithNN(cliSockDes, realDelayOfLastVeh, &numPrev, serAddr, &lastRecordedDelayAtLV, &predictedDelayAtLV, &PredErrorAtLV);
                predictedDelayOut.record(predictedDelayAtLV);
                predictionErrorOut.record(PredErrorAtLV);
            }
            if (isSBwithAvgDelay){ //SB with Avg CAM delays
                if (simTime()>=10){
                    calculateSBAvgWaitTime(realDelayOfLastVeh);
                }
            }
            lastDelaySeqNo = delayPkt->getDelSeqNumber();
            lastDelMsgGenTime = delayPkt->getDelayMsgGenTime();
        }
        delete delayPkt;
    }
    // if the message sent by the last vehicle is lost, but the LV receives it through relayed message,
    // the LV records the relayed message and performs the prediction
    else if (RelayMessage *relMsg = dynamic_cast<RelayMessage*>(enc)) {
        cPacket *decap = unicast->decapsulate();
        RelayMessage *relayPkt = check_and_cast<RelayMessage*>(decap);

        Plexe::VEHICLE_DATA data;
        traciVehicle->getVehicleData(&data);
        if (positionHelper->getPosition() == 0 && relayPkt->getRelayedDelay() != lastRecordedDelayAtLV) {
            realDelayOfLastVeh = relayPkt->getRelayedDelay();
            if (runSVR == true) {
                predictWithSVRAndTrain(&lastRecordedDelayAtLV, &predictedDelayAtLV, &PredErrorAtLV, realDelayOfLastVeh, SVRSizeLimit, SVR);
                predictedDelayOut.record(predictedDelayAtLV);
                predictionErrorOut.record(PredErrorAtLV);
            }
            if (runNN == true) {
                predictWithNN(cliSockDes, realDelayOfLastVeh, &numPrev, serAddr, &lastRecordedDelayAtLV, &predictedDelayAtLV, &PredErrorAtLV);
                predictedDelayOut.record(predictedDelayAtLV);
                predictionErrorOut.record(PredErrorAtLV);
            }
            if (isSBwithAvgDelay){
                if (simTime()>=50){
                    calculateSBAvgWaitTime(realDelayOfLastVeh);
                }
            }
        }
        delete relayPkt;
    }
    // if the mid vehicle receives a braking packet and if relaying of braking packets are enabled,
    // it relays the braking message using the mid platoon vehicle
    else if (BrakingPacket *bpkt = dynamic_cast<BrakingPacket*>(enc)) {
        cPacket *decap = unicast->decapsulate();
        BrakingPacket *brkPckt = check_and_cast<BrakingPacket*>(decap);

        Plexe::VEHICLE_DATA data;
        traciVehicle->getVehicleData(&data);
        if (isRelayEnabled) {
            if (positionHelper->getPosition() == platoonMidVehicleID && positionHelper->getPosition() != 0) {
                relayedSBAvgWaitTime = brkPckt->getSynchBrkAvgWaitTime();
                relayedPredictedDelayAtLV = brkPckt->getPredictedDelayOfLastVeh();
                DENMGenTime = brkPckt->getBrakingTime();
                DENM_seq_no = brkPckt->getSeqNumber();
                //this is the relay of the DENMs obtained from the LV
                sendDENMRelayMessage(-1);
            }
        }

        if (duplicateSet.count(positionHelper->getPosition())) { /*Whether the ego vehicle has already received a DENM*/
        }
        else {
            firstDENMDelay.record(simTime().dbl() - brkPckt->getBrakingTime());
            seqOutVector.record(brkPckt->getSeqNumber());
            duplicateSet.insert(positionHelper->getPosition());

            /*If SB, then schedule the braking at hazard detection time + waiting time*/
            if (isSynchronizedBraking) {
                if (isSBwithAvgDelay){
                    synchronizedBraking(brkPckt->getSynchBrkAvgWaitTime());
                }
                else {
                    synchronizedBraking(brkPckt->getPredictedDelayOfLastVeh());
                }
            }
            /*Synchronized braking with soft deceleration*/
            if (isESynchBrkEnabled) {
                if (isSBwithAvgDelay){
                    EnhancedSynchronizedBraking(brkPckt->getSynchBrkAvgWaitTime());
                }
                else {
                    EnhancedSynchronizedBraking(brkPckt->getPredictedDelayOfLastVeh());
                }
            }
            /*If AEB, and if the ego vehicle is the last vehicle, schedule a hard deceleration. If not, schedule soft deceleration*/
            else if (isAEBEnabled) {
                adaptiveBraking();
            }
            /*If CEBP, perform hard deceleration and schedule an ACK message*/
            else if (isCEBPEnabled) {
                CooperativeBrakingProtocol();
            }
            else if (isGDEnabled){
                traciVehicle->setFixedAcceleration(1, -decelarationRate);
            }
        }

        delete brkPckt;
    }

    else if (RelayDENMs *DPkt = dynamic_cast<RelayDENMs*>(enc)) {
        cPacket *decap = unicast->decapsulate();
        RelayDENMs *RelDenPkt = check_and_cast<RelayDENMs*>(decap);

        if (positionHelper->getPosition() != 0) {
            if (duplicateSet.count(positionHelper->getPosition())) { /*Whether the ego vehicle has already received a DENM*/
            }
            else {
                firstDENMDelay.record(simTime().dbl() - RelDenPkt->getRelayedBrakingTime());
                seqOutVector.record(RelDenPkt->getRelayedSeqNumber());

                duplicateSet.insert(positionHelper->getPosition());

                if (isSynchronizedBraking) { /*If SB, then schedule the braking at hazard detection time + waiting time*/
                    if (isSBwithAvgDelay){
                        synchronizedBraking(RelDenPkt->getRelayedSBAvgWaitTime());
                    }
                    else {
                        synchronizedBraking(RelDenPkt->getRelayedPredictedDelayOfLastVeh());
                    }
                }
                if (isESynchBrkEnabled) {                 /*Synchronized braking with soft deceleration*/
                    if (isSBwithAvgDelay){
                        EnhancedSynchronizedBraking(RelDenPkt->getRelayedSBAvgWaitTime());
                    }
                    else {
                        EnhancedSynchronizedBraking(RelDenPkt->getRelayedPredictedDelayOfLastVeh());
                    }
                }
                else if (isAEBEnabled) {
                    adaptiveBraking();
                }
                /*If CEBP, perform hard deceleration and schedule an ACK message*/
                else if (isCEBPEnabled) {
                    CooperativeBrakingProtocol();
                }
                else if (isGDEnabled){
                    traciVehicle->setFixedAcceleration(1, -decelarationRate);
                }
            }
        }
        delete RelDenPkt;
    }

    /*handle the ACK packets just like DENMs. Check duplication, record the delay, and schedule the ACK messages*/
    else if (ackPkt *aPkt = dynamic_cast<ackPkt*>(enc)) {
        cPacket *decap = unicast->decapsulate();
        ackPkt *acknPkt = check_and_cast<ackPkt*>(decap);

        Plexe::VEHICLE_DATA data;
        traciVehicle->getVehicleData(&data);

        if (acknPkt->getIdOfVehicle() == positionHelper->getPosition() + 1) {
            if (ackSet.count(positionHelper->getPosition())) {
                EV << positionHelper->getPosition() << " is already decelerated" << endl;
            } else {
                firstAckDelay.record((simTime().dbl() - brakeAtTime.dbl()) * 1000);
                ackSet.insert(positionHelper->getPosition());

                hardDeceleration = new cMessage("hardDeceleration");
                scheduleAt(simTime(), hardDeceleration);

//                if (positionHelper->getPosition()==6){
//                    ackMsg6 = new cMessage("ackMsg6");
//                    scheduleAt(simTime(), ackMsg6);
//                }
                if (positionHelper->getPosition() == 5) {
                    ackMsg5 = new cMessage("ackMsg5");
                    scheduleAt(simTime(), ackMsg5);
                } else if (positionHelper->getPosition() == 4) {
                    ackMsg4 = new cMessage("ackMsg4");
                    scheduleAt(simTime(), ackMsg4);
                } else if (positionHelper->getPosition() == 3) {
                    ackMsg3 = new cMessage("ackMsg3");
                    scheduleAt(simTime(), ackMsg3);
                } else if (positionHelper->getPosition() == 2) {
                    ackMsg2 = new cMessage("ackMsg2");
                    scheduleAt(simTime(), ackMsg2);
                } else if (positionHelper->getPosition() == 1) {
                    ackMsg1 = new cMessage("ackMsg1");
                    scheduleAt(simTime(), ackMsg1);
                }
            }

        }

        delete acknPkt;
    }

    // find the application responsible for this beacon
    ApplicationMap::iterator app = apps.find(unicast->getKind());
    if (app != apps.end() && app->second.size() != 0) {
        AppList applications = app->second;
        AppList::iterator i;
        for (AppList::iterator i = applications.begin();
                i != applications.end(); i++) {
            // send the message to the applications responsible for it
            send(unicast->dup(), std::get<1>(*i));
        }
    }
    delete unicast;
}



void BaseProtocol::synchronizedBraking (double SBWaitTime){
    synBrkMsg = new cMessage("synBrkMsg");
    if (simTime() < brakeAtTime + SBWaitTime) {
        scheduleAt(brakeAtTime + SBWaitTime, synBrkMsg);
    } else {
        scheduleAt(simTime(), synBrkMsg);
    }
}

void BaseProtocol::EnhancedSynchronizedBraking (double SBWaitTime){
    ESynchMsg = new cMessage("ESynchMsg");
    if (positionHelper->getPosition() == positionHelper->getPlatoonSize() - 1) {
        traciVehicle->setFixedAcceleration(1, -decelarationRate);
    } else if (positionHelper->getPosition() != 0) {
        if (simTime() < brakeAtTime + SBWaitTime) {
            traciVehicle->setFixedAcceleration(1, -softDecelRate);
            scheduleAt(brakeAtTime + SBWaitTime,ESynchMsg);
        } else {
            traciVehicle->setFixedAcceleration(1, -decelarationRate);
        }
    }
}

void BaseProtocol::adaptiveBraking(){
    if (positionHelper->getPosition() == positionHelper->getPlatoonSize() - 1) {
        traciVehicle->setFixedAcceleration(1, -decelarationRate);
        ackMsg6 = new cMessage("ackMsg6");
        scheduleAt(simTime(), ackMsg6);
    } else {
        alagMsg = new cMessage("alagMsg");
        scheduleAt(simTime(), alagMsg);
    }
}

void BaseProtocol::CooperativeBrakingProtocol(){
    if (positionHelper->getPosition() == positionHelper->getPlatoonSize() - 1) {
        traciVehicle->setFixedAcceleration(1, -decelarationRate);
        ackMsg6 = new cMessage("ackMsg6");
        scheduleAt(simTime(), ackMsg6);
    }
}

void BaseProtocol::calculateSBAvgWaitTime (double SBDelay){
    SBTotalDelay += SBDelay;
    ++SBDelayCounter;
}

void BaseProtocol::receiveSignal(cComponent *source, simsignal_t signalID,
        bool v, cObject *details) {

    Enter_Method_Silent
    ();
    if (signalID == sigChannelBusy) {
        if (v && !channelBusy) {
            // channel turned busy, was idle before
            startBusy = simTime();
            channelBusy = true;
            channelBusyStart();
            return;
        }
        if (!v && channelBusy) {
            // channel turned idle, was busy before
            busyTime += simTime() - startBusy;
            channelBusy = false;
            channelIdleStart();
            return;
        }
    }
    if (signalID == sigCollision) {
        collision();
        nCollisions++;
    }
}

void BaseProtocol::handleMessage(cMessage *msg) {
    if (msg->getArrivalGateId() >= minUpperId
            && msg->getArrivalGateId() <= maxUpperId)
        handleUpperMsg(msg);
    else if (msg->getArrivalGateId() >= minUpperControlId
            && msg->getArrivalGateId() <= maxUpperControlId)
        handleUpperControl(msg);
    else
        BaseApplLayer::handleMessage(msg);
}

void BaseProtocol::handleLowerMsg(cMessage *msg) {
    handleUnicastMsg(check_and_cast<UnicastMessage*>(msg));
}

void BaseProtocol::handleUpperMsg(cMessage *msg) {
    sendDown(check_and_cast<UnicastMessage*>(msg));
}

void BaseProtocol::handleUpperControl(cMessage *msg) {
    UnicastProtocolControlMessage *ctrl =
            dynamic_cast<UnicastProtocolControlMessage*>(msg);
    if (ctrl) {
        if (ctrl->getControlCommand() == SET_MAC_ADDRESS) {
            // set id to be the address we want to set to the NIC card
            myId = ctrl->getCommandValue();
        }
        sendControlDown(ctrl);
    }
}

void BaseProtocol::handleLowerControl(cMessage *msg) {
    UnicastProtocolControlMessage *ctrl =
            dynamic_cast<UnicastProtocolControlMessage*>(msg);
    if (ctrl) {
        UnicastMessage *unicast =
                dynamic_cast<UnicastMessage*>(ctrl->getEncapsulatedPacket());
        if (unicast) {
            // find the application responsible for this beacon
            ApplicationMap::iterator app = apps.find(unicast->getKind());
            if (app != apps.end() && app->second.size() != 0) {
                AppList applications = app->second;
                AppList::iterator i;
                for (AppList::iterator i = applications.begin();
                        i != applications.end(); i++) {
                    // send the message to the applications responsible for it
                    send(ctrl->dup(), std::get<3>(*i));
                }
            }
        }
        delete ctrl;
    }
}

void BaseProtocol::messageReceived(PlatooningBeacon *pkt,
        UnicastMessage *unicast) {
    ASSERT2(false,
            "BaseProtocol::messageReceived() not overridden by subclass");
}

void BaseProtocol::registerApplication(int applicationId,
        InputGate *appInputGate, OutputGate *appOutputGate,
        ControlInputGate *appControlInputGate,
        ControlOutputGate *appControlOutputGate) {
    if (usedGates == MAX_GATES_COUNT)
        throw cRuntimeError(
                "BaseProtocol: application with id=%d tried to register, but no space left",
                applicationId);
    // connect gates, if not already connected. a gate might be already
    // connected if an application is registering for multiple packet types
    cGate *upperIn, *upperOut, *upperCntIn, *upperCntOut;
    if (!appInputGate->isConnected() || !appOutputGate->isConnected()
            || !appControlInputGate->isConnected()
            || !appControlOutputGate->isConnected()) {
        if (appInputGate->isConnected() || appOutputGate->isConnected()
                || appControlInputGate->isConnected()
                || appControlOutputGate->isConnected())
            throw cRuntimeError(
                    "BaseProtocol: the application should not be connected but one of its gates is connected");
        upperOut = gate("upperLayerOut", usedGates);
        upperOut->connectTo(appInputGate);
        upperIn = gate("upperLayerIn", usedGates);
        appOutputGate->connectTo(upperIn);
        connections[appInputGate] = upperOut;
        connections[appOutputGate] = upperIn;
        upperCntOut = gate("upperControlOut", usedGates);
        upperCntOut->connectTo(appControlInputGate);
        upperCntIn = gate("upperControlIn", usedGates);
        appControlOutputGate->connectTo(upperCntIn);
        connections[appControlInputGate] = upperCntOut;
        connections[appControlOutputGate] = upperCntIn;
        usedGates++;
    } else {
        // find BaseProtocol gates already connected to the application
        GateConnections::iterator gate;
        gate = connections.find(appOutputGate);
        if (gate == connections.end())
            throw cRuntimeError(
                    "BaseProtocol: gate should already be connected by not found in the connection list");
        upperIn = gate->second;
        gate = connections.find(appInputGate);
        if (gate == connections.end())
            throw cRuntimeError(
                    "BaseProtocol: gate should already be connected by not found in the connection list");
        upperOut = gate->second;
        gate = connections.find(appControlOutputGate);
        if (gate == connections.end())
            throw cRuntimeError(
                    "BaseProtocol: gate should already be connected by not found in the connection list");
        upperCntIn = gate->second;
        gate = connections.find(appControlInputGate);
        if (gate == connections.end())
            throw cRuntimeError(
                    "BaseProtocol: gate should already be connected by not found in the connection list");
        upperCntOut = gate->second;
    }
    // save the mapping in the connection
    apps[applicationId].push_back(
            AppInOut(upperIn, upperOut, upperCntIn, upperCntOut));
}
