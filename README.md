![image](images/user_manual/logo_mdh.jpg)\
------------------------------------------------------------------------

\
\

------------------------------------------------------------------------

\

Shahriar Hasan and Joseba Gorospe\

\

Introduction
============

This document describes how to download, install, and use the
PlatoonSAFE simulator. The simulator is developed as an extension of the
Plexe[^1] simulator which is an OMNeT++-based platooning simulator. In
Plexe, several control algorithms, platoon maneuvering are implemented.
Plexe is developed as an extension of the popular VANET simulator
Veins[^2] in which the IEEE 802.11p protocol stack is implemented. In
addition, realistic channel models are available. Both Plexe and Veins
facilitates realistic large scale simulations through the SUMO
simulator. As the PlatoonSAFE simulator is an extension of the Plexe
simulator, it inherits all the features of Plexe and Veins. The
PlatoonSAFE simulator is mainly intended for enabling the simulation of
platoon safety applications. This includes the implementation of several
emergency braking strategies that a user can use to evaluate which one
is more suitable towards attaining the fail-safe state in platooning.
Moreover, several safety contracts are implemented in the PlatoonSAFE
simulator to facilitate the evaluation of fail-operational platooning in
a dense traffic scenario.

The PlatoonSAFE simulator is developed on the `Plexe 2.1` version. On 27
April 2021, a new version of Plexe was released (`Plexe 3.0`) and the
instructions for installing the `Plexe 2.1` was removed from the
website. The installation of the PlatoonSAFE simulator is the same as
the installation of `Plexe 2.1`. Hence, the installation guide is
repeated in this document. However, please refer to the Plexe website
(FAQ section) for common errors while installing the PlatoonSAFE
simulator; they should not be any different from installing the Plexe
simulator.

Downloading and Installing PlatoonSAFE
======================================

The Plexe simulator (V. 2.1) requires installing Plexe-Veins 2.1 and
Plexe-SUMO 2.1. The PlatoonSAFE simulator only extends the Plexe-Veins
2.1, but no modifications are performed in the Plexe-SUMO 2.1 part.
Therefore, a user of the PlatoonSAFE simulator needs to download the
PlatoonSAFE simulator from this git repository. In addition, the
Plexe-SUMO 2.1 has to be downloaded from the download section of Plexe
website[^3].

The PlatoonSAFE simulator has been tried in Ubuntu 16.04, 18.04, and
20.04. For installing in MAC OS, please follow the guide of Plexe
documentation 1.1[^4] (SUMO installation). Please follow the following
steps to have a working version of PlatoonSAFE in Ubuntu:

1.  Install OMNeT++ in your OS. The detailed installation guide for
    several operating systems can be found in OMNeT++ website[^5].

2.  Download PlatoonSAFE and Plexe-SUMO 2.1 and unzip them. Create a
    folder called `src` in your home directory (Ubuntu) and copy
    PlatoonSAFE and Plexe-SUMO 2.1 in there.

3.  Some third party libraries are required to be installed for building
    Plexe-SUMO 2.1. Run the following command

            sudo apt-get install libgdal-dev libproj-dev \ 
            libxerces-c-dev libfox-1.6-dev libtool \ 
            autoconf

4.  Configure and install SUMO using the following commands

                cd ~/src/plexe-sumo
                make -f Makefile.cvs
                ./configure
                make

5.  Add the `bin` directory of SUMO at the end of your `.bashrc` file.

                gedit ~/.bashrc
                export PATH=$PATH:$HOME/src/plexe-sumo/bin

    Please feel free to use any editor other than `gedit`.

6.  Reopen the terminal and type SUMO. You should see something as
    follows; otherwise, the SUMO installation has not been done
    properly.

         Eclipse SUMO Version git
         Build features: x86_64-pc-linux-gnu GDAL GUI
         Copyright (C) 2001-2018 German Aerospace Center (DLR) and others; 
         http://sumo.dlr.de
         License EPL-2.0: Eclipse Public License Version 2 
         <https://eclipse.org/legal/epl-v20.html>
         Use --help to get the list of options.

7.  Now build the PlatoonSAFE using the following commands

                cd ~/src/PlatoonSAFE-1.1
                make -f Makefile
                make MODE=release

8.  Now type `omnetpp` in your terminal to open the IDE and import
    PlatoonSAFE-2.1 (File $\rightarrow$ import $\rightarrow$ Existing
    projects into workspace $\rightarrow$ select root directory (browse
    into PlatoonSAFE-1.1) $\rightarrow$ finish). Make sure that your
    build configuration is in `gcc-release` mode (right click on
    PlatoonSAFE-1.1 $\rightarrow$ set active $\rightarrow$ gcc-release).

9.  Now go to PlatoonSAFE $\rightarrow$ examples $\rightarrow$
    safetyApps, and run `RMModule.ini`. In order to run it, right click
    on `RMModule.ini` $\rightarrow$ Run As $\rightarrow$ OMNeT++
    Simulation. An window should pop-up, select Sinusoidal in the Config
    name and 2 in Run number, and press OK. On the left-top corner of
    the OMNeT++ Qtenv window, click on either of the fast or express
    buttons. This should start the simulation, and a SUMO window should
    pop-up. At this point, you should see the platooning vehicles.

10. 
        opp_run -m -u Qtenv -c RTM-SB-SVR -n .:../veins:../../src/veins --image-path=../../images -l ../../src/veins RTMModule.ini

Please note that running the simulation in GUI has been instructed
above. However, in order to run large scale parallel simulations, you
will need to run the simulations in *Cmdenv* mode. Please refer to
OMNeT++ manual[^6] to learn how to run simulations using `Cmdenv`. For
any issues with installation, please first go to the `Building` and
`FAQ` tabs of the Plexe website where several common installation issues
are discussed. Also, refer to `OMNeT++ Users` group in google groups and
stackoverflow for OMNeT++-related issues.

PlatoonSAFE Simulation Parameters
=================================

In this section, the parameters used to simulate the fail-operational
and fail-safe features in the PlatoonSAFE simulator are explained.

Runtime Manager parameters
--------------------------

The parameters of Plexe simulator that specifies the PHY and MAC layer
specifications, neighbouring traffic, beacon frequencies, controller
parameters, and the simulated scenarios, are available in the
`omnetpp.ini` file. Please refer to the Plexe documentation for the
details of these parameters. The `RMModule.ini` file that specifies the
Runtime Manager parameters inherits the parameters of `omnetpp.ini`
file, Listing [\[rmModule\]](#rmModule){reference-type="ref"
reference="rmModule"}.

``` {#rmModule .numberLines numbers="left" caption="\\texttt{RMModule.ini}" label="rmModule"}
include omnetpp.ini
*.node[*].runtimeManager.rmEnabled = true
*.node[*].runtimeManager.rmMonitorInterval = 0.1 s
*.node[*].runtimeManager.expectedBeconInterval = .105 s
*.node[*].runtimeManager.readContractsFromInputFile = false
*.node[*].runtimeManager.contractInputFilename  =  
"/home/shahriar/plexe-veins-plexe-2.1/src/veins/modules/
application/platooning/runtimeManager/input/contracts.txt"
*.node[*].runtimeManager.outputFilename = 
"/home/shahriar/plexe-veins-plexe-2.1/src/veins/modules/
application/platooning/runtimeManager/output/output"
*.node[*].runtimeManager.write2file = true

*.node[*].runtimeManager.nPacketLossPoor = 3
*.node[*].runtimeManager.nPacketLossCritical = 6

*.node[*].runtimeManager.upgradationEnabled = true
*.node[*].runtimeManager.degradationEnabled = true
*.node[*].runtimeManager.gapControlEnabled  = true

*.node[*].runtimeManager.minSafetyDistance = ${minSafetyDistance = 2}m
*.node[*].runtimeManager.maxDeceleration   = -8.0mpsps

*.node[*].runtimeManager.accHeadwaytimeGap = ${headway}s
*.node[*].runtimeManager.ploegHeadwayTimeGap    = ${ploegH}s
*.node[*].runtimeManager.emergencyPloegHeadwayTimeGapFactor = 0.25
*.node[*].runtimeManager.caccConstantSpacing    = ${caccSpacing}m
*.node[*].runtimeManager.emergencyCaccConstantSpacingFactor = 0.25
```

The parameters of the Listing
[\[rmModule\]](#rmModule){reference-type="ref" reference="rmModule"} are
explained below:

-   Line 2: The `rmEnabled` has to be set `true` in order to use the
    runtime manager, otherwise, default Plexe simulator will be run.

-   Line 3: The interval at which the runtime manager checks the
    communication quality, safety distance, etc., based on which the
    state of the vehicle may change, i.e., controller change, gap
    adjustment, etc.

-   Line 4: The interval at which a beacon is expected from the lead or
    front vehicle. This has to be set according to the beacon frequency
    of the platooning vehicles. For instance, if the beacon frequency of
    the platooning vehicles is 10 Hz then a beacon is expected every 100
    ms. In addition, 5 ms is assumed as other delays. Therefore, if a
    vehicle does not receive a beacon for the last 210 ms from the lead
    vehicle, then 2 packet losses are assumed with respect to the lead
    vehicle.

-   Lines 5-8: These parameters are used for specifying the user defined
    contracts. If it is set to `true` then the default contracts are
    ignored.

-   Lines 10-12: If there is a safety violation, i.e., the gap between
    the vehicles become less than a predefined inter-vehicle distance
    then safety condition is assumed to be violated, and it is recorded
    in an output file.

-   14-15: These two parameters define the thresholds for *fair, poor*
    communication thresholds. Please notice that the communication
    qualities are divided into three categories and named *good, fair,
    poor* in the PlatoonSAFE paper. However, in the simulator they are
    called OK, POOR, CRITICAL. They are basically the same concept.
    During the time of developing the simulator we assumed these names.
    In a later version of the simulator, these names will be changed.
    For now, please notice that *good*, *fair* and *poor* in the paper
    corresponds to *OK*, *POOR*, and *CRITICAL*, respectively, in the
    simulator.

-   Lines 24-28: In the paper, we mention three controllers, ACC, CACC,
    and PLATOON, and there is a clear distinction between the CACC and
    PLATOON controllers[^7]. In the Plexe simulator, the
    ACC, CACC, and PLATOON controllers (as termed in the PlatoonSAFE
    paper) are called as ACC, Ploeg, and CACC, respectively. The time
    headway or distance gap of the controllers are read from the
    omnetpp.ini file. The factor by which the distance with the front
    vehicle is to be increased due to *fair* communication quality is
    specified in Lines 26 and 28.

Emergency Braking Strategies' Parameters
----------------------------------------

The parameters required for enabling the simulation of the emergency
braking strategies are specified in the `omnetpp.ini` file. In order to
model the neighbouring traffic, the number of human vehicles, their
beacon frequency, and their insertion time in the simulation are to be
specified. Please note that the runtime manager, and the emergency
braking strategies implemented in the PlatoonSAFE simulator are driven
by the fact that there can be large number of packet losses in a dense
traffic scenario, and the safety applications must account for this.
Therefore, a user of the PlatoonSAFE simulator is advised to include a
large number of human vehicles in the simulation to see the benefits of
the runtime manager and the emergency braking strategies. This is
important for inducing a scenario with packet losses and contention in
the MAC layer. Simulation of dense traffic scenarios and the
communication delays caused by them are a big motivation for simulation
studies as such scenarios are difficult to test on the real and busy
highways.

The time headway of the ACC and Ploeg controllers are specified by the
`headway` and `PloegH` parameters in the *omnetpp.ini* file. The
constant distance gap of the CACC controller is specified by the
`caccSpacing` parameter. At the end of the *omnetpp.ini* file, there are
several `configs`, e.g., Sinusoidal, NB, SB, ESB, AEB, and CEBP. These
are the configuration parameters for the braking strategies. In these
`configs`, if `rmEnabled` is set to `true` then the runtime manager will
be used until the braking starts. However, if a user only wants to
simulate the braking strategies, they need to set `rmEnabled` to
`false`. The parameters associated with the braking strategies are self
explanatory; hence, they are not discussed in details here. In case of
`config SB` `config ESB`, a user needs to specify the average waiting
time of before the platoon brakes that is found from the average
communication delay. Moreover, the DENM frequency is also required to be
specified, and it can be different from the CAM frequencies. In the
`config AEB` and `config CEBP`, the ACK frequency is assumed to be the
same as the DENM frequency. Moreover, the `softDecelRate` is required to
be set in case of ESB and AEB strategies. Please refer to the
PlatoonSAFE paper for the details of the braking strategies work.

In the `configs`, e.g., Sinusoidal, NB, SB, ESB, AEB, and CEBP, there is
a common parameter called `manager.useGui`. If this parameter is set
`true`, then the SUMO GUI will be shown during the runtime. If you run
large scale batch simulations, do not forget to set this parameter
`false`; otherwise, the simulation will take a very long time to
execute.

Implementation details of Runtime Manager
=========================================

The code of the Runtime Manager can be found in the directory\
$src/veins/modules/application/platooning/runtimeManager$.\
The Runtime Manager is implemented as a separate module in the Plexe
simulation framework. platooning application can access to it through
the `onPlatoonBeacon()` method of the `Runtime Manager` interface. This
method is called from platooning application every time a vehicle
receives a beacon from any of the vehicle in the platoon. Runtime
manager is designed to perform three different tasks: logging,
monitoring and taking actions based on monitored state of the vehicle.
Logging is done for front, leader and ego vehicles. Front and lead
vehicle's data are logged whenever the ego vehicle receives platooning
beacon, and this is invoked from the `onPlatoonBeacon()` method of
platooning application. `Runtime Manager` logs the ego vehicle's
information to be used in the safety violation check during monitoring.
Monitoring by `Runtime Manager` is carried out periodically at an
interval of `rmMonitorInterval`. Upon receiving a monitoring
`selfMessage`, runtime manager starts logging the information of the ego
vehicle required for safety violation check, such as currently active
controller (`ACC, CACC, Ploeg`), maximum deceleration, distance to the
front vehicle, and the simulation time when the data was computed. These
information are then processed in safety evaluation step and reported to
an output file if distance to the front vehicle or maximum deceleration
violate the user defined threshold values. The methods implementing the
logging, evaluating, monitoring, and scheduling tasks can be found in
the `runtimeManager.cc` file. Moreover, in the *runtimeManager.\**
class, vehicle colors are set to demonstrate the active controller
employed in a vehicle that can be seen in the SUMO GUI.

Assumption/Guarantee
--------------------

`Assumption` represents the current state of the ego vehicle, e.g.,
currently active controller, Connection to Front (C2F) vehicle,
Connection to Leader (C2L) vehicle, etc. For a set of assumptions, the
RM performs the task defined by the `Guarantee`. An assumption/Guarantee
pair results in a new data type named a `Contract`. Thus, a contract
represents a guaranteed performance ensured by the RM for the current
state of the ego vehicle.

*Runtime Manager* defines the C2F and C2F using the `WIFIAssumption`
class that is derived from the `Assumption` base class. The base class
contains the data member `ACTIVE_CONTROLLER` that represents the
controllers. Code snippet in Listing
[\[listing:assumption\]](#listing:assumption){reference-type="ref"
reference="listing:assumption"} depicts the notable data members and
member functions of `Assumption` class and the derived class
`WIFIAssumption`. The `WIFIAssumption` class can be found in the
directory\
`runtimeManager/assumptions`.

``` cpp
class Assumption {
public:
    virtual void evaluate(parameter list);
protected:
    ACTIVE_CONTROLLER controller;
};

class WIFIAssumption : public Assumption {
public:
    void evaluate(parameter list) override;
private:
    C2F c2f;
    C2L c2l;
};
```

In the current implementation, both `C2F` and `C2L` contain a single
data member to represent the connection quality. However, we define two
classes to represent the front and lead vehicle's connection quality,
and they are derived from the same indirect base class
`runtimeManager/StateParameter*`. This is done to facilitate a distinct
extension of the C2F and C2L class types. For instance, if a user wants
to add another data member to the C2F representing the distance to the
front vehicle such that a controller switching will be triggered based
on both these criteria, they can do so in the `C2F` class. The user do
not need to make changes in the C2L class in this case. The
communication quality of the C2L and C2F are categorized into OK, POOR,
and CRITICAL which are measured based on the number of consecutive
packet loss from the front and lead vehicles, Listing
[\[listing:wifi\_quality\]](#listing:wifi_quality){reference-type="ref"
reference="listing:wifi_quality"}. The C2L, C2F, `WIFI_QUALITY` can be
found in the `runtimeManager/StateParameter*` directory.

``` cpp
enum WIFI_QUALITY {
    CRITICAL,
    POOR,
    OK,
};
```

Every state of `Assumption` may or may not have an associated
`Guarantee` which dictates the action to be taken by the
`Runtime Manager`. The default assumption/guarantee `contracts` of the
PlatoonSAFE simulator can be found in the directory\
`runtimeManager/contracts.*`. Every `Contract` in the contract list in
`Runtime Manager` is stored as `key-value` pair using
`C++ Standard Template Library (STL)`. The STL provides the `map`
container, where `Assumption` is used as `key` and `Guarantee` is the
corresponding `value`. If the `Runtime Manager` detects any change in
communication quality perceived by the ego vehicle, it looks for the
corresponding `Guarantee` by iterating through the contracts list and
performs the action specified by the `Guarantee` through a call of the
`operator()` method. Current implementation of `Runtime Manager` deals
with three different kind of `Guarantee`, e.g., `ChangeController`,
`AdjustGap2Front`, and `ChangeControllerAndAdjustGap2Front`. The first
two are derived from the `Guarantee` class, and
`ChangeControllerAndAdjustGap2Front` has `ChangeController` and
`AdjustGap2Front` as direct base classes. Listing
[\[listing:contract\_guarantee\]](#listing:contract_guarantee){reference-type="ref"
reference="listing:contract_guarantee"} depicts the basic structure of
the `Guarantee` and its derived classes. In case of `ChangeController`,
the `Runtime Manager` changes the `ACTIVE_CONTROLLER` of the ego vehicle
to the controller indicated by the data member `to`, and this is
performed through the `setActiveController` method of the `TraCI`
interface. `AdjustGap2Front` class is responsible for adjusting the time
gap or constant distance gap with respect to the front vehicle. The\
`ChangeControllerAndAdjustGap2Front` designates the functions to be
performed by its base classes `ChangeController` and `AdjustGap2Front`.
The classes associated with the guarantees can be found in the directory
`runtimeManager/guarantees`.

``` cpp
enum class GAP2FRONT {
    DEFAULT,
    INCREASE,
};
class Guarantee {
public:
    virtual void operator()(std::shared_ptr<Assumption> assumption) const;
};

class ChangeController : virtual public Guarantee {
public:
    void operator()(std::shared_ptr<Assumption> assumption) const override;
protected:
    ACTIVE_CONTROLLER to;
};

class AdjustGap2Front : virtual public Guarantee {
public:
    void operator()(std::shared_ptr<Assumption> assumption) const override;
protected:
    GAP2FRONT gap2front;
};

class ChangeControllerAndAdjustGap2Front : public AdjustGap2Front, public ChangeController {
public:
    void operator()(std::shared_ptr<Assumption> assumption) const override;
};
```

In the implementation of `Runtime Manager`, polymorphism has been
extensively used. Moreover, the variables, e.g., `key`, `value` pairs of
contract list, default assumptions, etc., are declared through the base
class or a pointer to the base class where possible. The aim was to ease
the possible extension of the PlatoonSAFE simulator. For instance, if a
user wants to implement a new `Assumption`, they only require to derive
a new class from the base class and override the `operator()` method.

User Defined Contracts
----------------------

The PlatoonSAFE simulator facilitates user defined contracts. This is
motivated by the fact that a user can define their own contracts without
having deep knowledge on the OMNeT++ simulation framework. It is
possible to modify the `.txt` file to accommodate such user defined
contracts. The file is located in the directory
`runtimeManager/input/contract.txt`. A parser is developed to parse the
input to the OMNeT++ `contract` class type. The parser grammar can be
found in the directory `runtimeManager/rmParser/parserGrammars`. If a
user defines their own contracts they need to set the
`readContractFromInputFile` parameter in the `RMModule.ini` file to
`true`; in that case, the default contracts will be ignored.

``` {#listing:contract_format numbers="none" caption="Input format for user defined {\\tt Contract}" label="listing:contract_format"}
::contract[Type : comma/space separated Assumption variables : 
comma/space separated Guarantee variables]
```

The input format of a user defined contract is depicted in Listing
[\[listing:contract\_format\]](#listing:contract_format){reference-type="ref"
reference="listing:contract_format"}. The components of `Contract`,
`Type`, `Assumption`, and `Guarantee` are defined as a collection of
`key` and `value` pair. The keys and their corresponding values
available in the current PlatoonSAFE implementation are depicted in
Table [1](#table:contract_input_key_value){reference-type="ref"
reference="table:contract_input_key_value"}. For instance, the
components of Assumption are as Listing
[\[listing:contract\_assumption\_order\]](#listing:contract_assumption_order){reference-type="ref"
reference="listing:contract_assumption_order"}, and their corresponding
values are to be chosen from the Table
[1](#table:contract_input_key_value){reference-type="ref"
reference="table:contract_input_key_value"}. Similarly, the components
of the Guarantee are depicted in Listing
[\[listing:contract\_guarantee\_order\]](#listing:contract_guarantee_order){reference-type="ref"
reference="listing:contract_guarantee_order"}, and the values should be
chosen from the Guarantee row of Table
[1](#table:contract_input_key_value){reference-type="ref"
reference="table:contract_input_key_value"}.

``` {#listing:contract_assumption_order numbers="none" caption="Order of pairs of {\\tt Assumption} component" label="listing:contract_assumption_order"}
c2f=value ; c2l=value ; mode=value
```

``` {#listing:contract_guarantee_order numbers="none" caption="Order of pairs of {\\tt Guarantee} component" label="listing:contract_guarantee_order"}
transition2mode=value ; dist2pred=value
```

<table>
    <tr>
        <td><b>Component</b></td>
        <td><b>key</b></td>
        <td><b>value</b></td>
    </thead>
    <tr>
        <td><i>Type</i></td>
        <td>ctype</td>
        <td>wifi</td>
    </tr>
    <tr>
        <td rowspan="3"><i>Assumption</i></td>
        <td>c2f</td>
        <td>ok/poor/critical</td>
    </tr>
    <tr>
        <td>c2l</td>
        <td>ok/poor/critical</td>
    </tr>
    <tr>
        <td>mode</td>
        <td>acc/ploeg/cacc</td>
    </tr>
    <tr>
        <td rowspan="2"><i>Guaranteee</i></td>
        <td>transition2mode</td>
        <td>acc/ploeg/cacc</td>
    </tr>
    <tr>
        <td>dist2pred</td>
        <td>default/increase</td>
    </tr>
</table>


Based on the rules defined above, there are two possible formats that a
user can use to define their contracts, and it is depicted in Listing
[\[listing:contract\_option\]](#listing:contract_option){reference-type="ref"
reference="listing:contract_option"}. The first format is to be used
when one of the guarantees is provided, and the second one is to be used
when both the guarantees are provided.

``` {#listing:contract_option caption="Two possible combination of {\\tt Contract} input format" label="listing:contract_option"}
::contract[ctype=value : c2f=value ; c2l=value; mode=value : transition2mode=value]
::contract[ctype=value : c2f=value ; c2l=value; mode=value : transition2mode=value;
dist2pred=value]
```

Implementation of the Braking Strategies
========================================

The emergency braking strategies are implemented in the\
`src/veins/modules/application/platooning/protocols/BaseProtocol.*`
files of the Plexe simulator. The reason is that the developers of the
Plexe simulator implement the creation of CAMs and the handling of the
CAMs received from other vehicles in these files. In addition, we create
two other message types called `BrakingPacket` and `ackPkt`. The
`BrakingPacket` acts as the DENM, and the `ackPkt` acts as the ACKs
required in the AEB and CEBP strategies. For the details of how to
create messages in OMNeT++, please refer to the OMNeT++ user manual.

In the initialization of the `BaseProtocol.*`, we first check if the
vehicle is the lead vehicle in the platoon and whether the time at which
the imaginary road hazard is detected is reached already. Based on this,
the lead vehicle in the platoon schedules a `selfMessage` at the
`brakeAtTime` which is when the imaginary road hazard takes place. At
`brakeAtTime`, a `BrakingPacket` is generated by the lead vehicle, and
it is broadcasted using the `sendBrakingMessage(-1)` method. The braking
packet is repeated every `DENMInterval` using the `scheduleAt` function.
In this case, it acts as recursion. Please remember that when a node
wants to send a message to itself at a particular time using the
`scheduleAt` function, the message is captured by the `handleSelfMsg`
function.

When a node receives a packet from an another vehicle, the application
layer messages can be handled at the `handleUnicastMsg` method of Plexe.
Therefore, to implement the braking strategies, we use this method to
capture the braking packets (DENMs) and the ACKs. To this end, we first
check if the DENM or ACK received is the first one received by the ego
vehicle, otherwise the duplicate messages are discarded. This is
necessary because we do not want the vehicles to start braking every
time it receives a DENM that is broadcasted by the lead vehicle or
relayed by an intermediate vehicle. As we are considering only one
hazard, one DENM is enough to invoke the braking command of the ego
vehicle.

In case of synchronized braking, when a vehicle receives a DENM, it
schedules the braking action at `brakeAtTime + sbWaitTime` using the
`scheduleAt` function. However, if a vehicle does not receive a DENM by
`brakeAtTime + sbWaitTime`, it starts braking as soon as it receives a
DENM. The braking of the vehicles is achieved through the
*setFixedAcceleration* method in which the desired deceleration rate of
the vehicle can be specified in its argument. The implementation of the
ESB strategy is similar to the SB strategy, except the vehicles perform
soft deceleration until the `sbWaitTime` using the
*setFixedAcceleration* method. Moreover, the last vehicle in the platoon
does not perform synchronized braking, i.e., it starts braking as soon
as it receives a DENM.

In the CEBP strategy, it is first checked if the ego vehicle is the last
vehicle in the platoon. When the last vehicle receives a DENM, it
performs deceleration at a full rate and schedules a `ackMsg` using the
`scheduleAt` function. Then, a `ackPkt` is broadcasted by the last
vehicle using the `sendAckMessage(-1)` method. When an `ackPkt` is
received at the `handleUnicastMsg` method, similar duplicacy checking as
the `BrakingPkt` is performed. In addition, it checked if the ACK
received is from the immediate following vehicle. If so, another
`ackMsg` is scheduled that in turn broadcasts another `ackPkt` with its
own vehicle id. The distinction of the ACK packets received by a vehicle
is made through the checking of the vehicle id field in the packet using
the `getIdOfVehicle()` function. The implementation of the AEB strategy
is similar to the CEBP strategy except soft-braking is performed by the
vehicles while they are waiting for the ACK to be received. However, we
must make sure that an already in progress soft-deceleration is not
interrupted by another soft-deceleration, or a full-deceleration is not
interrupted by a soft-deceleration. The necessary conditions to ensure
these can be found in the *handleSelfMsg* function.

Results collection and Analysis
===============================
PlatoonSAFE includes several scripts to help the user collect and analize results of simulations. All these scripts are located in `src/examples/human/externalScripts/` and they can be used for the following purposes:

1. **Automatize simulations** to run different scenarios.

2. **Run Neural Network** simulations with PlatoonSAFE.

3. **Extract information** from `.vec` files.

4. **Evaluate ML algorithms** from simualtion reuslts.

`runSimulations.py` is the main script that executes all the scenarios defined in `simParam.py`. It will run all the simulations, parse the .vec files into .csv files, and save all the results (collision, stop time and stop distance) into a unique file with. The user shall adapt the scenarios that wants to execute in `simParam.py`. We have defined some of the parameters that can be changed, but new ones can be added. For this, it is necessary to include how the parameter is defined in the `.ini` file ant in which line. `simParamHelper.py` can be used to check easier in which lines are located the parameters. This script should be run from its folder, it they are moved to somewhere else, all paths in the script shall be adapted.

```
cd src/examples/human/externalScripts/
python3 runSimulations.py
```

`runSimNN.py` is the main script that runs the simulations using the NN defined in `NNServerThread.py`. It connects to PlatoonSAFE simulator via UDP to exchange communication delays in the LeadVehicle of the platoon and predict thefuture ones. This script is only prepared to run one scenario, but with multiple seeds. The structure of the NN can be changed in `NNServerThread.py`. UDP connection can also be changed (port, address). Remember to adapt boths sides, `NNServerThread.py` and `AIAlgotihms.cc` (function *getNNServerAdd*)This script should be run from its folder, it they are moved to somewhere else, all paths in the script shall be adapted.

```
cd src/examples/human/externalScripts/
python3 runSimNN.py
```

`simUtils.py` contains a function *createResultCSV* that converts resutls from PlatoonSAFE (`.vec`) into `.csv` files with the following structure: *ParameterName*, *VehicleID*, *SimulationTime*, *ParameterValue*. During this format conversion, the script also computes three important metrics required for analysing emergency braking strategies: inter-vehicle collisions, stopping distance of the LV, and the total time required for the whole platoon to transition to the fail-safe state from when braking started. `resultsFromVec.py` contains an example of how this function can be used to extrac results from several vector files obtained after executing several simulations.

`errorPlot.py` is a script that shows how the performance of the ML algorithms can be evaluated. in order to replicate this script, it is necessary to extract csv files of the predicted delay and the error of the algorithms from the vector files. To do that:
1. Open vector file in OMNet++
2. Select delay/error vector
3. Rigth click -> Export Data -> CSV for SpreadSeeths.
4. Save them with specific names and adapt the code below for that

Once the csv files are created, `joinResultsCSV` shall be used to combine delays and errors of all the algorithms into the same file, and with these files, use `errorPlot.py` to calculate the Root Mean Squared Error of all algorithms and plot the accumulative sum of the errors.

All these scripts contain explanations by themself, but it is recommendable to check them, specially regarding PATHS used inside.



References
===========

[^1]: http://plexe.car2x.org/

[^2]: https://veins.car2x.org/

[^3]: http://plexe.car2x.org/download/

[^4]: http://plexe.car2x.org/documentation/plexe-1.1-documentation.pdf

[^5]: https://doc.omnetpp.org/omnetpp/InstallGuide.pdf

[^6]: https://doc.omnetpp.org/omnetpp/manual/

[^7]: Steven E. Shladover, Christopher Nowakowski, Xiao-Yun Lu, and Robert Ferlis.Cooperative adaptive cruise control: Definitions and operating concepts. *Transportation Research Record*, 2489(1):145–152, 2015.
