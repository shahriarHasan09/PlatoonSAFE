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

packet BrakingPacket{
    double brakingTime = 0;
    double currentSimTime = 0;
    double decelarationRate = 0;
    double speedOfLeader = 0;
    int idOfVehicle = 0;
    int seqNumber = 0;
    double predictedDelayOfLastVeh = 0;
    double SynchBrkAvgWaitTime = 0;
};