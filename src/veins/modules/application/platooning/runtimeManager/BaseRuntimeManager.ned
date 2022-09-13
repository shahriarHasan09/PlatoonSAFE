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

package org.car2x.veins.modules.application.platooning.runtimeManager;
import org.car2x.veins.base.modules.IBaseApplLayer;


simple BaseRuntimeManager like IBaseApplLayer {
    parameters:
        int headerLength = default(88bit) @unit(bit); //header length of the application
        
        bool rmEnabled	 = default(false); 
        
        // rm monitor
        double rmMonitorInterval @unit("s") 	= default(.10s);  
        double expectedBeconInterval @unit("s") = default(.05s);
        
        // input
        bool readContractsFromInputFile = default(false);
        string contractInputFilename = default("");
        
        // record
        string outputFilename = default("output/output.txt");
        bool write2file 	  = default(true);
        
        // WIFI connection quality check        
        int nPacketLossPoor      = default(2);
        int nPacketLossCritical  = default(4);
        
        // transition/action to be taken based on wifi connection
        bool upgradationEnabled = default(true);
        bool degradationEnabled = default(true);
        bool gapControlEnabled  = default(true);
        
        
        // minimum distance between two vehicles
        double minSafetyDistance @unit("m")   = default(2m);
        double maxDeceleration @unit("mpsps") = default(-8.0mpsps);
        
        // actions to be taken on transition if enabled actionOnTransition
        bool actionOnTransitionEnabled        = default(true);
        double accHeadwaytimeGap @unit("s")   = default(1.2s);
        double ploegHeadwayTimeGap @unit("s") = default(0.5s);
        double caccConstantSpacing @unit("m") = default(5m);
        
        double emergencyPloegHeadwayTimeGapFactor = default(.25);
        double emergencyCaccConstantSpacingFactor = default(.25);
        
    gates:
        input lowerLayerIn; // from mac layer
        output lowerLayerOut; // to mac layer
        input lowerControlIn;
        output lowerControlOut;
}