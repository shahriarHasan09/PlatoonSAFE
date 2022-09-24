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

This document describes how to download, install, use, and modify the
PlatoonSAFE simulator. PlatoonSAFE is developed as an extension of
[Plexe](http://plexe.car2x.org/), an OMNeT++-based platooning simulator. In
Plexe, several control algorithms, platoon maneuvering operations, and mixed traffic scenarios are implemented.
Plexe is built on top of the widely used VANET simulator
[Veins](https://veins.car2x.org/). Plexe and Veins
facilitates realistic and large scale simulations through [OMNeT++](https://omnetpp.org/) and [SUMO](https://sumo.dlr.de/docs/index.html). As PlatoonSAFE is an extension of Plexes, it inherits all the features of Plexe and Veins. In addition, PlatoonSAFE implements fault tolerance mechanisms for evaluating platoon safety applications during platoon cruising and emergency braking. To this end, a Runtime Manager (RTM) is implemented, which takes into account the transient communication outages during runtime and assigns an appropriate controller and/or adjusts the inter-vehicle gaps based on the duration of outages. Several emergency braking strategies are also implemented that can be used for evaluating their efficacy in transitioning a platoon to the fail-safe state. Moreover, PlatoonSAFE integrates Machine Learning (ML) algorithms such as Neural Network (NN) and Online Support Vector Regression (Online SVR) that can be used to predict communication delays during platooning runtime and design delay-aware emergency braking strategies and control algorithms. 

Downloading and Installing PlatoonSAFE
======================================
The PlatoonSAFE simulator extends the `Plexe 2.1` version, which consists of `Plexe-Veins 2.1` and `Plexe-SUMO 2.1`. In 2021, `Plexe 3.0` was released and the Plexe-SUMO part was integrated to the official SUMO simulator. However, we have not yet migrated the codes of PlatoonSAFE to the latest version of Plexe. The installation guides of PlatoonSAFE and `Plexe-SUMO 2.1` are provided below following the [Plexe 1.1 documentation](http://plexe.car2x.org/documentation/plexe-1.1-documentation.pdf). Note that we have only extended Plexe-Veins 2.1 to develop the PlatoonSAFE simulator, but no changes are performed in the Plexe-SUMO 2.1.  

## Simulation Environment ##
* PlatoonSAFE has been tested on Ubuntu 16.04, 18.04, and 20.04. For OS X and Windows installation, please follow the guidelines given in [Plexe website] and [Plexe 1.1 documentation](http://plexe.car2x.org/documentation/plexe-1.1-documentation.pdf). 
* We have tried PlatoonSAFE in OMNeT++ versions upto 5.6.2
* Several libraries and packages are required for running Plexe-SUMO, which are described below.
* The external libraries required for running the NN algorithm in PlatoonSAFE are described below.
* In order to use the OMNeT++ IDE you need to have java environment in the system. Moreover, both python and python3 are required for installing some configuration files. If your OS does not have them, consider running the following commands in the Linux terminal: 
````
sudo apt-get update
sudo apt-get install default-jre python python3
````

## PlatoonSAFE Installation guide in Linux ##
1. First, install OMNeT++; the detailed installation guide for different operating systems can be found in [OMNeT++ website](https://doc.omnetpp.org/omnetpp/InstallGuide.pdf). Note that the `libosgearth` package required for 3D visualization in OMNeT++ `Qtenv` mode is no longer provided in Ubuntu 22.04. If you are using Ubuntu 22.04, install `libosgearth` from the source.  

:exclamation:The libraries and packages required for installing OMNeT++ and Plexe-SUMO in Linux are listed together in the [Plexe website](http://plexe.car2x.org/building/) (under the heading _Install required libraries and tools_). You may consider installing these libraries and follow the [Step 1](http://plexe.car2x.org/building/) for an easy installation of OMNeT++. 

2. Now, [download PlatoonSAFE](https://github.com/shahriarHasan09/PlatoonSAFE), create a folder called `src` in your home directory, and unzip PlatoonSAFE. Alternatively, you can run the following commands to perform these taks from the terminal (_recommended_):

```
cd
sudo apt-get install git-all
mkdir src
cd src/
git clone https://github.com/shahriarHasan09/PlatoonSAFE.git
```

3. Download Plexe-SUMO 2.1 from the [Plexe website](http://plexe.car2x.org/download/) (zip archives), unzip it, and move it to the `src` folder you just created in the `home` directory. 

4. Install the following third party libraries required for building SUMO
````
sudo apt-get install libgdal-dev libproj-dev libxerces-c-dev libfox-1.6-dev libtool autoconf
````

5. Configure and install SUMO using the following commands

```
cd ~/src/plexe-sumo-plexe-2.1
make -f Makefile.cvs
./configure
make
```
6. Add the `bin` directory of SUMO at the end of your `.bashrc` file. First, 

```
gedit ~/.bashrc
```
Then copy and paste the following line at the end of the file:
```
export PATH=$PATH:$HOME/src/plexe-sumo-plexe-2.1/bin
```
:exclamation:The end of your `.bashrc` file should now look something like this:
```
export PATH=$HOME/omnetpp-5.6.1/bin:$PATH
export PATH=$PATH:$HOME/src/plexe-sumo-plexe-2.1/bin
```

7. Close the terminal, reopen it, and type in `SUMO`. You should see something like this: 
```
 Eclipse SUMO Version git
 Build features: x86_64-pc-linux-gnu GDAL GUI
 Copyright (C) 2001-2018 German Aerospace Center (DLR) and others; 
 http://sumo.dlr.de
 License EPL-2.0: Eclipse Public License Version 2 
 <https://eclipse.org/legal/epl-v20.html>
 Use --help to get the list of options.
```

8. Now, build PlatoonSAFE using the following commands:
````
cd ~/src/PlatoonSAFE
./configure
make
````
The installation process is now completed with this step.

## Running Simulations ##
You can run the simulations from the IDE or the terminal. From IDE, simulations can be run in `Qtenv` mode to use the OMNeT++ and SUMO GUIs, and in `Cmdenv` mode, simulations run faster. A detailed guide on how to use the IDE, run the simulations, collect results, and take the advantage of the strong GUI facilitated by OMNeT++, please follow the [OMNeT++ IDE user guide](https://doc.omnetpp.org/omnetpp/UserGuide.pdf). A few instructions are provided below just to get you started:

1.  Type in `omnetpp` in your terminal to open the IDE and import
    PlatoonSAFE (File $\rightarrow$ import $\rightarrow$ Existing
    projects into workspace $\rightarrow$ select root directory (browse
    into PlatoonSAFE folder located in the `src`) $\rightarrow$ finish). Make sure that your
    build configuration is in `gcc-release` mode (right click on
    PlatoonSAFE $\rightarrow$ set active $\rightarrow$ gcc-release).

2.  Now, go to PlatoonSAFE $\rightarrow$ examples $\rightarrow$
    safetyApps, and run [RTMModule.ini](examples/human/RTMModule.ini). In order to run it, right click
    on [RTMModule.ini](examples/human/RTMModule.ini) $\rightarrow$ Run As $\rightarrow$ OMNeT++
    Simulation. An window should pop-up, select the configuration you want to run, e.g., sinusoidal-RTM, then press OK. On the top-left corner of
    the OMNeT++ Qtenv window, click on either of the fast or express
    buttons. This should start the simulation and a SUMO window should
    pop-up. At this point, you should see the platoon cruising.

3. In order to run the simulations from the terminal, for example, you can run the following commands:
````
cd src/PlatoonSAFE/examples/human
opp_run -m -u Qtenv -c sinusoidal-RTM -n .:../veins:../../src/veins --image-path=../../images -l ../../src/veins RTMModule.ini
````

Please note that running simulations with GUI have been instructed above. However, in order to run large scale and parallel simulations, you will need to run the simulations in *Cmdenv* mode. Please refer to the [OMNeT++ User Guide](https://doc.omnetpp.org/omnetpp/UserGuide.pdf) (Chapter 6) to learn how to run batch simulations in `Cmdenv` mode from the simulation launcher. In addition, we provide some [python scripts](examples/human/externalScripts/runSimulations.py) in PlatoonSAFE to automatize running selective and batch simulations. 

For general issues related to OMNeT++, Plexe, or Veins, we recommend [Plexe FAQ section](http://plexe.car2x.org/faq/), [Plexe closed issues](https://github.com/michele-segata/plexe/issues?q=is%3Aissue+is%3Aclosed), [Veins google group](https://groups.google.com/g/omnetpp?label=veins), and [stackoverflow](https://stackoverflow.com/questions/tagged/omnet%2b%2b).

PlatoonSAFE Simulation Parameters
=================================

The simulation parameters required for evaluating RTM, ML algorithms and the braking strategies can be found in the files [RTMModule.ini](examples/human/RTMModule.ini) and [RTM-CEB-ML.ini](examples/human/RTM-CEB-ML.ini). The RTM parameters are listed in a separate `.ini` file in order to distinct them from the Plexe and Veins parameters. The `RTMModule.ini` file inherits the parameters of `RTM-CEB-ML.ini` file. As a result, when you run the `RTMModule.ini` file, you need to choose from the simulation configurations listed in the `RTM-CEB-ML.ini` file.        

Runtime Manager parameters
--------------------------
The simulation parameters of the [RTMModule.ini](examples/human/RTMModule.ini) file are depicted below. First, you need to enable the RTM by setting the `rmEnabled` parameter to `true`. If you wish not to use RTM during platoon cruising, rather prefer the control algorithms implemented in Plexe, e.g., PATH CACC, PLOEG CACC, etc., simply disable the RTMModule and define the desired controller using `*.node[*].scenario.controller = "CACC"` in the [RTM-CEB-ML.ini](examples/human/RTM-CEB-ML.ini) file.  

The parameters of Plexe simulator that specifies the PHY and MAC layer specifications, neighbouring traffic, beacon frequencies, controller parameters, and the simulated scenarios, are available in the `omnetpp.ini` file. Please refer to the Plexe documentation for the details of these parameters. The [`RMModule.ini`](/examples/human/RMModule.ini) file that specifies the Runtime Manager parameters inherits the parameters of `omnetpp.ini` file, Listing X.

https://github.com/shahriarHasan09/PlatoonSAFE/blob/91788a2d884f27088104feb1d094843b7fd0f70c/examples/human/RTMModule.ini#L1-L43

``` javascript {.line-numbers}
include omnetpp.ini
*.node[*].runtimeManager.rmEnabled = true
*.node[*].runtimeManager.rmMonitorInterval = 0.1 s
*.node[*].runtimeManager.expectedBeconInterval = .105 s
*.node[*].runtimeManager.readContractsFromInputFile = false
*.node[*].runtimeManager.contractInputFilename  =  
"/home/shahriar/plexe-veins-plexe-2.1/src/veins/modules/application/platooning/runtimeManager/input/contracts.txt"
*.node[*].runtimeManager.outputFilename = 
"/home/shahriar/plexe-veins-plexe-2.1/src/veins/modules/application/platooning/runtimeManager/output/output"
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

The parameters of the Listing X are explained below:

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
    PLATOON controllers[^1]. In the Plexe simulator, the
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

The code of the Runtime Manager can be found in the directory [`runtimeManager`](src/veins/modules/application/platooning/runtimeManager). The Runtime Manager is implemented as a separate module in the Plexe
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
the [`RuntimeManager.cc`](src/veins/modules/application/platooning/runtimeManager/RuntimeManager.cc) file. Moreover, in the *runtimeManager.\** class, vehicle colors are set to demonstrate the active controller
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
controllers. Code snippet in Listing X depicts the notable data members and member functions of `Assumption` class and the derived class `WIFIAssumption`. The `WIFIAssumption` class can be found in the directory [`assumptions`](src/veins/modules/application/platooning/runtimeManager/assumptions).

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
and they are derived from the same indirect base class [`StateParameter.h`](src/veins/modules/application/platooning/runtimeManager/StateParameter.h). This is done to facilitate a distinct extension of the C2F and C2L class types. For instance, if a user wants to add another data member to the C2F representing the distance to the front vehicle such that a controller switching will be triggered based on both these criteria, they can do so in the `C2F` class. The user do not need to make changes in the C2L class in this case. The communication quality of the C2L and C2F are categorized into OK, POOR, and CRITICAL which are measured based on the number of consecutive packet loss from the front and lead vehicles, Listing X.

https://github.com/shahriarHasan09/PlatoonSAFE/blob/796c4232fe1ff68e2db9313ae357869845d5b562/src/veins/modules/application/platooning/runtimeManager/StateParameter.h#L23-L27

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
PlatoonSAFE simulator can be found in [`Contracts.h`](src/veins/modules/application/platooning/runtimeManager/Contracts.h). Every `Contract` in the contract list in `Runtime Manager` is stored as `key-value` pair using `C++ Standard Template Library (STL)`. The STL provides the `map` container, where `Assumption` is used as `key` and `Guarantee` is the corresponding `value`. If the `Runtime Manager` detects any change in communication quality perceived by the ego vehicle, it looks for the corresponding `Guarantee` by iterating through the contracts list and performs the action specified by the `Guarantee` through a call of the `operator()` method. Current implementation of `Runtime Manager` deals with three different kind of `Guarantee`, e.g., `ChangeController`, `AdjustGap2Front`, and `ChangeControllerAndAdjustGap2Front`. The first two are derived from the `Guarantee` class, and `ChangeControllerAndAdjustGap2Front` has `ChangeController` and `AdjustGap2Front` as direct base classes. Listing X depicts the basic structure of the `Guarantee` and its derived classes. In case of `ChangeController`, the `Runtime Manager` changes the `ACTIVE_CONTROLLER` of the ego vehicle to the controller indicated by the data member `to`, and this is performed through the `setActiveController` method of the `TraCI` interface. `AdjustGap2Front` class is responsible for adjusting the time gap or constant distance gap with respect to the front vehicle. The `ChangeControllerAndAdjustGap2Front` designates the functions to be performed by its base classes `ChangeController` and `AdjustGap2Front`. The classes associated with the guarantees can be found in the directory [`guarantees`](src/veins/modules/application/platooning/runtimeManager/guarantees/).

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
contracts. The file is located in [`contracts.txt`](src/veins/modules/application/platooning/runtimeManager/input/contracts.txt). A parser is developed to parse the input to the OMNeT++ `contract` class type. The parser grammar can be found in the directory [`rmParser/parserGrammars`](src/veins/modules/application/platooning/runtimeManager/rmParser/parserGrammars/). If a user defines their own contracts they need to set the `readContractFromInputFile` parameter in the [`RMModule.ini`](/examples/human/RMModule.ini) file to `true`; in that case, the default contracts will be ignored.

https://github.com/shahriarHasan09/PlatoonSAFE/blob/796c4232fe1ff68e2db9313ae357869845d5b562/src/veins/modules/application/platooning/runtimeManager/input/contracts.txt#L13-L14

``` {#listing:contract_format numbers="none" caption="Input format for user defined {\\tt Contract}" label="listing:contract_format"}
::contract[Type : comma/space separated Assumption variables : 
comma/space separated Guarantee variables]
```

The input format of a user defined contract is depicted in Listing
X. The components of `Contract`, `Type`, `Assumption`, and `Guarantee` are defined as a collection of `key` and `value` pair. The keys and their corresponding values available in the current PlatoonSAFE implementation are depicted in Table X. For instance, the components of Assumption are as Listing X, and their corresponding values are to be chosen from the Table X. Similarly, the components of the Guarantee are depicted in Listing X, and the values should be chosen from the Guarantee row of Table X.

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


Based on the rules defined above, there are two possible formats that a user can use to define their contracts, and it is depicted in Listing X. The first format is to be used when one of the guarantees is provided, and the second one is to be used when both the guarantees are provided.

https://github.com/shahriarHasan09/PlatoonSAFE/blob/796c4232fe1ff68e2db9313ae357869845d5b562/src/veins/modules/application/platooning/runtimeManager/input/contracts.txt#L61-L62

``` {#listing:contract_option caption="Two possible combination of {\\tt Contract} input format" label="listing:contract_option"}
::contract[ctype=value : c2f=value ; c2l=value; mode=value : transition2mode=value]
::contract[ctype=value : c2f=value ; c2l=value; mode=value : transition2mode=value;
dist2pred=value]
```

Implementation of the Braking Strategies
========================================

The emergency braking strategies are implemented in [`BaseProtocol.cc`](src/veins/modules/application/platooning/protocols/BaseProtocol.cc) and [`BaseProtocol.h`](src/veins/modules/application/platooning/protocols/BaseProtocol.h) files of the Plexe simulator. The reason is that the developers of the Plexe simulator implement the creation of CAMs and the handling of the CAMs received from other vehicles in these files. In addition, we create two other message types called `BrakingPacket` and `ackPkt`. The `BrakingPacket` acts as the DENM, and the `ackPkt` acts as the ACKs required in the AEB and CEBP strategies. For the details of how to create messages in OMNeT++, please refer to the OMNeT++ user manual.

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
PlatoonSAFE includes several scripts to help the user collect and analize results of simulations. All these scripts are located in [`externalScripts`](src/examples/human/externalScripts/) and they can be used for the following purposes:

1. **Automatize simulations** to run different scenarios.

2. **Run Neural Network** simulations with PlatoonSAFE.

3. **Extract information** from `.vec` files.

4. **Evaluate ML algorithms** from simualtion reuslts.

[`runSimulations.py`](examples/human/externalScripts/runSimulations.py) is the main script that executes all the scenarios defined in [`simParam.py`](examples/human/externalScripts/simParam.py). It will run all the simulations, parse the .vec files into .csv files, and save all the results (collision, stop time and stop distance) into a unique file with. The user shall adapt the scenarios that wants to execute in [`simParam.py`](examples/human/externalScripts/simParam.py). We have defined some of the parameters that can be changed, but new ones can be added. For this, it is necessary to include how the parameter is defined in the `.ini` file ant in which line. [`simParamHelper.py`](examples/human/externalScripts/simParamHelper.py) can be used to check easier in which lines are located the parameters. This script should be run from its folder, it they are moved to somewhere else, all paths in the script shall be adapted.

```
cd src/examples/human/externalScripts/
python3 runSimulations.py
```

[`runSimNN.py`](examples/human/externalScripts/runSimNN.py) is the main script that runs the simulations using the NN defined in [`NNServerThread.py`](examples/human/externalScripts/NNServerThread.py). It connects to PlatoonSAFE simulator via UDP to exchange communication delays in the LeadVehicle of the platoon and predict thefuture ones. This script is only prepared to run one scenario, but with multiple seeds. The structure of the NN can be changed in [`NNServerThread.py`](examples/human/externalScripts/NNServerThread.py). UDP connection can also be changed (port, address). Remember to adapt boths sides, [`NNServerThread.py`](examples/human/externalScripts/NNServerThread.py) and [`AIAlgorithms.cc`](src/veins/modules/AI/AIAlgorithms.cc) (function *getNNServerAdd()*)This script should be run from its folder, it they are moved to somewhere else, all paths in the script shall be adapted.

```
cd src/examples/human/externalScripts/
python3 runSimNN.py
```

[`simUtils.py`](examples/human/externalScripts/simUtils.py) contains a function *createResultCSV* that converts resutls from PlatoonSAFE (`.vec`) into `.csv` files with the following structure: *ParameterName*, *VehicleID*, *SimulationTime*, *ParameterValue*. During this format conversion, the script also computes three important metrics required for analysing emergency braking strategies: inter-vehicle collisions, stopping distance of the LV, and the total time required for the whole platoon to transition to the fail-safe state from when braking started. [`resultsFromVec.py`](examples/human/externalScripts/resultsFromVec.py) contains an example of how this function can be used to extrac results from several vector files obtained after executing several simulations.

[`errorPlot.py`](examples/human/externalScripts/errorPlot.py) is a script that shows how the performance of the ML algorithms can be evaluated. in order to replicate this script, it is necessary to extract csv files of the predicted delay and the error of the algorithms from the vector files. To do that:
1. Open vector file in OMNet++
2. Select delay/error vector
3. Rigth click $\rightarrow$ Export Data $\rightarrow$ CSV for SpreadSeeths.
4. Save them with specific names and adapt the code below for that

Once the csv files are created, [`joinResultsCSV.py`](examples/human/externalScripts/joinResultsCSV.py) shall be used to combine delays and errors of all the algorithms into the same file, and with these files, use [`errorPlot.py`](examples/human/externalScripts/errorPlot.py) to calculate the Root Mean Squared Error of all algorithms and plot the accumulative sum of the errors.

All these scripts contain explanations by themself, but it is recommendable to check them, specially regarding PATHS used inside.



References
===========

[^1]: Steven E. Shladover, Christopher Nowakowski, Xiao-Yun Lu, and Robert Ferlis.Cooperative adaptive cruise control: Definitions and operating concepts. *Transportation Research Record*, 2489(1):145–152, 2015.
