

#ifndef SRC_VEINS_MODULES_APPLICATION_PLATOONING_RUNTIMEMANAGER_RMUTILITY_H_
#define SRC_VEINS_MODULES_APPLICATION_PLATOONING_RUNTIMEMANAGER_RMUTILITY_H_

#include <limits>
#include <memory>
#include <vector>
#include <string>
#include "veins/modules/application/platooning/CC_Const.h"

// These preprocessor variables are used for debugging purpose
#define DEBUG_RM 1
#define DEBUG_RM1 0
#define DEBUG_RM2 0


// Forward declaration
class StateParameter;
class Assumption;

std::ostream &operator<<(std::ostream &os, const Plexe::ACTIVE_CONTROLLER value);

int controller2int(const Plexe::ACTIVE_CONTROLLER controller);

namespace RM {
    /**
         * This structure represents the RMLog, that is the information that RM
         * stores onPlatoonBeacon from leader or front vehicle
         */
        struct RMParameters {
            bool rmEnabled;
            double rmMonitorInterval;
            double expectedBeconInterval;

            // Contract InputFile
            bool readContractsFromInputFile;
            std::string contractInputFilename;

            // output
            std::string outputFilename;
            bool write2file;

            int nPacketLossPoor;
            int nPacketLossCritical;

            bool upgradationEnabled;
            bool degradationEnabled;
            bool gapControlEnabled;

            double minSafetyDistance;
            double maxDeceleration;

            bool actionOnTransitionEnabled;
            double accHeadwaytimeGap;
            double ploegHeadwayTimeGap;
            double caccConstantSpacing;

            double emergencyPloegHeadwayTimeGapFactor;
            double emergencyCaccConstantSpacingFactor;
        };

        /**
         * These structures represents logs for different vehicles (own, front, leader)
         *
         *
         */

        /**
         * This is common parameters of front and leader vehicle
         */
        struct RMLog {
            bool c2xInitiated = false;                                      // helper variable to decide whether logging has started or not
            double time;                                                    // information generated time
            double lastBeaconArrivalTime;                                   // received time of last beacon (can be done based on time)
            double acceleration;                                            // Acceleration of the vehicle
            double maxDeceleration = std::numeric_limits<double>::max();    // a default large value
            double controllerAcceleration;                                  // Acceleration computed by the Controller
        };

        /**
         * This is the log for the own vehicle
         */
        struct RMLog_Own {
            Plexe::ACTIVE_CONTROLLER activeController;
            double time;                                                    // information generated time
            double acceleration;                                            // Acceleration of the vehicle

            // There is possibility that we will write the following to output before setting values,
            // so we define some default value (that van not be the value) to test during writing to output
            double maxDeceleration = std::numeric_limits<double>::max();    // a default large value
            double dist2pred       = std::numeric_limits<double>::max();    // a default large value
            bool crashed           = false;                                 // default value is false

            double currentAccH        = -1.0;        // acc headway time gap with a negative value : to output current value
            double currentPloegH      = -1.0;        // ploeg headway time gap with a negative value : to output current value
            double currentCaccSpacing = -1.0;        // cacc spacing with a negative value : to output current value

            std::shared_ptr<std::vector<std::shared_ptr<Assumption>>> assumptions;
        };
        /**
         * This is the log for the front vehicle
         */
        struct RMLog_Front {
          RMLog common;
          double distance;      // Currently not using...Using it in RMLog_Own
          // TODO more parameters
        };

        /**
         * This is the log for the leader vehicle
         */
        struct RMLog_Leader {
          RMLog common;
          // TODO more parameters
        };


        // Some companion type
        using rm_log = std::tuple<RMLog_Own, RMLog_Front, RMLog_Leader>;
}

#endif /* SRC_VEINS_MODULES_APPLICATION_PLATOONING_RUNTIMEMANAGER_RMUTILITY_H_ */
