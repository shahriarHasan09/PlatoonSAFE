//
// Copyright (C) 2018 Julian Heinovski <julian.heinovski@ccs-labs.org>
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

#include "veins/modules/application/platooning/scenarios/SimpleScenario.h"

using namespace Veins;

Define_Module(SimpleScenario);

void SimpleScenario::initialize(int stage)
{

    BaseScenario::initialize(stage);

    if (stage == 0)
        // get pointer to application
        appl = FindModule<BaseApp*>::findSubModule(getParentModule());

    if (stage == 1) {

        accelerate = new cMessage("accelerate");
        accelerate2 = new cMessage("accelerate2");
        accelerate3 = new cMessage("accelerate3");
        decelerate = new cMessage("decelerate");
        // average speed
        leaderSpeed = par("leaderSpeed").doubleValue() / 3.6;

        if (positionHelper->isLeader()) {
            // set base cruising speed
            traciVehicle->setCruiseControlDesiredSpeed(leaderSpeed);
                scheduleAt(SimTime(200), accelerate);
                scheduleAt(SimTime(250), accelerate2);
                scheduleAt(SimTime(300), accelerate3);
                scheduleAt(SimTime(350), decelerate);

        }
        else {
             //let the follower set a higher desired speed to stay connected
             //to the leader when it is accelerating
            traciVehicle->setCruiseControlDesiredSpeed(leaderSpeed + 10);
        }
    }
}

SimpleScenario::~SimpleScenario(){
    cancelAndDelete(accelerate);
    accelerate = nullptr;
    cancelAndDelete(decelerate);
    decelerate = nullptr;
    cancelAndDelete(accelerate2);
    decelerate = nullptr;
    cancelAndDelete(accelerate3);
    decelerate = nullptr;
}

void SimpleScenario::handleSelfMsg(cMessage* msg){
    BaseScenario::handleSelfMsg(msg);
    if (msg==accelerate){
        traciVehicle->setCruiseControlDesiredSpeed(27.77);
    }
    if (msg == accelerate2){
        traciVehicle->setCruiseControlDesiredSpeed(38.88);
    }
    if (msg == accelerate3){
        traciVehicle->setCruiseControlDesiredSpeed(27.77);
    }
    if (msg == decelerate){
        traciVehicle->setCruiseControlDesiredSpeed(16.66);
    }
}
