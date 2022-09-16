# Assignment 1 Justification

If you think any of the questions within the sections are not applicable, please write "N/A".

## Section 1: It runs

### How have you fulfilled this requirement?
I have tested that the application builds with no errors and runs under the assignment provided by the course.

### When your application is run, what should the tutor look for or do to see that you have fulfilled this requirement? 
When you build and run the application it opens an OpenGL window with the moving objects that have been created.

### Where in the code should tutors look to see that you've fulfilled this requirement?
There are the relevant functions to initialise GLFW and create a GLFW window (from chicken3421), creating the shapes and shaders, and the rendering the program in the main function.


## Section 2: Visibility & Shaders

### How have you fulfilled this requirement?
The vertex and fragment shaders work fine and build without causing errors.

### When your application is run, what should the tutor look for or do to see that you have fulfilled this requirement?
The application window clearly shows an image of rotating circles all of different textures. I also used a texture for the background of the window.

### Where in the code should tutors look to see that you've fulfilled this requirement?
Inside res/shaders there are the vert.glsl and frag.glsl for the vertex shader and fragment shader respectively. 
I used uniforms for the texture and the transformation, uniform sampler2D u_tex inside the fragment shader and uniform mat4 u_transform inside the vertex shader.


## Section 3: Creation and Use of 2D Shapes

### How have you fulfilled this requirement?
This application completes 3a and 3b as I've created multiple 2D shapes of 7 circles clearly seen in the application window.
It completes 3c as the circles rotate alternatingly clockwise and counter-clockwise while the application is running.
It also completes 3d as the shapes were drawn and animated in a render loop seen in the main function.

### When your application is run, what should the tutor look for or do to see that you have fulfilled this requirement?
There are 6 of various sizes that rotate on running the application.
The 7th centre circle can also be rotated when you hold the R key and returns to its original position when you click your mouse.

### Where in the code should tutors look to see that you've fulfilled this requirement?
There is a struct circle_t and the functions make_circle and draw_circle to create the circles. 
For the transformations, I set the pointer to the centre circle then use the functions on_key_press and on_mouse_click.


## Section 4: Use of Textures

### How have you fulfilled this requirement?
This application completes 4a and 4b as it is able to load multiple textures and apply them to different objects.
It also completes 4c as the centre circle changes its texture, alternating between two images every second.
Evidence of this visible in the application window.

### When your application is run, what should the tutor look for or do to see that you have fulfilled this requirement?
The background of the window is a texture of a print and each circle has its own texture of a gradient color. The centre circle switches textures every second of differenc colored eyes.

### Where in the code should tutors look to see that you've fulfilled this requirement?
There are the functions load_image, delete_image, make_texture, and delete_texture to load images and create textures from them. I used a uniform for the textures and transformations in the shaders as explained in Section 2. 
I set the transform and passed the texture to the draw_circle function when creating a circle.
As for the background, I used a framebuffer.


### Section 5: Subjective Mark

### How have you fulfilled this requirement?
For this assignment I intended to create a psychedelic-inspired animation. The use of colours that blend together with the rotating circles that mached the background helped to create an interesting composition that draws in the viewer and is pleasing to the eye. The image of the eye also adds a striking point to the image.

All the images were taken from pinterest. Credits:
Background image was an illustrated cover for Leaving Records Spotify playlists (2018) http://www.nicoleginelli.com/leaving-records/.
The eye is a painting by Emilio Villalba from https://emiliovillalbaart.com/.

### When your application is run, what should the tutor look for or do to see that you have fulfilled this requirement?
N/A

### Where in the code should tutors look to see that you've fulfilled this requirement?
N/A

From a technical perspective this application was not as advanced as what I've seen my peers made from Discord. Although it is quite straightforward, I hoped that the image was able to invoke an artistic feel and I definitely enjoyed creating it :)


## Use of External Libraries
My application was not relatively complicated and I mainly followed the tutorials to help create this assignment.

### What does the library do?
N/A

### Why did you decide to use it?
N/A