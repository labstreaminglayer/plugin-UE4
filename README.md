# lsl-ue4

LabStreamingLayer [plugin for Unreal Engine 4](https://docs.unrealengine.com/en-US/ProductionPipelines/Plugins/index.html)

* LSL 1.14.0
* UE4.25

# Usage Instructions

## Install

This plugin isn't (yet?) on the marketplace. To add it to your project, copy this entire folder into your `/[Project Root]/Plugins/` folder. If you want to use this plugin across multiple projects, it might be better to place it in your `/[UE4 Root]/Engine/Plugins/` folder. The UE4 Editor must be restarted after moving the plugin. You may be asked to rebuild the module when the editor relaunches; please choose "Yes" to rebuild.

To verify the plugin is installed, open the Plugins menu (Edit > Plugins) and the plugin should be listed under the "Input Devices" category, possibly under the Project section at the bottom.

## Using LSL in a UE4 project

Here are some simple step-by-step instructions to a minimal LSL integration in a UE4 project.

0. Run a LSL stream on your network. It must have a `float` data type (and for this example, at least 2 channels).
1. Place an Actor (e.g. Cube) into the scene at Location (250, 0, 100).
2. In its transform property, make sure to set it to Movable.
3. Select the Actor, then Add Component, and under Custom, choose LSLInlet.
4. Provide the Stream Name and/or Stream Type to your float stream from step 0.
5. Convert the object to a Blueprint
6. Edit the blueprint.
7. Add a Vector variable called InitialLocation.
8. Add a float variable called ScaleFactor, click on the Eye to make it public, and set its default value to something reasonable (e.g. 50).
9. Select the Event Graph tab.
10. Wire GetActorLocation to SET Initial Location on Event BeginPlay (see screenshot).
11. Right-click on the LSLInlet component, choose events, and "On Stream Updated Float".
12. Connect the Float Array of the above component to 2 different Get (reference) nodes, indexing 0 and 1.
13. Connect those two floats to a Make Vector. Then scale the resulting vector by Scale Factor and add it to the initial location.
14. Connect the scaled and shifted vector to a Set Actor Location node that is updated by On Stream Updated Float.
15. Run the game and the cube should move according to the input.

![LSL Inlet](https://github.com/labstreaminglayer/plugin-UE4/raw/master/Resources/LSLInput.PNG "LSL Inlet in UE4")

# Known Issues

* The LSLInletComponent only supports float data types. Any advice on how to do type templating in UE4 would be appreciated.
