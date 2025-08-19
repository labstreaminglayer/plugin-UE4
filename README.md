# lsl-ue4

LabStreamingLayer [plugin for Unreal Engine](https://docs.unrealengine.com/en-US/ProductionPipelines/Plugins/index.html)

* LSL 1.14.0
* UE 4.26 to 5.1

# Usage Instructions

## Install

This plugin is also available on the [Marketplace](https://www.unrealengine.com/marketplace/en-US/product/labstreaminglayer-plugin).

To add it to your project, copy this entire folder into your `/[Project Root]/Plugins/` folder or you can even git clone it in there: `[Project Root]\Plugins> git clone https://github.com/labstreaminglayer/plugin-UE4.git LSL` or `git submodule add https://github.com/labstreaminglayer/plugin-UE4.git LSL` if you are already working in a git repo. If you want to use this plugin across multiple projects, it might be better to place it in your `/[UE4 Root]/Engine/Plugins/` folder. The UE4 Editor must be restarted after moving the plugin. You may be asked to rebuild the module when the editor relaunches; please choose "Yes" to rebuild.

To verify the plugin is installed, open the Plugins menu (Edit > Plugins) and the plugin should be listed under the "Input Devices" category, possibly under the Project section at the bottom.

## Using LSL in a UE4 project

### Receiving external data with an LSL Inlet

Inlets are available as 'scene components'. Typically that means attaching such a component to an object that needs access to a LSL data stream. The inlet component must be configured so that its [`continuous_resolver`](https://github.com/sccn/liblsl/blob/master/include/lsl_cpp.h#L1754-L1762) member can search for the desired stream. On every tick, the resolver checks to see if that stream is found and, if so, it creates its [`stream_inlet`](https://github.com/sccn/liblsl/blob/master/include/lsl_cpp.h#L1004-L1011) member and the resolver is deleted. On subsequent ticks the data will be pulled from the inlet then broadcast to `OnStreamUpdatedFloat`.

Multiple different objects can each have a unique inlet to the same stream. This isn't very efficient because it means extra data buffers and tcp/ip communication for each object accessing that stream. If you find that you have multiple objects that need to access the same stream, it is instead better to group those objects under a parent object with a single LSLInlet component, then the child objects can bind to the OnStreamUpdatedFloat event.

Here are some simple step-by-step instructions to a minimal LSL Inlet integration in a UE4 project.

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

If the stream that you are connected to dies then one of two things will happen. If the stream has a UUID then the inlet will persist and simply continue to attempt fetching until the stream reappears. If the stream does not have a UUID then it cannot be reconnected; instead the inlet is destroyed and the resolver is recreated to search for another stream with matching predicate. The latter event will also trigger an error broadcast to the `OnStreamCaughtException` event.

### Sending event markers out with an LSL Outlet

We are going to use overlap events in a sphere object to trigger sending markers over LSL. We need a way to have transient overlap events, both for onset and offset. I am going to use the cube from the inlet example above that will move (via LSL Inlet data) to overlap the target sphere, but you could use anything you want to create the overlap events.

1. Place a Sphere actor into the scene at (250, -150, 100).
2. Create and edit its Blueprint.
3. Add a LSLOutlet component.
4. Edit the LSLOutlet details. Stream Name: "UE4 Sphere"; Stream Type: "Markers"; Sampling Rate: 0.0; Channel Format: `string`. Add a Channel.
5. Drag and drop the LSLOutlet component onto the event graph.
6. From its instance node, drag off and create a "Push Sample String" node.
7. Wire the "Event ActorBeginOverlap" node to the Push Sample String node.
8. Drag off from the Push Sample String Node's "Data" port, and add a "Make Array" node.
9. Edit the Make Array first element to whatever string you want to indicate begin overlap. Try "Ouch!". (You could also wire in public variables so you could set these strings from the Editor.)
10. Repeat steps 5-9 except for "ActorEndOverlap".
11. Make sure the actor and the object that will overlap with it both have their collision setup properly. [See here for more info.](https://docs.unrealengine.com/en-US/InteractiveExperiences/Physics/Collision/Overview/index.html) Briefly, they both need "Generate Overlap Events" checked and the collision settings need Overlap checked for "World Dynamic".
12. Run the game and view the strings in another process with LSL. If you have Python and pylsl installed, this is as simple as `python -m pylsl.examples.ReceiveStringMarkers`

![LSL Outlet](https://github.com/labstreaminglayer/plugin-UE4/raw/master/Resources/LSLOutput.PNG "LSL Outlet in UE4")

# Notes for Linux
The library for Linux [liblsl.so](./Source/ThirdParty/liblsl/bin/liblsl.so), was compiled usig Unreal Engine 5.5.4 Linux toolchain. It has been tested with Unreal Engine 5.4.27 and 5.6.1. There is a chance that the library crashes the editor (e.g. Segmentation fault), if this is the case we suggest to re-compile liblsl from source. To do that:

1. Clone the [liblsl](https://github.com/sccn/liblsl) locally
2. Copy the script [build_liblsl_for_ue.sh](./build_liblsl_for_ue.sh) in the top folder of liblsl.
3. Run the script. The script will locate your Unreal Engine installations and ask which of the Engine's version to use for the compilation. Select the desired engine and the desired toolchain.
4. Upon successful compilation, the script will ask where it should move the compiled library, enter the path to the [bin](./Source/ThirdParty/liblsl/bin/) folder.

# Known Issues

* The LSLInletComponent only supports float, double, and string data types. And the double data will be cast to float before being made available in blueprints.
