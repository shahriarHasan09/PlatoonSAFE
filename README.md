# PlatoonSAFE User Manual (Version 0.1)

S. Hasan, J. Gorospe, S. Girs, A. A. Gomez, and E. Uhlemann, “PlatoonSAFE: An Integrated Simulation Tool for Evaluating
Platoon Safety with Fault Tolerance Mechanisms and ML Algorithms” Jounral ABC. X, Y, Z, 2022.

<table class="tg">
<thead>
  <tr>
    <th class="tg-baqh">Version no.</th>
    <th class="tg-baqh">Date</th>
    <th class="tg-baqh">Changes</th>
  </tr>
</thead>
<tbody>
  <tr>
    <td class="tg-baqh">PlatoonSAFE 0.1</td>
    <td class="tg-baqh">2022/11/01</td>
    <td class="tg-baqh">First version</td>
  </tr>
  <tr>
    <td class="tg-baqh"></td>
    <td class="tg-baqh"></td>
    <td class="tg-baqh"></td>
  </tr>
</tbody>
</table>


https://user-images.githubusercontent.com/93597343/198429943-087037cc-2b73-4f97-baf8-20c7ffbd8311.mp4

<!---
https://user-images.githubusercontent.com/93597343/198430751-30dfad55-9436-4ccf-ac8d-5e89f22d1f28.mp4

https://user-images.githubusercontent.com/93597343/198430822-ecf1a8f0-abbb-4fc7-a4dd-f368e95f0c56.mp4

https://user-images.githubusercontent.com/93597343/198430891-4e61367e-58cf-44b8-9088-1cd774082dfe.mp4
-->


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
* In order to run the ML algorithms, some libraries are required to be installed:
````
sudo apt-get install python3-pip
pip3 install numpy
pip3 install tensorflow
pip3 intall keras
````
* Finally, some external script to post process simulation results will also need the following libraries
````
pip3 install pandas
pip3 install matplotlib
````
_[requirements.txt](examples/human/externalScripts/requirements.txt) contains the versions of all the python libraries that are used for ML and post processing_
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

## Runtime Manager parameters ##

The simulation parameters of the [RTMModule.ini](examples/human/RTMModule.ini) file are depicted below. First, you need to enable the RTM by setting the `rmEnabled` parameter to `true`. If you wish not to use RTM during platoon cruising, rather prefer the control algorithms implemented in Plexe, e.g., PATH CACC, PLOEG CACC, etc., simply disable the RTMModule and define the desired controller using `*.node[*].scenario.controller = "CACC"` in the [RTM-CEB-ML.ini](examples/human/RTM-CEB-ML.ini) file. It can be useful should you want to experiment with the braking strategies and ML algorithms with a specific controller employed during cruising. Although the simulation parameters are well commented, we explain some of them for further clarification.            

https://github.com/shahriarHasan09/PlatoonSAFE/blob/91788a2d884f27088104feb1d094843b7fd0f70c/examples/human/RTMModule.ini#L1-L43

-   Line 12: The interval at which a beacon is expected from the lead or
    front vehicle, defined by `beaconInterval` in the [RTM-CEB-ML.ini](examples/human/RTM-CEB-ML.ini) file. The duration of communication outage can also be expressed in terms of no. of packet losses. For instance, if `beaconInterval` is `0.1 s (100 ms)`, a communication outage for `200 ms` can be interpreted as `two` packet losses.

-   Lines 14-17: These parameters are used for specifying the user defined contracts. If `readContractsFromInputFile` is set to `true` then the default contracts are ignored.

-   Lines 18: Any time during platooning, if safety is violated, e.g., the inter-vehicle gaps are less than a predefined safety distance (specified by `minSafetyDistance` in `line 29`), it is recorded in an output file that can be used for results analysis. 

-   Lines 14-15: The communication qualities *good*, *fair* and *poor* in the paper correspond to *OK*, *POOR*, and *CRITICAL*, respectively, in the simulator. These thresholds are represented by the number of packet losses. 
-   Lines 24-26: For instance, if you want the platoon to continue cruising in the *degraded* modes which are adopted due to transient communication errors, set `upgradationEnabled` to `false` (not recommended). The `gapControlEnabled` parameter enables gap adjustments due to *fair* communication quality. 
-   Lines 33, 35 and 40: These are the inter-vehicle gaps maintained in the ACC, PLOEG and CACC controllers, which are inherited from the [RTM-CEB-ML.ini](examples/human/RTM-CEB-ML.ini) file. You may choose to change them. 
-   Lines 37, 38, 42, 43: The factor by which the inter-vehicle gaps are increased due to *fair* communication quality. In this example, the first five vehicles in the platoon increase the gap by 10%, and the rest increase their gaps by 25%. The rationale is that the rear vehicles experience more packet losses than the front ones due to path loss and fading effects. 

## Emergency braking and ML parameters ##
The [RTM-CEB-ML.ini](examples/human/RTM-CEB-ML.ini) file contains several simulation configurations, one of which can be chosen, e.g., by openning the `run configurations` window (see the figure below).

<img width="495" alt="run_configs" src="https://user-images.githubusercontent.com/93597343/192091080-b2c22058-fd44-48a1-849c-f47fecd918d1.png">

Different `configs` specified in the [RTM-CEB-ML.ini](examples/human/RTM-CEB-ML.ini) represent different simulation scenarios. A short description of different `configs` is given below:

https://github.com/shahriarHasan09/PlatoonSAFE/blob/91788a2d884f27088104feb1d094843b7fd0f70c/examples/human/RTM-CEB-ML.ini#L1-L416

- `Config sinusoidal-RTM` The platoon cruises in a sinusoidal fashion until the `sim-time-limit` is reached, and the platooning vehicles switch between the controllers and/or adjusts gap following the contracts in RTM module.
- `Config RTM-CEBP` The platoon cruises in a sinusoidal fashion using the RTM and performs emergency braking at `40 s` into the simulation time using the CEBP braking strategy [(Bergenhem et al., 2018)](https://link.springer.com/chapter/10.1007/978-3-030-03424-5_26). You can specify the `DENMInterval` and whether the DENMs should be relayed by the mid vehicle in the platoon by defining the `isRelayEnabled` parameter.   
- `Config RTM-AEB` The RTM is enabled during platoon cruising in a sinusoidal fashion, and the platoon performs emrgency braking using the AEB strategy. The vehicles perform soft deceleration at a rate specified in the `softDecelRate` parameter.  
- `Config RTM-GD` The platoon cruises using the RTM in a sinusoidal fashion and emrgency braking is performed using the Gradual Deceleration strategy [(Zheng et al., 2014)](https://ieeexplore.ieee.org/document/6766244). 
- `Config RTM-SB` RTM is used during platoon cruising and Synchronized Braking [(Hasan et al., 2019)](https://ieeexplore.ieee.org/document/8891254) is used during emergency braking. A fixed $\tau_{wait}$ is specified by the user using `sbWaitTime` parameter. 
- `Config RTM-SB-AvgDelay` RTM is used during cruising, Synchronized Braking is used during braking, and $\tau_{wait}$ is determined by taking the average of CAM delays during cruising. 
- `Config RTM-SB-SVR` RTM and Synchronized Braking are used during cruising and braking, respectively. In this case, `Online SVR` is used for predicting the $\tau_{wait}$. The parameters specific to `Online SVR` are listed below:
````
*.node[*].prot.runSVR = true 
*.node[*].prot.SVRSizeLimit = 5
*.node[*].prot.SVRC = 0.03
*.node[*].prot.SVREpsilon = 0.0001
````
- `Config RTM-SB-NN` RTM and synchronized braking are used during cruising and braking, respectively. `Neural Network` is used for predicting $\tau_{wait}$. In order to use `NN`, just set `*.node[*].prot.runNN` to `true`. Note that a pthon script is required to be executed in order to run NN (more details are provided below).

## Running the NN Algorithm ##
In PlatoonSAFE, a UDP connection is used to perform predictions using the NN algorithm implemented in an external library. Therefore, in order to run `Config RTM-SB-NN`, it is necessary to run the NN externaly first. 

[`runSimNN.py`](examples/human/externalScripts/runSimNN.py) is the main script that runs the simulations using the NN defined in [`NNServerThread.py`](examples/human/externalScripts/NNServerThread.py). It connects to PlatoonSAFE simulator via UDP to exchange communication delays in the LV of the platoon and predict the future ones. The following script is only prepared to run one scenario, but with multiple seeds. This script runs the simulations from the command line. If the user wants to execute only the NN and start the simulator from OMNet++, `runSimNN.py` must be changed to something similar to this:

```` python
import NNServerThread
import time

def main():
    # Run NN thread
    t1 = NNServerThread.thread_with_trace(target = NNServerThread.runNN)
    t1.start()
    # Wait 5 seconds to ensure NN Thread is on
    time.sleep(5)
    # Start simulation in OMNet++
    print('Start simulation in OMNet++')
    t1.join()

if __name__ == "__main__":
    main()

````

This script should be run from its folder, it they are moved to somewhere else, all paths in the script shall be adapted.

```
cd src/examples/human/externalScripts/
python3 runSimNN.py
```

Results collection and Analysis
===============================
PlatoonSAFE includes several scripts to help the user collect and analize results of simulations. All these scripts are located in [`externalScripts`](src/examples/human/externalScripts/) and they can be used for the following purposes:

1. **Automatize simulations** to run different scenarios.

3. **Extract information** from `.vec` files.

4. **Evaluate ML algorithms** from simualtion reuslts.

[`runSimulations.py`](examples/human/externalScripts/runSimulations.py) is the main script that executes all the scenarios defined in [`simParam.py`](examples/human/externalScripts/simParam.py). It will run all the simulations, parse the .vec files into .csv files, and save all the results (collision, stop time and stop distance) into a unique file with. The user shall adapt the scenarios that wants to execute in [`simParam.py`](examples/human/externalScripts/simParam.py). We have defined some of the parameters that can be changed, but new ones can be added. For this, it is necessary to include how the parameter is defined in the `.ini` file ant in which line. [`simParamHelper.py`](examples/human/externalScripts/simParamHelper.py) can be used to check easier in which lines are located the parameters. This script should be run from its folder, it they are moved to somewhere else, all paths in the script shall be adapted.

```
cd src/examples/human/externalScripts/
python3 runSimulations.py
```

[`simUtils.py`](examples/human/externalScripts/simUtils.py) contains a function *createResultCSV* that converts resutls from PlatoonSAFE (`.vec`) into `.csv` files with the following structure: *ParameterName*, *VehicleID*, *SimulationTime*, *ParameterValue*. During this format conversion, the script also computes three important metrics required for analysing emergency braking strategies: inter-vehicle collisions, stopping distance of the LV, and the total time required for the whole platoon to transition to the fail-safe state from when braking started. [`resultsFromVec.py`](examples/human/externalScripts/resultsFromVec.py) contains an example of how this function can be used to extrac results from several vector files obtained after executing several simulations.

[`errorPlot.py`](examples/human/externalScripts/errorPlot.py) is a script that shows how the performance of the ML algorithms can be evaluated. in order to replicate this script, it is necessary to extract csv files of the predicted delay and the error of the algorithms from the vector files. To do that:
1. Open vector file in OMNet++
2. Select delay/error vector
3. Rigth click $\rightarrow$ Export Data $\rightarrow$ CSV for SpreadSeeths.
4. Save them with specific names and adapt the code below for that

Once the csv files are created, [`joinResultsCSV.py`](examples/human/externalScripts/joinResultsCSV.py) shall be used to combine delays and errors of all the algorithms into the same file, and with these files, use [`errorPlot.py`](examples/human/externalScripts/errorPlot.py) to calculate the Root Mean Squared Error of all algorithms and plot the accumulative sum of the errors.

All these scripts contain explanations by themself, but it is recommendable to check them, specially regarding PATHS used inside.



Implementation of Runtime Manager
=================================
Runtime Manager (RTM) is implemented as a separate [module](src/veins/modules/application/platooning/runtimeManager) in PlatoonSAFE. Every time a vehicle receives a platooning beacon, i.e., CAM, it is handled by the `onPlatoonBeacon` method in [BaseApp.cc](src/veins/modules/application/platooning/apps/BaseApp.cc) of Plexe. From here, the overridden `onPlatoonBeacon` method in [RuntimeManager.cc](src/veins/modules/application/platooning/runtimeManager/RuntimeManager.cc) is called. The `onPlatoonBeacon` method then logs the front and lead vehicles' data. The RTM mainly functions by monitoring the communication quality with the front and lead vehicles periodically. To this end, a `monitoringMsg` is initialized in  [RuntimeManager.cc](src/veins/modules/application/platooning/runtimeManager/RuntimeManager.cc) and every `rmMonitorInterval` the `handleSelfMsg` method in [RuntimeManager.cc](src/veins/modules/application/platooning/runtimeManager/RuntimeManager.cc) is invoked; please see the truncated version of `handleSelfMsg` method below.    

```` cpp
void RuntimeManager::handleSelfMsg(cMessage* msg) {
    if(msg == monitoringMsg) {
        SimTime callBackTime = simTime() + rmParam.rmMonitorInterval;
        egoLog();
        safetyViolationCheck();
        evaluate();
        scheduleAt(callBackTime, monitoringMsg);
    }
}
````
The `egoLog` method logs the ego vehicle data, e.g., active controller, simulation time, acceleration, etc., and the `safetyViolationCheck` method checks if the inter-vehicle gaps are smaller than the `minSafetyDistance` specified in [RTMModule.ini](examples/human/RTMModule.ini). The `evaluate` method is probably the most important one. Every `rmMonitorInterval` the `evaluate` method in [RuntimeManager.cc](src/veins/modules/application/platooning/runtimeManager/RuntimeManager.cc) invokes the `evaluate` method in [Contracts.cc](src/veins/modules/application/platooning/runtimeManager/Contracts.cc), where RTM searches for a contract from the `rmContractsList` for the `Assumptions`, i.e., communication quality, currently active controller, and the corresponding `Guarantee` is provided. Please follow the flow of codes from the `evaluate` method in [Contracts.cc](src/veins/modules/application/platooning/runtimeManager/Contracts.cc).   


Assumption/Guarantee
--------------------

`Assumption` represents the current state of the ego vehicle, e.g.,
currently active controller, Connection to Front (C2F) vehicle,
Connection to Leader (C2L) vehicle, etc. For a set of assumptions, the
RM performs the task defined by the `Guarantee`. An assumption/Guarantee
pair results in a new data type named a `Contract`. Thus, a contract
represents a guaranteed performance ensured by the RM for the current
state of the ego vehicle.

RTM defines the C2F and C2L using the [WIFIAssumption](src/veins/modules/application/platooning/runtimeManager/assumptions/WIFIAssumption.h)
class that is derived from the [Assumption](src/veins/modules/application/platooning/runtimeManager/Assumption.h) base class. The base class
contains the data member `ACTIVE_CONTROLLER` from which the active controller in ego vehicle can be obtained . The following truncated code snippet depicts the notable data members and member functions of the `Assumption` class and the derived class `WIFIAssumption`.

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

In the current implementation, [C2F](src/veins/modules/application/platooning/runtimeManager/StateParameter.h) and [C2L](src/veins/modules/application/platooning/runtimeManager/StateParameter.h) classes contain only one data member representing the communication qualities, and they both are derived from the same indirect base class [StateParameter](src/veins/modules/application/platooning/runtimeManager/StateParameter.h). This is to facilitate distinct extensions of the `C2F` and `C2L` class types. For instance, if a user wants to add another data member to the `C2F` representing the distance to the front vehicle such that a controller switching will be triggered based on both these criteria, they can do so in the `C2F` class. In this case, the user would not need to make changes in the `C2L` class. The `C2F` and `C2L` are further categorized into `OK`, `POOR`, and `CRITICAL`, which are measured based on the number of consecutive packet losses from the front and lead vehicles, see the following code snippet. Recall that the terms *good*, *fair* and *poor* in the paper are represented by *OK*, *POOR* and *CRITICAL* in the RTM module. 

``` cpp
enum WIFI_QUALITY {
    CRITICAL,
    POOR,
    OK,
};
```

The default assumption/guarantee `contracts` of RTM can be found in [Contracts.cc](src/veins/modules/application/platooning/runtimeManager/Contracts.cc). The contracts in the [rmContractList](src/veins/modules/application/platooning/runtimeManager/Contracts.h) are stored as `key-value` pairs using the C++ Standard Template Library (STL). The STL provides the `map` container, where `Assumption` and `Guarantee` are represented by `key` and `value`, respectively. If RTM detects a change in communication quality of the ego vehicle, it looks for the `Guarantee` on the current `Assumption` by iterating through the `rmContractList` and performs the guarantee specific actions through the [operator](src/veins/modules/application/platooning/runtimeManager/Guarantee.cc) method. RTM deals with three kinds of Guarantees, e.g., `ChangeController`, `AdjustGap2Front`, and `ChangeControllerAndAdjustGap2Front`. The first two classes are derived from the [Guarantee](src/veins/modules/application/platooning/runtimeManager/Guarantee.h) class, and the [ChangeControllerAndAdjustGap2Front](src/veins/modules/application/platooning/runtimeManager/guarantees/ChangeControllerAndAdjustGap2Front.h) class has [ChangeController](src/veins/modules/application/platooning/runtimeManager/guarantees/ChangeController.h) and [AdjustGap2Front](src/veins/modules/application/platooning/runtimeManager/guarantees/AdjustGap2Front.h) as the direct base classes. The following code snippet depicts the basic structure of the `Guarantee` class and its derived classes. In the `ChangeController` class, RTM changes the `ACTIVE_CONTROLLER` of the ego vehicle to the controller indicated by the data member `to`, and this is performed through the `setActiveController` method of `TraCI` interface (see the `operator` method in [ChangeController.cc](src/veins/modules/application/platooning/runtimeManager/guarantees/ChangeController.cc)). The `AdjustGap2Front` class is responsible for adjusting the time gap or constant distance gap with respect to the front vehicle. And the `ChangeControllerAndAdjustGap2Front` designates the functions to be performed by its base classes `ChangeController` and `AdjustGap2Front`.

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

If a user wants to implement a new `Assumption`, they only require to derive a new class from the base class and override the `operator()` method.

User Defined Contracts
----------------------
The PlatoonSAFE simulator facilitates user defined contracts so that a user can define their own contracts without
having deep knowledge on the OMNeT++ simulation framework. It is possible to modify the [`contracts.txt`](src/veins/modules/application/platooning/runtimeManager/input/contracts.txt) file to accommodate such user defined
contracts. A parser is developed to parse the input to the OMNeT++ `contract` class type. The parser grammar can be found in the directory [`rmParser/parserGrammars`](src/veins/modules/application/platooning/runtimeManager/rmParser/parserGrammars/). If a user defines their own contracts they need to set the `readContractFromInputFile` parameter in the [`RMModule.ini`](/examples/human/RMModule.ini) file to `true` and the the path in`contractInputFilename` should be changed. In that case, RTM default contracts will be ignored. The input format of a user defined contract is depicted below.  

https://github.com/shahriarHasan09/PlatoonSAFE/blob/796c4232fe1ff68e2db9313ae357869845d5b562/src/veins/modules/application/platooning/runtimeManager/input/contracts.txt#L13-L14

The components of `Contract`, `Type`, `Assumption`, and `Guarantee` are defined as a collection of `key` and `value` pair. The keys and their corresponding values available in PlatoonSAFE are depicted in the Table below. 


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


For instance, the components of Assumption and Guarantee are as depicted in the following code snippet, and their corresponding values are to be chosen from the Table above. 

``` {#listing:contract_assumption_order numbers="none" caption="Order of pairs of {\\tt Assumption} component" label="listing:contract_assumption_order"}
c2f=value ; c2l=value ; mode=value
```

``` {#listing:contract_guarantee_order numbers="none" caption="Order of pairs of {\\tt Guarantee} component" label="listing:contract_guarantee_order"}
transition2mode=value ; dist2pred=value
```

Based on the rules defined above, there are two possible formats that a user can use to define their contracts as shown in the code snippet below. The first format is to be used when one of the guarantees, e.g., `ChangeController`, is provided, and the second one is to be used when both the guarantees are provided, i.e., `ChangeControllerAndAdjustGap2Front`.

https://github.com/shahriarHasan09/PlatoonSAFE/blob/796c4232fe1ff68e2db9313ae357869845d5b562/src/veins/modules/application/platooning/runtimeManager/input/contracts.txt#L61-L63

Please refer to the [`contracts.txt`](src/veins/modules/application/platooning/runtimeManager/input/contracts.txt) file for further details and examples.

Implementation of ML Algorithms
===============================

Two ML algorithms have been integrated into PlatoonSAFE, NN and Online SVR. NN is implemented in python ([NNServerThread.py](examples/human/externalScripts/NNServerThread.py)) and connected to PlatoonSAFE via UDP socket. Online SVR is a C++ module ([OnlineSVR](src/veins/modules/AI/OnlineSVR)). In addition, all the functions have been collected into [AIAlgorithms.cc](src/veins/modules/AI/AIAlgorithms.cc) to facilitate the use of these two algorithms.

Regarding Online SVR, the first step is to initialize an [OnlineSVR](src/veins/modules/AI/OnlineSVR/OnlineSVR.cc) instance and define the parameters of the algorithm.

```cpp
SVR = new OnlineSVR();
SVR->SetEpsilon(SVREpsilon);
SVR->SetC(SVRC);
SVR->SetKernelType(OnlineSVR::KERNEL_RBF);
SVR->SetAutoErrorTollerance(false);
SVR->SetVerbosity(OnlineSVR::VERBOSITY_NORMAL);
```
After initializing the algorithm, it is trained using the `Train` function, and then, it is prepared to predict values with `Predict` function. The user can retrain it whenever it is necessary and, in addition, the less relevant samples can also be removed from the training process with the `Forget` function. All of them are implemented in the OnlineSVR module, but we have implemented an example in [AIAlgorithms.cc](src/veins/modules/AI/AIAlgorithms.cc), where `setupSVR` initialises a new OnlineSVR instance and `predictWithSVRAndTrain` predicts a new value, retrains the algorithm, and if it has more samples than the limit stablished by the user, it forgets them.

The NN is defined in [`NNServerThread.py`](examples/human/externalScripts/NNServerThread.py) and the users can change the structure (Lines 58-63), defining a different _tf.keras.Sequential_ network. In this script, the number of past samples used to predict (WD_SAMPLES Line 54), the optimizer (Line 64) or the learning rate (Line 57) can also be changed, but the code is only prepared to use a single prediction, therefore, the NN must give a single output (WD_PRED Line 55). `NNServerThread.py` is a thread that shall be called from outside (in this case, [`runSimNN.py`](examples/human/externalScripts/runSimNN.py))

```python
t1 = NNServerThread.thread_with_trace(target = NNServerThread.runNN)
t1.start()
```
Once the thread is running, it opens a UDP socket port, it defines the NN that will be used and it waits until the PlatoonSAFE simulator sneds the input. After receiving the input, it will make a prediction (`nn.predict_on_batch(input)`), send it back to the PlatoonSAFE via socket and retrain the network (`model.train_on_batch`). This thread will continue doing this until the PlatoonSAFE sends something that does not fit as an input (in our case, a string "end"). In the simulator side, it is also necessary to connect to the NN through UDP. [AIAlgorithms.cc](src/veins/modules/AI/AIAlgorithms.cc) has two functions to help with this: `setupNNConnection` initialize the UDP connection to the NN script and `getNNServerAdd` returns the information of the NN UDP connection to be able to send information to it.

In this case, we have used these ML algorithms to enhance an emergency braking strategy in a platooning scenario. The aim is to predict the optimal $\tau_{wait}$ for Synchronized Braking. For these, the last vehicle of the platoon will send `DelayMessage` whenever it receives a CAM message from the LV in the `handleUnicastMsg` method. The LV will receive the information of the delay that a message suffers to communicate from the first to the last vehicle and it will use it to predict the future delay using NN (`predictWithNN`), Online SVR (`predictWithSVRAndTrain`) or the average of the previous received delays (`calculateSBAvgWaitTime`). This prediction will be set as $\tau_{wait}$ in the `sendBrakingMessage` method (`pkt->setPredictedDelayOfLastVeh()`) and it will be transmited to all the vehicles in the platoon. Once they receive the `BrakingMessage`, they will schedule the braking so that all vehicles are synchronized (`synchronizedBraking`).

The user can also activate the option to add relay messages for `DelayMessage`. If so, the mid vehicle of the platoon will relay these type of messages. The LV will be waiting for `DelayMessage` from the last vehicle in the `handleUnicastMsg` method, but in case that they are lost, it will also check for the `RelayMessage`. This option increases the probability to receave the delay information in the LV but it makes the communication channel more busy.


Implementation of the Braking Strategies
========================================

The emergency braking strategies are implemented in [`BaseProtocol.cc`](src/veins/modules/application/platooning/protocols/BaseProtocol.cc) and [`BaseProtocol.h`](src/veins/modules/application/platooning/protocols/BaseProtocol.h) files. First, we need to schedule a DENM at the time the imaginary road hazard takes place. Then the DENM is required to be repeated every `DENMInterval`. To this end, we initialize the braking message at the braking time as follows:
````
brakingMessage = new cMessage("brakingMessage");
if (simTime() > brakeAtTime) {
    brakeAtTime = simTime();
    scheduleAt(simTime(), brakingMessage);
} else {
    scheduleAt(brakeAtTime, brakingMessage);
}
````
Now, remember that a message created using the `scheduleAt` function goes to the `handleSelfMsg` method at the time the message is scheduled. Therefore, from the `handleSelfMsg` method, the `brakingMessage` are repeated as follows:
````
void BaseProtocol::handleSelfMsg(cMessage *msg) {
sendBrakingMessage(-1);
scheduleAt(simTime() + DENMInterval, brakingMessage);
}
````
The DENMs are broadcasted using the `sendBrakingMessage`, then they are repeated every `DENMInterval`. Inside the `sendBrakingMessage` method, one can include the information required for the following vehicles to perform braking. To that end, we need to create a packet with OMNeT++ `.msg` files for every type of messages that we want to broadcast. See [BrakingPacket.msg](src/veins/modules/application/platooning/messages/BrakingPacket.msg) as an example. This is the basic mechanism of creating event driven or periodic messages. How a following vehicle in a platoon should act upon receiving a DENM depends on the braking strategy itselft. 

The reason is that the developers of the Plexe simulator implement the creation of CAMs and the handling of the CAMs received from other vehicles in these files. In addition, we create two other message types called `BrakingPacket` and `ackPkt`. The `BrakingPacket` acts as the DENM, and the `ackPkt` acts as the ACKs required in the AEB and CEBP strategies. For the details of how to create messages in OMNeT++, please refer to the OMNeT++ user manual.

When an FV in a platoon receives receives a packet, e.g., `BrakingPacket`, the packet is handled in the `handleUnicastMsg` method. Upon receiving a `BrakingPacket`, a user can choose to relay it using the mid vehicle in the platoon. In addition, we check if a DENM has already been received by the ego vehicle, in which case the DENM is discarded. Then the vehicles perform emergency braking following a braking strategy. The braking strategies are implemented using the `synchronizedBraking`, `isAEBEnabled`, `isCEBPEnabled`, `isGDEnabled` methods. Note that the `setFixedAcceleration` method is used to instruct a vehicle to accelerate or decelerate at a particular rate.  

There are several other types of messages that are used by the platooning vehicles for different purposes. For instance, 
- `DelayMessage` is used to reach the delays experienced by the last vehicle to the LV.
- `RelayMessage` is used to relay the `DelayMessage` to the LV.
- `RelayDENMs` is used to relay the DENMs from the LV to the FVs
- `ackPkt` is used by the platooning vehicles for AEB and CEBP strategies


References
===========

[^1]: Steven E. Shladover, Christopher Nowakowski, Xiao-Yun Lu, and Robert Ferlis.Cooperative adaptive cruise control: Definitions and operating concepts. *Transportation Research Record*, 2489(1):145–152, 2015.
