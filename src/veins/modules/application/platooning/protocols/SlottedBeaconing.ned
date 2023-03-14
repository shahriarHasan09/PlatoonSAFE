//
// Copyright (C) 2012-2018 Michele Segata <segata@ccs-labs.org>
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

package org.car2x.veins.modules.application.platooning.protocols;

import org.car2x.veins.modules.application.platooning.protocols.BBaseProtocol;

//
// Define slotted beaconing for platooning
//
simple SlottedBeaconing extends BBaseProtocol
{
    parameters:
        int SVRSizeLimit = default (10);
        bool runSVR = default (false);
        bool runNN = default (false);
        double SVRC = default (0.1);
        double SVREpsilon = default(0.00005);
        double decelarationRate @unit("mpsps") = default(8mpsps);
        double brakeAtTime @unit("s") = default(8s);
        double speedOfLeader @unit("kmph") = default(100kmph);
        double sbWaitTime @unit("s") = default(0.1s);
        double DENMInterval @unit("s") = default(0.01s);
        bool isCoopEmBrkEnabled = default(false);
        bool isSynchronizedBraking = default(false);
        
         //Expected ACK delay in CEBP strategy
		double CEBPTimeOut @unit("s") = default(3.0s);
		
        double softDecelRate @unit("mpsps") = default(-1mpsps);
        bool isAEBEnabled = default(false);
        
        bool isCEBPEnabled = default(false);
        
        bool isGDEnabled = default(false);
        
        bool isRelayEnabled =default(false);
        
        bool isESynchBrkEnabled = default(false);
        
        bool isSBwithAvgDelay = default(false);
        
        @display("i=block/network2");
        @class(SlottedBeaconing);
}
