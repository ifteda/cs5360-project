# Run.md

## Build & Run
```
python3 build.py number_of_frames frames_per_second
```
... or ...
```
py build.py number_of_frames frames_per_second
```

**Parameters:**

<table>
    <tbody>
        <tr>
            <th>Parameter</th>
            <th>Requirement</th>
            <th>Default</th>
            <th>Description</th>
        </tr>
        <tr>
            <td>number_of_frames</td>
            <td>optional</td>
            <td>2</td>
            <td>The number of frames to generate in the animation. Expects an integer >= 1. Value of 1 generates only the initial frame in the scene.</td>
        <tr>
        <tr>
            <td>frames_per_second</td>
            <td>optional</td>
            <td>2</td>
            <td>The number of frames to display per second of in the animation. Expects a value > 0.</td>
        <tr>
    </tbody>
</table>

### Examples:
```
py build.py 1
py build.py 8
py build.py 8 4
```
## Output
Generates GIF `output_animation.gif`.
Stores individual frames (`.ppm` files) in `/frames` subdirectory.

### Console Output
The console will display logs and metrics (per frame) throughout the execution of the program. For example:
```
Loading file: ../../common/objects/cube.obj
Successfully parsed .obj file
Generating triangles...
Successfully loaded ../../common/objects/cube.obj!
Rendering images...

Preparing frame 0...
Completed frame 0!
Metrics
-------
Render Time                     : 304.73 (sec)
Rays Cast                       : 30720000
Bounding Volume Intersections   : 119770667
Successful Object Intersections : 70485540
----------------------------------------------
```