* Scene
  The =scene.hpp= and =scene.cpp= files define classes for storing the information about the scene when it is loaded from the JSON file. These classes are responsible for constructing themselves from the parsed JSON representing them, as well as finding intersections and normals to the spheres and planes. Utility functions for initial parsing of a JSON document into a Scene are also provided

The classes defined here are:
  - SceneObject :: Abstract parent class to the objects that can appear in the scene
  - Sphere :: A sphere object
  - Plane :: A plane object
  - Light :: A light
  - Camera :: A camera
  - Scene :: An object storing an entire scene
* Math
  The =math.hpp= and =math.cpp= files define the mathematical objects used in the ray tracing, and some functions related to these objects. The objects are Vec3D, which represents a three dimensional vector and Ray, which represents a three dimensional ray. Common operations on and between these items are defined, as well as 3D vector rotation.
* Color
  The =color.hpp= and =color.cpp= files define the Color class, which represents a RGB color. Addition and scalar multiplication operations are also defined for this Color class.
* Image Write
  The =image_write.hpp= and =image_write.cpp= files contain functions that handle normalizing the colors of a generated image (auto-exposure), and writing a normalized image out to a specified file.
* JsonFormatError
  The =json_format_error.hpp= file defines a special error class representing errors in how a provided JSON file is formatted. This includes files that are valid JSON, but can not be meaningfully parsed into a scene.
* Tracer
  The =tracer.hpp= and =tracer.cpp= files define the function that computes the actual ray tracing. The function takes a scene and pixel coordinate and calculates the (unscaled) color for that pixel.
* VTRay
  The =vtray.cpp= file contains the main function, and puts all the above items together. It handles parsing command line arguments, and organizing the multithreading of the tracing. The multithreading is done by having each thread take one pixel at a time to calculate the color of. Once the color of that pixel is calculated it takes the next pixel, until all pixels have been calculated. The pixels are ordered by row and then by column, and are stored in an array.
